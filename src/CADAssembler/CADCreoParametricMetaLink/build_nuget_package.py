import os
import os.path
import sys
from win32api import GetFileVersionInfo, HIWORD, LOWORD
import subprocess

MSBUILD = os.path.join(os.environ['windir'], r"Microsoft.NET\Framework\v4.0.30319\MSBuild.exe")
THIS_DIR = os.path.dirname(os.path.abspath(__file__))
nuget = os.path.join(THIS_DIR, "../../.nuget/NuGet.exe")

sys.path.append(os.path.join(THIS_DIR, '../../../deploy'))
import vc_info

def system(args, dirname=THIS_DIR):
    subprocess.check_call(args, shell=True, cwd=dirname)

def _get_version_number(filename):
    info = GetFileVersionInfo (filename, "\\")
    ms = info['FileVersionMS']
    ls = info['FileVersionLS']
    return (HIWORD(ms), LOWORD(ms), HIWORD(ls), LOWORD(ls))

def _get_version():
    version = ".".join(map(str, _get_version_number(os.path.join(THIS_DIR, r"..\x64\Release\CADCreoParametricMetaLink.exe"))))
    return vc_info.update_version(version, vc_info.last_cad_rev())
  
def pack_nuget():
    with open(os.path.join(THIS_DIR, 'vcinfo'), 'wb') as vcinfo:
        vcinfo.write(vc_info.repo_rev())
    vc_info.throw_if_modified()
    system([nuget, "pack", os.path.join(THIS_DIR, "CADCreoParametricMetaLink.nuspec"),
        "-Verbosity", "detailed",
        "-Version", _get_version(),
        "-BasePath", THIS_DIR])

def push_nuget():
    system([nuget, "push", os.path.join(THIS_DIR, "META.CADCreoParametricMetaLink.%s.nupkg" % _get_version()),
        "-Source", "http://build.isis.vanderbilt.edu/"])
    

if __name__ == '__main__':
    for command in sys.argv[1:]:
        getattr(sys.modules['__main__'], command)()
