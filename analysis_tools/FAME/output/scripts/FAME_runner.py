# -*- Mode: Python -*-
#
# This is an example of a 'driver file' for the PARC_FAME_Toolkit
#

import sys, os, traceback, json
import PARC_FAME_Toolkit

if os.name == 'nt':
    # Windows
    import _winreg
    
    def query_analysis_tools():
        analysis_tools_key_name = r'SOFTWARE\META\AnalysisTools'
     
        analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                             _winreg.KEY_READ)
     
        number_of_keys =  _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys
     
        info = {}
        for sub_key_id in range(0, number_of_keys):
            sub_key_name = _winreg.EnumKey(analysis_tools_key, sub_key_id)
            sub_key = _winreg.OpenKey(analysis_tools_key, sub_key_name)
            info[sub_key_name] = {}
            number_of_values = _winreg.QueryInfoKey(sub_key)[1]
            for value_id in range(0, number_of_values):
                value_tuple = _winreg.EnumValue(sub_key, value_id)
                value_name = value_tuple[0]
                value = value_tuple[1]
                info[sub_key_name][value_name] = value
     
        return info
   
    tools = query_analysis_tools()
    # add QR.exe location to the path
    
    fame_cyphy_directory = os.path.join(tools['FAME']['InstallLocation']
    #os.environ['PATH'] += str(os.pathsep + os.path.join(tools['FAME']['InstallLocation'], 'bin'))

# TODO: model_config.json file
#this_file_path = os.path.dirname(os.path.abspath(__file__))
#cwd = os.getcwd()
#os.chdir(os.path.join(this_file_path, '..', 'CyPhy'))
#print 'current dir {0}'.format(os.getcwd())
#with open('model_config.json') as f_p:
#    model_config = json.load(f_p)
#for directory in model_config.get("lib_package_paths"):
#    libraries = [os.getcwd()+'\\']
#    for file in os.listdir(directory):
#        if not file.startswith(".") and (file.endswith(".mo") or (os.path.isdir(file) and os.path.isdir(os.path.join(file, "package.mo")))):
#            libraries.append(os.path.join(directory, file))

#model_name = model_config.get('model_name')
#print libraries
try:
    model_libraries = [os.path.abspath("/local/fame/v4x0lpvw/CyPhy"),
                       os.path.abspath("/local/fame/v4x0lpvw/Libraries/ModifiedMass")]
    results = PARC_FAME_Toolkit.fault_analyze_testbench(
        "CyPhy.TestBenches.MassSpringDamper_cfg2", model_libraries)
except:
    sys.stderr.write("Can't list faults:\n%s\n" % traceback.format_exc())
    # more complicated error handling can be added here, if desired
else:
    # now render it as JSON
    print json.dumps(results, indent=4)
