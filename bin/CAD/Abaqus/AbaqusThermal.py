"""

AbaqusThermal.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which generate loading conditions
        for thermal related concepts.

"""

import logging
from random import randint
import cad_library

from abaqus import *
from abaqusConstants import *
import mesh
import regionToolset
from AbaqusGeometry import myMask


def tempConv(tMagUnconv, tUnitsOrg):
    """ Thermal analysis to be conducted in terms of Kelvin """
    logger = logging.getLogger()
    tUnits = tUnitsOrg.lower()    
    if tUnits == 'c' or tUnits == 'degc' or tUnits == 'degree celsius':
        tMag = tMagUnconv + 273.15
    elif tUnits == 'k' or tUnits == 'kelvin' or tUnits == 'degree kelvin':
        tMag = tMagUnconv
    elif tUnits == 'f' or tUnits == 'degf' or tUnits == 'degree fahrenheit':
        tMag = ((tMagUnconv - 32) / 1.8) + 273.15
    else:
        logger.info('WARNING: Temperature units encountered were not found in conversion '
                    'function. Proceeding with analysis with assumption that units '
                    'asssociated with data are using Kelvin.\n')
        tMag = tMagUnconv
    return tMag


def check_for_conflicting_loads(loadBCLib, thermal):
    """ This function compares the current thermal element to loads previously added to
        to the load library and throws an error if a conflict arises. EG, a surface heat
        flux is applied to a face of a plate, and then another, different surface
        heat flux is applied for the entire plate. With no way of determining precedence,
        an error is thrown. """
    loadType = thermal.get('LoadType')
    for entry in loadBCLib:
        if 'Treatment' in entry:
            if entry['Treatment'] == loadType:
                try:
                    cId = thermal.find('Component').get('ComponentID')             # applied to component
                except AttributeError:                                             # applied to geometry
                    continue
                    #cId = thermal.find('Geometry/Features/Feature').get('ComponentID')
                if entry['ComponentID'] == cId and float(entry[loadType]) != float(thermal.get('Value')):
                    cad_library.exitwitherror('Multiple constraints of treatment ' + loadType +
                                              ' are associated with ComponentID ' + str(cId) + '.'
                                              ' No way of determining precendence - analysis is' +
                                              ' aborting. Please correct your test bench.', -1, 'AbaqusCAE.py')

                    
def grabGeometry(geometryXML, loadBCLib):
    """ Store datum points associated with each load to be applied. """
    featuresXML = geometryXML.find('Features')                      # go to geometry features
    loadBCLib[-1].update([['GeometryType', featuresXML.get('GeometryType')]])
    loadBCLib[-1].update([['1stGeomQual', featuresXML.get('PrimaryGeometryQualifier')]])
    loadBCLib[-1].update([['2ndGeomQual', featuresXML.get('SecondaryGeometryQualifier')]])
    featureSetXML = featuresXML.getiterator('Feature')              # ^ store geometry settings ^
    loadBCLib[-1].update([['ComponentID', featureSetXML[0].get('ComponentID')]])  # assumes all points in same component
    pointNames = []
    for feature in featureSetXML:                                   # for each geometry feature:
        pointNames.append(feature.get('Name'))
    loadBCLib[-1].update([['Points', pointNames]])                   # store names of datum points
    

def generateSpecifiedTemperatureDict(thermal, loadBCLib):
    """ Generate applied temperature entry in load dictionary. """
    logger = logging.getLogger()
    logger.info("Defining specified temperature BC" + '\n')
    loadBCLib[-1].update([['Treatment', 'SpecifiedTemperature']])    # load is type "SpecifiedTemperature"
    tUnits = thermal.get('Unit')                              
    tMagUnconv = float(thermal.get('Value'))                        # magnitude of temperature
    tMag = tempConv(tMagUnconv, tUnits)
    loadBCLib[-1].update([['SpecifiedTemperature', tMag]])           # store temperature value

    logger.info("Specified Temperature Magnitude = " + str(tMag) + '\n')

    
def generateConvectionDict(thermal, loadBCLib, thermalSetXML):
    """ Generate convection entry in load dictionary. """
    logger = logging.getLogger()
    logger.info("Defining heat convection BC" + '\n')
    loadBCLib[-1].update([['Treatment', 'Convection']])
    # No conversion needed for convection: [J/(s.(m^2 K))] ==[W/(m^2K)]
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['Convection', tMag]])           # store temperature value

    # Look for ambient temperature load - REQUIRED for convection
    ambient = False
    for therm in thermalSetXML:
        if therm.get('LoadType') == 'AmbientTemperature':
            ambient = True
            sink = tempConv(float(therm.get('Value')), therm.get('Unit'))
            loadBCLib[-1].update([['AmbientTemperature', sink]])
    if not ambient:
        cad_library.exitwitherror('No ambient temperature found! Needed for convection load!', -1, 'AbaqusThermal.py')
    
    logger.info("Convection coefficient = " + str(tMag) + '\n')
    logger.info("Ambient Temperature = " + str(sink) + '\n')


def generateHeatFluxDict(thermal, loadBCLib):
    """ Generate surface heat flux entry in load dictionary. """
    logger = logging.getLogger()
    logger.info("Defining surface heat flux load" + '\n')
    loadBCLib[-1].update([['Treatment', 'HeatFlux']])
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['HeatFlux', tMag]])           # store temperature value

    logger.info("Surface Heat Flux = " + str(tMag) + '\n') 


def generateHeatGenerationDict(thermal, loadBCLib):
    """ Generate heat generation entry in load dictionary. """
    logger = logging.getLogger()
    logger.info("Defining heat generation" + '\n')
    loadBCLib[-1].update([['Treatment', 'HeatGeneration']])
    tMag = float(thermal.get('Value'))                        # magnitude of temperature
    loadBCLib[-1].update([['HeatGeneration', tMag]])           # store temperature value
    logger.info("Heat Generation = " + str(tMag) + '\n')        
    
    
def setInitialTemperature(thermal, myAsm, instName):
    """ Create and set initial temperature. Only applied to component level, not geometry. """
    logger = logging.getLogger()
    try:
        logger.info("Defining initial temperature for " + str(thermal.find('Component').get('ComponentID')) + '\n')
    except AttributeError:
        cad_library.exitwitherror('Initial temperature specified for geometric subset of component. ' +
                                  'Initial temperature can only be applied to component/assembly.', -1)
    tUnits = thermal.get('Unit')                              
    tMagUnconv = float(thermal.get('Value'))                        # magnitude of temperature
    tMag = tempConv(tMagUnconv, tUnits)
    logger.info("Initial Temperature Magnitude = " + str(tMag) + '\n')
    try:
        region = (myAsm.instances[instName].cells, )
        initTempName = instName + '-InitTemp'
        mdb.models['Model-1'].Temperature(name=initTempName, 
                                          createStepName='Initial', region=region, distributionType=UNIFORM,
                                          crossSectionDistribution=CONSTANT_THROUGH_THICKNESS,
                                          magnitudes=(tMag,))
    except:
        cad_library.exitwitherror('Error setting initial temperature.', -1, 'AbaqusThermal.py')


def generateThermalConstructs(thermalSetXML, loadBCLib, asminfo):
    """ Dispatch function to create entry in load dictionary for all thermal constructs. """
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
                if thermalLoadType == 'Convection':
                    generateConvectionDict(thermal, loadBCLib, thermalSetXML)
                else:
                    globals()[str('generate' + thermalLoadType + 'Dict')](thermal, loadBCLib)
            except KeyError:
                cad_library.exitwitherror('Specified thermal treatment is not yet supported.', -1, 'AbaqusThermal.py')
        if thermal.find('Geometry'):         # Apply construct only to specified geometry, otherwise to whole body
            if thermalLoadType == 'HeatGeneration':
                msg = 'Heat Generation block incorrectly applied to geometry construct within a component. ' + \
                      'Block should only be applied to entire component/assembly. Please revise ' + \
                      'your GME model and re-run simulation.'
                cad_library.exitwitherror(msg, -1, 'AbaqusThermal.py')
            grabGeometry(thermal.find('Geometry'), loadBCLib)
            loadBCLib[-1].update([['Component', False]])
        else:
            # If load is applied to whole component, need to check if component has children.
            # If it does, need to create library entries for each child, as the assembly isn't
            # added to Abaqus model, each of its children are.
            compChildren = asminfo.componentsdict[thermal.find('Component').get('ComponentID')].children
            if len(compChildren) > 0:
                # Finish current entry, but set ComponentID to first child
                loadBCLib[-1].update([['ComponentID', compChildren[0].cyphyid]])
                loadBCLib[-1].update([['Component', True]])      # Apply constraint to entire instance
                for child in xrange(1, len(compChildren)):
                    # For remaining children (if app.), copy previous entry under remaining ComponentIDs
                    # Can't use line below as set comprehension not available in Python 2.6
                    # loadBCLib.append({x: loadBCLib[-1][x] for x in loadBCLib[-1].keys()})
                    d = {}
                    for x in loadBCLib[-1].keys():
                        d[x] = loadBCLib[-1][x]
                    loadBCLib.append(d)
                    loadBCLib[-1]['ComponentID'] = compChildren[child].cyphyid
            else:
                loadBCLib[-1].update([['ComponentID', thermal.find('Component').get('ComponentID')]])
                loadBCLib[-1].update([['Component', True]])      # Apply constraint to entire instance


def createSpecifiedTemperatureConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    """ Apply specified temperature boundary condition in Abaqus model. """
    logger = logging.getLogger()
    try:
        if not entry['Component']:
            tempFace = entry['FaceIDs'][0]
            instances = entry['Instance']
            faces = myAsm.instances[instances].faces
            region = regionToolset.Region(faces=faces[tempFace:tempFace+1])
        else:
            region = (myAsm.instances[instName].cells, )   # Apply BC to entire part
            logger.info('Creating load/BC on entirety of ' + instName + ' with treatment Specified Temperature. \n')
        specTempName = regionName + '-SpecTemp'
        myModel.TemperatureBC(name=specTempName, createStepName=myStep.name,
                              region=region, magnitude=entry['SpecifiedTemperature'],
                              distributionType=UNIFORM, amplitude=amp)              # create specified temperature BC
    except:
        cad_library.exitwitherror('Error creating specified temperature constraint.', -1, 'AbaqusThermal.py')
        
        
def createConvectionConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    """ Apply convection interaction in Abaqus model. """
    logger = logging.getLogger()
    try:
        if not entry['Component']:
            regionMask = myMask(entry['FaceIDs'][0])
            maskRegion = myAsm.instances[instName].faces.getSequenceFromMask(mask=(regionMask,),)
        else:
            regionMask = myAsm.instances[instName].faces
            maskRegion = regionMask.getSequenceFromMask(mask=('[#ff ]', ), )    # Apply to entire body
            logger.info('Creating load/BC on entirety of ' + instName + ' with treatment Convection. \n')
        region = myAsm.Surface(side1Faces=maskRegion, name=regionName)
        convecName = regionName + '-ConvecHeat'
        myModel.FilmCondition(name=convecName, createStepName=myStep.name,
                              surface=region, definition=EMBEDDED_COEFF, filmCoeff=entry['Convection'],
                              filmCoeffAmplitude=amp, sinkTemperature=entry['AmbientTemperature'],
                              sinkAmplitude=amp)
    except:
        cad_library.exitwitherror('Error creating convection heat constraint.', -1, 'AbaqusThermal.py')


def createHeatFluxConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    """ Apply surface heat flux load in Abaqus model. """
    logger = logging.getLogger()
    try:
        if not entry['Component']:
            regionMask = myMask(entry['FaceIDs'][0])
            maskRegion = myAsm.instances[instName].faces.getSequenceFromMask(mask=(regionMask,),)
        else:
            regionMask = myAsm.instances[instName].faces
            maskRegion = regionMask.getSequenceFromMask(mask=('[#ff ]', ), )    # Apply to entire body
            logger.info('Creating load/BC on entirety of ' + instName + ' with treatment Surface Heat Flux. \n')
        region = myAsm.Surface(side1Faces=maskRegion, name=regionName)
        surfFluxName = regionName + '-SurfHeatFlux'
        myModel.SurfaceHeatFlux(name=surfFluxName, createStepName=myStep.name,
                                region=region, magnitude=entry['HeatFlux'], distributionType=UNIFORM,
                                amplitude=amp)
    except:
        cad_library.exitwitherror('Error creating surface heat flux constraint.', -1, 'AbaqusThermal.py')


def createHeatGenerationConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName):
    """ Apply heat generation load in Abaqus model. """
    logger = logging.getLogger()
    try:
        #MPdict = myAsm.getMassProperties(regions=(myAsm.instances[instName],),)
        #vol = MPdict['volume']
        #if vol is None:
         #   cad_library.exitwitherror('Error getting volume from part instance ' + instName, -1, 'AbaqusThermal.py')
        #else:
            #vol_heat_rate = float(entry['HeatGeneration']) / float(vol)
        logger.info('Creating load/BC on entirety of ' + instName + ' with treatment Heat Generation. \n')
        region = (myAsm.instances[instName].cells, )
        HeatGenName = regionName + '-HeatGen'
        myModel.BodyHeatFlux(name=HeatGenName, createStepName=myStep.name, region=region,
                             magnitude=float(entry['HeatGeneration']), distributionType=UNIFORM, amplitude=amp)
    except:
        cad_library.exitwitherror('Error creating the Heat Generation constraint.', -1, 'AbaqusThermal.py')
        

def createInitialTemperatureConstraint(myModel, myAsm, entry):
    """ Apply initial temperature boundary condition in Abaqus model. """
    try:
        instances = entry['Instance']
        region = (myAsm.instances[instances].cells, )
        initTempName = instances + '-InitTemp'
        myModel.Temperature(name=initTempName, createStepName='Initial',
                            region=region, distributionType=UNIFORM,
                            crossSectionDistribution=CONSTANT_THROUGH_THICKNESS,
                            magnitudes=entry['InitialTemperature'])
    except:
        cad_library.exitwitherror('Error creating initial temperature.', -1, 'AbaqusThermal.py')

    
def createThermalConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName=None):
    """ Dispatch function to create thermal boundary condition/load. """
    if regionName is None:
        try:
            regionName = 'BodyThermConst_' + str(randint(1, 1000))
        except AbaqusError:
            # try again in case generated int already was created
            regionName = 'BodyThermConst_' + str(randint(1, 1000))
    if entry['Treatment'] == 'InitialTemperature':
        createInitialTemperatureConstraint(myModel, myAsm, entry)
    try:
        globals()[str('create' + entry['Treatment'] + 'Constraint')](myModel, myAsm, entry, 
                                                                     myStep, amp, instName,
                                                                     regionName)
    except KeyError:
        if entry['Treatment'] == 'AmbientTemperature':
            pass
        else:
            cad_library.exitwitherror('Specified thermal treatment ' + entry['Treatment'] + ' is not yet supported.',
                                      -1, 'AbaqusThermal.py')
        
        
def defineThermalElements(myAsm, regionPick, args):
    """ Set thermal element type based on standard/explicit analysis. """
    try:
        if args.dynamicExplicit:
            elemLibrary = EXPLICIT
        else:
            elemLibrary = STANDARD
        elemType1 = mesh.ElemType(elemCode=C3D20RT, elemLibrary=elemLibrary)
        elemType2 = mesh.ElemType(elemCode=UNKNOWN_WEDGE, elemLibrary=elemLibrary)
        elemType3 = mesh.ElemType(elemCode=C3D10MT, elemLibrary=elemLibrary,
                                  secondOrderAccuracy=OFF, hourglassControl=DEFAULT,
                                  distortionControl=DEFAULT)
        myAsm.setMeshControls(regions=regionPick, elemShape=TET, technique=FREE, sizeGrowthRate=1.05)
        myAsm.setElementType(regions=(regionPick,), elemTypes=(elemType1, elemType2, elemType3,))
    except:
        cad_library.exitwitherror('Error trying to assign thermal-type elements for mesh.', -1, 'AbaqusThermal.py')


def convert_postprocessing_metric(metric, value, inUnit, outUnit):
    """ If GME testbench metrics have either degrees Celsius or Fahrenheit,
        convert data from Kelvin to desired units.
    """
    logger = logging.getLogger()
    logger.info('Converting ' + metric + ' of ' + str(value) + ' ' + inUnit + ' to ' + outUnit)
    if str(outUnit) == 'Degree Celsius':
        return float(value - 273.15)
    elif outUnit == 'Degree Fahrenheit':
        return float(1.8*(value - 273.15) + 32)