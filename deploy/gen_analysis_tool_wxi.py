import sys
import os
import json

from xml.etree import ElementTree
from gen_dir_wxi import CommentedTreeBuilder, add_wix_to_path


def _adjacent_file(file_name):
    import os.path
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), file_name)


def pretty_print_xml(xml_filename):
    import xml.dom.minidom

    xml = xml.dom.minidom.parse(xml_filename)  # or xml.dom.minidom.parseString(xml_string)
    pretty_xml_as_string = xml.toprettyxml()
    with open(xml_filename, 'w') as f_p:
        f_p.write(pretty_xml_as_string)


def generate_wxi(src, output_filename=None, id=None, diskId=None):
    add_wix_to_path()

    prefix = "analysis_tool_"

    while src[-1] in ('/', '\\'):
        src = src[:-1]
    name = os.path.basename(src)
    id = id or prefix + name.replace('-', '_').replace(' ', '_')
    output_filename = output_filename or _adjacent_file(id + ".wxi")

    import subprocess

    def check_call(args):
        print " ".join(args)
        subprocess.check_call(args)

    check_call(['heat', 'dir', _adjacent_file(src), '-template', 'fragment', '-sreg', '-scom',
      '-o', output_filename, '-ag', '-cg', id, '-srd', '-var', 'var.' + id, '-dr', id, '-nologo'])

    tree = ElementTree.parse(output_filename, parser=CommentedTreeBuilder()).getroot()
    tree.insert(0, ElementTree.Comment('generated with gen_analysis_tool_wxi.py %s\n' % src))
    tree.insert(0, ElementTree.ProcessingInstruction('define', '%s=%s' % (id, os.path.normpath(src))))
    parent_map = dict((c, p) for p in tree.getiterator() for c in p)
    for file in tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}Component/{http://schemas.microsoft.com/wix/2006/wi}File"):
        if file.get('Source', '').find('.svn') != -1:
            comp = parent_map[file]
            parent_map[comp].remove(comp)
    for dir in tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}Directory"):
        if dir.get('Name', '') == '.svn':
            for dirref in tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}DirectoryRef"):
                if dirref.get('Id', '') == dir.get('Id', ''):
                    frag = parent_map[dirref]
                    parent_map[frag].remove(frag)
            parent_map[dir].remove(dir)
    if diskId:
        for component in tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}Component"):
            component.attrib['DiskId'] = diskId

    # add registry nodes
    componentGroup = tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}ComponentGroup[@Id='" + id + "']")[0]
    componentRegistry = ElementTree.SubElement(componentGroup, 'ns0:Component', {'Id': id + '_RegistryEntry', 'Directory': id, 'Guid': '*', 'Win64': 'no'})

    manifest = None
    with open(os.path.join(src, 'analysis_tool.manifest.json'), 'r') as f_p:
        manifest = json.load(f_p)

    for tool_name in manifest:

        registryKey = ElementTree.SubElement(componentRegistry, 'ns0:RegistryKey', {'Root': 'HKLM', 'Key': 'Software\\META\\AnalysisTools\\' + tool_name})
        ElementTree.SubElement(registryKey, 'ns0:RegistryValue', {'Name': 'InstallLocation', 'Type': 'string', 'Value': '[INSTALLDIR]\\analysis_tools\\' + name})
        ElementTree.SubElement(registryKey, 'ns0:RegistryValue', {'Name': 'Version', 'Type': 'string', 'Value': manifest[tool_name]['version']})
        ElementTree.SubElement(registryKey, 'ns0:RegistryValue', {'Name': 'OutputDirectory', 'Type': 'string', 'Value': '[INSTALLDIR]\\analysis_tools\\' + name + '\\' + manifest[tool_name]['outputDirectory']})
        ElementTree.SubElement(registryKey, 'ns0:RegistryValue', {'Name': 'RunCommand', 'Type': 'string', 'Value': manifest[tool_name]['runCommand']})
        ElementTree.SubElement(registryKey, 'ns0:RegistryValue', {'Name': 'RequiredInterpreter', 'Type': 'string', 'Value': manifest[tool_name]['requiredInterpreter']})

    ElementTree.ElementTree(tree).write(output_filename, xml_declaration=True)

    return name, id


def main(analysis_tools_dir, diskId=None):
    directories = [os.path.join(analysis_tools_dir, name) for name in os.listdir(analysis_tools_dir) if os.path.isdir(os.path.join(analysis_tools_dir, name)) and name not in ('.svn', 'images')]
    tools = []
    for tool_dir in directories:
        (name, tool_id) = generate_wxi(tool_dir, diskId=diskId)
        tools.append({'name': name, 'id': tool_id})

    # generate analysis_tools.wxi
    tree = ElementTree.Element('Wix', {'xmlns': "http://schemas.microsoft.com/wix/2006/wi"})
    comment = ElementTree.Comment(('generated with gen_analysis_tool_wxi.py %s\n' % analysis_tools_dir))
    tree.append(comment)
    fragment = ElementTree.SubElement(tree, 'Fragment')
    componentGroup = ElementTree.SubElement(fragment, 'ComponentGroup', {'Id': 'analysis_tools'})
    directoryRef = ElementTree.SubElement(fragment, 'DirectoryRef', {'Id': 'analysis_tools'})

    for tool in tools:
        tool_component_node = ElementTree.SubElement(componentGroup, 'ComponentGroupRef', {'Id': tool['id']})
        tool_dir_node = ElementTree.SubElement(directoryRef, 'Directory', {'Id': tool['id'], 'Name': tool['name']})

    for file_ in ('Readme.md', 'add_registry_entries.py'):
        file_component = ElementTree.SubElement(componentGroup, 'Component', {'Directory': 'analysis_tools', 'Id': 'analysis_tools_' + file_})
        file_file = ElementTree.SubElement(file_component, 'File', {'Source': '..\\analysis_tools\\' + file_})
        if file_ ==  'add_registry_entries.py':
            file_registrykey = ElementTree.SubElement(file_component, 'RegistryKey', {'Key': r'Software\META\AnalysisTools', 'Root': 'HKLM', 'ForceDeleteOnUninstall': 'yes'})
    
    ElementTree.ElementTree(tree).write("analysis_tools.wxi", xml_declaration=True)

    pretty_print_xml("analysis_tools.wxi")

if __name__ == '__main__':
    main(sys.argv[1])