#==========================================================================================
#   ISIS, Vanderbilt University
#   Name: PETUpdateCADParameter.py
#   Purpose: This script uses testbench_manifest.json and CADParamTestBenchMapping.json
#            to update the CADParameter values in a CADAssembly.xml for PET test benches.
#==========================================================================================

import os
import glob
import zipfile
import datetime 
import json
import sys
import argparse

class Layout_to_CadAssembly(object):
    """
    Class definition

    """
    # hard-coded names
    local_cadassembly_name = 'CADAssembly.xml'
    modified_cadassembly_name = 'CADAssembly.xml'

    testbench_manifest_json = 'testbench_manifest.json'
    testbench_cadparam_json = 'CADParamTestBenchMapping.json'
    layout_json_name = ''

    # dictionaries
    layout_json_dict = {}

    tbmanifest_param_list = []

    def __init__(self):
        """
        Constructor

        """
        # paths
        project_root = ''
        path_to_stats_folder = ''
        #self.project_root = os.path.abspath(os.pardir)
        #self.path_to_stats_folder = os.path.join(self.project_root, 'stats')
        #self.right_now = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')

    def parse_json(self, file):
        parsed_json_dict = {}
        with open(file, 'r') as f_in:
            parsed_json_dict = json.load(f_in)
        return parsed_json_dict
            
    def populate_cadparam_values(self):
        instanceguid_param_dict = {}

        tbmanifest_dict = self.parse_json(self.testbench_manifest_json)
        if 'Parameters' in tbmanifest_dict:
            self.tbmanifest_param_list = tbmanifest_dict['Parameters']
            
        if not os.path.isfile(self.testbench_cadparam_json):
            # There are no CAD parameters, even though there are TestBench parameters, so move along.
            pass
        else:
            # Map the TestBench parameters to CAD parameters
            cadparam_mapping_list = self.parse_json(self.testbench_cadparam_json)
            for cadparam in cadparam_mapping_list:
                if 'TestBenchParameterName' in cadparam:
                    value = self.find_testbench_param_value(cadparam['TestBenchParameterName'])
                    if value is not None:
                        if cadparam['ComponentInstanceGUID'] not in instanceguid_param_dict:
                            instanceguid_param_dict[cadparam['ComponentInstanceGUID']] = {}

                        instanceguid_param_dict[cadparam['ComponentInstanceGUID']][cadparam['CADParameterName']] = value
                        print instanceguid_param_dict

        return instanceguid_param_dict
                
    def find_testbench_param_value(self, param_name):
        value = None
        for param in self.tbmanifest_param_list:
            if param['Name'] == param_name:
                value = param['Value']
        return value

    def modify_cad_assembly_file(self):
        import xml.etree.ElementTree as ET
        tree = ET.parse(self.local_cadassembly_name)
        root = tree.getroot()

        instanceguid_param_dict = self.populate_cadparam_values()
        # find Component under <Assemblies><Assembly>
        for ass_elem in tree.iter(tag='Assembly'):
            for cc_parent in ass_elem.findall('CADComponent'):
                #print 'Found a parent component in CADAssembly.xml [' + str(cc_parent.attrib['DisplayName']) + ']'
                for cc in cc_parent.findall('CADComponent'):
                    #print 'Found a component in CADAssembly.xml [' + str(cc.attrib['DisplayName']) + ']'
                    if cc.attrib['ComponentID'] in instanceguid_param_dict:
                        cadparam_dict_src = instanceguid_param_dict[cc.attrib['ComponentID']]
                        # recurse Found Element Component to find CADParameters
                        for pp in cc.findall('ParametricParameters'):
                            for cp in pp.findall('CADParameter'):
                                if cp.get('Name') in cadparam_dict_src:
                                    cp.set('Value', str(cadparam_dict_src[cp.get('Name')]))
       
            tree.write(self.modified_cadassembly_name)
            print 'end'        
            
      
def main(argv=sys.argv):
    ltc = Layout_to_CadAssembly()

    ## copy CADAssembly.xml file to working results directory for debug
    ltc.modify_cad_assembly_file()

    return 0

if __name__ == '__main__':
    sys.exit(main())
    
