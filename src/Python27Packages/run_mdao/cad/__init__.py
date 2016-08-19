"""
Updates the CADAssembly.xml with parameters from testbench_manifest.json.

Functions in this file are always called from the TestBench directory. They must
return from that directory (if there is an exception it does not matter).
"""
import logging
import xml.etree.ElementTree as ET

# Maximum time in seconds to wait for each execution.
MAX_WAIT_TIME = 3600 * 2


class TestBenchParameter_to_CadAssembly(object):

    """Updates the CADAssembly.xml with parameters from testbench_manifest.json."""

    # hard-coded names
    local_cadassembly_name = 'CADAssembly.xml'
    modified_cadassembly_name = 'CADAssembly.xml'

    testbench_manifest_json = 'testbench_manifest.json'
    testbench_cadparam_json = 'CADParamTestBenchMapping.json'

    def __init__(self):
        self.logger = logging.getLogger()
        self.tbmanifest_param_list = []

    def _parse_json(self, filename):
        import json
        with open(filename, 'r') as f_in:
            return json.load(f_in)

    def populate_cadparam_values(self):
        instanceguid_param_dict = {}
        tbmanifest_dict = self._parse_json(self.testbench_manifest_json)
        if 'Parameters' in tbmanifest_dict:
            self.tbmanifest_param_list = tbmanifest_dict['Parameters']
            self.logger.info(self.tbmanifest_param_list)
        cadparam_mapping_list = self._parse_json(self.testbench_cadparam_json)
        for cadparam in cadparam_mapping_list:
            if 'TestBenchParameterName' in cadparam:
                value = self._find_testbench_param_value(cadparam['TestBenchParameterName'])
                if value is not None:
                    if cadparam['ComponentInstanceGUID'] not in instanceguid_param_dict:
                        instanceguid_param_dict[cadparam['ComponentInstanceGUID']] = {}

                    instanceguid_param_dict[cadparam['ComponentInstanceGUID']][cadparam['ComponentCADParameterName']] = value
                    self.logger.info('instanceguid_param_dict : {0}'.format(instanceguid_param_dict))

        return instanceguid_param_dict

    def _find_testbench_param_value(self, param_name):
        value = None
        for param in self.tbmanifest_param_list:
            if param['Name'] == param_name:
                value = param['Value']

        return value

    def modify_cad_assembly_file(self, statusfile):
        tree = ET.parse(self.local_cadassembly_name)

        instanceguid_param_dict = self.populate_cadparam_values()
        # find Component under <Assemblies><Assembly>
        for ass_elem in tree.iter(tag='Assembly'):
            for cc_parent in ass_elem.findall('CADComponent'):
                self.logger.debug('Found a parent component in CADAssembly.xml [' + str(cc_parent.attrib['DisplayName']) + ']')
                for cc in cc_parent.findall('CADComponent'):
                    self.logger.debug('Found a component in CADAssembly.xml [' + str(cc.attrib['DisplayName']) + ']')
                    if cc.attrib['ComponentID'] in instanceguid_param_dict:
                        guid = cc.attrib['ComponentID']
                        cadparam_dict_src = instanceguid_param_dict[guid]
                        # recurse Found Element Component to find CADParameters
                        for pp in cc.findall('ParametricParameters'):
                            for cp in pp.findall('CADParameter'):
                                if cp.get('Name') in cadparam_dict_src:
                                    cp.set('Value', str(cadparam_dict_src[cp.get('Name')]))
                                    statusfile.write('Replaced in XML [%s:%s:%s]\n' % (guid, cp.get('Name'), str(cadparam_dict_src[cp.get('Name')])))

            tree.write(self.modified_cadassembly_name)
            self.logger.debug('end')
