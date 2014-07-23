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


PyObject *
PyJit_function__PythonHelper::_wrap_fail(PyJit_function *self)
{
    PyObject *py_retval;
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);
    
    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method fail of class jit_function is protected and can only be called by a subclass");
        return NULL;
    }
    helper->fail__parent_caller();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject *
PyJit_function__PythonHelper::_wrap_create_signature(PyJit_function *self)
{
    PyObject *py_retval;
    jit_type_t retval;
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);
    PyJit_type *py_type;
    
    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method create_signature of class jit_function is protected and can only be called by a subclass");
        return NULL;
    }
    retval = helper->create_signature__parent_caller();
    py_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_type->obj = new jit_type(retval);
    py_type->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;
    py_retval = Py_BuildValue((char *) "N", py_type);
    return py_retval;
}


PyObject *
PyJit_function__PythonHelper::_wrap_build(PyJit_function *self)
{
    PyObject *py_retval;
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);
    
    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method build of class jit_function is protected and can only be called by a subclass");
        return NULL;
    }
    helper->build__parent_caller();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject *
PyJit_function__PythonHelper::_wrap_out_of_memory(PyJit_function *self)
{
    PyObject *py_retval;
    PyJit_function__PythonHelper *helper = dynamic_cast< PyJit_function__PythonHelper* >(self->obj);
    
    if (helper == NULL) {
        PyErr_SetString(PyExc_TypeError, "Method out_of_memory of class jit_function is protected and can only be called by a subclass");
        return NULL;
    }
    helper->out_of_memory__parent_caller();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


void
PyJit_function__PythonHelper::build()
{
    PyGILState_STATE __py_gil_state;
    PyObject *py_method;
    jit_function *self_obj_before;
    PyObject *py_retval;
    
    __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);
    py_method = PyObject_GetAttrString(m_pyself, (char *) "build"); PyErr_Clear();
    if (py_method == NULL || Py_TYPE(py_method) == &PyCFunction_Type) {
        jit_function::build();
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return;
    }
    self_obj_before = reinterpret_cast< PyJit_function* >(m_pyself)->obj;
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = (jit_function*) this;
    py_retval = PyObject_CallMethod(m_pyself, (char *) "build", (char *) "");
    if (py_retval == NULL) {
        PyErr_Print();
        reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return;
    }
    if (py_retval != Py_None) {
        PyErr_SetString(PyExc_TypeError, "function/method should return None");
        Py_DECREF(py_retval);
        reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return;
    }
    Py_DECREF(py_retval);
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
    Py_XDECREF(py_method);
    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);
    return;
}

jit_type_t
PyJit_function__PythonHelper::create_signature()
{
    PyGILState_STATE __py_gil_state;
    PyObject *py_method;
    jit_function *self_obj_before;
    PyObject *py_retval;
    jit_type_t retval;
    PyJit_type *py_type;
    
    __py_gil_state = (PyEval_ThreadsInitialized() ? PyGILState_Ensure() : (PyGILState_STATE) 0);
    py_method = PyObject_GetAttrString(m_pyself, (char *) "create_signature"); PyErr_Clear();
    if (py_method == NULL || Py_TYPE(py_method) == &PyCFunction_Type) {
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return jit_function::create_signature();
    }
    self_obj_before = reinterpret_cast< PyJit_function* >(m_pyself)->obj;
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = (jit_function*) this;
    py_retval = PyObject_CallMethod(m_pyself, (char *) "create_signature", (char *) "");
    if (py_retval == NULL) {
        PyErr_Print();
        reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return jit_function::create_signature();
    }
    py_retval = Py_BuildValue((char*) "(N)", py_retval);
    if (!PyArg_ParseTuple(py_retval, (char *) "O!", &PyJit_type_Type, &py_type)) {
        PyErr_Print();
        Py_DECREF(py_retval);
        reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
        Py_XDECREF(py_method);
        if (PyEval_ThreadsInitialized())
            PyGILState_Release(__py_gil_state);
        return jit_function::create_signature();
    }
    retval = py_type->obj->raw();
    Py_DECREF(py_retval);
    reinterpret_cast< PyJit_function* >(m_pyself)->obj = self_obj_before;
    Py_XDECREF(py_method);
    if (PyEval_ThreadsInitialized())
        PyGILState_Release(__py_gil_state);
    return retval;
}
static PyObject *
_wrap_PyJit_function__get_optimization_level(PyJit_function *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(self->obj->optimization_level()));
    return py_retval;
}

static int
_wrap_PyJit_function__set_optimization_level(PyJit_function *self, PyObject *value, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    unsigned int tmp_value;
    
    py_retval = Py_BuildValue((char *) "(O)", value);
    if (!PyArg_ParseTuple(py_retval, (char *) "I", &tmp_value)) {
        Py_DECREF(py_retval);
        return -1;
    }
    self->obj->set_optimization_level(tmp_value);
    Py_DECREF(py_retval);
    return 0;
}

static PyObject *
_wrap_PyJit_function__get_is_compiled(PyJit_function *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_compiled()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_function__get_is_recompilable(PyJit_function *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_recompilable()));
    return py_retval;
}

static int
_wrap_PyJit_function__set_is_recompilable(PyJit_function *self, PyObject *value, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    bool tmp_value;
    PyObject *py_boolretval;
    
    py_retval = Py_BuildValue((char *) "(O)", value);
    if (!PyArg_ParseTuple(py_retval, (char *) "O", &py_boolretval)) {
        Py_DECREF(py_retval);
        return -1;
    }
    tmp_value = PyObject_IsTrue(py_boolretval);
    self->obj->set_recompilable(tmp_value);
    Py_DECREF(py_retval);
    return 0;
}

static PyObject *
_wrap_PyJit_function__get_is_valid(PyJit_function *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_valid()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_function__get_signature(PyJit_function *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_type;
    
    py_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_type->obj = new jit_type(self->obj->signature());
    py_type->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;
    py_retval = Py_BuildValue((char *) "N", py_type);
    return py_retval;
}

static PyGetSetDef PyJit_function__getsets[] = {
    {
        (char*) "is_compiled", /* attribute name */
        (getter) _wrap_PyJit_function__get_is_compiled, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_valid", /* attribute name */
        (getter) _wrap_PyJit_function__get_is_valid, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_recompilable", /* attribute name */
        (getter) _wrap_PyJit_function__get_is_recompilable, /* C function to get the attribute */
        (setter) _wrap_PyJit_function__set_is_recompilable, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "optimization_level", /* attribute name */
        (getter) _wrap_PyJit_function__get_optimization_level, /* C function to get the attribute */
        (setter) _wrap_PyJit_function__set_optimization_level, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "signature", /* attribute name */
        (getter) _wrap_PyJit_function__get_signature, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};


static int
_wrap_PyJit_function__tp_init__0(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyJit_pycontext *context;
    const char *keywords[] = {"context", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_pycontext_Type, &context)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    if (Py_TYPE(self) != &PyJit_function_Type)
    {
        self->obj = new PyJit_function__PythonHelper(*((PyJit_pycontext *) context)->obj);
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
        ((PyJit_function__PythonHelper*) self->obj)->set_pyobj((PyObject *)self);
    } else {
        // visibility: 'public'
        self->obj = new jit_function(*((PyJit_pycontext *) context)->obj);
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    }
    return 0;
}


static int
_wrap_PyJit_function__tp_init__1(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyJit_pycontext *context;
    PyJit_type *py_signature = NULL;
    const char *keywords[] = {"context", "signature", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_pycontext_Type, &context, &PyJit_type_Type, &py_signature)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    if (Py_TYPE(self) != &PyJit_function_Type)
    {
        self->obj = new PyJit_function__PythonHelper(*((PyJit_pycontext *) context)->obj, py_signature->obj->raw());
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
        ((PyJit_function__PythonHelper*) self->obj)->set_pyobj((PyObject *)self);
    } else {
        // visibility: 'public'
        self->obj = new jit_function(*((PyJit_pycontext *) context)->obj, py_signature->obj->raw());
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    }
    return 0;
}


static int
_wrap_PyJit_function__tp_init__2(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyJit_function *ctor_arg;
    const char *keywords[] = {"ctor_arg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_function_Type, &ctor_arg)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    if (Py_TYPE(self) != &PyJit_function_Type)
    {
        self->obj = new PyJit_function__PythonHelper(*((PyJit_function *) ctor_arg)->obj);
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
        ((PyJit_function__PythonHelper*) self->obj)->set_pyobj((PyObject *)self);
    } else {
        // visibility: 'public'
        self->obj = new jit_function(*((PyJit_function *) ctor_arg)->obj);
        self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    }
    return 0;
}


int _wrap_PyJit_function__tp_init(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    int retval;
    PyObject *error_list;
    PyObject *exceptions[3] = {0,};
    retval = _wrap_PyJit_function__tp_init__0(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    retval = _wrap_PyJit_function__tp_init__1(self, args, kwargs, &exceptions[1]);
    if (!exceptions[1]) {
        Py_DECREF(exceptions[0]);
        return retval;
    }
    retval = _wrap_PyJit_function__tp_init__2(self, args, kwargs, &exceptions[2]);
    if (!exceptions[2]) {
        Py_DECREF(exceptions[0]);
        Py_DECREF(exceptions[1]);
        return retval;
    }
    error_list = PyList_New(3);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyList_SET_ITEM(error_list, 1, PyObject_Str(exceptions[1]));
    Py_DECREF(exceptions[1]);
    PyList_SET_ITEM(error_list, 2, PyObject_Str(exceptions[2]));
    Py_DECREF(exceptions[2]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return -1;
}


PyObject *
_wrap_PyJit_function_insn_sign(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sign(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sub(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sub(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_min(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_min(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call_filter(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_label *label;
    PyJit_value *value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"label", "value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_label_Type, &label, &PyJit_value_Type, &value, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_call_filter(*((PyJit_label *) label)->obj, *((PyJit_value *) value)->obj, py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_cos(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_cos(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_ceil(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_ceil(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_not(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_not(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_branch_if_not(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_label *label;
    const char *keywords[] = {"value", "label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_branch_if_not(*((PyJit_value *) value)->obj, *((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_push(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_push(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}




PyObject *
_wrap_PyJit_function_new_constant__0(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    char value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "c|O!", (char **) keywords, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant(value, py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject *
_wrap_PyJit_function_new_constant__1(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    int value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i|O!", (char **) keywords, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    if (value > 0xff) {
        PyErr_SetString(PyExc_ValueError, "Out of range");
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant(value, py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject *
_wrap_PyJit_function_new_constant__2(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    int value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i|O!", (char **) keywords, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant(value, py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject *
_wrap_PyJit_function_new_constant__3(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    unsigned int value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I|O!", (char **) keywords, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant(value, py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject *
_wrap_PyJit_function_new_constant__4(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    PyObject *value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|O!", (char **) keywords, &PyLong_Type, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant((void *)PyLong_AsVoidPtr(value), py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject *
_wrap_PyJit_function_new_constant__5(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    jit_value retval;
    double value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "d|O!", (char **) keywords, &value, &PyJit_type_Type, &py_type)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        retval = self->obj->new_constant(value, py_type ? py_type->obj->raw() : 0);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}


PyObject * _wrap_PyJit_function_new_constant(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[6] = {0,};
    retval = _wrap_PyJit_function_new_constant__0(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    retval = _wrap_PyJit_function_new_constant__1(self, args, kwargs, &exceptions[1]);
    if (!exceptions[1]) {
        Py_DECREF(exceptions[0]);
        return retval;
    }
    retval = _wrap_PyJit_function_new_constant__2(self, args, kwargs, &exceptions[2]);
    if (!exceptions[2]) {
        Py_DECREF(exceptions[0]);
        Py_DECREF(exceptions[1]);
        return retval;
    }
    retval = _wrap_PyJit_function_new_constant__3(self, args, kwargs, &exceptions[3]);
    if (!exceptions[3]) {
        Py_DECREF(exceptions[0]);
        Py_DECREF(exceptions[1]);
        Py_DECREF(exceptions[2]);
        return retval;
    }
    retval = _wrap_PyJit_function_new_constant__4(self, args, kwargs, &exceptions[4]);
    if (!exceptions[4]) {
        Py_DECREF(exceptions[0]);
        Py_DECREF(exceptions[1]);
        Py_DECREF(exceptions[2]);
        Py_DECREF(exceptions[3]);
        return retval;
    }
    retval = _wrap_PyJit_function_new_constant__5(self, args, kwargs, &exceptions[5]);
    if (!exceptions[5]) {
        Py_DECREF(exceptions[0]);
        Py_DECREF(exceptions[1]);
        Py_DECREF(exceptions[2]);
        Py_DECREF(exceptions[3]);
        Py_DECREF(exceptions[4]);
        return retval;
    }
    error_list = PyList_New(6);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyList_SET_ITEM(error_list, 1, PyObject_Str(exceptions[1]));
    Py_DECREF(exceptions[1]);
    PyList_SET_ITEM(error_list, 2, PyObject_Str(exceptions[2]));
    Py_DECREF(exceptions[2]);
    PyList_SET_ITEM(error_list, 3, PyObject_Str(exceptions[3]));
    Py_DECREF(exceptions[3]);
    PyList_SET_ITEM(error_list, 4, PyObject_Str(exceptions[4]));
    Py_DECREF(exceptions[4]);
    PyList_SET_ITEM(error_list, 5, PyObject_Str(exceptions[5]));
    Py_DECREF(exceptions[5]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return NULL;
}


PyObject *
_wrap_PyJit_function_insn_atan(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_atan(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_label(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_label(*((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_start_finally(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_start_finally(*((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_ushr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_ushr(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}





PyObject *
_wrap_Jit_function___enter__(PyJit_function *self, PyObject **return_exception)
{
    self->obj->build_start();

    Py_INCREF(self);
    return (PyObject *)self;
}


PyObject * _wrap_PyJit_function___enter__(PyJit_function *self)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[1] = {0,};
    retval = _wrap_Jit_function___enter__(self, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    error_list = PyList_New(1);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return NULL;
}


PyObject *
_wrap_PyJit_function_insn_branch_if_pc_not_in_range(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *start_label;
    PyJit_label *end_label;
    PyJit_label *label;
    const char *keywords[] = {"start_label", "end_label", "label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_label_Type, &start_label, &PyJit_label_Type, &end_label, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_branch_if_pc_not_in_range(*((PyJit_label *) start_label)->obj, *((PyJit_label *) end_label)->obj, *((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_floor(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_floor(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_move_blocks_to_end(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *from_label;
    PyJit_label *to_label;
    const char *keywords[] = {"from_label", "to_label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_label_Type, &from_label, &PyJit_label_Type, &to_label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_move_blocks_to_end(*((PyJit_label *) from_label)->obj, *((PyJit_label *) to_label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_add_ovf(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_add_ovf(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_rem(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_rem(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_mark_offset(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int offset;
    const char *keywords[] = {"offset", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &offset)) {
        return NULL;
    }
    try
    {
        self->obj->insn_mark_offset(offset);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_address_of_label(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_address_of_label(*((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_return_from_finally(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_return_from_finally();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_store_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *dest;
    unsigned int offset;
    PyJit_value *value;
    const char *keywords[] = {"dest", "offset", "value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!IO!", (char **) keywords, &PyJit_value_Type, &dest, &offset, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_store_relative(*((PyJit_value *) dest)->obj, offset, *((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sshr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sshr(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_incoming_reg(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    int reg;
    const char *keywords[] = {"value", "reg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!i", (char **) keywords, &PyJit_value_Type, &value, &reg)) {
        return NULL;
    }
    try
    {
        self->obj->insn_incoming_reg(*((PyJit_value *) value)->obj, reg);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_store_elem(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *base_addr;
    PyJit_value *index;
    PyJit_value *value;
    const char *keywords[] = {"base_addr", "index", "value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_store_elem(*((PyJit_value *) base_addr)->obj, *((PyJit_value *) index)->obj, *((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_load_elem(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *base_addr;
    PyJit_value *index;
    PyJit_type *py_elem_type = NULL;
    const char *keywords[] = {"base_addr", "index", "elem_type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_type_Type, &py_elem_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_load_elem(*((PyJit_value *) base_addr)->obj, *((PyJit_value *) index)->obj, py_elem_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_closure(PyJit_function *self)
{
    PyObject *py_retval;
    void *retval;
    
    retval = self->obj->closure();
    py_retval = Py_BuildValue((char *) "N", PyLong_FromVoidPtr((void *)retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_memmove(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *dest;
    PyJit_value *src;
    PyJit_value *size;
    const char *keywords[] = {"dest", "src", "size", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &src, &PyJit_value_Type, &size)) {
        return NULL;
    }
    try
    {
        self->obj->insn_memmove(*((PyJit_value *) dest)->obj, *((PyJit_value *) src)->obj, *((PyJit_value *) size)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_memset(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *dest;
    PyJit_value *value;
    PyJit_value *size;
    const char *keywords[] = {"dest", "value", "size", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &value, &PyJit_value_Type, &size)) {
        return NULL;
    }
    try
    {
        self->obj->insn_memset(*((PyJit_value *) dest)->obj, *((PyJit_value *) value)->obj, *((PyJit_value *) size)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_mark_breakpoint(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    unsigned int data1;
    unsigned int data2;
    const char *keywords[] = {"data1", "data2", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "II", (char **) keywords, &data1, &data2)) {
        return NULL;
    }
    try
    {
        self->obj->insn_mark_breakpoint(data1, data2);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_start_filter(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_label *label;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"label", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_label_Type, &label, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_start_filter(*((PyJit_label *) label)->obj, py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_vtable_pointer(PyJit_function *self)
{
    PyObject *py_retval;
    void *retval;
    
    retval = self->obj->vtable_pointer();
    py_retval = Py_BuildValue((char *) "N", PyLong_FromVoidPtr((void *)retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_return_reg(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    int reg;
    const char *keywords[] = {"value", "reg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!i", (char **) keywords, &PyJit_value_Type, &value, &reg)) {
        return NULL;
    }
    try
    {
        self->obj->insn_return_reg(*((PyJit_value *) value)->obj, reg);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_set_param_ptr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_type *py_type = NULL;
    unsigned int offset;
    const char *keywords[] = {"value", "type", "offset", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!I", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_type, &offset)) {
        return NULL;
    }
    try
    {
        self->obj->insn_set_param_ptr(*((PyJit_value *) value)->obj, py_type->obj->raw(), offset);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sub_ovf(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sub_ovf(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_dump(PyJit_function *self)
{
    PyObject *py_retval;
    
    self->obj->dump();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_ge(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_ge(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_new_block(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_new_block();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_load_small(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_load_small(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_default_return(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_default_return();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_gt(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_gt(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_add_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    unsigned int offset;
    const char *keywords[] = {"value", "offset", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!I", (char **) keywords, &PyJit_value_Type, &value, &offset)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_add_relative(*((PyJit_value *) value)->obj, offset);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_asin(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_asin(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_rem_ieee(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_rem_ieee(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_import(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_import(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_load(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_load(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_and(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_and(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_to_not_bool(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_to_not_bool(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_is_nan(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_is_nan(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}




PyObject *
_wrap_PyJit_function_insn_return__0(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        self->obj->insn_return(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject *
_wrap_PyJit_function_insn_return__1(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    try
    {
        self->obj->insn_return();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject * _wrap_PyJit_function_insn_return(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[2] = {0,};
    retval = _wrap_PyJit_function_insn_return__0(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    retval = _wrap_PyJit_function_insn_return__1(self, args, kwargs, &exceptions[1]);
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
    return NULL;
}


PyObject *
_wrap_PyJit_function_insn_uses_catcher(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_uses_catcher();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_push_ptr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        self->obj->insn_push_ptr(*((PyJit_value *) value)->obj, py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_throw(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_throw(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_get_struct_pointer(PyJit_function *self)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *py_jit_value;
    
    try
    {
        retval = self->obj->get_struct_pointer();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_convert(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    PyJit_type *py_type = NULL;
    int overflow_check = 0;
    const char *keywords[] = {"value", "type", "overflow_check", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_type, &overflow_check)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_convert(*((PyJit_value *) value)->obj, py_type->obj->raw(), overflow_check);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_signature_helper(PyJit_function *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type_t retval;
    PyJit_type *py_return_type = NULL;
    PyJit_type *py_arg0 = NULL;
    PyJit_type *py_arg1 = NULL;
    PyJit_type *py_arg2 = NULL;
    PyJit_type *py_arg3 = NULL;
    PyJit_type *py_arg4 = NULL;
    PyJit_type *py_arg5 = NULL;
    PyJit_type *py_arg6 = NULL;
    PyJit_type *py_arg7 = NULL;
    PyJit_type *py_arg8 = NULL;
    PyJit_type *py_arg9 = NULL;
    PyJit_type *py_arg10 = NULL;
    PyJit_type *py_arg11 = NULL;
    PyJit_type *py_arg12 = NULL;
    PyJit_type *py_arg13 = NULL;
    PyJit_type *py_arg14 = NULL;
    PyJit_type *py_arg15 = NULL;
    PyJit_type *py_arg16 = NULL;
    PyJit_type *py_arg17 = NULL;
    PyJit_type *py_arg18 = NULL;
    PyJit_type *py_arg19 = NULL;
    PyJit_type *py_arg20 = NULL;
    PyJit_type *py_arg21 = NULL;
    PyJit_type *py_arg22 = NULL;
    PyJit_type *py_arg23 = NULL;
    PyJit_type *py_arg24 = NULL;
    PyJit_type *py_arg25 = NULL;
    PyJit_type *py_arg26 = NULL;
    PyJit_type *py_arg27 = NULL;
    PyJit_type *py_arg28 = NULL;
    PyJit_type *py_arg29 = NULL;
    PyJit_type *py_arg30 = NULL;
    PyJit_type *py_arg31 = NULL;
    char const *name = "<sig>";
    const char *keywords[] = {"return_type", "arg0", "arg1", "arg2", "arg3", "arg4", "arg5", "arg6", "arg7", "arg8", "arg9", "arg10", "arg11", "arg12", "arg13", "arg14", "arg15", "arg16", "arg17", "arg18", "arg19", "arg20", "arg21", "arg22", "arg23", "arg24", "arg25", "arg26", "arg27", "arg28", "arg29", "arg30", "arg31", "name", NULL};
    PyJit_type *py_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!O!s", (char **) keywords, &PyJit_type_Type, &py_return_type, &PyJit_type_Type, &py_arg0, &PyJit_type_Type, &py_arg1, &PyJit_type_Type, &py_arg2, &PyJit_type_Type, &py_arg3, &PyJit_type_Type, &py_arg4, &PyJit_type_Type, &py_arg5, &PyJit_type_Type, &py_arg6, &PyJit_type_Type, &py_arg7, &PyJit_type_Type, &py_arg8, &PyJit_type_Type, &py_arg9, &PyJit_type_Type, &py_arg10, &PyJit_type_Type, &py_arg11, &PyJit_type_Type, &py_arg12, &PyJit_type_Type, &py_arg13, &PyJit_type_Type, &py_arg14, &PyJit_type_Type, &py_arg15, &PyJit_type_Type, &py_arg16, &PyJit_type_Type, &py_arg17, &PyJit_type_Type, &py_arg18, &PyJit_type_Type, &py_arg19, &PyJit_type_Type, &py_arg20, &PyJit_type_Type, &py_arg21, &PyJit_type_Type, &py_arg22, &PyJit_type_Type, &py_arg23, &PyJit_type_Type, &py_arg24, &PyJit_type_Type, &py_arg25, &PyJit_type_Type, &py_arg26, &PyJit_type_Type, &py_arg27, &PyJit_type_Type, &py_arg28, &PyJit_type_Type, &py_arg29, &PyJit_type_Type, &py_arg30, &PyJit_type_Type, &py_arg31, &name)) {
        return NULL;
    }
    retval = jit_function::signature_helper(py_return_type->obj->raw(), py_arg0 ? py_arg0->obj->raw() : 0, py_arg1 ? py_arg1->obj->raw() : 0, py_arg2 ? py_arg2->obj->raw() : 0, py_arg3 ? py_arg3->obj->raw() : 0, py_arg4 ? py_arg4->obj->raw() : 0, py_arg5 ? py_arg5->obj->raw() : 0, py_arg6 ? py_arg6->obj->raw() : 0, py_arg7 ? py_arg7->obj->raw() : 0, py_arg8 ? py_arg8->obj->raw() : 0, py_arg9 ? py_arg9->obj->raw() : 0, py_arg10 ? py_arg10->obj->raw() : 0, py_arg11 ? py_arg11->obj->raw() : 0, py_arg12 ? py_arg12->obj->raw() : 0, py_arg13 ? py_arg13->obj->raw() : 0, py_arg14 ? py_arg14->obj->raw() : 0, py_arg15 ? py_arg15->obj->raw() : 0, py_arg16 ? py_arg16->obj->raw() : 0, py_arg17 ? py_arg17->obj->raw() : 0, py_arg18 ? py_arg18->obj->raw() : 0, py_arg19 ? py_arg19->obj->raw() : 0, py_arg20 ? py_arg20->obj->raw() : 0, py_arg21 ? py_arg21->obj->raw() : 0, py_arg22 ? py_arg22->obj->raw() : 0, py_arg23 ? py_arg23->obj->raw() : 0, py_arg24 ? py_arg24->obj->raw() : 0, py_arg25 ? py_arg25->obj->raw() : 0, py_arg26 ? py_arg26->obj->raw() : 0, py_arg27 ? py_arg27->obj->raw() : 0, py_arg28 ? py_arg28->obj->raw() : 0, py_arg29 ? py_arg29->obj->raw() : 0, py_arg30 ? py_arg30->obj->raw() : 0, py_arg31 ? py_arg31->obj->raw() : 0, name);
    py_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_type->obj = new jit_type(retval);
    py_type->flags = PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED;
    py_retval = Py_BuildValue((char *) "N", py_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_cmpl(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_cmpl(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_return_ptr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "type", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        self->obj->insn_return_ptr(*((PyJit_value *) value)->obj, py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_thrown_exception(PyJit_function *self)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *py_jit_value;
    
    try
    {
        retval = self->obj->insn_thrown_exception();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_address_of(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_address_of(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_add(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_add(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_cmpg(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_cmpg(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_log10(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_log10(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sinh(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sinh(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_max(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_max(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_alloca(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *size;
    const char *keywords[] = {"size", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &size)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_alloca(*((PyJit_value *) size)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_outgoing_frame_posn(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    unsigned int posn;
    const char *keywords[] = {"value", "posn", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!I", (char **) keywords, &PyJit_value_Type, &value, &posn)) {
        return NULL;
    }
    try
    {
        self->obj->insn_outgoing_frame_posn(*((PyJit_value *) value)->obj, posn);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_get_param(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    unsigned int param;
    const char *keywords[] = {"param", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &param)) {
        return NULL;
    }
    try
    {
        retval = self->obj->get_param(param);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_rethrow_unhandled(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_rethrow_unhandled();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_eq(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_eq(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_incoming_frame_posn(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    unsigned int posn;
    const char *keywords[] = {"value", "posn", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!I", (char **) keywords, &PyJit_value_Type, &value, &posn)) {
        return NULL;
    }
    try
    {
        self->obj->insn_incoming_frame_posn(*((PyJit_value *) value)->obj, posn);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call_finally(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_call_finally(*((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_log(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_log(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_shl(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_shl(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sqrt(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sqrt(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_shr(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_shr(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_memcpy(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *dest;
    PyJit_value *src;
    PyJit_value *size;
    const char *keywords[] = {"dest", "src", "size", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &src, &PyJit_value_Type, &size)) {
        return NULL;
    }
    try
    {
        self->obj->insn_memcpy(*((PyJit_value *) dest)->obj, *((PyJit_value *) src)->obj, *((PyJit_value *) size)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_acos(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_acos(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_check_null(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_check_null(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_xor(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_xor(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_branch(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *label;
    const char *keywords[] = {"label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_branch(*((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_ne(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_ne(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_load_elem_address(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *base_addr;
    PyJit_value *index;
    PyJit_type *py_elem_type = NULL;
    const char *keywords[] = {"base_addr", "index", "elem_type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!", (char **) keywords, &PyJit_value_Type, &base_addr, &PyJit_value_Type, &index, &PyJit_type_Type, &py_elem_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_load_elem_address(*((PyJit_value *) base_addr)->obj, *((PyJit_value *) index)->obj, py_elem_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_get_call_stack(PyJit_function *self)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *py_jit_value;
    
    try
    {
        retval = self->obj->insn_get_call_stack();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}




PyObject *
_wrap_PyJit_function_create__0(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    PyJit_type *py_signature = NULL;
    const char *keywords[] = {"signature", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &py_signature)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    self->obj->create(py_signature->obj->raw());
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject *
_wrap_PyJit_function_create__1(PyJit_function *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    const char *keywords[] = {NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "", (char **) keywords)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    self->obj->create();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}


PyObject * _wrap_PyJit_function_create(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[2] = {0,};
    retval = _wrap_PyJit_function_create__0(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    retval = _wrap_PyJit_function_create__1(self, args, kwargs, &exceptions[1]);
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
    return NULL;
}


PyObject *
_wrap_PyJit_function_new_label(PyJit_function *self)
{
    PyObject *py_retval;
    PyJit_label *py_jit_label;
    
    jit_label retval = self->obj->new_label();
    py_jit_label = PyObject_New(PyJit_label, &PyJit_label_Type);
    py_jit_label->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_label->obj = new jit_label(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_label);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_dup(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_dup(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_store(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *dest;
    PyJit_value *value;
    const char *keywords[] = {"dest", "value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &dest, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->store(*((PyJit_value *) dest)->obj, *((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_jump_table(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_jump_table *jump_table;
    const char *keywords[] = {"value", "jump_table", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_jump_table_Type, &jump_table)) {
        return NULL;
    }
    try
    {
        self->obj->insn_jump_table(*((PyJit_value *) value)->obj, *((PyJit_jump_table *) jump_table)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_is_finite(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_is_finite(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_new_value(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->new_value(py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_build_end(PyJit_function *self)
{
    PyObject *py_retval;
    
    self->obj->build_end();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}





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

extern "C" void *PyArray_AsPointer(PyObject *array)
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


PyObject * _wrap_PyJit_function___call__(PyJit_function *self, PyObject *args)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[1] = {0,};
    retval = _wrap_PyJit_function__call__(self, args, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    error_list = PyList_New(1);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return NULL;
}


PyObject *
_wrap_PyJit_function_insn_round(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_round(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_is_inf(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_is_inf(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_set_param(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    unsigned int offset;
    const char *keywords[] = {"value", "offset", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!I", (char **) keywords, &PyJit_value_Type, &value, &offset)) {
        return NULL;
    }
    try
    {
        self->obj->insn_set_param(*((PyJit_value *) value)->obj, offset);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_compile(PyJit_function *self)
{
    PyObject *py_retval;
    int retval;
    
    retval = self->obj->compile();
    py_retval = Py_BuildValue((char *) "i", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_or(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_or(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_abs(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_abs(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_return_from_filter(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_return_from_filter(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_atan2(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_atan2(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_div(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_div(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call_indirect_vtable(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    PyJit_type *py_signature = NULL;
    PyObject *py_list;
    jit_value_t call_args[32];
    PyObject *elem;
    int idx;
    int n_params;
    int flags = 0;
    const char *keywords[] = {"value", "signature", "call_args", "flags", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_signature, &PyList_Type, &py_list, &flags)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_value_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `call_args' must be a list of jit.Value types");
            return NULL;
        }
        call_args[idx] = ((PyJit_value*)elem)->obj->raw();
    }
    try
    {
        retval = self->obj->insn_call_indirect_vtable(*((PyJit_value *) value)->obj, py_signature->obj->raw(), call_args, n_params, flags);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_start_catcher(PyJit_function *self)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *py_jit_value;
    
    try
    {
        retval = self->obj->insn_start_catcher();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_neg(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_neg(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_max_optimization_level(void)
{
    PyObject *py_retval;
    unsigned int retval;
    
    retval = jit_function::max_optimization_level();
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_load_relative(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    unsigned int offset;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"value", "offset", "type", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!IO!", (char **) keywords, &PyJit_value_Type, &value, &offset, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_load_relative(*((PyJit_value *) value)->obj, offset, py_type->obj->raw());
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_mul_ovf(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_mul_ovf(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}





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


PyObject * _wrap_PyJit_function___exit__(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject * retval;
    PyObject *error_list;
    PyObject *exceptions[1] = {0,};
    retval = _wrap_Jit_function___exit__(self, args, kwargs, &exceptions[0]);
    if (!exceptions[0]) {
        return retval;
    }
    error_list = PyList_New(1);
    PyList_SET_ITEM(error_list, 0, PyObject_Str(exceptions[0]));
    Py_DECREF(exceptions[0]);
    PyErr_SetObject(PyExc_TypeError, error_list);
    Py_DECREF(error_list);
    return NULL;
}


PyObject *
_wrap_PyJit_function_build_start(PyJit_function *self)
{
    PyObject *py_retval;
    
    self->obj->build_start();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    char const *name;
    PyJit_function *py_jit_func = NULL;
    PyJit_type *py_signature = NULL;
    PyObject *py_list;
    jit_value_t call_args[32];
    PyObject *elem;
    int idx;
    int n_params;
    int flags = 0;
    const char *keywords[] = {"name", "jit_func", "signature", "call_args", "flags", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O!O!|i", (char **) keywords, &name, &PyJit_function_Type, &py_jit_func, &PyJit_type_Type, &py_signature, &PyList_Type, &py_list, &flags)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_value_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `call_args' must be a list of jit.Value types");
            return NULL;
        }
        call_args[idx] = ((PyJit_value*)elem)->obj->raw();
    }
    try
    {
        retval = self->obj->insn_call(name, py_jit_func->obj->raw(), py_signature->obj->raw(), call_args, n_params, flags);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_tanh(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_tanh(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_le(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_le(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_cosh(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_cosh(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_rint(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_rint(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_lt(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_lt(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_to_bool(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_to_bool(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_push_return_area_ptr(PyJit_function *self)
{
    PyObject *py_retval;
    
    try
    {
        self->obj->insn_push_return_area_ptr();
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_sin(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_sin(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_setup_for_nested(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int nested_level;
    int reg;
    const char *keywords[] = {"nested_level", "reg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "ii", (char **) keywords, &nested_level, &reg)) {
        return NULL;
    }
    try
    {
        self->obj->insn_setup_for_nested(nested_level, reg);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_trunc(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_trunc(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_mul(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_mul(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_flush_struct(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    const char *keywords[] = {"value", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value)) {
        return NULL;
    }
    try
    {
        self->obj->insn_flush_struct(*((PyJit_value *) value)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call_indirect(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value;
    PyJit_type *py_signature = NULL;
    PyObject *py_list;
    jit_value_t call_args[32];
    PyObject *elem;
    int idx;
    int n_params;
    int flags = 0;
    const char *keywords[] = {"value", "signature", "call_args", "flags", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!O!|i", (char **) keywords, &PyJit_value_Type, &value, &PyJit_type_Type, &py_signature, &PyList_Type, &py_list, &flags)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_value_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `call_args' must be a list of jit.Value types");
            return NULL;
        }
        call_args[idx] = ((PyJit_value*)elem)->obj->raw();
    }
    try
    {
        retval = self->obj->insn_call_indirect(*((PyJit_value *) value)->obj, py_signature->obj->raw(), call_args, n_params, flags);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_pow(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    PyJit_value *value2;
    const char *keywords[] = {"value1", "value2", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value1, &PyJit_value_Type, &value2)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_pow(*((PyJit_value *) value1)->obj, *((PyJit_value *) value2)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_call_native(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    char const *name;
    PyObject *native_func;
    PyJit_type *py_signature = NULL;
    PyObject *py_list;
    jit_value_t call_args[32];
    PyObject *elem;
    int idx;
    int n_params;
    int flags = 0;
    const char *keywords[] = {"name", "native_func", "signature", "call_args", "flags", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "sO!O!O!|i", (char **) keywords, &name, &PyLong_Type, &native_func, &PyJit_type_Type, &py_signature, &PyList_Type, &py_list, &flags)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_value_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `call_args' must be a list of jit.Value types");
            return NULL;
        }
        call_args[idx] = ((PyJit_value*)elem)->obj->raw();
    }
    try
    {
        retval = self->obj->insn_call_native(name, (void *)PyLong_AsVoidPtr(native_func), py_signature->obj->raw(), call_args, n_params, flags);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_exp(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_exp(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_branch_if(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    PyJit_label *label;
    const char *keywords[] = {"value", "label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_value_Type, &value, &PyJit_label_Type, &label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_branch_if(*((PyJit_value *) value)->obj, *((PyJit_label *) label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_tan(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_value retval;
    PyJit_value *value1;
    const char *keywords[] = {"value1", NULL};
    PyJit_value *py_jit_value;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_value_Type, &value1)) {
        return NULL;
    }
    try
    {
        retval = self->obj->insn_tan(*((PyJit_value *) value1)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    py_jit_value = PyObject_New(PyJit_value, &PyJit_value_Type);
    py_jit_value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_jit_value->obj = new jit_value(retval);
    py_retval = Py_BuildValue((char *) "N", py_jit_value);
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_move_blocks_to_start(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_label *from_label;
    PyJit_label *to_label;
    const char *keywords[] = {"from_label", "to_label", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!O!", (char **) keywords, &PyJit_label_Type, &from_label, &PyJit_label_Type, &to_label)) {
        return NULL;
    }
    try
    {
        self->obj->insn_move_blocks_to_start(*((PyJit_label *) from_label)->obj, *((PyJit_label *) to_label)->obj);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_function_insn_outgoing_reg(PyJit_function *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_value *value;
    int reg;
    const char *keywords[] = {"value", "reg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!i", (char **) keywords, &PyJit_value_Type, &value, &reg)) {
        return NULL;
    }
    try
    {
        self->obj->insn_outgoing_reg(*((PyJit_value *) value)->obj, reg);
    } catch (jit_build_exception const &exc) {
        PyErr_SetNone((PyObject *) Pyjit_build_exception_Type);
        return NULL;
    }
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



static PyObject*
_wrap_PyJit_function__copy__(PyJit_function *self)
{

    PyJit_function *py_copy;
    py_copy = PyObject_GC_New(PyJit_function, &PyJit_function_Type);
    py_copy->inst_dict = NULL;
    py_copy->obj = new jit_function(*self->obj);
    py_copy->inst_dict = NULL;
    py_copy->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return (PyObject*) py_copy;
}

static PyMethodDef PyJit_function_methods[] = {
    {
        (char *) "insn_sign",
        (PyCFunction) _wrap_PyJit_function_insn_sign,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_sub",
        (PyCFunction) _wrap_PyJit_function_insn_sub,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_min",
        (PyCFunction) _wrap_PyJit_function_insn_min,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_call_filter",
        (PyCFunction) _wrap_PyJit_function_insn_call_filter,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_cos",
        (PyCFunction) _wrap_PyJit_function_insn_cos,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_ceil",
        (PyCFunction) _wrap_PyJit_function_insn_ceil,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_not",
        (PyCFunction) _wrap_PyJit_function_insn_not,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_branch_if_not",
        (PyCFunction) _wrap_PyJit_function_insn_branch_if_not,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_push",
        (PyCFunction) _wrap_PyJit_function_insn_push,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
    (char *) "new_constant",
    (PyCFunction) _wrap_PyJit_function_new_constant,
    METH_KEYWORDS|METH_VARARGS,
    NULL
    },
    {
        (char *) "insn_atan",
        (PyCFunction) _wrap_PyJit_function_insn_atan,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_label",
        (PyCFunction) _wrap_PyJit_function_insn_label,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_start_finally",
        (PyCFunction) _wrap_PyJit_function_insn_start_finally,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_ushr",
        (PyCFunction) _wrap_PyJit_function_insn_ushr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
    (char *) "__enter__",
    (PyCFunction) _wrap_PyJit_function___enter__,
    METH_KEYWORDS|METH_VARARGS,
    NULL
    },
    {
        (char *) "insn_branch_if_pc_not_in_range",
        (PyCFunction) _wrap_PyJit_function_insn_branch_if_pc_not_in_range,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_floor",
        (PyCFunction) _wrap_PyJit_function_insn_floor,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_move_blocks_to_end",
        (PyCFunction) _wrap_PyJit_function_insn_move_blocks_to_end,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_add_ovf",
        (PyCFunction) _wrap_PyJit_function_insn_add_ovf,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_rem",
        (PyCFunction) _wrap_PyJit_function_insn_rem,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_mark_offset",
        (PyCFunction) _wrap_PyJit_function_insn_mark_offset,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_address_of_label",
        (PyCFunction) _wrap_PyJit_function_insn_address_of_label,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_return_from_finally",
        (PyCFunction) _wrap_PyJit_function_insn_return_from_finally,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_store_relative",
        (PyCFunction) _wrap_PyJit_function_insn_store_relative,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_sshr",
        (PyCFunction) _wrap_PyJit_function_insn_sshr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_incoming_reg",
        (PyCFunction) _wrap_PyJit_function_insn_incoming_reg,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_store_elem",
        (PyCFunction) _wrap_PyJit_function_insn_store_elem,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_load_elem",
        (PyCFunction) _wrap_PyJit_function_insn_load_elem,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "closure",
        (PyCFunction) _wrap_PyJit_function_closure,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_memmove",
        (PyCFunction) _wrap_PyJit_function_insn_memmove,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_memset",
        (PyCFunction) _wrap_PyJit_function_insn_memset,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_mark_breakpoint",
        (PyCFunction) _wrap_PyJit_function_insn_mark_breakpoint,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_start_filter",
        (PyCFunction) _wrap_PyJit_function_insn_start_filter,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "vtable_pointer",
        (PyCFunction) _wrap_PyJit_function_vtable_pointer,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_return_reg",
        (PyCFunction) _wrap_PyJit_function_insn_return_reg,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_set_param_ptr",
        (PyCFunction) _wrap_PyJit_function_insn_set_param_ptr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_sub_ovf",
        (PyCFunction) _wrap_PyJit_function_insn_sub_ovf,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "dump",
        (PyCFunction) _wrap_PyJit_function_dump,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_ge",
        (PyCFunction) _wrap_PyJit_function_insn_ge,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_new_block",
        (PyCFunction) _wrap_PyJit_function_insn_new_block,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_load_small",
        (PyCFunction) _wrap_PyJit_function_insn_load_small,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_default_return",
        (PyCFunction) _wrap_PyJit_function_insn_default_return,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_gt",
        (PyCFunction) _wrap_PyJit_function_insn_gt,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_add_relative",
        (PyCFunction) _wrap_PyJit_function_insn_add_relative,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_asin",
        (PyCFunction) _wrap_PyJit_function_insn_asin,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_rem_ieee",
        (PyCFunction) _wrap_PyJit_function_insn_rem_ieee,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_import",
        (PyCFunction) _wrap_PyJit_function_insn_import,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_load",
        (PyCFunction) _wrap_PyJit_function_insn_load,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_and",
        (PyCFunction) _wrap_PyJit_function_insn_and,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_to_not_bool",
        (PyCFunction) _wrap_PyJit_function_insn_to_not_bool,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_is_nan",
        (PyCFunction) _wrap_PyJit_function_insn_is_nan,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
    (char *) "insn_return",
    (PyCFunction) _wrap_PyJit_function_insn_return,
    METH_KEYWORDS|METH_VARARGS,
    NULL
    },
    {
        (char *) "insn_uses_catcher",
        (PyCFunction) _wrap_PyJit_function_insn_uses_catcher,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_push_ptr",
        (PyCFunction) _wrap_PyJit_function_insn_push_ptr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_throw",
        (PyCFunction) _wrap_PyJit_function_insn_throw,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_struct_pointer",
        (PyCFunction) _wrap_PyJit_function_get_struct_pointer,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_convert",
        (PyCFunction) _wrap_PyJit_function_insn_convert,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "signature_helper",
        (PyCFunction) _wrap_PyJit_function_signature_helper,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "insn_cmpl",
        (PyCFunction) _wrap_PyJit_function_insn_cmpl,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_return_ptr",
        (PyCFunction) _wrap_PyJit_function_insn_return_ptr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_thrown_exception",
        (PyCFunction) _wrap_PyJit_function_insn_thrown_exception,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_address_of",
        (PyCFunction) _wrap_PyJit_function_insn_address_of,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_add",
        (PyCFunction) _wrap_PyJit_function_insn_add,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_cmpg",
        (PyCFunction) _wrap_PyJit_function_insn_cmpg,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_log10",
        (PyCFunction) _wrap_PyJit_function_insn_log10,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_sinh",
        (PyCFunction) _wrap_PyJit_function_insn_sinh,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_max",
        (PyCFunction) _wrap_PyJit_function_insn_max,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_alloca",
        (PyCFunction) _wrap_PyJit_function_insn_alloca,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_outgoing_frame_posn",
        (PyCFunction) _wrap_PyJit_function_insn_outgoing_frame_posn,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_param",
        (PyCFunction) _wrap_PyJit_function_get_param,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_rethrow_unhandled",
        (PyCFunction) _wrap_PyJit_function_insn_rethrow_unhandled,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_eq",
        (PyCFunction) _wrap_PyJit_function_insn_eq,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_incoming_frame_posn",
        (PyCFunction) _wrap_PyJit_function_insn_incoming_frame_posn,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_call_finally",
        (PyCFunction) _wrap_PyJit_function_insn_call_finally,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_log",
        (PyCFunction) _wrap_PyJit_function_insn_log,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_shl",
        (PyCFunction) _wrap_PyJit_function_insn_shl,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_sqrt",
        (PyCFunction) _wrap_PyJit_function_insn_sqrt,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_shr",
        (PyCFunction) _wrap_PyJit_function_insn_shr,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_memcpy",
        (PyCFunction) _wrap_PyJit_function_insn_memcpy,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_acos",
        (PyCFunction) _wrap_PyJit_function_insn_acos,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_check_null",
        (PyCFunction) _wrap_PyJit_function_insn_check_null,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_xor",
        (PyCFunction) _wrap_PyJit_function_insn_xor,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_branch",
        (PyCFunction) _wrap_PyJit_function_insn_branch,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_ne",
        (PyCFunction) _wrap_PyJit_function_insn_ne,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_load_elem_address",
        (PyCFunction) _wrap_PyJit_function_insn_load_elem_address,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_get_call_stack",
        (PyCFunction) _wrap_PyJit_function_insn_get_call_stack,
        METH_NOARGS,
        NULL
    },
    {
    (char *) "create",
    (PyCFunction) _wrap_PyJit_function_create,
    METH_KEYWORDS|METH_VARARGS,
    NULL
    },
    {
        (char *) "new_label",
        (PyCFunction) _wrap_PyJit_function_new_label,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_dup",
        (PyCFunction) _wrap_PyJit_function_insn_dup,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "store",
        (PyCFunction) _wrap_PyJit_function_store,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_jump_table",
        (PyCFunction) _wrap_PyJit_function_insn_jump_table,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_is_finite",
        (PyCFunction) _wrap_PyJit_function_insn_is_finite,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "new_value",
        (PyCFunction) _wrap_PyJit_function_new_value,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "build_end",
        (PyCFunction) _wrap_PyJit_function_build_end,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_round",
        (PyCFunction) _wrap_PyJit_function_insn_round,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_is_inf",
        (PyCFunction) _wrap_PyJit_function_insn_is_inf,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_set_param",
        (PyCFunction) _wrap_PyJit_function_insn_set_param,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "compile",
        (PyCFunction) _wrap_PyJit_function_compile,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_or",
        (PyCFunction) _wrap_PyJit_function_insn_or,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_abs",
        (PyCFunction) _wrap_PyJit_function_insn_abs,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_return_from_filter",
        (PyCFunction) _wrap_PyJit_function_insn_return_from_filter,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_atan2",
        (PyCFunction) _wrap_PyJit_function_insn_atan2,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_div",
        (PyCFunction) _wrap_PyJit_function_insn_div,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_call_indirect_vtable",
        (PyCFunction) _wrap_PyJit_function_insn_call_indirect_vtable,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_start_catcher",
        (PyCFunction) _wrap_PyJit_function_insn_start_catcher,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_neg",
        (PyCFunction) _wrap_PyJit_function_insn_neg,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "max_optimization_level",
        (PyCFunction) _wrap_PyJit_function_max_optimization_level,
        METH_NOARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "insn_load_relative",
        (PyCFunction) _wrap_PyJit_function_insn_load_relative,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_mul_ovf",
        (PyCFunction) _wrap_PyJit_function_insn_mul_ovf,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
    (char *) "__exit__",
    (PyCFunction) _wrap_PyJit_function___exit__,
    METH_KEYWORDS|METH_VARARGS,
    NULL
    },
    {
        (char *) "build_start",
        (PyCFunction) _wrap_PyJit_function_build_start,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_call",
        (PyCFunction) _wrap_PyJit_function_insn_call,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_tanh",
        (PyCFunction) _wrap_PyJit_function_insn_tanh,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_le",
        (PyCFunction) _wrap_PyJit_function_insn_le,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_cosh",
        (PyCFunction) _wrap_PyJit_function_insn_cosh,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_rint",
        (PyCFunction) _wrap_PyJit_function_insn_rint,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_lt",
        (PyCFunction) _wrap_PyJit_function_insn_lt,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_to_bool",
        (PyCFunction) _wrap_PyJit_function_insn_to_bool,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_push_return_area_ptr",
        (PyCFunction) _wrap_PyJit_function_insn_push_return_area_ptr,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "insn_sin",
        (PyCFunction) _wrap_PyJit_function_insn_sin,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_setup_for_nested",
        (PyCFunction) _wrap_PyJit_function_insn_setup_for_nested,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_trunc",
        (PyCFunction) _wrap_PyJit_function_insn_trunc,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_mul",
        (PyCFunction) _wrap_PyJit_function_insn_mul,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_flush_struct",
        (PyCFunction) _wrap_PyJit_function_insn_flush_struct,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_call_indirect",
        (PyCFunction) _wrap_PyJit_function_insn_call_indirect,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_pow",
        (PyCFunction) _wrap_PyJit_function_insn_pow,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_call_native",
        (PyCFunction) _wrap_PyJit_function_insn_call_native,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_exp",
        (PyCFunction) _wrap_PyJit_function_insn_exp,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_branch_if",
        (PyCFunction) _wrap_PyJit_function_insn_branch_if,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_tan",
        (PyCFunction) _wrap_PyJit_function_insn_tan,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_move_blocks_to_start",
        (PyCFunction) _wrap_PyJit_function_insn_move_blocks_to_start,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "insn_outgoing_reg",
        (PyCFunction) _wrap_PyJit_function_insn_outgoing_reg,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "fail",
        (PyCFunction) PyJit_function__PythonHelper::_wrap_fail,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "create_signature",
        (PyCFunction) PyJit_function__PythonHelper::_wrap_create_signature,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "build",
        (PyCFunction) PyJit_function__PythonHelper::_wrap_build,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "out_of_memory",
        (PyCFunction) PyJit_function__PythonHelper::_wrap_out_of_memory,
        METH_NOARGS,
        NULL
    },
    {(char *) "__copy__", (PyCFunction) _wrap_PyJit_function__copy__, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static void
PyJit_function__tp_clear(PyJit_function *self)
{
    Py_CLEAR(self->inst_dict);
        jit_function *tmp = self->obj;
    self->obj = NULL;
    if (!(self->flags & PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED)) {
        delete tmp;
    }
}


static int
PyJit_function__tp_traverse(PyJit_function *self, visitproc visit, void *arg)
{
    Py_VISIT(self->inst_dict);
    
    if (self->obj && typeid(*self->obj).name() == typeid(PyJit_function__PythonHelper).name() )
        Py_VISIT((PyObject *) self);

    return 0;
}


static void
_wrap_PyJit_function__tp_dealloc(PyJit_function *self)
{
    PyJit_function__tp_clear(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject PyJit_function_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Function",            /* tp_name */
    sizeof(PyJit_function),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_function__tp_dealloc,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)NULL,       /* tp_getattr */
    (setattrfunc)NULL,       /* tp_setattr */
    (cmpfunc)NULL,           /* tp_compare */
    (reprfunc)NULL,             /* tp_repr */
    (PyNumberMethods*)NULL,     /* tp_as_number */
    (PySequenceMethods*)NULL, /* tp_as_sequence */
    (PyMappingMethods*)NULL,   /* tp_as_mapping */
    (hashfunc)NULL,             /* tp_hash */
    (ternaryfunc)_wrap_PyJit_function___call__,          /* tp_call */
    (reprfunc)NULL,              /* tp_str */
    (getattrofunc)NULL,     /* tp_getattro */
    (setattrofunc)NULL,     /* tp_setattro */
    (PyBufferProcs*)NULL,  /* tp_as_buffer */
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_GC,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)PyJit_function__tp_traverse,     /* tp_traverse */
    (inquiry)PyJit_function__tp_clear,             /* tp_clear */
    (richcmpfunc)NULL,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_function_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    PyJit_function__getsets,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)_wrap_PyJit_function__descr_get__,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    offsetof(PyJit_function, inst_dict),                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_function__tp_init,             /* tp_init */
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


