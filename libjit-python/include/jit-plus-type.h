/*
 * Copyright (C) 2012 Dan Eicher
 *
 * The libjit library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The libjit library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the libjit library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef	_JIT_PLUS_TYPE_H
#define	_JIT_PLUS_TYPE_H

#include <jit/jit.h>

#include <cstdlib>
#include <string>
#include <sstream>

class jit_type
{
public:
    jit_type(const jit_type &other)
        : m_type(jit_type_copy(other.raw())), m_name(other.m_name),
          m_ref_type(jit_type_copy(other.m_ref_type)),
          m_ref_type_cached(
                  other.m_ref_type_cached ?
                    new jit_type(*other.m_ref_type_cached)
                  : NULL) {}
    jit_type(const jit_type_t other)
        : m_type(NULL), m_name("<unknown>"),
          m_ref_type(NULL), m_ref_type_cached(NULL)
    {
        if (other == jit_type_nint) {
            m_type = other;
            m_name = "nint";
        } else if (other == jit_type_int) {
            m_type = other;
            m_name = "int";
        } else if (other == jit_type_long) {
            m_type = other;
            m_name = "long";
        } else if (other == jit_type_float64) {
            m_type = other;
            m_name = "float64";
        } else if (other == jit_type_ubyte) {
            m_type = other;
            m_name = "ubyte";
        } else if (other == jit_type_void) {
            m_type = other;
            m_name = "void";
        } else if (jit_type_is_pointer(other)) {
            m_type = jit_type_copy(other);
            m_ref_type = jit_type_copy(jit_type_get_ref(other));
            m_name = "pointer";
        } else if (jit_type_is_signature(other)) {
            m_type = jit_type_copy(other);
            m_name = "signature";
        } else if (jit_type_is_tagged(other) &&
                   jit_type_get_tagged_kind(other) == 20) {
            m_type = jit_type_copy(other);
            jit_type_t tagged = jit_type_get_tagged_type(other);
            m_ref_type = jit_type_copy(jit_type_get_ref(tagged));
            m_name = "array";
        } else if (jit_type_is_tagged(other) &&
                   jit_type_get_tagged_kind(other) == 22) {
            m_type = jit_type_copy(other);
            jit_type_t tagged = jit_type_get_tagged_type(other);
            m_ref_type = jit_type_copy(jit_type_get_ref(tagged));
            m_name = "tuple";
        } else if (jit_type_get_kind(other) <= JIT_TYPE_MAX_PRIMITIVE) {
         // printf("can't make type %d\n", jit_type_get_kind(other));
            if (jit_type_get_kind(other) == JIT_TYPE_INVALID) {
                std::abort();
            }
            m_type = jit_type_copy(other);
         // std::abort();
        } else {
            m_type = jit_type_copy(other);
        }
     // printf("%p: making %s type, kind %d\n",
     //        this, m_name, jit_type_get_kind(m_type));
    }
    jit_type(const jit_type_t other, const char *nm)
        : m_type(jit_type_copy(other)), m_name(nm),
          m_ref_type(NULL), m_ref_type_cached(NULL) {}

    ~jit_type() {
        printf("dealloc ");
        name();
        jit_type_free(m_type);
    }

 // static jit_type *t_void()            { return new jit_type(jit_type_void); }
 // static jit_type *t_sbyte()           { return new jit_type(jit_type_sbyte); }
 // static jit_type *t_ubyte()           { return new jit_type(jit_type_ubyte); }
 // static jit_type *t_short()           { return new jit_type(jit_type_short); }
 // static jit_type *t_ushort()          { return new jit_type(jit_type_ushort); }
 // static jit_type *t_int()             { return new jit_type(jit_type_int); }
 // static jit_type *t_uint()            { return new jit_type(jit_type_uint); }
 // static jit_type *t_nint()            { return new jit_type(jit_type_nint); }
 // static jit_type *t_nuint()            { return new jit_type(jit_type_nuint); }
 // static jit_type *t_long()            { return new jit_type(jit_type_long); }
 // static jit_type *t_ulong()           { return new jit_type(jit_type_ulong); }
 // static jit_type *t_float32()         { return new jit_type(jit_type_float32); }
 // static jit_type *t_float64()         { return new jit_type(jit_type_float64); }
 // static jit_type *t_nfloat()          { return new jit_type(jit_type_nfloat); }
 // static jit_type *t_void_ptr()        { return new jit_type(jit_type_void_ptr); }
 // static jit_type *t_sys_bool()        { return new jit_type(jit_type_sys_bool); }
 // static jit_type *t_sys_char()        { return new jit_type(jit_type_sys_char); }
 // static jit_type *t_sys_schar()       { return new jit_type(jit_type_sys_schar); }
 // static jit_type *t_sys_uchar()       { return new jit_type(jit_type_sys_uchar); }
 // static jit_type *t_sys_short()       { return new jit_type(jit_type_sys_short); }
 // static jit_type *t_sys_ushort()      { return new jit_type(jit_type_sys_ushort); }
 // static jit_type *t_sys_int()         { return new jit_type(jit_type_sys_int); }
 // static jit_type *t_sys_uint()        { return new jit_type(jit_type_sys_uint); }
 // static jit_type *t_sys_long()        { return new jit_type(jit_type_sys_long); }
 // static jit_type *t_sys_ulong()       { return new jit_type(jit_type_sys_ulong); }
 // static jit_type *t_sys_longlong()    { return new jit_type(jit_type_sys_longlong); }
 // static jit_type *t_sys_ulonglong()   { return new jit_type(jit_type_sys_ulonglong); }
 // static jit_type *t_sys_float()       { return new jit_type(jit_type_sys_float); }
 // static jit_type *t_sys_double()      { return new jit_type(jit_type_sys_double); }
 // static jit_type *t_sys_long_double() { return new jit_type(jit_type_sys_long_double); }    
#define str_helper(x) #x
#define STR(x) str_helper(x)
#define make_constructor(type_name) \
    static jit_type *t_##type_name() { \
        return new jit_type(jit_type_##type_name, STR(type_name)); \
    }
    make_constructor(void)
    make_constructor(sbyte)
    make_constructor(ubyte)
    make_constructor(short)
    make_constructor(ushort)
    make_constructor(int)
    make_constructor(uint)
    make_constructor(nint)
    make_constructor(nuint)
    make_constructor(long)
    make_constructor(ulong)
    make_constructor(float32)
    make_constructor(float64)
    make_constructor(nfloat)
    make_constructor(void_ptr)
    make_constructor(sys_bool)
    make_constructor(sys_char)
    make_constructor(sys_schar)
    make_constructor(sys_uchar)
    make_constructor(sys_short)
    make_constructor(sys_ushort)
    make_constructor(sys_int)
    make_constructor(sys_uint)
    make_constructor(sys_long)
    make_constructor(sys_ulong)
    make_constructor(sys_longlong)
    make_constructor(sys_ulonglong)
    make_constructor(sys_float)
    make_constructor(sys_double)
    make_constructor(sys_long_double)

    // GB
    static jit_type *t_array(jit_type_t ref_type = NULL) {
        if (!ref_type) {
            std::abort();
        }
        jit_type_t pointer_type = jit_type_create_pointer(ref_type, 1);
        jit_type *t = create_tagged(pointer_type,
                                    /* tag: */ 20 /* PYJIT_TYPE_ARRAY */,
                                    /* extra data: */ NULL,
                                    /* free function: */ free,
                                    /* incref: */ 1);
        t->m_name = "array";
     // printf("made array %p\n", t);
        t->m_ref_type = jit_type_copy(ref_type);
     // printf("t_array(%p) => %p ", ref_type, t);
     // t->name();
        return t;
    }

    static jit_type *array_t(jit_type_t ref_type) {
        jit_type *t = t_array(ref_type);
     // printf("array_t => %p ", t);
     // t->name();
        return t;
    }

    static jit_type *t_tuple() {
        jit_type_t pointer_type = jit_type_void_ptr;
        jit_type *t = create_tagged(pointer_type,
                                    /* tag: */ 22 /* PYJIT_TYPE_TUPLE */,
                                    /* extra data: */ NULL,
                                    /* free function: */ free,
                                    /* incref: */ 1);
        t->m_name = "tuple";
        return t;
    }

    static jit_type *tuple_t() {
        jit_type *t = t_tuple();
        return t;
    }

#if 1
    // GB
    static jit_type *create_pointer(jit_type_t type, int incref)
    {
        jit_type_t retval = jit_type_create_pointer(type, incref);
        if (retval) {
            jit_type *t = new jit_type(retval, "pointer");
            return t;
        }
        return retval ? new jit_type(retval) : NULL;
    }
#else

    // GB
    static jit_type *create_pointer(jit_type *type, int incref)
    {
        printf("|\n| new create_pointer\n|\n");
        jit_type_t retval = jit_type_create_pointer(type->m_type, incref);
        if (retval) {
            jit_type *t = new jit_type(retval, "pointer");
            t->m_ref_type = type;
            printf("created pointer to:"); type->name();
            return t;
        }
        return retval ? new jit_type(retval) : NULL;
    }
#endif

    static jit_type *create_struct(jit_type_t *fields, unsigned int num_fields, int incref)
    {
        jit_type_t retval = jit_type_create_struct(fields, num_fields, incref);
        return retval ? new jit_type(retval) : NULL;
    }

    static jit_type *create_union(jit_type_t *fields, unsigned int num_fields, int incref)
    {
        jit_type_t retval = jit_type_create_union(fields, num_fields, incref);
        return retval ? new jit_type(retval) : NULL;
    }

    static jit_type *create_signature(jit_abi_t abi, jit_type_t return_type,
                                      jit_type_t *params,
									  unsigned int num_params, int incref)
    {
        jit_type_t retval = jit_type_create_signature(abi, return_type,
                                                      params, num_params, incref);
        if (retval) {
            jit_type *t = new jit_type(retval, "<signature>");
            return t;
        }
        return retval ? new jit_type(retval) : NULL;
    }

    static jit_type *create_tagged(jit_type_t type, int kind, void *data, jit_meta_free_func free_func, int incref)
    {
        jit_type_t retval = jit_type_create_tagged(type, kind, data, free_func, incref);
        return retval ? new jit_type(retval, "<tagged>") : NULL;
    }

    static jit_nuint best_alignment() { return jit_type_best_alignment(); }

    jit_type_t raw() const { return m_type; }

    int set_names(char **names, unsigned int num_names) { return jit_type_set_names(m_type, names, num_names); }

    void set_size_and_alignment(jit_nint size, jit_nint alignment)
        { jit_type_set_size_and_alignment (m_type, size, alignment); }

    void set_offset(unsigned int field_index, jit_nuint offset)
        { jit_type_set_offset (m_type, field_index, offset); }

    int get_kind() { return jit_type_get_kind(m_type); }

    jit_nuint get_size() { return jit_type_get_size(m_type); }

    unsigned int num_fields() { return jit_type_num_fields(m_type); }

    jit_type *get_field(unsigned int field_index)
    {
        jit_type_t retval = jit_type_get_field(m_type, field_index);
        return retval ? new jit_type(retval) : NULL;
    }

    jit_nuint get_offset(unsigned int field_index) { return jit_type_get_offset(m_type, field_index); }

    const char *get_name(unsigned int index) { return jit_type_get_name(m_type, index); }

    unsigned int find_name(const char *name) { return jit_type_find_name(m_type, name); }

    unsigned int num_params() { return jit_type_num_params(m_type); }

    jit_type *get_return()
    {
        jit_type_t retval = jit_type_get_return(m_type);
        return retval ? new jit_type(retval) : NULL;
    }


    jit_type *get_param(unsigned int param_index)
    {
        jit_type_t retval = jit_type_get_param(m_type, param_index);
        return retval ? new jit_type(retval) : NULL;
    }

    jit_abi_t get_abi() {return jit_type_get_abi(m_type); }

    jit_type *get_ref()
    {
        if (m_ref_type != NULL) {
         // printf("get_ref on %s, m_ref_type %p: ", m_name, m_ref_type);
            if (m_ref_type_cached != NULL) {
             // printf("return cached %p\n", m_ref_type_cached);
            } else {
                m_ref_type_cached = new jit_type(m_ref_type, "ref type");
             // printf("new cached %p (%p)\n", m_ref_type_cached, m_ref_type_cached->m_type);
            }
            return m_ref_type_cached;
        }
        jit_type_t retval = jit_type_get_ref(m_type);
        return retval ? new jit_type(retval, "<ref>") : NULL;
    }

    jit_type *get_tagged_type()
    {
        jit_type_t retval = jit_type_get_tagged_type(m_type);
        return retval ? new jit_type(retval) : NULL;
    }

    void set_tagged_type(jit_type_t underlying, int incref)
    { jit_type_set_tagged_type(m_type, underlying, incref); }

    void *get_tagged_data() { return jit_type_get_tagged_data(m_type); }

    void set_tagged_data(void *data, jit_meta_free_func free_func)
       { jit_type_set_tagged_data(m_type, data, free_func); }

    int is_primitive() { return jit_type_is_primitive(m_type); }

    int is_struct() { return jit_type_is_struct(m_type); }

    int is_union() { return jit_type_is_union(m_type); }

    int is_signature() { return jit_type_is_signature(m_type); }

    int is_array() const {
     // printf("is_array(%p): is_tagged? %d (m_type = %p)\n",
     //        this, is_tagged(), m_type);
     // if (is_tagged()) {
     //     printf("tag (expect 20): %d\n", jit_type_get_tagged_kind(m_type));
     // }
        return is_tagged() && jit_type_get_tagged_kind(m_type) == 20;
    }

    int is_tuple() const {
        return is_tagged() && jit_type_get_tagged_kind(m_type) == 22;
    }

    int is_pointer() const { return jit_type_is_pointer(m_type); }

    int is_tagged() const { return jit_type_is_tagged(m_type); }

    jit_type *normalize()
    {
        jit_type_t ptr = m_type;
        return new jit_type(jit_type_normalize(ptr));
    }

    jit_type *remove_tags()
    {
        jit_type_t ptr = m_type;
        return new jit_type(jit_type_remove_tags(ptr));
    }

    jit_type *promote_int() { return new jit_type(jit_type_promote_int(m_type)); }

    int return_via_pointer() { return jit_type_return_via_pointer(m_type); }

    int has_tag(int kind) { return jit_type_has_tag(m_type, kind); }

    bool operator==(const jit_type &right) { return m_type == right.raw(); }

    bool operator!=(const jit_type &right) { return m_type != right.raw(); }

    void name() const
    {
        printf("%p ", this);
        printf("kind %d ", jit_type_get_kind(m_type));
        if (is_pointer())
            printf("pointer ");
        else if (is_array())
            printf("array ");
        if (is_pointer() || is_array() || m_ref_type) {
            printf("type <%s> with ref %p", m_name, m_ref_type);
            if (is_tagged())
                printf(" and tag %d\n", jit_type_get_tagged_kind(m_type));
            else
                printf("\n");
        }
        else
            printf("type <%s>\n", m_name);
    }
private:
    jit_type() {}
    jit_type_t m_type;
    const char *m_name;
    jit_type_t m_ref_type;
    jit_type *m_ref_type_cached;
};

#endif /* _JIT_PLUS_TYPE_H */
