license = '''/*
 *
 * Copyright (c) 2012 Dan Eicher
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
'''
import os
import sys
import warnings

sys.path.append('/usr/local/lib/python2.7/site-packages/')

from pybindgen import (Module, FileCodeSink, param, retval, cppclass,
                       CppClass, typehandlers, ReturnValue, Parameter)
from pybindgen.module import SubModule, MultiSectionFactory
from pybindgen.typehandlers.base import PointerReturnValue

import pybindgen.settings


class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True


pybindgen.settings.error_handler = ErrorHandler()
pybindgen.settings.deprecated_virtuals = False
pybindgen.settings.min_python_version=(3, 0)

class JitMultiSectionFactory(MultiSectionFactory):

    def __init__(self, main_file_name):
        self.main_file_name = main_file_name
        self.main_sink = FileCodeSink(open('src/'+main_file_name, "wt"))
        self.header_name = "jit_module.h"
        header_file_name = os.path.join(os.path.dirname(self.main_file_name), self.header_name)
        self.header_sink = FileCodeSink(open('include/'+header_file_name, "wt"))
        self.section_sinks = {}

        self.main_sink.writeln(license)
        self.header_sink.writeln(license)

    def get_section_code_sink(self, section_name):
        if section_name == '__main__':
            return self.main_sink
        try:
            return self.section_sinks[section_name]
        except KeyError:
            file_name = os.path.join(os.path.dirname(self.main_file_name), "src/%s.cc" % section_name)
            sink = FileCodeSink(open(file_name, "wt"))
            self.section_sinks[section_name] = sink
            sink.writeln(license)
            return sink

    def get_main_code_sink(self):
        return self.main_sink

    def get_common_header_code_sink(self):
        return self.header_sink

    def get_common_header_include(self):
        return '"%s"' % self.header_name

    def close(self):
        self.header_sink.file.close()
        self.main_sink.file.close()
        for sink in self.section_sinks.itervalues():
            sink.file.close()


class VoidPtrParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN, Parameter.DIRECTION_INOUT, Parameter.DIRECTION_OUT]
    CTYPES = ['void *']

    def convert_python_to_c(self, wrapper):
        name = wrapper.declarations.declare_variable("PyObject *", self.name)
        wrapper.call_params.append('(%s)PyLong_AsVoidPtr(%s)' % (self.ctype, name))
        if self.direction & self.DIRECTION_IN:
            wrapper.parse_params.add_parameter('O!', ['&PyLong_Type', '&'+name], self.name)
        if self.direction & self.DIRECTION_OUT:
            wrapper.build_params.add_parameter('N', ["PyLong_FromVoidPtr((void *)%s)" % name])

class VoidPtrReturn(ReturnValue):
    CTYPES = ['void *']

    def get_c_error_return(self):
        return "return NULL;"

    def convert_python_to_c(self, wrapper):
        print self.ctype
        raise NotImplementedError

    def convert_c_to_python(self, wrapper):
        wrapper.build_params.add_parameter('N', ["PyLong_FromVoidPtr((void *)%s)" % self.value], prepend=True)


class PyBytesParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = []

    def convert_python_to_c(self, wrapper):
        name = wrapper.declarations.declare_variable("PyObject *", self.name)
        wrapper.call_params.append('(%s)PyBytes_AsString(%s)' % (self.ctype, name))
        wrapper.call_params.append('PyBytes_Size(%s)' % (name))
        wrapper.parse_params.add_parameter('O!', ['&PyBytes_Type', '&'+name], self.name)

class PyBytesReturn(ReturnValue):
    CTYPES = []

    def convert_c_to_python(self, wrapper):
        size = wrapper.declarations.declare_variable("jit_nuint", "size")
        wrapper.build_params.add_parameter('N', ["PyBytes_FromStringAndSize((const char *)%s, size)" % (self.value)], prepend=True)


class PyBytesSizeParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = []

    def convert_python_to_c(self, wrapper):
        wrapper.call_params.append('&size')


class JitPtrParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = ['jit_type_t', 'jit_pycontext_t', 'jit_function_t']

    def convert_python_to_c(self, wrapper):
        raise NotImplementedError

    def convert_python_to_c(self, wrapper):
        py_type = "PyJit_"+(self.ctype.split('_')[1])
        name = wrapper.declarations.declare_variable("%s *" % (py_type), "py_"+self.name, 'NULL')
        wrapper.parse_params.add_parameter('O!', ['&%s_Type' % (py_type), '&'+name],
                                           self.name, optional=bool(self.default_value))
        if self.default_value:
            wrapper.call_params.append('%s ? %s->obj->raw() : %s' % (name, name, self.default_value))
        else:
            wrapper.call_params.append('%s->obj->raw()' % (name))


class CStringListParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = ['char **']
    
    def convert_python_to_c(self, wrapper):
        py_list = wrapper.declarations.declare_variable("PyObject *", "py_list")
        name = wrapper.declarations.declare_variable('char **', self.name)
        elem = wrapper.declarations.declare_variable("PyObject *", "elem")
        idx = wrapper.declarations.declare_variable("int", "idx")
        n_params = wrapper.declarations.declare_variable("int", "num_names")
        wrapper.parse_params.add_parameter('O!', ['&PyList_Type', '&'+py_list], self.name)
        wrapper.before_call.write_code('%s = PyList_Size(%s);' % (n_params, py_list))
        wrapper.before_call.write_code('%s = (char **)malloc(sizeof(char *) * %s);'
                                        % (name, n_params))
        wrapper.before_call.write_error_check('%s == NULL' % (name),
            'PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory");')

        wrapper.before_call.write_code("for (%(idx)s = 0; %(idx)s < %(n_params)s; %(idx)s++) {" % vars())
        wrapper.before_call.indent()
        wrapper.before_call.write_code("%(elem)s = PyList_GetItem(%(py_list)s, %(idx)s);" % vars())
        wrapper.before_call.write_error_check('!PyUnicode_Check(%s)' % (elem),
            'PyErr_SetString(PyExc_TypeError, "Parameter `%s\' must be a list of `string\'");'
            % (self.name))
        wrapper.before_call.write_code("%(name)s[%(idx)s] = PyBytes_AsString(PyUnicode_AsEncodedString((%(elem)s), \"utf-8\", \"Error\"));" % vars())
        wrapper.before_call.unindent()
        wrapper.before_call.write_code('}')

        wrapper.call_params.append(name)
        wrapper.call_params.append(n_params)
        wrapper.after_call.write_code('free(%s);' % (name))

class JitListParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = []

    def convert_python_to_c(self, wrapper):
        py_type = "PyJit_"+(self.ctype.split('_')[1])
        py_list = wrapper.declarations.declare_variable("PyObject *", "py_list")
        name = wrapper.declarations.declare_variable(self.ctype, self.name, array="[32]")
        elem = wrapper.declarations.declare_variable("PyObject *", "elem")
        idx = wrapper.declarations.declare_variable("int", "idx")
        n_params = wrapper.declarations.declare_variable("int", "n_params")
        wrapper.parse_params.add_parameter('O!', ['&PyList_Type', '&'+py_list], self.name)

        wrapper.before_call.write_error_check('(%s = PyList_Size(%s)) > 32' % (n_params, py_list),
            'PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");')

        wrapper.before_call.write_code("for (%(idx)s = 0; %(idx)s < %(n_params)s; %(idx)s++) {" % vars())
        wrapper.before_call.indent()
        wrapper.before_call.write_code("%(elem)s = PyList_GetItem(%(py_list)s, %(idx)s);" % vars())
        wrapper.before_call.write_error_check('!PyObject_IsInstance(%s, (PyObject*) &%s_Type)' % (elem, py_type),
            'PyErr_SetString(PyExc_TypeError, "Parameter `%s\' must be a list of jit.%s types");'
            % (self.name, self.ctype.split('_')[1].capitalize()))
        wrapper.before_call.write_code("%(name)s[%(idx)s] = ((%(py_type)s*)%(elem)s)->obj->raw();" % vars())
        wrapper.before_call.unindent()
        wrapper.before_call.write_code('}')

        wrapper.call_params.append(name)
        wrapper.call_params.append(n_params)

class JitPtrReturn(PointerReturnValue):

    CTYPES = ['jit_type_t', 'jit_pycontext_t', 'jit_block_t']

    def get_c_error_return(self):
        return "return NULL;"

    def convert_python_to_c(self, wrapper):
        type_t = (self.ctype.split('_')[1])
        name = wrapper.declarations.declare_variable("PyJit_%s *" % (type_t), "py_"+type_t)
        wrapper.parse_params.add_parameter('O!', ['&PyJit_%s_Type' % (type_t), '&'+name], "py_"+type_t)
        wrapper.after_call.write_code("retval = %s->obj->raw();" % (name))

    def convert_c_to_python(self, wrapper):
        type_t = (self.ctype.split('_')[1])
        py_type = "PyJit_"+type_t
        py_name = wrapper.declarations.declare_variable(py_type+" *", "py_"+type_t)
        wrapper.after_call.write_code("%s = PyObject_New(%s, &%s_Type);" % (py_name, py_type, py_type))
        if type_t == 'type':
            code = '%s->obj = new jit_%s(%s);'
            wrapper.after_call.write_code(code % (py_name, type_t, self.value))
        else:
            wrapper.after_call.write_code("%s->obj = new jit_%s(%s);" % (py_name, type_t, self.value))
        if self.call_owns_return:
            wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_NONE;" % (py_name))
        else:
            wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;" % (py_name))
        wrapper.build_params.add_parameter("N", [py_name], prepend=True)


class JitStaticAttrReturn(PointerReturnValue):
    CTYPES = []

    def convert_c_to_python(self, wrapper):
        py_name = wrapper.declarations.declare_variable("PyJit_type *", "py_jit_type")
        wrapper.after_call.write_code("%s = PyObject_New(PyJit_type, &PyJit_type_Type);" % (py_name))
        wrapper.after_call.write_code("%s->obj = jit_type::t_%s();" % (py_name, self.ctype))
        wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_NONE;" % (py_name))

        wrapper.build_params.add_parameter("N", [py_name], prepend=True)


class JitFunctionPtrReturn(PointerReturnValue):

    CTYPES = ['jit_function_t']

    def get_c_error_return(self):
        return "return NULL;"

    def convert_c_to_python(self, wrapper):
        py_name = wrapper.declarations.declare_variable("PyJit_function *", "py_func")
        func_t = wrapper.declarations.declare_variable("jit_function *", "func_t")
        wrapper.after_call.write_code("%s = PyObject_New(PyJit_function, &PyJit_function_Type);" % (py_name))
        wrapper.after_call.write_code("%s = jit_function::from_raw(%s);" % (func_t, self.value))
        wrapper.after_call.write_code("if (%s != NULL) {" % (func_t))
        wrapper.after_call.write_code("    %s->obj = new jit_function(*%s);" % (py_name, func_t))
        wrapper.after_call.write_code("} else {")
        wrapper.after_call.write_code("    %s->obj = new PyJit_function__PythonHelper(%s);" % (py_name, self.value))
        wrapper.after_call.write_code("    ((PyJit_function__PythonHelper*) %s->obj)->set_pyobj((PyObject *)%s);" % (py_name, py_name))
        wrapper.after_call.write_code("}")
        if self.call_owns_return:
            wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_NONE;" % (py_name))
        else:
            wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;" % (py_name))
        wrapper.build_params.add_parameter("N", [py_name], prepend=True)


class FunctionListReturn(ReturnValue):
    CTYPES = ['jit_function_t *']

    def get_c_error_return(self):
        return "return NULL;"

    def convert_c_to_python(self, wrapper):
        py_list = wrapper.declarations.declare_variable("PyObject *", "py_list")
        py_func = wrapper.declarations.declare_variable("PyJit_function *", "py_func")
        elem = wrapper.declarations.declare_variable("jit_function_t", "elem", "NULL")
        func_t = wrapper.declarations.declare_variable("jit_function *", "func_t")
        wrapper.after_call.write_code("%s = PyList_New(0);" % (py_list))
        wrapper.after_call.write_code("while ((%s = jit_function_next(self->obj->raw(), %s)) != NULL) {"
                                          % (elem, elem))
        wrapper.after_call.indent()
        wrapper.after_call.write_code("%s = PyObject_New(PyJit_function, &PyJit_function_Type);" % (py_func))
        wrapper.after_call.write_code("%s = jit_function::from_raw(%s);" % (func_t, elem))
        wrapper.after_call.write_code("if (%s != NULL) {" % (func_t))
        wrapper.after_call.write_code("    %s->obj = new jit_function(*%s);" % (py_func, func_t))
        wrapper.after_call.write_code("} else {")
        wrapper.after_call.write_code("    %s->obj = new PyJit_function__PythonHelper(%s);" % (py_func, elem))
        wrapper.after_call.write_code("    ((PyJit_function__PythonHelper*) %s->obj)->set_pyobj((PyObject *)%s);" % (py_func, py_func))
        wrapper.after_call.write_code("}")
        wrapper.after_call.write_code("%s->flags = PYBINDGEN_WRAPPER_FLAG_NONE;" % (py_func))
        wrapper.after_call.write_code("PyList_Append(%s, (PyObject*)%s);" % (py_list, py_func))
        wrapper.after_call.unindent()
        wrapper.after_call.write_code('}')
        wrapper.build_params.add_parameter("N", [py_list], prepend=True)


class FreeFuncParam(Parameter):
    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = ['jit_meta_free_func']
    def convert_python_to_c(self, wrapper):
        wrapper.call_params.append(self.name)


class JitMemoryPolicy(cppclass.FreeFunctionPolicy):
    def get_pointer_type(self, class_full_name):
        return class_full_name+'_t '

    def __repr__(self):
        return 'JitMemoryPolicy(%r)' % self.free_function


def module_init():
    root_module = Module('jit', cpp_namespace='::')

    root_module.add_include('<jit/jit-plus.h>')
    root_module.add_include('<jit/jit.h>')
    root_module.add_include('<jit/jit-dynamic.h>')
    root_module.add_include('"jit-plus-type.h"')
    root_module.add_include('"jit-plus-pycontext.h"')

    return root_module


def register_types(module):
    root_module = module.get_root()

    root_module.add_exception('jit_build_exception', custom_name='BuildError')

    module.begin_section('jit_context')
    cls = module.add_class('jit_pycontext', allow_subclassing=True, custom_name='Context')
    cls.register_alias('jit_context')
    module.end_section('jit_context')

    module.begin_section('jit_function')
    module.add_class('jit_function', allow_subclassing=True, custom_name='Function')
    module.end_section('jit_function')

    module.begin_section('jit_value')
    module.add_class('jit_value', custom_name='Value')
    module.end_section('jit_value')

    module.begin_section('jit_label')
    module.add_class('jit_label', custom_name='Label')
    module.end_section('jit_label')

    module.begin_section('jit_jump_table')
    module.add_class('jit_jump_table', custom_name='JumpTable')
    module.end_section('jit_jump_table')

    module.begin_section('jit_type')
    module.add_class('jit_type',
                     custom_name='Type')
    module.header.writeln("""void _wrap_meta_free_func(void *data);""")
    module.body.writeln("void _wrap_meta_free_func(void *data)\n"
                        "{ Py_XDECREF((PyObject*) data); }\n")
    module.end_section('jit_type')

    module.begin_section('jit_elf')
    sub = SubModule('elf', parent=module)
    sub.add_class('jit_readelf',
                  memory_policy=JitMemoryPolicy('jit_readelf_close'),
                  custom_name='ReadElf')

    sub.add_class('jit_writeelf',
                  memory_policy=JitMemoryPolicy('jit_writeelf_destroy'),
                  custom_name='WriteElf')

    flags = SubModule('READELF_FLAGS', parent=sub)
    flags.add_enum('', [('FLAG_FORCE', 'JIT_READELF_FLAG_FORCE'),
                        ('FLAG_DEBUG', 'JIT_READELF_FLAG_DEBUG')])

    err = SubModule('READELF_ERROR', parent=sub)
    err.add_enum('', [('OK', 'JIT_READELF_OK'),
                      ('CANNOT_OPEN', 'JIT_READELF_CANNOT_OPEN'),
                      ('NOT_ELF', 'JIT_READELF_NOT_ELF'),
                      ('WRONG_ARCH', 'JIT_READELF_WRONG_ARCH'),
                      ('BAD_FORMAT', 'JIT_READELF_BAD_FORMAT'),
                      ('MEMORY', 'JIT_READELF_MEMORY')])
    module.end_section('jit_elf')

    module.begin_section('jit_debugger')
    module.add_class('jit_debugger',
                      memory_policy=JitMemoryPolicy('jit_debugger_destroy'),
                      custom_name='Debugger')
    module.end_section('jit_debugger')

    module.begin_section('jit_dynlib')
    sub = SubModule('dynlib', parent=module)
    sub.add_class('jit_dynlib_handle',
                  memory_policy=JitMemoryPolicy('jit_dynlib_close'),
                  custom_name='DynlibHandle')

    cpp = SubModule('cpp_flags', parent=sub)
    cpp.add_enum('', [('PUBLIC', 'JIT_MANGLE_PUBLIC'),
                      ('PROTECTED', 'JIT_MANGLE_PROTECTED'),
                      ('PRIVATE', 'JIT_MANGLE_PRIVATE'),
                      ('STATIC', 'JIT_MANGLE_STATIC'),
                      ('VIRTUAL', 'JIT_MANGLE_VIRTUAL'),
                      ('CONST', 'JIT_MANGLE_CONST'),
                      ('EXPLICIT_THIS', 'JIT_MANGLE_EXPLICIT_THIS'),
                      ('IS_CTOR', 'JIT_MANGLE_IS_CTOR'),
                      ('IS_DTOR', 'JIT_MANGLE_IS_DTOR'),
                      ('BASE', 'JIT_MANGLE_BASE')])
    module.end_section('jit_dynlib')

    abi = SubModule('abi', parent=module)
    abi.add_enum('jit_abi_t',
                 [('cdecl', 'jit_abi_cdecl'),
                  ('vararg', 'jit_abi_vararg'),
                  ('stdcall', 'jit_abi_stdcall'),
                  ('fastcall', 'jit_abi_fastcall')])

    TYPES = SubModule('TYPES', parent=module)
    TYPES.add_enum('JIT_TYPES',
                   [('INVALID', 'JIT_TYPE_INVALID'),
                    ('VOID', 'JIT_TYPE_VOID'),
                    ('SBYTE', 'JIT_TYPE_SBYTE'),
                    ('UBYTE', 'JIT_TYPE_UBYTE'),
                    ('SHORT', 'JIT_TYPE_SHORT'),
                    ('USHORT', 'JIT_TYPE_USHORT'),
                    ('INT', 'JIT_TYPE_INT'),
                    ('UINT', 'JIT_TYPE_UINT'),
                    ('NINT', 'JIT_TYPE_NINT'),
                    ('NUINT', 'JIT_TYPE_NUINT'),
                    ('LONG', 'JIT_TYPE_LONG'),
                    ('ULONG', 'JIT_TYPE_ULONG'),
                    ('FLOAT32', 'JIT_TYPE_FLOAT32'),
                    ('FLOAT64', 'JIT_TYPE_FLOAT64'),
                    ('NFLOAT', 'JIT_TYPE_NFLOAT'),
                    ('MAX_PRIMITIVE', 'JIT_TYPE_MAX_PRIMITIVE'),
                    ('STRUCT', 'JIT_TYPE_STRUCT'),
                    ('UNION', 'JIT_TYPE_UNION'),
                    ('SIGNATURE', 'JIT_TYPE_SIGNATURE'),
                    ('PTR', 'JIT_TYPE_PTR'),
                    ('FIRST_TAGGED', 'JIT_TYPE_FIRST_TAGGED')])

    typehandlers.add_type_alias('unsigned int', 'jit_label_t')
    typehandlers.add_type_alias('jit_readelf *', 'jit_readelf_t')
    typehandlers.add_type_alias('jit_writeelf *', 'jit_writeelf_t')
    typehandlers.add_type_alias('jit_debugger *', 'jit_debugger_t')

def register_methods(root_module):
    register_Jit_context_methods(root_module, root_module['jit_pycontext'])
    register_Jit_function_methods(root_module, root_module['jit_function'])
    register_Jit_jump_table_methods(root_module, root_module['jit_jump_table'])
    register_Jit_label_methods(root_module, root_module['jit_label'])
    register_Jit_value_methods(root_module, root_module['jit_value'])
    register_Jit_type_methods(root_module, root_module['jit_type'])
    register_Jit_readelf_methods(root_module, root_module['jit_readelf'])
    register_Jit_writeelf_methods(root_module, root_module['jit_writeelf'])
    register_Jit_debugger_methods(root_module, root_module['jit_debugger'])
    register_Jit_dynlib_methods(root_module, root_module['jit_dynlib_handle'])
    return

def register_Jit_context_methods(root_module, cls):
    cls.add_constructor([])

    cls.add_instance_attribute('debugger', 
                               ReturnValue.new('jit_debugger_t',
                                               reference_existing_object=True), 
                               is_const=True,
                               getter='debugger')

    cls.add_instance_attribute('functions', 
                               retval('jit_function_t *'),
                               is_const=True)

    cls.add_method('build_start', 
                   'void', 
                   [])

    cls.add_method('build_end', 
                   'void', 
                   [])

    cls.add_method('resolve_all', 
                   'bool', 
                   [param('bool', 'print_failures', default_value='0')])

    cls.add_method('register_symbol', 
                   'bool', 
                   [param('const char *', 'name'),
                    param('void *', 'value'),
                    param('bool', 'after')])
    return

_wrap_PyJit_function__descr_get__ = r'''
PyObject *get_self_token(void)
{
    static PyObject *self_token = NULL;
    if (self_token == NULL) {
        self_token = _PyObject_New(&PyBaseObject_Type);
    }
    return self_token;
}

static PyObject *
_wrap_PyJit_function__descr_get__(PyJit_function *self, PyObject *obj, PyObject **return_exceptions)
{
    /* Make sure the function's object dictionary contains a pointer to the
     * base object obj (or some other previously set one) at the self token. */
    if (self->inst_dict == NULL) {
        self->inst_dict = PyDict_New();
    }
    PyObject *self_token = get_self_token();
 // printf("self_token: %p %d\n", self_token, self_token->ob_refcnt);
    if (!PyDict_Contains(self->inst_dict, self_token)) {
        PyDict_SetItem(self->inst_dict, self_token, obj);
    } else if (PyDict_GetItem(self->inst_dict, self_token) != obj) {
     // PyObject *old = PyDict_GetItem(self->inst_dict, self_token);
     // printf("old self pointer %p?\n", old);
     // printf("new: %p\n", obj);
        PyDict_SetItem(self->inst_dict, self_token, obj);
     // Py_XDECREF(old);
    }
 // printf("function refcnt: %p %d\n", self, ((PyObject *) self)->ob_refcnt);
    Py_INCREF((PyObject *) self);  /* because we will return it for use */
    return (PyObject *) self;
}
'''

_wrap_PyJit_function__call__ = r'''
extern "C" void obj_printer(PyObject *);

// forward decl, generated definition follows below
PyObject * _wrap_PyJit_function___call__(PyJit_function *self, PyObject *args);

extern "C" void set_wrapper(PyJit_function *target, PyJit_function *wrapper)
{
    void *wrapper_address = wrapper->obj->closure();
    PyTypeObject *type = ((PyObject *) target)->ob_type;
    // Each function we jit has its very own class, so we can overwrite the
    // call slot with a function-specific wrapper. Just to be on the safe
    // side, make sure that the thing we overwrite is the generic wrapper.
    if (type->tp_call != (ternaryfunc) _wrap_PyJit_function___call__) {
        abort();
    }
    // Now actually set the wrapper.
    type->tp_call = (ternaryfunc) wrapper_address;
}

extern "C" PyObject *
get_self_arg(PyJit_function *func)
{
    PyObject *self_token = get_self_token();
    if (func->inst_dict != NULL &&
        PyDict_Contains(func->inst_dict, self_token)) {
        return PyDict_GetItem(func->inst_dict, self_token);
    }
    return NULL;
}

union jit_arg {
    PyObject *a_py_object;
    jit_sbyte   a_sbyte;
    jit_ubyte   a_ubyte;
    jit_short   a_short;
    jit_ushort  a_ushort;
    jit_int     a_int;
    jit_uint    a_uint;
    jit_long    a_long;
    unsigned long a_ulong;
    jit_float32 a_float32;
    jit_float64 a_float64;
    jit_nfloat  a_nfloat;
};

static PyObject *
_wrap_PyJit_function__call__(PyJit_function *self, PyObject *args, PyObject **return_exception)
{
    PyGILState_STATE __py_gil_state;
    PyObject *exc_type, *traceback, *element, *py_retval;
    jit_type_t signature;
 // void *retval, *call_args_array[32]; /* built-in max number of args */
    union jit_arg retval, call_args_array[32];    
 // void **call_args = call_args_array;
    void *call_args_a[32];
    void **call_args = call_args_a;
    int i, n_params;
    for (i = 0; i < 32; i++)
        call_args_a[i] = &call_args_array[i];
    PyObject *objs_to_decref[32];
    unsigned dec = 0;

    __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);

    if (!(signature = self->obj->signature())) {
        PyErr_SetString(PyExc_TypeError, "Function doesn't have a signature");
        return NULL;
    }

 // obj_printer(args);
    n_params = PyTuple_Size(args);
    int extra_arg = 0;
    element = get_self_arg(self);
    if (element) {
        extra_arg = 1;
     // jit_type_t param = jit_type_get_param(signature, 0);
     // call_args[0] = jit_malloc(jit_type_get_size(param));
     // *(void **)call_args[0] = element;
        call_args_array[0].a_py_object = element;
     // printf("call arg 0: self pointer %p\n", element);
     // obj_printer(element);
        Py_INCREF(element);              // ?
        objs_to_decref[dec++] = element; // ?
        call_args++;
    }
    if (n_params + extra_arg != (int)jit_type_num_params(signature)) {
        PyErr_SetString(PyExc_TypeError, "Number of arguments doesn't match function signature");
        return NULL;
    }

 // printf("n_params + extra_arg: %d + %d\n", n_params, extra_arg);
    for (i = extra_arg; i < n_params + extra_arg; i++) {
        element = PyTuple_GetItem(args, i-extra_arg);

        jit_type_t param = jit_type_get_param(signature, i /*+ extra_arg*/);
     // call_args[i] = jit_malloc(jit_type_get_size(param));
     // printf("call arg %d (%d): %d of size %d\n",
     //        i, i + extra_arg,
     //        jit_type_get_kind(param), jit_type_get_size(param));
     // obj_printer(element);

        switch (jit_type_get_kind(param)) {
            case JIT_TYPE_SBYTE: {
                int val = 0;

                if (PyLong_Check(element))
                    val = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                    val = (int)PyFloat_AsDouble(element);
                else
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);

                if (val > 0x7f || val < -0x7f)
                    PyErr_Format(PyExc_TypeError, "Argument %i out of range", i);
                else
                 // *(jit_sbyte*)call_args[i] = (jit_sbyte)val;
                    call_args_array[i].a_sbyte = val;

                break;
            }
            case JIT_TYPE_SHORT:
                if (PyLong_Check(element))
                 // *(jit_short*)call_args[i] = (jit_short)PyLong_AsLong(element);
                    call_args_array[i].a_short = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                 // *(jit_short*)call_args[i] = (jit_short)PyFloat_AsDouble(element);
                    call_args_array[i].a_short = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_INT:
            case JIT_TYPE_NINT:
                if (PyLong_Check(element))
                 // *(jit_int*)call_args[i] = (jit_int)PyLong_AsLong(element);
                    call_args_array[i].a_int = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                 // *(jit_int*)call_args[i] = (jit_int)PyFloat_AsDouble(element);
                    call_args_array[i].a_int = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_UBYTE: {
                long val = 0;
                if (PyLong_Check(element))
                    val = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                    val = PyFloat_AsDouble(element);
                else
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);

                if (val > 0xff)
                    PyErr_Format(PyExc_TypeError, "Argument %i out of range", i);
                else
                 // *(jit_sbyte*)call_args[i] = (jit_sbyte)val;
                    call_args_array[i].a_sbyte = val;

                break;
            }
            case JIT_TYPE_USHORT:
                if (PyLong_Check(element))
                 // *(jit_ushort*)call_args[i] = (jit_ushort)PyLong_AsLong(element);
                    call_args_array[i].a_ushort = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                 // *(jit_ushort*)call_args[i] = (jit_ushort)PyFloat_AsDouble(element);
                    call_args_array[i].a_ushort = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_UINT:
            case JIT_TYPE_NUINT:
                if (PyLong_Check(element))
                 // *(jit_uint*)call_args[i] = (jit_uint)PyLong_AsLong(element);
                    call_args_array[i].a_uint = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                 // *(jit_uint*)call_args[i] = (jit_uint)PyFloat_AsDouble(element);
                    call_args_array[i].a_uint = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_LONG:
                if (PyLong_Check(element))
                 // *(jit_long*)call_args[i] = PyLong_AsLong(element);
                    call_args_array[i].a_long = PyLong_AsLong(element);
                else if (PyFloat_Check(element))
                 // *(jit_long*)call_args[i] = (jit_long)PyFloat_AsDouble(element);
                    call_args_array[i].a_long = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_ULONG:
                if (PyLong_Check(element))
                 // *(unsigned long*)call_args[i] = PyLong_AsUnsignedLong(element);
                    call_args_array[i].a_ulong = PyLong_AsUnsignedLong(element);
                else if (PyFloat_Check(element))
                 // *(unsigned long*)call_args[i] = PyFloat_AsDouble(element);
                    call_args_array[i].a_ulong = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_FLOAT32:
                if (PyLong_Check(element))
                 // *(jit_float32*)call_args[i] = PyLong_AsDouble(element);
                    call_args_array[i].a_float32 = PyLong_AsDouble(element);
                else if (PyFloat_Check(element))
                 // *(jit_float32*)call_args[i] = PyFloat_AsDouble(element);
                    call_args_array[i].a_float32 = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_FLOAT64:
                if (PyLong_Check(element))
                 // *(jit_float64*)call_args[i] = PyLong_AsDouble(element);
                    call_args_array[i].a_float64 = PyLong_AsDouble(element);
                else if (PyFloat_Check(element))
                 // *(jit_float64*)call_args[i] = PyFloat_AsDouble(element);
                    call_args_array[i].a_float64 = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_NFLOAT:
                if (PyLong_Check(element))
                 // *(jit_nfloat*)call_args[i] = PyLong_AsDouble(element);
                    call_args_array[i].a_nfloat = PyLong_AsDouble(element);
                else if (PyFloat_Check(element))
                 // *(jit_nfloat*)call_args[i] = PyFloat_AsDouble(element);
                    call_args_array[i].a_nfloat = PyFloat_AsDouble(element);
                else {
                    PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
                }
                break;
            case JIT_TYPE_PTR:
             // printf("arg %d: pointer %p\n", i, element);
             // obj_printer(element);
                Py_INCREF(element);
                objs_to_decref[dec++] = element;
             // *(void **)call_args[i] = element;
                call_args_array[i].a_py_object = element;
                break;
            case JIT_TYPE_FIRST_TAGGED + PYJIT_TYPE_ARRAY:
                Py_INCREF(element);
                objs_to_decref[dec++] = element;
             // *(void **)call_args[i] = PyArray_AsPointer(element);
             // *(void **)call_args[i] = element;
                call_args_array[i].a_py_object = element;
                break;
            default:
                PyErr_Format(PyExc_TypeError, "Invalid argument type for %i", i);
        }

        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);

        if (return_exception[0] != NULL) {
            /* Py_DECREF(args); */
            if (extra_arg)
                call_args--;
         // for (idx = 0; idx <= i; idx++)
         //     jit_free(call_args[idx]);
            return NULL;
        }
     // printf(" %d:%p", i, call_args[i]);
    }

    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);

 // retval = jit_malloc(jit_type_get_size(jit_type_get_return(signature)));

 // printf("start call...\n");
 // for (i = 0; i < n_params + extra_arg; i++) {
 //     printf("arg %d: %p %d\n",
 //            i, call_args_array[i].a_py_object, call_args_array[i].a_int);
 //     obj_printer(call_args_array[i].a_py_object);
 // }
    self->obj->apply(call_args_a, &retval);
 // printf("done!\n");

    if (PyErr_Occurred()) {
        printf("*** oops, PyErr_Occurred ***\n");
        obj_printer(PyErr_Occurred());
        PyErr_Print();
        std::abort();
    }

    /* re-aquire the GIL */
    __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);

 // Py_DECREF(args);  // don't decref, owned by caller

 // printf("retval kind %d\n",
 //        jit_type_get_kind(jit_type_get_return(signature)));
    switch (jit_type_get_kind(jit_type_get_return(signature))) {
        case JIT_TYPE_SBYTE:
         // py_retval = PyLong_FromLong((long)(*(jit_sbyte*)retval));
            py_retval = PyLong_FromLong(retval.a_sbyte);
            break;
        case JIT_TYPE_SHORT:
         // py_retval = PyLong_FromLong((long)(*(jit_short*)retval));
            py_retval = PyLong_FromLong(retval.a_short);
            break;
        case JIT_TYPE_INT:
        case JIT_TYPE_NINT:
         // py_retval = PyLong_FromLong((long)(*(jit_int*)retval));
            py_retval = PyLong_FromLong(retval.a_int);
            break;
        case JIT_TYPE_LONG:
         // py_retval = PyLong_FromLong((long)(*(jit_long*)retval));
            py_retval = PyLong_FromLong(retval.a_long);
            break;
        case JIT_TYPE_UBYTE:
         // py_retval = PyLong_FromSsize_t((size_t)(*(jit_ubyte*)retval));
            py_retval = PyLong_FromSsize_t(retval.a_ubyte);
            break;
        case JIT_TYPE_USHORT:
         // py_retval = PyLong_FromSsize_t((size_t)(*(jit_ushort*)retval));
            py_retval = PyLong_FromSsize_t(retval.a_ushort);
            break;
        case JIT_TYPE_UINT:
        case JIT_TYPE_NUINT:
         // py_retval = PyLong_FromSsize_t((size_t)(*(jit_int*)retval));
            py_retval = PyLong_FromSsize_t(retval.a_int);
            break;
        case JIT_TYPE_ULONG:
         // py_retval = PyLong_FromUnsignedLong((unsigned long)(*(jit_ulong*)retval));
            py_retval = PyLong_FromUnsignedLong(retval.a_ulong);
            break;
        case JIT_TYPE_FLOAT32:
         // py_retval = PyFloat_FromDouble(*(jit_float32*)retval);
            py_retval = PyFloat_FromDouble(retval.a_float32);
            break;
        case JIT_TYPE_FLOAT64:
         // py_retval = PyFloat_FromDouble(*(jit_float64*)retval);
            py_retval = PyFloat_FromDouble(retval.a_float64);
            break;
        case JIT_TYPE_NFLOAT:
         // py_retval = PyFloat_FromDouble(*(jit_nfloat*)retval);
            py_retval = PyFloat_FromDouble(retval.a_nfloat);
            break;
        case JIT_TYPE_PTR:
        case JIT_TYPE_FIRST_TAGGED + PYJIT_TYPE_ARRAY:
         // py_retval = *(PyObject **) retval;
            py_retval = retval.a_py_object;
         // Py_INCREF(py_retval); // seems to be necessary?
         // obj_printer(py_retval);
            break;
        case JIT_TYPE_FIRST_TAGGED + PYJIT_TYPE_BOOL:
         // py_retval = PyBool_FromLong(*(jit_ubyte*)retval);
            py_retval = PyBool_FromLong(retval.a_ubyte);
            break;
        case JIT_TYPE_STRUCT: /* TODO: maybe use a dict to load up the struct? */
        case JIT_TYPE_UNION:
        case JIT_TYPE_VOID:
        default:
            Py_INCREF(Py_None);
            py_retval = Py_None;
    }

    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);

    for (unsigned i = 0; i < dec; i++) {
     // printf("decref %d\n", i);
     // obj_printer(objs_to_decref[i]);
        Py_DECREF(objs_to_decref[i]);
    }

    if (extra_arg)
        call_args--;
 // for (idx = 0; idx < n_params + extra_arg; idx++)
 //     jit_free(call_args[idx]);
 // jit_free(retval);

 // printf("return:\n");
 // obj_printer(py_retval);
 // printf("-\n");

    return py_retval;
} 
'''

py_array_as_pointer_code = r'''
static ptrdiff_t PyArray_buffer_offset = -1;

extern "C" void jitlib_nop(void)
{
}

extern "C" void jitlib_nop_1(void *)
{
}

extern "C" void jitlib_nop_2(void *, void *)
{
}

extern "C" void *jit_binaryfunc(void *, void *)
{
    return NULL;
}

#define NB_BINOP(method_struct, slot) \
        (((binaryfunc *) (method_struct))[slot])

extern "C" binaryfunc py_int_binaryfunc(int idx)
{
    return NB_BINOP(PyLong_Type.tp_as_number, idx);
}

extern "C" binaryfunc py_float_binaryfunc(int idx)
{
    return NB_BINOP(PyFloat_Type.tp_as_number, idx);
}

extern "C" int InitializeArrayOffset(void)
{
    /* UGLY HACK: Rely on the fact that an array object starts with a
     * PyVarObject head, immediately followed by the item pointer. */
    PyArray_buffer_offset = sizeof(PyVarObject);
 // printf("computed array offset %d\n", PyArray_buffer_offset);
    return PyArray_buffer_offset;
}

extern "C" void *PyBuffer_BasePointer(PyObject *array)
{
    if (PyArray_buffer_offset == -1)
    {
        fprintf(stderr, "error: should have called InitializeArrayOffset\n");
        abort();
        InitializeArrayOffset();
    }
 // printf("will try to fetch pointer from array %p\n", array);
 // obj_printer(array);
 // printf("length %ld\n", (long) PyObject_Size(array));
    char *p = (char *) array + PyArray_buffer_offset;
 // printf("-> %p -> %p\n", (void **) p, *(void **) p);
    if (PyList_Check(array)) {
        p = (char *) *(void **) p;
     // printf("list! -> %p\n", p);
        return p;
    } else if (PyTuple_Check(array)) {
     // printf("tuple! -> %p\n", p);
        return p;
    }
 // long l = PyObject_Size(array);
 // long limit = 0;
 // if (l > 10) {
 //     limit = l - 10;
 // }
 // while (l > limit) {
 //     l--;
 //     PyObject *o = (PyObject *) ((void **) p)[l];
 //     printf("%p[%ld] -> %p\n", p, l, o);
 //     obj_printer(o);
 // }
    return *(void **) p;
 // return p;
}

extern "C" PyObject *PyJIT_iternext(PyObject *v)
{
    PyObject *result = (*v->ob_type->tp_iternext)(v);
 // printf("iternext: %p\n", result);
 // obj_printer(result);
    return result;
}

extern "C" void PyJIT_print_int(int i)
{
    printf("int: %d\n", i);
}

extern "C" void PyJIT_print_double(double d)
{
    printf("double: %f\n", d);
}

extern "C" void marker(void)
{
    printf("<marker> called\n");
}

extern "C" void marker_int(int m)
{
    printf("<marker %d>\n", m);
}

extern "C" void obj_printer(PyObject *obj)
{
    printf("obj = %p\n", obj);
    if (!obj) return;
    printf("refcnt = %d\n", obj->ob_refcnt);
    printf("type = %p\n", obj->ob_type);
    if (!obj->ob_type) return;
    printf("type is %s\n", obj->ob_type->tp_name);
    if (PyBytes_Check(obj)) printf("value = %s\n", PyBytes_AsString(obj));
    if (PyLong_Check(obj)) printf("value = %ld\n", PyLong_AsLong(obj));
    if (PyType_Check(obj)) printf("name = %s\n", ((PyTypeObject *) obj)->tp_name);
    if (PyTuple_Check(obj)) printf("size = %d\n", PyTuple_Size(obj));
    if (!PyUnicode_Check(obj)) return;
    printf("value = '%s'\n", _PyUnicode_AsString(obj));
}

#define make_offset_func(type, field) \
    extern "C" int jit_##field##_offset() { \
        int offset = offsetof(type, field); \
        if (offset % sizeof (int) != 0) { \
            fprintf(stderr, "can't deal with non-multiple-of-int offset\n"); \
            abort(); \
        } \
        return offset / sizeof (int); \
    }

make_offset_func(PyObject, ob_refcnt)
make_offset_func(PyObject, ob_type)
make_offset_func(PyTypeObject, tp_call)
make_offset_func(PyMethodObject, im_func)
make_offset_func(PyMethodObject, im_self)
make_offset_func(PyVarObject, ob_size)

extern "C" int jit_tuple_ob_item_offset() {
    return offsetof(PyTupleObject, ob_item);
}

extern "C" void *obj_ptr(PyObject *obj) { return obj; }

#include <frameobject.h>

/* This is copied from Python's fast_function, which is static and thus
 * cannot be called directly. We can always call this if
 * PyFunction_Check(func) is true. But it requires setting up a call
 * stack. That might be faster than a tuple, but then again it might not. */
extern "C" PyObject *
jit_fast_function(PyObject *func, PyObject ***pp_stack, int n, int na, int nk)
{       
    PyCodeObject *co = (PyCodeObject *)PyFunction_GET_CODE(func);
    PyObject *globals = PyFunction_GET_GLOBALS(func);
    PyObject *argdefs = PyFunction_GET_DEFAULTS(func);
    PyObject *kwdefs = PyFunction_GET_KW_DEFAULTS(func);
    PyObject **d = NULL;
    int nd = 0;
            
    if (argdefs == NULL && co->co_argcount == n &&
        co->co_kwonlyargcount == 0 && nk==0 &&
        co->co_flags == (CO_OPTIMIZED | CO_NEWLOCALS | CO_NOFREE)) {
        PyFrameObject *f;
        PyObject *retval = NULL;
        PyThreadState *tstate = PyThreadState_GET();
        PyObject **fastlocals, **stack;
        int i;
        
        f = PyFrame_New(tstate, co, globals, NULL);
        if (f == NULL)
            return NULL;

        fastlocals = f->f_localsplus;
     // stack = (*pp_stack) - n;
        stack = *pp_stack;

        for (i = 0; i < n; i++) {
            Py_INCREF(*stack);
            fastlocals[i] = *stack++;
        }
        retval = PyEval_EvalFrameEx(f,0);
        ++tstate->recursion_depth;
        Py_DECREF(f);
        --tstate->recursion_depth;
        return retval;
    }
    if (argdefs != NULL) {
        d = &PyTuple_GET_ITEM(argdefs, 0);
        nd = Py_SIZE(argdefs);
    }
    return PyEval_EvalCodeEx((PyObject*)co, globals,
                             (PyObject *)NULL, *pp_stack, na,
                             (*pp_stack)-2*nk, nk, d, nd, kwdefs,
                             PyFunction_GET_CLOSURE(func));
}
'''

_wrap_PyJit_function__call__ += py_array_as_pointer_code
_wrap_PyJit_function__call__ += _wrap_PyJit_function__descr_get__

_wrap_Jit_function___enter__ = '''
PyObject *
_wrap_Jit_function___enter__(PyJit_function *self, PyObject **return_exception)
{
    self->obj->build_start();

    Py_INCREF(self);
    return (PyObject *)self;
}
'''

_wrap_Jit_function___exit__ = '''
PyObject *
_wrap_Jit_function___exit__(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *type, *value, *traceback;
    const char *keywords[] = {"type", "value", "traceback", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "OOO", (char **) keywords, &type, &value, &traceback)) {
        return NULL;
    }
    self->obj->build_end();

    Py_RETURN_NONE;
}
'''

def register_Jit_function_methods(root_module, cls):
    jit_build_exception = root_module['jit_build_exception']
    cls.add_constructor([param('jit_context &', 'context')])
    cls.add_constructor([param('jit_context &', 'context'),
                         param('jit_type_t', 'signature')])

    cls.add_copy_constructor()

    def helper_class_hook(helper_class):
        helper_class.add_custom_method("PyJit_function__PythonHelper(jit_function_t & func)\n"
                                       "    : jit_function(func), m_pyself(NULL) {}")
        helper_class.add_post_generation_code(r'''
            // gergo was here!
            #define PYJIT_TYPE_ARRAY 20
            #define PYJIT_TYPE_BOOL  21
            #define PYJIT_TYPE_TUPLE 22
            extern "C" void *PyBuffer_BasePointer(PyObject *);
            extern "C" PyObject *get_self_token(void);
        ''');
      # helper_class.add_post_generation_code(_wrap_PyJit_function__descr_get__)

    cls.add_helper_class_hook(helper_class_hook)

    cls.add_instance_attribute('optimization_level', 
                               'unsigned int', 
                               getter='optimization_level',
                               setter='set_optimization_level')

    cls.add_instance_attribute('is_compiled', 
                               'bool', 
                               is_const=True,
                               getter='is_compiled')

    cls.add_instance_attribute('is_recompilable', 
                               'bool', 
                               getter='is_recompilable',
                               setter='set_recompilable')

    cls.add_instance_attribute('is_valid', 
                               'bool', 
                               is_const=True,
                               getter='is_valid')

    cls.add_instance_attribute('signature', 
                               'jit_type_t', 
                               is_const=True,
                               getter='signature')

    cls.slots['tp_descr_get'] = '_wrap_PyJit_function__descr_get__'

    cls.add_custom_method_wrapper("__call__", "_wrap_PyJit_function__call__",
                                  _wrap_PyJit_function__call__,
                                  flags=["METH_VARARGS"])

    cls.add_custom_method_wrapper("__enter__", "_wrap_Jit_function___enter__",
                                  _wrap_Jit_function___enter__,
                                  flags=[])

    cls.add_custom_method_wrapper("__exit__", "_wrap_Jit_function___exit__",
                                  _wrap_Jit_function___exit__,
                                  flags=["METH_VARARGS", "METH_KEYWORDS"])

    cls.add_method('dump',
                   'void',
                   [])

    cls.add_method('build_start', 
                   'void', 
                   [])

    cls.add_method('build_end', 
                   'void', 
                   [])

    cls.add_method('compile', 
                   'int', 
                   [])

    cls.add_method('create', 
                   'void', 
                   [param('jit_type_t', 'signature')])

    cls.add_method('create', 
                   'void', 
                   [])

    cls.add_method('closure', 
                   'void *', 
                   [], 
                   is_const=True)

    cls.add_method('get_param', 
                   'jit_value', 
                   [param('unsigned int', 'param')],
                   throw=[jit_build_exception])

    cls.add_method('get_struct_pointer', 
                   'jit_value', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('store', 
                   'void', 
                   [param('jit_value const &', 'dest'),
                    param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('vtable_pointer', 
                   'void *', 
                   [], 
                   is_const=True)

    cls.add_method('build', 
                   'void', 
                   [], 
                   visibility='protected',
                   is_virtual=True)

    cls.add_method('create_signature', 
                   'jit_type_t', 
                   [], 
                   visibility='protected',
                   is_virtual=True)

    cls.add_method('max_optimization_level', 
                   'unsigned int', 
                   [], 
                   is_static=True)

    cls.add_method('fail', 
                   'void', 
                   [], 
                   visibility='protected',
                   throw=[jit_build_exception])

    cls.add_method('out_of_memory', 
                   'void', 
                   [], 
                   visibility='protected',
                   throw=[jit_build_exception])

    cls.add_method('signature_helper', 
                   'jit_type_t', 
                   [param('jit_type_t', 'return_type')] +
                   # semi-hacky way to deal with '...' -- max args == 32
                   [param('jit_type_t', 'arg%i' % (i), default_value='0') for i in range(32)] +
                   [param('const char *', 'name', default_value='"<sig>"')],
                   is_static=True)

    cls.add_method('insn_abs', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_acos', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_add', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_add_ovf', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_add_relative', 
                   'jit_value', 
                   [param('jit_value const &', 'value',),
                    param('unsigned int', 'offset')],
                   throw=[jit_build_exception])

    cls.add_method('insn_address_of', 
                   'jit_value', 
                   [param('jit_value const &', 'value1',)],
                   throw=[jit_build_exception])

    cls.add_method('insn_address_of_label', 
                   'jit_value', 
                   [param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_alloca', 
                   'jit_value', 
                   [param('jit_value const &', 'size',)],
                   throw=[jit_build_exception])

    cls.add_method('insn_and', 
                   'jit_value', 
                   [param('jit_value const &', 'value1',),
                    param('jit_value const &', 'value2',)],
                   throw=[jit_build_exception])

    cls.add_method('insn_asin', 
                   'jit_value', 
                   [param('jit_value const &', 'value1',)],
                   throw=[jit_build_exception])

    cls.add_method('insn_atan', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_atan2', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_branch', 
                   'void', 
                   [param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_branch_if', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_branch_if_not', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_branch_if_pc_not_in_range', 
                   'void', 
                   [param('jit_label const &', 'start_label'),
                    param('jit_label const &', 'end_label'),
                    param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call', 
                   'jit_value', 
                   [param('char const *', 'name'),
                    param('jit_function_t', 'jit_func'),
                    param('jit_type_t', 'signature'),
                    JitListParam('jit_value_t', 'call_args'),
                    param('int', 'flags', default_value='0')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call_filter', 
                   'jit_value', 
                   [param('jit_label &', 'label'),
                    param('jit_value const &', 'value'),
                    param('jit_type_t', 'type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call_finally', 
                   'void', 
                   [param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call_indirect', 
                   'jit_value', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'signature'),
                    JitListParam('jit_value_t', 'call_args'),
                    param('int', 'flags', default_value='0')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call_indirect_vtable', 
                   'jit_value', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'signature'),
                    JitListParam('jit_value_t', 'call_args'),
                    param('int', 'flags', default_value='0')],
                   throw=[jit_build_exception])

    cls.add_method('insn_call_native', 
                   'jit_value', 
                   [param('char const *', 'name'),
                    param('void *', 'native_func'),
                    param('jit_type_t', 'signature'),
                    JitListParam('jit_value_t', 'call_args'),
                    param('int', 'flags', default_value='0')],
                   throw=[jit_build_exception])

    cls.add_method('insn_ceil', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_check_null', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_cmpg', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_cmpl', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_convert', 
                   'jit_value', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'type'),
                    param('int', 'overflow_check', default_value='0')],
                   throw=[jit_build_exception])

    cls.add_method('insn_cos', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_cosh', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_default_return', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_div', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_dup', 
                   'jit_value', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_eq', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_exp', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_floor', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_flush_struct', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_ge', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_get_call_stack', 
                   'jit_value', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_gt', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_import', 
                   'jit_value', 
                   [param('jit_value', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_incoming_frame_posn', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('unsigned int', 'posn')],
                   throw=[jit_build_exception])

    cls.add_method('insn_incoming_reg', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('int', 'reg')],
                   throw=[jit_build_exception])

    cls.add_method('insn_is_finite', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_is_inf', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_is_nan', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_jump_table', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_jump_table &', 'jump_table')],
                   throw=[jit_build_exception])

    cls.add_method('insn_label', 
                   'void', 
                   [param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_le', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_load', 
                   'jit_value', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_load_elem', 
                   'jit_value', 
                   [param('jit_value const &', 'base_addr'),
                    param('jit_value const &', 'index'),
                    param('jit_type_t', 'elem_type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_load_elem_address', 
                   'jit_value', 
                   [param('jit_value const &', 'base_addr'),
                    param('jit_value const &', 'index'),
                    param('jit_type_t', 'elem_type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_load_relative', 
                   'jit_value', 
                   [param('jit_value const &', 'value'),
                    param('unsigned int', 'offset'),
                    param('jit_type_t', 'type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_load_small', 
                   'jit_value', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_log', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_log10', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_lt', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_mark_breakpoint', 
                   'void', 
                   [param('unsigned int', 'data1'),
                    param('unsigned int', 'data2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_mark_offset', 
                   'void', 
                   [param('int', 'offset')],
                   throw=[jit_build_exception])

    cls.add_method('insn_max', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_memcpy', 
                   'void', 
                   [param('jit_value const &', 'dest'),
                    param('jit_value const &', 'src'),
                    param('jit_value const &', 'size')],
                   throw=[jit_build_exception])

    cls.add_method('insn_memmove', 
                   'void', 
                   [param('jit_value const &', 'dest'),
                    param('jit_value const &', 'src'),
                    param('jit_value const &', 'size')],
                   throw=[jit_build_exception])

    cls.add_method('insn_memset', 
                   'void', 
                   [param('jit_value const &', 'dest'),
                    param('jit_value const &', 'value'),
                    param('jit_value const &', 'size')],
                   throw=[jit_build_exception])

    cls.add_method('insn_min', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_move_blocks_to_end', 
                   'void', 
                   [param('jit_label const &', 'from_label'),
                    param('jit_label const &', 'to_label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_move_blocks_to_start', 
                   'void', 
                   [param('jit_label const &', 'from_label'),
                    param('jit_label const &', 'to_label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_mul', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_mul_ovf', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_ne', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_neg', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_new_block', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_not', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_or', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_outgoing_frame_posn', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('unsigned int', 'posn')],
                   throw=[jit_build_exception])

    cls.add_method('insn_outgoing_reg', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('int', 'reg')],
                   throw=[jit_build_exception])

    cls.add_method('insn_pow', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_push', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_push_ptr', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_push_return_area_ptr', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_rem', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_rem_ieee', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_rethrow_unhandled', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_return', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_return', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_return_from_filter', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_return_from_finally', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_return_ptr', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_return_reg', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('int', 'reg')],
                   throw=[jit_build_exception])

    cls.add_method('insn_rint', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_round', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_set_param', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('unsigned int', 'offset')],
                   throw=[jit_build_exception])

    cls.add_method('insn_set_param_ptr', 
                   'void', 
                   [param('jit_value const &', 'value'),
                    param('jit_type_t', 'type'),
                    param('unsigned int', 'offset')],
                   throw=[jit_build_exception])

    cls.add_method('insn_setup_for_nested', 
                   'void', 
                   [param('int', 'nested_level'),
                    param('int', 'reg')],
                   throw=[jit_build_exception])

    cls.add_method('insn_shl', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_shr', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sign', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sin', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sinh', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sqrt', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sshr', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_start_catcher', 
                   'jit_value', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_start_filter', 
                   'jit_value', 
                   [param('jit_label &', 'label'),
                    param('jit_type_t', 'type')],
                   throw=[jit_build_exception])

    cls.add_method('insn_start_finally', 
                   'void', 
                   [param('jit_label &', 'label')],
                   throw=[jit_build_exception])

    cls.add_method('insn_store_elem', 
                   'void', 
                   [param('jit_value const &', 'base_addr'),
                    param('jit_value const &', 'index'),
                    param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_store_relative', 
                   'void', 
                   [param('jit_value const &', 'dest'),
                    param('unsigned int', 'offset'),
                    param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sub', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_sub_ovf', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_tan', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_tanh', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_throw', 
                   'void', 
                   [param('jit_value const &', 'value')],
                   throw=[jit_build_exception])

    cls.add_method('insn_thrown_exception', 
                   'jit_value', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_to_bool', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_to_not_bool', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_trunc', 
                   'jit_value', 
                   [param('jit_value const &', 'value1')],
                   throw=[jit_build_exception])

    cls.add_method('insn_uses_catcher', 
                   'void', 
                   [],
                   throw=[jit_build_exception])

    cls.add_method('insn_ushr', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('insn_xor', 
                   'jit_value', 
                   [param('jit_value const &', 'value1'),
                    param('jit_value const &', 'value2')],
                   throw=[jit_build_exception])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('char', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('unsigned char', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('int', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('unsigned int', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('void *', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    # These lead to compiler errors on 32-bit systems.
  # cls.add_method('new_constant', 
  #                'jit_value', 
  #                [param('long int', 'value'),
  #                 param('jit_type_t', 'type', default_value='0')],
  #                throw=[root_module['jit_build_exception']])

  # cls.add_method('new_constant', 
  #                'jit_value', 
  #                [param('long unsigned int', 'value'),
  #                 param('jit_type_t', 'type', default_value='0')],
  #                throw=[root_module['jit_build_exception']])

    cls.add_method('new_constant', 
                   'jit_value', 
                   [param('double', 'value'),
                    param('jit_type_t', 'type', default_value='0')],
                   throw=[root_module['jit_build_exception']])

    cls.add_method('new_label', 
                   'jit_label', 
                   [])

    cls.add_method('new_value', 
                   'jit_value', 
                   [param('jit_type_t', 'type')],
                   throw=[jit_build_exception])
    return

def register_Jit_jump_table_methods(root_module, cls):
    cls.add_constructor([param('int', 'size')])

    cls.add_method('size', 
                   'int', 
                   [])

    cls.add_method('get', 
                   'jit_label', 
                   [param('int', 'index')])

    cls.add_method('set', 
                   'void', 
                   [param('int', 'index'),
                    param('jit_label', 'label')])
    return

def register_Jit_label_methods(root_module, cls):
    cls.add_constructor([])
    cls.add_copy_constructor()

    cls.add_instance_attribute('is_valid', 
                               'bool', 
                               is_const=True,
                               getter='is_valid')

    return

def register_Jit_value_methods(root_module, cls):
    import types
    def _generate_tp_richcompare(self, code_sink):
        tp_richcompare_function_name = "_wrap_%s__tp_richcompare" % (self.pystruct)

        code_sink.writeln("static PyObject*\n%s(%s *self, %s *other, int opid)"
                          % (tp_richcompare_function_name, self.pystruct, self.pystruct))
        code_sink.writeln("{")
        code_sink.indent()
        code_sink.writeln("%s *retval;\n" % (self.pystruct))
        code_sink.writeln("if (!PyObject_IsInstance((PyObject*)other, (PyObject*)&%s)) {" % self.pytypestruct)
        code_sink.indent() 
        code_sink.writeln("Py_INCREF(Py_NotImplemented);")
        code_sink.writeln("return Py_NotImplemented;")
        code_sink.unindent()
        code_sink.writeln("}\n")

        code_sink.writeln("switch (opid) {")
        code_sink.indent()

        def wrap_operator(name, opid_code):
            code_sink.writeln("case %s:" % opid_code)
            code_sink.indent()
            if name in self.binary_comparison_operators:
                code_sink.writeln("retval = PyObject_New(%s, &%s);" % (self.pystruct, self.pytypestruct))
                code_sink.writeln("retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;")
                code_sink.writeln("retval->obj = new %s(*self->obj %s *other->obj);" % (self.get_construct_name(), name))
                code_sink.writeln("return (PyObject*)retval;")
            else:
                code_sink.writeln("Py_INCREF(Py_NotImplemented);\n"
                                  "return Py_NotImplemented;")
            code_sink.unindent()
        
        wrap_operator('<', 'Py_LT')
        wrap_operator('<=', 'Py_LE')
        wrap_operator('==', 'Py_EQ')
        wrap_operator('!=', 'Py_NE')
        wrap_operator('>=', 'Py_GE')
        wrap_operator('>', 'Py_GT')

        code_sink.unindent()
        code_sink.writeln("} /* closes switch (opid) */\n")

        code_sink.writeln("Py_INCREF(Py_NotImplemented);\n"
                          "return Py_NotImplemented;")

        code_sink.unindent()
        code_sink.writeln("}\n")

        return tp_richcompare_function_name

    # Duck Punch FTW!!!
    cls._generate_tp_richcompare = types.MethodType(_generate_tp_richcompare, cls, CppClass)

    cls.add_constructor([])
    cls.add_copy_constructor()

    cls.add_binary_numeric_operator('+')
    cls.add_binary_numeric_operator('-')
    cls.add_binary_numeric_operator('/')
    cls.add_binary_numeric_operator('*')
    cls.add_binary_numeric_operator('%')
    cls.add_binary_numeric_operator('<<')
    cls.add_binary_numeric_operator('>>')
    cls.add_binary_numeric_operator('&')
    cls.add_binary_numeric_operator('|')
    cls.add_binary_numeric_operator('^')

    cls.add_unary_numeric_operator('-')
    cls.add_unary_numeric_operator('~')

    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')

    cls.add_instance_attribute('context', 
                               retval('jit_pycontext_t', caller_owns_return=True), 
                               is_const=True,
                               getter='context')

    cls.add_instance_attribute('function', 
                               retval('jit_function_t', caller_owns_return=False), 
                               is_const=True,
                               getter='function')

    cls.add_instance_attribute('block', 
                               VoidPtrReturn('jit_block_t'), 
                               is_const=True,
                               getter='block')

    cls.add_instance_attribute('type', 
                               retval('jit_type_t', caller_owns_return=True), 
                               is_const=True,
                               getter='type')

    cls.add_instance_attribute('is_constant', 
                               'bool', 
                               is_const=True,
                               getter='is_constant')

    cls.add_instance_attribute('is_local', 
                               'bool', 
                               is_const=True,
                               getter='is_local')

    cls.add_instance_attribute('is_parameter', 
                               'bool', 
                               is_const=True,
                               getter='is_parameter')

    cls.add_instance_attribute('is_temporary', 
                               'bool', 
                               is_const=True,
                               getter='is_temporary')

    cls.add_instance_attribute('is_valid', 
                               'bool', 
                               is_const=True,
                               getter='is_valid')

    cls.add_instance_attribute('is_addressable', 
                               'bool', 
                               is_const=True,
                               getter='is_addressable')

    cls.add_instance_attribute('is_volatile', 
                               'bool', 
                               is_const=True,
                               getter='is_volatile')

    cls.add_method('set_addressable', 
                   'void', 
                   [])

    cls.add_method('set_volatile', 
                   'void', 
                   [])

    cls.add_method('float32_constant', 
                   'float', 
                   [], 
                   is_const=True)

    cls.add_method('float64_constant', 
                   'float', 
                   [], 
                   is_const=True)

    cls.add_method('long_constant', 
                   'long int', 
                   [], 
                   is_const=True)

    cls.add_method('nfloat_constant', 
                   'float', 
                   [], 
                   is_const=True)

    cls.add_method('nint_constant', 
                   'unsigned int', 
                   [], 
                   is_const=True)
    return

def register_Jit_type_methods(root_module, cls):
    types = ['void', 'sbyte', 'ubyte', 'short', 'ushort', 'int', 'uint', 'nint', 'nuint',
             'long', 'ulong', 'float32', 'float64', 'nfloat', 'void_ptr']
    # GB
    types.append('array')
    types.append('bool')
    # types.append(['sys_char', 'sys_schar', 'sys_uchar'])
             ## 'sys_bool', 'sys_char', 'sys_schar', 'sys_uchar', 'sys_short', 'sys_ushort',
             ## 'sys_int', 'sys_uint', 'sys_long', 'sys_ulong', 'sys_longlong', 'sys_ulonglong',
             ## 'sys_float', 'sys_double', 'sys_long_double']

    cls.add_copy_constructor()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('==')

    for name in types:
        cls.add_static_attribute(name, JitStaticAttrReturn(name), is_const=True)

    cls.add_instance_attribute('type', 
                               'int',
                               is_const=True,
                               getter='get_kind')

    cls.add_instance_attribute('size', 
                               'unsigned int',
                               is_const=True,
                               getter='get_size')

    cls.add_instance_attribute('num_fields', 
                               'unsigned int',
                               is_const=True,
                               getter='num_fields')

    cls.add_instance_attribute('num_params', 
                               'unsigned int',
                               is_const=True,
                               getter='num_params')

    cls.add_instance_attribute('abi',
                               'jit_abi_t',
                               is_const=True,
                               getter='get_abi')

    cls.add_instance_attribute('tagged_data', 
                               retval('PyObject *', caller_owns_return=True),
                               is_const=True,
                               getter='get_tagged_data')

    cls.add_instance_attribute('return_via_pointer',
                               'bool',
                               is_const=True,
                               getter='return_via_pointer')

    cls.add_instance_attribute('is_primitive',
                               'bool',
                               is_const=True,
                               getter='is_primitive')

    cls.add_instance_attribute('is_struct',
                               'bool',
                               is_const=True,
                               getter='is_struct')

    cls.add_instance_attribute('is_union',
                               'bool',
                               is_const=True,
                               getter='is_union')

    cls.add_instance_attribute('is_signature',
                               'bool',
                               is_const=True,
                               getter='is_signature')

    cls.add_instance_attribute('is_array',
                               'bool',
                               is_const=True,
                               getter='is_array')

    cls.add_instance_attribute('is_bool',
                               'bool',
                               is_const=True,
                               getter='is_bool')

    cls.add_instance_attribute('is_tuple',
                               'bool',
                               is_const=True,
                               getter='is_tuple')

    cls.add_instance_attribute('is_pointer',
                               'bool',
                               is_const=True,
                               getter='is_pointer')

    cls.add_instance_attribute('is_tagged',
                               'bool',
                               is_const=True,
                               getter='is_tagged')

    cls.add_method('array_t',
                   retval('jit_type *', caller_owns_return=False),
                   [param('jit_type_t', 'type')],
                   is_static=True)

    cls.add_static_attribute('tuple_t', JitStaticAttrReturn('tuple'),
                             is_const=True)

    cls.add_method('create_pointer',
                   retval('jit_type *', caller_owns_return=True),
                   [param('jit_type_t', 'type'),
                    param('int', 'incref', default_value='1')],
                   is_static=True)

    cls.add_method('create_struct', 
                   retval('jit_type *', caller_owns_return=True), 
                   [JitListParam('jit_type_t', 'fields'),
                    param('int', 'incref', default_value='1')],
                   is_static=True)

    cls.add_method('create_union', 
                   retval('jit_type *', caller_owns_return=True), 
                   [JitListParam('jit_type_t', 'fields'),
                    param('int', 'incref', default_value='1')],
                   is_static=True)

    cls.add_method('create_signature', 
                   retval('jit_type *', caller_owns_return=True), 
                   [param('jit_abi_t', 'abi'),
                    param('jit_type_t', 'return_type'),
                    JitListParam('jit_type_t', 'params'),
                    param('int', 'incref', default_value='1')],
                   is_static=True)

    cls.add_method('create_tagged', 
                   retval('jit_type *', caller_owns_return=True), 
                   [param('jit_type_t', 'type'),
                    param('int', 'kind'),
                    param('PyObject *', 'data', transfer_ownership=True),
                    param('jit_meta_free_func', '_wrap_meta_free_func'),
                    param('int', 'incref', default_value='1')],
                   is_static=True)

    cls.add_method('best_alignment', 
                   'unsigned int', 
                   [],
                   is_static=True)

    cls.add_method('set_names', 
                   'int', 
                   [param('char **', 'names')])

    cls.add_method('set_size_and_alignment', 
                   'void', 
                   [param('int', 'size'),
                    param('int', 'alignment')])

    cls.add_method('set_offset', 
                   'void', 
                   [param('unsigned int', 'field_index'),
                    param('unsigned int', 'offset')])

    cls.add_method('get_field', 
                   retval('jit_type *', caller_owns_return=True), 
                   [param('unsigned int', 'index')])

    cls.add_method('get_offset', 
                   'unsigned int', 
                   [param('unsigned int', 'index')])

    cls.add_method('get_name', 
                   'const char *', 
                   [param('unsigned int', 'index')])

    cls.add_method('find_name', 
                   'unsigned int', 
                   [param('const char *', 'name')])

    cls.add_method('get_return', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('get_param', 
                   retval('jit_type *', caller_owns_return=True), 
                   [param('unsigned int', 'index')])

    cls.add_method('get_ref', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('get_tagged_type', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('set_tagged_type', 
                   'void', 
                   [param('jit_type_t', 'underlying'),
                    param('int', 'incref', default_value='1')])

    cls.add_method('normalize', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('remove_tags', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('promote_int', 
                   retval('jit_type *', caller_owns_return=True), 
                   [])

    cls.add_method('has_tag',
                   'bool',
                   [param('int', 'kind')])

    cls.add_method('name',
                   'void',
                   [])

def register_Jit_readelf_methods(root_module, cls):
    def readelf_instance_creation_function(cpp_class, code_block, lvalue,
                                       parameters, construct_type_name):
        code_block.write_error_check('jit_readelf_open(&%s, %s) != JIT_READELF_OK' % (lvalue, parameters),
            'PyErr_SetString(PyExc_TypeError, "%s failed to open library");' % (cpp_class.get_python_name()))

    cls.set_instance_creation_function(readelf_instance_creation_function)
    cls.add_constructor([param('const char *', 'filename'),
                         param('int', 'flags', default_value='0')])

    func = cls.add_function_as_method('jit_readelf_close', 
                                      'void', 
                                      [param('jit_readelf_t', 'self', transfer_ownership=False)],
                                      custom_name='close')
    func.after_call.write_code("self->obj = NULL;")

    cls.add_function_as_method('jit_readelf_get_symbol', 
                               'void *', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('const char *', 'name')],
                               custom_name='get_symbol')

    cls.add_function_as_method('jit_readelf_num_needed', 
                               'unsigned int', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False)],
                               custom_name='num_needed')

    cls.add_function_as_method('jit_readelf_get_needed', 
                               'const char *', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('unsigned int', 'index')],
                               custom_name='get_needed')

    cls.add_function_as_method('jit_readelf_add_to_context', 
                               'void', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('jit_pycontext_t', 'context')],
                               custom_name='add_to_context')

    cls.add_function_as_method('jit_readelf_get_section', 
                               PyBytesReturn('void *'), 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('const char *', 'name'),
                                PyBytesSizeParam('int', 'size')],
                               custom_name='get_section')
    '''
    cls.add_function_as_method('jit_readelf_get_section_by_type', 
                               'void *', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('int', 'type'),
                                param('unsigned int *', 'size', direction=Parameter.DIRECTION_OUT)],
                               custom_name='get_section_by_type')

    cls.add_function_as_method('jit_readelf_map_vaddr', 
                               'void *', 
                               [param('jit_readelf_t', 'self', transfer_ownership=False),
                                param('unsigned int', 'vaddr')],
                               custom_name='map_vaddr')
    '''
    return

def register_Jit_writeelf_methods(root_module, cls):
    cls.add_function_as_constructor("jit_writeelf_create",
                                    ReturnValue.new('jit_writeelf_t', caller_owns_return=True),
                                    [param('const char *', 'library_name')])

    cls.add_function_as_method('jit_writeelf_write', 
                               'bool', 
                               [param('jit_writeelf_t', 'self', transfer_ownership=False),
                                param('const char *', 'filename')],
                               custom_name='write')

    cls.add_function_as_method('jit_writeelf_add_function', 
                               'bool', 
                               [param('jit_writeelf_t', 'self', transfer_ownership=False),
                                param('jit_function_t', 'function'),
                                param('const char *', 'name')],
                               custom_name='add_function')

    cls.add_function_as_method('jit_writeelf_add_needed', 
                               'bool', 
                               [param('jit_writeelf_t', 'self', transfer_ownership=False),
                                param('const char *', 'library_name')],
                               custom_name='add_needed')

    cls.add_function_as_method('jit_writeelf_write_section', 
                               'bool', 
                               [param('jit_writeelf_t', 'self', transfer_ownership=False),
                                param('const char *', 'name'),
                                param('int', 'type'),
                                PyBytesParam('void *', 'buf'),
                                param('int', 'discardable')],
                               custom_name='write_section')
    return

def register_Jit_debugger_methods(root_module, cls):
    cls.add_function_as_constructor("jit_debugger_create",
                                    ReturnValue.new('jit_debugger_t', caller_owns_return=True),
                                    [param('jit_pycontext_t', 'context')])

def register_Jit_dynlib_methods(root_module, cls):
    sub = root_module.get_submodule('dynlib')
    sub.add_function('jit_dynlib_set_debug',
                     'void',
                     [param('bool', 'flag')],
                     custom_name='set_debug')

    sub.add_function('jit_dynlib_get_suffix',
                     'const char *',
                     [],
                     custom_name='get_suffix')

    sub.add_function('jit_mangle_global_function',
                     'const char *',
                     [param('const char *', 'name'),
                      param('jit_type_t', 'signature'),
                      param('int', 'form', default_value='0')],
                     custom_name='cpp_mangle_global_function')

    sub.add_function('jit_mangle_member_function',
                     'const char *',
                     [param('const char *', 'class_name'),
                      param('const char *', 'name'),
                      param('jit_type_t', 'signature'),
                      param('int', 'form', default_value='0'),
                      param('int', 'flags', default_value='0')],
                     custom_name='cpp_mangle_member_function')

    cls.add_function_as_constructor('jit_dynlib_open',
                                    'jit_dynlib_handle',
                                    [param('const char *', 'name')])

    cls.add_function_as_method('jit_dynlib_get_symbol', 
                               'void *', 
                               [param('jit_dynlib_handle *', 'self',
                                      transfer_ownership=False),
                                param('const char *', 'symbol')],
                               custom_name='get_symbol')


def main():
    out = JitMultiSectionFactory('jit_module.cc')
    root_module = module_init()
    register_types(root_module)
    register_methods(root_module)
    root_module.generate(out)

if __name__ == '__main__':
    main()

