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

/* This file was generated by PyBindGen 0.16.0.791 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>


#if PY_VERSION_HEX >= 0x03000000
typedef void* cmpfunc;
#define Py_TPFLAGS_CHECKTYPES 0
#endif


#if     __GNUC__ > 2
# define PYBINDGEN_UNUSED(param) param __attribute__((__unused__))
#elif     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
# define PYBINDGEN_UNUSED(param) __attribute__((__unused__)) param
#else
# define PYBINDGEN_UNUSED(param) param
#endif  /* !__GNUC__ */

typedef enum _PyBindGenWrapperFlags {
   PYBINDGEN_WRAPPER_FLAG_NONE = 0,
   PYBINDGEN_WRAPPER_FLAG_OBJECT_NOT_OWNED = (1<<0),
} PyBindGenWrapperFlags;


#include <jit/jit-plus.h>
#include <jit/jit.h>
#include <jit/jit-dynamic.h>
#include "jit-plus-type.h"
#include "jit-plus-pycontext.h"
#include <typeinfo>
/* --- forward declarations --- */


typedef struct {
    PyObject_HEAD
    jit_pycontext *obj;
    PyObject *inst_dict;
    PyBindGenWrapperFlags flags:8;
} PyJit_pycontext;


extern PyTypeObject PyJit_pycontext_Type;


typedef struct {
    PyObject_HEAD
    jit_function *obj;
    PyObject *inst_dict;
    PyBindGenWrapperFlags flags:8;
} PyJit_function;


extern PyTypeObject PyJit_function_Type;

class PyJit_function__PythonHelper : public jit_function
{
public:
    PyObject *m_pyself;
    PyJit_function__PythonHelper(jit_context & context)
        : jit_function(context), m_pyself(NULL)
        {}

    PyJit_function__PythonHelper(jit_context & context, jit_type_t signature)
        : jit_function(context, signature), m_pyself(NULL)
        {}

    PyJit_function__PythonHelper(jit_function const & ctor_arg)
        : jit_function(ctor_arg), m_pyself(NULL)
        {}


    void set_pyobj(PyObject *pyobj)
    {
        Py_XDECREF(m_pyself);
        Py_INCREF(pyobj);
        m_pyself = pyobj;
    }

    virtual ~PyJit_function__PythonHelper()
    {
        Py_CLEAR(m_pyself);
    }


    static PyObject * _wrap_fail(PyJit_function *self);
    inline void fail__parent_caller()
    { jit_function::fail(); }

    static PyObject * _wrap_create_signature(PyJit_function *self);
    inline jit_type_t create_signature__parent_caller()
    { return jit_function::create_signature(); }

    static PyObject * _wrap_build(PyJit_function *self);
    inline void build__parent_caller()
    { jit_function::build(); }

    static PyObject * _wrap_out_of_memory(PyJit_function *self);
    inline void out_of_memory__parent_caller()
    { jit_function::out_of_memory(); }

    virtual void build();

    virtual jit_type_t create_signature();
    PyJit_function__PythonHelper(jit_function_t & func)
        : jit_function(func), m_pyself(NULL) {}
};


            // gergo was here!
            #define PYJIT_TYPE_ARRAY 20
            #define PYJIT_TYPE_BOOL  21
            extern "C" void *PyArray_AsPointer(PyObject *);
            extern "C" PyObject *get_self_token(void);



typedef struct {
    PyObject_HEAD
    jit_value *obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_value;


extern PyTypeObject PyJit_value_Type;


typedef struct {
    PyObject_HEAD
    jit_label *obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_label;


extern PyTypeObject PyJit_label_Type;


typedef struct {
    PyObject_HEAD
    jit_jump_table *obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_jump_table;


extern PyTypeObject PyJit_jump_table_Type;


typedef struct {
    PyObject_HEAD
    jit_type *obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_type;


extern PyTypeObject PyJit_type_Type;
extern PyTypeObject PyJit_typeMeta_Type;


typedef struct {
    PyObject_HEAD
    jit_debugger_t obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_debugger;


extern PyTypeObject PyJit_debugger_Type;


extern PyTypeObject *Pyjit_build_exception_Type;

/* --- forward declarations --- */


typedef struct {
    PyObject_HEAD
    jit_readelf_t obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_readelf;


extern PyTypeObject PyJit_readelf_Type;


typedef struct {
    PyObject_HEAD
    jit_writeelf_t obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_writeelf;


extern PyTypeObject PyJit_writeelf_Type;

/* --- forward declarations --- */


typedef struct {
    PyObject_HEAD
    jit_dynlib_handle_t obj;
    PyBindGenWrapperFlags flags:8;
} PyJit_dynlib_handle;


extern PyTypeObject PyJit_dynlib_handle_Type;

void _wrap_meta_free_func(void *data);

PyObject* _wrap_convert_c2py__jit_value(jit_value *cvalue);


int _wrap_convert_py2c__jit_value(PyObject *value, jit_value *address);
