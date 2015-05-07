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
from common import PostProcess

def createScript(component,fault,faultIndex,amount):

    script_name = 'fault_'+ str(faultIndex)+'.mos'

    file = open(os.path.join(fault_sim_rslt_dir,script_name),'w')
    file.write('openModel("'+os.path.join(aug_library_dir,'Modelica\package.mo')+'");\n')
    file.write('openModel("'+os.path.join(aug_library_dir,'FAME\package.mo')+'");\n')
    file.write('openModel("'+os.path.join(aug_library_dir,'C2M2L_Decl\package.mo')+'");\n')
    file.write('openModel("'+os.path.join(aug_library_dir,'C2M2L_CyPhy\package.mo')+'");\n')
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

    if faultIndex == 0:
        text = 'ok=simulateModel("CyPhy.TestBenches.SystemDesignTest()"'
        text = text + ',startTime=0,stopTime=100,numberOfIntervals=500,method="Radau IIa",tolerance = 1e-6'
        res = 'fault_'+ str(faultIndex)
        text = text + ',resultFile="'+res+'"'
        text = text + ');\n'
        file.write(text)
    else:
        text = 'ok=simulateModel("CyPhy.TestBenches.SystemDesignTest('+component+'(mode='+fault+',amount='+amount+'))"'
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
    elif os.paht.exists("C:/Program Files (x86)/Dymola 2014 FD01/bin64/Dymola.exe"):
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

def findcriticalFault(numFaults,outputFileName):
    FAMESimRsltDict = OrderedDict()
    # nominal design performance: 'fault_0.mat'
    os.chdir(fault_sim_rslt_dir)
    filter = [];
    vehicle_speed = 'FTP_Driver.driver_bus.vehicle_speed'
    #vehicle_speed = 'Road_Wheel_Load_Both_Sides.vehicleSpeed'
    filter.append(vehicle_speed)
    pp = PostProcess('fault_0.mat', filter)
    max_speed_0 = pp.global_max(vehicle_speed) * 3.6
    distance_0 = pp.integrate(vehicle_speed)
    acc20kph_0 = pp.get_time_for_crossing(vehicle_speed, 20 / 3.6)
    avgSpeed_0 = (distance_0 / pp.time[-1]) * 3.6
    final_time_0 = pp.time[-1]

    FAMESimRsltDict[0] = OrderedDict()
    FAMESimRsltDict[0]["MaxSpeed"] = max_speed_0
    FAMESimRsltDict[0]["Distance"] = distance_0
    FAMESimRsltDict[0]["Acc20kph"] = acc20kph_0
    FAMESimRsltDict[0]["AvgSpeed"] = avgSpeed_0

    crashedSimulationCnt = 0
    criticalFault = []
    for i in range(numFaults):
        faultID = i+1
        matfilename = 'fault_' + str(faultID) + '.mat'
        if os.path.exists(matfilename):
            try:
                filter = [];
                vehicle_speed = 'FTP_Driver.driver_bus.vehicle_speed'
                filter.append(vehicle_speed)
                pp = PostProcess(matfilename, filter)
                max_speed = pp.global_max(vehicle_speed) * 3.6
                distance = pp.integrate(vehicle_speed)
                acc20kph = pp.get_time_for_crossing(vehicle_speed, 20 / 3.6)
                avgSpeed = (distance / pp.time[-1]) * 3.6
                final_time = pp.time[-1]
                # check if simulation actually simulated to 
                # at least 1/100 of normal simulation
                if final_time/final_time_0 > 0.01:
                    # check if there is significant difference in output
                    if float(max_speed_0 - max_speed)/max_speed_0 > 0.05:
                        criticalFault.append(i)
                    if float(acc20kph - acc20kph_0)/acc20kph_0 > 0.05:
                        criticalFault.append(i)
                    if float(avgSpeed_0  - avgSpeed)/avgSpeed_0 > 0.05:
                        criticalFault.append(i)
                    if float(distance_0 - distance)/distance_0 > 0.05:
                        criticalFault.append(i)
                FAMESimRsltDict[faultID] = OrderedDict()
                FAMESimRsltDict[faultID]["MaxSpeed"] = max_speed
                FAMESimRsltDict[faultID]["Distance"] = distance
                FAMESimRsltDict[faultID]["Acc20kph"] = acc20kph
                FAMESimRsltDict[faultID]["AvgSpeed"] = avgSpeed
            except:
                print "could not understand the result: ", matfilename
                crashedSimulationCnt += 1
                FAMESimRsltDict[faultID] = OrderedDict()
                FAMESimRsltDict[faultID]["MaxSpeed"] = np.nan
                FAMESimRsltDict[faultID]["Distance"] = np.nan
                FAMESimRsltDict[faultID]["Acc20kph"] = np.nan
                FAMESimRsltDict[faultID]["AvgSpeed"] = np.nan   
        else:
            print "could not find the result: ", matfilename
            crashedSimulationCnt += 1
            FAMESimRsltDict[faultID] = OrderedDict()
            FAMESimRsltDict[faultID]["MaxSpeed"] = np.nan
            FAMESimRsltDict[faultID]["Distance"] = np.nan
            FAMESimRsltDict[faultID]["Acc20kph"] = np.nan
            FAMESimRsltDict[faultID]["AvgSpeed"] = np.nan
    with open(os.path.join(script_dir,outputFileName),'w') as outfile:
        json.dump(FAMESimRsltDict,outfile, indent=4,sort_keys=False)

    print set(criticalFault)
    print crashedSimulationCnt
    os.chdir(script_dir) 
    return len(set(criticalFault)),crashedSimulationCnt      


def main():
    fault_info_file = "faults.txt"
    component,fault,amount = loadFaults(fault_info_file)
    numSampleFaults = len(component)
    #numSampleFaults = 5
    dymolaScript = createScript("","",0,"0.0")
    runDymolaScript(dymolaScript, 120.0)
    for index in range(numSampleFaults):
        dymolaScript = createScript(component[index],fault[index],index+1,amount[index])
        runDymolaScript(dymolaScript, 120.0)
    outputFileName = "FaultSimulation.json"
    numKeyFaults, crashedSimulationCnt  = findcriticalFault(numSampleFaults,outputFileName)

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
        with open(keyfile,"w") as outfile:
            json.dump(jsondata,outfile, indent=4)

    else:
        os.remove(os.path.join(script_dir,outputFileName))


    #condenseDymolaResult(range(len(component)))

if __name__ == "__main__":
    main()
