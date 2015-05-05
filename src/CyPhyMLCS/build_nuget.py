
import os
import os.path
import sys

MSBUILD = os.path.join(os.environ['windir'], r"Microsoft.NET\Framework\v4.0.30319\MSBuild.exe")
THIS_DIR = os.path.dirname(os.path.abspath(__file__))
nuget = os.path.join(THIS_DIR, "../.nuget/NuGet.exe")
version_filename = os.path.join(THIS_DIR, 'Properties/AssemblyFileVersion.cs')

def system(args, dirname=THIS_DIR):
    import subprocess
    subprocess.check_call(args, shell=True, cwd=dirname)

def get_svnversion(filename):
    import subprocess
    p = subprocess.Popen(['svnversion', '-n', filename], stdout=subprocess.PIPE)
    out, err = p.communicate()
    if p.returncode:
        raise subprocess.CalledProcessError(p.returncode, 'svnversion')
    return out

def update_version():
    # system([MSBUILD, os.path.join(THIS_DIR, "../.nuget/NuGet.Targets"), "/t:CheckPrerequisites", "/p:DownloadNuGetExe=True"])
    cyphy_version = int(get_svnversion(os.path.join(THIS_DIR, "../../meta/CyPhyML/CyPhyML.xme")))
    print "CyPhyML.xme version: " + str(cyphy_version)
    version_str = '1.0.0.%d' % cyphy_version
    cyphy_version_data = 'using System.Reflection;\n' + '[assembly: AssemblyFileVersion("%s")]\n' % version_str
    if not os.path.isfile(version_filename) or cyphy_version_data != file(version_filename, 'rb').read():
        with file(version_filename, 'wb') as cyphy_version_cs:
            cyphy_version_cs.write(cyphy_version_data)


def build():
    system([MSBUILD, os.path.join(THIS_DIR, "../CyPhyML.sln"), "/t:CyPhyLanguage\\CyPhyMLCS", "/p:Configuration=Release;Platform=Mixed Platforms", "/m", "/nodeReuse:false"])

def _parse_version():
    return file(version_filename, 'rb').read().split('"')[1]
    
def pack_nuget():
    system([nuget, "pack", os.path.join(THIS_DIR, "CyPhyMLCS.csproj"),
        "-Verbosity", "detailed",
        "-Version", _parse_version(),
        "-BasePath", THIS_DIR])

def push_nuget():
    system([nuget, "push", "META.CyPhyML.%s.nupkg" % _parse_version(),
        "-Source", "http://build.isis.vanderbilt.edu/"])
    

if __name__ == '__main__':
    for command in sys.argv[1:]:
        getattr(sys.modules['__main__'], command)()
