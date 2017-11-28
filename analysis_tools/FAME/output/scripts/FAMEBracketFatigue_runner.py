# -*- Mode: Python -*-
#
# This is an example of a 'driver file' for the PARC_FAMEBracketFatique analysis
#

import sys, os, json

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
    
    fame_cyphy_directory = os.path.join(tools['FAMEBracketFatigue']['InstallLocation'], 'BracketFatigue')

print fame_cyphy_directory

# TODO: check if directory exists
# add fame_cyphy_dir to python search path
sys.path.append(fame_cyphy_directory)

# TODO: check if import fails
# import the analysis tool
import AnalyzeBracketFatigue

# TODO: check if analysis fails
# run the analysis tool
AnalyzeBracketFatigue.main()

if os.path.exists('testbench_manifest.json'):
    bracket_fatique_results = None
    # TODO: check if result file exists
    # read in result file
    with open('Bracket_Fatigue.testbench.json', 'r') as f_p:
        bracket_fatique_results = json.load(f_p)

    # TODO: check data structure
    # get metric value
    value = bracket_fatique_results['Metric'][0]['Value']


    # update metric in testbench_manifest.json
    with open('testbench_manifest.json', 'r') as f_p:
        testbench_manifest = json.load(f_p)
    # TODO: check if metrics exist with the right name
    testbench_manifest['Metrics'][0]['Value'] = value
    
    # update manifest file
    with open('testbench_manifest.json', 'w') as f_p:
        json.dump(testbench_manifest,f_p)


