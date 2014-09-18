import ast as AST
import jit
import collections
import copy
import math
import sys
from contextlib import contextmanager
import dis, traceback
import ctypes

from .utils import *

def extract_context_name():
    stack = traceback.extract_stack()
    assert len(stack) >= 3   # module, function_compiler, extract_context_name
    # Might also have a level of module import or a class definition.
  # if len(stack) > 5:
  #     for i, s in enumerate(stack):
  #         print(i, s)
  # else:
  #     assert len(stack) <= 5
    if len(stack) == 3:
        return None
    else:
        return stack[-3][2]  # the name of the class

# A unique sentinel object. We use this to devirtualize method calls within
# the same class (we cannot use the class name to declare variables in its
# methods because the name is not defined yet!).
def this_class():
    stack = traceback.extract_stack()
    class_name = stack[-2][2]
    return collections.namedtuple(class_name, [])  # easy way to make new class

def bc_compile(*, return_type=None, argument_types=[], variables={},
               global_vars={}, fixed_buffers=set(), intrinsics=set(),
               dump_code=True, verbose=False):
    if return_type is None:
        return_type = object
    global_vars = collections.defaultdict(object, global_vars)
    def function_compiler(function):
      # return function
        if dump_code:
            try:
                print(inspect.getsource(function))
            except:
                print('source not available, sorry')
            dis.show_code(function)
            dis.dis(function)
            print('')
            sys.stdout.flush()
        module = function.__module__
        name = function.__name__
        is_method = False
        context = extract_context_name()
        if context is not None and context != '<module>':
            is_method = True
            name = context + '.' + name
        assert name not in function_table[module]
        # Make an entry for this function in the function table. We don't
        # have the compiled code yet, but during compilation we already want
        # this name to be known; there might be a recursive call.
        function_table[module][name] = None
        machine_code = bc_compiler(function, return_type, argument_types,
                                   variables, global_vars, fixed_buffers,
                                   intrinsics, dump_code, verbose)
        setattr(machine_code, '__original_function__', function)
        function_table[module][name] = machine_code
        if dump_code or verbose:
            sys.stdout.flush()

        call_wrapper = compile_wrapper(name, machine_code,
                                       return_type, argument_types,
                                       is_method)
        set_wrapper(machine_code, call_wrapper)

        return machine_code

    return function_compiler

def compile_wrapper(name, machine_code, return_type, argument_types,
                    is_method):
    @builder(return_type=object,
             argument_types=[object, object, object],
             dump_code=False)
    def make_wrapper(func):
        # three arguments: function object, args (tuple), keyword arguments
        # (ignore, must be NULL)
        # First, grab the arguments and decode accoring to the signature we
        # were given.
        arg_types = argument_types
        unboxed_arguments = []
        if is_method:
            self_arg = get_self_arg(func, func.get_param(0))
            if self_arg:
                unboxed_arguments.append(self_arg)
            # Now we have accounted for the first argument type, so in the
            # loop below, only look at the remaining ones.
            arg_types = arg_types[1:]
        if arg_types:
            args = func.get_param(1)
            args = buffer_base_pointer(func, args, jit.Type.tuple_t)
            for i, t in enumerate(arg_types):
                arg = func.insn_load_elem(args, func.new_constant(i),
                                          jit.Type.void_ptr)
                if is_jit_number_type(t):
                    arg = func.unbox_value(arg, t, decref=False)
                # TODO: Otherwise, incref the arg? That's what the C++
                # wrapper does, though I don't know why it does it.
                unboxed_arguments.append(arg)
        # Make sure all arguments are accounted for.
        assert len(unboxed_arguments) == len(arg_types)+(1 if is_method else 0)
        # Call the wrapped code.
        retval = func.insn_call(name, machine_code,
                                machine_code.create_signature(),
                                unboxed_arguments)
        # Box the return value if needed.
        if is_jit_number_type(return_type):
            retval = func.box_value(retval)
        # Or if the function returns no value, return None.
        elif return_type == jit.Type.void:
            retval = func.new_constant(obj_ptr(None), jit.Type.void_ptr)
        # TODO: Clean up reference counts?
        func.insn_return(retval)
    return make_wrapper

class StackEntry:
    def __init__(self, name=None, value=None, boxed_value=None, type=None,
                 jit_type=None, abstract=None, builtin=None, api_builtin=None,
                 intrinsic=None, self_ptr=None, constant=None,
                 constant_idx=None, freshly_allocated=None, refcount=None):
        self.name = name
        self.value = value
        self.boxed_value = boxed_value
        self.type = type
        self.jit_type = jit_type
        self.abstract = abstract
        self.builtin = builtin
        self.api_builtin = api_builtin
        self.intrinsic = intrinsic
        self.self_ptr = self_ptr
        self.constant = constant
        self.constant_idx = constant_idx
        self.freshly_allocated = freshly_allocated
        self.refcount = refcount

    def __str__(self):
        members = {k: v for k, v in self.__dict__.items() if v is not None}
        return 'StackEntry' + str(members)

Call = collections.namedtuple('Call', ['function', 'args'])

class Loop:
    def __init__(self, label, iterator=None):
        self.label = label
        self.iterator = iterator
        self.iter_var = None

global_str_ptrs = set()

def GLOBAL_INCREF(func, x):
    if x.boxed_value is None and x.constant is None:
        return
    if x.boxed_value is None and x.constant is not None:
        x.boxed_value = x.constant
    assert x.refcount is not None
    if reference_counting:
        if x.boxed_value is not None:
            Py_IncRef(func, x.boxed_value)
    else:
        # No unnecessary reference counting. Is this necessary?
        if x.boxed_value is not None and x.refcount < 1:
            Py_IncRef(func, x.boxed_value)
        elif x.boxed_value is not None and rc_placeholders:
            # placeholder for omitted incref
            jitlib_nop_1(func, x.boxed_value)
    x.refcount += 1

def GLOBAL_DECREF(func, x):
    if x.boxed_value is None and x.constant is None:
        return
    assert x.refcount is not None
    x.refcount -= 1
    if reference_counting:
        if x.boxed_value is not None:
            Py_DecRef(func, x.boxed_value)
    else:
        # No unnecessary reference counting. Is this necessary?
        if x.boxed_value is not None and x.refcount < 1:
            Py_DecRef(func, x.boxed_value)
        elif x.boxed_value is not None and rc_placeholders:
            # placeholder for omitted decref
            jitlib_nop_1(func, x.boxed_value)

def bc_compiler(function, return_type, argument_types,
                variables, global_vars, fixed_buffers, intrinsics,
                dump_code, verbose):
    if fully_dynamic_typing:
        def transform_to_object(t):
            return object if isinstance(t, jit.Type) else t

        return_type = transform_to_object(return_type)
        for i, arg in enumerate(argument_types):
            argument_types[i] = transform_to_object(argument_types[i])
        for name in variables:
            variables[name] = transform_to_object(variables[name])
        for name in global_vars:
            global_vars[name] = transform_to_object(global_vars[name])
        intrinsics = {}
        if verbose:
            print('argument_types:', argument_types)
            print('variables:', variables)
            print('global_vars:', global_vars)
    elif boxed_numbers:
        def transform_to_number(t):
            if is_jit_number_type(t):
                return float if t == jit.Type.float64 else int
            else:
                return t
        
        return_type = transform_to_number(return_type)
        for i, arg in enumerate(argument_types):
            argument_types[i] = transform_to_number(argument_types[i])
        for name in variables:
            variables[name] = transform_to_number(variables[name])
        for name in global_vars:
            global_vars[name] = transform_to_number(global_vars[name])
        intrinsics = {}
        if verbose:
            print('argument_types:', argument_types)
            print('variables:', variables)
            print('global_vars:', global_vars)

    def enumerate_code(code):
        i = 0
        N = len(code)
        while i < N:
            bc = code[i]
            have_arg = (bc > dis.HAVE_ARGUMENT)
            arg = (operand_value(code, i) if have_arg else None)
            yield bc, arg, i
            i += (3 if have_arg else 1)

    def operand_value(code, offset):
        return code[offset+1] | code[offset+2] << 8

    def dump_stack():
        for i, e in enumerate(stack):
            print(i, e)

    def maybe_unbox_constant(func, boxed):
        type, value = None, None
        if isinstance(boxed, int) and boxed in range(-2**30, 2**30):
            type = jit.Type.int
            value = func.new_constant(boxed, type)
        elif isinstance(boxed, float):
            type = jit.Type.float64
            value = func.new_constant(boxed, type)
        return type, value

    def get_base_pointer(func, name, obj, type):
        if not optimize_fixed_buffers or name not in fixed_buffers:
            return buffer_base_pointer(func, obj, type)
        base_pointer_name = 'base pointer for ' + name
        symbol = symbols[base_pointer_name]
        if name in arguments:
            # We must have computed the base pointer before.
            base = symbol.location
        elif base_pointer_name in symbols:
            base = symbol.location
            if base is None:
                base = func.new_value(jit.Type.void_ptr)
                buffer = buffer_base_pointer(func, obj, type)
                func.store(base, buffer)
                symbol.location = base
        return base

    scratch_space = None

    def evaluate_abstract_call(func, a):
        target = func.new_constant(obj_ptr(a.abstract.function))
        abstract_args = [func.box_stack_entry(arg) for arg in a.abstract.args]
        a.boxed_value = func.call_py_object(target, abstract_args,
                                            kwargs=None,
                                            scratch_space=scratch_space,
                                            decref_args=False)
        a.abstract = None
        a.freshly_allocated = True
        return a.boxed_value

    symbols = dict()
    # FIXME: arguments
    for i, (name, type) in enumerate(zip(inspect.getfullargspec(function).args,
                                         argument_types)):
        symbols[name] = Argument(type, i)
    for name, type in variables.items():
        symbols[name] = Local(type, location=None)

    code = function.__code__.co_code
    names = function.__code__.co_names
    varnames = function.__code__.co_varnames
    consts = function.__code__.co_consts
    freevars = function.__code__.co_cellvars
    stack = []
    loop_stack = []
    for_loop_heads = {}
    for_loop_increments = {}
    phi_locations = dict()
    func = None

  # my_incref = pylib.Py_IncRef
  # my_incref.argtypes = (ctypes.py_object,)
  # my_incref(consts)
  # for c in consts:
  #     my_incref(c)

    # Python interpreter basic operations
    def INCREF(x):
        GLOBAL_INCREF(func, x)

    def DECREF(x):
        GLOBAL_DECREF(func, x)

    def PUSH(x):
        stack.append(x)

    def POP():
        return stack.pop()

    def TOP():
        return stack[-1]

    def SECOND():
        return stack[-2]

    def SET_TOP(x):
        stack[-1] = x

    def JUMPTO(arg):
        func.insn_branch(labels[arg])

    def JUMPBY(offset, arg):
        target_offset = offset + arg + 3
        func.insn_branch(labels[target_offset])

    def RAISE_VARARGS(func, arg, offset):
        # Just return NULL.
        zero = func.new_constant(0, jit.Type.void_ptr)
        func.insn_return(zero)

    # Bytecode implementations
    def LOAD_FAST(func, arg, offset):
        name = varnames[arg]
        type = symbols[name].type
        x = StackEntry(name=name, type=type)
        if is_jit_number_type(type):
            x.value = symbols[name].location
        else:
            x.boxed_value = symbols[name].location
            x.refcount = 1
          # obj_printer(func, x.boxed_value)
        # Note: This incref will lead the stack entry to record a refcount
        # of 2. That's OK: There is a reference from the stack, and a
        # reference from the place where local variables are stored.
        INCREF(x)
        PUSH(x)
      # if name == 'self':
      #   # insert_marker_call(func)
      #     marker_int(func, func.new_constant(offset))
      #     obj_printer(func, x.boxed_value)

    def STORE_FAST(func, arg, offset):
        v = POP()
        if v.abstract:
            return
        name = varnames[arg]
        if is_jit_number_type(symbols[name].type):
            if v.value is None:
                v.value = func.unbox_value(v.boxed_value, symbols[name].type,
                                           decref=False)
                DECREF(v)
            value = v.value
        else:
            value = func.box_stack_entry(v)
            if not reference_counting:
                # We (probably?) did not count the reference for the
                # object to be assigned yet. Do that now.
              # if v.constant is None and not v.freshly_allocated:
                if not v.freshly_allocated:
                  # obj_printer(func, value)
                    Py_IncRef(func, value)
            # The reference in this location will be lost when we overwrite
            # it, so decrement the target's reference count.
            Py_DecRef(func, symbols[name].location)
            # Compute the buffer base pointer for this value if appropriate.
            if optimize_fixed_buffers and name in fixed_buffers:
                get_base_pointer(func, name, value, symbols[name].type)
        func.store(symbols[name].location, value)
      # if not is_jit_number_type(symbols[name].type):
      #     obj_printer(func, value)

    def STORE_DEREF(func, arg, offset):
        w = POP()
        if verbose:
            print('freevars:', freevars, 'arg:', arg)
            print('symbols:', symbols)
        x = symbols[freevars[arg]].location
        if w.boxed_value is None and w.abstract is not None:
            w.boxed_value = evaluate_abstract_call(func, w)
        assert w.boxed_value is not None
        PyCell_Set(func, x, w.boxed_value)
        DECREF(w)

    def LOAD_DEREF(func, arg, offset):
        x = symbols[freevars[arg]].location
        w = PyCell_Get(func, x)
        null = func.make_null_ptr()
        with func.branch(w != null) as (false, end):
            PUSH(StackEntry(name=freevars[arg], boxed_value=w,
                            type=symbols[freevars[arg]].type,
                            freshly_allocated=True,
                            refcount=1))
            func.insn_branch(end)
        # else: error
            func.insn_label(false)
            func.insn_return(null)

    def LOAD_CLOSURE(func, arg, offset):
        x = symbols[freevars[arg]].location
        Py_IncRef(func, x)
        PUSH(StackEntry(name=freevars[arg], boxed_value=x, refcount=1))

    def LOAD_CONST(func, arg, offset):
        boxed = consts[arg]
      # if isinstance(boxed, bool):
      #     marker_int(func, func.new_constant(offset))
      #     obj_printer(func, func.new_constant(obj_ptr(boxed),
      #                                         jit.Type.void_ptr))
        name = str(boxed)
        type, value = maybe_unbox_constant(func, boxed)
        boxed = func.new_constant(obj_ptr(boxed), jit.Type.void_ptr)
        # Here's a thing: If the constant we load is the None object, then
        # this constant's boxed_value will be None. But that's OK, sort of.
        # We just need to be aware that None is a valid boxed value.
        if value is not None or boxed is not None:
            x = StackEntry(value=value, name=name, type=type, refcount=2,
                           constant=boxed, constant_idx=arg)
        else:
            x = StackEntry(boxed_value=boxed, name=name, type=type, refcount=1)
        INCREF(x)
        PUSH(x)

    def binary_op(func, op):
        w = POP()
        v = POP()
        w_is_int = (w.type is int or (w.constant and w.type == jit.Type.int))
        w_is_float = (w.type is float or
                      (w.constant and w.type == jit.Type.float64))
        v_is_int = (v.type is int or (v.constant and v.type == jit.Type.int))
        v_is_float = (v.type is float or
                      (v.constant and v.type == jit.Type.float64))
        unboxed = is_jit_number(v.value) and is_jit_number(w.value)
        if unboxed:
            left, right = v.value, w.value
            common_type = None
            if op == AST.Div:
                # This is always floating-point division, even on integer
                # arguments.
                left = func.insn_convert(left, jit.Type.float64)
                right = func.insn_convert(right, jit.Type.float64)
                common_type = jit.Type.float64
            elif (left.type == jit.Type.float64 or
                  right.type == jit.Type.float64):
                common_type = jit.Type.float64
            else:
                if left.type == right.type:
                    common_type = left.type
                else:
                    # FIXME: ugly hack
                    common_type = jit.Type.int
            if verbose:
                print('unboxed', op.__name__)
            x = StackEntry(value=binops[op].unboxed_op(left, right),
                           type=common_type, refcount=1)
        elif (static_number_typing and
              (v_is_int or v_is_float) and
              (w_is_int or w_is_float)):
            if v_is_float or w_is_float:
                common_type = float
            else:
                common_type = int
            if verbose:
                print('boxed', common_type.__name__, op.__name__)
            l = func.box_stack_entry(v)
            r = func.box_stack_entry(w)
            boxed_binops = int_binops if common_type is int else float_binops
            if op == AST.Div:
                common_type = float
          # if verbose:
          #     obj_printer(func, func.new_constant(obj_ptr(op.__name__)))
          #     obj_printer(func, l)
          #     obj_printer(func, r)
            result = func.insn_call_native(op.__name__,
                                           boxed_binops[op],
                                           jit_binaryfunc.sig,
                                           [l, r])
          # if verbose:
          #     obj_printer(func, result)
            x = StackEntry(boxed_value=result, freshly_allocated=True,
                           refcount=1, type=common_type)
            DECREF(v)
            DECREF(w)
        else:
            if verbose:
                print('boxed', op.__name__)
            l = func.box_stack_entry(v)
            r = func.box_stack_entry(w)
            common_type = None
            name = None
            # Is this operator overloaded?
            functions = function_table[function.__module__]
            if (op in overloads and
                hasattr(v.type, '__name__') and
                v.type.__name__ + '.' + overloads[op] in functions):
                op_name = v.type.__name__ + '.' + overloads[op]
                op_target = functions[op_name]
                result = func.insn_call(overloads[op],
                                        op_target,
                                        op_target.create_signature(),
                                        [l, r])
                name = '<anonymous ' + overloads[op] + '>'
                if name in variables:
                    common_type = variables[name]
                else:
                    common_type = result.type
            else:
                result = binops[op].boxed_op(func, l, r)
            if common_type is None and v.type == w.type:
                common_type = v.type
                if common_type == int and op == AST.Div:
                    common_type = float
            elif (v.type == jit.Type.int and w.type == int or
                  v.type == int and w.type == jit.Type.int):
                common_type = int
            elif ((v.type is float or w.type is float) and
                  is_number_type(v.type) and is_number_type(w.type)):
                common_type = float
            else:
                common_type = object
            x = StackEntry(boxed_value=result, freshly_allocated=True,
                           refcount=1, type=common_type, name=name)
          # Py_DecRef(func, l)
          # Py_DecRef(func, r)
            DECREF(v)
            DECREF(w)
        PUSH(x)

    def inplace_op(func, op):
        binary_op(func, op)

    def BINARY_ADD(func, arg, offset):
        binary_op(func, AST.Add)

    def BINARY_SUBTRACT(func, arg, offset):
        binary_op(func, AST.Sub)

    def BINARY_MULTIPLY(func, arg, offset):
        binary_op(func, AST.Mult)

    def BINARY_LSHIFT(func, arg, offset):
        binary_op(func, AST.LShift)

    def BINARY_RSHIFT(func, arg, offset):
        binary_op(func, AST.RShift)

    def BINARY_TRUE_DIVIDE(func, arg, offset):
        binary_op(func, AST.Div)

    def BINARY_FLOOR_DIVIDE(func, arg, offset):
        binary_op(func, AST.FloorDiv)

    def BINARY_MODULO(func, arg, offset):
        binary_op(func, AST.Mod)

    def BINARY_AND(func, arg, offset):
        binary_op(func, AST.BitAnd)

    def BINARY_XOR(func, arg, offset):
        binary_op(func, AST.BitXor)

    def BINARY_POWER(func, arg, offset):
        w = POP()
        v = TOP()
        # shoddy minimal implementation
        if w.value is not None and v.value is not None:
            assert w.type == jit.Type.float64 and v.type == jit.Type.float64
            x = func.insn_pow(v.value, w.value)
            x = StackEntry(type=w.type, value=x, refcount=1)
        else:
            func.box_stack_entry(w)
            func.box_stack_entry(v)
            assert w.boxed_value is not None and v.boxed_value is not None
            none = func.new_constant(obj_ptr(None), jit.Type.void_ptr)
            x = PyNumber_Power(func, v.boxed_value, w.boxed_value,
                               none)
            x = StackEntry(type=w.type, boxed_value=x, refcount=1,
                           freshly_allocated=True)
        DECREF(v)
        DECREF(w)
        SET_TOP(x)

    def INPLACE_ADD(func, arg, offset):
        inplace_op(func, AST.Add)

    def INPLACE_SUBTRACT(func, arg, offset):
        inplace_op(func, AST.Sub) 

    def INPLACE_MULTIPLY(func, arg, offset):
        inplace_op(func, AST.Mult)

    def INPLACE_LSHIFT(func, arg, offset):
        inplace_op(func, AST.LShift)

    def INPLACE_RSHIFT(func, arg, offset):
        inplace_op(func, AST.RShift)

    def INPLACE_TRUE_DIVIDE(func, arg, offset):
        inplace_op(func, AST.Div)

    def INPLACE_XOR(func, arg, offset):
        inplace_op(func, AST.BitXor)

    def UNARY_NOT(func, arg, offset):
        v = TOP()
        x = object_is_true(func, func.box_stack_entry(v), v.type)
        zero = func.new_constant(0, jit.Type.int)
        result = func.new_value(jit.Type.void_ptr)
        with func.branch(x == zero) as (false_label, end):
            t = func.new_constant(obj_ptr(True), jit.Type.void_ptr)
            Py_IncRef(func, t)
            func.store(result, t)
            func.insn_branch(end)
        # else: (no error checking)
            func.insn_label(false_label)
            f = func.new_constant(obj_ptr(False), jit.Type.void_ptr)
            Py_IncRef(func, f)
            func.store(result, f)
        SET_TOP(StackEntry(boxed_value=result, refcount=1))

    def UNARY_NEGATIVE(func, arg, offset):
        v = TOP()
        if is_jit_number_type(v.type):
            # unboxed
            x = -v.value
            x = StackEntry(value=x, type=v.type, refcount=1)
        else:
          # marker_int(func, func.new_constant(offset))
          # obj_printer(func, v.boxed_value)
            x = PyNumber_Negative(func, func.box_stack_entry(v))
            x = StackEntry(boxed_value=x, type=v.type, refcount=1)
          # obj_printer(func, x.boxed_value)
        DECREF(v)
        SET_TOP(x)

    def RETURN_VALUE(func, arg, offset):
        value = POP()
        
        # First, find out what value (if any) to return.
        generate_return_instruction = True
        if (not is_jit_number_type(func.return_type) and
            value.boxed_value is None and
            is_jit_number(value.value)):
            value.boxed_value = func.box_stack_entry(value)
        if is_jit_number_type(func.return_type) and value.value is not None:
            return_value = value.value
        elif (is_jit_number_type(func.return_type) and
              value.constant is not None and
              value.name == 'None'):
            # This function is meant to return a machine number. This is
            # probably the automagically generated "return None". Returning
            # the None pointer won't help very much. So try not generating
            # any code at all, since presumably there are proper return
            # statements somewhere else in the code.
            generate_return_instruction = False
        else:
            # The boxed value is *always* a valid Python value, though it
            # might be the None value.
            if value.boxed_value is None and value.constant is not None:
                # Incref the return value here if it's a constant?
                return_value = value.constant
              # obj_printer(func, return_value)
                Py_IncRef(func, return_value)
            elif not isinstance(value.boxed_value, jit.Value):
                print('fishy return?', value)
                raise CompileError('fishy return!')
                value.boxed_value = obj_ptr(func, value.boxed_value)
                return_value = func.new_constant(value.boxed_value)
            elif is_jit_number_type(return_type):
                return_value = func.unbox_value(value.boxed_value, return_type)
            else:
                return_value = value.boxed_value
                if value.name not in symbols and not value.freshly_allocated:
                    Py_IncRef(func, return_value)
                elif not reference_counting and value.name in arguments:
                    Py_IncRef(func, return_value)
              # if value.type == bool:
              #     marker_int(func, func.new_constant(offset))
              #     obj_printer(func, return_value)

        # Clean up local variables.
        for v in symbols:
            if (v.isidentifier() and
                ((v not in arguments and
                  v not in consts) or v in freevars)):
              # print('local symbol', v)
                name = v
                v = symbols[v].location
                if not is_jit_number(v):
                    if not reference_counting and name == value.name:
                        if verbose:
                            print('return', name, 'do not decref')
                    else:
                        if verbose:
                            print('decref', name)
                        Py_DecRef(func, v)
            elif v in consts:
                if verbose:
                    print('constant:', v)
                  # obj_printer(func, v)
            else:
                if verbose:
                    print('other symbol', v)
                pass
        # Also clean up any loops we might be returning out of.
        for loop in reversed(loop_stack):
            if verbose:
                print('loop entry', loop)
            if loop.iterator is not None:
                Py_DecRef(func, loop.iterator)
            if loop.iter_var is not None:
                # FIXME: only if not reference_counting?
                Py_DecRef(func, loop.iter_var)
        # Now finally release arguments as well.
        for name in arguments:
            if not is_jit_number(symbols[name].location):
                Py_DecRef(func, symbols[name].location)

        # and now return
        if generate_return_instruction:
            func.insn_return(return_value)

    def COMPARE_OP(func, arg, offset):
        w = POP()
        v = POP()
        unboxed = is_jit_number(v.value) and is_jit_number(w.value)
        if unboxed:
            cmp = comparisons[arg].unboxed_op
            result = cmp(v.value, w.value)
          # stack.append(StackEntry(value=cmp(left.value, right.value)))
          # return
        else:
            l = func.box_stack_entry(v)
            r = func.box_stack_entry(w)
            if 0 <= arg <= 5:
                op = func.new_constant(arg)
                result = PyObject_RichCompareBool(func, l, r, op)
            elif arg == 6:
                # in
                result = func.insn_to_bool(PySequence_Contains(func, r, l))
            elif arg == 7:
                # not in
                result = func.insn_to_not_bool(PySequence_Contains(func, r, l))
            elif arg == 8:
                # is
                result = (l == r)
            elif arg == 9:
                # is not
                result = (l != r)
            else:
                raise CompileError('bad comparison: ' + str(arg))
        x = StackEntry(value=result, type=jit.Type.int, refcount=1)
      # if v.value is None or not is_jit_number_type(v.type):
        if not v.builtin:
            DECREF(v)
      # if w.value is None or not is_jit_number_type(w.type):
        if not w.builtin:
            DECREF(w)
        PUSH(x)

    def POP_JUMP_IF_x(func, arg, offset, x):
        assert x == False or x == True
        w = POP()
        condition = w.value
        if condition is not None:
            condition = (func.insn_to_not_bool(condition) if x == False
                         else condition)
        else:
            err = object_is_true(func, w.boxed_value, w.type)
          # if not reference_counting:
          #     # Needed for go benchmark?
          #     DECREF(w)
            zero = func.new_constant(0, jit.Type.int)
            condition = (err == zero if x == False else err != zero)
        is_for_backedge = (arg < offset and arg in for_loop_heads)
        if is_for_backedge:
            # We must increment the loop counter if the branch back up is
            # taken, i.e., the condition is true.
            with func.branch(condition) as (false, end):
                counter_var = for_loop_heads[arg].location
                func.store(counter_var, counter_var + for_loop_increments[arg])
                func.insn_branch(labels[arg])
            # else:
                func.insn_label(false)
                # Nothing to do, fall through.
        else:
            func.insn_branch_if(condition, labels[arg])

    def POP_JUMP_IF_FALSE(func, arg, offset):
        POP_JUMP_IF_x(func, arg, offset, False)

    def POP_JUMP_IF_TRUE(func, arg, offset):
        POP_JUMP_IF_x(func, arg, offset, True)

    def JUMP_IF_x_OR_POP(func, arg, offset, x):
        w = POP()
        # First, check if the condition is an unboxed int. That makes things
        # a lot simpler.
        if (w.value is not None and
            w.boxed_value is None and
            w.type == jit.Type.int):
            # OK, checking the condition is simple.
            zero = func.new_constant(0, jit.Type.int)
            cond = (w.value != zero if x else w.value == zero)
            true = func.new_constant(obj_ptr(True), jit.Type.void_ptr)
            false = func.new_constant(obj_ptr(False), jit.Type.void_ptr)
            phi_value = (true if x else false)
            with func.branch(cond) as (false_label, end_label):
                func.store(phi_locations[arg], phi_value)
                JUMPTO(arg)
            # else:
                func.insn_label(false_label)
                # POP()
            return
        # Standard boxed implementation.
        true = func.new_constant(obj_ptr(True), jit.Type.void_ptr)
        false = func.new_constant(obj_ptr(False), jit.Type.void_ptr)
        cond = func.new_constant(obj_ptr(x), jit.Type.void_ptr)
        opp = (true if x == False else false)
        with func.branch(func.box_stack_entry(w) == opp) as (false_label, end):
          # POP()
            DECREF(w)
            func.insn_branch(end)
        # else:
            func.insn_label(false_label)
            with func.branch(w.boxed_value == cond) as (false_label, end):
                func.store(phi_locations[arg], w.boxed_value)
                JUMPTO(arg)
            # else:
                func.insn_label(false_label)
                zero = func.new_constant(0, jit.Type.int)
                err = object_is_true(func, w.boxed_value, w.type)
                condition = (err > zero if x == False else err == zero)
                with func.branch(condition) as (false_label, end):
                  # POP()
                    if reference_counting:
                        DECREF(w)
                    func.insn_branch(end)
                # else: (no error checking)
                    func.insn_label(false_label)
                    func.store(phi_locations[arg], w.boxed_value)
                    JUMPTO(arg)

    def JUMP_IF_FALSE_OR_POP(func, arg, offset):
        JUMP_IF_x_OR_POP(func, arg, offset, False)

    def JUMP_IF_TRUE_OR_POP(func, arg, offset):
        JUMP_IF_x_OR_POP(func, arg, offset, True)

    def JUMP_ABSOLUTE(func, arg, offset):
      # print(arg, offset, for_loop_heads, loop_stack)
        if arg < offset and arg in for_loop_heads:
            # This is a for loop backedge. Increment the loop counter.
            counter_var = for_loop_heads[arg].location
            func.store(counter_var, counter_var + for_loop_increments[arg])
        JUMPTO(arg)

    def JUMP_FORWARD(func, arg, offset):
        # The top-of-stack here must be popped and stored in the appropriate
        # phi variable.
        if stack:
            target = offset + arg + 3
            x = POP()
            assert x.boxed_value is not None or x.abstract is not None
            if x.boxed_value is None and x.abstract is not None:
                evaluate_abstract_call(func, x)
            assert x.boxed_value is not None
            func.store(phi_locations[target], x.boxed_value)
        JUMPBY(offset, arg)

    def LOAD_GLOBAL(func, arg, offset):
        name = names[arg]
        api_builtins = {'divmod': PyNumber_Divmod,
                        'isinstance': PyObject_IsInstance,
                        'len': PyObject_Size}
        builtins = {'abs': abs, 'chr': chr, 'enumerate': enumerate,
                    'float': float, 'hash': hash, 'int': int,
                    'len': len, 'list': list, 'map': map, 'max': max,
                    'min': min, 'ord': ord, 'print': print,
                    'reversed': reversed, 'slice': slice, 'str': str,
                    'sum': sum, 'tuple': tuple, 'type': builtin_type,
                    'zip': zip}
        x = None
        # Special non-Pythonic handling if this is a function name we know
        # about.
        if name == function.__name__:
            x = StackEntry(name=name, value=func,
                           type=func.create_signature())
        elif name in function_table[function.__module__]:
            target = function_table[function.__module__][name]
            x = StackEntry(name=name, value=target,
                           type=target.create_signature())
        elif name == 'range' or name == 'xrange':  # xrange is fishy
            # We might have to call this function, or if it controls a for
            # loop, we might have to unbox everything entirely. We don't
            # know yet (TODO: an analysis to determine such things!), so for
            # now we push a symbolic entry onto the stack and handle it
            # later.
            x = StackEntry(abstract=range, name=name)
        # Intrinsics are checked before builtins because abs is both an
        # "intrinsic" (there is a libjit instruction for it) and a builtin.
        elif (not late_binding and
              name in intrinsics and
              name in known_intrinsics):
            x = StackEntry(intrinsic=known_intrinsics[name], name=name)
        elif name in api_builtins:
            x = StackEntry(api_builtin=api_builtins[name], name=name)
        elif name in builtins:
            x = StackEntry(builtin=builtins[name], name=name, refcount=1)
        elif name == 'math':
            # The math module, used in lookups of the form math.log.
            x = StackEntry(abstract=math, name=name, refcount=1)
        if x is None:
            # Do the general lookup.
            globals_obj = func.new_constant(obj_ptr(function.__globals__),
                                            jit.Type.void_ptr)
            builtins_obj = func.new_constant(obj_ptr(__builtins__),
                                             jit.Type.void_ptr)
            id = func.new_constant(obj_ptr(names[arg]), jit.Type.void_ptr)
            result = func.new_value(jit.Type.void_ptr)
            g = PyDict_GetItem(func, globals_obj, id)
            with func.branch(g == func.make_null_ptr()) as (false, end):
                g2 = PyDict_GetItem(func, builtins_obj, id)
                with func.branch(g2 == func.make_null_ptr()) as (f2, e2):
                    obj_printer(func, globals_obj)
                    obj_printer(func, builtins_obj)
                    obj_printer(func, id)
                    func.insn_return(g2)
                # else:  (found)
                    func.insn_label(f2)
                    func.store(result, g2)
                func.insn_branch(end)
            # else:  (found)
                func.insn_label(false)
                func.store(result, g)
            x = StackEntry(name=name, boxed_value=result,
                           type=global_vars[name], refcount=0)
          # marker_int(func, func.new_constant(offset))
          # obj_printer(func, x.boxed_value)
            INCREF(x)
        elif late_binding:
            globals_obj = func.new_constant(obj_ptr(function.__globals__),
                                            jit.Type.void_ptr)
            builtins_obj = func.new_constant(obj_ptr(__builtins__),
                                             jit.Type.void_ptr)
            id = func.new_constant(obj_ptr(names[arg]), jit.Type.void_ptr)
            g = PyDict_GetItem(func, globals_obj, id)
            with func.branch(g == func.make_null_ptr()) as (false, end):
                g2 = PyDict_GetItem(func, builtins_obj, id)
                with func.branch(g2 == func.make_null_ptr()) as (f2, e2):
                    func.insn_return(g2)
                # else:
                    func.insn_label(f2)
            # else:
                func.insn_label(false)
        PUSH(x)

    def STORE_GLOBAL(func, arg, offset):
        globals = func.new_constant(obj_ptr(function.__globals__),
                                    jit.Type.void_ptr)
        w = func.new_constant(obj_ptr(names[arg]), jit.Type.void_ptr)
        v = POP()
        PyDict_SetItem(func, globals, w, func.box_stack_entry(v))
        DECREF(v)

    def CALL_FUNCTION(func, arg, offset):
        args = []
        kwargs = dict()
        na = arg & 0xff
        nk = (arg >> 8) & 0xff
        n = na + 2 * nk
        for i in range(nk):
            val = stack.pop()
            key = stack.pop()
            assert key.name is not None
            assert isinstance(key.name, str)
            if verbose:
                print(key.name, ' => ', val)
            kwargs[key.name] = val
        stack_args = []
        for i in range(na):
            a = stack.pop()
            stack_args.append(a)
            if verbose:
                print(i, ' => ', a)
            if a.abstract:
                # This function has an abstract call, such as to range, as
                # its argument. We must evaluate it here.
                a.boxed_value = evaluate_abstract_call(func, a)
            args.append(a)
        args.reverse()
        tgt = stack.pop()
        if verbose:
            print(tgt)
        if nk > 0:
            assert (not tgt.abstract and
                    not tgt.builtin and
                    not tgt.api_builtin)
        if tgt.abstract is not None:
            stack.append(StackEntry(abstract=Call(tgt.abstract, args)))
        elif tgt.builtin == int and is_jit_number(args[0]):
            t = jit.Type.int
            stack.append(StackEntry(value=func.insn_convert(args[0].value, t),
                                    type=t))
        elif tgt.builtin == float and is_jit_number(args[0]):
            t = jit.Type.float64
            stack.append(StackEntry(value=func.insn_convert(args[0].value, t),
                                    type=t))
        elif tgt.builtin is not None:
            target = func.new_constant(obj_ptr(tgt.builtin))
            actual_args = []
            original_args = args
            for a in args:
                if a.boxed_value is None and isinstance(a.constant, jit.Value):
                    a.boxed_value = a.constant
                if isinstance(a.boxed_value, jit.Value):
                    a = a.boxed_value
                elif a.builtin is not None:
                    a.boxed_value = func.new_constant(obj_ptr(a.builtin),
                                                      jit.Type.void_ptr)
                    INCREF(a)
                    a = a.boxed_value
                  # Py_IncRef(func, a)
                else:
                    assert a.value is not None
                    a = a.value
                actual_args.append(a)
            if not reference_counting:
                decref_args = func.compute_decref_args(original_args)
            else:
                decref_args = True
            result = func.call_py_object(target, actual_args,
                                         kwargs=None,
                                         scratch_space=scratch_space,
                                         name=tgt.builtin.__name__,
                                         decref_args=decref_args)
            stack.append(StackEntry(boxed_value=result,
                                    type=jit.Type.void_ptr,
                                    name=tgt.builtin.__name__+'()',
                                    refcount=1,
                                    freshly_allocated=True))
        elif (tgt.api_builtin is not None and
              tgt.name == 'len' and
              len(args) == 1 and  # this cannot really fail
              (is_list(args[0]) or is_tuple(args[0]) or is_array(args[0]))):
            # Load length field directly.
            size_offset = func.new_constant(jit_ob_size_offset())
            obj_size = func.insn_load_elem(args[0].boxed_value,
                                           size_offset,
                                           jit.Type.int)
            # Don't forget to decref the argument!
            DECREF(args[0])
            stack.append(StackEntry(value=obj_size, type=jit.Type.int,
                                    refcount=1))
        elif tgt.api_builtin is not None:
            arguments = [func.box_stack_entry(a) for a in args]
            result = tgt.api_builtin(func, *arguments)
            for a in args:
                DECREF(a)
            return_type = tgt.api_builtin.sig.get_return()
            if is_jit_number_type(return_type):
                stack.append(StackEntry(value=result, type=return_type,
                                        refcount=1))
            else:
                stack.append(StackEntry(boxed_value=result, type=return_type,
                                        refcount=1))
        elif tgt.intrinsic is not None:
            assert len(args) == 1
            arg = args[0]
            if verbose:
                print(arg)
            if arg.value is None:
                assert arg.boxed_value is not None
                arg.value = func.unbox_value(arg.boxed_value, jit.Type.float64)
            arg = func.insn_convert(arg.value, jit.Type.float64)
            result = tgt.intrinsic(arg)
            stack.append(StackEntry(value=result, refcount=1))
        elif isinstance(tgt.boxed_value, jit.Value):
            # This is a call to some Python object.
            original_args = args
            decref_args = True
          # args = [a.boxed_value if isinstance(a.boxed_value, jit.Value)
          #         else a.value for a in args]
            args = []
            for a in original_args:
                if a.builtin is not None and a.boxed_value is None:
                    a.boxed_value = func.new_constant(obj_ptr(a.builtin),
                                                      jit.Type.void_ptr)
                args.append(func.box_stack_entry(a))
            if not reference_counting:
                decref_args = func.compute_decref_args(original_args)
            if verbose:
                print('kwargs:', kwargs)
                print('decref_args:', decref_args)
            result = func.call_py_object(tgt.boxed_value, args, kwargs,
                                         scratch_space=scratch_space,
                                         name=tgt.name,
                                         decref_args=decref_args)
          # obj_printer(func, tgt.boxed_value)
            if verbose:
                print('object tgt:', tgt)
            DECREF(tgt)
            stack.append(StackEntry(boxed_value=result, name=tgt.name + '()',
                                    refcount=1, freshly_allocated=True))
        else:
            name, target, signature = tgt.name, tgt.value, tgt.type
            increfed_self_ptr = None
            if isinstance(target, Call):
                # This is a boxed method. Take it apart.
                new_args = target.args[:]
                for i, a in enumerate(args):
                    # j is the argument's index in the function signature.
                    # Since it's a method, it has an extra implicit
                    # argument!
                    j = i + 1
                    if (a.value is not None and
                        a.value.type == signature.get_param(j)):
                        new_args.append(a.value)
                    elif (a.value is not None and
                          signature.get_param(j) == jit.Type.void_ptr):
                      # if a.boxed_value is None:
                      #     a.boxed_value = func.box_value(a.value)
                        func.box_stack_entry(a)
                        new_args.append(a.boxed_value)
                    elif (a.value is None and
                          is_jit_number_type(signature.get_param(j))):
                        assert a.boxed_value is not None
                        value = func.unbox_value(a.boxed_value,
                                                 signature.get_param(j))
                        new_args.append(value)
                    elif (a.value is None and
                          a.boxed_value is None and
                          a.constant is not None):
                        assert signature.get_param(j) == jit.Type.void_ptr
                        new_args.append(a.constant)
                    else:
                        value = func.unbox_value(a.boxed_value, a.type)
                        new_args.append(value)
                args = new_args
                target = target.function
            else:
                # The args are still stack entries. Take them out.
                for i, a in enumerate(args):
                    type = signature.get_param(i)
                    if is_jit_number_type(type):
                        arg = a.value or func.unbox_value(a.boxed_value, type)
                    else:
                        assert a.boxed_value is not None
                        arg = a.boxed_value
                    args[i] = arg
            if verbose:
                print(name, target, signature, args)
            if allocate_frames:
                tstate = PyThreadState_Get(func)
                co = func.new_constant(obj_ptr(function.__code__),
                                       jit.Type.void_ptr)
                globals = func.new_constant(obj_ptr(function.__globals__),
                                            jit.Type.void_ptr)
                nullptr = func.make_null_ptr()
                f = PyFrame_New(func, tstate, co, globals, nullptr)
            try:
                call = func.insn_call(name, target, signature, args)
            except:
                print(name, target, signature, args)
                raise
            # Decref any boxed arguments.
          # for a in args:
          #     if not is_jit_number(a):
          #       # obj_printer(func, a)
          #         Py_DecRef(func, a)
            for a in stack_args:
                DECREF(a)
            if allocate_frames:
                Py_DecRef(func, f)
          # if tgt.self_ptr is not None:
          #     # This is set to the base object pointer on known method
          #     # calls. Decref it here.
          #     obj_printer(func, tgt.self_ptr)
          #   # Py_DecRef(func, tgt.self_ptr)
            value, boxed_value = None, None
            if is_jit_number(call):
                value = call
            else:
                boxed_value = call
            stack.append(StackEntry(value=value, boxed_value=boxed_value,
                                    name=name + '()',
                                    type=signature.get_return(), refcount=1,
                                    freshly_allocated=True))
        # Fix up the return type if needed due to an annotation.
        if stack[-1].name in variables:
            stack[-1].type = variables[stack[-1].name]

    def MAKE_CLOSURE(func, arg, offset):
        assert arg == 0  # can't handle the general case yet
        v = POP()  # code object
        globals = func.new_constant(obj_ptr(function.__globals__),
                                    jit.Type.void_ptr)
        x = PyFunction_New(func, func.box_stack_entry(v), globals)
        DECREF(v)
        v = POP()
        PyFunction_SetClosure(func, x, func.box_stack_entry(v))
        DECREF(v)
        x = StackEntry(boxed_value=x, name='<closure>', refcount=1,
                       freshly_allocated=True)
        PUSH(x)

    def MAKE_FUNCTION(func, arg, offset):
        posdefaults = arg & 0xff
        kwdefaults = (arg >> 8) & 0xff
        assert kwdefaults == 0  # can't handle the general case yet
        num_annotations = (arg >> 16) & 0x7fff
        assert num_annotations == 0  # can't handle the general case yet
        posdefaults = arg
        v = POP()  # code object
        funcname = v.name
        globals = func.new_constant(obj_ptr(function.__globals__),
                                    jit.Type.void_ptr)
        x = PyFunction_New(func, func.box_stack_entry(v), globals)
        DECREF(v)
        if posdefaults > 0:
            v = PyTuple_New(func, func.new_constant(posdefaults, jit.Type.int))
            while posdefaults > 0:
                posdefaults -= 1
                w = POP()
                index = func.new_constant(posdefaults, jit.Type.int)
                PyTuple_SetItem(func, v, index, func.box_stack_entry(w))
            PyFunction_SetDefaults(func, x, v)
            Py_DecRef(func, v)
        x = StackEntry(boxed_value=x, name=funcname, refcount=2)
        PUSH(x)

    def SETUP_LOOP(func, arg, offset):
        loop_stack.append(Loop(label=arg + 3 + offset))

    def BREAK_LOOP(func, arg, offset):
        # Do the same thing as POP_BLOCK except don't do it destructively on
        # the loop_stack. That is, duplicate the topmost loop_stack entry so
        # POP_BLOCK can pop it off.
        loop_stack.append(loop_stack[-1])
        POP_BLOCK(func, arg, offset)
        # And now jump.
        func.insn_branch(labels[loop_stack[-1].label])

    def POP_BLOCK(func, arg, offset):
        if loop_stack[-1].iterator is not None:
            if verbose:
                print('decref iterator')
            Py_DecRef(func, loop_stack[-1].iterator)
        if loop_stack[-1].iter_var is not None:
            # FIXME: only if not reference_counting?
            if verbose:
                print('decref itervar')
            Py_DecRef(func, loop_stack[-1].iter_var)
        loop_stack.pop()

    def POP_TOP(func, arg, offset):
        v = POP()
        DECREF(v)

    def ROT_TWO(func, arg, offset):
        stack[-1], stack[-2] = stack[-2], stack[-1]

    def ROT_THREE(func, arg, offset):
        stack[-1], stack[-2], stack[-3] = stack[-2], stack[-3], stack[-1]

    def DUP_TOP(func, arg, offset):
        v = TOP()
        INCREF(v)
        PUSH(v)

    def DUP_TOP_TWO(func, arg, offset):
        x = TOP()
        INCREF(x)
        w = SECOND()
        INCREF(w)
        PUSH(w)
        PUSH(x)

    def UNPACK_SEQUENCE(func, arg, offset):
      # raise CompileError('UNPACK_SEQUENCE executed?')
        v = POP()
        sequence = v.boxed_value or v.constant
        assert sequence is not None
        if (sequence is v.constant and
            len(consts[v.constant_idx]) == arg and
            [dis.opname[c] for c in [code[offset+i*3] for i in range(1,arg+1)]]
                == ['STORE_FAST'] * arg):
            # This is a constant sequence which is being unpacked into just
            # the right number of variables. Access the elements directly
            # and unbox if necessary.
            while arg > 0:
                arg -= 1
                store_idx = offset + (arg+1) * 3
                element = consts[v.constant_idx][arg]
                name = str(element)
                varname = varnames[operand_value(code, store_idx)]
                if is_jit_number_type(symbols[varname].type):
                    typ = symbols[varname].type
                    t_unboxed, unboxed = maybe_unbox_constant(func, element)
                    if t_unboxed == typ:
                        unboxed_value = func.new_constant(element, typ)
                    else:
                        unboxed_value = func.insn_convert(unboxed, typ)
                    w = StackEntry(value=unboxed, type=typ, refcount=0,
                                   name=name, freshly_allocated=True)
                else:
                    element = func.new_constant(obj_ptr(element),
                                                jit.Type.void_ptr)
                    w = StackEntry(boxed_value=element,
                                   type=jit.Type.void_ptr, refcount=0,
                                   name=name, freshly_allocated=True)
                    INCREF(w)
                PUSH(w)
        else:
            # General case: Unpack at execution time.
            N = func.new_constant(arg, jit.Type.int)
            with func.branch(PyObject_Size(func,sequence) != N) as (false,end):
                # oops, cause exception to be raised
                null = func.make_null_ptr()
                func.insn_return(null)
            # else: OK
                func.insn_label(false)
            items = buffer_base_pointer(func, sequence, v.type)
            base_type = jit.Type.void_ptr
            while arg > 0:
                arg -= 1
                index = func.new_constant(arg, jit.Type.int)
                x = func.insn_load_elem(items, index, base_type)
                # Set these refcounts to 0 here; the incref will increment
                # them to 1 afterwards. In reality, there are still
                # references from the tuple (v), but those will disappear
                # with the decref below.
                w = StackEntry(boxed_value=x, type=base_type, refcount=0,
                               freshly_allocated=True)
                INCREF(w)
                PUSH(w)
        DECREF(v)

    def BUILD_LIST(func, arg, offset):
        x = PyList_New(func, func.new_constant(arg, jit.Type.int))
        while arg > 0:
            arg -= 1
            w = POP()
            index = func.new_constant(arg, jit.Type.int)
            PyList_SetItem(func, x, index, func.box_stack_entry(w))
            if not reference_counting and not w.freshly_allocated:
                # Force this incref now.
                Py_IncRef(func, w.boxed_value)
            else:
                INCREF(w)
        x = StackEntry(boxed_value=x, type=[object], refcount=1,
                       freshly_allocated=True)
        PUSH(x)

    def BUILD_TUPLE(func, arg, offset):
        x = PyTuple_New(func, func.new_constant(arg, jit.Type.int))
        if arg:
            base = buffer_base_pointer(func, x, jit.Type.tuple_t)
            while arg > 0:
                arg -= 1
                w = POP()
                index = func.new_constant(arg, jit.Type.int)
                func.insn_store_elem(base, index, func.box_stack_entry(w))
        x = StackEntry(boxed_value=x, refcount=1, freshly_allocated=True)
        PUSH(x)

    def BUILD_MAP(func, arg, offset):
        x = PyDict_NewPresized(func, func.new_constant(arg, jit.Type.int))
        x = StackEntry(boxed_value=x, refcount=1)
        PUSH(x)

    def STORE_MAP(func, arg, offset):
        w = POP()
        u = POP()
        v = TOP()  # leave this on the stack
        PyDict_SetItem(func, v.boxed_value, w.boxed_value, u.boxed_value)
        DECREF(u)
        DECREF(w)

    def GET_ITER(func, arg, offset):
        # Check if this is an abstract range object.
        if stack[-1].abstract is not None:
            assert(stack[-1].abstract.function == range)
            # If the next instruction is FOR_ITER and the one after that is
            # STORE_FAST, this loop will have been unboxed before, so there
            # is nothing to do here.
            if verbose:
                print('at offset', offset, 'for_loop_heads is', for_loop_heads)
            if (dis.opname[code[offset+1]] == 'FOR_ITER' and
                offset + 1 in for_loop_heads and
                dis.opname[code[offset+1+3]] == 'STORE_FAST'):
                if verbose:
                    print('unboxed loop, nothing to do for GET_ITER')
                return
            # FIXME: Also check for STORE_DEREF here?
            else:
                v = TOP()
                if verbose:
                    print('-- evaluate abstract call --')
                v.boxed_value = evaluate_abstract_call(func, v)
                if v.refcount is None:
                    v.refcount = 1
                if not reference_counting:
                    INCREF(v)
        else:
            v = TOP()
        # General implementation.
        x = PyObject_GetIter(func, v.boxed_value)
        if not reference_counting and v.freshly_allocated:
            # Force a decref.
            Py_DecRef(func, v.boxed_value)
            v.refcount -= 1
        else:
            DECREF(v)
        # Check for null return value; implemented differently from CPython.
        null = func.new_constant(0, jit.Type.void_ptr)
        with func.branch(x == null) as (false, end):
            func.insn_return(null)
        # else:
            func.insn_label(false)
        x = StackEntry(boxed_value=x, type=x.type, refcount=1,
                       freshly_allocated=True)
        SET_TOP(x)

    def FOR_ITER(func, arg, offset):
        # Check if this is an abstract range object.
        if stack[-1].abstract is not None:
            assert(stack[-1].abstract.function == range)
            # If the next instruction is STORE_FAST, this loop will have
            # been unboxed before, so there is nothing to do here.
            if dis.opname[code[offset+3]] == 'STORE_FAST':
                return
        # Record the iterator in the loop stack.
        if loop_stack:
            loop_target = arg + offset + 3 + 1
            if verbose:
                print('at FOR_ITER: arg =', arg, 'offset =', offset,
                      'target =', loop_target, '/', loop_target + 3,
                      'label =', loop_stack[-1].label)
            # Sometimes this is off by 3. FIXME: Understand generation of
            # loops in more detail.
            found_loop_stack_entry = False
            if (loop_stack[-1].label == loop_target or
                loop_stack[-1].label == loop_target + 3):
                found_loop_stack_entry = True
            elif dis.opname[code[loop_target]] == 'JUMP_ABSOLUTE':
                # Follow the jump.
                loop_target = operand_value(code, loop_target)
                if (loop_stack[-1].label == loop_target or
                    loop_stack[-1].label == loop_target + 3 or
                    loop_stack[-1].label + 3 == loop_target):
                    found_loop_stack_entry = True

            if found_loop_stack_entry:
                assert loop_stack[-1].iterator is None
                loop_stack[-1].iterator = TOP().boxed_value
                # If the next instruction is STORE_FAST, also store that
                # variable as the loop iteration variable. We need to decref
                # it when exiting the loop.
                if dis.opname[code[offset+3]] == 'STORE_FAST':
                    var_index = operand_value(code, offset+3)
                    iter_var = symbols[varnames[var_index]].location
                    iter_var = None  # FIXME
                    loop_stack[-1].iter_var = iter_var
            else:
                raise CompileError('no loop stack entry at FOR_ITER ' +
                                   str(offset) + ', control flow too complex?')
        # General implementation.
        v = POP()
        x = PyJIT_iternext(func, v.boxed_value)
        null = func.new_constant(0, jit.Type.void_ptr)
        with func.branch(x != null) as (false, end):
            x = StackEntry(boxed_value=x, type=x.type, refcount=1,
                           freshly_allocated=True)
            PUSH(x)
            func.insn_branch(end)
        # else: iterator ended (no error check here!)
            func.insn_label(false)
            # This pop is conditional in a real stack machine, but the
            # compile-time stack machine took care of popping the iterator
            # above.
          # x = v = POP()
          # DECREF(v)
            # Do not decref the iterator here, that's done by POP_BLOCK or
            # RETURN_VALUE.
          # Py_DecRef(func, v.boxed_value)
            JUMPBY(offset, arg)

    def BINARY_SUBSCR(func, arg, offset):
        w = POP()
        v = TOP()
        container, index = v, w
        assert container.name
        container_name = container.name
        name = container.name + '[]'
        # There are a few similar cases here:
        # - array with numeric index
        # - list with numeric index
        # - tuple with numeric index
        # - any kind of container with any kind of index
        if (# not bounds_checking and
            is_array(container.boxed_value) and
            is_jit_number(index.value)):
            base = get_base_pointer(func, container.name,
                                    container.boxed_value, container.type)
            base_type = container.type.get_ref()
            value = func.insn_load_elem(base, index.value, base_type)
            x = StackEntry(value=value, type=base_type, name=name)
            if bounds_checking:
                # Also need to simulate bounds checks. This is dumb.
                zero = func.new_constant(0)
                with func.branch(index.value < zero) as (false, end):
                    func.insn_return(zero)
                # else:
                    func.insn_label(false)
                    size_idx = func.new_constant(jit_ob_size_offset())
                    arr_size = func.insn_load_elem(container.boxed_value,
                                                   size_idx, jit.Type.int)
                    with func.branch(index.value >= arr_size) as (f2, e2):
                        func.insn_return(zero)
                    # else:
                        func.insn_label(f2)
                # That's it!
        elif (not bounds_checking and
              isinstance(container.type, list) and
              is_jit_number(index.value)):
            base = get_base_pointer(func, container.name,
                                    container.boxed_value, container.type)
            base_type = jit.Type.void_ptr
            boxed_value = func.insn_load_elem(base, index.value, base_type)
            base_type = container.type[0]
            if is_jit_number_type(base_type):
                value = func.unbox_value(boxed_value, base_type)
            else:
                Py_IncRef(func, boxed_value)
                value = None
            x = StackEntry(value=value, boxed_value=boxed_value,
                           type=base_type, name=name, refcount=1,
                           freshly_allocated=True)
        elif (not bounds_checking and
              is_tuple(container.boxed_value) and
              is_jit_number(index.value)):
            base = get_base_pointer(func, container.name,
                                    container.boxed_value, container.type)
            boxed_value = func.insn_load_elem(base, index.value,
                                              jit.Type.void_ptr)
            Py_IncRef(func, boxed_value)  # is this really necessary?
            base_type = (symbols[name].type if name in symbols
                         else jit.Type.void_ptr)
            if False and is_jit_number_type(base_type):
                Py_IncRef(func, boxed_value)  # why is this needed?
                value = func.unbox_value(boxed_value, base_type)
            else:
              # Py_IncRef(func, boxed_value)  # wasn't needed so far?
                value = None
            x = StackEntry(boxed_value=boxed_value, value=value,
                           type=base_type, name=name, refcount=1,
                           freshly_allocated=True)
        else:
            getitem_target = None
            if (container.type is not object and
                hasattr(container.type, '__name__')):
                getitem = container.type.__name__ + '.__getitem__'
                table = function_table[function.__module__]
                if getitem in table:
                    getitem_target = table[getitem]
                    getitem_target.__name__ = getitem.replace('.', '')
            container = func.box_stack_entry(v)
            base_type = object
            if getitem_target is not None:
                if late_binding:
                    # Simulate the lookup of the getitem attribute.
                    attr = func.new_constant(obj_ptr('__getitem__'))
                    getitem_ptr = PyObject_GetAttr(func,
                                                   container,
                                                   attr)
                    # And throw the result away.
                    Py_DecRef(func, getitem_ptr)
                index_type = getitem_target.create_signature().get_param(1)
                if is_jit_number_type(index_type):
                    assert w.value is not None
                    index = w.value
                else:
                    index = func.box_stack_entry(w)
                # Call this known getitem function directly.
                value = func.insn_call(getitem_target.__name__,
                                       getitem_target,
                                       getitem_target.create_signature(),
                                       [container, index])
            elif (container_name in variables and
                  is_list_type(variables[container_name])):
                index = func.box_stack_entry(w)
                value = PyObject_GetItem(func, container, index)
                base_type = variables[container_name][0]
            else:
                index = func.box_stack_entry(w)
                value = PyObject_GetItem(func, container, index)
            x = StackEntry(boxed_value=value, name=name, refcount=1,
                           type=base_type, freshly_allocated=True)
            if name in variables:
                x.type = variables[name]
            if is_jit_number_type(x.type):
                x.value = func.unbox_value(x.boxed_value, x.type, decref=False)
            elif static_number_typing:
                if is_array(v.boxed_value):
                    base_type = v.type.get_ref()
                elif isinstance(v.type, list):
                    base_type = v.type[0]
                else:
                    base_type = None
                if base_type is not None and is_jit_number_type(base_type):
                    if base_type == jit.Type.float64:
                        x.type = float
                    else:
                        x.type = int
        DECREF(v)
        DECREF(w)
        SET_TOP(x)

    def STORE_SUBSCR(func, arg, offset):
        w = POP()
        v = POP()
        u = POP()
        container, index, value = v, w, u
        if verbose:
            print('container', container)
            print('index', index)
            print('value', value)
        # There are a few similar cases here:
        # - array with numeric index
        # - list with numeric index
        # - tuple with numeric index
        # - any kind of container with any kind of index
        if (not bounds_checking and
            is_array(container.boxed_value) and
            is_jit_number(index.value)):
            base = get_base_pointer(func, container.name,
                                    container.boxed_value, container.type)
            base_type = container.type.get_ref()
            if value.value is None:
                value.value = func.unbox_value(value.boxed_value, base_type,
                                               decref=False)
            value = func.insn_convert(value.value, base_type)
            func.insn_store_elem(base, index.value, value)
        elif (not bounds_checking and
              (isinstance(container.type, list) or
               is_tuple(container.boxed_value)) and
              is_jit_number(index.value)):
            base = get_base_pointer(func, container.name,
                                    container.boxed_value, container.type)
            old = func.insn_load_elem(base, index.value, jit.Type.void_ptr)
            Py_DecRef(func, old)
            if value.boxed_value is None:
                # This does not use box_stack_entry but performs an explicit
                # incref. Is that very good?
                value.boxed_value = func.box_value(value.value)
                value.refcount = 1
            func.insn_store_elem(base, index.value, value.boxed_value)
            Py_IncRef(func, value.boxed_value)
        else:
            base = container.boxed_value
            if index.boxed_value is None:
                func.box_stack_entry(index)
            if value.boxed_value is None:
                func.box_stack_entry(value)
            PyObject_SetItem(func, base, index.boxed_value, value.boxed_value)
        DECREF(u)
        DECREF(v)
        # Do we need to decref the index? Not if it's an int constant.
        if w.name is None or not w.name.isdigit():
            DECREF(w)

    def BUILD_SLICE(func, arg, offset):
        assert arg in {2, 3}
        w = POP() if arg == 3 else None
        v = POP()
        u = TOP()
        start, end, step = u, v, w
        if verbose:
            print('start', start, 'end', end, 'step', step)
        start_box, end_box, step_box = \
                [func.box_stack_entry(x, allow_none=True) for x in (u, v, w)]
        slice = PySlice_New(func, start_box, end_box, step_box)
        x = StackEntry(boxed_value=slice, refcount=1)
        DECREF(u)
        DECREF(v)
        if w is not None and (w.value is None or
                              not is_jit_number_type(w.type)):
            DECREF(w)
        SET_TOP(x)

    def LOAD_ATTR(func, arg, offset):
        w = names[arg]
        v = TOP()
        base, attr = v, w
        if base.name is None:
            base.name = '<anonymous object>'
        if ((base.type is None or base.type == jit.Type.void_ptr) and
            base.name in variables):
            base.type = variables[base.name]
        name = base.name + '.' + attr
        did_getattr = False
        if hasattr(base, 'type') and hasattr(base.type, '__name__'):
            # Qualify the attribute name by class name. Used for methods.
            qualname = base.type.__name__ + '.' + attr
        else:
            qualname = name
        if verbose:
            print(name, qualname, attr, function_table)
        base_value = base.boxed_value
        if base_value is None and base.abstract is not None:
            base_value = func.new_constant(obj_ptr(base.abstract))
        if name in variables or base.name in function.__code__.co_consts:
            attr = func.new_constant(obj_ptr(attr))
            value = PyObject_GetAttr(func, base_value, attr)
            did_getattr = True
            if name in variables:
                typ = variables[name]
            else:
                assert base.name in function.__code__.co_consts
                global type
                typ = type(base.name)
            unboxed = None
            if is_jit_number_type(typ):
                unboxed = func.unbox_value(value, typ)
                value = None
            x = StackEntry(name=name, boxed_value=value,
                           value=unboxed, type=typ, refcount=1)
        elif qualname in function_table[function.__module__]:
          # obj_printer(func, base.boxed_value)
            target = function_table[function.__module__][qualname]
            if target is None:
                # recursive call
                target = func
            call = Call(target, [base.boxed_value])
          # Py_IncRef(func, base.boxed_value)  # simulate method incref
            x = StackEntry(name=attr, value=call, self_ptr=base.boxed_value,
                           type=target.create_signature(), refcount=1)
            INCREF(x)
        elif qualname == 'math.pi' and qualname in intrinsics:
            pi = func.new_constant(math.pi, jit.Type.float64)
            x = StackEntry(name=name, value=pi)
        elif qualname in intrinsics and qualname in known_intrinsics:
            x = StackEntry(name=name, intrinsic=known_intrinsics[qualname])
        else:
            attr = func.new_constant(obj_ptr(attr))
            value = PyObject_GetAttr(func, base_value, attr)
            did_getattr = True
          # if w == 'score':
          #     marker_int(func, func.new_constant(offset))
          #     obj_printer(func, base_value)
          #     obj_printer(func, attr)
          #     obj_printer(func, value)
            x = StackEntry(name=name, boxed_value=value, type=object,
                           refcount=1, freshly_allocated=True)
        if late_binding and not did_getattr:
            # Simulate this call.
            attr = func.new_constant(obj_ptr(attr))
            value = PyObject_GetAttr(func, base_value, attr)
            # And throw the result away.
            Py_DecRef(func, value)
        DECREF(v)
        SET_TOP(x)

    def STORE_ATTR(func, arg, offset):
        w = names[arg]
        attr = func.new_constant(obj_ptr(w))
        v = POP()
        base = v.boxed_value
        u = POP()
        value = func.box_stack_entry(u)
      # if w == 'ledges':
      #     marker_int(func, func.new_constant(1))
      #     obj_printer(func, value)
        PyObject_SetAttr(func, base, attr, value)
        DECREF(v)
        DECREF(u)
        

    operations = locals()
    binops = dict()
    int_binops = dict()
    float_binops = dict()
    overloads = dict()
    comparisons = dict()
    labels = dict()
    arguments = dict()
    known_intrinsics = dict()

    @builder(return_type=return_type, argument_types=argument_types,
             dump_code=dump_code)
    def function_builder(func_):
        nonlocal func
        func = func_
        # set up variables
        for name, var in symbols.items():
            if isinstance(var.type, jit.Type):
                if name in freevars:
                    raise CompileError("cell variable must be "
                                       "of Python type: " + name)
            if isinstance(var, Argument):
                arguments[name] = var
                val = func.get_param(var.idx)
                type = (var.type if isinstance(var.type, jit.Type)
                                 else jit.Type.void_ptr)
                symbols[name] = Local(var.type, func.new_value(type, val),
                                      is_argument=True)
                if name in freevars:
                    func.store(symbols[name].location, PyCell_New(func, val))
            else:
                assert isinstance(var, Local)
                if isinstance(var.type, jit.Type):
                    var.location = func.new_value(var.type)
                    if not is_jit_number_type(var.type):
                        func.store(var.location, func.make_null_ptr())
                else:
                    var.location = func.new_value(jit.Type.void_ptr)
                    null = func.make_null_ptr()
                    if name in freevars:
                        func.store(var.location, PyCell_New(func, null))
                    else:
                        func.store(var.location, null)
        for c in function.__code__.co_consts:
            if c is not None and isinstance(c, collections.Hashable):
                const = func.new_constant(obj_ptr(c), jit.Type.void_ptr)
                func.boxed_constants[c] = const
        # Precompute base pointers for any fixed buffer arguments.
        if optimize_fixed_buffers:
            for name in fixed_buffers:
                base_pointer_name = 'base pointer for ' + name
                if name in arguments:
                    obj = symbols[name].location
                    base_pointer = func.new_value(jit.Type.void_ptr)
                    buffer = buffer_base_pointer(func, obj, symbols[name].type)
                    func.store(base_pointer, buffer)
                    symbols[base_pointer_name] = Local(jit.Type.void_ptr,
                                                       base_pointer)
                else:
                    # This fixed buffer is a local. Or even a global.
                    symbols[base_pointer_name] = Local(jit.Type.void_ptr,
                                                       None)
        # Allocate scratch space for function calls. (TODO: Check if the
        # code contains calls.)
        stacksize = jit.Type.void_ptr.size * function.__code__.co_stacksize
        nonlocal scratch_space
        scratch_space = func.insn_alloca(func.new_constant(stacksize))

        nonlocal binops
        binops = {
                AST.Add:      BinOp(func.insn_add, PyNumber_Add),
                AST.Sub:      BinOp(func.insn_sub, PyNumber_Subtract),
                AST.Mult:     BinOp(func.insn_mul, PyNumber_Multiply),
                AST.Div:      BinOp(func.insn_div, PyNumber_TrueDivide),
                AST.Mod:      BinOp(func.insn_rem, PyNumber_Remainder),
              # AST.Pow: not supported (missing integer case in libjit)
                AST.LShift:   BinOp(func.insn_shl, PyNumber_Lshift),
                AST.RShift:   BinOp(func.insn_shr, PyNumber_Rshift),
                AST.BitOr:    BinOp(func.insn_or,  PyNumber_Or),
                AST.BitXor:   BinOp(func.insn_xor, PyNumber_Xor),
                AST.BitAnd:   BinOp(func.insn_and, PyNumber_And),
                AST.FloorDiv: BinOp(func.insn_div, PyNumber_FloorDivide)
        }
        binop_indices = {
                AST.Add:       0,
                AST.Sub:       1,
                AST.Mult:      2,
                AST.Mod:       3,
                AST.Pow:       5,
                AST.LShift:   11,
                AST.RShift:   12,
                AST.BitAnd:   13,
                AST.BitXor:   14,
                AST.BitOr:    15,
                AST.FloorDiv: 29,
                AST.Div:      30
        }
        nonlocal int_binops, float_binops
        for k, v in binop_indices.items():
            int_binops[k] = py_int_binaryfunc(v)
            float_binops[k] = py_float_binaryfunc(v)
        nonlocal overloads
        overloads = {
                AST.Add: '__add__',
                AST.Sub: '__sub__'
        }
        nonlocal comparisons
        comparisons = {
                2: BinOp(func.insn_eq, AST.Eq),
                3: BinOp(func.insn_ne, AST.NotEq),
                0: BinOp(func.insn_lt, AST.Lt),
                1: BinOp(func.insn_le, AST.LtE),
                4: BinOp(func.insn_gt, AST.Gt),
                5: BinOp(func.insn_ge, AST.GtE)
        }
        nonlocal known_intrinsics
        known_intrinsics = {
                'abs':  func.insn_abs,
                'cos':  func.insn_cos,
                'exp':  func.insn_exp,
                'log':  func.insn_log,
                'sin':  func.insn_sin,
                'sqrt': func.insn_sqrt,
                'tan':  func.insn_tan,
                'pi':   None  # a constant, not really an intrinsic
        }
        # Add qualified names as well.
        for name in list(known_intrinsics.keys()):
            known_intrinsics['math.'+name] = known_intrinsics[name]
        for i in intrinsics:
            if i not in known_intrinsics:
                raise CompileError('unknown intrinsic ' + i)


        nonlocal labels, stack, phi_locations
        func.stack = stack
        labels = {l: func.new_label() for l in dis.findlabels(code)}
        phi_locations = {l: func.new_value(jit.Type.void_ptr, 0)
                         for l in dis.findlabels(code)}

        # Ensure that we own references to our arguments.
        for name in arguments:
            if not is_jit_number(symbols[name].location):
                Py_IncRef(func, symbols[name].location)

        for bc, arg, offset in enumerate_code(code):
          # marker_int(func, func.new_constant(offset, jit.Type.int))
          # if verbose:
          #     marker_int(func, func.new_constant(offset, jit.Type.int))
          #     obj_printer(func, PyErr_Occurred(func))
            if verbose:
                dump_stack()
                print(str(offset) + ':',
                      dis.opname[bc],
                      arg if arg is not None else '')
            if stack and stack[-1].boxed_value is not None:
                # Make sure we track reference counts.
                assert stack[-1].refcount is not None
            if (dis.opname[bc] == 'GET_ITER' and
                dis.opname[code[offset+1]] == 'FOR_ITER'):
                iter_var_idx = operand_value(code, offset+1+3)
                if dis.opname[code[offset+1+3]] == 'STORE_DEREF':
                    iter_var_container = freevars
                else:
                    iter_var_container = varnames
                iter_var_name = iter_var_container[iter_var_idx]
                iter_var = symbols[iter_var_name]
            else:
                iter_var = None
            if iter_var is not None:
                if verbose:
                    print('offset', offset, 'iter_var_name:', iter_var_name)
                    print(dis.opname[bc])
                    print(stack[-1].abstract)
                    print(dis.opname[code[offset+1]])
                    print(iter_var)
                    print(is_jit_number_type(iter_var.type))
            if (dis.opname[bc] == 'GET_ITER' and
                dis.opname[code[offset+1]] == 'FOR_ITER' and
                stack[-1].abstract is not None and
                dis.opname[code[offset+1+3]] == 'STORE_FAST' and
                iter_var is not None and
                is_jit_number_type(iter_var.type)):
                for_offset = offset + 1
                assert for_offset in labels  # we will need this label
                # Perform magic: Unbox the for loop head (assume for now
                # that it's a counting loop).
                loop_range = stack[-1]
                assert loop_range.abstract.function == range
                args = loop_range.abstract.args
                assert len(args) in [1, 2, 3]
                # Initialize iteration variable.
                assert is_jit_number_type(iter_var.type)
                for_loop_heads[for_offset] = iter_var
                if len(args) == 1:
                    func.store(iter_var.location, func.new_constant(0))
                else:
                    if verbose:
                        print('---', args[0])
                    if args[0].value is None:
                        t = jit.Type.int
                        args[0].value = func.unbox_value(args[0].boxed_value,t)
                    func.store(iter_var.location, args[0].value)
                # Find the counting loop's limit. Note that this is constant
                # throughout the loop (range is only evaluated once).
                if len(args) == 1:
                    limit = args[0]
                else:
                    limit = args[1]
                assert not isinstance(limit, jit.Value)
                if not isinstance(limit, jit.Value):
                    if limit.value is None:
                        type, value = maybe_unbox_constant(func,
                                                           limit.boxed_value)
                    else:
                        type, value = limit.type, limit.value
                    if value:
                        limit = value
                        if verbose:
                            print('limit:', limit)
                    else:
                        if verbose:
                            print('limit:', limit)
                            dump_stack()
                        assert limit.boxed_value is not None
                        limit = PyLong_AsLong(func, limit.boxed_value)
                # Evaluate labeled loop condition.
                func.insn_label(labels[for_offset])
                if len(args) == 3:
                    # The third argument, the increment, may be negative.
                    # The loop condition depends on this.
                    increment = args[2]
                  # if not isinstance(increment.boxed_value, int):
                  #     raise CompileError('cannot handle counting loop '
                  #                        'with non-constant increment')
                    try:
                        # If this is a numeric constant, get the value back.
                        inc_value = int(increment.name)
                    except:
                        inc_value = None
                    if inc_value is not None:
                        # Constant increment. We can deduce the direction.
                        if inc_value < 0:
                            # Count downwards.
                            cond = func.insn_gt(iter_var.location, limit)
                        else:
                            # Count upwards.
                            cond = func.insn_lt(iter_var.location, limit)
                        const = func.new_constant(inc_value)
                        for_loop_increments[for_offset] = const
                    else:
                        # Non-constant increment. Assume upward counting
                        # loop; this is not checked!
                        cond = func.insn_lt(iter_var.location, limit)
                        assert increment.value is not None
                        for_loop_increments[for_offset] = increment.value
                else:
                    # Count upwards by 1.
                    cond = func.insn_lt(iter_var.location, limit)
                    for_loop_increments[for_offset] = func.new_constant(1)
                loop_exit = loop_stack[-1].label
                func.insn_branch_if_not(cond, labels[loop_exit])
            elif (dis.opname[bc] == 'BUILD_TUPLE' and
                  dis.opname[code[offset+3]] == 'UNPACK_SEQUENCE'):
                operand = operand_value(code, offset)
                assert operand == operand_value(code, offset+3)
                # Build and unpack a tuple: This is for parallel
                # assignments. Ignore it here and handle this pair of
                # instructions below.
                continue
            elif (dis.opname[bc] == 'UNPACK_SEQUENCE' and
                  offset >= 3 and
                  dis.opname[code[offset-3]] == 'BUILD_TUPLE'):
                N = operand_value(code, offset)
                # A pair of build-unpack instructions simply reverses the
                # order of the uppermost N elements on the stack. We can do
                # this at compile time and can skip construction of a tuple.
                stack[-N:] = reversed(stack[-N:])
                continue
            elif offset in labels and offset not in for_loop_heads:
                do_transform = len(stack) > 0 and stack[-1].abstract is None
                if do_transform:
                    if verbose:
                        print('at', offset, 'transform:')
                        dump_stack()
                    x = POP()
                    assert x.boxed_value is not None or x.value is not None
                    if x.boxed_value is None and x.type == jit.Type.int:
                        # "Box" this by comparing to 0 and using a constant
                        # pointer to True or False.
                        zero = func.new_constant(0, jit.Type.int)
                        true = func.new_constant(obj_ptr(True),
                                                 jit.Type.void_ptr)
                        false = func.new_constant(obj_ptr(False),
                                                  jit.Type.void_ptr)
                        with func.branch(x.value != zero) as (false_lbl, end):
                            func.store(phi_locations[offset], true)
                            func.insn_branch(end)
                        # else:
                            func.insn_label(false_lbl)
                            func.store(phi_locations[offset], false)
                    else:
                        boxed_value = func.box_stack_entry(x)
                        if not reference_counting:
                            if not x.freshly_allocated: 
                                # If this value is the result of some control
                                # flow stuff, which it apparently is, make sure
                                # we have a refcount on it. This is a bit of a
                                # hack.
                                Py_IncRef(func, x.boxed_value)
                        func.store(phi_locations[offset], boxed_value)
                    # And make sure the top-of-stack is set correctly.
                    PUSH(StackEntry(boxed_value=phi_locations[offset],
                                    name='phi_'+str(offset), refcount=1,
                                    freshly_allocated=x.freshly_allocated))
                    if verbose:
                        print('into:')
                        dump_stack()
                # Now place the label.
                func.insn_label(labels[offset])
            try:
                operations[dis.opname[bc]](func, arg, offset)
            except:
                func.dump()
                dump_stack()
                raise
        if verbose:
            print('done! return type:')
            func.signature.name()
            func.signature.get_return().name()
            sys.stdout.flush()

    return function_builder

compile = bc_compile

puts = ctypes.cast(pylib.puts, ctypes.c_void_p).value
puts_sig = jit.Type.create_signature(jit.abi.cdecl, jit.Type.int, [jit.Type.void_ptr])

def str_ptr(s):
    orig = ctypes.c_char_p(bytes(s, 'utf-8'))
    ptr = ctypes.cast(orig, ctypes.c_void_p).value
    return ptr, orig

def py_str_ptr(func, s):
    ptr = ctypes.cast(s, ctypes.c_void_p).value
    return func.new_constant(ptr, jit.Type.void_ptr)

# import gc
# gc.set_debug(gc.DEBUG_LEAK)
