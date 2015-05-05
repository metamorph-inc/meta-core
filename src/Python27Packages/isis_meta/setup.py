from setuptools import setup, find_packages

import isis_meta as this_package

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
      packages=find_packages(),
      package_dir={name: name},
      #zip_safe=False,
      )
