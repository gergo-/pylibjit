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

#ifndef	_JIT_PLUS_PYCONTEXT_H
#define	_JIT_PLUS_PYCONTEXT_H

#include <jit/jit-plus.h>

class jit_pycontext : public jit_context
{
public:
	jit_pycontext() : jit_context() {}
	jit_pycontext(jit_context_t context) : jit_context(context) {}

    bool resolve_all(bool print_failures) {
        return (bool)jit_readelf_resolve_all(this->raw(), print_failures ? 1 : 0);
    }

    bool register_symbol(const char *name, void *value, bool after) {
        return (bool)jit_readelf_register_symbol(this->raw(), name, value, after ? 1 : 0);
    }

    jit_debugger_t debugger() {
        return jit_debugger_from_context(this->raw());
    }
};

#endif /* _JIT_PLUS_PYCONTEXT_H */
