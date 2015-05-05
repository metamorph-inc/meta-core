#This uses distutils rather than setuptools so that
# I can access material_library.json from where it
# installs, rather than having to extract it from
# a Python egg (much more work)

from distutils.core import setup

import sys
import os.path
sys.path.append(os.path.join(os.path.dirname(__file__), '.'))

import MaterialLibraryInterface as this_package

name = this_package.__name__
version = this_package.__version__
author = this_package.__author__
email = this_package.__email__
url = this_package.__url__

setup(name=name,
      version=version,
      author=author,
      author_email=email,
      url=url,
      packages=['MaterialLibraryInterface'],
      package_dir={'MaterialLibraryInterface': 'MaterialLibraryInterface'})