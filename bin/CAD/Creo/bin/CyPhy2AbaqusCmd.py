"""
CyPhy2AbaqusCmd.py, version 6.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Generates an Abaqus/CAE model from an AP203 STEP file
containing the entire CAD assembly.

Optional arguments: unitLength, noFEA, adaptive,
    standard, globalSeed, edgeSeedDensity,
    seedDevProduct, seedMinSize, parallelCores,
    ramAllocated, memoryUnits, separationTolerance,
    sizeGrowthRate, saveCAE, meshIterations, errorIndicator

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>,
Jesse Hayes <jhayes@isis.vanderbilt.edu> and
Matthew Pike <mpike@isis.vanderbilt.edu>.

If you can\'t get the script to work, perform the steps in
the GUI and then compare this script to the file
\'abaqus.rpy\' when it appears in the working directory.

A triple hash \'###\' comment indicates that the code in
that line places a limitation on the expansion of script
functionality, and is marked for revision.

For more information on this script, see README.txt.

For information on Abaqus-specific objects, consult the
Abaqus Scripting Reference Manual in your Abaqus
documentation.
"""

from abaqus import *
from abaqusConstants import *
from caeModules import *
from numpy import *
from jobMessage import *
from optparse import *
from operator import xor
import multiprocessing
import math, re, sys, json, os
import interaction, visualization, regionToolset, job, mesh
import numpy.linalg as LA
import xml.etree.ElementTree as ET
import string as STR
import uuid, ctypes
import csv
import _winreg
import odbAccess
import shutil
from collections import defaultdict
import os
import glob
import traceback
from subprocess import Popen
import pickle
import time

root = os.getcwd()                              # initial working directory
logDir = os.path.join(root,"log",
    "CyPhy2AbaqusCmd.log")                      # path to log file

try:

    with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
        META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]

    adamsDir = os.path.join(META_PATH, 'bin', 'CAD', 'Adams2Abaqus')
    sys.path.append(adamsDir)

    from Adams2AbaqusMain import *

    materialLibInterfacePath = os.path.join(META_PATH,"bin","Python27","Lib","site-packages")

    sys.path.insert(0, materialLibInterfacePath)

    from MaterialLibraryInterface import LibraryManager

    path = ctypes.c_wchar_p(chr(0x00) * 256)
    FOLDERID_Documents = ctypes.c_char_p(uuid.UUID("ED4824AF-DCE4-45A8-81E2-FC7965083634").bytes_le)
    ctypes.windll.shell32.SHGetKnownFolderPath(FOLDERID_Documents, 0, None, ctypes.byref(path))
    materialLibPath = os.path.join(path.value,"META Documents","MaterialLibrary","material_library.json")

    library_manager = LibraryManager(materialLibPath)

except Exception as e:
    f.write('ERROR: Error during finding the material library\n')
    f.write(STR.join(traceback.format_exception(*sys.exc_info())))
    raise


def afterJobModal(jobName,analysisStepName):

    f = open(logDir, "a")                       # open the log file
    odb = odbAccess.openOdb(path=jobName + '.odb')
    
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write('Job complete\n')

    
    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        headers = (('Mode Number','Frequency(Hz)'))
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in opening %s.\n' % jobName)
        raise

    try:
        for histReg in odb.steps[analysisStepName].historyRegions.keys():
            eigenFrequencies = odb.steps[analysisStepName].historyRegions[histReg].historyOutputs['EIGFREQ'].data
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in reading eigenfrequencies\n')
        raise
        
    forCSV = (headers,) + eigenFrequencies

    f.write("Creating the CSV file" + '\n')
    
    report_file2 = 'modalOutput.csv'
    try:
        with open(report_file2,'wb') as f2:
            writer = csv.writer(f2)
            for (number,frequency) in forCSV:
##                if not frequency == 'Frequency(Hz)':
##                    correctedFreq = frequency*31.623
##                else:
##                    correctedFreq = frequency
##                val = (number, correctedFreq)
                val = (number, frequency)
                writer.writerow(val)                    
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in exporting data to %s.\n' % report_file2)
        pass
    

    idArray = []
    reportFile = 'testbench_manifest.json'                              # name of metric JSON
    reportPath = os.path.join(root, reportFile)
    f.write("Updating the testbench_manifest.json file" + '\n')

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        ourResults = data["Metrics"]

        minMode = -1
        for (key,entry) in eigenFrequencies:
            if entry < minMode or minMode == -1:
                   minMode = entry

        try:
            if minMode == 0:
                f.write('WARNING: Rigid body modes are present, model has not been constrained properly' + '\n')
        except:
            pass

        for cs in ourResults:
            if cs["Name"] == 'Minimum_Mode':
                cs["Value"] = minMode
                cs["Unit"] = 'Hz'
    
        with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
            json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary

    except:
        f.write('\n')
        f.write('ERROR: Error in exporting data to %s.\n' % reportFile)
        pass

    f.close()
    odb.close()

    try:
        CopyOrDeleteResults()
    except:
        f.write('ERROR: Error in copying or deleting result files to users machine\n')
        pass

    f = open(logDir, "a")

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write('Success\n')
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.close()
    

####  Rigid Bodies will results in no value for summary.  Summary will need to be updated to allow this.  Currently it prodeces an error.
# callback function used to monitor job status, export results, execute post-processing
def afterJob(jobName,superRef,metricRef2,metricRef3,allAssembly,analysisStepName):

    analysisDir = os.path.join(root,"Analysis", "Abaqus")
    if not os.path.exists(analysisDir):
        os.makedirs(analysisDir)
    f = open(logDir, "a")                       # open the log file

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write('Job complete\n')

    check = 0
    if jobName.startswith("Adaptivity-1-iter"):
        lastIterStr = jobName.replace("Adaptivity-1-iter","")
        lastIter = int(lastIterStr)

        while lastIter > 0 and check == 0:
            try:
                AdaptiveJobName = "Adaptivity-1-iter" + str(lastIter)
                checkOdb = odbAccess.openOdb(path=AdaptiveJobName + '.odb')
                frameCheck = checkOdb.steps[analysisStepName].frames[1]
                check = 1
                jobName = AdaptiveJobName
            except:
                f.write('Error in reading results of %s.\n' % AdaptiveJobName)
                lastIter = lastIter - 1

    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        f.write('Reporting results of %s.\n' % jobName)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in reporting results of %s.\n' % jobName)
        raise

    #new3DXMLPath = os.path.join(root, jobName + '.3dxml')
    #session.write3DXMLFile(fileName=new3DXMLPath)   # export 3DXML

    try:
        instRef = superRef['instRef']                         # identify refs in superRef
        mtrlRef = superRef['mtrlRef']                         # ''
        metricRef = superRef['metricRef']                     # ''
        lengthUnit = superRef['lengthUnit']                   # ''
        #frame = odb.steps['staticLoading'].frames[1]    # odb frame containing field outputs
        frame = odb.steps[analysisStepName].frames       # odb frame containing field outputs
        #sout = frame.fieldOutputs['S'].values           # stress field output
        maxStress = {}
        maxDisp = {}
        fosFatigue = {}
        fosYield = {}
        fosUltimate = {}
        fos = {}
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('Error in defining variables of post-processing\n')
        raise

    
    try:
        for key in instRef.keys():                      # for each part/subasm instance:
            if instRef[key]['isRigid']:
                continue
            maxStress.update([[key,0]])                     # create a maxStress entry
            maxDisp.update([[key,0]])                       # create a maxDisp entry
            fosFatigue.update([[key,0]])                    # create a fos entry for fatigue strength
            fosUltimate.update([[key,0]])                   # create a fos entry for ultimate strength
            fos.update([[key,0]])
        for j in range(len(frame)):
            currentFrame = odb.steps[analysisStepName].frames[j]
            sout = currentFrame.fieldOutputs['S'].values
            for i in range(len(sout)):                      # for each value in the stress field output:
                try:
                    inst = sout[i].instance.name                    # get name of part instance
                    if instRef[inst]['isRigid']:
                        continue
                    if sout[i].mises > maxStress[inst]:             # if the Mises stress is greater than maxStress for that part:
                        maxStress[inst] = sout[i].mises             # set the part's maxStress equal to that Mises stress
                except AttributeError:
                    pass
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('Error in creating entires for post-processing\n')
        raise

    if allAssembly == False:

        if 'Mises' in metricRef:                        # if Mises stress is a metric in the XML:
            for inst in maxStress.keys():                   # iterating through key+value pairs in maxStress
                if inst in metricRef['Mises']:
                    metricRef['Mises'][inst] = maxStress[inst]


        #if 'FactorOfSafety' in metricRef:               # if FOS is a metric in the XML:  
        try:
            for inst in maxStress.keys():                   # for each entry in maxStress:
                mtrlID = instRef[inst]['MaterialID']            # get the Material ID
                failFatigueStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_fatigue']                        # get the Fatigue Strength material property
                failYieldStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_tensile_yield']                  # get the Yield Tensile Strength material property
                failUltimateStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_tensile_ultimate']               # get the Ultimate Tensile Strength material property
                fosFatigue[inst] = failFatigueStress/maxStress[inst]        # store the factor of safety for fatigue strength
                fosYield[inst] = failYieldStress/maxStress[inst]           # store the factor of safety for fatigue strength
                fosUltimate[inst] = failUltimateStress/maxStress[inst]      # store the factor of safety for ultimate strength
                fos[inst] = failFatigueStress/maxStress[inst]
            for inst in fos.keys():                                         # for each entry in fos:
                if inst in metricRef['FactorOfSafety']:
                    metricRef['FactorOfSafety'][inst] = fos[inst]
        except:
            f.write("Insufficient data for FactorOfSafety\n")
            pass

        try:
            for j in range(len(frame)):
                currentFrame = odb.steps[analysisStepName].frames[j]
                uout = currentFrame.fieldOutputs['U'].values           # displacement field output
        except:
            f.write('ERROR: Error in reading the displacement data\n')
            pass
   
            for i in range(len(uout)):                      # for each value in the disp. field output:
                try:
                    inst = uout[i].instance.name
                    if uout[i].magnitude > maxDisp[inst]:           # if disp. magnitude is greater than maxDisp:
                        maxDisp[inst] = uout[i].magnitude           # set maxDisp equal to that disp. magnitude
                except AttributeError:
                    continue
        if 'Displacement' in metricRef:                 # if displacement is a metric in the XML:
            for inst in maxDisp.keys():                     # for each entry in maxDisp:
                if inst in metricRef['Displacement']:
                    metricRef['Displacement'][inst] = maxDisp[inst]

        idArray = []
        reportFile = 'testbench_manifest.json'         # name of metric JSON
        reportPath = os.path.join(root, reportFile)

        f.write("Updating the testbench_manifest.json file" + '\n')

        try:
            with open(reportPath, 'r') as json_data:
                data = json.load(json_data)
            ourResults = data["Metrics"]
            for rs in ourResults:
                idArray.append(rs["GMEID"])

            for component in idArray:
                for component2 in metricRef2:
                    if component == component2:
                        for component3 in maxStress:
                            if metricRef3[component] == instRef[component3]['ComponentID']:
                                if metricRef2[component] == 'Mises':
                                    for cs in ourResults:
                                        if cs["GMEID"] == component:
                                            cs["Value"] = maxStress[component3]
                                            cs["Unit"] = 'MPa'
                                elif metricRef2[component] == 'FactorOfSafety':
                                    for cs in ourResults:
                                        if cs["GMEID"] == component:
                                            cs["Value"] = fos[component3]
                                            cs["Unit"] = 'None'
                                else:
                                    for cs in ourResults:
                                        if cs["GMEID"] == component:
                                            cs["Value"] = 'None'
                                            cs["Unit"] = 'None'

            minFosFatigue = -1
            for inst in fosFatigue.keys():
                if fosFatigue[inst] < minFosFatigue or minFosFatigue == -1:
                    minFosFatigue = fosFatigue[inst]

            minFosYield = -1
            for inst in fosYield.keys():
                if fosYield[inst] < minFosYield or minFosYield == -1:
                    minFosYield = fosYield[inst]

            minFosUltimate = -1
            for inst in fosUltimate.keys():
                if fosUltimate[inst] < minFosUltimate or minFosUltimate == -1:
                    minFosUltimate = fosUltimate[inst]

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Fatigue_Strength_Factor_of_Safety':
                    cs["Value"] = minFosFatigue
                    cs["Unit"] = 'None'

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Yield_Strength_Factor_of_Safety':
                    cs["Value"] = minFosYield
                    cs["Unit"] = 'None'

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Ultimate_Tensile_Strength_Factor_of_Safety':
                    cs["Value"] = minFosUltimate
                    cs["Unit"] = 'None'
                

    
            with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
                json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary       

        
        except:
            f.write('\n')
            f.write('ERROR: Error in exporting data to %s.\n' % reportFile)
            pass

    else:
        try:
            for inst in maxStress.keys():                   # for each entry in maxStress:
                mtrlID = instRef[inst]['MaterialID']            # get the Material ID
                failFatigueStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_fatigue']                        # get the Fatigue Strength material property
                failYieldStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_tensile_yield']                  # get the Yield Tensile Strength material property
                failUltimateStress = mtrlRef[mtrlID]\
                    ['mechanical__strength_tensile_ultimate']               # get the Ultimate Tensile Strength material property
                fosFatigue[inst] = failFatigueStress/maxStress[inst]        # store the factor of safety for fatigue strength
                fosYield[inst] = failYieldStress/maxStress[inst]            # store the factor of safety for yield strength
                fosUltimate[inst] = failUltimateStress/maxStress[inst]      # store the factor of safety for ultimate strength
                fos[inst] = failFatigueStress/maxStress[inst]

                
        except:
            f.write("Insufficient data for FactorOfSafety\n")
            pass

        idArray = []
        reportFile = 'testbench_manifest.json'         # name of metric JSON
        reportPath = os.path.join(root, reportFile)

        f.write("Updating the testbench_manifest.json file" + '\n')

        try:
            with open(reportPath, 'r') as json_data:
                data = json.load(json_data)
            ourResults = data["Metrics"]

            minFosFatigue = -1
            for inst in fosFatigue.keys():
                if fosFatigue[inst] < minFosFatigue or minFosFatigue == -1:
                    minFosFatigue = fosFatigue[inst]

            minFosYield = -1
            for inst in fosYield.keys():
                if fosYield[inst] < minFosYield or minFosYield == -1:
                    minFosYield = fosYield[inst]

            minFosUltimate = -1
            for inst in fosUltimate.keys():
                if fosUltimate[inst] < minFosUltimate or minFosUltimate == -1:
                    minFosUltimate = fosUltimate[inst]

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Fatigue_Strength_Factor_of_Safety':
                    cs["Value"] = minFosFatigue
                    cs["Unit"] = 'None'

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Yield_Strength_Factor_of_Safety':
                    cs["Value"] = minFosYield
                    cs["Unit"] = 'None'

            for cs in ourResults:
                if cs["Name"] == 'Minimum_Ultimate_Tensile_Strength_Factor_of_Safety':
                    cs["Value"] = minFosUltimate
                    cs["Unit"] = 'None'               
    
            with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
                json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary

        except:
            f.write('\n')
            f.write('ERROR: Error in exporting data to %s.\n' % reportFile)
            pass


    try:    
        f.write("Creating the CSV file" + '\n')

        tempInstRef = {}
        headers = {}
        val = {}
        forCSVlist = []   
    
        headers['Part Name'] = {'fatigueStr': 'Fatigue Strength (MPa)', 'yieldStr': 'Yield Strength (MPa)', 'ultimateStr': 'Ultimate Tensile Strength (MPa)', 'fosFatigue': 'Factor of Safety for Fatigue Strength', 
            'fosYield': 'Factor of Safety for Yield Strength', 'fosUltimate': 'Factor of Safety for Ultimate Tensile Strength', 'maxStress': 'Maximum Stress (MPa)', 'MaterialID': 'IloveMyJob', 'order': 1,
            'isPart': 'NA', 'ComponentID': 'Unique ID'}
        tempInstRef = instRef

        for key in tempInstRef:
            if key in maxStress:
                try:
                    tempInstRef[key]['maxStress'] = maxStress[key]
                except:
                    tempInstRef[key]['maxStress'] = 'Err'
                try:
                    tempInstRef[key]['fosFatigue'] = fosFatigue[key]
                except:
                    tempInstRef[key]['fosFatigue'] = 'Err'
            else:
                tempInstRef[key]['maxStress'] = 'N/A'
                tempInstRef[key]['fosFatigue'] = 'N/A'

        for key in tempInstRef:
            if key in maxStress:
                try:
                    tempInstRef[key]['maxStress'] = maxStress[key]
                except:
                    tempInstRef[key]['maxStress'] = 'Err'
                try:
                    tempInstRef[key]['fosYield'] = fosYield[key]
                except:
                    tempInstRef[key]['fosYield'] = 'Err'
            else:
                tempInstRef[key]['maxStress'] = 'N/A'
                tempInstRef[key]['fosYield'] = 'N/A'

        for key in tempInstRef:
            if key in maxStress:
                try:
                    tempInstRef[key]['maxStress'] = maxStress[key]
                except:
                    tempInstRef[key]['maxStress'] = 'Err'
                try:
                    tempInstRef[key]['fosUltimate'] = fosUltimate[key]
                except:
                    tempInstRef[key]['fosUltimate'] = 'Err'
            else:
                tempInstRef[key]['maxStress'] = 'N/A'
                tempInstRef[key]['fosUltimate'] = 'N/A'

        for key in instRef:
            matID = instRef[key]['MaterialID']
            tempInstRef[key]['fatigueStr'] = mtrlRef[matID]['mechanical__strength_fatigue']

        for key in instRef:
            matID = instRef[key]['MaterialID']
            tempInstRef[key]['yieldStr'] = mtrlRef[matID]['mechanical__strength_tensile_yield']

        for key in instRef:
            matID = instRef[key]['MaterialID']
            tempInstRef[key]['ultimateStr'] = mtrlRef[matID]['mechanical__strength_tensile_ultimate']

        orderNum = 2
        for key in tempInstRef:
            tempInstRef[key]['order'] = orderNum
            orderNum = orderNum + 1
    
        forCSV = dict(headers, **tempInstRef)

        for d in forCSV:
            temp = (d, forCSV[d]['ComponentID'], forCSV[d]['fatigueStr'], forCSV[d]['yieldStr'], forCSV[d]['ultimateStr'], forCSV[d]['maxStress'],
                forCSV[d]['fosFatigue'],forCSV[d]['fosYield'],forCSV[d]['fosUltimate'], forCSV[d]['order'])
            forCSVlist.append(temp)

        forCSVlist.sort(key=lambda x: float(x[9]))
      
        report_file2 = 'stressOutput.csv'
        try:
            with open(report_file2,'wb') as f:
                writer = csv.writer(f)
                for d in range(0, orderNum-1):
                    val = (forCSVlist[d][0], forCSVlist[d][1], forCSVlist[d][2], forCSVlist[d][3], forCSVlist[d][4],
                           forCSVlist[d][5], forCSVlist[d][6], forCSVlist[d][7], forCSVlist[d][8])
                    writer.writerow(val)
                    
        except:
            f.write('\n')
            f.write('ERROR: Error in exporting data to %s.\n' % report_file2)
            pass
    except:
        f.write('ERROR: Error in creating the CSV file\n')
        pass

    if not analysisStepName == 'dynamicLoading':
        try:
            CreateViewportPNG(odb,jobName)
        except:
            f.write('ERROR: Error in creating result PNG files\n')
            pass

    f.close()
    odb.close()

    try:
        CopyOrDeleteResults()
    except:
        f.write('ERROR: Error in copying or deleting result files to users machine\n')
        pass

    f = open(logDir, "a")

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write('Success\n')
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.close()

def CopyOrDeleteResults():

    f = open(logDir, "a")                                       # open the log file  
    
    reportFile = 'testbench_manifest.json'                      # name of metric JSON
    reportPath = os.path.join(root, reportFile)
    mainDir = os.path.join(root,"Analysis","Abaqus")

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        usrInput = data["CopyTestResults"]
        if not usrInput:
            f.write('\n')
            f.write("**********************************************************************************" + '\n')
            f.write('\n')
            f.write('Deleting result files\n')
            f.write('\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            for hgx in glob.glob("*.odb"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.023"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.com"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.dat"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.inp"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.mdl"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.msg"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.prt"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sim"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.stt"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                try:
                    os.remove(hgx)
                    f.write('Deleting file ' + str(hgx) + '\n')
                except:
                    pass
            for hgx in glob.glob("*.sta"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.csv"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.cae"):
                try:
                    shutil.move(hgx,mainDir)
                    f.write('Moving file ' + str(hgx) + '\n')
                except:
                    pass
        else:
            f.write('\n')
            f.write("**********************************************************************************" + '\n')
            f.write('\n')
            f.write('Moving result files\n')
            f.write('\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            for hgx in glob.glob("*.odb"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.023"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.com"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.dat"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.inp"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.mdl"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.msg"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.prt"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sim"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.stt"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                try:
                    shutil.move(hgx,mainDir)
                    f.write('Moving file ' + str(hgx) + '\n')
                except:
                    pass
            for hgx in glob.glob("*.sta"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.csv"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.cae"):
                try:
                    shutil.move(hgx,mainDir)
                    f.write('Moving file ' + str(hgx) + '\n')
                except:
                    pass

    except:
        f.write('WARNING: Can not move/delete Abaqus artifacts\n')
        pass

    f.close()


def EliminateOverlaps(instRef,rigidParts,myAsm,myModel):
    
    count = 0
    noOverlap = True

    f = open(logDir, "a") 

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Checking for overlapping (buy) parts" + '\n')
    f.write('\n')
        
    for rigid in rigidParts:
        for (key,entry) in instRef.iteritems():
            if not rigid == key:
                try:
                    newName = 'temp_' + str(count)
                    obsName = 'obs_' + str(count)
                    myAsm.InstanceFromBooleanCut(name=newName,instanceToBeCut=myAsm.instances[rigid],cuttingInstances=(myAsm.instances[key],),originalInstances=DELETE)
                    myModel.parts.changeKey(fromName=rigid,toName=obsName)
                    myModel.parts.changeKey(fromName=newName,toName=rigid)
                    p = myModel.parts[key]
                    myAsm.Instance(name=key, part=p, dependent=OFF)
                    for newi in myAsm.instances.keys():
                        if newi.startswith(newName):
                            myAsm.features.changeKey(fromName=newi, toName=rigid)
                            myAsm.makeIndependent(instances=(myAsm.instances[rigid], ))
                    f.write("Eliminating overlap between " + str(rigid) + ' and ' + str(key) + '\n')
                    noOverlap = False
                except:
                    pass

                count+=1

    if noOverlap:
        f.write("None of the (buy) parts are overlapping" + '\n')

    f.close()


def CreateViewportPNG(myOdb, fileName):

    f = open(logDir, "a")             # open the log file

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Creating the PNG files of the results" + '\n')
    f.write('\n')
    
    try:

        mainDir = os.path.join(root,"Analysis","Abaqus","Contour_and_BC_plots")
        if not os.path.exists(mainDir):
            os.mkdir(mainDir)

        os.chdir(mainDir)
        
        myViewport=session.Viewport(name='Contour Plot', origin=(0, 0), width=200, height=100)
        #myOdb = visualization.openOdb(path=odbName)
        myViewport.setValues(displayedObject=myOdb)

        mySteps = myOdb.steps
        numSteps = len(mySteps)

        t = myOdb.userData.Text(name='Text-1', text='MPa',offset=(34.1042, 179.422),\
            font='-*-arial-medium-r-normal-*-*-120-*-*-p-*-*-*')
        myViewport.plotAnnotation(annotation=t)
        
        session.printOptions.setValues(rendition=COLOR,
                                       vpDecorations=OFF, vpBackground=OFF)
        
        for i in range(numSteps):
            stepKey = mySteps.keys()[i]
            step = mySteps[stepKey]
            numFrames = len(step.frames)

            save2fileName=fileName.replace(' ','') + "_"
			
            #temporary solution until write3DXMLFile issue fixed-------------------------------------------------------------
            displacement = step.frames[-1].fieldOutputs['U']
            myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                                     outputPosition=NODAL,
                                                     refinement=(INVARIANT, 'Magnitude'))                   
            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
            session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
            
            f.write("Creating PNG file " + save2fileName+"Displacement_DEF" + '\n')
            
            #temporary solution end--------------------------------------------------------------------------------------
			
            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
            session.viewports['Contour Plot'].makeCurrent()
            session.viewports['Contour Plot'].maximize()
            session.viewports['Contour Plot'].viewportAnnotationOptions.setValues(
                legendFont='-*-verdana-medium-r-normal-*-*-120-*-*-p-*-*-*')
            session.writeVrmlFile(save2fileName+"DEF", False, (myViewport,))
            session.write3DXMLFile(save2fileName+"Displacement_DEF", False, (myViewport,))

            f.write("Creating 3DXML file " + save2fileName+"Displacement_DEF" + '\n')

            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
            session.viewports['Contour Plot'].makeCurrent()
            session.viewports['Contour Plot'].maximize()            
            session.writeVrmlFile(save2fileName+"UNDEF", False, (myViewport,))
            session.write3DXMLFile(save2fileName+"Displacement_UNDEF", False, (myViewport,))

            f.write("Creating 3DXML file " + save2fileName+"Displacement_UNDEF" + '\n')
            
           
##            #get displacement data
##            f.write("Grabbing displacement data" + '\n')
##            displacement = step.frames[-1].fieldOutputs['U']
##            myViewport.odbDisplay.setPrimaryVariable(field=displacement,
##                                                     outputPosition=NODAL,
##                                                     refinement=(INVARIANT, 'Magnitude'))                   
##            myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
##            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
##            myViewport.view.setValues(session.views['Iso'])
##            myViewport.view.zoom(0.8)
##            session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
            

            #get stress data
            f.write("Grabbing stress data" + '\n')
            stress = step.frames[-1].fieldOutputs['S']
            myViewport.odbDisplay.setPrimaryVariable(field=stress,
                                                     outputPosition=INTEGRATION_POINT,
                                                     refinement=(INVARIANT, 'Mises'))
            myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))

            #--------------------------------------------------------------------------------------------------------------------

            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
            session.viewports['Contour Plot'].makeCurrent()
            session.viewports['Contour Plot'].maximize()
            session.viewports['Contour Plot'].viewportAnnotationOptions.setValues(
                legendFont='-*-verdana-medium-r-normal-*-*-120-*-*-p-*-*-*')
            session.writeVrmlFile(save2fileName+"Stress_DEF", False, (myViewport,))
            session.write3DXMLFile(save2fileName+"Stress_DEF", False, (myViewport,))

            f.write("Creating 3DXML file " + save2fileName+"Stress_DEF" + '\n')

            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
            session.viewports['Contour Plot'].makeCurrent()
            session.viewports['Contour Plot'].maximize()            
            session.writeVrmlFile(save2fileName+"Stress_UNDEF", False, (myViewport,))
            session.write3DXMLFile(save2fileName+"Stress_UNDEF", False, (myViewport,))

            f.write("Creating 3DXML file " + save2fileName+"Stress_UNDEF" + '\n')

            #--------------------------------------------------------------------------------------------------------------------


            myViewport.view.setValues(session.views['Iso'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Iso_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Iso_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Left'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Left_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Left_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Right'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Right_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Right_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Back'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Back_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Back_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Front'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Front_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Front_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Top'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Stress_Top_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Top_UNDEF_Meshed" + '\n')

            myViewport.view.setValues(session.views['Bottom'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Stress_Bottom_UNDEF_Meshed", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Bottom_UNDEF_Meshed" + '\n')

            #--------------------------------------------------------------------------------------------------------------------

            myViewport.odbDisplay.commonOptions.setValues(
            visibleEdges=FEATURE)

            #--------------------------------------------------------------------------------------------------------------------

            myViewport.view.setValues(session.views['Iso'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Iso_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Iso_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Left'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Left_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Left_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Right'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Right_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Right_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Back'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Back_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Back_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Front'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Stress_Front_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Front_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Top'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Stress_Top_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Top_UNDEF" + '\n')

            myViewport.view.setValues(session.views['Bottom'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Stress_Bottom_UNDEF", PNG, (myViewport,))

            f.write("Creating PNG file " + save2fileName+"Stress_Bottom_UNDEF" + '\n')

            
            
    except KeyError:
        f.write('\n')
        f.write('ERROR: Key Error' + '\n')
        odb.close()
        exit(0)
    except (AbaqusException), value:
        f.write('ERROR:' + value + '\n')
        odb.close()
        exit(0)

    f.close()

def CreateOverlapPNG(jobName,overlapCheck):

    f = open(logDir, "a")             # open the log file

    try:
        myOdb = odbAccess.openOdb(path=jobName + '.odb')
        save2fileName=jobName.replace(' ','') + "_"

        resultsDir = os.path.join(root,"Analysis","Abaqus")
        if not os.path.exists(resultsDir):
            os.mkdir(resultsDir)
        
        mainDir = os.path.join(root,"Analysis","Abaqus","Contour_and_BC_plots")
        if not os.path.exists(mainDir):
            os.mkdir(mainDir)

        os.chdir(mainDir)

        myViewport = session.viewports['Viewport: 1']
        
        myViewport.setValues(displayedObject=myOdb)

        for k in range(len(overlapCheck.elements)):
            overlappingElems = overlapCheck.elements[k]
            for i in range(len(overlappingElems)):
                overlappingElem = overlappingElems[i]
                highlight(overlappingElem)

        myViewport.view.setValues(session.views['Iso'])
        myViewport.view.zoom(0.8)
        session.printToFile("Overlapping_Elements_1", PNG, (myViewport,))

        myViewport.view.setValues(cameraPosition=(987.505, -35.8282, 7.68834), cameraUpVector=(0, 1, 0))
        myViewport.view.fitView()

        session.printToFile("Overlapping_Elements_2", PNG, (myViewport,))

        session.viewports['Viewport: 1'].view.setValues(nearPlane=757.99, farPlane=1200.36, width=542.18, height=365.72, cameraPosition=(0.996667, 
            36.7201, -977.094), cameraUpVector=(0.0, 1.0, 0.0), cameraTarget=(-0.87486, -35.267, 7.11584))
        myViewport.view.fitView()

        session.printToFile("Overlapping_Elements_3", PNG, (myViewport,))

        session.viewports['Viewport: 1'].view.setValues(nearPlane=759.096, farPlane=1215.06, width=542.971, height=366.255, cameraPosition=(-91.9079, 
            -1009.75, -32.4658), cameraUpVector=(-1.0, 0.0, 0.0), cameraTarget=(1.67948, -27.8817, -0.616374))
        myViewport.view.fitView()

        session.printToFile("Overlapping_Elements_4", PNG, (myViewport,))
        
    except:
        pass

    f.close()

    os.chdir(root)
    

def CreateLOADSBCPNG(PreProcessFileName,myAsm,analysisStepName):

    f = open(logDir, "a")             # open the log file

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Creating the PNG files of the loads and BC's" + '\n')
    f.write('\n')

    analysisDir = os.path.join(root,"Analysis", "Abaqus")
    if not os.path.exists(analysisDir):
        os.makedirs(analysisDir)

    mainDir = os.path.join(root,"Analysis","Abaqus","Contour_and_BC_plots")
    if not os.path.exists(mainDir):
        os.mkdir(mainDir)

    os.chdir(mainDir)

    save2fileName=PreProcessFileName
	##########################################################	Print pictures of load and BCs
    session.viewports['Viewport: 1'].setValues(displayedObject=myAsm)
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(
        step=analysisStepName)
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(loads=ON, bcs=ON, 
        predefinedFields=ON, connectors=ON, optimizationTasks=OFF, 
        geometricRestrictions=OFF, stopConditions=OFF)
    session.viewports['Viewport: 1'].enableMultipleColors()
    session.viewports['Viewport: 1'].setColor(initialColor='#E9E9E9')
    cmap=session.viewports['Viewport: 1'].colorMappings['Load']
    session.viewports['Viewport: 1'].setColor(colorMapping=cmap)
    session.viewports['Viewport: 1'].disableMultipleColors()
    session.viewports['Viewport: 1'].assemblyDisplay.symbolOptions.setValues(
        arrowSymbolSize=21)    
    session.printOptions.setValues(vpBackground=OFF, compass=ON)
    session.printToFile(fileName='Boundary Conditions and Loads_1', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    f.write("Creating PNG file " + 'Boundary Conditions and Loads_1' + '\n')
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON, loads=OFF, 
        bcs=OFF, predefinedFields=OFF, connectors=OFF)
    session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
        meshTechnique=ON)	
    session.printToFile(fileName='Model Initial Mesh_1', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    f.write("Creating PNG file " + 'Model Initial Mesh_1' + '\n')
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF, loads=ON, 
        bcs=ON, predefinedFields=ON, connectors=ON)
    session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
        meshTechnique=OFF)		
    session.viewports['Viewport: 1'].view.setValues(cameraPosition=(987.505, 
    -35.8282, 7.68834), cameraUpVector=(0, 1, 0))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_2', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    f.write("Creating PNG file " + 'Boundary Conditions and Loads_2' + '\n')
    session.viewports['Viewport: 1'].view.setValues(nearPlane=757.99, 
        farPlane=1200.36, width=542.18, height=365.72, cameraPosition=(0.996667, 
        36.7201, -977.094), cameraUpVector=(0.0, 1.0, 0.0), 
        cameraTarget=(-0.87486, -35.267, 7.11584))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_3', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    f.write("Creating PNG file " + 'Boundary Conditions and Loads_3' + '\n')
    session.viewports['Viewport: 1'].view.setValues(nearPlane=759.096, 
        farPlane=1215.06, width=542.971, height=366.255, cameraPosition=(-91.9079, 
        -1009.75, -32.4658), cameraUpVector=(-1.0, 0.0, 0.0), 
        cameraTarget=(1.67948, -27.8817, -0.616374))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_4', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    f.write("Creating PNG file " + 'Boundary Conditions and Loads_4' + '\n')
    f.write('\n')
		
##############################################

    #except KeyError:
    #    f.write('\n')
    #    f.write('ERROR: Key Error' + '\n')
    #   print 'Key Error'
    #    exit(0)
    #except (AbaqusException), value:
    #    f.write('ERROR:' + value + '\n')
    #    print 'ERROR:', value
    #    exit(0)

    f.close()

    os.chdir(root)
	
# cleans up coordinates read from STEP file, prior to applying regular expressions
def regexFriendly(inString):
    outString = STR.replace(inString,'\'','%')
    outString = STR.replace(outString,'(','')
    outString = STR.replace(outString,')',',')
    return outString

e = re.compile(',\S+,,')                        # regular expression
# extracts tuple of cartesian coordinates from STEP coordinate string
def coordinate(stepString):
    coordFind = e.search(stepString)                        # extract substring containing coordinates
    coordList = coordFind.group(0).strip(',').split(',')    # separate x, y, and z coordinates by commas
    coords = (float(coordList[0]),float(coordList[1]),\
        float(coordList[2]))                                # convert coordinate strings to a tuple of floats
    return coords                                           # return the coordinate tuple
                                                            
# calculates transformation matrix between two coordinate systems as defined in STEP
def get3DTransformArray(fromDir1,fromDir2,toDir1,toDir2):
    fromDir1 = array(fromDir1)                          # convert u1 vector to an array object
    fromDir2 = array(fromDir2)                          # convert u2 vector to an array object
    fromDir3 = cross(fromDir1,fromDir2)                 # extrapolate u3 vector from u1 and u2
    toDir1 = array(toDir1)                              # convert v1 vector to an array object
    toDir2 = array(toDir2)                              # convert v2 vector to an array object
    toDir3 = cross(toDir1,toDir2)                       # extrapolate v3 vector from v1 and v2
    inva = LA.inv(transpose(vstack([fromDir1,fromDir2,fromDir3])))
    b = transpose(vstack([toDir1,toDir2,toDir3]))
    transformArray = dot(b,inva)
    return transformArray

# uses vector operations to get unit normal vector, given a center coordinate and two planar coordinates
def unv(center,planarA,planarB):
    center = array(center)
    planarA = array(planarA)
    planarB = array(planarB)
    vA = planarA - center
    vB = planarB - center
    xV = cross(vA, vB)
    return xV/LA.norm(xV)

# transforms/translates a cartesian point from one coordinate system to another
def transCoord(fromCoord,transformArray,translationVector):
    vprod = dot(transformArray,fromCoord)
    vprod = vprod + translationVector
    toCoord = tuple(vprod)
    return toCoord

# recursively identifies parts in sub-assemblies, in the order they are imported from STEP
def asmRecursion(asm,subAsms,asmParts):
    parts = []
    try:
        for child in subAsms[asm]:
            if child in subAsms:
                parts.extend(asmRecursion(child,subAsms,asmParts))
            else:
                parts.extend(asmParts[child])
    except KeyError:
        pass
    if asm in asmParts:
        parts.extend(asmParts[asm])
    return parts

# iterates through sub-assemblies and top-level parts to transform/translate
# every datum point to assembly coordinates; uses transCoord()
# note: ignores top-level datums in highest assembly, which will not exist
#       in a CyPhy assembly anyway
def coordTransform(localTMs,localTVs,asm,subAsms,asmParts,localCoords):
    globalCoords = {}                                               # create dictionary object to hold new point library
    if asm in subAsms:                                              # if assembly has sub-assemblies:
        for subAsm in subAsms[asm]:                                     # for each sub-assembly in the assembly:
            subCoords = coordTransform(localTMs,localTVs,                   # get point library local to sub-assembly
                subAsm,subAsms,asmParts,localCoords)
            for part in subCoords.keys():                                   # for each part/sub-sub-assembly in chosen sub-assembly:
                globalCoords.update([[part,{}]])                                # create new entry in globalCoords
                for (point,coord) in subCoords[part].iteritems():               # for each point in part/sub-sub-assembly:
                    globalCoords[part].update([[point,transCoord(                   # translate/transform point to globalCoords
                        array(coord),localTMs[subAsm],localTVs[subAsm])]])
            globalCoords.update([[subAsm,{}]])                              # create entry for sub-assembly in globalCoords
            for (point,coord) in localCoords[subAsm].iteritems():           # for each point specified at top level of that sub-assembly:
                globalCoords[subAsm].update([[point,transCoord(                 # translate/transform point to globalCoords
                    array(coord),localTMs[subAsm],localTVs[subAsm])]])
    if asm in asmParts:                                             # if assembly has top-level parts:
        for part in asmParts[asm]:                                      # for each top-level part:
            globalCoords.update([[part,{}]])                                # create new entry in globalCoords
            for (point,coord) in localCoords[part].iteritems():             # for each point in part:
                globalCoords[part].update([[point,transCoord(                   # translate/transform point to globalCoords
                    array(coord),localTMs[part],localTVs[part])]])
    return globalCoords

# produces mask string for getSequenceFromMask(...) from a feature ID or set of IDs
def myMask(idnums):
    try:
        idnums = tuple(idnums)                                  # make the input a tuple!
    except TypeError:                                       # if input is not iterable:
        idnums = (idnums,)                                      # make it a tuple anyway!
    powersum = 0                                            # integer to hold mask number
    for num in idnums:                                      # iterating through input IDs:
        powersum += 2**num                                      # add 2**ID to powersum
    rawmask = hex(powersum)[2:]                             # convert powermask to hexadecimal
    rawmask = STR.rstrip(rawmask,'L')                       # strip "long" character, if necessary
    if max(idnums) < 32:                                    # if hex number is 8 digits or less:
        mask = '[#' + rawmask + ' ]'                            # create mask
    else:                                                   # if hex number is >8 digits:
        maskpieces = []                                         # container for fragments of hex string
        piececount = int(math.ceil(len(rawmask)/8))             # number of times to split hex string
        for i in range(piececount):                             # for each split needed:
            maskpieces.append(rawmask[-8:])                         # append last 8 characters of hex string to fragment list
            rawmask = rawmask[:-8]                                  # trim last 8 characters from hex string
        maskpieces.append(rawmask)                              # append remaining hex string to fragment list
        mask = '[#' + STR.join(maskpieces,' #') + ' ]'          # join fragments, using the correct delimiters, to create mask
    return mask

# translates a degree of freedom as read from the XML to the appropriate SymbolicConstant
def toBC(constraint):
    if constraint == 'FIXED':
        return 0
    elif constraint == 'FREE':
        return UNSET
    else:
        return float(constraint)

# the function that is executed in the command line
def main():

    f = open(logDir, "w")             # create a log file

    now = time.strftime("%c")
    
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write("**********************************************************************************" + '\n')
    f.write("************************STARTING FEA MODEL-BASED v6.1.0***************************" + '\n')
    f.write("**********************************************************************************" + '\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Initializing Abaqus/CAE" + '\n')
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write(now + '\n')
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    
    # take command line arguments
    usage = "usage: abaqus cae script=CyPhy2AbaqusCmd.py -- [options]"
    parser = OptionParser(usage=usage)
    parser.add_option("-l","--unitLength", default="millimeter",
        choices=["millimeter","meter","inch"],
        help="unit of length used in Creo models")
    parser.add_option("-o","--noFEA", default=False, action="store_true",
        help="""disable assembly of faces from datum points and creation of loads and BCs
            (necessary for META 13.13)""")
    parser.add_option("-a","--adaptive", default=False, action="store_true",
        help="run an adaptive meshing process")
    parser.add_option("-f","--standard", default=False, action="store_true",
        help="run a standard FEA analysis (ignored if -a is called)")
    parser.add_option("-s","--globalSeeds", default=False, action="store_true",
        help="""define mesh seed size proportional to size of entire assembly;
                if not called, seed size is defined by size of individual part""")
    parser.add_option("-e","--edgeSeedDensity", default=40, type=float,
        help="coefficient inversely proportional to seed size")
    parser.add_option("-x","--seedDevFactor", default=0.0, type=float,
        help="chordal deviation factor (sensitivity of seeding to curves)")
    parser.add_option("-d","--dynamic", default=False, action="store_true",
        help="run a dynamic implicit FEA analysis")
    parser.add_option("-y","--seedMinSize", default=0.0, type=float,
        help="ratio of smallest permitted seed size to standard seed size")
    parser.add_option("-m","--modal", default=False, action="store_true",
        help="run a modal analysis")
    parser.add_option("-p","--parallelCores", default=1, type=int,
        help="number of CPU cores to use in solver")
    parser.add_option("-r","--ramAllocated", default=90, type=int,
        help="integer amount of memory allocated to solver, defined in units memoryUnits")
    parser.add_option("-u","--memoryUnits", default="PERCENTAGE",
        choices=["PERCENTAGE","MEGA_BYTES","GIGA_BYTES"],
        help="units of memory allocated to solver")
    parser.add_option("-t","--separationTolerance", default=1E-3, type=float,
        help="tolerance for separation between faces when creating tie constraints")
    parser.add_option("-c","--saveCAE", default=True, action="store_true",
        help="save the model as a .cae model database file")
    parser.add_option("-g","--sizeGrowthRate", default=1.05, type=float,
        help="interior element growth rate; minimum 1.00, maximum 2.00")
    parser.add_option("-n","--meshIterations", default=3, type=int,
        help="number of adaptive mesh iterations")
    parser.add_option("-q","--errorIndicator", default="ENDENERI",
        choices=["MISESERI","ENDENERI","CPRESSERI","CSHEARERI","PEEQUERI",
                 "PEERI","CEERI","HFLERI","EFLERI","EPGERI"],
        help="""error indicator variable for adaptive meshing;
                not all variables are supported yet
                (see Section 4.1.4 of the Abaqus Analysis Manual)""")
    parser.add_option("-v","--rigidParts", default=True, action="store_true",
        help="use Rigid parts as specified in CADAssembly.xml")

    if sys.argv[2] == '-noGUI':
        (args,testBenchArg) = parser.parse_args(sys.argv[8:])
    else:
        (args,testBenchArg) = parser.parse_args(sys.argv[6:])

    # assign variable to the blank model produced at startup
    myModel = mdb.models['Model-1']
    myAsm = myModel.rootAssembly
    runAdams = False

    try:
        reportFile = 'testbench_manifest.json'
        kinComValFile = "kinComputedValues.xml"
        reportPath = os.path.join(root, reportFile)
        kinComValPath = os.path.join(root, kinComValFile)

        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        adamsData = data["Dependencies"]
        adamsPath = adamsData[0]["Directory"]

        os.chdir(adamsPath)
        for hgx in glob.glob("*.lod"):
            shutil.copy(hgx,root)
        for hgx in glob.glob("*.xml"):
            if hgx == "ComputedValues.xml":
                shutil.copy(hgx,kinComValPath)
        runAdams = True
    except:
        pass

    os.chdir(root)

    if runAdams:
        primary()
        sys.exit()
    else:
        pass  

    f.write('\n')
    f.write("Calculating geometry scale factor" + '\n')
    # calculate geometry scale factor (with respect to meters)
    if args.unitLength == 'millimeter':
        unitScale = 0.001
        unitShort = 'mm'
    elif args.unitLength == 'inch':
        unitScale = 0.0254
        unitShort = 'in'
    elif args.unitLength == 'meter':
        unitScale = 1.0
        unitShort = 'm'
    else:
        f.write('\n')
        f.write("ERROR: Length unit not supported" + '\n')
        raise ValueError("Length unit not supported")

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write("Length unit is " + str(unitShort) + '\n')
    f.write("Unit scale of " + str(unitScale) + ' will be used during the analysis\n')

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    lengthUnit = unitShort
    
    # unit conversion dictionary uses mmNs, more convenient than mmKs
    conv = {}                                                       # unit conversion dictionary
    conv.update([['GPa',(10**9)*(unitScale**2)],                        # to N/length^2
                 ['MPa',(10**6)*(unitScale**2)],                        # to N/length^2
                 ['kPa',(10**3)*(unitScale**2)],                        # to N/length^2
                 ['pascal',unitScale**2],                               # to N/length^2
                 ['Pa',unitScale**2],                                   # to N/length^2
                 ['g/cm^3',(10**6)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['kg/m^3',(10**-3)*(unitScale**3)],                     # to (N-s^2/length)/length^3
                 ['kg/mm^3',(10**6)*(unitScale**3)],                   # to (N-s^2/length)/length^3
                 ['newton',1],                                          # to N
                 ['kN',10**3],                                          # to N
                 ['newton',1],                                          # to N
                 ['newton-mm',(10**-3)*(unitScale**-1)],                # to N-length
                 ['m/s2',1],                                            # to length/s^2
                 ['mm/s2',(10**-3)/unitScale],                          # to length/s^2
                 ['N/A',1],                                             # dimensionless
                 ['1/C',1],                                             # to 1/C
                 ['C',1],                                               # to Celsius
                 ['W/(m-K)',1],                                         # to (N-length/s)/(K-length)
                 ['J/(K-kg)',unitScale**-2]])                           # to (N-length)/(K-(N-s^2/length))


    try:
        f.write("XML parser is obtaining necessary data from CyPhy" + '\n')                     #XML parser obtains necessary data from CyPhy
        
        xmlName = 'CADAssembly.xml'
        xmlPath = os.path.join(root, xmlName)

        f.write("Reading data from CADAssembly.xml" + '\n')
        
        try:
            tree = ET.parse(xmlPath)
            xmlRoot = tree.getroot()
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot parse CADAssembly.xml.\n')
            raise
        
        try:
            assemblyXML = xmlRoot.find('Assembly')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"Assembly/" in CADAssembly.xml.\n')
            raise
            
        try:
            analysesXML = assemblyXML.find('Analyses')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"Analyses/" in CADAssembly.xml.\n')
            raise
            
        try:
            feaXML = analysesXML.find('FEA')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"FEA/" in CADAssembly.xml.\n')
            raise

        try:
            cadAssemblyXML = assemblyXML.find('CADComponent')
            cadComponentSetXML = cadAssemblyXML.findall('CADComponent')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"CADComponent/" in CADAssembly.xml.\n')
            raise
            
        if not args.noFEA:
            try:
                analysisConstraintsXML = feaXML.find('AnalysisConstraints')
                analysisConstraintSetXML = analysisConstraintsXML.getiterator('AnalysisConstraint')
            except:
                f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                f.write('WARNING: The model has not been constrained properly. This may cause rigid body motion during regular FEA analysis.\n')
                f.write('ERROR: Terminating.\n')
                raise

    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in reading CADAssembly.xml file.\n')
        raise

    try:
        f.write("XML parser is obtaining necessary data from Create Assemby program" + '\n')
        
        xmlMetricsName = 'CADAssembly_metrics.xml'
        xmlMetricsPath = os.path.join(root, xmlMetricsName)

        f.write("Reading data from CADAssembly_metrics.xml" + '\n')
        
        try:
            treeMetrics = ET.parse(xmlMetricsPath)
            xmlMetricsRoot = treeMetrics.getroot()
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot parse CADAssembly_metrics.xml.\n')
            raise

        try:    
            metricComponentsXML = xmlMetricsRoot.find('MetricComponents')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"MetricComponents/" in CADAssembly_metrics.xml.\n')
            raise

        try:    
            metricComponentXML = metricComponentsXML.findall('MetricComponent')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"MetricComponent/" in CADAssembly_metrics.xml.\n')
            raise

        try:
            metricAssembliesXML = xmlMetricsRoot.find('Assemblies')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"Assemblies/" in CADAssembly_metrics.xml.\n')
            raise

        try:
            metricAssemblyXML = metricAssembliesXML.find('Assembly')
        except:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Cannot find the block /"Assembly/" in CADAssembly_metrics.xml.\n')
            raise
            
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in reading CADAssembly_metrics.xml file.\n')
        raise

    if args.adaptive:
        try:
            f.write("Reading the user input for maximum number of iterations" + '\n')
            maxNumberIter = int(feaXML.get("MaxAdaptiveIterations"))
            if maxNumberIter > 10:
                maxNumberIter = 10
            elif maxNumberIter < 2:
                maxNumberIter = 2
            args.meshIterations = maxNumberIter
            f.write(str(maxNumberIter) + ' mesh adaptivity iterations will be performed during analysis\n')
        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error in reading maximum number of iterations from CADAssembly.xml file.\n')
            raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
        
    if not args.noFEA: 
        f.write("Creating load and boundary condition dictionaries" + '\n')
        f.write('\n')
        f.write('\n')
        try:
            loadBCLib = []                                                  # set to contain a dictionary for each load and BC
            i = 0
            for constraint in analysisConstraintSetXML:                     # for each constraint definition:
                loadBCLib.append({})                                            # add new dictionary
                i += 1
                dispConstraintXML = constraint.find('Displacement')         # look for displacement constraint                
                pinConstraintXML = constraint.find('Pin')                       # look for pin constraint
                if ET.iselement(dispConstraintXML):                             # if displacement constraint found:
                    
                    f.write("Defining displacement constraint" + '\n')

                    f.write('\n')
                    
                    loadBCLib[-1].update([['Treatment','Displacement']])            # face is labeled "Displacement"
                    rotConstraintXML = dispConstraintXML.find('Rotation')
                    xRot = toBC(rotConstraintXML.get('x'))                          # rotation about 1 axis
                    yRot = toBC(rotConstraintXML.get('y'))                          # rotation about 2 axis
                    zRot = toBC(rotConstraintXML.get('z'))                          # rotation about 3 axis
                    loadBCLib[-1].update([['RotDOF',(xRot,yRot,zRot)]])             # store degrees of freedom as tuple
                    transConstraintXML = dispConstraintXML.find('Translation')
                    xTrans = toBC(transConstraintXML.get('x'))                      # translation in 1 axis
                    yTrans = toBC(transConstraintXML.get('y'))                      # translation in 2 axis
                    zTrans = toBC(transConstraintXML.get('z'))                      # translation in 3 axis
                    loadBCLib[-1].update([['TransDOF',(xTrans,yTrans,zTrans)]])     # store DOF as tuple

                    f.write("Rotation in x-dir = " + str(xRot) + '\n')
                    f.write("Rotation in y-dir = " + str(yRot) + '\n')
                    f.write("Rotation in z-dir = " + str(zRot) + '\n')
                    f.write('\n')
                    f.write("Translation in x-dir = " + str(xTrans) + '\n')
                    f.write("Translation in y-dir = " + str(yTrans) + '\n')
                    f.write("Translation in z-dir = " + str(zTrans) + '\n')

                    f.write('\n')
                    
                elif ET.iselement(pinConstraintXML):                            # if pin constraint found:

                    f.write("Defining pin constraint" + '\n')

                    f.write('\n')
                    
                    loadBCLib[-1].update([['Treatment','Pin']])                     # face is labeled "Pin"
                    axialDispXML = pinConstraintXML.find('AxialDisplacement')
                    pinDisp = toBC(axialDispXML.get('Property'))                    # get axial displacement parameter
                    loadBCLib[-1].update([['AxialDisp',pinDisp]])                   # store SymbolicConstant
                    axialRotXML = pinConstraintXML.find('AxialRotation')
                    pinRot = toBC(axialRotXML.get('Property'))                      # get axial rotation parameter
                    loadBCLib[-1].update([['AxialRot',pinRot]])                     # store SymbolicConstant

                    f.write("Pin Dispacement = " + str(pinDisp) + '\n')
                    f.write('\n')
                    f.write("Pin Rotation = " + str(pinRot) + '\n')

                    f.write('\n')
                    
                geometryXML = constraint.find('Geometry')
                featuresXML = geometryXML.find('Features')                      # go to geometry features
                loadBCLib[-1].update([['GeometryType',featuresXML.get('GeometryType')]])
                loadBCLib[-1].update([['1stGeomQual',featuresXML.get('PrimaryGeometryQualifier')]])
                loadBCLib[-1].update([['2ndGeomQual',featuresXML.get('SecondaryGeometryQualifier')]])
                featureSetXML = featuresXML.getiterator('Feature')              # ^^store geometry settings 
                loadBCLib[-1].update([['ComponentID',featureSetXML[0].get('ComponentID')]])
                pointNames = []
                for feature in featureSetXML:                                   # for each geometry feature:
                    pointNames.append(feature.get('Name'))                          # store new point name
                loadBCLib[-1].update([['Points',pointNames]])                   # store names of datum points
            loadSetXML = feaXML.getiterator('Load')
            accel = dict(x=0, y=0, z=0)
            for load in loadSetXML:                                         # for each load definition:
                loadBCLib.append({})                                            # add new dictionary
                forceMomentXML = load.find('ForceMoment')                       # look for ForceMoment load
                pressureXML = load.find('Pressure')                             # look for Pressure load
                accelXML = load.find('Acceleration')                            # look for Acceleration load
                if ET.iselement(forceMomentXML):                                # if ForceMoment load found:

                    f.write("Defining Force/Moment" + '\n')

                    f.write('\n')
                    
                    loadBCLib[-1].update([['Treatment','ForceMoment']])             # load is type "ForceMoment"
                    forceXML = forceMomentXML.find('Force')
                    fUnits = forceXML.get('Units')                                  # get force units
                    xForce = float(forceXML.get('x'))*conv[fUnits]                  # force along 1 axis
                    yForce = float(forceXML.get('y'))*conv[fUnits]                  # force along 2 axis
                    zForce = float(forceXML.get('z'))*conv[fUnits]                  # force along 3 axis
                    loadBCLib[-1].update([['Force',(xForce,yForce,zForce)]])        # store forces as tuple
                    momentXML = forceMomentXML.find('Moment')
                    mUnits = momentXML.get('Units')                                 # get moment units
                    xMoment = float(momentXML.get('x'))*conv[mUnits]                # moment about 1 axis
                    yMoment = float(momentXML.get('y'))*conv[mUnits]                # moment about 2 axis
                    zMoment = float(momentXML.get('z'))*conv[mUnits]                # moment about 3 axis
                    loadBCLib[-1].update([['Moment',(xMoment,yMoment,zMoment)]])    # store moments as tuple
                    hasGeometry = True

                    f.write("Force in x-dir = " + str(xForce) + '\n')
                    f.write("Force in y-dir = " + str(yForce) + '\n')
                    f.write("Force in z-dir = " + str(zForce) + '\n')
                    f.write('\n')
                    f.write("Moment in x-dir = " + str(xMoment) + '\n')
                    f.write("Moment in y-dir = " + str(yMoment) + '\n')
                    f.write("Moment in z-dir = " + str(zMoment) + '\n')

                    f.write('\n')
                    
                elif ET.iselement(pressureXML):                                 # if Pressure load found:

                    f.write("Defining pressure load" + '\n')

                    f.write('\n')
                    
                    loadBCLib[-1].update([['Treatment','Pressure']])                # load is type "Pressure"
                    pUnits = 'MPa'                                                  ### manually assigning units (not given)
                    pMag = float(pressureXML.get('Value'))*conv[pUnits]             # magnitude of pressure
                    loadBCLib[-1].update([['Pressure',pMag]])                       # store pressure value
                    hasGeometry = True

                    f.write("Pressure Magnitude = " + str(pMag) + '\n')

                    f.write('\n')
                    
                elif ET.iselement(accelXML):

                    f.write("Defining acceleration load" + '\n')

                    f.write('\n')

                    aUnits = accelXML.get('Units')
                    accel['x'] = float(accelXML.get('x'))*conv[aUnits]
                    accel['y'] = float(accelXML.get('y'))*conv[aUnits]
                    accel['z'] = float(accelXML.get('z'))*conv[aUnits]

                    f.write("Acceleration in x-dir = " + str(accel['x']) + '\n')
                    f.write("Acceleration in y-dir = " + str(accel['y']) + '\n')
                    f.write("Acceleration in z-dir = " + str(accel['z']) + '\n')

                    f.write('\n')
                    
                    hasGeometry = False
                if hasGeometry:
                    geometryXML = load.find('Geometry')
                    featuresXML = geometryXML.find('Features')                      # go to geometry features
                    loadBCLib[-1].update([['GeometryType',featuresXML.get('GeometryType')]])
                    loadBCLib[-1].update([['1stGeomQual',featuresXML.get('PrimaryGeometryQualifier')]])
                    loadBCLib[-1].update([['2ndGeomQual',featuresXML.get('SecondaryGeometryQualifier')]])
                    featureSetXML = featuresXML.getiterator('Feature')              # ^ store geometry settings ^
                    loadBCLib[-1].update([['ComponentID',featureSetXML[0].get('ComponentID')]])    ### assumes all points in same component
                    pointNames = []
                    for feature in featureSetXML:                                   # for each geometry feature:
                        pointNames.append(feature.get('Name'))
                    loadBCLib[-1].update([['Points',pointNames]])                   # store names of datum points
                else:
                    loadBCLib.pop()

        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error in creating load and boundary condition dictionaries\n')
            f.write('Possible causes of error are:\n')
            f.write('1- Unknown BC or Load definiton in CADAssembly.xml file, \n')
            f.write('2- Missing information in the CADAssembly.xml file BC definition(i.e. Rotation, Translation, x, y, z, RotDOF, TransDOF, AxialDisplacement, Property, AxialRotation, Geometry\n')
            f.write('                                                                      , Features, Feature, GeometryType, PrimaryGeometryQualifier, SecondaryGeometryQualifier, Feature, ComponentID) \n')
            f.write('3- Missing information in the  CADAssembly.xml file load definition(i.e. ForceMoment, Pressure, Acceleration, Force, Units, x, y, z, Moment, Value, Units, Features\n')
            f.write('                                                                         , Feature, GeometryType, PrimaryGeometryQualifier, SecondaryGeometryQualifier, Feature, ComponentID) \n')
            raise
    else:
       loadBCLib = [] 
                              
    try:
        instRef = {}
        instIndex = {}
        instAssemblyRef = {}
        instAssemblyIndex = {}
        rigidPartMasses = []
        rigidPartVolumes = []
        rigidPartDensities = []
        rigidParts = []

        nsmap = {}
        rigidPartPresent = False
        Jan24_deactivate_rigidity = False
        
        for component in metricComponentsXML.findall('MetricComponent'):
            if component.get('Type') == 'PART':
                instName = STR.upper(component.get('Name'))
                metricID = component.get('MetricID')
                mtrl = component.find('Material')
                materialIDRead = mtrl.get('Type')
                materialID = materialIDRead.lower()
                
                isPart = True
                for event, elem in ET.iterparse(xmlMetricsName, events=('start', 'end')):
                    if event == 'start' and elem.tag == 'CADComponent':
                        if metricID == elem.get('MetricID'):
                            componentInstanceID = elem.get('ComponentInstanceID')                  
                            
            elif component.get('Type') == 'ASSEMBLY':
                instName = STR.upper(component.get('Name')) + '_ASM'
                isPart = False
                metricID = component.get('MetricID')
                for event, elem in ET.iterparse(xmlMetricsName, events=('start', 'end')):
                    if event == 'start' and elem.tag == 'CADComponent':
                        if metricID == elem.get('MetricID'):
                            componentInstanceID = elem.get('ComponentInstanceID')

            if component.get('Type') == 'PART':
                instRef.update([[instName,{}]])
                instRef[instName].update([['isPart',isPart]])
                instRef[instName].update([['ComponentID',componentInstanceID]])
                try:
                    instRef[instName].update([['MaterialID',materialID]])
                except:
                    pass
            
                instRef[instName].update([['isShell',
                    "123zzzz123333331312121" in component.get('Name').lower()]])                                                    # temporarily deactivating 'isShell' key
                if args.rigidParts:                          
                    instRef[instName].update([['isRigid',
                        component.get('Rigidity')=="RIGID"]])
                else:
                    instRef[instName].update([['isRigid',False]])
                instIndex.update([[componentInstanceID,instName]])
      
                for event2, elem2 in ET.iterparse(xmlName, events=('start', 'end')):
                    if event2 == 'start' and elem2.tag == 'CADComponent':
                        if componentInstanceID == elem2.get('ComponentID'):
                            nsmap[componentInstanceID] = elem2

                for elem2Partial in nsmap:
                    holder = nsmap[elem2Partial]
                    if componentInstanceID == elem2Partial:
                        for manParam in holder.findall('ManufacturingParameter'):
                            manParamName = manParam.get('Name')
                            if manParamName == 'procurement__make_or_buy':
                                manParamValue = manParam.get('Value').lower()
                                if manParamValue == 'buy':
                                    instRef[instName].update([['isRigid',"RIGID"]])
                                    instRef[instName].update([['MaterialID',instName+"_Rigid"]])

                                    if materialID == "undefined":
                                        instRef[instName].update([['MaterialID',"steel_low_alloy_a572_gr50"]])
                                        
                                    scalars = component.find('Scalars')
                                    for scalar in scalars.findall('Scalar'):
                                        if scalar.get('Name') == 'Mass':
                                            conversionFactor = 1
                                            if scalar.get('Unit') == 'kg':
                                                conversionFactor = 1
                                            if scalar.get('Unit') == 'g':
                                                conversionFactor = 0.001
                                            if scalar.get('Unit') == 'tonne' or scalar.get('Unit') == 'ton' or scalar.get('Unit') == 'tons':
                                                conversionFactor = 1000

                                            mass = scalar.get('Value')
                                            convertedMass = float(mass) * conversionFactor

                                        if scalar.get('Name') == 'Volume':
                                            conversionFactor = 1
                                            if scalar.get('Unit') == 'mm3' or scalar.get('Unit') == 'mm^3':
                                                conversionFactor = 1
                                            if scalar.get('Unit') == 'cm3' or scalar.get('Unit') == 'cm^3':
                                                conversionFactor = 1000
                                            if scalar.get('Unit') == 'dm3' or scalar.get('Unit') == 'dm^3':
                                                conversionFactor = 1000000
                                            if scalar.get('Unit') == 'm3' or scalar.get('Unit') == 'm^3':
                                                conversionFactor = 1000000000

                                            volume = scalar.get('Value')
                                            convertedVolume = float(volume) * conversionFactor

                                    density = convertedMass/convertedVolume
                                            
                                    rigidPartMasses.append((instName,convertedMass))
                                    rigidPartVolumes.append((instName,convertedVolume))
                                    rigidPartDensities.append((instName,density))
                                    rigidParts.append(instName)
                                    rigidPartPresent = True
                            
            elif component.get('Type') == 'ASSEMBLY':
                instAssemblyRef.update([[instName,{}]])
                instAssemblyRef[instName].update([['isPart',isPart]])
                instAssemblyRef[instName].update([['ComponentID',componentInstanceID]])
                instAssemblyRef[instName].update([['MaterialID','N/A']])
                instAssemblyRef[instName].update([['isRigid',False]])
                instAssemblyRef[instName].update([['isShell',
                    "123zzzz123333331312121" in component.get('Name').lower()]])                                                    # temporarily deactivating 'isShell' key
                instAssemblyIndex.update([[componentInstanceID,instName]])
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in creating necessary data arrays\n')
        f.write('Possible causes of error are:\n')
        f.write('1- Unknown component type definiton (not Part or Assembly) in CADAssembly_metrics.xml file, \n')
        f.write('2- Missing information in the CADAssembly_metrics.xml file Part definition(i.e. Name, MetricID, Material, Type, ComponentInstanceID\n')
        f.write('3- Missing information in the CADAssembly.xml file Part definition(i.e. ComponentID, MaterialID, ManufacturingParameter, Scalars, Mass, Volume\n')
        f.write('4- Missing information in the CADAssembly_metrics.xml file Assembly definition(i.e. Name, MetricID, ComponentInstanceID\n')
        raise

    mtrlRef = {}                                                                                                                    # set to contain a dictionary for each material
    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Grabbing properties of each material from the material library" + '\n')
    f.write('Material properties that will be grabbed are density, modulus of elasticity, poisson\'s ratio,\n')
    f.write('ultimate tensile strength, fatigue strength, yield strength\n')
    mtrlProps = ["density","mechanical__modulus_elastic","mechanical__ratio_poissons",
        "mechanical__strength_tensile_ultimate","mechanical__strength_fatigue","mechanical__strength_tensile_yield"]                # set of names of material properties to get

    f.write('\n')
    
    try:
        for component in metricComponentsXML.findall('MetricComponent'):
            rigidPart = False
            try:
                instName = STR.upper(component.get('Name'))
            except:
                f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                f.write('ERROR: Missing part name in CADAssembly_metrics.xml.\n')
                raise

            try:    
                for key in rigidParts:
                    if key == instName:
                        rigidPart = True
                        for rigidPartDensityNumAndName in rigidPartDensities:
                            if rigidPartDensityNumAndName[0] == key:
                                rigidDensity = rigidPartDensityNumAndName[1]
            except:
                f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                f.write('ERROR: Rigid part definition discrepancy\n')
                raise                
                
            if component.get("Type") == "PART" and not rigidPart:

                try:
                    mtrl = component.find('Material')
                except:
                    f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f.write('ERROR: Missing material for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise

                try:
                    mtrlName = mtrl.get('Type').lower()
                except:
                    f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f.write('ERROR: Missing material type for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise  

                mtrlRef.update([[mtrlName,{}]])
                    
                try:
                    mtrlData = library_manager.materialData(mtrlName)               # get mtrl data from the material library for the corresponding material
                except:
                    f.write('\n')
                    f.write("ERROR: " + mtrlName + \
                        " is not in the material library." + '\n')
                    raise ValueError("ERROR: " + mtrlName + \
                        " is not in the material library." + '\n')
                for prop in mtrlProps:
                    if mtrlData[prop]["value"]=="None":
                        f.write('\n')
                        f.write("ERROR: Property " + prop + " of " + mtrlName + \
                            " has not been defined in the material library." + '\n')
                        raise ValueError("Property " + prop + " of " + mtrlName + \
                            " has not been defined in the material library.")
                    if "unit" in mtrlData[prop]:                                            # if property has units
                        units = conv[mtrlData[prop]["unit"]]                                # get unit scaling factor
                    else:                                                                   # if no units (e.g. Poisson)
                        units = 1.0                                                         # unit scaling factor is 1
                    mtrlRef[mtrlName].update([[prop,
                        float(mtrlData[prop]["value"])*units]])
                    
            elif component.get("Type") == "PART" and rigidPart:

                try:
                    mtrl = component.find('Material')
                except:
                    f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f.write('ERROR: Missing material for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise

                try:
                    mtrlName = mtrl.get('Type').lower()
                except:
                    f.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f.write('ERROR: Missing material type for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise
                
                mtrlNameRigid = instName + "_Rigid"

                mtrlRef.update([[mtrlNameRigid ,{}]])

                mtrlDataRigid = {'density': {'value': 'None','unit': 'None'}, 'mechanical__modulus_elastic': {'value': 'None','unit': 'None'}, 'mechanical__ratio_poissons': {'value': 'None'},\
                    'mechanical__strength_tensile_ultimate': {'value': 'None','unit': 'None'}, 'mechanical__strength_fatigue': {'value': 'None','unit': 'None'},\
                    'mechanical__strength_tensile_yield': {'value': 'None','unit': 'None'}}
                    
                try:
                    mtrlData = library_manager.materialData(mtrlName)                       # get mtrl data from the material library for the corresponding material
                    for key in mtrlDataRigid.keys():
                        mtrlDataRigid[key]["value"] = mtrlData[key]["value"]
                        if "unit" in mtrlDataRigid[key]:
                            mtrlDataRigid[key]["unit"] = mtrlData[key]["unit"]
                except:
                    pass
                
                for prop in mtrlProps:
                    if prop == "density":
                        mtrlDataRigid[prop]["value"] = rigidDensity
                        mtrlDataRigid[prop]["unit"] = "kg/mm^3"
                    if mtrlDataRigid[prop]["value"]=="None":
                        if prop == "mechanical__modulus_elastic":
                            mtrlDataRigid[prop]["value"] = 200000
                            mtrlDataRigid[prop]["unit"] = "MPa"
                        elif prop == "mechanical__ratio_poissons":
                            mtrlDataRigid[prop]["value"] = 0.3
                        elif prop == "mechanical__strength_tensile_ultimate":
                            mtrlDataRigid[prop]["value"] = 400
                            mtrlDataRigid[prop]["unit"] = "MPa"
                        elif prop == "mechanical__strength_fatigue":
                            mtrlDataRigid[prop]["value"] = 200
                            mtrlDataRigid[prop]["unit"] = "MPa"
                        elif prop == "mechanical__strength_tensile_yield":
                            mtrlDataRigid[prop]["value"] = 250
                            mtrlDataRigid[prop]["unit"] = "MPa"
                    if "unit" in mtrlDataRigid[prop]:                                       # if property has units
                        units = conv[mtrlDataRigid[prop]["unit"]]                           # get unit scaling factor
                    else:                                                                   # if no units (e.g. Poisson)
                        units = 1.0                                                         # unit scaling factor is 1
                    mtrlRef[mtrlNameRigid].update([[prop,
                        float(mtrlDataRigid[prop]["value"])*units]])

            if component.get("Type") == "PART":
                f.write('Grabbing material properties of ' + str(mtrlName) + ' for the part ' + str(instName) + '\n')

                    
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in grabbing and defining material names and properties\n')
        raise

    try:
        metricSetXML = feaXML.getiterator('Metric')
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Cannot find \"Metric\" inside the block \"FEA\" in CADAssembly.xml.\n')
        raise
    
    
    try:
        metricsSetXML = feaXML.find('Metrics')
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Cannot find \"Metrics\" inside the block \"FEA\" in CADAssembly.xml.\n')
        raise
    
    try:
        metricsComponentSetXML = metricsSetXML.findall('Metric')
    except:
        pass

    
    metricRef = {}
    metricRef2 = {}
    metricRef3 = {}
    allAssembly = True
    try:
        for metric in metricSetXML:
            if metric.get('ComponentID') in instIndex:
                allAssembly = False
                if not metric.get('MetricType') in metricRef:
                    metricRef.update([[metric.get('MetricType'),{}]])
                metricRef[metric.get('MetricType')].update([[
                    instIndex[metric.get('ComponentID')],None]])

        for metrics in metricsComponentSetXML:
            if metrics.get('ComponentID') in instIndex:
                metricRef2.update([[metrics.get('MetricID'),metrics.get('MetricType')]])
                metricRef3.update([[metrics.get('MetricID'),metrics.get('ComponentID')]])

    except:
        f.write('WARNING: In the FEA testbench there are no individual parts, just the assembly. Otherwise there might be an error while\n')
        f.write('         MetricType, ComponentID or MetricID values are being grabbed from CADAssembly.xml\n')
        allAssembly = True

    try:
        superRef = {}
        superRef.update([['instRef',instRef],
                         ['mtrlRef',mtrlRef],
                         ['metricRef',metricRef],
                         ['instIndex',instIndex],
                         ['loadBCLib',loadBCLib],
                         ['lengthUnit',lengthUnit]])
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Cannot define the super dictionary, probably because of input discrepancies\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
        
    # define the path to the STEP file and open it
    f.write("Defining the path for the STEP file" + '\n')
    try:
        if cadAssemblyXML.get("Type") == "ASSEMBLY":
            testBenchName = cadAssemblyXML.get("Name")
        stepName = testBenchName + '_asm.stp'
        stepPath = os.path.join(root, 'AP203_E2_SINGLE_FILE', stepName)
        step = mdb.openStep(fileName=stepPath)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during finding and opening the step file\n')
        raise

    f.write("Opening the STEP file " + str(stepName) + ' and converting it into raw data' + '\n')

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # generate datum points from STEP raw data
    f.write("Generating datum points from STEP raw data" + '\n')
    try:
        wtl = []                                    # whole text list
        ld = {}                                     # list index dictionary
        a = re.compile('#\d+(?==)')                 # regular expression a gets portion of line left of the = sign, e.g. "#4507"
        b = re.compile('(?<==).+;')                 # regular expression b gets portion of line right of the = sign
        lineB = ''                                  # placeholder for concatenated string
        addLine = False                             # initial setting for string concatenation mode
        i = 0
        with open(stepPath, 'rb') as stepData:
            for line in stepData:
                starts = a.search(line)
                stops = re.search(';',line)
                if not addLine and not starts:              # case 1: header
                    continue                                    # goto next line
                elif not addLine and stops:                 # case 2: '#num=...;'
                    lineB = line                                # save current line
                elif not addLine:                           # case 3: '#num=...\n'
                    lineB = line                                # save current line
                    lineB = lineB.strip('\n')                   # strip standard newline char
                    lineB = lineB.strip('\r')                   # strip Mac-style newline char
                    addLine = True                              # start adding lines
                    continue                                    # goto next line
                elif not stops:                             # case 4: '...\n'
                    lineB += line                               # add current line to previous line(s)
                    lineB = lineB.strip('\n')                   # strip standard newline char
                    lineB = lineB.strip('\r')                   # strip Mac-style newline char
                    continue                                    # goto next line
                else:                                       # case 5: '...;'
                    lineB += line                               # add current line to previous line(s)
                    addLine = False                             # stop adding lines
                aResult = a.search(lineB)                   # obtain pointer string
                bResult = b.search(lineB)                   # obtain value string
                key = aResult.group(0)
                data = bResult.group(0)
                wtl.append(data)
                ld.update([[key,i]])
                i += 1
        c = re.compile('(?<!\s)#\d+')
        d = re.compile('(\w+) with respect to (\w+)')
        localTMs = {}                               # dict of local transformation matrices
        localTVs = {}                               # dict of local translation vectors
        subAsms = {}                                # dict of sub-assemblies by parent assembly
        asmParts = {}                               # dict of parts by direct parent assembly
        i = 1
        inst2SR = {}
        pointsBySR = {}
        topLevelAsm = STR.upper(testBenchName) + '_ASM'
        for data in wtl:
            if 'CONTEXT_DEPENDENT_SHAPE_REPRESENTATION' in data:
                cdsrRE = c.findall(data)                    # grab all STEP pointers in CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
                rr = wtl[ld[cdsrRE[0]]]                     # find REPRESENTATION_RELATIONSHIP
                pds = wtl[ld[cdsrRE[1]]]                    # find PRODUCT_DEFINITION_SHAPE
                pdsRE = c.findall(pds)                      # grab all STEP pointers in PRODUCT_DEFINITION_SHAPE
                pdsRE2 = d.search(pds)                      # grab names of component and direct parent assembly
                if pdsRE2.group(1)[-4:] == '_ASM':          # if component is a subassembly:
                    if pdsRE2.group(2) in subAsms:              # and if parent assembly has entry in subAsms:
                        subAsms[pdsRE2.group(2)].append(
                            pdsRE2.group(1))                        # add subassembly to subAsms[parent]
                    else:                                       # if parent has no entry in subAsms yet:
                        subAsms.update([[pdsRE2.group(2),
                            [pdsRE2.group(1)]]])                    # create subAsms[parent]
                elif 'ZONE' not in pdsRE2.group(1):
                    if pdsRE2.group(2) in asmParts:
                        asmParts[pdsRE2.group(2)].append(pdsRE2.group(1))
                    else:
                        asmParts.update([[pdsRE2.group(2),[pdsRE2.group(1)]]])
                rrRE = c.findall(rr)                        # grab all STEP pointers in REPRESENTATION_RELATIONSHIP
                idt = wtl[ld[rrRE[2]]]                      # find ITEM_DEFINED_TRANSFORMATION
                idtRE = c.findall(idt)                      # grab all STEP pointers in ITEM_DEFINED_TRANSFORMATION
                inst2SR.update([[pdsRE2.group(1),rrRE[0]]]) # store SHAPE_RELATIONSHIP pointer, indexed by part/asm name
                axisA = wtl[ld[idtRE[0]]]                   # find first AXIS2_PLACEMENT_3D
                axisB = wtl[ld[idtRE[1]]]                   # find second AXIS2_PLACEMENT_3D
                axisARE = c.findall(axisA)                  # grab all STEP pointers in first AXIS2_PLACEMENT_3D
                axisBRE = c.findall(axisB)                  # grab all STEP pointers in second AXIS2_PLACEMENT_3D
                originA = coordinate(regexFriendly(
                    wtl[ld[axisARE[0]]]))                   # get CARTESIAN_POINT of axisA
                dirA1 = coordinate(regexFriendly(
                    wtl[ld[axisARE[1]]]))                   # get first DIRECTION of axisA
                dirA2 = coordinate(regexFriendly(
                    wtl[ld[axisARE[2]]]))                   # get second DIRECTION of axisA
                originB = coordinate(regexFriendly(
                    wtl[ld[axisBRE[0]]]))                   # get CARTESIAN_POINT of axisB
                dirB1 = coordinate(regexFriendly(
                    wtl[ld[axisBRE[1]]]))                   # get first DIRECTION of axisB
                dirB2 = coordinate(regexFriendly(
                    wtl[ld[axisBRE[2]]]))                   # get second DIRECTION of axisB
                localTMs.update([[pdsRE2.group(1),get3DTransformArray(
                    dirA1,dirA2,dirB1,dirB2)]])             # create transformation matrix from axisA to axisB
                localTVs.update([[
                    pdsRE2.group(1),originB]])              # create translation vector from axisA to axisB
            elif 'CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP' in data:
                cgrrRE = c.findall(data)                    # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP
                sr = cgrrRE[0]                              # STEP pointer for SHAPE_REPRESENTATION
                pointsBySR.update([[sr,{}]])                # new dict entry to store datums by SHAPE_REPRESENTATION pointer
                cgr = wtl[ld[cgrrRE[1]]]                    # get CONSTRUCTIVE_GEOMETRY_REPRESENTATION
                cgrRE = c.findall(cgr)                      # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION
                for datum in cgrRE[:-1]:                    # for each STEP pointer in CGR, excluding last one:
                    datumline = wtl[ld[datum]]                  # get datum entry at pointer
                    if 'CARTESIAN_POINT' in datumline:          # if datum entry is CARTESIAN_POINT:
                        regexable = regexFriendly(datumline)        # replace/remove problem characters in line
                        tagFind = re.search('%.+%',regexable)       # get name of datum
                        tag = tagFind.group(0).strip('%')           # remove % characters
                        coords = coordinate(regexable)              # get coordinate tuple
                        pointsBySR[sr].update([[tag,coords]])       # record datum info in SHAPE_REPRESENTATION dict

    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during parsing the STEP file\n')
        f.write('       Probably due to corrupted or erronoeus STEP raw data\n')
        raise 

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # new dict to index points by part name, rather than by SHAPE_REPRESENTATION pointer
    localCoords = {}
    CGs = {}
    try:
        for (key,value) in inst2SR.iteritems():
            localCoords.update([[key,pointsBySR[value]]])
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in creating the dictionary to index points by part name\n')
        f.write('       Probably due to corrupted or erronoeus STEP file\n')
        raise

    f.write('Grabbing the CG\'s of the parts from CADAssembly_metrics.xml file.\n')

    try:
        for component in metricComponentsXML.findall('MetricComponent'):
            if component.get('Type') == 'PART':
                componentName = component.get('Name').upper()
                centerOfGravity = component.find('CG')
                CGX = float(centerOfGravity.get('X'))
                CGY = float(centerOfGravity.get('Y'))
                CGZ = float(centerOfGravity.get('Z'))
                CGs.update([[componentName,(CGX, CGY, CGZ)]]) 

        for (key,value) in CGs.iteritems():
            localCoords[key].update([['CG_'+key,value]])

    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error in grabbing the CG of the parts from CADAssembly_metrics.xml file.\n')
        raise 
        
    ### rigidCenters = dict(MY_PART=(CG['x'],CG['y'],CG['z']))
    ### for (key,value) in rigidCenters.iteritems():
        ### localCoords[key].update([['CG_'+key,value]])
    # new dict with points translated to global coordinates
    f.write("Creating a new dictionary with points translated into the global coordinate system" + '\n')

    try:
        datumPointDict = coordTransform(localTMs,localTVs,
            topLevelAsm,subAsms,asmParts,localCoords)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during coordinate transformation\n')
        f.write('       The units of the model used might not be "mmKs"\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write('Importing parts into the model\n')

    f.write('\n')
    
    i = 0
    newPart = 'I love my job.'
    try:                                                                                    # STEP part import stage
        while newPart:                                                                      # while part exists:
            i += 1
            genericPartName = "part" + '-%d' % i
            newPart = myModel.PartFromGeometryFile(                                         # create part from file
                name=genericPartName,geometryFile=step,
                bodyNum=i,combine=False,dimensionality=THREE_D,
                type=DEFORMABLE_BODY,usePartNameFromFile=True)
		
    except Exception:                                                                       # if exception occurs:
        pass                                                                                # move along

    try:
        for key in myModel.parts.keys():                                                    # for each part in the model:
            currentPart = myModel.parts[key]                                                # get part object
            f.write('Importing part ' + str(key) + '\n')
            if len(currentPart.cells) == 0:                                                 # if part has no volume:
                del myModel.parts[key]                                                      # delete the part
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during the deletion of the invalid part\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write("Creating materials for the model" + '\n')

    f.write('\n')

    # create materials (rough)
    try:
    
        for (key,mtrl) in mtrlRef.iteritems():              # for each material described in mtrlRef:
            myMaterial = myModel.Material(name=key)             # declare a new Material object
            elasticP = (mtrl['mechanical__modulus_elastic'],
                mtrl['mechanical__ratio_poissons'])
            myMaterial.Elastic(table=(elasticP,))
            if 'density' in mtrl:
                densityP = (mtrl['density'],)
                myMaterial.Density(table=(densityP,))
            if 'HeatCapacity' in mtrl:
                heatCapP = (mtrl['HeatCapacity'],)
                myMaterial.SpecificHeat(table=(heatCapP,))
            if 'CoefficientOfThermalExpansion' in mtrl:
                expandP = (mtrl['CoefficientOfThermalExpansion'],)
                myMaterial.Expansion(table=(expandP,))
            if 'ThermalConductivity' in mtrl:
                conductP = (mtrl['ThermalConductivity'],)
                myMaterial.Conductivity(table=(conductP,))
            f.write("Creating material " + str(key) + '\n')
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during creating materials inside the model\n')
        raise

##    if rigidPartPresent:
##        myMaterial = myModel.Material(name='Rigid')
##        elasticP = (1e15,0.25)
##        densityP = (1e-10,)
##        myMaterial.Elastic(table=(elasticP,))
##        myMaterial.Density(table=(densityP,))        
    
    ### enable to generate ALL Creo datum points in Abaqus model
    # for part in datumPointDict.keys():
        # for (key,value) in datumPointDict[part].iteritems():
            # newDatum = myAsm.DatumPointByCoordinate(coords=datumPointDict[part][key])
            # datumGUIname = STR.join([part,'-',key])
            # myAsm.features.changeKey(fromName=newDatum.name,toName=datumGUIname)

    try:
        for (key,entry) in instRef.iteritems():
            sectionName = key
            if entry['isShell']:
                myModel.HomogeneousShellSection(name=sectionName,			# create shell section assignemtn- only used for shell parts
                    material=entry['MaterialID'],
                    thicknessType=UNIFORM, thickness=1.0, 
                    thicknessField='', idealization=NO_IDEALIZATION,
                    poissonDefinition=DEFAULT, thicknessModulus=None, 
                    temperature=GRADIENT, useDensity=OFF, integrationRule=SIMPSON, 
                    numIntPts=5)
    ##        elif entry['isRigid']:
    ##            myModel.HomogeneousSolidSection(name=sectionName,
    ##                material='Rigid')            
            else:
                myModel.HomogeneousSolidSection(name=sectionName,
                    material=entry['MaterialID'])
            if entry['isPart'] == False:                            # if component is a sub-assembly:
                instanceSet = []                                    # create container for parts in sub-assembly
                containedParts = tuple(asmRecursion(key,
                    subAsms,asmParts))                              # recursively find parts within sub-assembly
                for part in containedParts:                         # for each part in sub-assembly:
                    p = myModel.parts[part]
                    if entry['isShell']:
                        p.RemoveCells(cellList=p.cells)
                        p.checkGeometry()
                        region = (p.faces,)
                    else:
                        region = (p.cells,)
                    p.SectionAssignment(region=region,
                        sectionName=sectionName)                        # assign section to part
                    instanceSet.append(myAsm.Instance(name=part,
                        part=p,dependent=OFF))                          # create part instance
                ### redundant and causes issues if using part/assembly info from CADAssembly_metrics.xml
                subAsmInstance = myAsm.InstanceFromBooleanMerge(
                    name=key,instances=tuple(instanceSet),
                    originalInstances=DELETE,domain=GEOMETRY)       # merge part instances into one sub-assembly instance
                myAsm.makeIndependent(instances=(subAsmInstance,),) # make new instance independent of corresponding new part
                myAsm.instances.changeKey(fromName=subAsmInstance.name,
                    toName=subAsmInstance.name[:-2])                # remove "-1" tacked to end of instance name
            else:                                               # if component is a single part:
                p = myModel.parts[key]
                if entry['isShell']:
                    p.RemoveCells(cellList=p.cells)
                    p.checkGeometry()
                    region = (p.faces,)
                else:
                    region = (p.cells,)
                p.SectionAssignment(region=region,
                    sectionName=sectionName)                                            # assign section to part
                myAsm.Instance(name=key,dependent=OFF,part=p)                           # create instance from part
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during creating and assigining sections. Names in the STEP file, CADAssembly_metrics.xml and in Abaqus does not match\n')
        f.write('Possible causes of error are:\n')
        f.write('1- STEP file of one or more parts might be missing (Create Assembly program issue)\n')
        f.write('2- Part names generated by Create Assembly program is not unique (Create Assembly program issue)\n')
        f.write('3- Name/s of part/s might be invalid, try simplifying part names (i.e. part names starting with numbers, too long part names, etc.)\n')
        raise

    f.close()

    try:
        EliminateOverlaps(instRef,rigidParts,myAsm,myModel)
    except Exception as e:
        f.write('ERROR: An error has been encountered during the elimination of the overlapping parts\n')
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        raise

    f = open(logDir, "a")

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # define step (static, dynamic, modal)
    try:
        if args.dynamic:
            f.write("Defining dynamic step" + '\n')
            myStep = myModel.ImplicitDynamicsStep(name='dynamicLoading',
                previous='Initial',timePeriod=15.0,nlgeom=ON,maxNumInc=100000,
                initialInc=0.01,minInc=0.000001,maxInc=1)                               ### parameters may need to change
            myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
                'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
                frequency=1)
            myModel.TabularAmplitude(name='dynamicAmplitude',
                data=((0,0),(5,1),(10,1),(15,0)))
            amp = 'dynamicAmplitude'
            analysisStepName = 'dynamicLoading'

        elif args.modal:
            f.write("Defining modal analysis step" + '\n')
            myStep = myModel.FrequencyStep(name='modalAnalysis',
                previous='Initial',eigensolver=LANCZOS,numEigen=30)                     ### parameters may need to change
            myModel.TabularAmplitude(name='dummyAmplitude',
                data=((0,0),(1,1)))
            amp = 'dummyAmplitude'
            analysisStepName = 'modalAnalysis'
            
        else:
            f.write("Defining static step" + '\n')
            myStep = myModel.StaticStep(name='staticLoading',previous='Initial',
                timePeriod=1.0,nlgeom=ON,maxNumInc=100000,
                initialInc=0.01,minInc=0.000001)                                            ### parameters may need to change
            myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
                'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
                frequency=LAST_INCREMENT)                                                   # setting field variables for the Field Output Request
            myModel.TabularAmplitude(name='staticAmplitude',
                data=((0,0),(1,1)))
            amp = 'staticAmplitude'
            analysisStepName = 'staticLoading'
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during analysis step creation\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # define contact interactions
    f.write("Defining general contact interactions" + '\n')
    
    try:
        contactProp = myModel.ContactProperty('contact')
        contactProp.TangentialBehavior(
            formulation=FRICTIONLESS)
        contactProp.NormalBehavior(
            pressureOverclosure=HARD, allowSeparation=ON, 
            constraintEnforcementMethod=DEFAULT)
        myInteract = myModel.ContactStd(name='Int-1',
            createStepName='Initial')
        myInteract.includedPairs.setValuesInStep(
            stepName='Initial', useAllstar=ON)
        myInteract.contactPropertyAssignments.appendInStep(
            stepName='Initial', assignments=((GLOBAL, SELF, 'contact'), ))
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during creating general contact behavior\n')
        raise
		
#####################    Define Rigid Body Constraint
###  if part is flagged as rigid, make that part a rigid body
####  Rigid Bodies will results in no value for summary.  Summary will need to be updated to allow this.  Currently it prodeces an error.
    try:
        refPointKeys = []
        refPointLocation = []
        for (key,entry) in instRef.iteritems():
            sectionName = key
#           if entry['isRigid']:
            if entry['isRigid'] and Jan24_deactivate_rigidity:
                #regionMask = region.getMask()   
                instName = instIndex[entry['ComponentID']]
                myInstcell = myAsm.instances[instName].cells                          # get instance
                
                ### temporary, to be replaced by center of gravity from XML
                #p = myModel.parts['ComponentID']
                #point = myAsm.ReferencePoint(point=(0.0, 0.0, 0.0))
                            #cells1 =  myInstcell.getSequenceFromMask(mask=(regionMask,),)
                #partPosition = myModel.parts[sectionName].getMassProperties()
                #point = myAsm.ReferencePoint(partPosition['centerOfMass'])
                ###

                #point = myAsm.ReferencePoint(localCoords[sectionName]['CG_'+sectionName])
                point = myAsm.ReferencePoint(datumPointDict[sectionName]['CG_'+sectionName])
                
                cells1 = myInstcell.getSequenceFromMask(mask=('[#1 ]', ), )
                region2=regionToolset.Region(cells=cells1)
                #r1 = myAsm.instances['ComponentID'].referencePoints
                r1 = myAsm.referencePoints
                for point in r1.keys():
                    if not point in refPointKeys:
                        refPoints1=(r1[point],)
                        refPointKeys.append(point)
                        refPointLocation.append((key,point))
                        break
                region1=regionToolset.Region(referencePoints=(refPoints1,))
                rigidName = 'Rigid_%s' % key
                myModel.RigidBody(name=rigidName,refPointRegion=region1, 
                    bodyRegion=region2,refPointAtCOM=OFF)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during defining rigid body constraints\n')
        raise

###################################

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write("Creating loads and BC's for the model" + '\n')

    f.write('\n')

    try:
        if not args.noFEA:                                  # if load/BC assembly is enabled:
            # shellParts = []
            faceCount=0
            uniqueName=0
            
            for entry in loadBCLib:                             # for each load and/or BC definition:
                try:
                    instName = instIndex[entry['ComponentID']]
                except KeyError:
                    instName = instAssemblyIndex[entry['ComponentID']]
                partDatums = datumPointDict[instName]
                try:
                    myInst = myAsm.instances[instName]
                except:
                    myInst = None

                needsWork = "%s support still in progress. Oops!" % entry['GeometryType']
                if entry['GeometryType'] == 'POLYGON':
                    edgeIndex = []
                    entry.update([['Edges',[]]])
    #                for datumName in entry['Points']:
    #                    corner = None
    #                    for key in myAsm.instances.keys():
    #                        myInst = myAsm.instances[key]
    #                        corner = myInst.vertices.findAt(
    #                            coordinates=partDatums[datumName],printWarning=False)
    #                        if corner:
    #                            print corner.instanceName
    #                            entry.update([['Instance',key]])
    #                            break

                    for key in myAsm.instances.keys():
                        check = len(entry['Points'])
                        for datumName in entry['Points']:
                            myInst2 = myAsm.instances[key]                      
                            corner = myInst2.vertices.findAt(
                                coordinates=partDatums[datumName],printWarning=False)

                            if corner != None:
                                check = check - 1

                        if check == 0:
                            for datumName in entry['Points']:
                                myInst = myAsm.instances[key]
                                corner = myInst.vertices.findAt(
                                    coordinates=partDatums[datumName],printWarning=False)
                                entry.update([['Instance',key]])
                                edgeIndex.append(corner.getEdges())
                                faceName = key
                                
                    for i in range(len(edgeIndex)-1):
                        for edge in edgeIndex[i]:
                            for edgeSet in edgeIndex[i+1:]:
                                if edge in edgeSet:
                                    if edge not in entry['Edges']:
                                        entry['Edges'].append(edge)

                    faceIndex = []
                    for edge in entry['Edges']:
                        faceIndex.append(myInst.edges[edge].getFaces())
                    for face in faceIndex[0]:
                        try:
                            for faceSet in faceIndex[1:]:
                                if face not in faceSet:
                                    raise Exception
                            entry.update([['FaceIDs',(face,)]])
                            #entry.update([['RegionName','Face_'+repr(face)]])
                            entry.update([['RegionName','Face_'+faceName+str(faceCount)]])
                            faceCount=faceCount+1
                        except Exception:
                            pass
                        
    ##                edgeIndex = []                                      # create a temporary container for edges
    ##                entry.update([['Edges',[]]])                        # create a list of edges in the dictionary
    ##                for datumName in entry['Points']:
    ##                    corner = myInst.vertices.findAt(coordinates=partDatums[datumName])
    ##                    edgeIndex.append(corner.getEdges())                 # seq of IDs of edges on given vertex
    ##                for i in range(len(edgeIndex)-1):
    ##                    for edge in edgeIndex[i]:
    ##                        for edgeSet in edgeIndex[i+1:]:
    ##                            if edge in edgeSet:
    ##                                if edge not in entry['Edges']:
    ##                                    entry['Edges'].append(edge)
    ##                faceIndex = []                                      # create a temporary container for faces
    ##                for edge in entry['Edges']:                         # for each edge ID in the list:
    ##                    faceIndex.append(myInst.edges[edge].getFaces())     # collect IDs of all faces on that edge
    ##                for face in faceIndex[0]:                           # for each face in the first face set:
    ##                    try:                                                # a Pythonic improvement over first version...
    ##                        for faceSet in faceIndex[1:]:                       # for each of the other face sets:
    ##                            if face not in faceSet:                             # if the face is not in a set:
    ##                                raise Exception                                     # raise generic exception
    ##                        entry.update([['FaceIDs',(face,)]])                 # store the one common face
    ##                        entry.update([['RegionName','Face_'+repr(face)]])   # store a name string for the face
    ##                    except Exception:                                   # exception handling used...
    ##                        pass                                                # to skip ineligible faces
                elif entry['GeometryType'] == 'CYLINDER':           ### assumes particular order of points
                    cylTop = partDatums[entry['Points'][0]]
                    cylBottom = partDatums[entry['Points'][1]]
                    cylRadius = partDatums[entry['Points'][2]]
                    topDatum = myAsm.DatumPointByCoordinate(coords=cylTop)
                    botDatum = myAsm.DatumPointByCoordinate(coords=cylBottom)
                    surDatum = myAsm.DatumPointByCoordinate(coords=cylRadius)
                    v1 = array(cylTop)-array(cylBottom)
                    v2 = array(cylRadius)-array(cylBottom)
                    v1mag = LA.norm(v1)
                    v2mag = LA.norm(v2)
                    theta = acos(dot(v1,v2)/(v1mag*v2mag))
                    cylR = v2mag*sin(theta)                             # radius of cylinder
                    cylFaces = myInst.faces.getByBoundingCylinder(center1=cylTop,
                        center2=cylBottom,radius=cylR+args.separationTolerance)
                    innerFaces = myInst.faces.getByBoundingCylinder(
                        center1=cylTop,center2=cylBottom,radius=cylR)
                    for i in range(len(innerFaces)):
                        cylFaces.remove(innerFaces[i])
                    cylFaceIDs = []
                    for i in range(len(cylFaces)):
                        cylFaceIDs.append(cylFaces[i].index)
                    if entry['2ndGeomQual'] == 'ExcludeEndCaps':
                        diskTolVector = args.separationTolerance*(v1/v1mag)
                        diskTSideA = tuple(array(cylTop)+diskTolVector)
                        diskTSideB = tuple(array(cylTop)-diskTolVector)
                        diskBSideA = tuple(array(cylBottom)+diskTolVector)
                        diskBSideB = tuple(array(cylBottom)-diskTolVector)
                        topFace = myInst.faces.getByBoundingCylinder(center1=diskTSideA,
                            center2=diskTSideB,radius=cylR+args.separationTolerance)
                        botFace = myInst.faces.getByBoundingCylinder(center1=diskBSideA,
                            center2=diskBSideB,radius=cylR+args.separationTolerance)
                        try:
                            cylFaceIDs.remove(topFace[0].index)
                        except:
                            pass
                        try:
                            cylFaceIDs.remove(botFace[0].index)
                        except:
                            pass
                    entry.update([['FaceIDs',tuple(cylFaceIDs)]])
                    faceArrayName = 'Faces_%d--' % cylFaceIDs[0]
                    entry.update([['RegionName',faceArrayName]])
                    csysName = faceArrayName+'CSYS'
                    entry.update([['CSYS',csysName]])
                    myAsm.DatumCsysByThreePoints(coordSysType=CARTESIAN,
                        origin=topDatum,point1=botDatum,point2=surDatum,
                        name=csysName)                                  # coordinate system for pin constraints
                elif entry['GeometryType'] == 'CIRCLE':             ### assumes particular order of points
                    #circleCenter = partDatums[entry['Points'][0]]
                    #circleFaces = myInst.faces.findAt(
                    #    coordinates=circleCenter)                       ### shortcut (not rigorous) method
                    #entry.update([['FaceIDs',(circleFaces.index,)]])
                    #entry.update([['RegionName','Face_%d' % circleFaces.index]])
                    #entry.update([['Instance',None]])

                    for key in myAsm.instances.keys():
                        circleCenter = partDatums[entry['Points'][0]]
                        myInst2 = myAsm.instances[key]
                        circleFaces = myInst2.faces.findAt(
                            coordinates=circleCenter,printWarning=False)

                        if circleFaces:
                            myInst = myInst2
                            instance = key
                            entry.update([['FaceIDs',(circleFaces.index,)]])
                            entry.update([['RegionName','Face_%d' % uniqueName]])
                            entry.update([['Instance',key]])

                elif entry['GeometryType'] == 'FACE':                    ### needs just one point

                    
                    #circleCenter = partDatums[entry['Points'][0]]
                    #circleFaces = myInst.faces.findAt(
                    #    coordinates=circleCenter)                       ### shortcut (not rigorous) method
                    #entry.update([['FaceIDs',(circleFaces.index,)]])
                    #entry.update([['RegionName','Face_%d' % circleFaces.index]])
                    #entry.update([['Instance',None]])

                    for key in myAsm.instances.keys():
                        facePoint = partDatums[entry['Points'][0]]
                        myInst2 = myAsm.instances[key]
                        selectedFace = myInst2.faces.findAt(
                            coordinates=facePoint,printWarning=False)

                        if selectedFace:
                            myInst = myInst2
                            instance = key
                            entry.update([['FaceIDs',(selectedFace.index,)]])
                            entry.update([['RegionName','PointFace_%d' % uniqueName]])
                            entry.update([['Instance',key]])
                            selectedFaceEdges = selectedFace.getEdges()
                            selectedFaceEdge = myInst2.edges[selectedFaceEdges[0]].pointOn
                            selectedFaceEdgeCoord = selectedFaceEdge[0]
                              
                elif entry['GeometryType'] == 'CONCENTRIC_CIRCLES':
                    circleCenter = partDatums[entry['Points'][0]]
                    circleOuter = partDatums[entry['Points'][1]]
                    circleInner = partDatums[entry['Points'][2]]
                    innerR = LA.norm(array(circleInner)-array(circleCenter))
                    outerR = LA.norm(array(circleOuter)-array(circleCenter))
                    diskTolVector = args.separationTolerance*unv(circleCenter,circleInner,circleOuter)
                    diskSideA = tuple(array(circleCenter)+diskTolVector)
                    diskSideB = tuple(array(circleCenter)-diskTolVector)
                    outerFaces = myInst.faces.getByBoundingCylinder(
                        center1=diskSideA,center2=diskSideB,radius=args.separationTolerance+outerR) ### hard-coded tolerance
                    innerFaces = myInst.faces.getByBoundingCylinder(
                        center1=diskSideA,center2=diskSideB,radius=args.separationTolerance+innerR) ### hard-coded tolerance
                    outerFaceIDs = []
                    innerFaceIDs = []
                    for i in range(len(outerFaces)):
                        outerFaceIDs.append(outerFaces[i].index)
                    for i in range(len(innerFaces)):
                        innerFaceIDs.append(innerFaces[i].index)
                    faceCount = 0
                    for ID in outerFaceIDs:
                        if ID not in innerFaceIDs:
                            entry.update([['FaceIDs',(ID,)]])
                            entry.update([['RegionName','Face_'+uniqueName]])
                            faceCount += 1
                    if faceCount > 1:
                        f.write('\n')
                        f.write("ERROR: Uncommon concentric circle geometry, e.g. hemitoroid" + '\n')
                        raise ValueError("Uncommon concentric circle geometry, e.g. hemitoroid")
                elif entry['GeometryType'] == 'SPHERE':
                    sphereCenter = array(partDatums[entry['Points'][0]])
                    sphereRadial = array(partDatums[entry['Points'][1]])
                    sphereR = LA.norm(sphereRadial-sphereCenter)
                    rVector = unitV(sphereRadial-sphereCenter)
                    xVector = array((1,0,0))
                    if rVector == xVector or rVector == -xVector:
                        perpVector = unitV(cross(rVector,array((0,1,0))))
                    else:
                        perpVector = unitV(cross(rVector,xVector))
                    sphereOffset = tuple(sphereCenter + sphereR*perpVector)
                    sphereCenter = tuple(sphereCenter)
                    sphereRadial = tuple(sphereRadial)
                    ctrDatum = myAsm.DatumPointByCoordinate(coords=sphereCenter)
                    radDatum = myAsm.DatumPointByCoordinate(coords=sphereRadial)
                    offDatum = myAsm.DatumPointByCoordinate(coords=sphereOffset)
                    sphereFaces = myInst.faces.getByBoundingSphere(
                        center=sphereCenter,radius=args.separationTolerance+sphereR)
                    innerFaces = myInst.faces.getByBoundingSphere(
                        center=sphereCenter,radius=sphereR)
                    sphereFaceIDs = []
                    innerFaceIDs = []
                    for i in range(len(innerFaces)):
                        innerFaceIDs.append(innerFaces[i].index)
                    for i in range(len(sphereFaces)):
                        sphereFaceIDs.append(sphereFaces[i].index)
                    for index in sphereFaceIDs:
                        if index in innerFaceIDs:
                            sphereFaceIDs.remove(index)
                    entry.update([['FaceIDs',tuple(sphereFaceIDs)]])
                    entry.update([['RegionName','Face_%d' % uniqueName]])
                    faceArrayName = 'Faces_%d--' % sphereFaceIDs[0]
                    csysName = faceArrayName+'CSYS'
                    entry.update([['CSYS',csysName]])
                    myAsm.DatumCsysByThreePoints(coordSysType=CARTESIAN,
                        origin=ctrDatum,point1=radDatum,point2=offDatum,
                        name=csysName)
                else:
                    f.write('\n')
                    error = "ERROR: Geometry type %s not supported." % entry['GeometryType']
                    f.write(error)
                    raise ValueError(error)

                uniqueName = uniqueName + 1

                f.write("Preparing geometry for load/BC on part " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')   

            flatFaceTypes = ['CIRCLE','CONCENTRIC_CIRCLES','POLYGON']
            # create loads and BCs (very rough)
            for entry in loadBCLib:
                if entry['GeometryType'] in flatFaceTypes and entry['1stGeomQual'] != 'BoundaryOnly':
                    regionMask = myMask(entry['FaceIDs'])
                    try:
                        instName = instIndex[entry['ComponentID']]
                    except KeyError:
                        instName = instAssemblyIndex[entry['ComponentID']]
                    myInst = myAsm.instances[entry['Instance']]
                    partDatums = datumPointDict[instName]
                    maskRegion = myInst.faces.getSequenceFromMask(mask=(regionMask,),)
                    regionName = entry['RegionName']                            # get name of face(s)
                    region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
                    if entry['Treatment'] == 'Displacement':                    # if displacement condition:
                        (xt,yt,zt) = entry['TransDOF']
                        (xr,yr,zr) = entry['RotDOF']
                        myModel.DisplacementBC(name=regionName,
                            createStepName=myStep.name,region=(maskRegion,),
                            u1=xt,u2=yt,u3=zt,ur1=xr,ur2=yr,ur3=zr)                 # create displacement BC
                    elif entry['Treatment'] == 'Pressure':                      # if pressure condition:
                        if not args.modal:
                            pressureName = regionName + '-Pressure'
                            myModel.Pressure(name=pressureName,createStepName=myStep.name,
                                region=region,magnitude=entry['Pressure'],amplitude=amp)              # create pressure load
                    elif entry['Treatment'] == 'ForceMoment':                   # if distributed translation load:
                        if not args.modal:
                            vectorF = array(entry['Force'])                             ### moment is not supported yet
                            unitN = myInst.faces[entry['FaceIDs'][0]].getNormal()       # normal vector of face
                            pointA = array(partDatums[entry['Points'][0]])              # first point on perimeter
                            pointB = array(partDatums[entry['Points'][1]])              # second point on perimeter
                            vectorA = pointB-pointA                                     # vector between two points
                            unitA = vectorA/LA.norm(vectorA)                            # unit vector
                            unitB = cross(unitA, unitN)                                 # vector perpendicular to other unit vectors
                            faceArea = myInst.faces[entry['FaceIDs'][0]].getSize(
                                printResults=False)                                     # get face area to distribute force
                            tractionForce = unitA*dot(vectorF,unitA)+unitB*dot(
                                vectorF,unitB)                                          # planar component in terms of planar vectors
                            unitT = tractionForce/LA.norm(tractionForce)                # direction of planar force component
                            tupleA = tuple(pointA)                                      # first point in tuple form
                            tupleC = tuple(pointA+unitT)                                # arbitrary second point for planar force direction
                            pressureName = regionName + '-Normal'                       # name for normal component force
                            tractionName = regionName + '-Planar'                       # name for planar component force
                            pressureMag = float(-(dot(vectorF,unitN))/faceArea)         # magnitude of normal component force
                            tractionMag = float(LA.norm(tractionForce)/faceArea)        # magnitude of planar component force
                            if pressureMag != 0:                                        # if normal component is nonzero:
                                myModel.Pressure(name=pressureName,createStepName=myStep.name,
                                    region=region,magnitude=pressureMag,amplitude=amp)                    # apply pressure load
                            if tractionMag > 0:                                         # if planar component is nonzero:
                                myModel.SurfaceTraction(name=tractionName,
                                    createStepName=myStep.name,region=region,
                                    magnitude=tractionMag,directionVector=(tupleA,tupleC),
                                    distributionType=UNIFORM,traction=GENERAL,
                                    follower=OFF,resultant=ON,amplitude=amp)                              # apply traction load

                if entry['GeometryType'] == 'FACE':
                    regionMask = myMask(entry['FaceIDs'])
                    try:
                        instName = instIndex[entry['ComponentID']]
                    except KeyError:
                        instName = instAssemblyIndex[entry['ComponentID']]
                    myInst = myAsm.instances[entry['Instance']]
                    partDatums = datumPointDict[instName]
                    maskRegion = myInst.faces.getSequenceFromMask(mask=(regionMask,),)
                    regionName = entry['RegionName']                            # get name of face(s)
                    region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
                    if entry['Treatment'] == 'Displacement':                    # if displacement condition:
                        (xt,yt,zt) = entry['TransDOF']
                        (xr,yr,zr) = entry['RotDOF']
                        myModel.DisplacementBC(name=regionName,
                            createStepName=myStep.name,region=(maskRegion,),
                            u1=xt,u2=yt,u3=zt,ur1=xr,ur2=yr,ur3=zr)                 # create displacement BC
                    elif entry['Treatment'] == 'Pressure':                      # if pressure condition:
                        if not args.modal:
                            pressureName = regionName + '-Pressure'
                            myModel.Pressure(name=pressureName,createStepName=myStep.name,
                                region=region,magnitude=entry['Pressure'],amplitude=amp)              # create pressure load
                    elif entry['Treatment'] == 'ForceMoment':                   # if distributed translation load:
                        if not args.modal:
                            vectorF = array(entry['Force'])                             ### moment is not supported yet
                            unitN = myInst.faces[entry['FaceIDs'][0]].getNormal()       # normal vector of face
                            pointA = array(partDatums[entry['Points'][0]])              # first point on perimeter
                            pointB = selectedFaceEdgeCoord                              # second point on perimeter
                            vectorA = pointB-pointA                                     # vector between two points
                            unitA = vectorA/LA.norm(vectorA)                            # unit vector
                            unitB = cross(unitA, unitN)                                 # vector perpendicular to other unit vectors
                            faceArea = myInst.faces[entry['FaceIDs'][0]].getSize(
                                printResults=False)                                     # get face area to distribute force
                            tractionForce = unitA*dot(vectorF,unitA)+unitB*dot(
                                vectorF,unitB)                                          # planar component in terms of planar vectors
                            unitT = tractionForce/LA.norm(tractionForce)                # direction of planar force component
                            tupleA = tuple(pointA)                                      # first point in tuple form
                            tupleC = tuple(pointA+unitT)                                # arbitrary second point for planar force direction
                            pressureName = regionName + '-Normal'                       # name for normal component force
                            tractionName = regionName + '-Planar'                       # name for planar component force
                            pressureMag = float(-(dot(vectorF,unitN))/faceArea)         # magnitude of normal component force
                            tractionMag = float(LA.norm(tractionForce)/faceArea)        # magnitude of planar component force
                            if pressureMag != 0:                                        # if normal component is nonzero:
                                myModel.Pressure(name=pressureName,createStepName=myStep.name,
                                    region=region,magnitude=pressureMag,amplitude=amp)                    # apply pressure load 
                            if tractionMag > 0:                                         # if planar component is nonzero:
                                try:
                                    myModel.SurfaceTraction(name=tractionName,
                                        createStepName=myStep.name,region=region,
                                        magnitude=tractionMag,directionVector=(tupleA,tupleC),
                                        distributionType=UNIFORM,traction=GENERAL,
                                        follower=OFF,resultant=ON,amplitude=amp)                            # apply traction load
                                except:
                                    f.write('ERROR: Can not apply traction component of the force to the surface.\n')
                                    pass

                elif entry['GeometryType'] == 'CYLINDER' and entry['1stGeomQual'] == 'BoundaryOnly':
                    # initial region stuff
                    regionMask = myMask(entry['FaceIDs'])
                    instName = instIndex[entry['ComponentID']]
                    myInst = myAsm.instances[instName]
                    partDatums = datumPointDict[instName]
                    maskRegion = myInst.faces.getSequenceFromMask(mask=(regionMask,),)
                    regionName = entry['RegionName']
                    region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
                    if entry['Treatment'] == 'Pin':                             # if pin condition:
                        myModel.DisplacementBC(name=regionName,createStepName=myStep.name,
                            region=(maskRegion,),u1=entry['AxialDisp'],u2=SET,u3=SET,
                            ur1=entry['AxialRot'],ur2=UNSET,ur3=UNSET,
                            localCsys=myAsm.features[entry['CSYS']])                # define pin constraint
                    elif entry['Treatment'] == 'Pressure':
                        if not args.modal:
                            myModel.Pressure(name=pressureName,createStepName=myStep.name,
                                region=region,magnitude=entry['Pressure'],amplitude=amp)
                    elif entry['Treatment'] == 'Displacement':
                        myModel.DisplacementBC(name=regionName,createStepName=myStep.name,
                            region=(maskRegion,),u1=SET,u2=SET,u3=SET,ur1=SET,ur2=SET,ur3=SET,
                            localCsys=myAsm.features[entry['CSYS']])
                elif entry['GeometryType'] == 'SPHERE' and entry['1stGeomQual'] == 'BoundaryOnly':
                    regionMask = myMask(entry['FaceIDs'])
                    instName = instIndex[entry['ComponentID']]
                    myInst = myAsm.instances[instName]
                    partDatums = datumPointDict[instName]
                    maskRegion = myInst.faces.getSequenceFromMask(mask=(regionMask,),)
                    regionName = entry['RegionName']
                    region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
                    if entry['Treatment'] == 'Ball':
                        myModel.DisplacementBC(name=regionName,createStepName=myStep.name,
                            region=(maskRegion,),u1=SET,u2=SET,u3=SET,ur1=UNSET,ur2=UNSET,
                            ur3=UNSET,localCsys=myAsm.features[entry['CSYS']])
                    elif entry['Treatment'] == 'Pressure':
                        myModel.Pressure(name=pressureName,createStepName=myStep.name,
                            region=region,magnitude=entry['Pressure'],amplitude=amp)
                    elif entry['Treatment'] == 'Displacement':
                        myModel.DisplacementBC(name=regionName,createStepName=myStep.name,
                            region=(maskRegion,),u1=SET,u2=SET,u3=SET,ur1=SET,ur2=SET,ur3=SET,
                            localCsys=myAsm.features[entry['CSYS']])
        
                f.write("Creating load/BC " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')   
                
            if accel['x'] or accel['y'] or accel['z']:
                if not args.modal:
                    #myModel.Gravity(name='Gravity_Load',
                    #    createStepName=myStep.name,comp1=accel['x'],
                    #    comp2=accel['y'],comp3=accel['z'])
                    for (key,entry) in instRef.iteritems():
                        loadName = str(key)
                        myRegion = (myAsm.instances[key].cells,)
#                        if not entry['isRigid']:
                        if not Jan24_deactivate_rigidity:
                            myModel.Gravity(name='Accel_'+loadName,
                                createStepName=myStep.name,region=myRegion,
                                comp1=(accel['x']),comp2=(accel['y']),comp3=(accel['z']),amplitude=amp)
                        else:
                            for refPointNum in myAsm.referencePoints.keys():
                                for refPointLocNumAndName in refPointLocation:
                                    if refPointNum == refPointLocNumAndName[1]:
                                        for rigidPartMassNumAndName in rigidPartMasses:
                                            if rigidPartMassNumAndName[0] == refPointLocNumAndName[0] and rigidPartMassNumAndName[0] == loadName:
                                                myRefPointRegion = (myAsm.referencePoints[refPointNum],)
                                                mass = float(rigidPartMassNumAndName[1])
                                                AccelX = accel['x']
                                                AccelY = accel['y']
                                                AccelZ = accel['z']
                                                ForceX = mass*AccelX
                                                ForceY = mass*AccelY
                                                ForceZ = mass*AccelZ
                                                myModel.ConcentratedForce(name='Rigid_Accel_'+loadName,region=myRefPointRegion,
                                                    createStepName=myStep.name,cf1=ForceX,cf2=ForceY,cf3=ForceZ,amplitude=amp)
                    f.write("Creating acceleration\n")

    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during defining loads and BCs\n')
        f.write('       Check your testbench and CAD model for any invalidities, misplaced points or illegal use of CyPhy constructs\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # define global element size as distance between opposite corners of assembly bounding box
    f.write("Meshing the model" + '\n')
    f.write("Mesh size will be determined by using the distance between opposite corners of each parts bounding box" + '\n')

    try:
        boxCorner1x = []
        boxCorner1y = []
        boxCorner1z = []
        boxCorner2x = []
        boxCorner2y = []
        boxCorner2z = []
        for key in myAsm.instances.keys():
            myInst = myAsm.instances[key]
            region = myInst.faces
            regionBox = region.getBoundingBox()
            boxCorner1x.append(regionBox['low'][0])
            boxCorner1y.append(regionBox['low'][1])
            boxCorner1z.append(regionBox['low'][2])
            boxCorner2x.append(regionBox['high'][0])
            boxCorner2y.append(regionBox['high'][1])
            boxCorner2z.append(regionBox['high'][2])
        gbc1x = min(boxCorner1x)
        gbc1y = min(boxCorner1y)
        gbc1z = min(boxCorner1z)
        gbc2x = max(boxCorner2x)
        gbc2y = max(boxCorner2y)
        gbc2z = max(boxCorner2z)
        globalBoxCorner1 = array([gbc1x,gbc1y,gbc1z])
        globalBoxCorner2 = array([gbc2x,gbc2y,gbc2z])
        globalSize = LA.norm(globalBoxCorner2-globalBoxCorner1)/args.edgeSeedDensity

        # mesh instances
        f.write("Meshing each part" + '\n')
        f.write('\n')
        for key in myAsm.instances.keys():
            myInst = myAsm.instances[key]
            entry = instRef[key]
            if entry['isShell']:
                region = myInst.faces
            else:
                region = myInst.cells
            if args.globalSeeds:
                seedSize = globalSize
            else:
                regionBox = region.getBoundingBox()
                boxCorner1 = array(regionBox['low'])
                boxCorner2 = array(regionBox['high'])
                seedSize = LA.norm(boxCorner2-boxCorner1)/args.edgeSeedDensity
                f.write(str(key) + " element size: " +str(seedSize) + " " + str(unitShort) + "\n")
            # regionPick = region.getSequenceFromMask(mask=(myMask(0),),)    ### for older Abaqus versions
            regionMask = region.getMask()                               # preferred for Abaqus 6.13
            regionPick = region.getSequenceFromMask(mask=regionMask)
            #seedSize = globalSize
            if entry['isShell']:
                elemType = mesh.ElemType(elemCode=S4R,elemLibrary=STANDARD)
                myAsm.setMeshControls(regions=regionPick,elemShape=QUAD_DOMINATED,technique=FREE,
                    sizeGrowthRate=args.sizeGrowthRate)
                myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType,))
                seedSize = seedSize/2
            else:
                elemType = mesh.ElemType(elemCode=C3D10,elemLibrary=STANDARD)
                myAsm.setMeshControls(regions=regionPick,elemShape=TET,technique=FREE,
                    sizeGrowthRate=args.sizeGrowthRate)
                myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType,))
            if args.seedDevFactor and args.seedMinSize:
                myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER,
                    deviationFactor=args.seedDevFactor,minSizeFactor=args.seedMinSize)
            elif args.seedDevFactor:
                myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER,
                    deviationFactor=args.seedDevFactor)
            elif args.seedMinSize:
                myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER,
                    minSizeFactor=args.seedMinSize)
            else:
                myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER)
            myAsm.generateMesh(regions=(myInst,),meshTechniqueOverride=ON,
                seedConstraintOverride=ON)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during seeding and meshing\n')
        f.write('       Unmeshable parts or parts with invalid geometry might be present\n')
        raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write("Connecting parts" + '\n')
    f.write('\n')
    
    try:	
        myModel.contactDetection(name='CC-',defaultType=TIE,
            includeCylindricalSphericalToric=ON,includePlanar=OFF,
            surfaceSmoothing=NONE,useShellThickness=OFF,extendByAngle=None,
            separationTolerance=args.separationTolerance)
        myModel.contactDetection(name='CP-',defaultType=TIE,
            includeCylindricalSphericalToric=OFF,includePlanar=ON,
            surfaceSmoothing=NONE,useShellThickness=OFF,extendByAngle=None,
            separationTolerance=args.separationTolerance)
        for key in myModel.constraints.keys():
            f.write("Creating the constraint: " + str(key) + ' ' + '\n')
            if key[:5] == 'Rigid':
                continue
            myModel.constraints[key].setValues(adjust=OFF)
            if key[0:3] == 'CC-':
                myModel.constraints[key].setValues(tieRotations=OFF)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during defining tie constraints\n')
        raise

    try:
        for key in myModel.constraints.keys():
            if key.startswith("Rigid_"):
                instName = key.replace("Rigid_","")
                setName = "Set_" + instName
                instFace = myAsm.instances[instName].faces
                yourSet = myAsm.Set(name=setName, faces=instFace)

                allNodes = []
                surfNodes = []

                for instNodes in myAsm.instances[instName].nodes:
                    allNodes.append(instNodes.label)
            
                for setNodes in myAsm.sets[setName].nodes:
                    surfNodes.append(setNodes.label)

                interNodes = tuple(set(allNodes)-set(surfNodes))
                interNodeObjects = myAsm.instances[instName].nodes.sequenceFromLabels(labels=interNodes)
                myRegion = regionToolset.Region(nodes=interNodeObjects)
                myModel.constraints[key].setValues(bodyRegion=None,tieRegion=myRegion)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during defining rigid body constraints\n')
        raise


######################################### Create solid to shell coupling 		
    
##    tielist=[]
##    shelllist=[]
##    shelllistA=[]
##    for cons in myModel.constraints.keys():         # for each constraint:
##        if cons[:3] == 'SS-':                           # if constraint is shell-solid (i.e. not tie):
##            continue                                        # skip the constraint
##        myTie = myModel.constraints[cons]               # get tie constraint object
##        p1key = myTie.master[0]                         # get name of master Surface
##        p1keysub = p1key.split('-')[2]
##        p2key = myTie.slave[0]                          # get name of slave Surface
##        p2keysub = p2key.split('-')[2]
##        p1type = instRef[p1keysub]['isShell']           # true if master Surface is on a shell part
##        p2type = instRef[p2keysub]['isShell']           # true if slave Surface is on a shell part
##        if xor(p1type,p2type):
##            #del myModel.constraints[cons]
##            if p1type:
##                shellEdges = myAsm.instances[p1keysub].edges
##                solidFaces = myAsm.surfaces[p2key].faces
##            else:
##                shellEdges = myAsm.instances[p2keysub].edges
##                solidFaces = myAsm.surfaces[p1key].faces
##            surface1 = myAsm.Surface(side1Edges=shellEdges,name=cons+"-Edges")
##            surface2 = myAsm.Surface(side1Faces=solidFaces,name=cons+"-Faces")
##            ssname = 'SS-'+cons[3:]
##            ssnameonly = cons[5:]
##            shelllist.append(ssnameonly)
##            shelllistA.append(ssname)
##            tielist.append(myTie.name) 
##            myModel.ShellSolidCoupling(name=ssname,	shellEdge=surface1,
##                solidFace=surface2, positionToleranceMethod=SPECIFIED, 
##                positionTolerance=0.0001)
##    if tielist:         	
##        for tsp in tielist[1:]:
##           del myModel.constraints[tsp]				
##        myModel.constraints[tielist[0]].suppress()

########Print pictures of load and BCs

    f.close()

    try:
        PreProcessFileName = 'PreProcessing'
        CreateLOADSBCPNG(PreProcessFileName,myAsm,analysisStepName)
    except:
        f.write('ERROR: Error during creating BC PNG files\n')
        f.write('       Program will keep executing\n')
        pass

    f = open(logDir, "a")
	
##########################################################	Print pictures of load and BCs

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    try:
        f.write("Checking the model for possible constraint errors (1st check)." + '\n')
        f.write('\n')
        dataCheckJobName = 'DataCheck'
        myDataCheckJob = mdb.Job(name=dataCheckJobName, model=myModel.name,
            description='1st Data Check job',multiprocessingMode=DEFAULT,
            parallelizationMethodExplicit=DOMAIN,numCpus=args.parallelCores,
            numDomains=args.parallelCores)
        myDataCheckJob.submit(datacheckJob=True)
        myDataCheckJob.waitForCompletion()
        fControl = 's'
        f.write("1st data check completed with success" + '\n')
        f.write('\n')
    except:
        fControl = 'f'
        f.write("1st data check failed" + '\n')
        f.write("Constraint issues might be present" + '\n')
        f.write('\n')

    #problemConstraints = defaultdict(list)

    problemConstraints = []

    if fControl == 'f':
        f.write("Modifying constraints" + '\n')
        f.write('\n')
        jobName = 'DataCheck'
        odb = odbAccess.openOdb(path=jobName + '.odb')

        try:
            errorNodes = odb.rootAssembly.nodeSets['ErrNodeOverconTieSlave']

            for key in errorNodes.instanceNames:
                stringKey = str(key)
                for surf in myModel.rootAssembly.allSurfaces.keys():
                    surfKey = str(surf)
                    if stringKey in surfKey:
                        CPswitchKey = surfKey.replace(stringKey,"").strip()
                        for swap in myModel.constraints.keys():
                            swapInstance = str(swap)
                            if swap.startswith(CPswitchKey) == True:
                                f.write("Modifying constraint " + str(swap) + '\n')
                                myModel.constraints[swap].swapSurfaces()
                                problemConstraints.append(CPswitchKey)

        except:
            f.write("Cannot modifying constraints" + '\n')
            f.write("Turning ON the option for adjusting initial surface positions and specifying initial clearances" + '\n')
            f.write('\n')
            for key in myModel.constraints.keys():
                if key[:5] == 'Rigid':
                    continue
                myModel.constraints[key].setValues(adjust=ON)
                if key[0:3] == 'CC-':
                    myModel.constraints[key].setValues(tieRotations=OFF)

        try:
            f.write('\n')
            f.write("**********************************************************************************" + '\n')
            f.write('\n')
            f.write("Checking the model for possible constraint errors (2nd check)." + '\n')
            f.write('\n')
            dataCheckJobName2 = 'DataCheck2'
            myDataCheckJob2 = mdb.Job(name=dataCheckJobName2, model=myModel.name,
                description='2nd Data Check job',multiprocessingMode=DEFAULT,
                parallelizationMethodExplicit=DOMAIN,numCpus=args.parallelCores,
               numDomains=args.parallelCores)
            myDataCheckJob2.submit(datacheckJob=True)
            myDataCheckJob2.waitForCompletion()
            f.write("2nd data check completed with success" + '\n')
            f.write('\n')

        except:
            f.write("2nd data check failed" + '\n')
            f.write('\n')
            jobName2 = 'DataCheck2'
            odb2 = odbAccess.openOdb(path=jobName2 + '.odb')
            errorNodeCheck = False
            overlapCheck = False
            try:
                errorNodes = odb2.rootAssembly.nodeSets['ErrNodeOverconTieSlave']
                errorNodeCheck = True
                f.write("Constraint issues might be present" + '\n')
                f.write("Modifying constraints" + '\n')
            except:
                pass
            try:
                overlapCheck = odb2.rootAssembly.elementSets['ErrElemVolSmallNegZero']
            except:
                pass
            if errorNodeCheck:
                for key in errorNodes.instanceNames:
                    stringKey = str(key)
                    for surf in myModel.rootAssembly.allSurfaces.keys():
                        surfKey = str(surf)
                        if stringKey in surfKey:
                            f.write("Modifying nodes on surface " + str(surfKey) + " on instance " + str(stringKey)+ '\n')
                            CPswitchKey = surfKey.replace(stringKey,"").strip()
                            for swap in myModel.constraints.keys():
                                swapInstance = str(swap)
                                if swap.startswith(CPswitchKey) == True:
                                    if CPswitchKey in problemConstraints:
                                        f.write("Modifying constraint " + str(swap) + '\n')
                                        myModel.constraints[swap].swapSurfaces()
            elif overlapCheck:
                f.write("There are elements in the model with zero or negative volume" + '\n')
                f.write("Possibly parts are overlapping each other excessively" + '\n')
                f.write("Unable to run the analyis, please fix your CAD model" + '\n')
                f.write("Terminating" + '\n')
                f.write('\n')
                try:
                    f.close()
                    CreateOverlapPNG(jobName2,overlapCheck)
                    f = open(logDir, "a")
                    f.write("Check \"results/abcdef/Analysis/Abaqus/Contour_and_BC_plots/Overlapping_Elements_#.png\" files to determine problematic parts/elements" + '\n')
                except:
                    pass
                raise
            else:
                f.write("There might be some unknown issues present with the model" + '\n')
                f.write("Terminating" + '\n')
                f.write('\n')
                raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
                                        
    try:
        f.write("Checking the model for possible constraint errors (final check)." + '\n')
        f.write('\n')
        dataCheckJobName3 = 'DataCheckFinal'
        myDataCheckJob3 = mdb.Job(name=dataCheckJobName3, model=myModel.name,
            description='Final Data Check job',multiprocessingMode=DEFAULT,
            parallelizationMethodExplicit=DOMAIN,numCpus=args.parallelCores,
            numDomains=args.parallelCores)
        myDataCheckJob3.submit(datacheckJob=True)
        myDataCheckJob3.waitForCompletion()
        f.write("Final data check completed with success" + '\n')
        f.write('\n')

    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write("Final data check failed" + '\n')
        f.write('\n')
        jobName3 = 'DataCheckFinal'
        odb3 = odbAccess.openOdb(path=jobName3 + '.odb')
        overlapCheck = False
        try:
            overlapCheck = odb3.rootAssembly.elementSets['ErrElemVolSmallNegZero']
        except:
            pass
        if overlapCheck:
            f.write("There are elements in the model with zero or negative volume" + '\n')
            f.write("Possibly parts are overlapping each other excessively" + '\n')
            f.write("Unable to run the analyis, please fix your CAD model" + '\n')
            f.write("Terminating" + '\n')
            f.write('\n')
            try:
                f.close()
                CreateOverlapPNG(jobName3,overlapCheck)
                f = open(logDir, "a")
                f.write("Check \"results/abcdef/Analysis/Abaqus/Overlapping_Elements_#.png\" files to determine problematic parts/elements" + '\n')
            except:
                pass
            raise
        else:
            f.write("There might be some unknown issues present with the model" + '\n')
            f.write("Terminating" + '\n')
            f.write('\n')
            raise

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    f.write('Creating CAE file\n')

    try:     
        if args.saveCAE and not args.noFEA:                                     # if .cae file is requested:
            caePath = os.path.join(root,testBenchName) + '.cae'
            mdb.saveAs(pathName=caePath)                                        # save it as <testBenchName>.cae
            caeCopyName = 'AbaqusMeshAndAssembly.cae'
            shutil.copy2(testBenchName + '.cae', caeCopyName)
        elif args.noFEA:
            caePath = os.path.join(root,testBenchName) + '.cae'
            mdb.saveAs(pathName=caePath)                                        # save it as <testBenchName>.cae
            caeCopyName = 'AbaqusMesh.cae'
            shutil.copy2(testBenchName + '.cae', caeCopyName)
        else:
            pass
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during saving the CAE file\n')
        raise

    f.write('Creating' + str(caeCopyName) + '\n')
    f.write('\n')

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')

    # export to Nastran deck
    f.write("Exporting the Nastran deck" + '\n')

    try:
        mdb.Job(activateLoadBalancing=False, atTime=None, contactPrint=OFF, 
            description='', echoPrint=OFF, explicitPrecision=SINGLE, 
            getMemoryFromAnalysis=True, historyPrint=OFF, memory=90, memoryUnits=
            PERCENTAGE, model='Model-1', modelPrint=OFF, multiprocessingMode=DEFAULT, 
            name='NastranModel', nodalOutputPrecision=SINGLE, numCpus=1, numDomains=
            1, parallelizationMethodExplicit=DOMAIN, queue=None, scratch='', type=
            ANALYSIS, userSubroutine='', waitHours=0, waitMinutes=0)
        mdb.jobs['NastranModel'].writeNastranInputFile(consistencyChecking=OFF)
    except Exception as e:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Error during exporting the Nastran deck\n')
        raise

    f.write("Exporting NastranModel.bdf" + '\n')
    f.write('\n')

    if args.modal or args.dynamic or args.standard:

        if args.modal:
            jobName = 'modalAnalysis'
        elif args.dynamic:
            jobName = 'dynamicFEAAnalysis'
        elif args.standard:
            jobName = 'staticFEAAnalysis'  

    if args.adaptive:

        f.write('\n')
        f.write("**********************************************************************************" + '\n')
        f.write('\n')
        
        f.write("Creating and submitting the adaptive meshing process" + '\n')
        f.write('\n')
        
        try:
            j = job.Job(model=myModel.name, type=ANALYSIS,
                description='Adaptive mesh', multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores, numDomains=args.parallelCores,
                numGPUs=0, queue=None, memory=args.ramAllocated,
                memoryUnits=eval(args.memoryUnits), echoPrint=OFF, 
                modelPrint=OFF, contactPrint=OFF, historyPrint=OFF, 
                userSubroutine='', scratch='', explicitPrecision=SINGLE, 
                nodalOutputPrecision=SINGLE)
            processName = 'Adaptivity-1'
            mdb.AdaptivityProcess(jobPrefix='',job=j,name=processName,
                maxIterations=args.meshIterations)
            instList = myAsm.instances.keys()
            for i in range(len(myAsm.instances)):
                ruleName = 'RemeshingRule-%d' % i
                myInst = myAsm.instances[instList[i]]
                myInstName = myInst.name
                if rigidPartPresent:
                    if myInstName in rigidParts:
                        pass
                    else:
                        setName = 'Set-%d' % i
                        if instRef[instList[i]]['isShell']:
                            f1Mask = myInst.faces.getMask()
                            f1 = myInst.faces.getSequenceFromMask(mask=f1Mask)
                            region = myAsm.Set(faces=f1,name=setName)
                        else:
                            c1Mask = myInst.cells.getMask()                     # preferred for Abaqus 6.13
                            c1 = myInst.cells.getSequenceFromMask(mask=c1Mask)
                            region = myAsm.Set(cells=c1,name=setName)
                        mdb.models['Model-1'].RemeshingRule(name=ruleName, 
                            region=region, description='Remesh rule from element energy error indicator', 
                            stepName=analysisStepName, outputFrequency=LAST_INCREMENT, sizingMethod=DEFAULT,
                            variables=(args.errorIndicator,), errorTarget=0.0, coarseningFactor=NOT_ALLOWED,
                            maxSolutionErrorTarget=0.0, minSolutionErrorTarget=0.0, specifyMinSize=False,
                            specifyMaxSize=False, elementCountLimit=None, refinementFactor=DEFAULT_LIMIT)
                else:
                    setName = 'Set-%d' % i
                    if instRef[instList[i]]['isShell']:
                        f1Mask = myInst.faces.getMask()
                        f1 = myInst.faces.getSequenceFromMask(mask=f1Mask)
                        region = myAsm.Set(faces=f1,name=setName)
                    else:
                        c1Mask = myInst.cells.getMask()                     # preferred for Abaqus 6.13
                        c1 = myInst.cells.getSequenceFromMask(mask=c1Mask)
                        region = myAsm.Set(cells=c1,name=setName)
                    mdb.models['Model-1'].RemeshingRule(name=ruleName, 
                        region=region, description='Remesh rule from Mises stress error indicator', 
                        stepName=analysisStepName, outputFrequency=LAST_INCREMENT, sizingMethod=DEFAULT,
                        variables=(args.errorIndicator,), errorTarget=0.0, coarseningFactor=NOT_ALLOWED,
                        maxSolutionErrorTarget=0.0, minSolutionErrorTarget=0.0, specifyMinSize=False,
                        specifyMaxSize=False, elementCountLimit=None, refinementFactor=DEFAULT_LIMIT)
                    
            lastJobName = processName + '-iter' + repr(args.meshIterations)
        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error during creating and submitting the adaptive meshing process\n')
            raise
     
        mdb.adaptivityProcesses[processName].submit(waitForCompletion=True)
        f.close()
        afterJob(lastJobName,superRef,metricRef2,metricRef3,allAssembly,analysisStepName)
    elif args.standard:
        # create a basic analysis job and submit it

        f.write('\n')
        f.write("**********************************************************************************" + '\n')
        f.write('\n')
        
        f.write("Creating the static analysis job and submitting it" + '\n')
        f.write('\n')
        
        try:
            myJob = mdb.Job(name=jobName, model=myModel.name,
                description='Static FEA job',multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores,numDomains=args.parallelCores,
                memory=args.ramAllocated,memoryUnits=eval(args.memoryUnits),
                parallelizationMethodExplicit=DOMAIN)
##            myJob.writeInput(consistencyChecking=OFF)
        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error during creating and submitting the standard nonadaptive analysis process\n')
            raise

        myJob.submit()
        myJob.waitForCompletion()
        f.close()
        afterJob(jobName,superRef,metricRef2,metricRef3,allAssembly,analysisStepName)
        
    elif args.dynamic:
        # create a basic analysis job and submit it

        f.write('\n')
        f.write("**********************************************************************************" + '\n')
        f.write('\n')
        
        f.write("Creating the dynamic analysis job and submitting it" + '\n')

        f.write('\n')
        
        try:
            myJob = mdb.Job(name=jobName, model=myModel.name,
                description='Dynamic FEA job',multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores,numDomains=args.parallelCores,
                memory=args.ramAllocated,memoryUnits=eval(args.memoryUnits),
                parallelizationMethodExplicit=DOMAIN)
##            myJob.writeInput(consistencyChecking=OFF)
        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error during creating and submitting the dynamic analysis process\n')
            raise

        myJob.submit()
        myJob.waitForCompletion()
        f.close()
        afterJob(jobName,superRef,metricRef2,metricRef3,allAssembly,analysisStepName)
        
    elif args.modal:
        # create a modal analysis job and submit it

        f.write('\n')
        f.write("**********************************************************************************" + '\n')
        f.write('\n')
        
        f.write("Creating the modal analysis job and submitting it" + '\n')
        f.write('\n')
        
        try:
            myJob = mdb.Job(name=jobName, model=myModel.name,
                description='Modal Analysis job',multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores,numDomains=args.parallelCores,
                memory=args.ramAllocated,memoryUnits=eval(args.memoryUnits),
                parallelizationMethodExplicit=DOMAIN)
##            myJob.writeInput(consistencyChecking=OFF)
        except Exception as e:
            f.write(STR.join(traceback.format_exception(*sys.exc_info())))
            f.write('ERROR: Error during creating and submitting the modal analysis process\n')
            raise

        myJob.submit()
        myJob.waitForCompletion()
        f.close()
        afterJobModal(jobName,analysisStepName)

if __name__ == "__main__":
    main()
