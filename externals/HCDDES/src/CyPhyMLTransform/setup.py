from distutils.core import setup
import py2exe
import time

_current_time = time.gmtime()

class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # versioninfo resources
        self.version = time.strftime("%Y.%m.%d.%H%M%S", _current_time)
        self.company_name = "Vanderbilt University"
        self.name = "ESM2SFC_ESMoL2CyPhyML.py"

setup(
  console=[ Target( script = "ESM2SLC_ESMoL2CyPhyML.py" ), Target( script = "ESM2SFC_ESMoL2CyPhyML.py" ) ],
  zipfile=None,
  options={
    "py2exe":{
#     "dll_excludes" : ['w9xpopen.exe'],
      "bundle_files" : 1,
      "dist_dir"     : '../../bin'
    }
  },
)
