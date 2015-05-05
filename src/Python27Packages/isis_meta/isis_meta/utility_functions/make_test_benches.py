# console logger helper begins
from logging.handlers import RotatingFileHandler
import logging
import sys

import update_class_list_with_replacables as update

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
import json
from optparse import OptionParser

class MakeTestBenches():
    
    filter = '*xml__.json'
    
    parser = OptionParser()
    #parser.add_option("-v", "--version", action="store_true", default=False,
    #    help='Displays the version number of this package.')
    #parser.add_option("-j", "--json", action="store_true", default=False,
    #    help='Stores json files for each instance.')
    
    instances = []
    
    def __init__(self, test_benches, log = instantiate_logger(__file__ + '.log')):
        self.log = log
        
        (self.opts, self.args) = self.parser.parse_args()
        
        # TODO: get option settings dynamically
        #if self.opts.json:
        #    self.log.info('Option: Store json files.')
        
        self.instance_files = []
        for root, dirnames, filenames in os.walk('.'):
          for filename in fnmatch.filter(filenames, self.filter):
              self.instance_files.append(os.path.join(root, filename))

        for instance_file in self.instance_files:
            self.log.info('Loading {0}'.format(instance_file))
            with open(instance_file, 'r') as f:
                self.instances.append(json.load(f))
                
        self.config_filename = __file__ + '.config.json'
        
        if not os.path.exists(self.config_filename):
            self.log.error('Config file does not exist. {0}'.format(self.config_filename))
            
            self.log.info('Generating default config...')
            with open(self.config_filename, 'w') as f:
                json.dump(self.sample_config(), f, indent=4)
            self.log.info('Default config is here: {0}'.format(self.config_filename))
        
        with open(self.config_filename, 'r') as f:
            self.config = json.load(f)
            self.log.info('Config loaded from: {0}'.format(self.config_filename))
        
        self.config = test_benches
        
    def sample_config(self):
        config = {
            'C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Drive_Line.Drive_Shafts.Test_Bench_Drive_Shaft' : {
                'parameters': [
                    {
                        'name':'drive_Shaft',
                        'constraint_clause':'C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Drive_Line.Drive_Shafts.Drive_Shaft',
                        'default_instance':'Drive_Shaft_param_length_2.xml__.xml'
                    }
                ]
            }
        }
        
        return config
        
    def generate(self):
        self.log.info('Generating test benches...')
        print self.config
        for c in self.config.keys():
            self.config[c].update({'base_model' : c})
            
            if len(self.config[c]['parameters']) == 1:
                # if only one parameters generate all test benches
                self._generate_unit_test_benches(self.config[c])
            elif len(self.config[c]['parameters']) > 1:
                # if more parameters generate only one test bench
                self._generate_single_test_bench(self.config[c])
            else:
                self.log.warning('{0} has no parameters in config file'.format(c))
            pass
        
    def _get_instance_by_name(self, instance_name):
        for instance in self.instances:
            if instance['instance_name'] == instance_name:
                return instance

    def _get_instances_by_base_type(self, base_type):
        for instance in self.instances:
            if instance['ModelicaType'] == base_type:
                yield instance
                
    def _generate_unit_test_benches(self, config):
        #modelicaurl + base model name
        #print config
        instances = self._get_instances_by_base_type(config['parameters'][0]['constraint_clause'])
        for instance in instances:
            model_name = instance['modelica_url'] + config['base_model'].split('.')[-1]
            self._generate_model(model_name, config, instance)

        
        
        
    def _generate_single_test_bench(self, config):
        model_name = config['base_model'].split('.')[0] + '_SystemTests.' + '.'.join(config['base_model'].split('.')[1:])
        self._generate_model(model_name, config)
        
        
    def _generate_model(self, model_name, config, instance=None):
        mo_file = model_name.replace('.','\\') + '.mo'
        self._ensure_package_dir(mo_file)        
        
        with open(mo_file, 'w') as f:
            if instance:
                comment = 'Test bench for {0} instance'.format(instance['xml_file'])
            else:
                comment = 'System level test bench {0}'.format(model_name.split('.')[-1])
            f.write('model {0} "{1}"\n'.format(model_name.split('.')[-1], comment))
            f.write('  extends {0}(\n'.format(config['base_model']))
            
            id = len(config['parameters'])
            
            for parameter in config['parameters']:
                id = id - 1
                if instance:
                    f.write('    redeclare {0} {1}'.format(instance['modelica_url'], parameter['name']))
                else:
                    inst = self._get_instance_by_name(os.path.splitext(parameter['default_instance'])[0])
                    if not inst:
                        self.log.warning('{0} was not found.'.format(parameter['default_instance']))
                        continue
                    f.write('    redeclare {0} {1}'.format(inst['modelica_url'], parameter['name']))
                    
                if id > 0:
                    f.write(',')
                f.write('\n')
                
            f.write(');\n');
            f.write('  annotation (uses(Modelica(version="3.2")));\n')
            f.write('end {0};\n'.format(model_name.split('.')[-1]))
                    
        self.log.info('Created: {0}'.format(mo_file))
        pass
        
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
            
def main():
    
    tb = update.main()
    
    mtb = MakeTestBenches(tb)
    mtb.generate()
    
if __name__ == '__main__':
    main()