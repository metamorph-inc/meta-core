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


import sys
from optparse import OptionParser
from odbAccess import *
from abaqus import *
from abaqusConstants import *
import visualization
import ComputedMetricsSummary
import logging
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
#gStructuralMetricTypes = ['FactorOfSafety', 'MaximumDisplacement', 'Mises', 'Bearing', 'Shear']

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

def CalculateMetrics(fileName, componentList):
    gLogger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    gLogger.info('FUNCTION: CalculateMetrics\n')
    #global gComponentList
    #global gQualityLookup
    
    odbFileName=fileName + '.odb'
    myOdb = visualization.openOdb(path=odbFileName)

    maxShear = 0
    maxBearing = 0
    maxMises = 0
    minFOS = 0    
    lowYieldRange = 0
    upperYieldRange = 0

    stress = 'S'
    displacement = 'U'

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
        

    # Shows how to print key()
    #elemset = assembly.elementSets.keys()
    #for item in elemset:
    #    Write2Log(str(item) +'\n')  

    for component in componentList.values():
        elemset = assembly.instances['PART-1-1'].elementSets[component.ElementID]
        distortedStressDict = {}        # empty dict

        noneDistortedStressDict = {}    # empty dict

        for step in myOdb.steps.values():
            lastFrame = step.frames[-1]
            stressFields = lastFrame.fieldOutputs[stress]
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
                    #Write2Log('Distorted[' + str(elementLabel) + ']:		' + str(stressValue.tresca) + '		' + str(stressValue.mises) + '		' + str(stressValue.press))
                    if elementLabel not in distortedStressDict:
                        distortedStressDict[elementLabel] = dict()
                        distortedStressDict[elementLabel]['Shear'] = list()
                        distortedStressDict[elementLabel]['Mises'] = list()
                        distortedStressDict[elementLabel]['Bearing'] = list()

                    distortedStressDict[elementLabel]['Shear'].append(stressValue.tresca)
                    distortedStressDict[elementLabel]['Mises'].append(stressValue.mises)
                    distortedStressDict[elementLabel]['Bearing'].append(stressValue.press)

                    
        # calculate FactorOfSafety
        factorOfSafety = 0
        mShear = float(component.MaterialProperty['Shear'])
        mBear = float(component.MaterialProperty['Bearing'])
        mMises = float(component.MaterialProperty['Mises'])
        #factorOfSafety = min(mShear/maxShear,
        #                     mBear/maxBearing,
        #                     mMises/maxMises)
        factorOfSafety = mMises/maxMises

        if component.MetricsInfo.has_key('Shear'):
            component.MetricsOutput[component.MetricsInfo['Shear']] = maxShear
            #Write2Log('\nMaxMises: ' + str(maxMises))
        if component.MetricsInfo.has_key('Bearing'):
            component.MetricsOutput[component.MetricsInfo['Bearing']] = maxBearing
            #Write2Log('\nMaxBearing: ' + str(maxBearing))
        if component.MetricsInfo.has_key('Mises'):
            component.MetricsOutput[component.MetricsInfo['Mises']] = maxMises
            #Write2Log('\nMaxShear: ' + str(maxShear))
        if component.MetricsInfo.has_key('FactorOfSafety'):
            component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = factorOfSafety
            #Write2Log('\nFactorOfSafety: ' + str(factorOfSafety))       
        
        shearQuality = bearQuality = miseQuality = 1
        shearRange = bearRange = miseRange = 1
        if (len(distortedStressDict) > 0):
            shearQuality, bearQuality, miseQuality = ProcessDistorted(distortedStressDict, (maxMises, maxBearing, maxShear))

        shearRange, bearRange, miseRange, outRangeIDs = EvaluateStressGradient(noneDistortedStressDict)
        component.OutOfRangeDistorted = outRangeIDs
        
        component.QualityOutput['Shear'] = ComputedMetricsSummary.gQualityLookup[shearQuality]
        component.QualityOutput['Bearing'] = ComputedMetricsSummary.gQualityLookup[bearQuality]
        component.QualityOutput['Mises'] = ComputedMetricsSummary.gQualityLookup[miseQuality]
        component.RangeInfo['Shear'] = ComputedMetricsSummary.gQualityLookup[shearRange]
        component.RangeInfo['Bearing'] = ComputedMetricsSummary.gQualityLookup[bearRange]
        component.RangeInfo['Mises'] = ComputedMetricsSummary.gQualityLookup[miseRange]

        

        #end for

    gLogger.info('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
    CreateViewportPNG(myOdb, fileName)
    
    myOdb.close()           # close odb
    
     
def ProcessDistorted(distortedStressDict, metricTuple):
    shearQuality = bearQuality = miseQuality = 1        # 1 - low, 2 - medium, 3 - errorneous
    
    lowYieldRange = 0.3 * metricTuple[0]        # mises
    highYieldRange = 0.6 * metricTuple[0]
    lowBearRange = 0.3 * metricTuple[1]         # bear
    highBearRange = 0.6 * metricTuple[1]
    lowShearRange = 0.3 * metricTuple[2]        # shear
    highShearRange = 0.6 * metricTuple[2]
    
    for item in distortedStressDict:
        #Write2Log('Processing Distorted: ' + str(item))
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
    gLogger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    gLogger.info('FUNCTION: EvaluateStressGradient()')
    shearRange = bearRange = miseRange = 1
    distortedOutRangeIDs = []       #list


    for item in distortedStressDict:
        minShear = min(distortedStressDict[item]['Shear'])
        maxShear = max(distortedStressDict[item]['Shear'])
        minBear = min(distortedStressDict[item]['Bearing'])
        maxBear = max(distortedStressDict[item]['Bearing'])
        minMise = min(distortedStressDict[item]['Mises'])
        maxMise = max(distortedStressDict[item]['Mises'])

        gLogger.info('Element[{0}]: {1}     {2}'.format(item,
                                                        minShear,
                                                        maxShear))
        gLogger.info('             {0}     {1}'.format(minBear,
                                                       maxBear))
        gLogger.info('             {0}     {1}'.format(minMise,
                                                       maxMise))
                
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

    
def CreateViewportPNG(myOdb, fileName):
    try:
        myViewport=session.Viewport(name='Contour Plot', origin=(0, 0), width=200, height=100)
        #myOdb = visualization.openOdb(path=odbName)
        myViewport.setValues(displayedObject=myOdb)

        mySteps = myOdb.steps
        numSteps = len(mySteps)
        

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
            #temporary solution end--------------------------------------------------------------------------------------
            
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
            displacement = step.frames[-1].fieldOutputs['U']
            myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                                     outputPosition=NODAL,
                                                     refinement=(INVARIANT, 'Magnitude'))                   
            myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
            myViewport.view.setValues(session.views['Iso'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))

            #get stress data
            stress = step.frames[-1].fieldOutputs['S']
            myViewport.odbDisplay.setPrimaryVariable(field=stress,
                                                     outputPosition=INTEGRATION_POINT,
                                                     refinement=(INVARIANT, 'Mises'))
            myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))


            myViewport.view.setValues(session.views['Iso'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Iso_UNDEF", PNG, (myViewport,))

            myViewport.view.setValues(session.views['Left'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Left_UNDEF", PNG, (myViewport,))

            myViewport.view.setValues(session.views['Right'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Right_UNDEF", PNG, (myViewport,))

            myViewport.view.setValues(session.views['Back'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Back_UNDEF", PNG, (myViewport,))

            myViewport.view.setValues(session.views['Front'])
            myViewport.view.zoom(0.8)
            session.printToFile(save2fileName+"Front_UNDEF", PNG, (myViewport,))

            myViewport.view.setValues(session.views['Top'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Top_UNDEF", PNG, (myViewport,))         

            myViewport.view.setValues(session.views['Bottom'])
            myViewport.view.zoom(0.7)
            session.printToFile(save2fileName+"Bottom_UNDEF", PNG, (myViewport,))
            
            
    except KeyError:
        print 'Key Error'
        odb.close()
        exit(0)
    except (AbaqusException), value:
        print 'Error:', value
        odb.close()
        exit(0)

    
# ===================================================================================================

# ===================================================================================================
# START
#

if __name__ == '__main__':
    global gVersion
    odbName = None
    paramFile = None
    argList = sys.argv
    argc = len(argList)
    i = 0
    while (i < argc):
        if (argList[i][:2] == '-o'):
            i+=1
            odbName = utility_functions.right_trim(argList[i], '.odb')
        elif (argList[i][:2] == '-p'):
            i+=1
            paramFile = argList[i]
        i+=1
    if not odbName or not paramFile:
        exit(1)

    componentList = ComputedMetricsSummary.ParseXMLFile(paramFile) 
    CalculateMetrics(odbName, componentList)     
    ComputedMetricsSummary.WriteXMLFile(componentList) 