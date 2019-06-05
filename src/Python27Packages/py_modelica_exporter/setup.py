from __future__ import absolute_import
from setuptools import setup, find_packages

import py_modelica_exporter

name = py_modelica_exporter.__name__
version = py_modelica_exporter.__version__
author = py_modelica_exporter.__author__
email = py_modelica_exporter.__email__
url = py_modelica_exporter.__url__

setup(name=name,
      license='GPLv3',
      version=version,
      author=author,
      author_email=email,
      url=url,
      packages=find_packages(),
      package_dir={name: name},
      install_requires=["six", "omniorb", "OMPython", "pyparsing"]
      #zip_safe=False,
      )
