__author__ = 'imatei,thonda'

import subprocess
import time, signal
import os, sys, shutil
import numpy as np
import json
from collections import OrderedDict
import fetch

script_dir = os.path.dirname(os.path.realpath(__file__))
#script_dir = os.getcwd()
output_dir = os.path.abspath(os.path.join(script_dir,"../"))
library_dir = os.path.realpath(os.path.join(script_dir,"..\Libraries"))
aug_library_dir = os.path.realpath(os.path.join(script_dir,"AugmentedLibraries"))
if not os.path.exists(aug_library_dir):
    aug_library_dir = os.path.realpath(os.path.join(script_dir,"..\AugmentedLibraries"))
    if not os.path.exists(aug_library_dir):
        try:
            fetch.fetch_and_unpack_zip_file("http://fame-deploy.parc.com/C2M2L_Decl/fault-enabled-libraries/AugmentedLibraries.zip", output_dir)
        except:
            outfile = open(os.join(output_dir,"_FAILED.txt"),"w")
            outfile.write("Missing Augmented Library.\n")
            outfile.write("Please download and put in ..\AugmentedLibraries folder \n")
            outfile.close()
            sys.exit()

cyphy_model_dir = os.path.realpath(os.path.join(script_dir,"..\CyPhy"))

fault_sim_rslt_dir = os.path.realpath(os.path.join(script_dir,"fault_sim_rslt"))
shutil.rmtree(fault_sim_rslt_dir,ignore_errors=True)
os.makedirs(fault_sim_rslt_dir)

# using CyPhy's Driveline postprocessing module.

sys.path.insert(0,os.path.join(cyphy_model_dir,'PostProcessing'))

for filename in os.listdir(os.path.join(cyphy_model_dir,'PostProcessing')):
    if filename.endswith(".py") and not filename.startswith("__"):
        Metrics = __import__(filename[:-3],globals())

from common import PostProcess

def createScript(component,fault,faultIndex,amount):

    script_name = 'fault_'+ str(faultIndex)+'.mos'

    file = open(os.path.join(fault_sim_rslt_dir,script_name),'w')
    folderlist = os.listdir(aug_library_dir)
    if os.path.exists(os.path.join(aug_library_dir,"Modelica",'package.mo')):
        file.write('openModel("'+os.path.join(aug_library_dir,"Modelica",'package.mo')+'");\n')
        folderlist.remove("Modelica")
    for foldername in folderlist:
        if os.path.exists(os.path.join(aug_library_dir,foldername,'package.mo')):
            file.write('openModel("'+os.path.join(aug_library_dir,foldername,'package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(aug_library_dir,'Modelica\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(aug_library_dir,'FAME\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(aug_library_dir,'C2M2L_Decl\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(aug_library_dir,'C2M2L_CyPhy\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(library_dir,'Modelica_FAME\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(library_dir,'C2M2L_CyPhy_FAME\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(library_dir,'C2M2L_Decl_FAME\package.mo')+'");\n')
    #file.write('openModel("'+os.path.join(library_dir,'ModelicaServices_FAME\package.mo')+'");\n')
    file.write('openModel("'+os.path.join(cyphy_model_dir,'package.mo')+'");\n')

    file.write('cd("'+fault_sim_rslt_dir.replace("\\","/")+'");\n')
    file.write('Advanced.TranslationInCommandLog := true;\n')
    file.write('Advanced.CheckPackageRestriction:=false;\n')
    file.write('Advanced.NewInitializationOfPre:= false;\n')
    file.write('experimentSetupOutput();\n')

    
    model_name = ''
    # TODO: Error handling
    with open(os.path.join(cyphy_model_dir,'model_config.json'), 'r') as f_p:
        model_name = json.load(f_p)['model_name']
    
    if not model_name:
        # TODO: log/indicate error
        pass
    
    if faultIndex == 0:
        text = 'ok=simulateModel("' + model_name + '()"'
        text = text + ',startTime=0,stopTime=100,numberOfIntervals=500,method="Radau IIa",tolerance = 1e-6'
        res = 'fault_'+ str(faultIndex)
        text = text + ',resultFile="'+res+'"'
        text = text + ');\n'
        file.write(text)
    else:
        text = 'ok=simulateModel("' + model_name + '('+component+'(mode='+fault+',amount='+amount+'))"'
        text = text + ',startTime=0,stopTime=100,numberOfIntervals=500,method="Radau IIa",tolerance = 1e-6'
        res = 'fault_'+ str(faultIndex)
        text = text + ',resultFile="'+res+'"'
        text = text + ');\n'
        file.write(text)
    
    file.write('cd("'+script_dir.replace("\\","/")+'");\n')
    file.write('if not ok then \n')
    file.write('savelog("'+os.path.join(fault_sim_rslt_dir, res+'_log.txt').replace('\\','/')+'");\n')
    file.write('end if;\n ')
    file.write('Advanced.CheckPackageRestriction:=true;\n')
    file.write('exit();\n ')
    file.close()

    return script_name


def loadFaults(file_name):
    infile = open(os.path.join(script_dir,file_name),'rU')
    components=[]
    faults=[]
    amounts=[]
    for text in infile:
        temp = text.split()
        components.append(temp[0])
        faults.append(temp[1])
        amounts.append(temp[2])
    infile.close()
    return (components,faults,amounts)

def runDymolaScript(script_name, timeout):

    if os.path.exists("C:/Program Files (x86)/Dymola 2015/bin64/Dymola.exe"):
        DYMOLA_EXE = "C:/Program Files (x86)/Dymola 2015/bin64/Dymola.exe"
    elif os.path.exists("C:/Program Files (x86)/Dymola 2014/bin64/Dymola.exe"):
        DYMOLA_EXE = "C:/Program Files (x86)/Dymola 2014/bin64/Dymola.exe"
    elif os.path.exists("C:/Program Files (x86)/Dymola 2013/bin64/Dymola.exe"):
        DYMOLA_EXE = "C:/Program Files (x86)/Dymola 2013/bin64/Dymola.exe"
    elif os.path.exists("C:/Program Files (x86)/Dymola 2014 FD01/bin64/Dymola.exe"):
        DYMOLA_EXE = "C:/Program Files (x86)/Dymola 2014 FD01/bin64/Dymola.exe"
    else:
        msg = "DYMOLA NOT FOUND\n"
        fp = open("_FAILED.txt", "w")
        fp.write(msg)
        fp.close()
        sys.stderr.write(msg + "\n")
        raise RuntimeError(msg)
        
    DYMOLA_EXE = DYMOLA_EXE.replace('\\','/')

    startTime = time.time()
    currentTime = startTime

    cmd  = DYMOLA_EXE+ ' /nowindow '+ os.path.join(fault_sim_rslt_dir,script_name)

    proc = subprocess.Popen(cmd)
    while proc.poll()==None and currentTime - startTime <timeout:
            time.sleep(0.2)
            currentTime = time.time()
    if currentTime - startTime >=timeout:
            # dymola takes too long and it will be shut down
            #proc.kill()
            #os.system('taskkill /f /IM dymosim.exe')
            #os.kill(proc.pid, signal.SIGTERM)
            killtree(proc.pid)
            success = 0;
            totalTime = currentTime - startTime;
            return (success, totalTime)
    else:
            success = 1;
            totalTime = currentTime - startTime;
            return (success, totalTime)


def killtree(pid, including_parent=True):
    subprocess.call(['taskkill', '/F', '/T', '/PID', str(pid)])
    #pgid = os.getpgid(pid)
    #os.killpg(pgid, signal.SIGKILL)

def findCriticalFault(numFaults,outputFileName,component,fault,amount):
    FAMESimRsltDict = OrderedDict()
    # nominal design performance: 'fault_0.mat'
    os.chdir(cyphy_model_dir)
    result = Metrics.get_metrics(os.path.join(fault_sim_rslt_dir,'fault_0.mat'))
    metricValues0 = {}
    kpiList = []
    criticality = 0
    FAMESimRsltDict[0] = OrderedDict()
    FAMESimRsltDict[0]['fault_description'] = OrderedDict()
    FAMESimRsltDict[0]['fault_description']['component'] = ''
    FAMESimRsltDict[0]['fault_description']['fault'] = 'nominal'
    FAMESimRsltDict[0]['fault_description']['amount'] = '0'
    FAMESimRsltDict[0]['metrics'] = OrderedDict()
    for kpi in sorted(result["metrics"], key=lambda key: result["metrics"][key]):
        kpiList.append(kpi)
        metricValues0[kpi] = result["metrics"][kpi]['value']
        FAMESimRsltDict[0]['metrics'][kpi] = result["metrics"][kpi]['value']
    metricValues0["final_time"] = result["end_time"]
    FAMESimRsltDict[0]['criticality'] = criticality

    crashedSimulationCnt = 0
    criticalFault = []
    for i in range(numFaults):
        faultID = i+1
        matfilename = 'fault_' + str(faultID) + '.mat'
        metricValues = {}
        criticality = 0
        
        FAMESimRsltDict[faultID] = OrderedDict()
        FAMESimRsltDict[faultID]['fault_description'] = OrderedDict()
        FAMESimRsltDict[faultID]['fault_description']['component'] = component[i]
        FAMESimRsltDict[faultID]['fault_description']['fault'] = fault[i]
        FAMESimRsltDict[faultID]['fault_description']['amount'] = amount[i]
        FAMESimRsltDict[faultID]['metrics'] = OrderedDict()
        if os.path.exists(os.path.join(fault_sim_rslt_dir,matfilename)):
            try:
                #print os.path.join(fault_sim_rslt_dir,matfilename)
                result = Metrics.get_metrics(os.path.join(fault_sim_rslt_dir,matfilename))
                
                for kpi in kpiList:
                    metricValues[kpi] = result["metrics"][kpi]['value']
                    FAMESimRsltDict[faultID]['metrics'][kpi] = result["metrics"][kpi]['value']
                metricValues["final_time"] = result["end_time"]
                if metricValues["final_time"]/metricValues0["final_time"] > 0.01:
                    # check if there is significant difference in output
                    for kpi in kpiList:
                        if abs(float(metricValues[kpi]  - metricValues0[kpi] )/
                                metricValues0[kpi] ) > 0.05:

                            criticalFault.append(i)
                            criticality = 1
            except:
                print("could not understand the result: ", matfilename)
                crashedSimulationCnt += 1
                for kpi in kpiList:
                    FAMESimRsltDict[faultID]['metrics'][kpi] = np.nan
        else:
            print("could not find the result: ", matfilename)
            crashedSimulationCnt += 1
            for kpi in kpiList:
                FAMESimRsltDict[faultID]['metrics'][kpi] = np.nan
        FAMESimRsltDict[faultID]['criticality'] = criticality
        FAMESimRsltDict[faultID]['simulation_file']  = OrderedDict()
        FAMESimRsltDict[faultID]['simulation_file']['model']  = "scripts/fault_sim_rslt/fault_" + str(faultID) + '.mos'
        if criticality == 1:
            FAMESimRsltDict[faultID]['simulation_file']['output']  = "scripts/fault_sim_rslt/fault_" + str(faultID) + '.mat'
    with open(os.path.join(script_dir,outputFileName),'w') as outfile:
        json.dump(FAMESimRsltDict,outfile, indent=4,sort_keys=False)

    print(set(criticalFault))
    print(crashedSimulationCnt)
    os.chdir(script_dir) 
    return len(set(criticalFault)),crashedSimulationCnt      


def main():
    fault_info_file = "faults.txt"
    component,fault,amount = loadFaults(fault_info_file)
    numSampleFaults = len(component)
    #numSampleFaults = 25
    dymolaScript = createScript("","",0,"0.0")
    runDymolaScript(dymolaScript, 120.0)
    for index in range(numSampleFaults):
        dymolaScript = createScript(component[index],fault[index],index+1,amount[index])
        runDymolaScript(dymolaScript, 120.0)
    outputFileName = "FaultSimulation.json"
    numKeyFaults, crashedSimulationCnt  = findCriticalFault(numSampleFaults,outputFileName,
            component,fault,amount)

    if crashedSimulationCnt < numSampleFaults:

        keyfilename = "testbench_manifest.json"
        keyfile = os.path.join(output_dir,keyfilename)
        with open(keyfile,"r") as infile:
            jsondata = json.load(infile, object_pairs_hook=OrderedDict)
        for i in range(len(jsondata["Metrics"])):
            if jsondata["Metrics"][i]["Name"] == "NumCritFaults":
                jsondata["Metrics"][i]["Value"] = str(numKeyFaults)
                jsondata["Status"] = "EXECUTED"
            if jsondata["Metrics"][i]["Name"] == "NumCriticalFaults":
                jsondata["Metrics"][i]["Value"] = str(numKeyFaults)
                jsondata["Status"] = "EXECUTED"
            if jsondata["Metrics"][i]["Name"] == "Number_Critical_Faults":
                jsondata["Metrics"][i]["Value"] = str(numKeyFaults)
                jsondata["Status"] = "EXECUTED"
            if jsondata["Metrics"][i]["Name"] == "NumPossFaults":
                if not (jsondata["Metrics"][i]["Value"]).isdigit():
                    jsondata["Metrics"][i]["Value"] = str(numSampleFaults)
            if jsondata["Metrics"][i]["Name"] == "NumPossibleFaults":
                if not (jsondata["Metrics"][i]["Value"]).isdigit():
                    jsondata["Metrics"][i]["Value"] = str(numSampleFaults)
            if jsondata["Metrics"][i]["Name"] == "Number_Faults":
                if not (jsondata["Metrics"][i]["Value"]).isdigit():
                    jsondata["Metrics"][i]["Value"] = str(numSampleFaults)
        if numKeyFaults > 0:
            newArtifact = OrderedDict()
            newArtifact["Location"] = "FaultSimulation.json"
            newArtifact["Tag"] = "Fault Analysis Results"
            jsondata["Artifacts"].append(newArtifact)
            os.rename(os.path.join(script_dir, outputFileName),os.path.join(output_dir, outputFileName))
        with open(keyfile,"w") as outfile:
            json.dump(jsondata,outfile, indent=4)

    else:
        os.remove(os.path.join(script_dir,outputFileName))


    #condenseDymolaResult(range(len(component)))

if __name__ == "__main__":
    main()
