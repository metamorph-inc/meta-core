import os
import sys
import win32com.client

# this config can be read from a json file
config = {
    'metaName' : 'CyPhyML',
    'extension' : 'png',
    'collapsedSuffix' : 'Folder',
    'expandedSuffix' : 'Folder_b'
}


if len(sys.argv) > 1:
	file = sys.argv[1]
else:
	import os.path
	file = os.path.abspath(config['metaName'] + '.mta')

base = os.path.splitext(file)[0]

if os.path.exists(file) == False:
	print '%s does not exist' % file
	xmplog = base + '.xmp.log'
	if os.path.exists(xmplog):
		with open(xmplog, 'r') as f_p:
			for line in iter(f_p):
				print line

import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None

project = win32com.client.DispatchEx('Mga.MgaMetaProject')
project.Open('MGA=' + file)
project.BeginTransaction()

icon_list_collapsed = list()
icon_list_expanded = list()

# get all defined folders
for folder in project.RootFolder.DefinedFolders:
	# add folder kind and icon files to the lists
	# replace :: namespace delimiter to __ in filenames
	icon_list_collapsed.append(
		(folder.Name,
		folder.Name.replace(':','_') + config['collapsedSuffix'] + '.' + config['extension']))

	icon_list_expanded.append(
		(folder.Name,
		folder.Name.replace(':','_') + config['expandedSuffix'] + '.' + config['extension']))

## iterate through kind, icon pairs and add those as registry nodes
#print 'Collapsed icons:'
for kind, icon in icon_list_collapsed:
	#print ' - ' + kind + ' : ' + icon
	cas = project.RootFolder.GetDefinedFolderByNameDisp(kind, True)
	cas.GetRegistryNodeDisp('treeIcon').Value = icon
	#for regnode in cas.RegistryNodes:
	#	print kind + ': ' + regnode.Name + '=' + regnode.Value
	
#print 'Expanded icons:'
for kind, icon in icon_list_expanded:
	#print ' - ' + kind + ' : ' + icon
	cas = project.RootFolder.GetDefinedFolderByNameDisp(kind, True)
	cas.GetRegistryNodeDisp('expandedTreeIcon').Value = icon
	#for regnode in cas.RegistryNodes:
	#	print kind + ': ' + regnode.Name + '=' + regnode.Value
	

project.CommitTransaction()
project.Close()
