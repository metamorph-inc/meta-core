from setuptools import setup, find_packages
import sys
import shutil
import os
import os.path
import platform

if sys.argv[1] == 'py2exe':
    import py2exe

setup(
    name="matlab_proxy",
    version="0.1",
    packages=find_packages(),
    options={"py2exe": {
                        # 'xref': True,
                        # 'unbuffered': True,
                        "dll_excludes": ['w9xpopen.exe'],
                        "excludes": """Tkinter _tkinter matlab matlab.engine site
                            _ctypes _hashlib _socket _ssl bz2 pyexpat select""".split(),
                        "includes": ["pkgutil", "importlib"],
                        "dist_dir": 'dist_' + platform.architecture()[0]

                        }},
    zipfile="Python{}{}.zip".format(*sys.version_info[0:2]),
    console=[{'script': "matlab_proxy.py"}],
)

if sys.argv[1] == 'py2exe':
    shutil.copyfile(sys.executable, os.path.join('dist_' + platform.architecture()[0], os.path.basename(sys.executable)))
    os.unlink(os.path.join('dist_' + platform.architecture()[0], 'matlab_proxy.exe'))
