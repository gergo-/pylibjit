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
_wrap_PyJit_value__get_context(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_pycontext *py_pycontext;
    
    py_pycontext = PyObject_New(PyJit_pycontext, &PyJit_pycontext_Type);
    py_pycontext->obj = new jit_pycontext(self->obj->context());
    py_pycontext->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_pycontext);
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_function(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_function *py_func;
    jit_function *func_t;
    
    py_func = PyObject_New(PyJit_function, &PyJit_function_Type);
    func_t = jit_function::from_raw(self->obj->function());
    if (func_t != NULL) {
        py_func->obj = new jit_function(*func_t);
    } else {
        py_func->obj = new PyJit_function__PythonHelper(self->obj->function());
        ((PyJit_function__PythonHelper*) py_func->obj)->set_pyobj((PyObject *)py_func);
    }
    py_func->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;
    py_retval = Py_BuildValue((char *) "N", py_func);
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_block(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyLong_FromVoidPtr((void *)self->obj->block()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_type(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_type;
    
    py_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_type->obj = new jit_type(self->obj->type());
    py_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_constant(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_constant()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_local(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_local()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_parameter(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_parameter()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_temporary(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_temporary()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_valid(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_valid()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_addressable(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_addressable()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_value__get_is_volatile(PyJit_value *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_volatile()));
    return py_retval;
}

static PyGetSetDef PyJit_value__getsets[] = {
    {
        (char*) "function", /* attribute name */
        (getter) _wrap_PyJit_value__get_function, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_local", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_local, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "type", /* attribute name */
        (getter) _wrap_PyJit_value__get_type, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_addressable", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_addressable, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_parameter", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_parameter, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_valid", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_valid, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_volatile", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_volatile, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "context", /* attribute name */
        (getter) _wrap_PyJit_value__get_context, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_temporary", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_temporary, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_constant", /* attribute name */
        (getter) _wrap_PyJit_value__get_is_constant, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "block", /* attribute name */
        (getter) _wrap_PyJit_value__get_block, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};


static int
_wrap_PyJit_value__tp_init__0(PyJit_value *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    self->obj = new jit_value();
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}


static int
_wrap_PyJit_value__tp_init__1(PyJit_value *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyJit_value *ctor_arg;
    const char *keywords[] = {"ctor_arg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &ctor_arg)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    self->obj = new jit_value(*((PyJit_value *) ctor_arg)->obj);
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}


int _wrap_PyJit_value__tp_init(PyJit_value *self, PyObject *args, PyObject *kwargs)
{
    int retval;
    PyObject *error_list;
    PyObject *exceptions[2] = {0,};
    retval = _wrap_PyJit_value__tp_init__0(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    retval = _wrap_PyJit_value__tp_init__1(self, args, kwargs, &exceptions[1]);
    if (!exceptions[1]) {
        Py_DECREF(exceptions[0]);
        return retval;
    }
    error_list = PyList_New(2);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyList_SET_ITEM(error_list, 1, PyObject_Str(exceptions[1]));
    Py_DECREF(exceptions[1]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return -1;
}


PyObject *
_wrap_PyJit_value_float32_constant(PyJit_value *self)
{
    PyObject *py_retval;
    float retval;
    
    retval = self->obj->float32_constant();
    py_retval = Py_BuildValue((char *) "f", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_value_float64_constant(PyJit_value *self)
{
    PyObject *py_retval;
    float retval;
    
    retval = self->obj->float64_constant();
    py_retval = Py_BuildValue((char *) "f", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_value_long_constant(PyJit_value *self)
{
    PyObject *py_retval;
    long int retval;
    
    retval = self->obj->long_constant();
    py_retval = Py_BuildValue((char *) "l", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_value_nfloat_constant(PyJit_value *self)
{
    PyObject *py_retval;
    float retval;
    
    retval = self->obj->nfloat_constant();
    py_retval = Py_BuildValue((char *) "f", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_value_set_addressable(PyJit_value *self)
{
    PyObject *py_retval;
    
    self->obj->set_addressable();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_value_set_volatile(PyJit_value *self)
{
    PyObject *py_retval;
    
    self->obj->set_volatile();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_value_nint_constant(PyJit_value *self)
{
    PyObject *py_retval;
    unsigned int retval;
    
    retval = self->obj->nint_constant();
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



static PyObject*
_wrap_PyJit_value__copy__(PyJit_value *self)
{

    PyJit_value *py_copy;
    py_copy = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_copy->obj = new jit_value(*self->obj);
    py_copy->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return (PyObject*) py_copy;
}

static PyMethodDef PyJit_value_methods[] = {
    {
        (char *) "float32_constant",
        (PyCFunction) _wrap_PyJit_value_float32_constant,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "float64_constant",
        (PyCFunction) _wrap_PyJit_value_float64_constant,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "long_constant",
        (PyCFunction) _wrap_PyJit_value_long_constant,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "nfloat_constant",
        (PyCFunction) _wrap_PyJit_value_nfloat_constant,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "set_addressable",
        (PyCFunction) _wrap_PyJit_value_set_addressable,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "set_volatile",
        (PyCFunction) _wrap_PyJit_value_set_volatile,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "nint_constant",
        (PyCFunction) _wrap_PyJit_value_nint_constant,
        METH_NOARGS,
        NULL
    },
    {(char *) "__copy__", (PyCFunction) _wrap_PyJit_value__copy__, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static void
_wrap_PyJit_value__tp_dealloc(PyJit_value *self)
{
        jit_value *tmp = self->obj;
        self->obj = NULL;
        if (!(self->flags & PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED)) {
            delete tmp;
        }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
_wrap_PyJit_value__tp_richcompare(PyJit_value *self, PyJit_value *other, int opid)
{
    PyJit_value *retval;
    
    if (!PyObject_IsInstance((PyObject*)other, (PyObject*)&PyJit_value_Type)) {
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    }
    
    switch (opid) {
        case Py_LT:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj < *other->obj);
            return (PyObject*)retval;
        case Py_LE:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj <= *other->obj);
            return (PyObject*)retval;
        case Py_EQ:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj == *other->obj);
            return (PyObject*)retval;
        case Py_NE:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj != *other->obj);
            return (PyObject*)retval;
        case Py_GE:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj >= *other->obj);
            return (PyObject*)retval;
        case Py_GT:
            retval = PyObject_New(PyJit_value, &PyJit_value_Type);
            retval->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
            retval->obj = new jit_value(*self->obj > *other->obj);
            return (PyObject*)retval;
    } /* closes switch (opid) */
    
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}


PyObject* _wrap_convert_c2py__jit_value(jit_value *cvalue)
{
    PyObject *py_retval;
    PyJit_value *py_jit_value;
    
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(*cvalue);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


int _wrap_convert_py2c__jit_value(PyObject *value, jit_value *address)
{
    PyObject *py_retval;
    PyJit_value *tmp_jit_value;
    
    py_retval = Py_BuildValue((char *) "(O)", value);
    if (!PyArg_ParseTuple(py_retval, (char *) "O!", &PyJit_value_Type, &tmp_jit_value)) {
        Py_DECREF(py_retval);
        return 0;
    }
    *address = *tmp_jit_value->obj;
    Py_DECREF(py_retval);
    return 1;
}

static PyObject*
Jit_value__nb_rshift (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left >> right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_lshift (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left << right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_xor (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left ^ right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_remainder (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left % right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_and (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left & right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_add (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left + right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_multiply (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left * right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_subtract (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left - right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_true_divide (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left / right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_or (PyObject *py_left, PyObject *py_right)
{
    {
        jit_value left;
        jit_value right;
        if (_wrap_convert_py2c__jit_value(py_left, &left) && _wrap_convert_py2c__jit_value(py_right, &right)) {
            jit_value result = (left | right);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_negative (PyObject *py_self)
{
    {
        jit_value self;
        if (_wrap_convert_py2c__jit_value(py_self, &self)) {
            jit_value result = -(self);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyObject*
Jit_value__nb_invert (PyObject *py_self)
{
    {
        jit_value self;
        if (_wrap_convert_py2c__jit_value(py_self, &self)) {
            jit_value result = ~(self);
            return _wrap_convert_c2py__jit_value(&result);
        }
        PyErr_Clear();
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}
static PyNumberMethods Jit_value__py_number_methods = {
    (binaryfunc) Jit_value__nb_add,  /* nb_add */
    (binaryfunc) Jit_value__nb_subtract,  /* nb_subtract */
    (binaryfunc) Jit_value__nb_multiply,  /* nb_multiply */
#if PY_VERSION_HEX < 0x03000000
    (binaryfunc) NULL,  /* nb_divide */
#endif
    (binaryfunc) Jit_value__nb_remainder,  /* nb_remainder */
    (binaryfunc) NULL,  /* nb_divmod */
    (ternaryfunc) NULL,  /* nb_power */
    (unaryfunc) Jit_value__nb_negative,  /* nb_negative */
    (unaryfunc) NULL,  /* nb_positive */
    (unaryfunc) NULL,  /* nb_absolute */
    (inquiry) NULL,  /* nb_nonzero */
    (unaryfunc) Jit_value__nb_invert,  /* nb_invert */
    (binaryfunc) Jit_value__nb_lshift,  /* nb_lshift */
    (binaryfunc) Jit_value__nb_rshift,  /* nb_rshift */
    (binaryfunc) Jit_value__nb_and,  /* nb_and */
    (binaryfunc) Jit_value__nb_xor,  /* nb_xor */
    (binaryfunc) Jit_value__nb_or,  /* nb_or */
#if PY_VERSION_HEX < 0x03000000
    (coercion) NULL,  /* nb_coerce */
#endif
    (unaryfunc) NULL,  /* nb_int */
#if PY_VERSION_HEX >= 0x03000000
    (void*) NULL,  /* nb_long */
#else
    (unaryfunc) NULL,  /* nb_long */
#endif
    (unaryfunc) NULL,  /* nb_float */
#if PY_VERSION_HEX < 0x03000000
    (unaryfunc) NULL,  /* nb_oct */
    (unaryfunc) NULL,  /* nb_hex */
#endif
    /* Added in release 2.0 */
    (binaryfunc) NULL,  /* nb_inplace_add */
    (binaryfunc) NULL,  /* nb_inplace_subtract */
    (binaryfunc) NULL,  /* nb_inplace_multiply */
#if PY_VERSION_HEX < 0x03000000
    (binaryfunc) NULL,  /* nb_inplace_divide */
#endif
    (binaryfunc) NULL,  /* nb_inplace_remainder */
    (ternaryfunc) NULL,  /* nb_inplace_power */
    (binaryfunc) NULL,  /* nb_inplace_lshift */
    (binaryfunc) NULL,  /* nb_inplace_rshift */
    (binaryfunc) NULL,  /* nb_inplace_and */
    (binaryfunc) NULL,  /* nb_inplace_xor */
    (binaryfunc) NULL,  /* nb_inplace_or */

    /* Added in release 2.2 */
    /* The following require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc) NULL,  /* nb_floor_divide */
    (binaryfunc) Jit_value__nb_true_divide,  /* nb_true_divide */
    (binaryfunc) NULL,  /* nb_inplace_floor_divide */
    (binaryfunc) NULL,  /* nb_inplace_true_divide */

#if PY_VERSION_HEX >= 0x020500F0
    /* Added in release 2.5 */
    (unaryfunc) NULL,  /* nb_index */

#endif
};

PyTypeObject PyJit_value_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Value",            /* tp_name */
    sizeof(PyJit_value),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_value__tp_dealloc,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)NULL,       /* tp_getattr */
    (setattrfunc)NULL,       /* tp_setattr */
    (cmpfunc)NULL,           /* tp_compare */
    (reprfunc)NULL,             /* tp_repr */
    (PyNumberMethods*)&Jit_value__py_number_methods,     /* tp_as_number */
    (PySequenceMethods*)NULL, /* tp_as_sequence */
    (PyMappingMethods*)NULL,   /* tp_as_mapping */
    (hashfunc)NULL,             /* tp_hash */
    (ternaryfunc)NULL,          /* tp_call */
    (reprfunc)NULL,              /* tp_str */
    (getattrofunc)NULL,     /* tp_getattro */
    (setattrofunc)NULL,     /* tp_setattro */
    (PyBufferProcs*)NULL,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_CHECKTYPES,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)NULL,     /* tp_traverse */
    (inquiry)NULL,             /* tp_clear */
    (richcmpfunc)_wrap_PyJit_value__tp_richcompare,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_value_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    PyJit_value__getsets,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_value__tp_init,             /* tp_init */
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


