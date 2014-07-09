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






static int
_wrap_PyJit_readelf__tp_init(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    char const *filename;
    int flags = 0;
    const char *keywords[] = {"filename", "flags", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s|i", (char **) keywords, &filename, &flags)) {
        return -1;
    }
    if (jit_readelf_open(&self->obj, filename, flags) != JIT_READELF_OK) {
        PyErr_SetString(PyExc_TypeError, "ReadElf failed to open library");
        return -1;
    }
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}



PyObject *
_wrap_jit_elf_jit_readelf_get_needed(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    char const *retval;
    unsigned int index;
    const char *keywords[] = {"index", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &index)) {
        return NULL;
    }
    retval = jit_readelf_get_needed(self->obj, index);
    py_retval = Py_BuildValue((char *) "s", retval);
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_readelf_get_section(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    void *retval;
    char const *name;
    const char *keywords[] = {"name", NULL};
    jit_nuint size;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &name)) {
        return NULL;
    }
    retval = jit_readelf_get_section(self->obj, name, &size);
    py_retval = Py_BuildValue((char *) "N", PyBytes_FromStringAndSize((const char *)retval, size));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_readelf_get_symbol(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    void *retval;
    char const *name;
    const char *keywords[] = {"name", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &name)) {
        return NULL;
    }
    retval = jit_readelf_get_symbol(self->obj, name);
    py_retval = Py_BuildValue((char *) "N", PyLong_FromVoidPtr((void *)retval));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_readelf_close(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return NULL;
    }
    jit_readelf_close(self->obj);
    self->obj = NULL;
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_readelf_num_needed(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    unsigned int retval;
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        return NULL;
    }
    retval = jit_readelf_num_needed(self->obj);
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_readelf_add_to_context(PyJit_readelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_pycontext *py_context = NULL;
    const char *keywords[] = {"context", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_pycontext_Type, &py_context)) {
        return NULL;
    }
    jit_readelf_add_to_context(self->obj, py_context->obj->raw());
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


static PyMethodDef PyJit_readelf_methods[] = {
    {
        (char *) "get_needed",
        (PyCFunction) _wrap_jit_elf_jit_readelf_get_needed,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_section",
        (PyCFunction) _wrap_jit_elf_jit_readelf_get_section,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_symbol",
        (PyCFunction) _wrap_jit_elf_jit_readelf_get_symbol,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "close",
        (PyCFunction) _wrap_jit_elf_jit_readelf_close,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "num_needed",
        (PyCFunction) _wrap_jit_elf_jit_readelf_num_needed,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "add_to_context",
        (PyCFunction) _wrap_jit_elf_jit_readelf_add_to_context,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {NULL, NULL, 0, NULL}
};

static void
_wrap_PyJit_readelf__tp_dealloc(PyJit_readelf *self)
{
    if (self->obj) {
        jit_readelf_t tmp = self->obj;
        self->obj = NULL;
        jit_readelf_close(tmp);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_readelf_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.elf.ReadElf",            /* tp_name */
    sizeof(PyJit_readelf),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_readelf__tp_dealloc,        /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)NULL,     /* tp_traverse */
    (inquiry)NULL,             /* tp_clear */
    (richcmpfunc)NULL,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_readelf_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    0,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_readelf__tp_init,             /* tp_init */
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




static int
_wrap_PyJit_writeelf__tp_init(PyJit_writeelf *self, PyObject *args, PyObject *kwargs)
{
    char const *library_name;
    const char *keywords[] = {"library_name", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &library_name)) {
        return -1;
    }
    self->obj = jit_writeelf_create(library_name);
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}



PyObject *
_wrap_jit_elf_jit_writeelf_write(PyJit_writeelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    char const *filename;
    const char *keywords[] = {"filename", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &filename)) {
        return NULL;
    }
    retval = jit_writeelf_write(self->obj, filename);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_writeelf_add_function(PyJit_writeelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    PyJit_function *py_function = NULL;
    char const *name;
    const char *keywords[] = {"function", "name", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!s", (char **) keywords, &PyJit_function_Type, &py_function, &name)) {
        return NULL;
    }
    retval = jit_writeelf_add_function(self->obj, py_function->obj->raw(), name);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_writeelf_write_section(PyJit_writeelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    char const *name;
    int type;
    PyObject *buf;
    int discardable;
    const char *keywords[] = {"name", "type", "buf", "discardable", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "siO!i", (char **) keywords, &name, &type, &PyBytes_Type, &buf, &discardable)) {
        return NULL;
    }
    retval = jit_writeelf_write_section(self->obj, name, type, (void *)PyBytes_AsString(buf), PyBytes_Size(buf), discardable);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_jit_elf_jit_writeelf_add_needed(PyJit_writeelf *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    char const *library_name;
    const char *keywords[] = {"library_name", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &library_name)) {
        return NULL;
    }
    retval = jit_writeelf_add_needed(self->obj, library_name);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}


static PyMethodDef PyJit_writeelf_methods[] = {
    {
        (char *) "write",
        (PyCFunction) _wrap_jit_elf_jit_writeelf_write,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "add_function",
        (PyCFunction) _wrap_jit_elf_jit_writeelf_add_function,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "write_section",
        (PyCFunction) _wrap_jit_elf_jit_writeelf_write_section,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "add_needed",
        (PyCFunction) _wrap_jit_elf_jit_writeelf_add_needed,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {NULL, NULL, 0, NULL}
};

static void
_wrap_PyJit_writeelf__tp_dealloc(PyJit_writeelf *self)
{
    if (self->obj) {
        jit_writeelf_t tmp = self->obj;
        self->obj = NULL;
        jit_writeelf_destroy(tmp);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_writeelf_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.elf.WriteElf",            /* tp_name */
    sizeof(PyJit_writeelf),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_writeelf__tp_dealloc,        /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)NULL,     /* tp_traverse */
    (inquiry)NULL,             /* tp_clear */
    (richcmpfunc)NULL,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_writeelf_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    0,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_writeelf__tp_init,             /* tp_init */
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


