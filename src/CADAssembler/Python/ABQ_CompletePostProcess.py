#title           :ABQ_CompletePostProcess.py
#description     :This script performs post processing on Abaqus output database
#                 files (.odb).
#author          :Di Yao
#date            :2013-09-06
#version         :1.0.3.0
#usage           :python pyscript.py
#notes           :
#python_version  :2.7
#==============================================================================

import os
import sys
import _winreg
import logging

with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
    META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]
    
    ppDir = os.path.join(META_PATH,'bin','CAD')
    abaqusDir = os.path.join(META_PATH,'bin','CAD', 'Abaqus')
    sys.path.append(ppDir)
    sys.path.append(abaqusDir)
    
from optparse import OptionParser
from odbAccess import *
from abaqus import *
from abaqusConstants import *
import visualization
import ComputedMetricsSummary
import UpdateReportJson_CAD
import utility_functions



# ===================================================================================================
# Global Variables 
# ===================================================================================================
#gVersion = '1.0.0.0'
#gConfigurationID = ''
#gComponentList = list()
#gFile = open('DebugLog.txt', 'w')
#gFile.write('#script:       ABQ_CompletePostProcess.py')
#gFile.write('#version:      ' + gVersion)
#gFile.write('#author:       Di Yao')
gStructuralMetricTypes = ['FactorOfSafety', 'MaximumDisplacement', 'Mises', 'Bearing', 'Shear']
gThermalMetricTypes = ['MaximumTemperature', 'MinimumTemperature']

#gQualityLookup = {1:'LOW', 2:'MEDIUM', 3:'HIGH'}

# ===================================================================================================

gLogger = utility_functions.setup_logger('ABQ_CompletePostProcess.log')

# ===================================================================================================
# Helper Functions
# 

# ===================================================================================================


# ===================================================================================================
# Functions
#

def CalculateMetrics(fileName, componentList, reqMetricSet):
    gLogger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    gLogger.info('FUNCTION: CalculateMetrics\n')
    #global gComponentList
    #global gQualityLookup
    global gStructuralMetricTypes
    global gThermalMetricTypes
    
    odbFileName=fileName + '.odb'
    myOdb = visualization.openOdb(path=odbFileName)

    maxShear = 0
    maxBearing = 0
    maxMises = 0
    minFOS = 0    
    lowYieldRange = 0
    upperYieldRange = 0
    maxTemp = -10**9
    minTemp = 10**9

    mainDir = os.path.join(os.getcwd(),odbName)
    if not os.path.exists(mainDir):
        os.makedirs(mainDir)
    os.chdir(mainDir)

    assembly = myOdb.rootAssembly

    distortedElementIDList = list()

    # get the distorted Element set
    distortedElements = None
    if 'WarnElemDistorted' in assembly.elementSets.keys():
        warnElemDistorted = assembly.elementSets['WarnElemDistorted']
        distortedElementSet = warnElemDistorted.elements
        if (len(distortedElementSet) > 0):
            distortedElements = distortedElementSet[0]
            for item in distortedElements:
                distortedElementIDList.append(item.label)

    for component in componentList.values():
        if len(component.Children) > 0:
            continue
        try:    # deck-based
            elemset = assembly.instances['PART-1-1'].elementSets[component.ElementID]
        except: # model-based
            elemset = assembly.instances[component.ElementID]
        
        distortedStressDict = {}
        noneDistortedStressDict = {}
        for step in myOdb.steps.values():
            lastFrame = step.frames[-1]
            if 'S' in reqMetricSet:
                distortedStressDict, noneDistortedStressDict, maxMises, maxBearing, maxShear = \
                        ProcessStress(lastFrame, elemset, maxMises, maxBearing, maxShear,
                            distortedStressDict, distortedElementIDList, noneDistortedStressDict)
            if 'TEMP' in reqMetricSet:
                maxTemp, minTemp = ProcessThermal(lastFrame, elemset, maxTemp, minTemp)
        
        if 'S' in reqMetricSet:
            # calculate FactorOfSafety
            factorOfSafety = 0
            factorOfSafety = float(component.MaterialProperty['Mises'])/maxMises
            
            shearQuality = bearQuality = miseQuality = 1
            shearRange = bearRange = miseRange = 1
            if (len(distortedStressDict) > 0):
                shearQuality, bearQuality, miseQuality = ProcessDistorted(distortedStressDict, (maxMises, maxBearing, maxShear))

            shearRange, bearRange, miseRange, outRangeIDs = EvaluateStressGradient(noneDistortedStressDict)
            component.OutOfRangeDistorted = outRangeIDs
            UpdateComponentStressMetrics(componentList, component, maxMises, maxShear, maxBearing, factorOfSafety,
                                         shearQuality, bearQuality, miseQuality, shearRange,
                                         bearRange, miseRange)
        if 'TEMP' in reqMetricSet:
            UpdateComponentThermalMetrics(componentList, component, maxTemp, minTemp)

        #end for

    gLogger.info('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
    SetupViewportPNG(myOdb, fileName, reqMetricSet)
    myOdb.close()           # close odb
    os.chdir('..') # bring you back to /Analysis/Abaqus
 

def ProcessThermal(lastFrame, elemset, maxTemp, minTemp):
    thermalFields = lastFrame.fieldOutputs['TEMP']
    elementThermalSet = thermalFields.getSubset(region=elemset)
    logger = logging.getLogger()
    
    for thermalValue in elementThermalSet.values:
        if thermalValue.data > maxTemp:
            maxTemp = thermalValue.data
        if thermalValue.data < minTemp:
            minTemp = thermalValue.data
            
    return (maxTemp, minTemp)
 
 
def ProcessStress(lastFrame, elemset, maxMises, maxBearing, maxShear, distortedStressDict,
                  distortedElementIDList, noneDistortedStressDict):
    stressFields = lastFrame.fieldOutputs['S']
    elementStressSet = stressFields.getSubset(region=elemset)
    for stressValue in elementStressSet.values:
        elementLabel = stressValue.elementLabel
        if elementLabel not in distortedElementIDList:
            if (stressValue.mises > maxMises):
                maxMises = stressValue.mises
            if (stressValue.press > maxBearing):
                maxBearing = stressValue.press
            if (stressValue.tresca > maxShear):
                maxShear = stressValue.tresca

            if elementLabel not in noneDistortedStressDict:
                noneDistortedStressDict[elementLabel] = dict()
                noneDistortedStressDict[elementLabel]['Shear'] = list()
                noneDistortedStressDict[elementLabel]['Mises'] = list()
                noneDistortedStressDict[elementLabel]['Bearing'] = list()

            noneDistortedStressDict[elementLabel]['Shear'].append(stressValue.tresca)
            noneDistortedStressDict[elementLabel]['Mises'].append(stressValue.mises)
            noneDistortedStressDict[elementLabel]['Bearing'].append(stressValue.press)
            
        else:
            if elementLabel not in distortedStressDict:
                distortedStressDict[elementLabel] = dict()
                distortedStressDict[elementLabel]['Shear'] = list()
                distortedStressDict[elementLabel]['Mises'] = list()
                distortedStressDict[elementLabel]['Bearing'] = list()

            distortedStressDict[elementLabel]['Shear'].append(stressValue.tresca)
            distortedStressDict[elementLabel]['Mises'].append(stressValue.mises)
            distortedStressDict[elementLabel]['Bearing'].append(stressValue.press)
            
    return (distortedStressDict, noneDistortedStressDict, maxMises, maxBearing, maxShear)


def UpdateComponentThermalMetrics(componentList, component, maxTemp, minTemp):
    for comp in componentList.values():
        if component.ComponentID in comp.Children:
            component = comp
            break
    if component.MetricsInfo.has_key('MaximumTemperature'):
        max = component.MetricsOutput[component.MetricsInfo['MaximumTemperature']]
        if max is None or max > maxTemp:
            component.MetricsOutput[component.MetricsInfo['MaximumTemperature']] = maxTemp
    if component.MetricsInfo.has_key('MinimumTemperature'):
        min = component.MetricsOutput[component.MetricsInfo['MinimumTemperature']]
        if min is None or min < minTemp:
            component.MetricsOutput[component.MetricsInfo['MinimumTemperature']] = minTemp

    
    
def UpdateComponentStressMetrics(componentList, component, maxMises, maxShear, maxBearing, factorOfSafety,
                                 shearQuality, bearQuality, miseQuality, shearRange,
                                 bearRange, miseRange):
    
    for comp in componentList.values():
        if component.ComponentID in comp.Children:
            # component is actually a child, so parent's metric data
            # should be updated - provided that child metrics are larger
            component = comp
            break

    if component.MetricsInfo.has_key('Shear'):
        shear = component.MetricsOutput[component.MetricsInfo['Shear']]
        if shear is None or shear < maxShear:
            component.MetricsOutput[component.MetricsInfo['Shear']] = maxShear
    if component.MetricsInfo.has_key('Bearing'):
        bearing = component.MetricsOutput[component.MetricsInfo['Bearing']]
        if bearing is None or bearing < maxBearing:
            component.MetricsOutput[component.MetricsInfo['Bearing']] = maxBearing
    if component.MetricsInfo.has_key('VonMisesStress'):
        mises = component.MetricsOutput[component.MetricsInfo['VonMisesStress']]
        if mises is None or mises < maxMises:
            component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = maxMises
    if component.MetricsInfo.has_key('FactorOfSafety'):
        fos = component.MetricsOutput[component.MetricsInfo['FactorOfSafety']]
        if fos is None or fos > factorOfSafety:
            component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = factorOfSafety
            
    component.QualityOutput['Shear'] = ComputedMetricsSummary.gQualityLookup[shearQuality]
    component.QualityOutput['Bearing'] = ComputedMetricsSummary.gQualityLookup[bearQuality]
    component.QualityOutput['Mises'] = ComputedMetricsSummary.gQualityLookup[miseQuality]
    component.RangeInfo['Shear'] = ComputedMetricsSummary.gQualityLookup[shearRange]
    component.RangeInfo['Bearing'] = ComputedMetricsSummary.gQualityLookup[bearRange]
    component.RangeInfo['Mises'] = ComputedMetricsSummary.gQualityLookup[miseRange]   
    
     
def ProcessDistorted(distortedStressDict, metricTuple):
    shearQuality = bearQuality = miseQuality = 1        # 1 - low, 2 - medium, 3 - errorneous
    
    lowYieldRange = 0.3 * metricTuple[0]        # mises
    highYieldRange = 0.6 * metricTuple[0]
    lowBearRange = 0.3 * metricTuple[1]         # bear
    highBearRange = 0.6 * metricTuple[1]
    lowShearRange = 0.3 * metricTuple[2]        # shear
    highShearRange = 0.6 * metricTuple[2]
    
    for item in distortedStressDict:
        minShear = min(distortedStressDict[item]['Shear'])
        maxShear = max(distortedStressDict[item]['Shear'])
        minBear = min(distortedStressDict[item]['Bearing'])
        maxBear = max(distortedStressDict[item]['Bearing'])
        minMise = min(distortedStressDict[item]['Mises'])
        maxMise = max(distortedStressDict[item]['Mises'])
        
        #quality
        if (shearQuality < 3):
            if (maxShear > highShearRange):
                shearQuality = 3
            elif (maxShear > lowShearRange and shearQuality < 2):
                shearQuality = 2
                
        if (bearQuality < 3):
            if (maxBear > highBearRange):
                bearQuality = 3
            elif (maxBear > lowBearRange and bearQuality < 2):
                bearQuality = 2
        if (miseQuality < 3):
            if (maxMise > highYieldRange):
                miseQuality = 3
            elif (maxMise > lowYieldRange and miseQuality < 2):
                miseQuality = 2        

    return shearQuality, bearQuality, miseQuality


def EvaluateStressGradient(distortedStressDict):
    #gLogger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    #gLogger.info('FUNCTION: EvaluateStressGradient()')
    shearRange = bearRange = miseRange = 1
    distortedOutRangeIDs = []       #list


    for item in distortedStressDict:
        minShear = min(distortedStressDict[item]['Shear'])
        maxShear = max(distortedStressDict[item]['Shear'])
        minBear = min(distortedStressDict[item]['Bearing'])
        maxBear = max(distortedStressDict[item]['Bearing'])
        minMise = min(distortedStressDict[item]['Mises'])
        maxMise = max(distortedStressDict[item]['Mises'])

        # gLogger.info('Element[{0}]: {1}     {2}'.format(item,
                                                        # minShear,
                                                        # maxShear))
        # gLogger.info('             {0}     {1}'.format(minBear,
                                                       # maxBear))
        # gLogger.info('             {0}     {1}'.format(minMise,
                                                       # maxMise))
                
        #range
        if ( maxShear != 0 ):
            if (abs((maxShear - minShear)/maxShear) > 0.25):
                shearRange = 3
        if ( maxBear != 0 ):
            if (abs((maxBear - minBear)/maxBear) > 0.25):
                bearRange = 3
        if (maxMise != 0):
            if (abs((maxMise - minMise)/maxMise) > 0.25):
                miseRange = 3

        if (max(shearRange, bearRange, miseRange) == 3):
            distortedOutRangeIDs.append(item)

    gLogger.info('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
    return shearRange, bearRange, miseRange, distortedOutRangeIDs

    
def saveContourPNG(myViewport, view, zoom, type, save2fileName, meshed):
    myViewport.view.setValues(session.views[view])
    myViewport.view.zoom(zoom)
    if meshed:
        session.printToFile(save2fileName + type + "_" + view + "_UNDEF_Meshed", PNG, (myViewport,))
    else:
        session.printToFile(save2fileName + type + "_" + view + "_UNDEF", PNG, (myViewport,))
    
    
def SetupViewportPNG(myOdb, fileName, reqMetricSet, maxStressStep=None):
    try:
        myViewport=session.Viewport(name='Contour Plot', origin=(0, 0), width=200, height=100)
        myViewport.setValues(displayedObject=myOdb)

        mySteps = myOdb.steps
        numSteps = len(mySteps)
        
        session.printOptions.setValues(rendition=COLOR,
                                       vpDecorations=OFF, vpBackground=OFF)
                                       
        text = { 'S' : 'MPa', 'U' : 'MPa', 'TEMP' : 'K' }
        for metric in reqMetricSet:
            t = myOdb.userData.Text(name='Text-1', text=text[metric],offset=(34.1042, 179.422),\
                font='-*-arial-medium-r-normal-*-*-120-*-*-p-*-*-*')
            myViewport.plotAnnotation(annotation=t)
            
            if metric == 'S' and maxStressStep in mySteps.keys():
                stepKey = mySteps.keys()[maxStressStep]
                myViewport.odbDisplay.commonOptions.setValues(visibleEdges=EXTERIOR)
                step = mySteps[stepKey]
                CreateViewportPNG(myOdb, metric, fileName, myViewport, step)
            else:
                for i in range(numSteps):
                    myViewport.odbDisplay.commonOptions.setValues(visibleEdges=EXTERIOR)
                    stepKey = mySteps.keys()[i]
                    step = mySteps[stepKey]
                    CreateViewportPNG(myOdb, metric, fileName, myViewport, step)
    except KeyError:
        logger.error('Key Error' + '\n')
        odb.close()
        raise
    except (AbaqusException), value:
        logger.error(value + '\n')
        odb.close()
        raise      


def CreateViewportPNG(myOdb, metric, fileName, myViewport, step):        
    dirNames = { 'S':'Stress', 'U':'Stress', 'TEMP':'Thermal' }
    plotDir = os.path.join(os.getcwd(),str(dirNames[metric] + '_Contours'))
    if not os.path.exists(plotDir):
        os.mkdir(plotDir)
    os.chdir(plotDir)

    save2fileName=fileName.replace(' ','') + "_"
    
    if metric == 'U':   # Displacement plots are dealt with differently due to 'temporary solution'
        #temporary solution until write3DXMLFile issue fixed----------------
        displacement = step.frames[-1].fieldOutputs[metric]
        myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                                 outputPosition=NODAL,
                                                 refinement=(INVARIANT, 'Magnitude'))                   
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
        session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
        #temporary solution end----------------------------------------------
        
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
        session.viewports['Contour Plot'].makeCurrent()
        session.viewports['Contour Plot'].maximize()            
        session.writeVrmlFile(save2fileName+"DEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"DEF", False, (myViewport,))

        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
        session.viewports['Contour Plot'].makeCurrent()
        session.viewports['Contour Plot'].maximize()            
        session.writeVrmlFile(save2fileName+"UNDEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"UNDEF", False, (myViewport,))
        
        #get displacement data
        displacement = step.frames[-1].fieldOutputs[metric]
        myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                                 outputPosition=NODAL,
                                                 refinement=(INVARIANT, 'Magnitude'))                   
        myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
        myViewport.view.setValues(session.views['Iso'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
    else:
        #get stress data
        output = step.frames[-1].fieldOutputs[metric]
        if metric == 'S':
            myViewport.odbDisplay.setPrimaryVariable(field=output,
                                                     outputPosition=INTEGRATION_POINT,
                                                     refinement=(INVARIANT, 'Mises'))
        elif metric == 'TEMP':
            myViewport.odbDisplay.setPrimaryVariable(outputPosition=INTEGRATION_POINT,
                                                     variableLabel='TEMP')
        myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
        
        views = { 'Iso':0.8, 'Left':0.8, 'Right':0.8, 'Back':0.8, 'Front':0.8, 'Top':0.7, 'Bottom':0.7 }
        for view in views.keys():
            saveContourPNG(myViewport, view, views[view], dirNames[metric], save2fileName, meshed=True)
            
        myViewport.odbDisplay.commonOptions.setValues(visibleEdges=FEATURE)
        for view in views.keys():
            saveContourPNG(myViewport, view, views[view], dirNames[metric], save2fileName, meshed=False)
    os.chdir('..')  # Get out of contour plot directory
    
    
def afterJobModal(jobName,analysisStepName):
    """ This is only called in Model-Based. Not mentioned in this PostProcessing program. """
    logger = logging.getLogger()
    odb = odbAccess.openOdb(path=jobName + '.odb')
    logger.info("**********************************************************************************" + '\n')
    logger.info('Job complete\n')
    root = os.getcwd()
    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        headers = (('Mode Number','Frequency(Hz)'))
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in opening %s.\n' % jobName)
        raise
    try:
        for histReg in odb.steps[analysisStepName].historyRegions.keys():
            eigenFrequencies = odb.steps[analysisStepName].historyRegions[histReg].historyOutputs['EIGFREQ'].data
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in reading eigenfrequencies\n')
        raise
        
    forCSV = (headers,) + eigenFrequencies
    logger.info("Creating the CSV file" + '\n')
    
    report_file2 = 'modalOutput.csv'
    try:
        with open(report_file2,'wb') as f2:
            writer = csv.writer(f2)
            for (number,frequency) in forCSV:
                val = (number, frequency)
                writer.writerow(val)                    
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in exporting data to %s.\n' % report_file2)
        pass
    
    idArray = []
    reportFile = 'testbench_manifest.json'                              # name of metric JSON
    reportPath = os.path.join(os.getcwd(), '..', '..', reportFile)
    logger.info("Updating the testbench_manifest.json file" + '\n')

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        ourResults = data["Metrics"]

        minMode = -1
        for (key,entry) in eigenFrequencies:
            if entry < minMode or minMode == -1:
                   minMode = entry
        if minMode == 0:
            logger.info('WARNING: Rigid body modes are present, model has not been constrained properly' + '\n')
        for cs in ourResults:
            if cs["Name"] == 'Minimum_Mode':
                cs["Value"] = minMode
                cs["Unit"] = 'Hz'
    
        with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
            json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary

    except:
        logger.error('Error in exporting data to %s.\n' % reportFile)
        raise

    odb.close()
    os.chdir(root)
    try:
        CopyOrDeleteResults(root, jobName)
    except:
        logger.error('Error in copying or deleting result files to users machine\n')
        pass

    logger.info("**********************************************************************************" + '\n')
    logger.info('Success\n')
    logger.info("**********************************************************************************" + '\n')    
    
    
    
def ParseArgs():
    odbName = None
    metaDataFile = None
    reqMetricsFile = None
    resultsJson = None
    adams = None
    argList = sys.argv
    argc = len(argList)
    i = 0
    while (i < argc):
        if (argList[i][:2] == '-o'):
            i+=1
            odbName = utility_functions.right_trim(argList[i], '.odb')
        elif (argList[i][:2] == '-p'):
            i+=1
            metaDataFile = os.path.join(os.getcwd(),argList[i])
        elif (argList[i][:2] == '-m'):
            i+=1
            reqMetricsFile = os.path.join(os.getcwd(),argList[i])
        elif (argList[i][:2] == '-j'):
            i+=1
            resultsJson = os.path.join(os.getcwd(),argList[i])
        elif (argList[i][:2] == '-a'):
            i+=1
            adams = True
        i+=1
        
    if not any([odbName, metaDataFile, reqMetricsFile, resultsJson]):
        exit(1)
    
    return (odbName, metaDataFile, reqMetricsFile, resultsJson, adams)
    
    
    
    
    
# ===================================================================================================

# ===================================================================================================
# START
#

if __name__ == '__main__':
    global gVersion
    odbName, metaDataFilePath, reqMetricsFilePath, resultsJsonPath, adams = ParseArgs()
    componentList = ComputedMetricsSummary.ParseMetaDataFile(metaDataFilePath, odbName, adams)
    reqMetrics = ComputedMetricsSummary.ParseReqMetricsFile(reqMetricsFilePath, componentList)
    CalculateMetrics(odbName, componentList, reqMetrics)     
    computedValuesXml = ComputedMetricsSummary.WriteXMLFile(componentList) 
    ComputedMetricsSummary.WriteMetric2File(componentList)
    UpdateReportJson_CAD.update_manifest(resultsJsonPath, computedValuesXml)
    
    if adams:
        utility_functions.CopyOrDeleteResults(odbName, True)
    else:
        utility_functions.CopyOrDeleteResults(odbName)
    