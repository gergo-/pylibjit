from distutils.core import setup

import pylibjit

setup(name = 'pylibjit',
      version = pylibjit.__version__,
      description='Compiler for fragments of Python code',
      author='Gergö Barany',
      author_email='gergo@complang.tuwien.ac.at',
      license='GPL',
      packages=['pylibjit'])
