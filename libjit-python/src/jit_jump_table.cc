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
_wrap_PyJit_jump_table__tp_init(PyJit_jump_table *self, PyObject *args, PyObject *kwargs)
{
    int size;
    const char *keywords[] = {"size", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &size)) {
        return -1;
    }
    self->obj = new jit_jump_table(size);
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}



PyObject *
_wrap_PyJit_jump_table_get(PyJit_jump_table *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int index;
    const char *keywords[] = {"index", NULL};
    PyJit_label *py_jit_label;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &index)) {
        return NULL;
    }
    jit_label retval = self->obj->get(index);
    py_jit_label = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_jit_label->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_label->obj = new jit_label(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_label);
    return py_retval;
}



PyObject *
_wrap_PyJit_jump_table_set(PyJit_jump_table *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int index;
    PyJit_label *label;
    const char *keywords[] = {"index", "label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "iO!", (char **) keywords, &index, &PyJit_label_Type, &label)) {
        return NULL;
    }
    self->obj->set(index, *((PyJit_label *) label)->obj);
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_jump_table_size(PyJit_jump_table *self)
{
    PyObject *py_retval;
    int retval;
    
    retval = self->obj->size();
    py_retval = Py_BuildValue((char *) "i", retval);
    return py_retval;
}


static PyMethodDef PyJit_jump_table_methods[] = {
    {
        (char *) "get",
        (PyCFunction) _wrap_PyJit_jump_table_get,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "set",
        (PyCFunction) _wrap_PyJit_jump_table_set,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "size",
        (PyCFunction) _wrap_PyJit_jump_table_size,
        METH_NOARGS,
        NULL
    },
    {NULL, NULL, 0, NULL}
};

static void
_wrap_PyJit_jump_table__tp_dealloc(PyJit_jump_table *self)
{
        jit_jump_table *tmp = self->obj;
        self->obj = NULL;
        if (!(self->flags & PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED)) {
            delete tmp;
        }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_jump_table_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.JumpTable",            /* tp_name */
    sizeof(PyJit_jump_table),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_jump_table__tp_dealloc,        /* tp_dealloc */
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
    (struct PyMethodDef*)PyJit_jump_table_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    0,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_jump_table__tp_init,             /* tp_init */
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


