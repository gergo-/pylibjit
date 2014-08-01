import collections
from contextlib import contextmanager
import ctypes
import inspect
import jit
import traceback

from .settings import *

builtin_type = type

pylib = ctypes.cdll.LoadLibrary(None)

class PythonCall:
    def __init__(self, function, return_type, argument_types):
        self.name = function.__name__
        self.target = ctypes.cast(function, ctypes.c_void_p).value
        self.argument_types = argument_types
        self.sig = jit.Type.create_signature(jit.abi.cdecl,
                                             return_type, argument_types)
        # Store this function in the global namespace.
        assert self.name not in self.__init__.__globals__
        self.__init__.__globals__[self.name] = self

    def __call__(self, func, *args):
        args = list(args)
        assert len(args) == len(self.argument_types)
        return func.insn_call_native(self.name, self.target, self.sig, args)

PythonCall(pylib.PyObject_Call, jit.Type.void_ptr, [jit.Type.void_ptr] * 3)
PythonCall(pylib.PyObject_GetItem, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyObject_SetItem, jit.Type.int, [jit.Type.void_ptr] * 3)
PythonCall(pylib.PyObject_Size, jit.Type.int, [jit.Type.void_ptr])
PythonCall(pylib.PyObject_IsInstance, jit.Type.int, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyObject_GetIter, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(pylib.PyObject_GC_UnTrack, jit.Type.void, [jit.Type.void_ptr])
PythonCall(pylib.PyCell_New, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(pylib.PyCell_Set, jit.Type.int, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyCell_Get, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(pylib.PyDict_New, jit.Type.void_ptr, [])
PythonCall(pylib.PyDict_GetItem, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyDict_SetItem, jit.Type.int, [jit.Type.void_ptr] * 3)
PythonCall(pylib.Py_IncRef, jit.Type.void, [jit.Type.void_ptr])
PythonCall(pylib.Py_DecRef, jit.Type.void, [jit.Type.void_ptr])
PythonCall(pylib._Py_Dealloc, jit.Type.void, [jit.Type.void_ptr])
PythonCall(pylib.PyLong_AsLong, jit.Type.int, [jit.Type.void_ptr])
PythonCall(pylib.PyLong_FromLong, jit.Type.void_ptr, [jit.Type.int])
PythonCall(pylib.PyBool_FromLong, jit.Type.void_ptr, [jit.Type.int])
PythonCall(pylib.PyFloat_AsDouble, jit.Type.float64, [jit.Type.void_ptr])
PythonCall(pylib.PyFloat_FromDouble, jit.Type.void_ptr, [jit.Type.float64])
PythonCall(pylib.PyTuple_New, jit.Type.void_ptr, [jit.Type.int])
PythonCall(pylib.PyTuple_SetItem, jit.Type.void,
           [jit.Type.void_ptr, jit.Type.int, jit.Type.void_ptr])
PythonCall(pylib.PyTuple_GetItem, jit.Type.void_ptr,
           [jit.Type.void_ptr, jit.Type.int])
PythonCall(pylib.PySlice_New, jit.Type.void_ptr, [jit.Type.void_ptr] * 3)
PythonCall(pylib.PyList_New, jit.Type.void_ptr, [jit.Type.int])
PythonCall(pylib.PyList_SetItem, jit.Type.void,
           [jit.Type.void_ptr, jit.Type.int, jit.Type.void_ptr])
PythonCall(pylib.PyObject_GetAttr, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyObject_SetAttr, jit.Type.int, [jit.Type.void_ptr] * 3)
PythonCall(pylib.PyObject_IsTrue, jit.Type.int, [jit.Type.void_ptr])
PythonCall(pylib.PyObject_RichCompareBool,
           jit.Type.int, [jit.Type.void_ptr] * 2 + [jit.Type.int])
PythonCall(pylib.PyFunction_New, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyFunction_SetClosure, jit.Type.int, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyFunction_SetDefaults, jit.Type.int, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PySequence_Contains, jit.Type.int, [jit.Type.void_ptr] * 2)
PythonCall(pylib._PyDict_NewPresized, jit.Type.void_ptr, [jit.Type.int])
PythonCall(pylib.PyThreadState_Get, jit.Type.void_ptr, [])
PythonCall(pylib.PyFrame_New, jit.Type.void_ptr, [jit.Type.void_ptr] * 4)

for f in [pylib.PyNumber_Add, pylib.PyNumber_Subtract, pylib.PyNumber_Multiply,
          pylib.PyNumber_Remainder, pylib.PyNumber_Lshift,
          pylib.PyNumber_Rshift, pylib.PyNumber_Or, pylib.PyNumber_Xor,
          pylib.PyNumber_And, pylib.PyNumber_FloorDivide,
          pylib.PyNumber_TrueDivide, pylib.PyNumber_Divmod]:
    PythonCall(f, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(pylib.PyNumber_Negative, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(pylib.PyNumber_Power, jit.Type.void_ptr, [jit.Type.void_ptr] * 3)

PythonCall(pylib.PyErr_Occurred, jit.Type.void_ptr, [])

import site
import distutils.sysconfig
libname = 'jit' + distutils.sysconfig.get_config_var('SO')
try:
    libpath = site.getsitepackages()[0]
    jitlib = ctypes.cdll.LoadLibrary(libpath + '/' + libname)
except:
    try:
        libuserpath = site.getusersitepackages()
        jitlib = ctypes.cdll.LoadLibrary(libuserpath + '/' + libname)
    except:
        print("*** can't find {0}\n*** in {1}\n*** or {2}, giving up\n"
                .format(libname, libpath, libuserpath))
        raise

# libpath = '/usr/local/lib/python3.2/dist-packages/'
# jitlib = ctypes.cdll.LoadLibrary(libpath + '/' + 'jit.cpython-32mu.so')
# jitlib = ctypes.cdll.LoadLibrary(libpath + 'jit.cpython-32dmu.so')
# libpath = '/home/gergo/mython/lib/python3.2/site-packages/'
# jitlib = ctypes.cdll.LoadLibrary(libpath + 'jit.cpython-32dm.so')

PythonCall(jitlib.jitlib_nop, jit.Type.void, [])
PythonCall(jitlib.jitlib_nop_1, jit.Type.void, [jit.Type.void_ptr])
PythonCall(jitlib.jitlib_nop_2, jit.Type.void, [jit.Type.void_ptr] * 2)
PythonCall(jitlib.jit_binaryfunc, jit.Type.void_ptr, [jit.Type.void_ptr] * 2)
PythonCall(jitlib.get_self_arg, jit.Type.void_ptr, [jit.Type.void_ptr])

py_int_binaryfunc = jitlib.py_int_binaryfunc
py_int_binaryfunc.argtypes = (ctypes.c_int,)
py_int_binaryfunc.restype = ctypes.c_void_p

py_float_binaryfunc = jitlib.py_float_binaryfunc
py_float_binaryfunc.argtypes = (ctypes.c_int,)
py_float_binaryfunc.restype = ctypes.c_void_p

obj_ptr = jitlib.obj_ptr
obj_ptr.argtypes = (ctypes.py_object,)
obj_ptr.restype = ctypes.c_void_p

set_wrapper = jitlib.set_wrapper
set_wrapper.argtypes = (ctypes.py_object, ctypes.py_object)
# set_wrapper.restype = void

def some_function():
    pass
some_function_type = obj_ptr(type(some_function))
class SomeClass:
    def some_method():
        pass
some_method_type = obj_ptr(type(SomeClass().some_method))

PyFunction_Type = obj_ptr(pylib.PyFunction_Type)
def PyFunction_Check(func, op):
  # marker_int(func, func.new_constant(-2))
  # obj_printer(func, func.new_constant(some_function_type, jit.Type.void_ptr))
  # marker_int(func, func.new_constant(-1))
  # obj_printer(func, op)
    type_offset = func.new_constant(jit_ob_type_offset())
    type = func.insn_load_elem(op, type_offset, jit.Type.void_ptr)
  # function_type = func.new_constant(PyFunction_Type, jit.Type.void_ptr)
  # marker_int(func, func.new_constant(0))
  # obj_printer(func, type)
  # marker_int(func, func.new_constant(1))
  # obj_printer(func, function_type)
  # return type == function_type
    return type == func.new_constant(some_function_type, jit.Type.void_ptr)

def PyMethod_Check(func, op):
    type_offset = func.new_constant(jit_ob_type_offset())
    type = func.insn_load_elem(op, type_offset, jit.Type.void_ptr)
  # obj_printer(func, type)
    method_const = func.new_constant(some_method_type, jit.Type.void_ptr)
  # obj_printer(func, method_const)
  # obj_printer(func, func.new_constant(some_function_type, jit.Type.void_ptr))
    result = func.new_value(jit.Type.int)
    with func.branch(type == method_const) as (false, end):
        with func.branch(PyFunction_Check(func, PyMethod_GetFunc(func, op))) \
                as (false2, end2):
            func.store(result, func.new_constant(1))
            func.insn_branch(end)  # outer end!
        # else:
            func.insn_label(false2)
    # else:
        func.insn_label(false)
        func.store(result, func.new_constant(0))
    return result

def PyMethod_GetFunc(func, op):
    func_offset = func.new_constant(jit_im_func_offset())
    im_func = func.insn_load_elem(op, func_offset, jit.Type.void_ptr)
    return im_func

def PyMethod_GetSelf(func, op):
    self_offset = func.new_constant(jit_im_self_offset())
    self = func.insn_load_elem(op, self_offset, jit.Type.void_ptr)
    return self

PythonCall(jitlib.PyBuffer_BasePointer, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(jitlib.PyJIT_iternext, jit.Type.void_ptr, [jit.Type.void_ptr])
PythonCall(jitlib.PyJIT_print_int, jit.Type.void, [jit.Type.int])
PythonCall(jitlib.PyJIT_print_double, jit.Type.void, [jit.Type.float64])
PythonCall(jitlib.marker_int, jit.Type.void, [jit.Type.int])
PythonCall(jitlib.jit_fast_function, jit.Type.void_ptr,
           [jit.Type.void_ptr] * 2 + [jit.Type.int] * 3)

marker = ctypes.cast(jitlib.marker, ctypes.c_void_p).value
marker_sig = jit.Type.create_signature(jit.abi.cdecl, jit.Type.void, [])

PythonCall(jitlib.obj_printer, jit.Type.void, [jit.Type.void_ptr])
# obj_printer = ctypes.cast(jitlib.obj_printer, ctypes.c_void_p).value
# obj_printer_sig = jit.Type.create_signature(jit.abi.cdecl, jit.Type.void, [jit.Type.void_ptr])

obj_printer_ct = jitlib.obj_printer
obj_printer_ct.argtypes = (ctypes.py_object,)
# obj_printer_ct.restype = ctypes.void

Py_IncRef_func = Py_IncRef
Py_DecRef_func = Py_DecRef

jit_ob_refcnt_offset = jitlib.jit_ob_refcnt_offset
jit_ob_refcnt_offset.argtypes = ()
jit_ob_refcnt_offset.restype = ctypes.c_int
jit_ob_type_offset = jitlib.jit_ob_type_offset
jit_ob_type_offset.argtypes = ()
jit_ob_type_offset.restype = ctypes.c_int
jit_tuple_ob_item_offset = jitlib.jit_tuple_ob_item_offset
jit_tuple_ob_item_offset.argtypes = ()
jit_tuple_ob_item_offset.restype = ctypes.c_int
jit_tp_call_offset = jitlib.jit_tp_call_offset
jit_tp_call_offset.argtypes = ()
jit_tp_call_offset.restype = ctypes.c_int
jit_im_func_offset = jitlib.jit_im_func_offset
jit_im_func_offset.argtypes = ()
jit_im_func_offset.restype = ctypes.c_int
jit_im_self_offset = jitlib.jit_im_self_offset
jit_im_self_offset.argtypes = ()
jit_im_self_offset.restype = ctypes.c_int
jit_ob_size_offset = jitlib.jit_ob_size_offset
jit_ob_size_offset.argtypes = ()
jit_ob_size_offset.restype = ctypes.c_int
# Initialize these once, they will remain constant.
refcnt_offset = jit_ob_refcnt_offset()
type_offset = jit_ob_type_offset()
tuple_item_offset = jit_tuple_ob_item_offset()
call_offset = jit_tp_call_offset()

InitializeArrayOffset = jitlib.InitializeArrayOffset
InitializeArrayOffset.argtypes = ()
InitializeArrayOffset()

def Py_IncRef(func, value):
    if inline_rc:
        # Note that this offset must be in multiples of the size of int;
        # jit_ob_refcnt_offset ensures that this is the case. It is *not* an
        # offset in bytes.
        offset = func.new_constant(refcnt_offset, jit.Type.int)
        one = func.new_constant(1, jit.Type.int)
        refcnt = func.insn_load_elem(value, offset, jit.Type.int)
        func.insn_store_elem(value, offset, refcnt + one)
    else:
        Py_IncRef_func(func, value)

def Py_DecRef(func, value):
    if inline_rc:
        nullptr = func.new_constant(0, jit.Type.void_ptr)
        # Note that this offset must be in multiples of the size of int;
        # jit_ob_refcnt_offset ensures that this is the case. It is *not* an
        # offset in bytes.
        offset = func.new_constant(refcnt_offset, jit.Type.int)
        zero = func.new_constant(0, jit.Type.int)
        one = func.new_constant(1, jit.Type.int)
        with func.branch(value != nullptr) as (false, end):
            refcnt = func.insn_load_elem(value, offset, jit.Type.int) - one
            func.insn_store_elem(value, offset, refcnt)
            with func.branch(refcnt == zero) as (refcnt_nonzero, inner_end):
                _Py_Dealloc(func, value)
            # else:
                func.insn_label(refcnt_nonzero)
        # else:
            func.insn_label(false)
    else:
        Py_DecRef_func(func, value)

def py_obj_printer(func, obj):
    args = [func.new_constant(obj_ptr(obj), jit.Type.void_ptr)]
    args = func.make_arg_tuple(args)
    printer = func.new_constant(obj_ptr(print), jit.Type.void_ptr)
    kws = func.make_null_ptr()
    print(args)
    PyObject_Call(func, printer, args, kws)

class CompileError(Exception):
    pass

def insert_marker_call(func):
    func.insn_call_native('marker', marker, marker_sig, [])

def is_jit_number_type(type):
    return (type in [jit.Type.int, jit.Type.float64, jit.Type.ubyte,
                     # uint is needed because ubyte is internally a variant
                     # of it, or something. libjit's type system is a mess.
                     jit.Type.uint])

def is_jit_number(obj):
  # if hasattr(obj, 'type'):
  #     obj.type.name()
  #     print('number type?', is_jit_number_type(obj.type))
    return hasattr(obj, 'type') and is_jit_number_type(obj.type)

def is_array(obj):
    try:
        return obj.type.is_array
    except:
        return False

def is_list(obj, type):
    if type is None:
        type = obj.type
    print('is list?', obj, isinstance(type, list))
    return isinstance(type, list)

def is_tuple(obj):
    try:
        return obj.type.is_tuple
    except:
        return False

function_table = collections.defaultdict(dict)

Argument = collections.namedtuple('Argument', ['type', 'idx'])
class Local:
    def __init__(self, type, location, is_argument=False):
        self.type = type
        self.location = location
        self.is_argument = is_argument
class Constant:
    def __init__(self, type, value=None):
        self.type = type
        self.value = value

BinOp = collections.namedtuple('BinOp', ['unboxed_op', 'boxed_op'])

class Function(jit.Function):
    '''Wrapper class for jit.Function with some utilities.'''
    def __init__(self, builder, return_type, argument_types, context):
        super().__init__(context)
        self.__builder__ = builder
        self.return_type = return_type
        self.argument_types = argument_types
        self.loop_stack = []
        self.boxed_constants = dict()
        self.create()
        self.build()

    def create_signature(self):
        argtypes = []
        for a in self.argument_types:
            if isinstance(a, jit.Type):
                argtypes.append(a)
          # elif isinstance(a, tuple):
          #     array_type, base_type = a
          #     argtypes.append(array_type)
            else:
                argtypes.append(jit.Type.void_ptr)

        ret = self.return_type
        if not isinstance(ret, jit.Type):
            ret = jit.Type.void_ptr
        signature = [ret] + list(argtypes)
        return self.signature_helper(*signature)

    @contextmanager
    def branch(self, cond):
        false_label = self.new_label()
        end_label = self.new_label()
        self.insn_branch_if_not(cond, false_label)
        yield false_label, end_label
        self.insn_label(end_label)

    @contextmanager
    def loop(self, cond_label, cond):
        end_label = self.new_label()
        self.insn_branch_if_not(cond, end_label)
        self.loop_stack.append(LoopDescriptor(end_label, cond_label))
        yield
        self.loop_stack.pop()
        self.insn_branch(cond_label)
        self.insn_label(end_label)

    @contextmanager
    def loop_with_else(self, cond_label, cond, end_label):
        else_label = self.new_label()
        self.insn_branch_if_not(cond, else_label)
        self.loop_stack.append(LoopDescriptor(end_label, cond_label))
        yield else_label
        self.loop_stack.pop()
        self.insn_label(end_label)

    @contextmanager
    def for_loop(self, iteration_space):
        start, stop, step = range_parameters(iteration_space)
        ascending = (start <= stop)
        stop, step = self.new_constant(stop), self.new_constant(step)
        counter = self.new_value(jit.Type.int, start)
        with self.loop(self.loop_start(),
                       counter < stop if ascending else counter > stop):
            yield counter
            self.store(counter, counter + step)

    def loop_start(self):
        loop_start = self.new_label()
        self.insn_label(loop_start)
        return loop_start

    def new_value(self, type, init=None):
        value = super().new_value(type)
        if init is not None:
            self.store(value, self.make_value(init))
        return value

    def make_value(self, value):
        if isinstance(value, jit.Value):
            return value
        else:
            return self.new_constant(value)

    def make_null_ptr(self):
        return self.new_constant(0, jit.Type.void_ptr)

    def make_arg_tuple(self, args):
      # print('make_arg_tuple', args)
        t = PyTuple_New(self, self.new_constant(len(args)))
        if args:
            # base = PyArray_AsPointer(self, t)
            base = t + self.new_constant(tuple_item_offset)
        for i, a in enumerate(args):
            a = self.box_value(a)
          # if len(args) == 1:
          #     obj_printer(self, a)
          # PyTuple_SetItem(self, t, self.new_constant(i), a)
          # if reference_counting:
          #     Py_IncRef(self, a)
            self.insn_store_elem(base, self.new_constant(i), a)
        return t

    def zero_arg_tuple(self, args, size):
        if size:
            # base = PyArray_AsPointer(self, args)
            base = args + self.new_constant(tuple_item_offset)
            zero = self.new_constant(0, jit.Type.void_ptr)
            for i in range(size):
                self.insn_store_elem(base, self.new_constant(i), zero)

    def call_py_object(self, target, args, kwargs, scratch_space, name=None,
                       decref_args=True):
        if isinstance(target, str):
            # Builtins are specified as strings. Look up and make address.
            target = self.new_constant(obj_ptr(__builtins__[target]))
        if not isinstance(target, jit.Value):
            target = self.new_value(jit.Type.void_ptr, target)
        # Inlined handling of fast calls if there are no keyword arguments.
        if False and not kwargs:
            result = self.new_value(jit.Type.void_ptr)
            with self.branch(PyMethod_Check(self, target)) as (false, end):
                # unpack the method
                meth_self = PyMethod_GetSelf(self, target)
                Py_IncRef(self, meth_self)
                meth_func = PyMethod_GetFunc(self, target)
                Py_IncRef(self, meth_func)
                # same as below; this causes some code bloat                
                margs = [meth_self] + args                
                boxed_args = []
                # call jit_fast_function with a stacky representation
                for i, a in enumerate(margs):
                    boxed = self.box_value(a)
                    boxed_args.append(boxed)
                    self.insn_store_elem(scratch_space, self.new_constant(i),
                                         boxed)
                na = self.new_constant(len(margs))
                nk = self.new_constant(len(kwargs) if kwargs else 0)
                n = na + self.new_constant(2) * nk
                pp_stack = self.insn_address_of(scratch_space)
                call = jit_fast_function(self, meth_func, pp_stack, n, na, nk)
                self.store(result, call)
                for a in boxed_args:
                    Py_DecRef(self, a)
                Py_DecRef(self, meth_func)  # ?
                self.insn_branch(end)
            # else:
                self.insn_label(false)
                ## TODO: refactor, allow static type annotations
                with self.branch(PyFunction_Check(self, target)) as \
                        (false2, end2):
                    # call jit_fast_function with a stacky representation
                    boxed_args = []
                    for i, a in enumerate(args):
                        boxed = self.box_value(a)
                        boxed_args.append(boxed)
                        self.insn_store_elem(scratch_space,
                                             self.new_constant(i),
                                             boxed)
                    na = self.new_constant(len(args))
                    nk = self.new_constant(len(kwargs) if kwargs else 0)
                    n = na + self.new_constant(2) * nk
                    pp_stack = self.insn_address_of(scratch_space)
                  # obj_printer(self, target)
                  # for i, a in enumerate(boxed_args):
                  #     marker_int(self, self.new_constant(i))
                  #     obj_printer(self, a)
                    call = jit_fast_function(self, target, pp_stack, n, na, nk)
                  # marker_int(self, self.new_constant(-42))
                  # obj_printer(self, call)
                    self.store(result, call)
                    for a in boxed_args:
                        Py_DecRef(self, a)
                    self.insn_branch(end2)
                # else:
                    self.insn_label(false2)
                  # obj_printer(self, target)
                  # self.insn_return(self.new_constant(0, jit.Type.void))
                    call = self.call_py_object_general(target,args,kwargs,name)
                    self.store(result, call)
            return result
        else:
            return self.call_py_object_general(target, args, kwargs, name,
                                               decref_args)

    def call_py_object_general(self, target, args, kwargs=None, name=None,
                               decref_args=True):
        args0 = args
        args = self.make_arg_tuple(args)
        if kwargs is None or kwargs == {}:
            kws = self.make_null_ptr()
        else:
            kws = PyDict_New(self)
            for key, value in kwargs.items():
                key = self.new_constant(obj_ptr(key), jit.Type.void_ptr)
                PyDict_SetItem(self, kws, key, self.box_stack_entry(value))
                Py_DecRef(self, key)
                Py_DecRef(self, value.boxed_value)
        try:
        #   type_offset = self.new_constant(jit_ob_type_offset())
        #   call = self.insn_load_elem(target, type_offset, jit.Type.void_ptr)
        #   call_offset = self.new_constant(jit_tp_call_offset())
        #   call = self.insn_load_elem(call, call_offset, jit.Type.void_ptr)
        #   result = self.insn_call_indirect(call, PyObject_Call.sig,
        #                                    [target, args, kws])
            result = PyObject_Call(self, target, args, kws)
            null = self.make_null_ptr()
            with self.branch(result == null) as (false_label, end_label):
              # name = self.new_constant(obj_ptr(name), jit.Type.void_ptr)
              # obj_printer(self, name)
                self.insn_return(result)
            # else:
                self.insn_label(false_label)
        except:
            print('PyObject_Call failed with target', target, 'args', args)
            raise
        # This decref frees the tuple (we own it) and decrefs all the tuple
        # elements.
        if not reference_counting and not decref_args:
            # Ah, but don't actually decref pointers we didn't incref before
            # because we try to avoid redundant refcount operations.
            self.zero_arg_tuple(args, len(args0))
        Py_DecRef(self, args)
        if kwargs is not None and kwargs != {}:
            Py_DecRef(self, kws)
        return result

    def compute_decref_args(self, original_args):
        decref_args = False
        # If we try to do optimized reference counting, we must still ensure
        # that we own references to these args. We should decref arguments
        # that have freshly_allocated set (which really means that someone
        # incremented a refcount for us) and arguments that are not boxed
        # yet; they will be boxed, which will produce an object that we
        # should decref later.
        fresh_args = len([a for a in original_args
                          if a.freshly_allocated or a.boxed_value is None])
        if fresh_args > 0:
            # Incref non-fresh args and make sure to later decref
            # them with the argument tuple.
            decref_args = True
            for a in original_args:
                if not a.freshly_allocated and a.boxed_value is not None:
                    Py_IncRef(self, a.boxed_value)
        return decref_args

    def box_value(self, value):
        if hasattr(value, 'is_constant') and value.is_constant:
            if value.type == jit.Type.int:
                value = value.nint_constant()
            elif value.type == jit.Type.float64:
                value = value.float64_constant()
            if isinstance(value, collections.Hashable):
                value = self.boxed_constants[value]
                # This constant is about to be used. We must incref it (most
                # of the time, at least).
                Py_IncRef(self, value)
                return value
        if hasattr(value, 'type'):
            if (value.type == jit.Type.int or value.type == jit.Type.ubyte or
                value.type == jit.Type.uint):
                return PyLong_FromLong(self, value)
            elif value.type == jit.Type.float64:
                return PyFloat_FromDouble(self, value)
            elif value.type == jit.Type.void_ptr or value.type.is_array:
                return value
            else:
                value.type.name()
                raise CompileError('unexpected type for boxing')
        else:
            print('no type? weird:', value)
            assert value is not None
            return value

    def box_stack_entry(self, stack_entry, allow_none=False):
      # print('** box this thing:', stack_entry)
        if not allow_none:
            assert stack_entry is not None
        if stack_entry is not None:
            if stack_entry.boxed_value is not None:
              # ptr = self.new_constant(obj_ptr(stack_entry.boxed_value))
                if stack_entry.constant is not None:
                    if not reference_counting and stack_entry.refcount <= 2:
                        count = len([x for x in self.stack if x is stack_entry])
                        for i in range(count + 1):
                            Py_IncRef(self, stack_entry.constant)
                            stack_entry.refcount += 1
                ptr = stack_entry.boxed_value
                return ptr
            elif stack_entry.value is not None:
                if is_jit_number(stack_entry.value):
                    stack_entry.freshly_allocated = True
                else:
                    print('not a number type?')
              # stack_entry.type.name()
                stack_entry.boxed_value = self.box_value(stack_entry.value)
                if stack_entry.refcount is None or stack_entry.refcount <= 1:
                    stack_entry.refcount = 1
                    # Adjust refcounts (both static and dynamic) for other
                    # instances of this object on the stack. The stack entry
                    # may have been DUP'd.
                    count = len([x for x in self.stack if x is stack_entry])
                    for i in range(count-1):
                        Py_IncRef(self, stack_entry.boxed_value)
                        stack_entry.refcount += 1
                stack_entry.freshly_allocated = True
                return stack_entry.boxed_value
            elif stack_entry.builtin is not None:
                ptr = self.new_constant(obj_ptr(stack_entry.builtin))
                Py_IncRef(self, ptr)
                stack_entry.boxed_value = ptr
                return stack_entry.boxed_value
            else:
                # We are asked to box the None object, I think.
                assert stack_entry.name == 'None'
                return self.new_constant(obj_ptr(None))
        return self.make_null_ptr()

    def unbox_value(self, value, type, decref=True):
        if (type == jit.Type.int or type == jit.Type.ubyte or
            type == jit.Type.uint):
            result = PyLong_AsLong(self, value)
            if decref:
                Py_DecRef(self, value)
            return result
        elif type == jit.Type.float64:
            result = PyFloat_AsDouble(self, value)
            if decref:
                Py_DecRef(self, value)
            return result
        else:
            return value

    def recursive_call(self, name, args):
        return self.insn_call(name, self, self.create_signature(), args)

    def recursive_call_target(self):
        return self

def builder(*, return_type, argument_types,
            context=jit.Context(), dump_code=False):
    '''This function decorator takes a build function and uses it to build a
    JIT function in an anonymous context.'''
    class build(Function):
        def build(self):
            self.__builder__(self)
            if dump_code:
                self.dump()
            self.compile()
            if dump_code:
                self.dump()

    def function_builder(builder):
        function = build(builder, return_type, argument_types, context)
        function.__name__ = builder.__name__
        return function

    return function_builder

