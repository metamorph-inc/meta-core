"""
Builds a combined .sln file, enabling parallel build (which can be a couple minutes faster)

sln is based on:
src\CyPhyML.sln
externals\desert\desertVS2010.sln
src\Cyber2Code.sln
src\Cyber-Tools.sln
and some .vcxprojs
"""
import os
import os.path
import sys
import win32com.client
from time import sleep # FIXME appears there are races...
import ctypes
import subprocess

meta_path = os.path.dirname(os.path.abspath(__file__))
os.chdir(meta_path)
_vs_version = "14.0" # do not use VS 2010 or earlier

subprocess.check_call([r'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat'] + '& cl RetryIOleFilter.cpp'.split() +
    [r'/IC:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\km\crt'] + [r'/IC:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\km'] + [r'/IC:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\um'] +
    '/LD /link /out:RetryIOleFilter.dll'.split() + [r'/libpath:C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x64'], shell=True)

RetryIOleFilter = ctypes.windll.LoadLibrary(os.path.join(meta_path, 'RetryIOleFilter'))
RetryIOleFilter.Register()

def allProjects(sln):
    '''get all projects in a DTE solution, even ones in solution folders'''
    import collections
    q = collections.deque()
    q.extend(sln)
    while len(q):
        i = q.pop()
        if getattr(i, "CodeModel", None):
            yield i
        elif getattr(i, "ProjectItems", None):
            q.extendleft(list(i.ProjectItems))
        if getattr(i, "SubProject", None):
            q.extendleft([i.SubProject])
        #print str(i) + "   " + str(win32com.client.DispatchEx("InterfaceEnum").Enum(i))


def get_projs(rel_sln, platform="x64", configuration="Release"):
    sln = win32com.client.DispatchEx("VisualStudio.Solution.%s" % _vs_version)
    # print win32com.client.DispatchEx("InterfaceEnum").Enum(sln)
    slnname = meta_path + "\\" + rel_sln
    assert os.path.isfile(slnname)
    sln.Open(slnname)
    sleep(3)
    desert_projects = [os.path.normpath(os.path.join(os.path.dirname(slnname), proj.UniqueName)) for proj in allProjects(sln)]
    print desert_projects
    desert_configs = {}
    for solutionConfiguration2 in sln.SolutionBuild.SolutionConfigurations:
        if solutionConfiguration2.Name == configuration and solutionConfiguration2.PlatformName == platform:
            for solutionContext in solutionConfiguration2.SolutionContexts:
                desert_configs[os.path.relpath(os.path.dirname(rel_sln) + "\\" + solutionContext.ProjectName, "src")] = solutionContext.ConfigurationName + "|" + solutionContext.PlatformName
    sln.Close()
    return (desert_projects, desert_configs)

desert_projects, desert_configs = get_projs(r"externals\desert\desertVS2010.sln", configuration="ReleaseUnicode")
#cyber_projects, cyber_configs = get_projs(r"src\Cyber2Code.sln")
#cybertools_projects, cybertools_configs = get_projs(r"src\Cyber-Tools.sln")

dep_projects = set(desert_projects)
dep_configs = dict(desert_configs.items())

sln = win32com.client.DispatchEx("VisualStudio.Solution.%s" % _vs_version)
# print win32com.client.DispatchEx("InterfaceEnum").Enum(sln)
sln.Open(meta_path + r"\src\CyPhyML.sln")
sleep(3)
cyphy_projects = [os.path.normpath(os.path.join(os.path.dirname(meta_path + r"\src\CyPhyML.sln"), proj.UniqueName)) for proj in allProjects(sln)]
dep_projects2 = []
for proj in dep_projects:
    print proj
    if proj not in cyphy_projects:
        dep_projects2.append(sln.AddFromFile(proj, False))
for vcxproj in (r"src\MetaLink\MetaLink_maven.vcxproj", ):
    dep_projects2.append(sln.AddFromFile(os.path.join(meta_path, vcxproj), False))
    dep_configs[os.path.relpath(vcxproj, "src")] = "Release|Win32"
dep_projects = dep_projects2
dep_projects_names = [proj.UniqueName for proj in dep_projects]

sleep(2.5)
sln.SaveAs(os.path.join(meta_path, r"src\CyPhyMLCombined.sln"))

for solutionConfiguration2 in sln.SolutionBuild.SolutionConfigurations:
    if solutionConfiguration2.Name == "Release" and solutionConfiguration2.PlatformName == "Mixed Platforms":
        for solutionContext in solutionConfiguration2.SolutionContexts:
            if solutionContext.ProjectName in dep_projects_names:
                solutionContext.ConfigurationName = dep_configs[solutionContext.ProjectName] # n.b. this does not work with VS 2010

proj_byname = dict(((os.path.basename(proj.FullName), proj) for proj in allProjects(sln)))

def add_dep(proj, dep):
    bldDepends = sln.SolutionBuild.BuildDependencies.Item(proj_byname[proj].UniqueName)
    bldDepends.AddProject(proj_byname[dep].UniqueName)

add_dep('DesignSpaceHelper.vcxproj', 'desert.vcxproj')
#add_dep('CyPhy2SFC_CodeGen.vcxproj', 'libctemplate.vcxproj')
#add_dep('Cyber2SLC_CodeGen.vcxproj', 'libctemplate.vcxproj')
#add_dep('CyberCS.csproj', 'CyberRegister.vcxproj')
add_dep('CyPhyMetaLinkBridgeClient.csproj', 'MetaLink_maven.vcxproj')
#for vcxproj in map(os.path.basename, cyber_projects):
#    if vcxproj not in ('UpdateCyber2SLC_udm.vcxproj', 'Cyber.vcxproj'):
#        add_dep(vcxproj, 'UpdateCyber2SLC_udm.vcxproj')

# TODO: copy solution dependencies from dependent slns

sln.SaveAs(os.path.join(meta_path, r"src\CyPhyMLCombined.sln"))


# .FullName > c:\....vcxproj 
# .UniqueName > ..\externals\..vcxproj
# len([str(p) for p in allProjects(sln)])
# 'ComponentExporterUnitTests' in [str(p) for p in allProjects(sln)]
# AnalysisInterpreters  (u'IUnknown', u'IDispatch', u'ISupportVSProperties', u'Project', u'ISupportErrorInfo')
# http://msdn.microsoft.com/en-US/library/envdte.projectitem_members(v=vs.80).aspx
