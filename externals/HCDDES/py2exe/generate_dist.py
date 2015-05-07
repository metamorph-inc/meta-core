# Script to generate .exe file for gme.py
# /c/Program\ Files/Python26/python gmepy-setup.py py2exe && mv dist/gme.exe gmepy.exe
 
from distutils.core import setup
import py2exe
import time
 
class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # versioninfo resources
        self.version = time.strftime("%Y.%m.%d.%H%M%S", time.gmtime())
        self.company_name = "Vanderbilt University"
        self.name = "sl_codegen" 
 
setup(console=[Target(script = "sl_codegen.py")],
options={"py2exe":{
                        "dll_excludes": ['w9xpopen.exe'],
                        "skip_archive": True,
                }
        },
)