import os
import glob
from xml.etree.ElementTree import ElementTree

def parse_xml(xml_filename):
    results = []
    
    if not os.path.exists(xml_filename):
        print 'XML file does not exist'
        return result
    
    
    mydoc = ElementTree(file=xml_filename)
    
    if not mydoc.getroot().tag == 'AVMComponent':
        print '{0} is not AVMComponent instance'.format(xml_filename)
        return result
    
    mo_file_path = os.path.normpath(os.path.join('Instances', os.path.splitext(xml_filename)[0])).replace('.', '_')
    
    for e in mydoc.findall("./Feature/BehaviorModels/BehaviorModel[@ModelingTool='Dymola']"):
        result = dict()
        result['parameters'] = []
        result['parsed'] = False
        
        result['xml_file'] = xml_filename
        result['name'] = mydoc.getroot().get('Name')
        #result['avmid'] = mydoc.getroot().get('AVMID')
        
        # ASSUMPTION: only one behavioral model
        
        result['ModelicaType'] = e.get('Name')
        if not result['ModelicaType']:
            result['ModelicaType'] = e.get('name')
        
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
            
        
        
        for p in e.findall('./BehaviorParameter'):
            if p.get('Value'):
                result['parameters'].append({'name':p.get('Name'), 'value': p.get('Value')})
            else:
                # value is not defined at the modelica model level
                
                bm_param_id = p.get('id')
                # find association DstFeature
                associations = mydoc.findall("./Associations/Association[@DstFeature='" + bm_param_id +"']")
                if not associations:
                    print 'No association has found with DstFeature id: {0} for parameter {1}.'.format(bm_param_id, p.get('Name'))
                    continue
                
                association = associations[0]
                
                if len(associations) > 1:
                    print 'More than one association has found with DstFeature id: {0} for parameter {1}.'
                    print 'Using first one: {0}'.format(association.get('Note'))
                
                nv_id = association.get('SrcFeature')
                named_values = mydoc.findall("./Feature/NamedValues/NamedValue[@id='" + nv_id +"']")
                
                if not named_values:
                    print 'No named value has found with id: {0} for association {1}.'.format(nv_id, association.get('Note'))
                    continue
    
                # Using first value
                named_value = named_values[0]
                
                if named_value.get('Value'):
                    result['parameters'].append({'name':p.get('Name'), 'value': named_value.get('Value')})
    
        result['mo_file'] = mo_file_path + '.mo'
        result['model_name'] = os.path.basename(os.path.splitext(mo_file_path)[0])

        result['parsed'] = True
        
        results.append(result)
    
    return results

def spacedman_parts(path):
    components = [] 
    while True:
        (path,tail) = os.path.split(path)
        if tail == "":
            components.reverse()
            return components
        components.append(tail)
    
    
def make_package(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

    package_file = os.path.join(dir_name, 'package.mo')
    with open(package_file, 'w') as f:
        f.write('package {0}\n'.format(os.path.basename(dir_name)))
        f.write('  extends Modelica.Icons.Package;\n')
        f.write('  annotation (uses(Modelica(version="3.2")));')
        f.write('end {0};\n'.format(os.path.basename(dir_name)))
    
    print 'Created: {0}'.format(package_file)
    
def ensure_package_dir(mo_filename):
    dir_name = os.path.dirname(mo_filename)
    
    if os.path.isabs(mo_filename):
        raise Exception('Absolute path is not supported')
    
    dir_parts = spacedman_parts(dir_name)
    #print dir_parts
    for i in range(1,len(dir_parts) + 1):
        dir_part = os.sep.join(dir_parts[0:i])
        make_package(dir_part)
    
    
    
def make_instance(instance):
    if not instance['parsed']:
        return

    ensure_package_dir(instance['mo_file'])
    
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
        
        for parameter in instance['parameters']:
            f.write('    {0} = {1},\n'.format(parameter['name'], parameter['value']))
        
        f.write(');\n');
        f.write('  annotation (uses(Modelica(version="3.2")));\n')
        f.write('end {0};\n'.format(instance['model_name']))
    
    print 'Created: {0}'.format(instance['mo_file'])
    
    # TODO: save instances for debugging purposes
    #print instance

def generate_modelica_model(xml_filename):

    instances = parse_xml(xml_filename)
    
    for instance in instances:
        make_instance(instance)
    
    return instances

    
import fnmatch

filter = '*Water*.xml'
filter = '*.xml__.xml'

xml_files = []
for root, dirnames, filenames in os.walk('.'):
  for filename in fnmatch.filter(filenames, filter):
      xml_files.append(os.path.join(root, filename))

for xml in xml_files:
    generate_modelica_model(xml)
