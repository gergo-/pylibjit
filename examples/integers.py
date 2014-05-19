# pylibjit can use both Python's arbitrary-precision integers (int) or
# machine integers, which can overflow and wrap around. Only use machine
# integers if you know that overflow will not occur. For now, jit.Type.int
# appears to be a 32-bit integer type even on 64-bit platforms.

import jit, pylibjit

# arbitrary-precision integers
@pylibjit.compile(return_type=int, argument_types=[int],
                  dump_code=False)
def square_boxed(n):
    return n * n

# machine integers
@pylibjit.compile(return_type=jit.Type.int, argument_types=[jit.Type.int],
                  dump_code=False)
def square_unboxed(n):
    return n * n

print('square_unboxed(42) = {}, expected {}'.format(
        square_unboxed(42), square_boxed(42)))
# The correct result is 2**60, but that cannot be represented in a 32-bit
# integer; the unboxed multiplication silently overflows. The boxed
# multiplication is performed using the arbitrary-precision integers that
# Python normally uses.
print('square_unboxed(2**30) = {}, expected {}'.format(
        square_unboxed(2**30), square_boxed(2**30)))
