import os
from xml.etree import ElementTree

# get all dlls that will be present in the GAC on the user's machine
meta_x64_wxs = '..\deploy\META_x64.wxs'
meta_x64 = ElementTree.parse(meta_x64_wxs)

dlls_in_gac = []

_wix = '{http://schemas.microsoft.com/wix/2006/wi}'
for assmebly in list(meta_x64.findall('.//' + _wix + 'File[@Assembly=\'.net\']')):
    #print assmebly.get('Source')
    dlls_in_gac.append(os.path.basename(assmebly.get('Source')))

#print dlls_in_gac
    
_msbuild = '{http://schemas.microsoft.com/developer/msbuild/2003}'

ElementTree.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003') 

cwd = os.getcwd()

projects = [l.split(",")[1][2:-1] for l in open("CyPhy-FunctionalTests.sln") if l.startswith("Project")]
project_basenames = set((os.path.basename(project) for project in projects))

for project in projects:
    print 'Updating ' + project
    csproj = ElementTree.parse(project)
    
    # switch directly referenced dlls that are GACed to GAC.
    for reference in list(csproj.findall(_msbuild + 'ItemGroup/' + _msbuild + 'Reference')):
        #ElementTree.dump(reference)
        remove = False
        children = []
        for child in reference:
            children.append(child)
            if child.tag == _msbuild + 'HintPath':
                if os.path.basename(child.text) in dlls_in_gac:
                    #print 'In gac' + child.text
                    remove = True
        
        # remove hint path and specific version
        if remove:
            for child in children:
                #print child.tag
                # we remove all elements inside the Reference element (SpecificVersion and HintPath)
                reference.remove(child)
    
    parent_map = dict((c, p) for p in csproj.getiterator() for c in p)
    for project_reference in list(csproj.findall(_msbuild + 'ItemGroup/' + _msbuild + 'ProjectReference')):
        #print project_reference.get('Include')
        dep = ElementTree.parse(os.path.join(os.path.dirname(project), project_reference.get('Include')))
        dep_name = os.path.basename(project_reference.get('Include'))
        if dep_name in project_basenames: 
            continue
        for outputpath in dep.findall(_msbuild + 'PropertyGroup/' + _msbuild + 'AssemblyName'):
            dep_name = outputpath.text
        extension = '.dll'
        for outputtype in dep.findall(_msbuild + 'PropertyGroup/' + _msbuild + 'OutputType'):
            extension = outputtype.text
            #print extension
        dep_asm = dep_name + {'WinExe': '.exe', 'Exe': '.exe', 'Library': '.dll'}[extension]
        # TODO check if dep_asm is in C:\Program Files (x86)\META\bin
        # TODO get install location from registry
        
        new_reference = ElementTree.fromstring("""<?xml version="1.0" encoding="utf-8"?>
<Reference Include="{0}" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
        <SpecificVersion>False</SpecificVersion>
        <HintPath>C:\\Program Files\\META\\bin\\{1}</HintPath>
    </Reference>""".format(dep_name, dep_asm))
        
        reference_itemgroup = csproj.find(_msbuild + 'ItemGroup/' + _msbuild + 'Reference/..')
        reference_itemgroup.insert(1, new_reference)
        parent_map[project_reference].remove(project_reference)


    # switch projectreferences that are GACed to GAC.
    # TODO: use strong names!
    for reference in list(csproj.findall(_msbuild + 'ItemGroup/' + _msbuild + 'Reference')):
        #ElementTree.dump(reference)
        remove = False
        children = []
        for child in reference:
            children.append(child)
            if child.tag == _msbuild + 'HintPath':
                if os.path.basename(child.text) in dlls_in_gac:
                    #print 'In gac' + child.text
                    remove = True
        
        # remove hint path and specific version
        if remove:
            for child in children:
                #print child.tag
                # we remove all elements inside the Reference element (SpecificVersion and HintPath)
                reference.remove(child)     
        
        
    #ElementTree.dump(csproj)
    with open(project, 'w') as f_p:
        csproj.write(f_p)
    # with open(project + '.csproj', 'w') as f_p:
        # csproj.write(f_p)