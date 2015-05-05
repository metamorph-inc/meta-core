"""

AbaqusThermal.py, version 1.3.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which generate loading conditions
        for thermal related concepts.

"""

import os, re, json
from random import randint
from numpy import array, cross, transpose, vstack, dot
import numpy.linalg as LA
import string as STR
import xml.etree.ElementTree as ET
import _winreg, sys, ctypes, uuid, traceback
from abaqus import *
from abaqusConstants import *
import mesh, regionToolset, interaction
import logging

from AbaqusGeometry import myMask
from AbaqusError import *

# Thermal analysis to be conducted in terms of Kelvin
def tempConv(tMagUnconv,tUnitsOrg):
    logger = logging.getLogger()
    tUnits = tUnitsOrg.lower()    
    if tUnits == 'c' or tUnits == 'degc' or tUnits == 'degree celsius':
        tMag = tMagUnconv + 273.15
    elif tUnits == 'k' or tUnits == 'kelvin' or tUnits == 'degree kelvin':
        tMag = tMagUnconv
    elif tUnits == 'f' or tUnits == 'degf' or tUnits == 'degree fahrenheit':
        tMag = ((tMagUnconv - 32) / 1.8) + 273.15
    else:
        logger.info('WARNING: Temperature units encountered were not found in conversion ' \
                    'function. Proceeding with analysis with assumption that units ' \
                    'asssociated with data are using Kelvin.\n')
        tMag = tMagUnconv
    return tMag
    
def check_for_conflicting_loads(loadBCLib, thermal):
    """ This function compares the current thermal element to loads previously added to
        to the load library and throws an error if a conflict arises. EG, a surface heat
        flux is applied to a face of a plate, and then another, different surface
        heat flux is applied for the entire plate. With no way of determining precedence,
        an error is thrown. """
    logger = logging.getLogger()
    loadType = thermal.get('LoadType')
    for entry in loadBCLib:
        if 'Treatment' in entry:
            if entry['Treatment'] == loadType:
                try:
                    id = thermal.find('Component').get('ComponentID')             # applied to component
                except:                                                           # applied to geometry
                    id = thermal.find('Geometry/Features/Feature').get('ComponentID')                
                if entry['ComponentID'] == id and float(entry[loadType]) != float(thermal.get('Value')):
                    logger.error('Multiple constraints of treatment ' + loadType + \
                        ' are associated with ComponentID ' + str(id) + '.')
                    logger.error('No way of determining ' + \
                        'precendence - analysis is aborting. Please correct your test bench.')
                    raise ConflictingThermalConstraintException

                    
def grabGeometry(geometryXML, loadBCLib):
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
    

def generateSpecifiedTemperatureDict(thermal, loadBCLib, thermalSetXML):
    logger = logging.getLogger()
    logger.info("Defining specified temperature BC" + '\n')
    loadBCLib[-1].update([['Treatment','SpecifiedTemperature']])    # load is type "SpecifiedTemperature"
    tUnits = thermal.get('Unit')                              
    tMagUnconv = float(thermal.get('Value'))                        # magnitude of temperature
    tMag = tempConv(tMagUnconv,tUnits)
    loadBCLib[-1].update([['SpecifiedTemperature',tMag]])           # store temperature value

    logger.info("Specified Temperature Magnitude = " + str(tMag) + '\n')

    
def generateConvectionHeatDict(thermal, loadBCLib, thermalSetXML):
    logger = logging.getLogger()
    logger.info("Defining heat convection BC" + '\n')
    loadBCLib[-1].update([['Treatment','ConvectionHeat']])
    tUnits = thermal.get('Unit')                              
    # No conversion needed for convection: [J/(s.(m^2 K))] ==[W/(m^2K)]
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['ConvectionHeat',tMag]])           # store temperature value

    # Look for ambient temperature load - REQUIRED for convection
    ambient = False
    for therm in thermalSetXML:
        if therm.get('LoadType') == 'AmbientTemperature':
            ambient = True
            sink = tempConv(float(therm.get('Value')),therm.get('Unit'))
            loadBCLib[-1].update([['AmbientTemperature',sink]])
    if not ambient:
        logger.error('Ambient temperature not specified.')
        raise Exception('No ambient temperature found! ' + \
                        'Needed for convection load!')
                        
    
    logger.info("Convection coefficient = " + str(tMag) + '\n')
    logger.info("Ambient Temperature = " + str(sink) + '\n')


def generateHeatFluxDict(thermal, loadBCLib, thermalSetXML):
    logger = logging.getLogger()
    logger.info("Defining surface heat flux load" + '\n')
    loadBCLib[-1].update([['Treatment','HeatFlux']])
    tUnits = thermal.get('Unit')                              
    # No conversion needed for convection: [J/(s.(m^2))] == [W/(m^2)]
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['HeatFlux',tMag]])           # store temperature value

    logger.info("Surface Heat Flux = " + str(tMag) + '\n') 


def generateHeatGenerationDict(thermal, loadBCLib, thermalSetXML):
    logger = logging.getLogger()
    logger.info("Defining heat generation" + '\n')
    loadBCLib[-1].update([['Treatment','HeatGeneration']])
    # Need volumetric heat rate, not heat rate. However, unit
    # will only be stored for now. Conversion will occur when 
    # entry is applied as that is where instance volume can be
    # determined.
    tUnits = thermal.get('Unit')                              
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['HeatGeneration',tMag]])           # store temperature value

    logger.info("Heat Generation = " + str(tMag) + '\n')        
    
    
def setInitialTemperature(thermal, myAsm, instName):             # Only applied for component level, not geometry
    logger = logging.getLogger()
    try:
        logger.info("Defining initial temperature for " + \
            str(thermal.find('Component').get('ComponentID')) + '\n')
    except:
        raise InitialTempException          # Component block not found, ie, specified for specific geometry.
    tUnits = thermal.get('Unit')                              
    tMagUnconv = float(thermal.get('Value'))                        # magnitude of temperature
    tMag = tempConv(tMagUnconv,tUnits)

    logger.info("Initial Temperature Magnitude = " + str(tMag) + '\n')
    try:
        region = (myAsm.instances[instName].cells, )
        initTempName = instName + '-InitTemp'
        mdb.models['Model-1'].Temperature(name=initTempName, 
            createStepName='Initial', region=region, distributionType=UNIFORM, 
            crossSectionDistribution=CONSTANT_THROUGH_THICKNESS, 
            magnitudes=(tMag,))
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during Initial Temp.\n')
        raise

def generateThermalConstructs(thermalSetXML, loadBCLib):
    logger = logging.getLogger()
    
    for thermal in thermalSetXML:                                           # for each load definition:
        loadBCLib.append({})                                                # add new dictionary
        thermalLoadType = thermal.get('LoadType')                           # look for SpecifiedTemperature load
        if (thermalLoadType == 'InitialTemperature' 
                or thermalLoadType == 'AmbientTemperature'):
            loadBCLib.pop()
            continue
        else:
            check_for_conflicting_loads(loadBCLib, thermal)
            try:
                globals()[str('generate' + thermalLoadType + 'Dict')](thermal, loadBCLib, thermalSetXML)
            except KeyError:
                logger.error('Specified thermal treatment is not yet supported.')
        if thermal.find('Geometry'):         # Apply construct only to specified geometry, otherwise to whole body
            if thermalLoadType == 'HeatGeneration':
                msg = 'Heat Generation block incorrectly applied to geometry construct within a component. ' +\
                      'Block should only be applied to entire component/assembly. Please revise ' +\
                      ' your GME model and re-run simulation.'
                logger.error(msg)
                raise ThermalConstraintException(msg)
            grabGeometry(thermal.find('Geometry'), loadBCLib)
            loadBCLib[-1].update([['Component',False]])
        else:
            loadBCLib[-1].update([['ComponentID',thermal.find('Component').get('ComponentID')]])
            loadBCLib[-1].update([['Component',True]])      # Apple constraint to entire instance

def createSpecifiedTemperatureConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    logger = logging.getLogger()
    try:
        if entry['Component'] == False:
            tempFace = entry['FaceIDs'][0]
            instances = entry['Instance']
            faces = myAsm.instances[instances].faces
            region = regionToolset.Region(faces=faces[3:4])
        else:
            region = (myAsm.instances[instName].cells, )   # Apply BC to entire part
            logger.info('Creating load/BC on entirety of ' + instName + \
                        ' with treatment Specified Temperature. \n')
        specTempName = regionName + '-SpecTemp'
        myModel.TemperatureBC(name=specTempName,createStepName=myStep.name,
            region=region,magnitude=entry['SpecifiedTemperature'],
            distributionType=UNIFORM, amplitude=amp)              # create specified temperature BC
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during Specified Temp.\n')
        raise
        
        
def createConvectionHeatConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    logger = logging.getLogger()
    try:
        if entry['Component'] == False:
            regionMask = myMask(entry['FaceIDs'][0])
            maskRegion = myAsm.instances[instName].faces.getSequenceFromMask(mask=(regionMask,),)
        else:
            regionMask = myAsm.instances[instName].faces
            maskRegion = regionMask.getSequenceFromMask(mask=('[#ff ]', ), )    # Apply to entire body
            logger.info('Creating load/BC on entirety of ' + instName + \
                        ' with treatment Convection. \n')
        region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
        convecName = regionName + '-ConvecHeat'
        myModel.FilmCondition(name=convecName,createStepName=myStep.name,
            surface=region,definition=EMBEDDED_COEFF,filmCoeff=entry['ConvectionHeat'],
            filmCoeffAmplitude=amp,sinkTemperature=entry['AmbientTemperature'],
            sinkAmplitude=amp) # missing sinkTemp and sinkAmp
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error creating Convection Heat constraint.\n')
        raise        


def createHeatFluxConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    logger = logging.getLogger()
    try:
        if entry['Component'] == False:
            regionMask = myMask(entry['FaceIDs'][0])
            maskRegion = myAsm.instances[instName].faces.getSequenceFromMask(mask=(regionMask,),)
        else:
            regionMask = myAsm.instances[instName].faces
            maskRegion = regionMask.getSequenceFromMask(mask=('[#ff ]', ), )    # Apply to entire body
            logger.info('Creating load/BC on entirety of ' + instName + \
                        ' with treatment Surface Heat Flux. \n')
        region = myAsm.Surface(side1Faces=maskRegion,name=regionName)
        surfFluxName = regionName + '-SurfHeatFlux'
        myModel.SurfaceHeatFlux(name=surfFluxName,createStepName=myStep.name,
            region=region,magnitude=entry['HeatFlux'],distributionType=UNIFORM,
            amplitude=amp)
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error creating Surface Heat Flux constraint.\n')
        raise


def createHeatGenerationConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    logger = logging.getLogger()
    try:
        MPdict = myAsm.getMassProperties(regions=(myAsm.instances[instName],),)
        vol = MPdict['volume']
        if vol == None:
            logger.error('Error getting volume from part instance ' + instName)
        else:
            vol_heat_rate = float(entry['HeatGeneration']) / float(vol)
        logger.info('Creating load/BC on entirety of ' + instName + \
                    ' with treatment Heat Generation. \n')
        region = (myAsm.instances[instName].cells, )
        HeatGenName = regionName + '-HeatGen'
        myModel.BodyHeatFlux(name=HeatGenName,createStepName=myStep.name,
            region=region,magnitude=entry['HeatGeneration'],distributionType=UNIFORM,
            amplitude=amp)
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error creating Heat Generation constraint.\n')
        raise           
        

def createInitialTemperatureConstraint(myModel, myAsm, entry, myStep, regionName):
    logger = logging.getLogger()
    try:
        instances = entry['Instance']
        region = (myAsm.instances[instances].cells, )
        initTempName = instances + '-InitTemp'
        mdb.models['Model-1'].Temperature(name=initTempName, 
            createStepName='Initial', region=region, distributionType=UNIFORM, 
            crossSectionDistribution=CONSTANT_THROUGH_THICKNESS, 
            magnitudes=entry['InitialTemperature'])

    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during Initial Temp.\n')
        raise
    
def createThermalConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName=None):
    # Dispatch function based on geometry type.
    logger = logging.getLogger()
    if regionName == None:
        try:
            regionName = 'BodyThermConst_' + str(randint(1,1000))
        except AbaqusError:
            # try again in case generated int already was created
            regionName = 'BodyThermConst_' + str(randint(1,1000))  
    try:
        globals()[str('create' + entry['Treatment'] + 'Constraint')](myModel, myAsm, entry, 
                                                                     myStep, amp, instName,
                                                                     regionName)
    except KeyError:
        if entry['Treatment'] == 'AmbientTemperature':
            pass
        else:
            logger.error('Specified thermal treatment ' + entry['Treatment'] + \
                         ' is not yet supported.')
        
        
def defineThermalElements(myAsm, regionPick, args):
    logger = logging.getLogger()
    try:
        elemType1 = mesh.ElemType(elemCode=C3D20RT, elemLibrary=STANDARD)
        elemType2 = mesh.ElemType(elemCode=UNKNOWN_WEDGE, elemLibrary=STANDARD)
        elemType3 = mesh.ElemType(elemCode=C3D10MT, elemLibrary=STANDARD, 
            secondOrderAccuracy=OFF, hourglassControl=DEFAULT, 
            distortionControl=DEFAULT)
        myAsm.setMeshControls(regions=regionPick,elemShape=TET,technique=FREE,
            sizeGrowthRate=1.05)
        myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType1,elemType2,elemType3,))
    except:
        logger.error('Error trying to assign thermal-type elements for mesh \n')
        raise
        
def convert_postprocessing_metric(metric, value, inUnit, outUnit):
    # If GME testbench metrics have either degrees Celsius or Fahrenheit,
    # convert data from Kelvin to desired units.    
    logger = logging.getLogger()
    logger.info('Converting ' + metric + ' of ' + str(value) + ' ' + \
                inUnit + ' to ' + outUnit)
    if str(outUnit) == 'Degree Celsius':
        return float(value - 273.15)
    elif outUnit == 'Degree Fahrenheit':
        return float(1.8*(value - 273.15) + 32)        

        
def thermalPostProcessing(odb, mySteps, numSteps, jobName, superRef, analysisStepName):
    logger = logging.getLogger()
    
    instRef = superRef['instRef']
    metricRef = superRef['metricRef']
    maxTemp = {}
    minTemp = {}
    
    for key in instRef.keys():
        maxTemp.update([[key, 0]])
        minTemp.update([[key, 10000]])
    for j in range(numSteps):
        stepKey = mySteps.keys()[j]
        step = mySteps[stepKey]
        tout = step.frames[-1].fieldOutputs['TEMP'].values
        for i in range(len(tout)):                      # for each value in the stress field output:
            try:
                inst = tout[i].instance.name                    # get name of part instance
                if tout[i].data > maxTemp[inst]:             # if the Mises stress is greater than maxStress for that part:
                    maxTemp[inst] = tout[i].data             # set the part's maxStress equal to that Mises stress
                if tout[i].data < minTemp[inst]:
                    minTemp[inst] = tout[i].data
            except:
                raise
    
    if 'MaximumTemperature' in metricRef:
        for inst in maxTemp.keys():
            if inst in metricRef['MaximumTemperature']:
                metricRef['MaximumTemperature'][inst] = maxTemp[inst]
    if 'MinimumTemperature' in metricRef:
        for inst in minTemp.keys():
            if inst in metricRef['MinimumTemperature']:
                metricRef['MinimumTemperature'][inst] = minTemp[inst]
                
    with open('testbench_manifest.json','r') as json_data:
        data = json.load(json_data)
    
    # The block below allows the user to name their metrics whatever they want
    # as the manifest is populated based off the derived metric type and not the name        
    # metricRef2: contains MetricType (Max/Min temperature)
    # metricRef3: contains componentID associated with that type.
    idArray = [rs['GMEID'] for rs in data['Metrics']]
    for component in idArray:
        for component2 in superRef['metricRef2']:
            if component == component2:
                for component3 in maxTemp:
                    if superRef['metricRef3'][component] == instRef[component3]['ComponentID']:
                        if superRef['metricRef2'][component] == 'MaximumTemperature':
                            for cs in data['Metrics']:
                                if cs['GMEID'] == component:
                                    if cs['Unit'] == '':
                                        cs['Value'] = max(maxTemp.values())
                                        cs['Unit'] = 'Kelvin'
                                    else:
                                        cs['Value'] = convert_postprocessing_metric(cs['Name'], 
                                                        max(maxTemp.values()), 'Kelvin', cs['Unit'])
                        elif superRef['metricRef2'][component] == 'MinimumTemperature':
                            for cs in data['Metrics']:
                                if cs['GMEID'] == component:
                                    if cs['Unit'] == '':
                                        cs['Value'] = min(minTemp.values())
                                        cs['Unit'] = 'Kelvin'
                                    else:
                                        cs['Value'] = convert_postprocessing_metric(cs['Name'], 
                                                        min(minTemp.values()), 'Kelvin', cs['Unit'])
            
    with open('testbench_manifest.json','wb') as out_file:
        json.dump(data, out_file, indent=4)
        
        
    return (maxTemp, minTemp)
    

def CreateThermalViewportPNG(myOdb, fileName, myViewport, mySteps, i=None):
    logger = logging.getLogger()
    if i is not None:
        stepKey = mySteps.keys()[i]
    elif maxStressStep is not None:
        stepKey = mySteps.keys()[maxStressStep]
    step = mySteps[stepKey]
    
    thermDir = os.path.join(os.getcwd(),'Thermal_Contours')
    if not os.path.exists(thermDir):
        os.mkdir(thermDir)
    os.chdir(thermDir)
    
    save2fileName=fileName.replace(' ','') + "_"
    
    #get thermal data
    logger.info("Grabbing thermal data" + '\n')
    #temp = step.frames[-1].fieldOutputs['TEMP']
    myViewport.odbDisplay.setPrimaryVariable(outputPosition=INTEGRATION_POINT,
                                             variableLabel='TEMP')
    myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))

    #--------------------------------------------------------------------------------------------------------------------

    session.viewports['Contour Plot'].viewportAnnotationOptions.setValues(
        legendFont='-*-verdana-medium-r-normal-*-*-120-*-*-p-*-*-*')

    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
    session.viewports['Contour Plot'].makeCurrent()
    session.viewports['Contour Plot'].maximize()            

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.view.setValues(session.views['Iso'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Iso_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Iso_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Left'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Left_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Left_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Right'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Right_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Right_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Back'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Back_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Back_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Front'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Front_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Front_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Top'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Thermal_Top_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Top_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Bottom'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Thermal_Bottom_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Bottom_UNDEF_Meshed" + '\n')

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.odbDisplay.commonOptions.setValues(
    visibleEdges=FEATURE)

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.view.setValues(session.views['Iso'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Iso_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Iso_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Left'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Left_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Left_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Right'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Right_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Right_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Back'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Back_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Back_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Front'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Thermal_Front_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Front_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Top'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Thermal_Top_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Top_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Bottom'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Thermal_Bottom_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Thermal_Bottom_UNDEF" + '\n')
    os.chdir('..')