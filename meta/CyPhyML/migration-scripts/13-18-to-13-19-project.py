# This is a migrator for CyPhy projects.
# We recently started tracking a Component's "backend folder" location
# inside CyPhy itself, instead of in the project.manifest.json.
# This migrator will take component location data from the project.manifest.json
# and insert it into the CyPhy model.


# Algorithm:
# - For each XME file, open it and export it again
#   - Ensure latest paradigm, and that the new Path attribute will exist for components
# - Crawl the CyPhy model
#   - For each component found, locate the analogue in the project.manifest.json
#   - Populate the Path field in the XME


### For each XME, import it and export it again.


import json
import fnmatch
import os
import xml.etree.ElementTree as ET


def get_component_path_fom_manifest(avmid):
    for c in a_manifestComponents:
        if c["avmid"] == avmid:
            path = c["modelpath"]
            return os.path.dirname(path)


def get_cyphy_component_avmid(component):
    for child in component.iter('attribute'):
        if child.attrib['kind'] == 'AVMID':
            return child.find('value').text


def set_cyphy_component_path(component, path):
    for child in component.findall("attribute[@kind='Path']"):
        child.find('value').text = path
        del child.attrib['status']


def migrate_xme(xme_path):
    tree = ET.parse(xme_path)
    root = tree.getroot()

    components = root.findall(".//folder/model[@kind='Component']")
    for component in components:
        avmid = get_cyphy_component_avmid(component)
        path = get_component_path_fom_manifest(avmid)

        if path is not None:
            set_cyphy_component_path(component, path)
        else:
            print("Could not find path in manifest for: ", component.find('name').text)

    _xml = """<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE project SYSTEM "mga.dtd">

""" + ET.tostring(root)

    xme_newpath = os.path.join(os.path.dirname(xme_path),
                               os.path.splitext(os.path.basename(xme_path))[0] + '_migrated.xme')
    with open(xme_newpath, 'w') as f:
        f.write(_xml)


### Open up the manifest.project.json file and get all component entries
with open("manifest.project.json") as f:
    d_manifest = json.load(f)

a_manifestComponents = d_manifest["Project"]["Components"]

### Using XPath, crawl the XME for Components
ap_xme = []
for file in os.listdir('.'):
    if fnmatch.fnmatch(file, '*.xme'):
        xml = migrate_xme(file)

print("done")