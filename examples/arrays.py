# pylibjit knows about Python's array class and can optimize element
# accesses if the array's type is known and the index type is an unboxed
# integer. Loops of the form  for i in range(...)  are unboxed (i.e., turned
# into counting loops without involving iterators) if i is declared as an
# unboxed integer.
# Beware: The optimization also throws away bounds checks! Only compile
# tested code that is known not to use bad indices.
# If you know that during each execution of the function some variable
# always refers to a single array object which does not change its size, you
# can document this fact by using the fixed_buffers annotation. This enables
# an aggressive optimization.

import array
import jit, pylibjit

arr = array.array('d', [1, 2, 3, 4, 5])

print('original array:', arr)

@pylibjit.compile(return_type=object,
                  argument_types=[jit.Type.array_t(jit.Type.float64)],
                  variables={'i': jit.Type.int},
                  fixed_buffers={'an_array'},
                  dump_code=False)
def map_add_1(an_array):
    for i in range(len(an_array)):
        an_array[i] += 1

map_add_1(arr)
print('modified array:', arr)

@pylibjit.compile(return_type=jit.Type.int,
                  argument_types=[jit.Type.array_t(jit.Type.float64)],
                  variables={'sum': jit.Type.float64, 'i': jit.Type.int},
                  fixed_buffers={'an_array'},
                  dump_code=False)
def array_sum(an_array):
    sum = 0
    for i in range(len(an_array)):
        sum += an_array[i]
    return sum

print('sum of', arr, 'is', array_sum(arr))
