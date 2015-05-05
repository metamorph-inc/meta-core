import sys, os, json

import PARC_QRM_Toolkit

this_file_path = os.path.dirname(os.path.abspath(__file__))
os.chdir(os.path.join(this_file_path, '..', 'CyPhy'))
print 'current dir {0}'.format(os.getcwd())

PARC_QRM_Toolkit.OMC = os.path.join(os.getenv("OPENMODELICAHOME"), 'bin', 'omc')
PARC_QRM_Toolkit.QRM = 'QRM'

if os.name == 'nt':
    # Windows
    import _winreg
    
    def query_analysis_tools():
        analysis_tools_key_name = r'SOFTWARE\Wow6432Node\META\AnalysisTools'
     
        analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                             _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY)
     
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
    
    os.environ['PATH'] += str(os.pathsep + os.path.join(tools['QR']['InstallLocation'], 'bin'))

model_config = json.load(open('model_config.json'))
for directory in model_config.get("lib_package_paths"):
    libraries = [os.getcwd()+'\\']
    for file in os.listdir(directory):
        if not file.startswith(".") and (file.endswith(".mo") or (os.path.isdir(file) and os.path.isdir(os.path.join(file, "package.mo")))):
            libraries.append(os.path.join(directory, file))

model_name = model_config.get('verification_model_name')
print libraries
jsonfile = PARC_QRM_Toolkit.do_envisionment_from_modelica(model_name, libraries,CyPhy_output=True)
json_filename = os.path.join("..", model_config.get("result_file")) + ".qrm.json"
open(json_filename, "w").write(open(jsonfile).read())
os.unlink(jsonfile)

# update summary.testresults.json file
qrm_results = {}
with open(json_filename, 'r') as f_p_qrm_results:
    qrm_results = json.load(f_p_qrm_results)

results_filename = os.path.join('..', 'testbench_manifest.json')
results = {}
with open(results_filename, 'r') as f_p_results:
    results = json.load(f_p_results)
    
results.update(qrm_results)
results['Status'] = 'OK'
with open(results_filename, 'w') as f_p_results:
    json.dump(results, f_p_results, indent=4)
    

