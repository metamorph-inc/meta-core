from setuptools import setup, find_packages

import meta_nrmm as this_package
from meta_nrmm.process_nrmm import FANG_DATA

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
      package_data={'meta_nrmm': ['templates/*'] + FANG_DATA},
      zip_safe=False,
      entry_points = {
        'console_scripts': [
            'process_nrmm = meta_nrmm.process_nrmm:main',
            ],
        }
    )
