from distutils.core import setup, Extension

module = Extension('jit',
                   include_dirs = ['include', '/usr/include',
                                   # modify the following if needed
                                   '../../install/include'],
                   libraries = ['jit', 'jitplus', 'jitdynamic'],
                   library_dirs = ['/usr/lib64', '/usr/lib',
                                   # modify the following if needed
                                   '../../install/lib'],
                   sources = ['src/jit_context.cc',
                              'src/jit_function.cc',
                              'src/jit_jump_table.cc',
                              'src/jit_label.cc',
                              'src/jit_type.cc',
                              'src/jit_value.cc',
                              'src/jit_elf.cc',
                              'src/jit_debugger.cc',
                              'src/jit_dynlib.cc',
                              'src/jit_module.cc'])

setup (name = 'jit',
       version = '0.1',
       description = 'Python bindings for libjit',
       ext_modules = [module])
