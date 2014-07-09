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

static PyObject *
_wrap_PyJit_pycontext__get_debugger(PyJit_pycontext *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_debugger *py_jit_debugger;
    
    if (!(self->obj->debugger())) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_debugger = PyObject_New(PyJit_debugger, &PyJit_debugger_Type);
    
    py_jit_debugger->obj = self->obj->debugger();
    py_jit_debugger->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;
    py_retval = Py_BuildValue((char *) "N", py_jit_debugger);
    return py_retval;
}

static PyObject *
_wrap_PyJit_pycontext__get_functions(PyJit_pycontext *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyObject *py_list;
    PyJit_function *py_func;
    jit_function_t elem = NULL;
    jit_function *func_t;
    
    py_list = PyList_New(0);
    while ((elem = jit_function_next(self->obj->raw(), elem)) != NULL) {
        py_func = PyObject_New(PyJit_function, &PyJit_function_Type);
        func_t = jit_function::from_raw(elem);
        if (func_t != NULL) {
            py_func->obj = new jit_function(*func_t);
        } else {
            py_func->obj = new PyJit_function__PythonHelper(elem);
            ((PyJit_function__PythonHelper*) py_func->obj)->set_pyobj((PyObject *)py_func);
        }
        py_func->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
        PyList_Append(py_list, (PyObject*)py_func);
    }
    py_retval = Py_BuildValue((char *) "N", py_list);
    return py_retval;
}

static PyGetSetDef PyJit_pycontext__getsets[] = {
    {
        (char*) "functions", /* attribute name */
        (getter) _wrap_PyJit_pycontext__get_functions, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "debugger", /* attribute name */
        (getter) _wrap_PyJit_pycontext__get_debugger, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

static int
_wrap_PyJit_pycontext__tp_init(PyJit_pycontext *self, PyObject *args, PyObject *kwargs)
{
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return -1;
    }
    self->obj = new jit_pycontext();
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}



PyObject *
_wrap_PyJit_pycontext_register_symbol(PyJit_pycontext *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    char const *name;
    PyObject *value;
    bool after;
    PyObject *py_after;
    const char *keywords[] = {"name", "value", "after", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O", (char **) keywords, &name, &PyLong_Type, &value, &py_after)) {
        return NULL;
    }
    after = (bool) PyObject_IsTrue(py_after);
    retval = self->obj->register_symbol(name, (void *)PyLong_AsVoidPtr(value), after);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_pycontext_resolve_all(PyJit_pycontext *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    bool print_failures;
    PyObject *py_print_failures = NULL;
    const char *keywords[] = {"print_failures", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "|O", (char **) keywords, &py_print_failures)) {
        return NULL;
    }
    print_failures = py_print_failures? (bool) PyObject_IsTrue(py_print_failures) : 0;
    retval = self->obj->resolve_all(print_failures);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_pycontext_build_start(PyJit_pycontext *self)
{
    PyObject *py_retval;
    
    self->obj->build_start();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_pycontext_build_end(PyJit_pycontext *self)
{
    PyObject *py_retval;
    
    self->obj->build_end();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


static PyMethodDef PyJit_pycontext_methods[] = {
    {
        (char *) "register_symbol",
        (PyCFunction) _wrap_PyJit_pycontext_register_symbol,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "resolve_all",
        (PyCFunction) _wrap_PyJit_pycontext_resolve_all,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "build_start",
        (PyCFunction) _wrap_PyJit_pycontext_build_start,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "build_end",
        (PyCFunction) _wrap_PyJit_pycontext_build_end,
        METH_NOARGS,
        NULL
    },
    {NULL, NULL, 0, NULL}
};

static void
PyJit_pycontext__tp_clear(PyJit_pycontext *self)
{
    Py_CLEAR(self->inst_dict);
        jit_pycontext *tmp = self->obj;
    self->obj = NULL;
    if (!(self->flags & PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED)) {
        delete tmp;
    }
}


static int
PyJit_pycontext__tp_traverse(PyJit_pycontext *self, visitproc visit, void *arg)
{
    Py_VISIT(self->inst_dict);
    
    return 0;
}


static void
_wrap_PyJit_pycontext__tp_dealloc(PyJit_pycontext *self)
{
    PyJit_pycontext__tp_clear(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_pycontext_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Context",            /* tp_name */
    sizeof(PyJit_pycontext),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_pycontext__tp_dealloc,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)NULL,       /* tp_getattr */
    (setattrfunc)NULL,       /* tp_setattr */
    (cmpfunc)NULL,           /* tp_compare */
    (reprfunc)NULL,             /* tp_repr */
    (PyNumberMethods*)NULL,     /* tp_as_number */
    (PySequenceMethods*)NULL, /* tp_as_sequence */
    (PyMappingMethods*)NULL,   /* tp_as_mapping */
    (hashfunc)NULL,             /* tp_hash */
    (ternaryfunc)NULL,          /* tp_call */
    (reprfunc)NULL,              /* tp_str */
    (getattrofunc)NULL,     /* tp_getattro */
    (setattrofunc)NULL,     /* tp_setattro */
    (PyBufferProcs*)NULL,  /* tp_as_buffer */
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_GC,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)PyJit_pycontext__tp_traverse,     /* tp_traverse */
    (inquiry)PyJit_pycontext__tp_clear,             /* tp_clear */
    (richcmpfunc)NULL,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_pycontext_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    PyJit_pycontext__getsets,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    offsetof(PyJit_pycontext, inst_dict),                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_pycontext__tp_init,             /* tp_init */
    (allocfunc)PyType_GenericAlloc,           /* tp_alloc */
    (newfunc)PyType_GenericNew,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)NULL,             /* tp_is_gc */
    NULL,                              /* tp_bases */
    NULL,                              /* tp_mro */
    NULL,                              /* tp_cache */
    NULL,                              /* tp_subclasses */
    NULL,                              /* tp_weaklist */
    (destructor) NULL                  /* tp_del */
};


