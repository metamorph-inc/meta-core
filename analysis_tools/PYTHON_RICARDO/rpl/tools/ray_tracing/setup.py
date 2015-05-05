## add command line options: build_ext --inplace
import numpy
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [Extension("calc_r", ["calc_r.pyx"], include_dirs=[numpy.get_include()])]

setup(
  name = 'calc_r ext',
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules
)
