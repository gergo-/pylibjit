import jit
import collections
import copy
import math
import sys

import ast as AST

from .utils import *

def range_parameters(range):
    '''range objects have no start, stop, step attributes until Python 3.3.'''
    if not range:
        return 0, 0, 1
    start = range[0]
    if len(range) > 1:    
        step = range[1] - range[0]
    else:
        step = 1
    stop = range[-1] + step # first value *not* in range
    return start, stop, step

LoopDescriptor = collections.namedtuple('LoopDescriptor',
                                        ['break_label', 'continue_label'])

def get_ast(function):
    # from Python Cookbook
  # srclines = inspect.getsource(function).splitlines()
  # src = '\n'.join(srclines)
    src = inspect.getsource(function)
    # hack for indented code
    if src.startswith((' ', '\t')):
        src = 'if 1:\n' + src
    ast = AST.parse(src, mode='exec')
    # unwrap module
    funcdef = ast.body[0]
    if isinstance(funcdef, AST.If):
        # unwrap if
        funcdef = funcdef.body[0]
    funcdef.decorator_list = []
    return funcdef

def compiler(function_def, function, return_type, argument_types,
             variables, global_vars, constant_size, intrinsics,
             verbose):
    assert len(function_def.args.args) == len(argument_types)
    symbols = dict()
    for i, (arg, type) in enumerate(zip(function_def.args.args,
                                        argument_types)):
        symbols[arg.arg] = Argument(type, i)
    for name, type in variables.items():
        symbols[name] = Local(type, location=None)
    names = dict()

    cached_base_addresses = dict()

    @builder(return_type=return_type, argument_types=argument_types,
             dump_code=verbose)
    def function_builder(func):
        def compile_expr(expr):
            if verbose:
                print(AST.dump(expr))
            v = compile_expr_(expr)
            return v

        def compile_slice(slice):
            nil = func.make_null_ptr()
            lower, upper, step = [func.box_value(compile_expr(e)) if e else nil
                                  for e in [slice.lower, slice.upper,
                                            slice.step]]
            result = PySlice_New(func, lower, upper, step)
            for e in [lower, upper, step]:
                if e is not nil:
                    Py_DecRef(func, e)
            return result

        def compile_bool_expr(expr):
            """If a numeric expression is used in a boolean-like context (in
            a branch or loop condition), produce a comparison against 0."""
            v = compile_expr(expr)
            if expr.__class__ != AST.Compare:
                if is_jit_number(v):
                    v = (v != func.new_constant(0, v.type))
                else:
                    # Create a boxed comparison.
                    zero = func.new_constant(0)
                    r = func.box_value(zero)
                    op = comparisons[AST.NotEq].boxed_op
                    Py_IncRef(func, v)
                    v = (PyObject_RichCompareBool(func, v, r, op) != zero)
            return v

        def is_list_access(expr):
            return (expr.__class__ == AST.Name and
                    isinstance(symbols[expr.id].type, list))

        def container_base_type(expr, jit_value):
            if is_list_access(expr):
                return symbols[expr.id].type[0]
            else:
                return jit_value.type.get_ref()

        def access_path(expr):
            """A string of the form foo or foo.bar.baz if expr is a name or
            a series of attribute accesses where the base is a name."""
            if expr.__class__ == AST.Name:
                return expr.id
            elif expr.__class__ == AST.Attribute:
                base = access_path(expr.value)
                if base:
                    return base + '.' + expr.attr
            return None

        def base_address(array_or_list, expr, type=None):
            if type is None:
                type = array_or_list.type
            if not (type.is_array or is_list_access(expr)):
                return None
            if expr:
                name = access_path(expr)
                if name and name in cached_base_addresses:
                    return cached_base_addresses[name]
                elif name and name in constant_size:
                    base = PyArray_AsPointer(func, array_or_list)
                    cached_base_addresses[name] = base
                    return base
            base = PyArray_AsPointer(func, array_or_list)
            return base

        def container_store(base, index, val, expr=None):
            if not is_jit_number(index):
                index = func.unbox_value(index, jit.Type.int)
            if base.type.is_array:
                base_type = base.type.get_ref()
                base = base_address(base, expr)
                if not is_jit_number(val):
                    val = func.unbox_value(val, base_type)
                val = func.insn_convert(val, base_type)
                func.insn_store_elem(base, index, val)
            else:
                base = base_address(base, expr)
                val = func.box_value(val)
                func.insn_store_elem(base, index, val)

        known_intrinsics = {
                'cos':  func.insn_cos,
                'exp':  func.insn_exp,
                'log':  func.insn_log,
                'sin':  func.insn_sin,
                'sqrt': func.insn_sqrt
        }
        # Add qualified names as well.
        for name in list(known_intrinsics.keys()):
            known_intrinsics['math.'+name] = known_intrinsics[name]
        print(known_intrinsics)

        for i in intrinsics:
            if i not in known_intrinsics:
                raise CompileError('unknown intrinsic ' + i)

        binops = {
                AST.Add:      BinOp(func.insn_add, PyNumber_Add),
                AST.Sub:      BinOp(func.insn_sub, PyNumber_Subtract),
                AST.Mult:     BinOp(func.insn_mul, PyNumber_Multiply),
              # AST.Div: special handling depending on types
                AST.Mod:      BinOp(func.insn_rem, PyNumber_Remainder),
              # AST.Pow: not supported (missing integer case in libjit)
                AST.LShift:   BinOp(func.insn_shl, PyNumber_Lshift),
                AST.RShift:   BinOp(func.insn_shr, PyNumber_Rshift),
                AST.BitOr:    BinOp(func.insn_or,  PyNumber_Or),
                AST.BitXor:   BinOp(func.insn_xor, PyNumber_Xor),
                AST.BitAnd:   BinOp(func.insn_and, PyNumber_And),
                AST.FloorDiv: BinOp(func.insn_div, PyNumber_FloorDivide)
        }

        comparisons = {
                AST.Eq:       BinOp(func.insn_eq, func.new_constant(2)),
                AST.NotEq:    BinOp(func.insn_ne, func.new_constant(3)),
                AST.Lt:       BinOp(func.insn_lt, func.new_constant(0)),
                AST.LtE:      BinOp(func.insn_le, func.new_constant(1)),
                AST.Gt:       BinOp(func.insn_gt, func.new_constant(4)),
                AST.GtE:      BinOp(func.insn_ge, func.new_constant(5))
        }

        def compile_expr_(expr):
            global function_table
            if isinstance(expr, AST.Name):
                if expr.id in global_vars:
                    globals = func.new_constant(obj_ptr(function.__globals__),
                                                jit.Type.void_ptr)
                    id = func.new_constant(obj_ptr(expr.id), jit.Type.void_ptr)
                    g = PyObject_GetItem(func, globals, id)
                    return func.unbox_value(g, global_vars[expr.id])
                elif isinstance(symbols[expr.id], Local):
                    return symbols[expr.id].location
                elif isinstance(symbols[expr.id], Constant):
                    return symbols[expr.id].value
                else:
                    raise CompileError("can't handle" + expr + "name")
            elif isinstance(expr, AST.Num):
                return func.new_constant(expr.n)
            elif isinstance(expr, AST.BinOp):
                left = compile_expr(expr.left)
                right = compile_expr(expr.right)
                unboxed = is_jit_number(left) and is_jit_number(right)
                if expr.op.__class__ == AST.Div:
                    assert unboxed
                    # Python semantics is to always do floating-point
                    # division.
                    left = func.insn_convert(left, jit.Type.float64)
                    right = func.insn_convert(right, jit.Type.float64)
                    return left / right
                elif unboxed:
                    return binops[expr.op.__class__].unboxed_op(left, right)
                else:
                    l = func.box_value(left)
                    r = func.box_value(right)
                    return binops[expr.op.__class__].boxed_op(func, l, r)
            elif isinstance(expr, AST.UnaryOp):
                assert expr.op.__class__ == AST.USub
                val = compile_expr(expr.operand)
                if is_jit_number(val) and val.is_constant:
                    if val.type == jit.Type.int and False:
                        # FIXME: Negative constants are broken.
                        pass
                    elif val.type == jit.Type.float64:
                        return func.new_constant(-val.float64_constant())
                return -compile_expr(expr.operand)
            elif isinstance(expr, AST.Compare):
                left = compile_expr(expr.left)
                assert len(expr.ops) == 1
                op = expr.ops[0]
                right = compile_expr(expr.comparators[0])
                unboxed = is_jit_number(left) and is_jit_number(right)
                if unboxed:
                    return comparisons[op.__class__].unboxed_op(left, right)
                else:
                    l = func.box_value(left)
                    r = func.box_value(right)
                    op = comparisons[op.__class__].boxed_op
                    return PyObject_RichCompareBool(func, l, r, op)
            elif isinstance(expr, AST.BoolOp):
                assert len(expr.values) == 2
                if isinstance(expr.op, AST.And):
                    left = compile_bool_expr(expr.values[0])
                    result = func.new_value(left.type, left)
                    with func.branch(result) as (false, end):
                        right = compile_bool_expr(expr.values[1])
                        func.store(result, right)
                        func.insn_branch(end)
                    # else:
                        func.insn_label(false)
                    return result
                else:
                    assert isinstance(expr.op, AST.Or)
                    raise CompileError('TODO: implement logical or')
            elif isinstance(expr, AST.Call):
                args = [compile_expr(arg) for arg in expr.args]
                if expr.func.__class__ == AST.Name:
                    funcname = expr.func.id
                elif expr.func.value.id == function_def.args.args[0].arg:
                    # Attribute access through plain name, assume that
                    # this name is the first argument to this function
                    # (i.e., the 'self' pointer).
                    funcname = expr.func.attr
                    args = [compile_expr(expr.func.value)] + args
                else:
                    # General attribute access.
                    funcname = access_path(expr.func)
                    if funcname not in intrinsics:
                        obj = compile_expr(expr.func.value)
                        attr = func.new_constant(obj_ptr(expr.func.attr))
                        method = PyObject_GetAttr(func, obj, attr)
                        return func.call_py_object(method, args)
                if funcname == function_def.name:
                    return func.recursive_call(funcname, args)
                elif funcname == 'len':
                    return PyObject_Size(func, args[0])
                elif funcname in ['list', 'print', 'range']:
                    return func.call_py_object(funcname, args)
                elif funcname in function_table[function.__module__]:
                    target = function_table[function.__module__][funcname]
                    return func.insn_call(
                            funcname, target, target.create_signature(), args)
                elif funcname in symbols:
                    target = symbols[funcname].location
                    return func.call_py_object(target, args)
                elif funcname in intrinsics:
                    target = known_intrinsics[funcname]
                    assert len(args) == 1
                    arg = args[0]
                    if not is_jit_number(arg):
                        arg = func.unbox_value(arg, jit.Type.float64)
                    arg = func.insn_convert(arg, jit.Type.float64)
                    return target(arg)
                else:
                    raise CompileError("can't handle call to " + funcname)
            elif isinstance(expr, AST.Subscript):
                base = compile_expr(expr.value)
                if expr.slice.__class__ == AST.Slice:
                    assert expr.ctx.__class__ == AST.Load
                    slc = compile_slice(expr.slice)
                    result = PyObject_GetItem(func, base, slc)
                    Py_DecRef(func, slc)
                    return result
                # Otherwise, it's an index.
                index = compile_expr(expr.slice.value)
                # See if the container can be unboxed.
                if base.type.is_array or is_list_access(expr.value):
                    if not is_jit_number(index):
                        index = func.unbox_value(index, jit.Type.int)
                    if expr.ctx.__class__ == AST.Load:
                        base_type = container_base_type(expr.value, base)
                        if isinstance(base_type, jit.Type):
                            load_type = base_type
                        else:
                            load_type = jit.Type.void_ptr
                        base = base_address(base, expr.value)
                        val = func.insn_load_elem(base, index, load_type)
                        if is_list_access(expr.value):
                            if is_jit_number_type(base_type):
                                val = func.unbox_value(val, base_type)
                            else:
                                Py_IncRef(func, val)
                        return val
                    else:
                        # base = base_address(base, expr.value)
                        return base, index
                else:
                    raise CompileError('general subscripting not supported')
            elif expr.__class__ == AST.List:
                l = PyList_New(func, func.new_constant(len(expr.elts)))
                for i, e in enumerate(expr.elts):
                    i = func.new_constant(i)
                    PyList_SetItem(func, l, i, compile_expr(e))
                return l
            elif expr.__class__ == AST.Attribute:
                attr = func.new_constant(obj_ptr(expr.attr))
                attr = PyObject_GetAttr(func, compile_expr(expr.value), attr)
                attr_name = access_path(expr)
                assert attr_name is not None
                type = symbols[attr_name].type
                if is_jit_number_type(type):
                    attr = func.unbox_value(attr, type)
                return attr
            else:
                raise CompileError('bad expr:', AST.dump(expr))

        def compile_stmt(stmt):
            if verbose:
                print('compile', stmt)
            if isinstance(stmt, list):
                for child in stmt:
                    compile_stmt(child)
            elif isinstance(stmt, AST.If):
                with func.branch(compile_bool_expr(stmt.test)) as (false, end):
                    compile_stmt(stmt.body)
                    func.insn_branch(end)
                # else:
                    func.insn_label(false)
                    compile_stmt(stmt.orelse)
            elif isinstance(stmt, AST.While):
                if not stmt.orelse:
                    with func.loop(func.loop_start(),
                                   compile_bool_expr(stmt.test)):
                        compile_stmt(stmt.body)
                else:
                    cond_label = func.loop_start()
                    end_label = func.new_label()
                    with func.loop_with_else(cond_label,
                                             compile_bool_expr(stmt.test),
                                             end_label) as else_label:
                        compile_stmt(stmt.body)
                        func.insn_branch(cond_label)
                    # else:
                        func.insn_label(else_label)
                        compile_stmt(stmt.orelse)
            elif isinstance(stmt, AST.For):
                iteration_space = stmt.iter
                if isinstance(iteration_space, AST.Name):
                    iteration_space = symbols[iteration_space.id]
                    assert isinstance(iteration_space, Constant)
                    assert iteration_space.value is not None
                    assert isinstance(iteration_space.value, AST.Call)
                    iteration_space = iteration_space.value
                if isinstance(iteration_space, AST.Call) and \
                   iteration_space.func.id == 'range':
                    # parse arguments
                    args = iteration_space.args
                    if len(args) == 1:
                        args = (AST.Num(n=0), args[0], AST.Num(n=1))
                    elif len(args) == 2:
                        args = (args[0], args[1], AST.Num(n=1))
                    ascending = None
                    if isinstance(args[2], AST.Num):
                        if args[2].n > 0:
                            ascending = True
                        else:
                            ascending = False
                    start, stop, step = (compile_expr(a) for a in args)
                # Now implement the loop.
                counter = compile_expr(stmt.target)
                func.store(counter, start)
                if ascending is not None:
                    # We know statically which way this loop will go.
                    if ascending:
                        with func.loop(func.loop_start(), counter < stop):
                            compile_stmt(stmt.body)
                            func.store(counter, counter + step)
                    else:
                        with func.loop(func.loop_start(), counter > stop):
                            compile_stmt(stmt.body)
                            func.store(counter, counter + step)
                else:
                    # The direction of the loop must be determined dynamically.
                    # This is a bit of a bummer.
                    zero = func.new_constant(0)
                    with func.branch(step > zero) as (false, end):
                        # step >= 0
                        with func.loop(func.loop_start(), counter < stop):
                            compile_stmt(stmt.body)
                            func.store(counter, counter + step)
                        func.insn_branch(end)
                    # else: step < 0
                        func.insn_label(false)
                        with func.loop(func.loop_start(), counter > stop):
                            compile_stmt(stmt.body)
                            func.store(counter, counter + step)
            elif isinstance(stmt, AST.Break):
                func.insn_branch(func.loop_stack[-1].break_label)
            elif isinstance(stmt, AST.Continue):
                func.insn_branch(func.loop_stack[-1].continue_label)
            elif isinstance(stmt, AST.Return):
                if stmt.value is not None:
                    func.insn_return(compile_expr(stmt.value))
                else:
                    func.insn_return()
            elif isinstance(stmt, AST.Assign):
                target = stmt.targets[0]
                if not isinstance(target, AST.Tuple):
                    val = compile_expr(stmt.value)
                    # Assignment to single object.
                    if isinstance(target, AST.Name):
                        # It's a variable.
                        var = symbols[target.id]
                        if (access_path(target) and
                            access_path(target) in constant_size):
                            # This is *the* single assignment to this thing.
                            # See if the value's base address should be
                            # cached.
                            type = val.type
                            if (access_path(stmt.value) and
                                access_path(stmt.value) in symbols):
                                type = symbols[access_path(stmt.value)].type
                            base_address(val, target, type)
                        if isinstance(var, Local):
                            if (is_jit_number_type(var.type) and
                                    not is_jit_number(val)):
                                val = func.unbox_value(val, var.type)
                            elif (not is_jit_number_type(var.type) and
                                    is_jit_number(val)):
                                val = func.box_value(val)
                            if not isinstance(var.type, jit.Type):
                                Py_DecRef(func, var.location)
                            func.store(var.location, val)
                        else:
                            raise CompileError('bad assignment target', target)
                    elif isinstance(target, AST.Subscript):
                        if target.slice.__class__ == AST.Slice:
                            obj = compile_expr(target.value)
                            slc = compile_slice(target.slice)
                            PyObject_SetItem(func, obj, slc, val)
                            Py_DecRef(func, slc)
                            Py_DecRef(func, val)
                            return
                        base, index = compile_expr(target)
                        if base.type.is_array or is_list_access(target.value):
                            container_store(base, index, val, target.value)
                        else:
                            raise CompileError('general store stuff')
                    else:
                        raise CompileError('bad assignment target',
                                           stmt.targets[0])
                else:
                    # single-element list containing tuple containing list
                    target_exprs = stmt.targets[0].elts
                    # Allocate temporaries.
                    targets = [symbols[t.id] if isinstance(t, AST.Name)
                               else compile_expr(t)
                               for t in target_exprs]
                    types = (t.type if isinstance(t, Local)
                             else container_base_type(None, t[0])  # hack
                             for t in targets)
                    temps = [func.new_value(t) for t in types]
                    # Generate assignments to temps
                    for tmp, val in zip(temps, stmt.value.elts):
                        func.store(tmp, compile_expr(val))
                    # Generate assignments from temps to targets
                    for t, e, tmp in zip(targets, target_exprs, temps):
                        if isinstance(t, tuple):
                            base, index = t
                            container_store(base, index, tmp, e.value)
                        else:
                            func.store(t.location, tmp)
            elif isinstance(stmt, AST.AugAssign):
                if verbose:
                    print(AST.dump(stmt))
                lhs = copy.copy(stmt.target)
                lhs.ctx = AST.Load()
                assignment = AST.Assign(targets=[stmt.target],
                                        value=AST.BinOp(left=lhs,
                                                        op=stmt.op,
                                                        right=stmt.value))
                if verbose:
                    print(AST.dump(assignment))
                compile_stmt(assignment)
            elif isinstance(stmt, AST.Expr):
                if isinstance(stmt.value, AST.Call):
                    compile_expr(stmt.value)
                else:
                    # expression without an effect, presumably docstring
                    if isinstance(stmt.value, AST.Str):
                        pass
                    else:
                        print('statement without effect', AST.dump(stmt))
            elif isinstance(stmt, AST.Pass):
                pass
            else:
                raise CompileError(AST.dump(stmt))

        # set up variables
        for name, var in symbols.items():
            if isinstance(var, Argument):
                # Turn Arguments into Locals
                val = func.get_param(var.idx)
                if isinstance(var.type, jit.Type):
                    typ = var.type
                else:
                    typ = jit.Type.void_ptr
                symbols[name] = Local(typ, func.new_value(typ, val),
                                      is_argument=True)
                if name in constant_size:
                    # Force precomputation of base_address.
                    base_address(symbols[name].location,
                                 AST.Name(name, AST.Load()))
            elif isinstance(var, Local):
                if isinstance(var.type, jit.Type):
                    var.location = func.new_value(var.type)
                else:
                    var.location = func.new_value(jit.Type.void_ptr)
                    null = func.make_null_ptr()
                    if name in freevars:
                        func.store(var.location, PyCell_New(func, null))
                    else:
                        func.store(var.location, null)
        # cache boxed constants
        for c in function.__code__.co_consts:
            if c is not None and isinstance(c, collections.Hashable):
                if isinstance(c, int) and c < 0:
                    # libjit isn't good at this signed-unsigned stuff, need
                    # to fix this up via the largest integer.
                    s = jit.Type.int.size * 8
                    c = 2**s + c
                const = func.new_constant(obj_ptr(c), jit.Type.void_ptr)
                func.boxed_constants[c] = const

        try:
            compile_stmt(function_def.body)
        except:
            print(AST.dump(function_def))
            func.dump()
            raise

    return function_builder

def ast_compiler(*, return_type, argument_types=[], variables={},
                 global_vars={}, constant_size=set(), intrinsics=set(),
                 verbose=False):
    def function_compiler(function):
        ast = get_ast(function)
        if verbose:
            print(AST.dump(ast))
        code = compiler(ast, function, return_type, argument_types,
                        variables, global_vars, constant_size, intrinsics,
                        verbose)
        setattr(code, '__original_function__', function)
        module = function.__module__
        name = function.__name__
        assert name not in function_table[module]
        function_table[module][name] = code
        return code

    return function_compiler
