r"""
Make a 64-bit matlab_proxy (for use with 32-bit Python).

C:\Python27_x64\python -m virtualenv venv
.\venv\Scripts\pip install -r requirements.txt
.\venv\Scripts\python setup.py py2exe
.\venv\Scripts\python setup.py bdist_wheel -p win32
"""

from setuptools import setup, find_packages
import sys
import shutil
import os
import os.path
import platform

if sys.argv[1] == 'py2exe':
    import py2exe

with open('MANIFEST.in', 'wb') as manifest:
    if 'bdist_wheel' in sys.argv and 'win32' in sys.argv:
        manifest.write('recursive-include matlab_proxy/dist_{} *\n'.format(platform.architecture()[0]))
    else:
        manifest.write('prune matlab_proxy/dist_64bit\n')

setup(
    name="matlab_proxy",
    version="0.2",
    packages=['matlab_proxy'],
    include_package_data=True,
    zip_safe=False,
    options={"py2exe": {
                        # 'xref': True,
                        # 'unbuffered': True,
                        "dll_excludes": ['w9xpopen.exe'],
                        "excludes": """Tkinter _tkinter matlab matlab.engine site
                            _ctypes _hashlib _socket _ssl bz2 pyexpat select""".split(),
                        "includes": ["pkgutil", "importlib"],
                        "dist_dir": 'matlab_proxy/dist_' + platform.architecture()[0]

                        }},
    zipfile="Python{}{}.zip".format(*sys.version_info[0:2]),
    console=[{'script': "matlab_proxy/__init__.py"}],
)

if sys.argv[1] == 'py2exe':
    shutil.copyfile(sys.executable, os.path.join('matlab_proxy/dist_' + platform.architecture()[0], os.path.basename(sys.executable)))
    os.unlink(os.path.join('matlab_proxy/dist_' + platform.architecture()[0], '__init__.exe'))
