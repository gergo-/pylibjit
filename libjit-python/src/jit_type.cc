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
_wrap_PyJit_type__get_type(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "i", self->obj->get_kind());
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_size(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(self->obj->get_size()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_num_fields(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(self->obj->num_fields()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_num_params(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(self->obj->num_params()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_abi(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "i", self->obj->get_abi());
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_tagged_data(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", self->obj->get_tagged_data());
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_return_via_pointer(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->return_via_pointer()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_primitive(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_primitive()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_struct(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_struct()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_union(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_union()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_signature(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_signature()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_array(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_array()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_tuple(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_tuple()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_pointer(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_pointer()));
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_is_tagged(PyJit_type *self, void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(self->obj->is_tagged()));
    return py_retval;
}

static PyGetSetDef PyJit_type__getsets[] = {
    {
        (char*) "num_params", /* attribute name */
        (getter) _wrap_PyJit_type__get_num_params, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_pointer", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_pointer, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "num_fields", /* attribute name */
        (getter) _wrap_PyJit_type__get_num_fields, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "tagged_data", /* attribute name */
        (getter) _wrap_PyJit_type__get_tagged_data, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "abi", /* attribute name */
        (getter) _wrap_PyJit_type__get_abi, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_array", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_array, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_signature", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_signature, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_union", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_union, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_tagged", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_tagged, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_struct", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_struct, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_tuple", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_tuple, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "return_via_pointer", /* attribute name */
        (getter) _wrap_PyJit_type__get_return_via_pointer, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "type", /* attribute name */
        (getter) _wrap_PyJit_type__get_type, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "is_primitive", /* attribute name */
        (getter) _wrap_PyJit_type__get_is_primitive, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "size", /* attribute name */
        (getter) _wrap_PyJit_type__get_size, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};
static PyObject *
_wrap_PyJit_type__get_void(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_void();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_sbyte(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_sbyte();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_ubyte(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_ubyte();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_short(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_short();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_ushort(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_ushort();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_int(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_int();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_uint(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_uint();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_nint(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_nint();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_nuint(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_nuint();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_long(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_long();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_ulong(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_ulong();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_float32(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_float32();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_float64(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_float64();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_nfloat(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_nfloat();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_void_ptr(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_void_ptr();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_array(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_array();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyObject *
_wrap_PyJit_type__get_tuple_t(PyObject * PYBINDGEN_UNUSED(obj),  void * PYBINDGEN_UNUSED(closure))
{
    PyObject *py_retval;
    PyJit_type *py_jit_type;
    
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = jit_type::t_tuple();
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}

static PyGetSetDef Jit_typeMeta__getsets[] = {
    {
        (char*) "short", /* attribute name */
        (getter) _wrap_PyJit_type__get_short, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "sbyte", /* attribute name */
        (getter) _wrap_PyJit_type__get_sbyte, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "nint", /* attribute name */
        (getter) _wrap_PyJit_type__get_nint, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "ulong", /* attribute name */
        (getter) _wrap_PyJit_type__get_ulong, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "nuint", /* attribute name */
        (getter) _wrap_PyJit_type__get_nuint, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "int", /* attribute name */
        (getter) _wrap_PyJit_type__get_int, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "void", /* attribute name */
        (getter) _wrap_PyJit_type__get_void, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "ushort", /* attribute name */
        (getter) _wrap_PyJit_type__get_ushort, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "long", /* attribute name */
        (getter) _wrap_PyJit_type__get_long, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "void_ptr", /* attribute name */
        (getter) _wrap_PyJit_type__get_void_ptr, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "uint", /* attribute name */
        (getter) _wrap_PyJit_type__get_uint, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "nfloat", /* attribute name */
        (getter) _wrap_PyJit_type__get_nfloat, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "tuple_t", /* attribute name */
        (getter) _wrap_PyJit_type__get_tuple_t, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "ubyte", /* attribute name */
        (getter) _wrap_PyJit_type__get_ubyte, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "array", /* attribute name */
        (getter) _wrap_PyJit_type__get_array, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "float32", /* attribute name */
        (getter) _wrap_PyJit_type__get_float32, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    {
        (char*) "float64", /* attribute name */
        (getter) _wrap_PyJit_type__get_float64, /* C function to get the attribute */
        (setter) NULL, /* C function to set the attribute */
        NULL, /* optional doc string */
        NULL /* optional additional data for getter and setter */
    },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject PyJit_typeMeta_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "Jit_typeMeta",		        /* tp_name */
    0,					/* tp_basicsize */
    0,					/* tp_itemsize */
    0,	 				/* tp_dealloc */
    0,					/* tp_print */
    0,					/* tp_getattr */
    0,					/* tp_setattr */
    0,					/* tp_compare */
    0,					/* tp_repr */
    0,					/* tp_as_number */
    0,					/* tp_as_sequence */
    0,					/* tp_as_mapping */
    0,					/* tp_hash */
    0,					/* tp_call */
    0,					/* tp_str */
    0,					/* tp_getattro */
    0,					/* tp_setattro */
    0,					/* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_HAVE_GC|Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,					/* tp_doc */
    0,					/* tp_traverse */
    0,					/* tp_clear */
    0,					/* tp_richcompare */
    0,					/* tp_weaklistoffset */
    0,					/* tp_iter */
    0,					/* tp_iternext */
    0,					/* tp_methods */
    0,					/* tp_members */
    Jit_typeMeta__getsets,			/* tp_getset */
    0,					/* tp_base */
    0,					/* tp_dict */
    0,					/* tp_descr_get */
    0,					/* tp_descr_set */
    0,					/* tp_dictoffset */
    0,					/* tp_init */
    0,					/* tp_alloc */
    0,					/* tp_new */
    0,					/* tp_free */
    0,					/* tp_is_gc */
    0,					/* tp_bases */
    0,					/* tp_mro */
    0,					/* tp_cache */
    0,					/* tp_subclasses */
    0,					/* tp_weaklist */
    0					/* tp_del */
};


static int
_wrap_PyJit_type__tp_init(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyJit_type *ctor_arg;
    const char *keywords[] = {"ctor_arg", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &ctor_arg)) {
        return -1;
    }
    self->obj = new jit_type(*((PyJit_type *) ctor_arg)->obj);
    self->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return 0;
}



PyObject *
_wrap_PyJit_type_create_pointer(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_type = NULL;
    int incref = 1;
    const char *keywords[] = {"type", "incref", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|i", (char **) keywords, &PyJit_type_Type, &py_type, &incref)) {
        return NULL;
    }
    retval = jit_type::create_pointer(py_type->obj->raw(), incref);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_create_tagged(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_type = NULL;
    int kind;
    PyObject *data;
    int incref = 1;
    const char *keywords[] = {"type", "kind", "data", "incref", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!iO|i", (char **) keywords, &PyJit_type_Type, &py_type, &kind, &data, &incref)) {
        return NULL;
    }
    Py_INCREF((PyObject*) data);
    retval = jit_type::create_tagged(py_type->obj->raw(), kind, data, _wrap_meta_free_func, incref);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_return(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->get_return();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_set_size_and_alignment(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int size;
    int alignment;
    const char *keywords[] = {"size", "alignment", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "ii", (char **) keywords, &size, &alignment)) {
        return NULL;
    }
    self->obj->set_size_and_alignment(size, alignment);
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_type_find_name(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    unsigned int retval;
    char const *name;
    const char *keywords[] = {"name", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s", (char **) keywords, &name)) {
        return NULL;
    }
    retval = self->obj->find_name(name);
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_type_has_tag(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    bool retval;
    int kind;
    const char *keywords[] = {"kind", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "i", (char **) keywords, &kind)) {
        return NULL;
    }
    retval = self->obj->has_tag(kind);
    py_retval = Py_BuildValue((char *) "N", PyBool_FromLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_type_set_names(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    int retval;
    PyObject *py_list;
    char **names;
    PyObject *elem;
    int idx;
    int num_names;
    const char *keywords[] = {"names", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyList_Type, &py_list)) {
        return NULL;
    }
    num_names = PyList_Size(py_list);
    names = (char **)malloc(sizeof(char *) * num_names);
    if (names == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory");
        return NULL;
    }
    for (idx = 0; idx < num_names; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyUnicode_Check(elem)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `names' must be a list of `string'");
            return NULL;
        }
        names[idx] = PyBytes_AsString(PyUnicode_AsEncodedString((elem), "utf-8", "Error"));
    }
    retval = self->obj->set_names(names, num_names);
    free(names);
    py_retval = Py_BuildValue((char *) "i", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_normalize(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->normalize();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_best_alignment(void)
{
    PyObject *py_retval;
    unsigned int retval;
    
    retval = jit_type::best_alignment();
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_type_array_t(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_type = NULL;
    const char *keywords[] = {"type", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!", (char **) keywords, &PyJit_type_Type, &py_type)) {
        return NULL;
    }
    retval = jit_type::array_t(py_type->obj->raw());
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = new jit_type((*retval));
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_field(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    unsigned int index;
    const char *keywords[] = {"index", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &index)) {
        return NULL;
    }
    retval = self->obj->get_field(index);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_remove_tags(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->remove_tags();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_create_union(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    PyObject *py_list;
    jit_type_t fields[32];
    PyObject *elem;
    int idx;
    int n_params;
    int incref = 1;
    const char *keywords[] = {"fields", "incref", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|i", (char **) keywords, &PyList_Type, &py_list, &incref)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_type_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `fields' must be a list of jit.Type types");
            return NULL;
        }
        fields[idx] = ((PyJit_type*)elem)->obj->raw();
    }
    retval = jit_type::create_union(fields, n_params, incref);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_tagged_type(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->get_tagged_type();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_set_tagged_type(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    PyJit_type *py_underlying = NULL;
    int incref = 1;
    const char *keywords[] = {"underlying", "incref", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|i", (char **) keywords, &PyJit_type_Type, &py_underlying, &incref)) {
        return NULL;
    }
    self->obj->set_tagged_type(py_underlying->obj->raw(), incref);
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_offset(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    unsigned int retval;
    unsigned int index;
    const char *keywords[] = {"index", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &index)) {
        return NULL;
    }
    retval = self->obj->get_offset(index);
    py_retval = Py_BuildValue((char *) "N", PyLong_FromUnsignedLong(retval));
    return py_retval;
}



PyObject *
_wrap_PyJit_type_promote_int(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->promote_int();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_create_struct(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    PyObject *py_list;
    jit_type_t fields[32];
    PyObject *elem;
    int idx;
    int n_params;
    int incref = 1;
    const char *keywords[] = {"fields", "incref", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!|i", (char **) keywords, &PyList_Type, &py_list, &incref)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_type_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `fields' must be a list of jit.Type types");
            return NULL;
        }
        fields[idx] = ((PyJit_type*)elem)->obj->raw();
    }
    retval = jit_type::create_struct(fields, n_params, incref);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_create_signature(PyJit_type *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    jit_abi_t abi;
    PyJit_type *py_return_type = NULL;
    PyObject *py_list;
    jit_type_t params[32];
    PyObject *elem;
    int idx;
    int n_params;
    int incref = 1;
    const char *keywords[] = {"abi", "return_type", "params", "incref", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "iO!O!|i", (char **) keywords, &abi, &PyJit_type_Type, &py_return_type, &PyList_Type, &py_list, &incref)) {
        return NULL;
    }
    if ((n_params = PyList_Size(py_list)) > 32) {
        PyErr_SetString(PyExc_TypeError, "Maximum of 32 function args supported");
        return NULL;
    }
    for (idx = 0; idx < n_params; idx++) {
        elem = PyList_GetItem(py_list, idx);
        if (!PyObject_IsInstance(elem, (PyObject*) &PyJit_type_Type)) {
            PyErr_SetString(PyExc_TypeError, "Parameter `params' must be a list of jit.Type types");
            return NULL;
        }
        params[idx] = ((PyJit_type*)elem)->obj->raw();
    }
    retval = jit_type::create_signature(abi, py_return_type->obj->raw(), params, n_params, incref);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_name(PyJit_type *self)
{
    PyObject *py_retval;
    
    self->obj->name();
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_ref(PyJit_type *self)
{
    PyObject *py_retval;
    jit_type *retval;
    PyJit_type *py_jit_type;
    
    retval = self->obj->get_ref();
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_name(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    char const *retval;
    unsigned int index;
    const char *keywords[] = {"index", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &index)) {
        return NULL;
    }
    retval = self->obj->get_name(index);
    py_retval = Py_BuildValue((char *) "s", retval);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_get_param(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    jit_type *retval;
    unsigned int index;
    const char *keywords[] = {"index", NULL};
    PyJit_type *py_jit_type;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "I", (char **) keywords, &index)) {
        return NULL;
    }
    retval = self->obj->get_param(index);
    if (!(retval)) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    py_jit_type = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_jit_type->obj = retval;
    py_jit_type->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_retval = Py_BuildValue((char *) "N", py_jit_type);
    return py_retval;
}



PyObject *
_wrap_PyJit_type_set_offset(PyJit_type *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_retval;
    unsigned int field_index;
    unsigned int offset;
    const char *keywords[] = {"field_index", "offset", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "II", (char **) keywords, &field_index, &offset)) {
        return NULL;
    }
    self->obj->set_offset(field_index, offset);
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}



static PyObject*
_wrap_PyJit_type__copy__(PyJit_type *self)
{

    PyJit_type *py_copy;
    py_copy = PyObject_New(PyJit_type, &PyJit_type_Type);
    py_copy->obj = new jit_type(*self->obj);
    py_copy->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    return (PyObject*) py_copy;
}

static PyMethodDef PyJit_type_methods[] = {
    {
        (char *) "create_pointer",
        (PyCFunction) _wrap_PyJit_type_create_pointer,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "create_tagged",
        (PyCFunction) _wrap_PyJit_type_create_tagged,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "get_return",
        (PyCFunction) _wrap_PyJit_type_get_return,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "set_size_and_alignment",
        (PyCFunction) _wrap_PyJit_type_set_size_and_alignment,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "find_name",
        (PyCFunction) _wrap_PyJit_type_find_name,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "has_tag",
        (PyCFunction) _wrap_PyJit_type_has_tag,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "set_names",
        (PyCFunction) _wrap_PyJit_type_set_names,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "normalize",
        (PyCFunction) _wrap_PyJit_type_normalize,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "best_alignment",
        (PyCFunction) _wrap_PyJit_type_best_alignment,
        METH_NOARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "array_t",
        (PyCFunction) _wrap_PyJit_type_array_t,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "get_field",
        (PyCFunction) _wrap_PyJit_type_get_field,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "remove_tags",
        (PyCFunction) _wrap_PyJit_type_remove_tags,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "create_union",
        (PyCFunction) _wrap_PyJit_type_create_union,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "get_tagged_type",
        (PyCFunction) _wrap_PyJit_type_get_tagged_type,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "set_tagged_type",
        (PyCFunction) _wrap_PyJit_type_set_tagged_type,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_offset",
        (PyCFunction) _wrap_PyJit_type_get_offset,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "promote_int",
        (PyCFunction) _wrap_PyJit_type_promote_int,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "create_struct",
        (PyCFunction) _wrap_PyJit_type_create_struct,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "create_signature",
        (PyCFunction) _wrap_PyJit_type_create_signature,
        METH_KEYWORDS|METH_VARARGS|METH_STATIC,
        NULL
    },
    {
        (char *) "name",
        (PyCFunction) _wrap_PyJit_type_name,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "get_ref",
        (PyCFunction) _wrap_PyJit_type_get_ref,
        METH_NOARGS,
        NULL
    },
    {
        (char *) "get_name",
        (PyCFunction) _wrap_PyJit_type_get_name,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "get_param",
        (PyCFunction) _wrap_PyJit_type_get_param,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {
        (char *) "set_offset",
        (PyCFunction) _wrap_PyJit_type_set_offset,
        METH_KEYWORDS|METH_VARARGS,
        NULL
    },
    {(char *) "__copy__", (PyCFunction) _wrap_PyJit_type__copy__, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static void
_wrap_PyJit_type__tp_dealloc(PyJit_type *self)
{
    /* FIXME: GB removed this */
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
_wrap_PyJit_type__tp_richcompare (PyJit_type *PYBINDGEN_UNUSED(self), PyJit_type *other, int opid)
{
    
    if (!PyObject_IsInstance((PyObject*) other, (PyObject*) &PyJit_type_Type)) {
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    }
    switch (opid)
    {
    case Py_LT:
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    case Py_LE:
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    case Py_EQ:
        if (*self->obj == *other->obj) {
            Py_INCREF(Py_True);
            return Py_True;
        } else {
            Py_INCREF(Py_False);
            return Py_False;
        }
    case Py_NE:
        if (*self->obj != *other->obj) {
            Py_INCREF(Py_True);
            return Py_True;
        } else {
            Py_INCREF(Py_False);
            return Py_False;
        }
    case Py_GE:
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    case Py_GT:
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    } /* closes switch (opid) */
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

PyTypeObject PyJit_type_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    (char *) "jit.Type",            /* tp_name */
    sizeof(PyJit_type),                  /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_wrap_PyJit_type__tp_dealloc,        /* tp_dealloc */
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
    (richcmpfunc)_wrap_PyJit_type__tp_richcompare,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)NULL,          /* tp_iter */
    (iternextfunc)NULL,     /* tp_iternext */
    (struct PyMethodDef*)PyJit_type_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    PyJit_type__getsets,                     /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)NULL,    /* tp_descr_get */
    (descrsetfunc)NULL,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)_wrap_PyJit_type__tp_init,             /* tp_init */
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


