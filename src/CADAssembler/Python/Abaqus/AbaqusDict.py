"""

AbaqusDict.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which create the dictionaries storing
      the essential data.

"""

import os, re
from numpy import array, cross, transpose, vstack, dot
import numpy.linalg as LA
import string as STR
import xml.etree.ElementTree as ET
import _winreg, sys, ctypes, uuid, traceback
import logging

from AbaqusGeometry import toBC
from AbaqusThermal import *

# Unit conversion dictionary uses mmNs, more convenient than mmKs
def generateConvDict(unitScale):
    conv = {}                                                           # unit conversion dictionary
    conv.update([['GPa',(10**9)*(unitScale**2)],                        # to N/length^2
                 ['MPa',(10**6)*(unitScale**2)],                        # to N/length^2
                 ['kPa',(10**3)*(unitScale**2)],                        # to N/length^2
                 ['pascal',unitScale**2],                               # to N/length^2
                 ['Pa',unitScale**2],                                   # to N/length^2
                 ['g/cm^3',(10**6)*(unitScale**3)],                     # to (N-s^2/length)/length^3
                 ['kg/m^3',(10**-3)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['kg/mm^3',(10**6)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['N',1],                                               # to N
                 ['kN',10**3],                                          # to N
                 ['N',1],                                               # to N
                 ['N-mm',(10**-3)*(unitScale**-1)],                     # to N-length
                 ['m/s^2',1],                                           # to length/s^2
                 ['mm/s^2',(10**-3)/unitScale],                         # to length/s^2
                 ['N/A',1],                                             # dimensionless
                 ['1/C',1],                                             # to 1/C
                 ['C',1],                                               # to Celsius (temperature values will be converted somewhere else)
                 ['1/K',1],                                             # to 1/K
                 ['K',1],                                               # to Kelvin (temperature values will be converted somewhere else)
                 ['F',1],                                               # to Fahrenheit (temperature values will be converted somewhere else)
                 ['W/(m*K)',unitScale],                                 # to (N-length/s)/(K-length)
                 ['J/(kg*K)',unitScale**-2],                            # to (N-length)/(K-(N-s^2/length))
                 ['J/(kg-K)',unitScale**-2]])                           # to (N-length)/(K-(N-s^2/length))

    return conv
    
    
def generateInstRef(metricComponentsXML, metricCADComponentsXML, 
                    xmlMetricsName, jointsXML, xmlName, runAdams, args):
    logger = logging.getLogger()
    
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
        instance_num = 0
        
        merged_asms = []
        if jointsXML.find('JointsMetaData') is not None:
            for comp in jointsXML.find('JointsMetaData').getchildren():
                if comp.get('Primary') == 'LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS' \
                  and comp.get('Secondary') == 'TREAT_AS_ONE_BODY':
                    merged_asms.append(comp.get('ComponentInstanceID'))
            logger.info("Sub-Assemblies to be merged:" + str(merged_asms))
        
        for cadcomp in metricCADComponentsXML.findall('CADComponent'):
            # The following line doesn't work in Python 2.6 (used w/ Abaqus - must loop each time)
            # Otherwise you could search by attribute value
            #m_comp = metricComponentsXML.findall("MetricComponent[@MetricID='%s']" % str(cadcomp.get('MetricID'))) 
            for component in metricComponentsXML.findall('MetricComponent'):
                if component.get('MetricID') == cadcomp.get('MetricID'):
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
                        instance_num += 1
                        inst = str(instName + '__Z' + str(instance_num))
                        instRef.update([[inst,{}]])
                        instRef[inst].update([['Name',instName]])
                        instRef[inst].update([['isPart',isPart]])
                        instRef[inst].update([['ComponentID',componentInstanceID]])
                        try:
                            instRef[inst].update([['MaterialID',materialID]])
                        except:
                            pass
                    
                        instRef[inst].update([['isShell',
                            "123zzzz123333331312121" in component.get('Name').lower()]])                                                    # temporarily deactivating 'isShell' key
                        if runAdams or args.rigidParts:                          
                            instRef[inst].update([['isRigid',
                                component.get('Rigidity')=="RIGID"]])
                        else:
                            instRef[inst].update([['isRigid',False]])
                        instIndex.update([[componentInstanceID,inst]])
              
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
                                            instRef[inst].update([['isRigid',"RIGID"]])
                                            instRef[inst].update([['MaterialID',instName+"_Rigid"]])

                                            if materialID == "undefined":
                                                instRef[inst].update([['MaterialID',"steel_low_alloy_a572_gr50"]])
                                                
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
                        instName = STR.upper(component.get('Name')) + '_ASM'
                        isPart = False
                        metricID = component.get('MetricID')
                        for event, elem in ET.iterparse(xmlMetricsName, events=('start', 'end')):
                            if event == 'start' and elem.tag == 'CADComponent':
                                if metricID == elem.get('MetricID'):
                                    componentInstanceID = elem.get('ComponentInstanceID')
                        instance_num += 1
                        inst = str(instName + '__Z' + str(instance_num))
                        instAssemblyRef.update([[inst,{}]])
                        instAssemblyRef[inst].update([['Name',instName]])
                        instAssemblyRef[inst].update([['isPart',isPart]])
                        instAssemblyRef[inst].update([['ComponentID',componentInstanceID]])
                        instAssemblyRef[inst].update([['MaterialID','N/A']])
                        if runAdams:
                            instAssemblyRef[inst].update([['isRigid',True]])
                        else:
                            instAssemblyRef[inst].update([['isRigid',False]])
                        instAssemblyRef[inst].update([['isShell',
                            "123zzzz123333331312121" in component.get('Name').lower()]])                                                    # temporarily deactivating 'isShell' key
                        instAssemblyIndex.update([[componentInstanceID,inst]])
                        
                        # Check if leaf components are to be merged or not
                        if componentInstanceID in merged_asms:
                            # If being merged, rigidity constraint needs to be relaxed
                            instAssemblyRef[inst]['isRigid'] = False
                            instAssemblyRef[inst].update([['MergeChildren',True]])
                            # Assembly will be at the part level of assembly, so add to instRef
                            instRef.update([[inst,{}]])
                            instRef[inst].update(instAssemblyRef[inst])
                        else:
                            instAssemblyRef[inst].update([['MergeChildren',False]])

                    break  # Desired CADComponent found, break out of searching loop
                else:
                    pass   # MetricComponent.get('MetricID'_) != CADComponent.get('MetricID')
        
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in creating necessary data arrays\n')
        logger.error('Possible causes of error are:\n')
        logger.error('1- Unknown component type definiton (not Part or Assembly) in CADAssembly_metrics.xml file, \n')
        logger.error('2- Missing information in the CADAssembly_metrics.xml file Part definition(i.e. Name, MetricID, Material, Type, ComponentInstanceID\n')
        logger.error('3- Missing information in the CADAssembly.xml file Part definition(i.e. ComponentID, MaterialID, ManufacturingParameter, Scalars, Mass, Volume\n')
        logger.error('4- Missing information in the CADAssembly_metrics.xml file Assembly definition(i.e. Name, MetricID, ComponentInstanceID\n')
        raise

    return (instRef, instIndex, instAssemblyRef, instAssemblyIndex,
            rigidPartMasses, rigidPartVolumes, rigidPartDensities,
            rigidParts, rigidPartPresent, Jan24_deactivate_rigidity)
            
            
def generateMtrlRef(LIBRARY_MANAGER, metricComponentsXML, metricCADComponentsXML, thermalSetXML, rigidParts, unitScale, conv):
    logger = logging.getLogger()

    mtrlRef = {}   # Set to contain a dictionary for each material
    logger.info("**********************************************************************************" + '\n')
    logger.info("Grabbing properties of each material from the material library" + '\n')
    logger.info('Material properties that will be grabbed are density, modulus of elasticity, poisson\'s ratio,\n')
    logger.info('ultimate tensile strength, fatigue strength, yield strength\n')
    # Set of names of material properties to get
    mtrlProps = ["density","mechanical__modulus_elastic","mechanical__ratio_poissons",
        "mechanical__strength_tensile_ultimate","mechanical__strength_fatigue","mechanical__strength_tensile_yield"]
    if thermalSetXML:
        logger.info('coefficient of thermal expansion, conductivity, specific heat\n')
        mtrlProps.extend(["thermal__coefficient_expansion_linear", "thermal__conductivity", "thermal__capacity_specific_heat"])
        
    i = 1
    for cadcomp in metricCADComponentsXML.findall('CADComponent'): 
        for component in metricComponentsXML.findall('MetricComponent'):
            if component.get('MetricID') == cadcomp.get('MetricID'):
                i += 1
                rigidPart = False
                try:
                    instName = STR.upper(component.get('Name'))
                except:
                    logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                    logger.error('Missing part name in CADAssembly_metrics.xml.\n')
                    raise

                try:    
                    for key in rigidParts:
                        if key == instName:
                            rigidPart = True
                            for rigidPartDensityNumAndName in rigidPartDensities:
                                if rigidPartDensityNumAndName[0] == key:
                                    rigidDensity = rigidPartDensityNumAndName[1]
                except:
                    logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                    logger.error('Rigid part definition discrepancy\n')
                    raise                
                    
                if component.get("Type") == "PART" and not rigidPart:
                    try:
                        mtrl = component.find('Material')
                    except:
                        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                        logger.error('ERROR: Missing material for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                        raise
                    try:
                        mtrlName = mtrl.get('Type').lower()
                    except:
                        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                        logger.error('Missing material type for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                        raise  

                    mtrlRef.update([[mtrlName,{}]])
                        
                    try:
                        # get mtrl data from the material library for the corresponding material
                        mtrlData = LIBRARY_MANAGER.materialData(mtrlName)
                    except:
                        logger.error(mtrlName + " is not in the material library. Material is" + \
                                " assigned to " + instName + '.\n')
                        raise ValueError("ERROR: " + mtrlName + \
                                " is not in the material library. Material is assigned to " + \
                                instName + '.\n')
                    for prop in mtrlProps:
                        if mtrlData[prop]["value"]=="None":
                            logger.error("Property " + prop + " of " + mtrlName + \
                                " has not been defined in the material library." + '\n')
                            raise ValueError("ERROR: Property " + prop + " of " + mtrlName + \
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
                        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                        logger.error('Missing material for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                        raise
                    try:
                        mtrlName = mtrl.get('Type').lower()
                    except:
                        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                        logger.error('Missing material type for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                        raise
                    
                    mtrlNameRigid = instName + "_Rigid"
                    mtrlRef.update([[mtrlNameRigid ,{}]])
                    mtrlDataRigid = {'density': {'value': 'None','unit': 'None'}, 
                                     'mechanical__modulus_elastic': {'value': 'None','unit': 'None'}, 
                                     'mechanical__ratio_poissons': {'value': 'None'},
                                     'mechanical__strength_tensile_ultimate': {'value': 'None','unit': 'None'}, 
                                     'mechanical__strength_fatigue': {'value': 'None','unit': 'None'},
                                     'mechanical__strength_tensile_yield': {'value': 'None','unit': 'None'} }
                        
                    try:
                        # get mtrl data from the material library for the corresponding material
                        mtrlData = library_manager.materialData(mtrlName)                       
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
                    logger.info('Grabbing material properties of ' + str(mtrlName) + ' for the part ' + str(instName))
                break  # CADComponent & MetricID match, break out of search loop
            else:
                pass # CADComponent & MetricID do not match, continue searching
    logger.info("**********************************************************************************" + '\n')
    
    return mtrlRef     


def generateMetricRef(metricSetXML, metricsComponentSetXML, instIndex, instAssemblyIndex):
    logger = logging.getLogger()
    metricRef = {}
    metricRef2 = {}
    metricRef3 = {}
    allAssembly = True
    
    try:
        for metric in metricSetXML:
            if metric.get('ComponentID') in instIndex or metric.get('ComponentID') in instAssemblyIndex:
                allAssembly = False
                if not metric.get('MetricType') in metricRef:
                    metricRef.update([[metric.get('MetricType'),{}]])
                metricRef[metric.get('MetricType')].update([[
                    instIndex[metric.get('ComponentID')],None]])
        for metrics in metricsComponentSetXML:
            if metrics.get('ComponentID') in instIndex or metric.get('ComponentID') in instAssemblyIndex:
                metricRef2.update([[metrics.get('MetricID'),metrics.get('MetricType')]])
                metricRef3.update([[metrics.get('MetricID'),metrics.get('ComponentID')]])
    except:
        allAssembly = True
        logger.info('WARNING: In the FEA testbench there are no individual parts, just the assembly. Otherwise there might be an error while\n')
        logger.info('         MetricType, ComponentID or MetricID values are being grabbed from CADAssembly.xml\n')
    
    return (metricRef, metricRef2, metricRef3, allAssembly) 

    
def generateLoadBCLib(analysisConstraintSetXML, feaXML, thermalSetXML, conv):
    logger = logging.getLogger()
    logger.info("Creating load and boundary condition dictionaries" + '\n')
    try:
        loadBCLib = []                                                  # set to contain a dictionary for each load and BC
        i = 0
        if analysisConstraintSetXML:
            for constraint in analysisConstraintSetXML:                     # for each constraint definition:
                loadBCLib.append({})                                            # add new dictionary
                i += 1
                dispConstraintXML = constraint.find('Displacement')         # look for displacement constraint                
                pinConstraintXML = constraint.find('Pin')                       # look for pin constraint
                if ET.iselement(dispConstraintXML):                             # if displacement constraint found:
                    logger.info("Defining displacement constraint" + '\n')

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

                    logger.info("Rotation in x-dir = " + str(xRot) + '\n')
                    logger.info("Rotation in y-dir = " + str(yRot) + '\n')
                    logger.info("Rotation in z-dir = " + str(zRot) + '\n')
                    logger.info("Translation in x-dir = " + str(xTrans) + '\n')
                    logger.info("Translation in y-dir = " + str(yTrans) + '\n')
                    logger.info("Translation in z-dir = " + str(zTrans) + '\n')
                    
                elif ET.iselement(pinConstraintXML):                            # if pin constraint found:
                    logger.info("Defining pin constraint" + '\n')

                    loadBCLib[-1].update([['Treatment','Pin']])                     # face is labeled "Pin"
                    axialDispXML = pinConstraintXML.find('AxialDisplacement')
                    pinDisp = toBC(axialDispXML.get('Property'))                    # get axial displacement parameter
                    loadBCLib[-1].update([['AxialDisp',pinDisp]])                   # store SymbolicConstant
                    axialRotXML = pinConstraintXML.find('AxialRotation')
                    pinRot = toBC(axialRotXML.get('Property'))                      # get axial rotation parameter
                    loadBCLib[-1].update([['AxialRot',pinRot]])                     # store SymbolicConstant

                    logger.info("Pin Dispacement = " + str(pinDisp) + '\n')
                    logger.info("Pin Rotation = " + str(pinRot) + '\n')
                    
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
                logger.info("Defining Force/Moment" + '\n')
                
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

                logger.info("Force in x-dir = " + str(xForce) + '\n')
                logger.info("Force in y-dir = " + str(yForce) + '\n')
                logger.info("Force in z-dir = " + str(zForce) + '\n')
                logger.info("Moment in x-dir = " + str(xMoment) + '\n')
                logger.info("Moment in y-dir = " + str(yMoment) + '\n')
                logger.info("Moment in z-dir = " + str(zMoment) + '\n')
                
            elif ET.iselement(pressureXML):                                 # if Pressure load found:
                logger.info("Defining pressure load" + '\n')
                
                loadBCLib[-1].update([['Treatment','Pressure']])                # load is type "Pressure"
                pUnits = 'MPa'                                                  ### manually assigning units (not given)
                pMag = float(pressureXML.get('Value'))*conv[pUnits]             # magnitude of pressure
                loadBCLib[-1].update([['Pressure',pMag]])                       # store pressure value
                hasGeometry = True

                logger.info("Pressure Magnitude = " + str(pMag) + '\n')
            elif ET.iselement(accelXML):
                logger.info("Defining acceleration load" + '\n')

                aUnits = accelXML.get('Units')
                accel['x'] = float(accelXML.get('x'))*conv[aUnits]
                accel['y'] = float(accelXML.get('y'))*conv[aUnits]
                accel['z'] = float(accelXML.get('z'))*conv[aUnits]

                logger.info("Acceleration in x-dir = " + str(accel['x']) + '\n')
                logger.info("Acceleration in y-dir = " + str(accel['y']) + '\n')
                logger.info("Acceleration in z-dir = " + str(accel['z']) + '\n')
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

        if thermalSetXML is not None:
            generateThermalConstructs(thermalSetXML, loadBCLib)
                
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in creating load and boundary condition dictionaries\n')
        raise
    
    return (loadBCLib, accel)
    

def generateSuperRef(instRef, instAssemblyRef, mtrlRef, metricRef, metricRef2, metricRef3, 
                     instIndex, loadBCLib, unitLength, allAssembly):
    logger = logging.getLogger()
    superRef = {}
    try:
        superRef.update([['instRef',instRef],
                         ['instAssemblyRef',instAssemblyRef],   
                         ['mtrlRef',mtrlRef],
                         ['metricRef',metricRef],
                         ['metricRef2',metricRef2],
                         ['metricRef3',metricRef3],
                         ['instIndex',instIndex],
                         ['loadBCLib',loadBCLib],
                         ['lengthUnit',unitLength],
                         ['allAssembly',allAssembly]])
    except:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Cannot define the super dictionary, probably because of input discrepancies\n')
        raise    

    return superRef   