# -*- Mode: Python -*-
#
# This file calls PARC_FAME_Toolkit and determine possible fault modes
# that exists in CyPhy Driveline Model.


import sys, os, traceback, json, shutil
from collections import OrderedDict
import fetch

script_dir = os.path.dirname(os.path.realpath(__file__))
output_dir = os.path.abspath(os.path.join(script_dir,"../"))
cyphy_model_dir = os.path.abspath(os.path.join(script_dir,"../CyPhy"))

# function to put the result into json format  
def output(faultCount,output_dir):

    # creating ordered dictionary to be outputted in testbench.json format
    data = OrderedDict()
    data["$id"] = "1"
    data["Name"] = "FAME_Possible_Faults"
    
    MetricDict = OrderedDict()
    MetricDict["$id"] = "2"
    #setting arbitruary number as default requirement value
    MetricDict["Requirement"] = "1000"   
    MetricDict["Name"] = "Possible_Faults"
    MetricDict["Unit"] = "count"
    MetricDict["Value"] = faultCount
    data["Metric"] = [MetricDict]
    
    with open(os.path.join(script_dir,'FAME_Possible_Faults.testbench.json'),'w') as outfile:
        json.dump(data,outfile, indent=2,sort_keys=False)


# quick bug fix for space in modelica folder name
# this is stripping the version number from Modelica library (if version is separated by space).

def set_library_dir():
    library_dir = os.path.join(script_dir,'../Libraries/')
    if os.path.exists(library_dir):
        for foldername in os.listdir(library_dir):
            try:
                if foldername.split(" ")>1:
                    os.rename(os.path.join(library_dir,foldername),os.path.join(library_dir,foldername.split()[0]))
            except WindowsError:
                shutil.rmtree(os.path.join(library_dir,foldername.split()[0]))
                os.rename(os.path.join(library_dir,foldername),os.path.join(library_dir,foldername.split()[0]))
    else:
        outfile = open("_FAILED.txt","w")
        outfile.write("Missing Modelica Library which should be in ../Libraries\n")
        outfile.close()
        sys.exit()       
    return library_dir


def get_fame_toolbox_modelica_libraries():
    flag = 1
    # check if any critical library is missing
    if (os.path.isdir(os.path.join(script_dir,"FAME")) and 
            os.path.isdir(os.path.join(script_dir,"MSL")) and
            os.path.isdir(os.path.join(script_dir,"pre-faulted"))):
        flag = 0
    if flag == 1:
        # going redownload whole set of key libraries
        if os.path.exists(os.path.join(script_dir,"FAME")):
            shutil.rmtree(os.path.join(script_dir,"FAME"))
        if os.path.exists(os.path.join(script_dir,"MSL")):
            shutil.rmtree(os.path.join(script_dir,"MSL"))
        if os.path.exists(os.path.join(script_dir,"pre-faulted")):
            shutil.rmtree(os.path.join(script_dir,"pre-faulted"))
        fetch.fetch_and_unpack_zip_file("http://fame-deploy.parc.com/C2M2L_Decl/fault-enabled-libraries/FAME_Toolkit_Modelica_Files.zip", script_dir)
        shutil.move(os.path.join(script_dir,"FAME_Toolkit_Modelica_Files","FAME"),
                os.path.join(script_dir,"FAME"))
        shutil.move(os.path.join(script_dir,"FAME_Toolkit_Modelica_Files","MSL"),
                os.path.join(script_dir,"MSL"))
        shutil.move(os.path.join(script_dir,"FAME_Toolkit_Modelica_Files","pre-faulted"),
                os.path.join(script_dir,"pre-faulted"))
        shutil.rmtree(os.path.join(script_dir,"FAME_Toolkit_Modelica_Files"))
    

def get_testbench_name():
    model_raw_data = open(os.path.join(cyphy_model_dir,"model_config.json"))
    model_json = json.load(model_raw_data)
    return model_json["model_name"]
    

try:
    import PARC_FAME_Toolkit
    library_dir = set_library_dir()

    # finding testbench name from json file that CyPhy created.
    testbench_name = get_testbench_name()
    
    # finding all the necessary model library to run testbench
    """
    # This approach failed because Postprocessing folder, which is not Modelica package
    # was in CyPhy folder.
    model_libraries = [cyphy_model_dir]
    for directory in os.listdir(cyphy_model_dir):
        if os.path.isdir(os.path.join(cyphy_model_dir,directory)):
            model_libraries.append(os.path.abspath(os.path.join(cyphy_model_dir,directory)))
    """
            
    model_libraries = [os.path.abspath(os.path.join(script_dir,"../CyPhy/"))]
    for directory in os.listdir(library_dir):
        if os.path.isdir(os.path.join(library_dir,directory)):
            model_libraries.append(os.path.abspath(os.path.join(library_dir,directory)))
    print(model_libraries)

    get_fame_toolbox_modelica_libraries()
    
    results = PARC_FAME_Toolkit.fault_analyze_testbench(
        testbench_name, model_libraries)
        
except:
    sys.stderr.write("Can't list faults:\n%s\n" % traceback.format_exc())
    # more complicated error handling can be added here, if desired
else:
    # now render it as JSON
    with open(os.path.join(script_dir,'possibleFault.json'),'w') as outfile:
        jsondata = json.dumps(results, indent=4)
        outfile.write(jsondata)
    faultCnt = 0
    for i in range(len(results)):
        try:
            faultCnt = faultCnt + len(results[i]["modes"])
        except:
            pass
    output(faultCnt,output_dir)
    if faultCnt > 0:
        keyfilename = "testbench_manifest.json"
        keyfile = os.path.join(output_dir,keyfilename)
        with open(keyfile,"r") as infile:
            jsondata = json.load(infile, object_pairs_hook=OrderedDict)
        for i in range(len(jsondata["Metrics"])):
            if jsondata["Metrics"][i]["Name"] == "NumPossFaults":
                jsondata["Metrics"][i]["Value"] = str(faultCnt)
                jsondata["Status"] = "EXECUTED"
            if jsondata["Metrics"][i]["Name"] == "NumPossibleFaults":
                jsondata["Metrics"][i]["Value"] = str(faultCnt)
                jsondata["Status"] = "EXECUTED"
            if jsondata["Metrics"][i]["Name"] == "Number_Faults":
                jsondata["Metrics"][i]["Value"] = str(faultCnt)
                jsondata["Status"] = "EXECUTED"
        with open(keyfile,"w") as outfile:
            json.dump(jsondata,outfile, indent=4)
        
    
