import os
import os.path
import sys
from win32api import GetFileVersionInfo, HIWORD, LOWORD
import subprocess

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
nuget = os.path.join(THIS_DIR, "../src/.nuget/NuGet.exe")

sys.path.append(THIS_DIR)
import svn_info

package_name = 'MDL2MGACyber'

def system(args, dirname=THIS_DIR):
    subprocess.check_call(args, shell=True, cwd=dirname)

def _get_version():
    from xml.etree import ElementTree
    nuspec = ElementTree.parse(os.path.join(THIS_DIR, '%s.nuspec' % package_name))
    nuspec_version = nuspec.find('metadata/version').text

    _version = svn_info.update_version(nuspec_version, svn_info.last_mdl2mga_rev())
    return _version

def pack_nuget():
    with open(os.path.join(THIS_DIR, 'svnversion'), 'wb') as svnversion:
        svnversion.write(svn_info.svnversion())
    system([nuget, "pack", os.path.join(THIS_DIR, package_name + ".nuspec"),
        "-Verbosity", "detailed",
        "-Version", _get_version(),
        "-BasePath", THIS_DIR])

def push_nuget():
    system([nuget, "push", "META.%s.%s.nupkg" % (package_name, _get_version()),
        "-Source", "http://build.isis.vanderbilt.edu/"])
    
if __name__ == '__main__':
    for command in sys.argv[1:] or ['pack_nuget']:
        getattr(sys.modules['__main__'], command)()
