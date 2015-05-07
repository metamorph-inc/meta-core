# Script to generate .exe file for gme.py
# /c/Python27/python gmepy-setup.py py2exe && mv dist/gme.exe gmepy.exe

# XP requires embedded manifests. To embed into CSharpDSMLGenerator.dll:
# mt -manifest CSharpDSMLGenerator.manifest -outputresource:CSharpDSMLGenerator.dll;2
# sn -R CSharpDSMLGenerator.dll %GME_ROOT%\SDK\DotNet\DsmlGenerator\CSharpDsmlGenerator\AssemblySignature.snk

# mt -manifest ISIS.MetaGME2Uml.manifest -outputresource:MetaGME2Uml.dll;2

# http://www.py2exe.org/index.cgi/win32com.shell
# ModuleFinder can't handle runtime changes to __path__, but win32com uses them
try:
    # py2exe 0.6.4 introduced a replacement modulefinder.
    # This means we have to add package paths there, not to the built-in
    # one.  If this new modulefinder gets integrated into Python, then
    # we might be able to revert this some day.
    # if this doesn't work, try import modulefinder
    try:
        import py2exe.mf as modulefinder
    except ImportError:
        import modulefinder
    import win32com, sys
    for p in win32com.__path__[1:]:
        modulefinder.AddPackagePath("win32com", p)
    for extra in ["win32com.shell"]: #,"win32com.mapi"
        __import__(extra)
        m = sys.modules[extra]
        for p in m.__path__[1:]:
            modulefinder.AddPackagePath(extra, p)
except ImportError:
    # no build path setup, no worries.
    pass

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
    <assemblyIdentity type="win32" name="ISIS.MetaGME2UML" version="1.7.7.10" />
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
                        "includes": [
                                "win32com.shell.shell",
                                "win32com.shell",
                                "win32process",
                                "win32event",
                                "winxpgui",
                                "win32api",
                                "win32con"]

                }
        },
)
if sys.argv[1] == 'py2exe':
    import shutil
    import os.path
    this_dir = os.path.dirname(os.path.abspath(__file__))
    shutil.copyfile(os.path.join(this_dir, r'dist\gme.exe'), os.path.join(this_dir, 'gmepy.exe'))
