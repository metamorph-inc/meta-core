﻿
from __future__ import absolute_import
from __future__ import print_function
import sys
import os
import six.moves.winreg as winreg
import six
with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
    meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
sys.path.append(os.path.join(meta_path, 'bin'))
import udm


def log(s):
    print(s)


try:
    import CyPhyPython  # will fail if not running under CyPhyPython
    import html

    def log(s):
        CyPhyPython.log(html.escape(s))
except ImportError:
    pass


def log_formatted(s):
    print(s)


try:
    import CyPhyPython  # will fail if not running under CyPhyPython
    import html

    def log(s):
        CyPhyPython.log(s)
except ImportError:
    pass


def start_pdb():
    ''' Starts pdb, the Python debugger, in a console window
    '''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    import sys
    sys.stdout = open('CONOUT$', 'wt')
    sys.stderr = sys.stdout
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


class ComputeClassificationCounts(object):
    def compute(self, currentobj, output_dir):
        json_filename = os.path.join(output_dir, 'summary.testresults.json')
        classifications = {}
        sut = [child for child in currentobj.children() if child.type.name == "TopLevelSystemUnderTest"]
        if len(sut) == 0:
            raise Exception(currentobj.name + ' has no TopLevelSystemUnderTest')
        sut = sut[0]
        import collections
        que = collections.deque()
        que.extend(sut.ref.children())
        while que:
            obj = que.pop()
            if obj.type.name == "ComponentRef":
                obj = obj.ref
            que.extend(obj.children())

            def add_classification(component):
                for class_ in (s for s in component.Classifications.replace("\r", "").split("\n") if not s.isspace() and s != ""):
                    classifications.setdefault(class_, [])
                    classifications[class_].append(component)
            if obj.type.name == "Component":
                add_classification(obj)

        import json
        json_data = {}
        if os.path.isfile(json_filename):
            with open(json_filename, "r") as json_file:
                json_data = json.load(json_file)
        counts = dict(((class_, len(members)) for class_, members in six.iteritems(classifications)))
        # must conform to MetaTBReport.cs
        # json_data.setdefault('Metrics', []).append(
        #    {
        #    "Name": "Classification_counts",
        # "ID": "a9986fb8-b319-4f71-ba57-bcxx",
        # "DisplayedName": null,
        # "Unit": "",
        #    "Value": json.dumps(counts)
        #    })
        json_data.setdefault('Metrics', [])
        for item in json_data['Metrics']:
            if isinstance(item, dict) and item.get("ID") == "d408d8c0-fcd0-426a-be4a-c60d1ad9ff4f":
                json_data['Metrics'].remove(item)
        human_models = 0
        for category, count in six.iteritems(counts):
            if category.startswith("AVM.Ontology.") and category.endswith(".Category.Human_Models"):
                human_models += count
        # META-554
        human_models = human_models - 3
        if human_models < 0:
            human_models = 0
        json_data['Metrics'].append(
            {
                "Name": "Troop_Capacity",
                "DisplayedName": "Troop_Capacity",
                "Value": human_models,
                "ID": "d408d8c0-fcd0-426a-be4a-c60d1ad9ff4f",
                "Unit": ""
            })
        with open(json_filename, "w") as json_file:
            json.dump(json_data, json_file, indent=4)

        stat_json_path = os.path.join(output_dir, 'stat.json')
        if os.path.isfile(stat_json_path):
            with open(stat_json_path, 'r') as stat_file:
                stat_json = json.load(stat_file)
        else:
            stat_json = {}
        stat_json.setdefault('ToolSpecifics', {})
        stat_json['ToolSpecifics']['classifications'] = counts
        with open(stat_json_path, 'w') as stat_file:
            json.dump(stat_json, stat_file, indent=4)


# This is the entry point
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    log(rootObject.name)
    print(repr(rootObject.name))
    output_dir = componentParameters['output_dir']
    if output_dir == '':
        output_dir = os.getcwd()
    ComputeClassificationCounts().compute(focusObject, output_dir)
    #log_formatted("Output files are <a href=\"file:///{0}\" target=\"_blank\">{0}</a>.".format(output_dir))
    log("Output files are <a href=\"file:///{0}\" target=\"_blank\">{0}</a>.".format(output_dir))
    log('done')
    with open(os.path.join(output_dir, 'run_classifications.cmd'), 'w'):
        pass
    componentParameters['runCommand'] = 'run_classifications.cmd'
    #componentParameters['labels'] = 'Windows13.16'


# Allow calling this script with a .mga file as an argument
if __name__ == '__main__':
    import six.moves.winreg as winreg
    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
        meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")

    # need to open meta DN since it isn't compiled in
    uml_diagram = udm.uml_diagram()
    meta_dn = udm.SmartDataNetwork(uml_diagram)
    import os.path
    CyPhyML_udm = os.path.join(meta_path, r"generated\CyPhyML\models\CyPhyML_udm.xml")
    if not os.path.isfile(CyPhyML_udm):
        CyPhyML_udm = os.path.join(meta_path, r"meta\CyPhyML_udm.xml")
    meta_dn.open(CyPhyML_udm, "")

    dn = udm.SmartDataNetwork(meta_dn.root)
    dn.open(sys.argv[1], "")
    # TODO: what should focusObject be
    # invoke(None, dn.root);
    dn.close_no_update()
    meta_dn.close_no_update()
