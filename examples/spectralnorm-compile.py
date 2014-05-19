# The Computer Language Benchmarks Game
# http://benchmarksgame.alioth.debian.org/
#
# Contributed by Sebastien Loisel
# Fixed by Isaac Gouy
# Sped up by Josh Goldfoot
# Dirtily sped up by Simon Descarpentries
# Sped up by Joseph LaFata

from array     import array
from math      import sqrt
from sys       import argv
import sys

if sys.version_info < (3, 0):
    from itertools import izip as zip
else:
    xrange = range

import jit, pylibjit

@pylibjit.compile(return_type=jit.Type.float64,
                  argument_types=[jit.Type.int] * 2)
def eval_A (i, j):
    return 1.0 / (((i + j) * (i + j + 1) >> 1) + i + 1)

eval_A_times_u_compiler = \
    pylibjit.compile(return_type=jit.Type.void,
                     argument_types=[jit.Type.array_t(jit.Type.float64)] * 2,
                     variables={'u_len': jit.Type.int, 'i': jit.Type.int,
                                'j': jit.Type.int,
                                'partial_sum': jit.Type.float64,
                                'eval_A': jit.Type.float64},
                     dump_code=False)

@eval_A_times_u_compiler
def eval_A_times_u (u, results):
    u_len = len (u)
    
    for i in range (u_len):
        partial_sum = 0
        
        j = 0
        while j < u_len:
            partial_sum += eval_A(i, j) * u[j]
            j += 1
        
        results[i] = partial_sum

@eval_A_times_u_compiler
def eval_At_times_u (u, results):
    u_len = len (u)
    
    for i in range (u_len):
        partial_sum = 0
        
        j = 0
        while j < u_len:
            partial_sum += eval_A(j, i) * u[j]
            j += 1
        
        results[i] = partial_sum

def eval_AtA_times_u (u, out, tmp):
    eval_A_times_u (u, tmp)
    eval_At_times_u (tmp, out)

def main():
    n = int (argv [1])
    u = array("d", [1]) * n
    v = array("d", [1]) * n
    tmp = array("d", [1]) * n
    local_eval_AtA_times_u = eval_AtA_times_u
    
    for dummy in xrange (10):
        local_eval_AtA_times_u (u, v, tmp)
        local_eval_AtA_times_u (v, u, tmp)
    
    vBv = vv = 0
    
    for ue, ve in zip (u, v):
        vBv += ue * ve
        vv  += ve * ve
    
    print("result = %0.9f" % (sqrt(vBv/vv)))

main() 
