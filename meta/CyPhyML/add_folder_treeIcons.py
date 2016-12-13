import os
import os.path
import itertools
import sys
import uuid
from lxml import etree
import win32com.client.dynamic

# this config can be read from a json file
config = {
    'metaName': 'CyPhyML',
    'extension': 'png',
    'collapsedSuffix': 'Folder',
    'expandedSuffix': 'Folder_b'
}


if len(sys.argv) > 1:
    file = sys.argv[1]
else:
    file = os.path.abspath(config['metaName'] + '.mta')

base = os.path.splitext(file)[0]

if os.path.exists(file) == False:
    print '%s does not exist' % file
    xmplog = base + '.xmp.log'
    if os.path.exists(xmplog):
        with open(xmplog, 'r') as f_p:
            for line in iter(f_p):
                print line

metarefsFilename = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'metarefs')


def write_metarefs_from_xmp(output_filename):
    tree = etree.parse(base + '.xmp')
    with open(output_filename, 'w') as metarefs_out:
        def getNameXPath(element, name_attr='name'):
            return "{}[@{}='{}']".format(element.tag, name_attr, element.attrib[name_attr])

        def writeMetaref(xpath, element):
            metarefs_out.write(xpath + ' ' + element.attrib['metaref'] + '\n')
        parent_map = dict((c, p) for p in tree.getiterator() for c in p)
        for element in itertools.chain(*(tree.xpath('//' + kind) for kind in ('folder', 'model', 'atom', 'reference', 'set', 'connection'))):
            writeMetaref('//' + getNameXPath(element), element)

        for element in itertools.chain(*(tree.xpath('//' + tag) for tag in ('role', 'aspect', 'attrdef'))):
            writeMetaref('//' + getNameXPath(parent_map[element]) + '/' + getNameXPath(element), element)

        for element in tree.xpath('//part'):
            parent = parent_map[element]
            writeMetaref('//' + getNameXPath(parent_map[parent]) + '/' + getNameXPath(parent_map[element]) + '/' + getNameXPath(element, 'role'), element)

if not os.path.isfile(metarefsFilename):
    # remove this line if bootstrapping
    raise ValueError('"metarefs" is missing; it must be present to maintain metarefs in mta')
    write_metarefs_from_xmp(metarefsFilename + '_new')
else:
    tree = etree.parse(base + '.xmp')
    for element in itertools.chain(itertools.chain(*(tree.xpath('//' + kind) for kind in ('folder', 'model', 'atom', 'reference', 'set', 'connection'))),
            itertools.chain(*(tree.xpath('//' + tag) for tag in ('role', 'aspect', 'attrdef'))),
            tree.xpath('//part')):
        element.attrib['metaref'] = ''

    metarefMax = 0
    with open(metarefsFilename) as metarefs_in:
        for metarefLine in metarefs_in:
            xpath, metaref = metarefLine.split()
            matches = tree.xpath(xpath)
            if len(matches) > 1:
                raise ValueError('Too many matches for XPath {}'.format(xpath))
            if len(matches) == 0:
                sys.stderr.write('Warning: {} not found\n'.format(xpath))
                continue
            element = matches[0]
            element.attrib['metaref'] = metaref
            metarefMax = max(int(metaref), metarefMax)

    for element in itertools.chain(itertools.chain(*(tree.xpath('//' + kind) for kind in ('folder', 'model', 'atom', 'reference', 'set', 'connection'))),
            itertools.chain(*(tree.xpath('//' + tag) for tag in ('role', 'aspect', 'attrdef'))),
            tree.xpath('//part')):
        if element.attrib['metaref'] == '':
            metarefMax = metarefMax + 1
            element.attrib['metaref'] = str(metarefMax)

    xmpstat = os.stat(base + '.xmp')
    with open(base + '.xmp', 'w') as out:
        tree.write(out, xml_declaration=True, encoding=tree.docinfo.encoding)
    os.utime(base + '.xmp', (xmpstat.st_atime, xmpstat.st_mtime))

    write_metarefs_from_xmp(metarefsFilename)

    registrar = win32com.client.dynamic.Dispatch("Mga.MgaRegistrar")
    registrar.RegisterParadigmFromDataDisp('XML=' + base + '.xmp', 1)

project = win32com.client.dynamic.Dispatch('Mga.MgaMetaProject')
project.Open('MGA=' + file)
project.BeginTransaction()

icon_list_collapsed = list()
icon_list_expanded = list()

# get all defined folders
for folder in project.RootFolder.DefinedFolders:
    # add folder kind and icon files to the lists
    # replace :: namespace delimiter to __ in filenames
    icon_list_collapsed.append(
        (folder.Name, folder.Name.replace(':', '_') + config['collapsedSuffix'] + '.' + config['extension']))

    icon_list_expanded.append(
        (folder.Name, folder.Name.replace(':', '_') + config['expandedSuffix'] + '.' + config['extension']))

# iterate through kind, icon pairs and add those as registry nodes
# print 'Collapsed icons:'
for kind, icon in icon_list_collapsed:
    # print ' - ' + kind + ' : ' + icon
    cas = project.RootFolder.GetDefinedFolderByNameDisp(kind, True)
    cas.GetRegistryNodeDisp('treeIcon').Value = icon
    # for regnode in cas.RegistryNodes:
    #    print kind + ': ' + regnode.Name + '=' + regnode.Value

# print 'Expanded icons:'
for kind, icon in icon_list_expanded:
    # print ' - ' + kind + ' : ' + icon
    cas = project.RootFolder.GetDefinedFolderByNameDisp(kind, True)
    cas.GetRegistryNodeDisp('expandedTreeIcon').Value = icon
    # for regnode in cas.RegistryNodes:
    #    print kind + ': ' + regnode.Name + '=' + regnode.Value

# TODO: MetaGME has had General Preferences for Folders for a long time. Put treeIcon and expandedTreeIcon into the xme

project.CommitTransaction()
print 'metamodel GUID is {}'.format(uuid.UUID(bytes_le=project.GUID))
project.Close()
