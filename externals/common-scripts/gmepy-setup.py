# Script to generate .exe file for gme.py
# /c/Python27/python gmepy-setup.py py2exe && mv dist/gme.exe gmepy.exe

# XP requires embedded manifests. To embed into CSharpDSMLGenerator.dll:
# mt -manifest CSharpDSMLGenerator.manifest -outputresource:CSharpDSMLGenerator.dll;2
# sn -R CSharpDSMLGenerator.dll %GME_ROOT%\SDK\DotNet\DsmlGenerator\CSharpDsmlGenerator\AssemblySignature.snk

import sys
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
        self.name = "gmepy"


manifest = """
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
  <security>
    <requestedPrivileges>
      <requestedExecutionLevel level="asInvoker" uiAccess="false"></requestedExecutionLevel>
    </requestedPrivileges>
  </security>
</trustInfo>
<assemblyIdentity type="win32" name="gmepy" version="%(version)s" />
<dependency>
  <dependentAssembly>
    <assemblyIdentity type="win32" name="ISIS.MetaGME2UML" version="1.7.4.5" />
  </dependentAssembly>
</dependency>
<dependency>
  <dependentAssembly>
    <assemblyIdentity type="win32" name="CSharpDSMLGenerator" version="1.0.8.0" publicKeyToken="1321e6b92842fe54" processorArchitecture="msil"/>
  </dependentAssembly>
</dependency>
<dependency>
  <dependentAssembly>
    <assemblyIdentity type="win32" name="Microsoft.VC90.CRT" version="9.0.21022.8" processorArchitecture="x86" publicKeyToken="1fc8b3b9a1e18e3b" />
  </dependentAssembly>
</dependency>
</assembly>
""" % dict(version=time.strftime("%Y.%m.%d.0", _current_time))

if len(sys.argv) < 2:
    sys.argv.append('py2exe')
setup(console=[Target(script = "gme.py", other_resources = [(24, 1, manifest)] )], zipfile=None,
options={"py2exe":{
                        "dll_excludes": ['w9xpopen.exe','API-MS-Win-Core-LocalRegistry-L1-1-0.dll', 'MPR.dll'],
                        "bundle_files": 1,
                }
        },
)
if sys.argv[1] == 'py2exe':
    import shutil
    import os.path
    this_dir = os.path.dirname(os.path.abspath(__file__))
    shutil.copyfile(os.path.join(this_dir, r'dist\gme.exe'), os.path.join(this_dir, 'gmepy.exe'))
