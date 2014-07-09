/*
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

#include "jit_module.h"
static PyMethodDef jit_elf_READELF_FLAGS_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- enumerations --- */

void _wrap_meta_free_func(void *data)
{ Py_XDECREF((PyObject*) data); }

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef READELF_FLAGS_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.elf.READELF_FLAGS",
    NULL,
    -1,
    jit_elf_READELF_FLAGS_functions,
};
#endif

static PyObject *
initjit_elf_READELF_FLAGS(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&READELF_FLAGS_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.elf.READELF_FLAGS", jit_elf_READELF_FLAGS_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "FLAG_FORCE", JIT_READELF_FLAG_FORCE);
    PyModule_AddIntConstant(m, (char *) "FLAG_DEBUG", JIT_READELF_FLAG_DEBUG);
    return m;
}
static PyMethodDef jit_elf_READELF_ERROR_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- enumerations --- */

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef READELF_ERROR_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.elf.READELF_ERROR",
    NULL,
    -1,
    jit_elf_READELF_ERROR_functions,
};
#endif

static PyObject *
initjit_elf_READELF_ERROR(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&READELF_ERROR_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.elf.READELF_ERROR", jit_elf_READELF_ERROR_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "OK", JIT_READELF_OK);
    PyModule_AddIntConstant(m, (char *) "CANNOT_OPEN", JIT_READELF_CANNOT_OPEN);
    PyModule_AddIntConstant(m, (char *) "NOT_ELF", JIT_READELF_NOT_ELF);
    PyModule_AddIntConstant(m, (char *) "WRONG_ARCH", JIT_READELF_WRONG_ARCH);
    PyModule_AddIntConstant(m, (char *) "BAD_FORMAT", JIT_READELF_BAD_FORMAT);
    PyModule_AddIntConstant(m, (char *) "MEMORY", JIT_READELF_MEMORY);
    return m;
}
static PyMethodDef jit_elf_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- classes --- */

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef elf_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.elf",
    NULL,
    -1,
    jit_elf_functions,
};
#endif

static PyObject *
initjit_elf(void)
{
    PyObject *m;
    PyObject *submodule;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&elf_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.elf", jit_elf_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    /* Register the 'jit_readelf' class */
    if (PyType_Ready(&PyJit_readelf_Type)) {
        return NULL;
    }
    PyModule_AddObject(m, (char *) "ReadElf", (PyObject *) &PyJit_readelf_Type);
    /* Register the 'jit_writeelf' class */
    if (PyType_Ready(&PyJit_writeelf_Type)) {
        return NULL;
    }
    PyModule_AddObject(m, (char *) "WriteElf", (PyObject *) &PyJit_writeelf_Type);
    submodule = initjit_elf_READELF_FLAGS();
    if (submodule == NULL) {
        return NULL;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "READELF_FLAGS", submodule);
    submodule = initjit_elf_READELF_ERROR();
    if (submodule == NULL) {
        return NULL;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "READELF_ERROR", submodule);
    return m;
}
static PyMethodDef jit_dynlib_cpp_flags_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- enumerations --- */

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef cpp_flags_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.dynlib.cpp_flags",
    NULL,
    -1,
    jit_dynlib_cpp_flags_functions,
};
#endif

static PyObject *
initjit_dynlib_cpp_flags(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&cpp_flags_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.dynlib.cpp_flags", jit_dynlib_cpp_flags_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "PUBLIC", JIT_MANGLE_PUBLIC);
    PyModule_AddIntConstant(m, (char *) "PROTECTED", JIT_MANGLE_PROTECTED);
    PyModule_AddIntConstant(m, (char *) "PRIVATE", JIT_MANGLE_PRIVATE);
    PyModule_AddIntConstant(m, (char *) "STATIC", JIT_MANGLE_STATIC);
    PyModule_AddIntConstant(m, (char *) "VIRTUAL", JIT_MANGLE_VIRTUAL);
    PyModule_AddIntConstant(m, (char *) "CONST", JIT_MANGLE_CONST);
    PyModule_AddIntConstant(m, (char *) "EXPLICIT_THIS", JIT_MANGLE_EXPLICIT_THIS);
    PyModule_AddIntConstant(m, (char *) "IS_CTOR", JIT_MANGLE_IS_CTOR);
    PyModule_AddIntConstant(m, (char *) "IS_DTOR", JIT_MANGLE_IS_DTOR);
    PyModule_AddIntConstant(m, (char *) "BASE", JIT_MANGLE_BASE);
    return m;
}
/* --- module functions --- */


PyObject *
_wrap_jit_dynlib_jit_dynlib_set_debug(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool flag;
    PyObject *py_flag;
    const char *keywords[] = {"flag", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O", (char **) keywords, &py_flag)) {
        return NULL;
    }
    flag = (bool) PyObject_IsTrue(py_flag);
    jit_dynlib_set_debug(flag);
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}

PyObject * _wrap_jit_dynlib_jit_dynlib_set_debug(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs);


PyObject *
_wrap_jit_dynlib_jit_dynlib_get_suffix()
{
    PyObject *py_retval;
    char const *retval;
    
    retval = jit_dynlib_get_suffix();
    py_retval = Py_BuildValue((char *) "s", retval);
    return py_retval;
}

PyObject * _wrap_jit_dynlib_jit_dynlib_get_suffix();


PyObject *
_wrap_jit_dynlib_jit_mangle_global_function(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    char const *retval;
    char const *name;
    PyJit_type *py_signature = NULL;
    int form = 0;
    const char *keywords[] = {"name", "signature", "form", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!|i", (char **) keywords, &name, &PyJit_type_Type, &py_signature, &form)) {
        return NULL;
    }
    retval = jit_mangle_global_function(name, py_signature->obj->raw(), form);
    py_retval = Py_BuildValue((char *) "s", retval);
    return py_retval;
}

PyObject * _wrap_jit_dynlib_jit_mangle_global_function(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs);


PyObject *
_wrap_jit_dynlib_jit_mangle_member_function(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    char const *retval;
    char const *class_name;
    char const *name;
    PyJit_type *py_signature = NULL;
    int form = 0;
    int flags = 0;
    const char *keywords[] = {"class_name", "name", "signature", "form", "flags", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "ssO!|ii", (char **) keywords, &class_name, &name, &PyJit_type_Type, &py_signature, &form, &flags)) {
        return NULL;
    }
    retval = jit_mangle_member_function(class_name, name, py_signature->obj->raw(), form, flags);
    py_retval = Py_BuildValue((char *) "s", retval);
    return py_retval;
}

PyObject * _wrap_jit_dynlib_jit_mangle_member_function(PyObject * PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs);

static PyMethodDef jit_dynlib_functions[] = {
    {
        (char *) "set_debug",
        (PyCFunction) _wrap_jit_dynlib_jit_dynlib_set_debug,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_suffix",
        (PyCFunction) _wrap_jit_dynlib_jit_dynlib_get_suffix,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "cpp_mangle_global_function",
        (PyCFunction) _wrap_jit_dynlib_jit_mangle_global_function,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "cpp_mangle_member_function",
        (PyCFunction) _wrap_jit_dynlib_jit_mangle_member_function,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {NULL, NULL, 0, NULL}
};
/* --- classes --- */

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef dynlib_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.dynlib",
    NULL,
    -1,
    jit_dynlib_functions,
};
#endif

static PyObject *
initjit_dynlib(void)
{
    PyObject *m;
    PyObject *submodule;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&dynlib_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.dynlib", jit_dynlib_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    /* Register the 'jit_dynlib_handle' class */
    if (PyType_Ready(&PyJit_dynlib_handle_Type)) {
        return NULL;
    }
    PyModule_AddObject(m, (char *) "DynlibHandle", (PyObject *) &PyJit_dynlib_handle_Type);
    submodule = initjit_dynlib_cpp_flags();
    if (submodule == NULL) {
        return NULL;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "cpp_flags", submodule);
    return m;
}
static PyMethodDef jit_abi_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- enumerations --- */



#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef abi_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.abi",
    NULL,
    -1,
    jit_abi_functions,
};
#endif

static PyObject *
initjit_abi(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&abi_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.abi", jit_abi_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "cdecl", jit_abi_cdecl);
    PyModule_AddIntConstant(m, (char *) "vararg", jit_abi_vararg);
    PyModule_AddIntConstant(m, (char *) "stdcall", jit_abi_stdcall);
    PyModule_AddIntConstant(m, (char *) "fastcall", jit_abi_fastcall);
    return m;
}
static PyMethodDef jit_TYPES_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- enumerations --- */



#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef TYPES_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit.TYPES",
    NULL,
    -1,
    jit_TYPES_functions,
};
#endif

static PyObject *
initjit_TYPES(void)
{
    PyObject *m;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&TYPES_moduledef);
    #else
    m = Py_InitModule3((char *) "jit.TYPES", jit_TYPES_functions, NULL);
    #endif
    if (m == NULL) {
        return NULL;
    }
    PyModule_AddIntConstant(m, (char *) "INVALID", JIT_TYPE_INVALID);
    PyModule_AddIntConstant(m, (char *) "VOID", JIT_TYPE_VOID);
    PyModule_AddIntConstant(m, (char *) "SBYTE", JIT_TYPE_SBYTE);
    PyModule_AddIntConstant(m, (char *) "UBYTE", JIT_TYPE_UBYTE);
    PyModule_AddIntConstant(m, (char *) "SHORT", JIT_TYPE_SHORT);
    PyModule_AddIntConstant(m, (char *) "USHORT", JIT_TYPE_USHORT);
    PyModule_AddIntConstant(m, (char *) "INT", JIT_TYPE_INT);
    PyModule_AddIntConstant(m, (char *) "UINT", JIT_TYPE_UINT);
    PyModule_AddIntConstant(m, (char *) "NINT", JIT_TYPE_NINT);
    PyModule_AddIntConstant(m, (char *) "NUINT", JIT_TYPE_NUINT);
    PyModule_AddIntConstant(m, (char *) "LONG", JIT_TYPE_LONG);
    PyModule_AddIntConstant(m, (char *) "ULONG", JIT_TYPE_ULONG);
    PyModule_AddIntConstant(m, (char *) "FLOAT32", JIT_TYPE_FLOAT32);
    PyModule_AddIntConstant(m, (char *) "FLOAT64", JIT_TYPE_FLOAT64);
    PyModule_AddIntConstant(m, (char *) "NFLOAT", JIT_TYPE_NFLOAT);
    PyModule_AddIntConstant(m, (char *) "MAX_PRIMITIVE", JIT_TYPE_MAX_PRIMITIVE);
    PyModule_AddIntConstant(m, (char *) "STRUCT", JIT_TYPE_STRUCT);
    PyModule_AddIntConstant(m, (char *) "UNION", JIT_TYPE_UNION);
    PyModule_AddIntConstant(m, (char *) "SIGNATURE", JIT_TYPE_SIGNATURE);
    PyModule_AddIntConstant(m, (char *) "PTR", JIT_TYPE_PTR);
    PyModule_AddIntConstant(m, (char *) "FIRST_TAGGED", JIT_TYPE_FIRST_TAGGED);
    return m;
}
static PyMethodDef jit_functions[] = {
    {NULL, NULL, 0, NULL}
};
/* --- classes --- */

/* --- exceptions --- */


PyTypeObject *Pyjit_build_exception_Type;

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef jit_moduledef = {
    PyModuleDef_HEAD_INIT,
    "jit",
    NULL,
    -1,
    jit_functions,
};
#endif

#if PY_VERSION_HEX >= 0x03000000
    #define MOD_ERROR NULL
    #define MOD_INIT(name) PyInit_##name(void)
    #define MOD_RETURN(val) val
#else
    #define MOD_ERROR
    #define MOD_INIT(name) init##name(void)
    #define MOD_RETURN(val)
#endif

PyMODINIT_FUNC 
MOD_INIT(jit)
{
    PyObject *m;
    PyObject *submodule;
    #if PY_VERSION_HEX >= 0x03000000
    m = PyModule_Create(&jit_moduledef);
    #else
    m = Py_InitModule3((char *) "jit", jit_functions, NULL);
    #endif
    if (m == NULL) {
        return MOD_ERROR;
    }
    /* Register the 'jit_pycontext' class */
    if (PyType_Ready(&PyJit_pycontext_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Context", (PyObject *) &PyJit_pycontext_Type);
    /* Register the 'jit_function' class */
    if (PyType_Ready(&PyJit_function_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Function", (PyObject *) &PyJit_function_Type);
    /* Register the 'jit_value' class */
    if (PyType_Ready(&PyJit_value_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Value", (PyObject *) &PyJit_value_Type);
    /* Register the 'jit_label' class */
    if (PyType_Ready(&PyJit_label_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Label", (PyObject *) &PyJit_label_Type);
    /* Register the 'jit_jump_table' class */
    if (PyType_Ready(&PyJit_jump_table_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "JumpTable", (PyObject *) &PyJit_jump_table_Type);
    /* Register the 'jit_type' class */
    
    PyJit_typeMeta_Type.tp_base = Py_TYPE(&PyBaseObject_Type);
    /* Some fields need to be manually inheritted from the parent metaclass */
    PyJit_typeMeta_Type.tp_traverse = Py_TYPE(&PyBaseObject_Type)->tp_traverse;
    PyJit_typeMeta_Type.tp_clear = Py_TYPE(&PyBaseObject_Type)->tp_clear;
    PyJit_typeMeta_Type.tp_is_gc = Py_TYPE(&PyBaseObject_Type)->tp_is_gc;
    /* PyType tp_setattro is too restrictive */
    PyJit_typeMeta_Type.tp_setattro = PyObject_GenericSetAttr;
    PyType_Ready(&PyJit_typeMeta_Type);
    
    Py_TYPE(&PyJit_type_Type) = &PyJit_typeMeta_Type;
    if (PyType_Ready(&PyJit_type_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Type", (PyObject *) &PyJit_type_Type);
    /* Register the 'jit_debugger' class */
    if (PyType_Ready(&PyJit_debugger_Type)) {
        return MOD_ERROR;
    }
    PyModule_AddObject(m, (char *) "Debugger", (PyObject *) &PyJit_debugger_Type);
    /* Register the 'jit_build_exception' exception */
    if ((Pyjit_build_exception_Type = (PyTypeObject*) PyErr_NewException((char*)"jit.BuildError", NULL, NULL)) == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF((PyObject *) Pyjit_build_exception_Type);
    PyModule_AddObject(m, (char *) "BuildError", (PyObject *) Pyjit_build_exception_Type);
    submodule = initjit_elf();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "elf", submodule);
    submodule = initjit_dynlib();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "dynlib", submodule);
    submodule = initjit_abi();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "abi", submodule);
    submodule = initjit_TYPES();
    if (submodule == NULL) {
        return MOD_ERROR;
    }
    Py_INCREF(submodule);
    PyModule_AddObject(m, (char *) "TYPES", submodule);
    return MOD_RETURN(m);
}
