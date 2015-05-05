# console logger helper begins
from logging.handlers import RotatingFileHandler
import logging
import sys

class ConsoleHandler(logging.StreamHandler):
    """A handler that logs to console in the sensible way.

    StreamHandler can log to *one of* sys.stdout or sys.stderr.

    It is more sensible to log to sys.stdout by default with only error
    (logging.ERROR and above) messages going to sys.stderr. This is how
    ConsoleHandler behaves.
    """

    def __init__(self):
        logging.StreamHandler.__init__(self)
        self.stream = None # reset it; we are not going to use it anyway

    def emit(self, record):
        if record.levelno >= logging.ERROR:
            self.__emit(record, sys.stderr)
        else:
            self.__emit(record, sys.stdout)

    def __emit(self, record, strm):
        self.stream = strm
        logging.StreamHandler.emit(self, record)

    def flush(self):
        # Workaround a bug in logging module
        # See:
        #   http://bugs.python.org/issue6333
        if self.stream and hasattr(self.stream, 'flush') and not self.stream.closed:
            logging.StreamHandler.flush(self)

def instantiate_logger(log_file='debug.log'):
    """
    Creates a logger with two handlers.

    One RotatingFileHandler that writes to the log_file(='debug.log') at level DEBUG.

    One ConsoleHandler that writes to console at level INFO.

    """
    log = logging.getLogger()
    log.setLevel(logging.DEBUG)
    log_formatter = logging.Formatter("%(asctime)s %(levelname)s: %(message)s",
                                      datefmt="%Y-%m-%d %H:%M:%S")

    file_log = RotatingFileHandler(log_file)
    file_log.setFormatter(log_formatter)
    console_log = ConsoleHandler()
    console_log.setFormatter(log_formatter)
    console_log.setLevel(logging.INFO)
    log.addHandler(file_log)
    log.addHandler(console_log)

    return log

def flush_handlers():
    """
    Flushes the loggers to get console outprints during execution.
    """

    log = logging.getLogger()
    for handler in log.handlers:
        handler.flush()
        
# console logger helper ends
# ==============================================================================

import os
import fnmatch
from xml.etree.ElementTree import ElementTree
from optparse import OptionParser

class RicardoInstancesToModelica():
    XPATH_root_tag = 'AVMComponent'
    XPATH_Dymola = "./Feature/BehaviorModels/BehaviorModel[@ModelingTool='Dymola']"
    XPATH_behavioral_parameter = './BehaviorParameter'
    XPATH_named_value_by_id = "./Feature/NamedValues/NamedValue[@id='{0}']"
    XPATH_association_by_DstFeature = "./Associations/Association[@DstFeature='{0}']"
        
    binding_tool_version = "0.0.5"
    
    filter = '*.xml__.xml'
    
    parser = OptionParser()
    #parser.add_option("-v", "--version", action="store_true", default=False,
    #    help='Displays the version number of this package.')
    parser.add_option("-j", "--json", action="store_true", default=False,
        help='Stores json files for each instance.')
    
    def __init__(self, log = instantiate_logger(__file__ + '.log')):
        self.log = log
        
        (self.opts, self.args) = self.parser.parse_args()
        
        # TODO: get option settings dynamically
        if self.opts.json:
            self.log.info('Option: Store json files.')
        
        #self.filter = '*Water*.xml'
        self.xml_files = []
        for root, dirnames, filenames in os.walk('.'):
          for filename in fnmatch.filter(filenames, self.filter):
              self.xml_files.append(os.path.join(root, filename))
        
        self.log.info('Discovered xml files: {0}'.format(self.xml_files))
        
    def generate(self):
        
        all = len(self.xml_files)
        success = 0
        failure = 0
        id = 0
        
        self.log.info('Processing {0} xml files...'.format(all))
        
        for xml in self.xml_files:
            id = id + 1
            self.log.info('Processing {0}/{1} xml file...'.format(id, all))
            instances = self.generate_modelica_model(xml)
            for instance in instances:
                if instance['parsed']:
                    success = success + 1
                else:
                    failure = failure + 1
            
        self.log.info('Generator has finished with {0} success {1} failure out of {2} (instances) from {3} xml files'.format(success, failure, success + failure, all))
        
    def parse_xml(self, xml_filename):
        results = []
        
        if not os.path.exists(xml_filename):
            self.log.error('XML file does not exist')
            return results
        
        
        mydoc = ElementTree(file=xml_filename)
        
        # xml root tag check
        if not mydoc.getroot().tag == self.XPATH_root_tag:
            self.log.error('{0} is not {1} instance, it is {2}'.format(xml_filename, self.XPATH_root_tag, mydoc.getroot().tag))
            return results
        
        # version check
        if not mydoc.getroot().get('binding_tool_version') == self.binding_tool_version:
            self.log.error('{0} is using {1} version, only {2} supported.'.format(xml_filename, mydoc.getroot().get('binding_tool_version'), self.binding_tool_version))
            return results
        
        for e in mydoc.findall(self.XPATH_Dymola):
            result = dict()
            result['parameters'] = []
            result['parsed'] = False
            
            result['xml_file'] = xml_filename
            result['name'] = mydoc.getroot().get('Name')
            #result['avmid'] = mydoc.getroot().get('AVMID')
            
            
            
            result['ModelicaType'] = e.get('Name')
            if not result['ModelicaType']:
                result['ModelicaType'] = e.get('name')
            
            result['package_name'] = 'Instances'
            # TODO: error handling for [0]
            result['package_name'] = result['ModelicaType'].split('.')[0] + '_' + result['package_name']
            
            mo_file_path = os.path.normpath(os.path.join(result['package_name'], os.path.splitext(xml_filename)[0])).replace('.', '_')
            
            result['phenomenon'] = []
            
            phenomenon = e.findall('./Phenomena')
            #print phenomenon
            if phenomenon:
                phenomena_suffix = ''
                for phenomena in phenomenon:
                    phenomena_suffix += '_'
                    phenomena_suffix += phenomena.get('name')
                    
                    result['phenomenon'].append(phenomena.get('name'))
                    
                mo_file_path = mo_file_path + phenomena_suffix
                
            
            
            for p in e.findall(self.XPATH_behavioral_parameter):
                if p.get('Value'):
                    result['parameters'].append({'name':p.get('Name'), 'value': p.get('Value')})
                else:
                    # value is not defined at the modelica model level
                    
                    bm_param_id = p.get('id')
                    # find association DstFeature
                    associations = mydoc.findall(self.XPATH_association_by_DstFeature.format(bm_param_id))
                    if not associations:
                        self.log.warning('No association has found with DstFeature id: {0} for parameter {1}.'.format(bm_param_id, p.get('Name')))
                        continue
                    
                    association = associations[0]
                    
                    if len(associations) > 1:
                        log.warning('More than one association has found with DstFeature id: {0} for parameter {1}.')
                        log.warning('Using first one: {0}'.format(association.get('Note')))
                    
                    nv_id = association.get('SrcFeature')
                    named_values = mydoc.findall(self.XPATH_named_value_by_id.format(nv_id))
                    
                    if not named_values:
                        self.log.warning('No named value has found with id: {0} for association {1}.'.format(nv_id, association.get('Note')))
                        continue
        
                    # Using first value
                    named_value = named_values[0]
                    
                    if named_value.get('Value'):
                        result['parameters'].append({'name':p.get('Name'), 'value': named_value.get('Value')})
                    else:
                        self.log.warning('No other fallback option to get the value of {0}'.format(p.get('Name')))
                    
        
            result['json_file'] = mo_file_path + '.json'
            result['mo_file'] = mo_file_path + '.mo'
            result['modelica_url'] = mo_file_path.replace('\\','.')
            result['model_name'] = os.path.basename(os.path.splitext(mo_file_path)[0])
            result['parsed'] = True
            result['instance_name'] = os.path.splitext(os.path.basename(result['xml_file']))[0]
            
            results.append(result)
        
        return results

    def _spacedman_parts(self, path):
        components = [] 
        while True:
            (path,tail) = os.path.split(path)
            if tail == "":
                components.reverse()
                return components
            components.append(tail)
        
    def _make_package(self, dir_name):
        if not os.path.exists(dir_name):
            os.makedirs(dir_name)

        package_file = os.path.join(dir_name, 'package.mo')
        with open(package_file, 'w') as f:
            f.write('package {0}\n'.format(os.path.basename(dir_name)))
            f.write('  extends Modelica.Icons.Package;\n')
            f.write('  annotation (uses(Modelica(version="3.2")));')
            f.write('end {0};\n'.format(os.path.basename(dir_name)))
        
        self.log.info('Created: {0}'.format(package_file))
        
    def _ensure_package_dir(self, mo_filename):
        dir_name = os.path.dirname(mo_filename)
        
        if os.path.isabs(mo_filename):
            self.log.critical('Absolute path is not supported')
            return
        
        dir_parts = self._spacedman_parts(dir_name)
        #print dir_parts
        for i in range(1,len(dir_parts) + 1):
            dir_part = os.sep.join(dir_parts[0:i])
            self._make_package(dir_part)
        
    def _make_instance(self, instance):
        if not instance['parsed']:
            log.warning('Instance is not parsed: {0}'.format(instance))
            return False
            
        self._ensure_package_dir(instance['mo_file'])
        
        with open(instance['mo_file'], 'w') as f:
            phenomenon = ''
            if instance['phenomenon']:
                phenomenon = 'Phenomenon: '
                for phenomena in instance['phenomenon']:
                    phenomenon += phenomena
                    phenomenon += ','
                    
            comment = '{0} instance of {1} {2}'.format(instance['xml_file'], instance['name'], phenomenon)
            f.write('model {0} "{1}"\n'.format(instance['model_name'], comment))
            f.write('  extends {0}(\n'.format(instance['ModelicaType']))
            
            i = len(instance['parameters'])
            
            for parameter in instance['parameters']:
                i = i - 1
                f.write('    {0} = {1}'.format(parameter['name'], parameter['value']))
                if i > 0:
                    f.write(',')
                    
                f.write('\n')
            
            f.write(');\n');
            f.write('  annotation (uses(Modelica(version="3.2")));\n')
            f.write('end {0};\n'.format(instance['model_name']))
        
        self.log.info('Created: {0}'.format(instance['mo_file']))
        
        if self.opts.json:
            with open(instance['json_file'], 'w') as f:
                import json
                json.dump(instance,f)
                self.log.info('Created: {0}'.format(instance['json_file']))
        return True
        
    def generate_modelica_model(self, xml_filename):
        instances = self.parse_xml(xml_filename)
        for instance in instances:
            self._make_instance(instance)
        
        return instances


def main():
    ri2m = RicardoInstancesToModelica()
    ri2m.generate()
    
if __name__ == '__main__':
    main()
