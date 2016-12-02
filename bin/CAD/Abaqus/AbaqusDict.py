"""

AbaqusDict.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which create the dictionaries storing
      the essential data.

"""

import string as STR
import xml.etree.ElementTree as ET
import logging
import cad_library
import cad_library.material_properties as material_properties
import cad_library.allowable_stress_levels as allowable_stress_levels
import cad_library.unit_utils as unit_utils
from AbaqusGeometry import toBC
from AbaqusThermal import *

    
def generateInstRef(asminfo, xmlName, uniqueSuffix, asmIdentifier, runAdams, args):
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

        # For kinematic SOTs
        merged_asms = []
        for child in asminfo.componentsdict.values():
            if child.is_assembly() and not child.kinematic:
                merged_asms.append(child.cyphyid)
        if len(merged_asms) > 0:
            logger.info("Sub-Assemblies to be merged:" + str(merged_asms))

        for child in asminfo.componentsdict.values():
            if child.cyphyid == asminfo.root.cyphyid:
                # System under test - ignore this as it isn't a component
                continue
            if not child.is_assembly():
                instName = STR.upper(child.cadmodelname)
                inst = instName + uniqueSuffix + str(child.steporder + 1)
                logger.info('instRef Unique Name: ' + inst)
                componentInstanceID = child.cyphyid
                material = child.materialname.lower()

                instRef.update([[inst, {}]])
                instRef[inst].update([['Name', instName]])
                instRef[inst].update([['isPart', True]])
                instRef[inst].update([['ComponentID', componentInstanceID]])
                instRef[inst].update([['MaterialID', material]])

                # temporarily deactivating 'isShell' key
                instRef[inst].update([['isShell', "123zzzz123333331312121" in instName.lower()]])
                if runAdams or args.rigidParts:
                    #TODO: RESOLVE THIS - There is no "rigidity" attribute
                    instRef[inst].update([['isRigid', False]])
                                         #component.get('Rigidity') == "RIGID"]])
                else:
                    instRef[inst].update([['isRigid', False]])
                instIndex.update([[componentInstanceID, inst]])

                # This grabs the relevant CADComponent element from CADAssembly.XML
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
                                    instRef[inst].update([['isRigid', True]])
                                    instRef[inst].update([['MaterialID', instName+"_Rigid"]])

                                    if materialID == "undefined":
                                        instRef[inst].update([['MaterialID', "steel_low_alloy_a572_gr50"]])

                                    mass = child.scalars['Mass']
                                    conversionFactor = 1
                                    if mass.unit == 'kg':
                                        conversionFactor = 1
                                    elif mass.unit == 'g':
                                        conversionFactor = 0.001
                                    elif mass.unit == 'tonne' or mass.unit == 'ton' \
                                            or mass.unit == 'tons':
                                        conversionFactor = 1000
                                    convertedMass = float(mass.value) * conversionFactor

                                    conversionFactor = 1
                                    volume = child.scalars['Volume']
                                    if volume.unit == 'mm3' or volume.unit == 'mm^3':
                                        conversionFactor = 1
                                    elif volume.unit == 'cm3' or volume.unit == 'cm^3':
                                        conversionFactor = 1000
                                    elif volume.unit == 'dm3' or volume.unit == 'dm^3':
                                        conversionFactor = 1000000
                                    elif volume.unit == 'm3' or volume.unit == 'm^3':
                                        conversionFactor = 1000000000
                                    convertedVolume = float(volume.value) * conversionFactor

                                    density = convertedMass/convertedVolume

                                    rigidPartMasses.append((instName, convertedMass))
                                    rigidPartVolumes.append((instName, convertedVolume))
                                    rigidPartDensities.append((instName, density))
                                    rigidParts.append(instName)
                                    rigidPartPresent = True
            elif child.is_assembly():
                instName = STR.upper(child.cadmodelname) + asmIdentifier
                inst = str(instName + uniqueSuffix + str(child.steporder + 1))
                logger.info('instRef Unique Name: ' + inst)
                componentInstanceID = child.cyphyid

                instAssemblyRef.update([[inst, {}]])
                instAssemblyRef[inst].update([['Name', instName]])
                instAssemblyRef[inst].update([['isPart', False]])
                instAssemblyRef[inst].update([['ComponentID', componentInstanceID]])
                instAssemblyRef[inst].update([['MaterialID', 'N/A']])
                if runAdams:
                    instAssemblyRef[inst].update([['isRigid', True]])
                else:
                    instAssemblyRef[inst].update([['isRigid', False]])

                # temporarily deactivating 'isShell' key
                instAssemblyRef[inst].update([['isShell', "123zzzz123333331312121" in
                                               instName.lower()]])
                instAssemblyIndex.update([[componentInstanceID, inst]])

                # Check if leaf components are to be merged or not
                if componentInstanceID in merged_asms:
                    # If being merged, rigidity constraint needs to be relaxed
                    instAssemblyRef[inst]['isRigid'] = False
                    instAssemblyRef[inst].update([['MergeChildren', True]])
                    # Assembly will be at the part level of assembly, so add to instRef
                    instRef.update([[inst, {}]])
                    instRef[inst].update(instAssemblyRef[inst])
                else:
                    instAssemblyRef[inst].update([['MergeChildren', False]])
    except:
        msg = ('\nError in creating necessary data arrays\nPossible causes of error are:\n' +
               '1- Unknown component type definiton (not Part or Assembly) in metrics xml file,\n' +
               '2- Missing information in the metrics xml file Part definition ' +
               '(i.e. Name, MetricID, Material, Type, ComponentInstanceID\n' +
               '3- Missing information in the ' + xmlname + ' file Part definition(i.e. ComponentID, ' +
               'MaterialID, ManufacturingParameter, Scalars, Mass, Volume\n' +
               '4- Missing information in the metrics xml file Assembly definition ' +
               '(i.e. Name, MetricID, ComponentInstanceID\n')
        cad_library.exitwitherror(msg, -1, 'AbaqusDict.py')

    return (instRef, instIndex, instAssemblyRef, instAssemblyIndex,
            rigidPartMasses, rigidPartVolumes, rigidPartDensities,
            rigidParts, rigidPartPresent, Jan24_deactivate_rigidity)
            
            
def generateMtrlRef(asminfo, thermalSetXML, rigidParts, rigidPartDensities, conv):
    logger = logging.getLogger()

    mtrlRef = {}   # Set to contain a dictionary for each material
    materials = []
    logger.info("**********************************************************************************" + '\n')
    logger.info("Grabbing properties of each material from the material library" + '\n')
    logger.info('Material properties that will be grabbed are density, modulus of elasticity, poisson\'s ratio,\n')
    logger.info('ultimate tensile strength, fatigue strength, yield strength\n')
    # Set of names of material properties to get
    mtrlProps = ["density", "mechanical__modulus_elastic", "mechanical__ratio_poissons",
                 "mechanical__strength_tensile_ultimate", "mechanical__strength_fatigue",
                 "mechanical__strength_tensile_yield"]
    if thermalSetXML:
        logger.info('coefficient of thermal expansion, conductivity, specific heat\n')
        mtrlProps.extend(["thermal__coefficient_expansion_linear", "thermal__conductivity",
                          "thermal__capacity_specific_heat"])

    for child in asminfo.componentsdict.values():
        if child.cyphyid == asminfo.root.cyphyid:
            continue  # Ignore root entry
        rigidPart = False
        instName = STR.upper(child.cadmodelname)
        try:
            for key in rigidParts:
                if key == instName:
                    rigidPart = True
                    for rigidPartDensityNumAndName in rigidPartDensities:
                        if rigidPartDensityNumAndName[0] == key:
                            rigidDensity = rigidPartDensityNumAndName[1]
        except:
            cad_library.exitwitherror('Rigid part definition discrepancy.', -1, 'AbaqusDict.py')

        if child.is_assembly():
            continue

        material = child.materialname.lower()
        if material == '':
            cad_library.exitwitherror('ERROR: Missing material for part ' + str(instName) +
                                      ' in metrics xml.', -1, 'AbaqusDict.py')

        if rigidPart:
            mtrlRef['RigidMaterial'] = material_properties.AnalysisMaterialProperties('RigidMaterial')
            mtrlRef['RigidMaterial'].density = rigidDensity
            mtrlRef['RigidMaterial'].density_unit = "kg/mm^3"
            mtrlRef['RigidMaterial'].density_defined = True
            mtrlRef['RigidMaterial'].mechanical__modulus_elastic = 200000
            mtrlRef['RigidMaterial'].mechanical__modulus_elastic_unit = "MPa"
            mtrlRef['RigidMaterial'].mechanical__modulus_elastic_defined = True
            mtrlRef['RigidMaterial'].mechanical__ratio_poissons = 0.3
            mtrlRef['RigidMaterial'].mechanical__ratio_poissons_defined = True
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_ultimate = 400
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_ultimate_unit = "MPa"
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_ultimate_defined = True
            mtrlRef['RigidMaterial'].mechanical__strength_fatigue = 200
            mtrlRef['RigidMaterial'].mechanical__strength_fatigue_unit = "MPa"
            mtrlRef['RigidMaterial'].mechanical__strength_fatigue_defined = True
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_yield = 250
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_yield_unit = "MPa"
            mtrlRef['RigidMaterial'].mechanical__strength_tensile_yield_defined = True
        else:
            materials.append(material)

        logger.info('Grabbing material properties of ' + str(material) + ' for the part ' + str(instName))
    logger.info("**********************************************************************************" + '\n')

    # At this point all materials have been appended to 'materials'
    material_properties.get_props_from_material_library(materials, mtrlRef)

    # Make sure all material properties are provided
    for k in mtrlRef.keys():
        m = mtrlRef[k]
        structprops = [m.density_defined, m.mechanical__modulus_elastic_defined, m.mechanical__ratio_poissons_defined,
                       m.mechanical__strength_tensile_yield_defined, m.mechanical__strength_tensile_ultimate_defined,
                       m.mechanical__strength_fatigue_defined]
        if not any(structprops):
            cad_library.exitwitherror('Structural material properties not fully defined for ' + k, -1, 'AbaqusDict.py')
        if thermalSetXML:
            thermalprops = [m.thermal__coefficient_expansion_linear_defined, m.thermal__conductivity,
                            m.thermal__capacity_specific_heat]
            if not any(thermalprops):
                cad_library.exitwitherror('Thermal material properties not fully define for ' + k, -1, 'AbaqusDict.py')

    # Convert the extracted properties to units to be used in Abaqus.
    unit_utils.convert_material_properties_given_conversion_dict(mtrlRef, conv)

    return mtrlRef

    
def generateLoadBCLib(analysisConstraintSetXML, feaXML, thermalSetXML, conv, asminfo):
    logger = logging.getLogger()
    logger.info("Creating load and boundary condition dictionaries" + '\n')
    try:
        loadBCLib = []                                                # set to contain a dictionary for each load and BC
        i = 0
        if analysisConstraintSetXML:
            for constraint in analysisConstraintSetXML:                     # for each constraint definition:
                loadBCLib.append({})                                            # add new dictionary
                i += 1
                dispConstraintXML = constraint.find('Displacement')         # look for displacement constraint
                pinConstraintXML = constraint.find('Pin')                       # look for pin constraint
                if ET.iselement(dispConstraintXML):                             # if displacement constraint found:
                    logger.info("Defining displacement constraint" + '\n')

                    loadBCLib[-1].update([['Treatment', 'Displacement']])            # face is labeled "Displacement"
                    rotConstraintXML = dispConstraintXML.find('Rotation')
                    xRot = toBC(rotConstraintXML.get('x'))                          # rotation about 1 axis
                    yRot = toBC(rotConstraintXML.get('y'))                          # rotation about 2 axis
                    zRot = toBC(rotConstraintXML.get('z'))                          # rotation about 3 axis
                    loadBCLib[-1].update([['RotDOF', (xRot, yRot, zRot)]])           # store degrees of freedom as tuple
                    transConstraintXML = dispConstraintXML.find('Translation')
                    xTrans = toBC(transConstraintXML.get('x'))                      # translation in 1 axis
                    yTrans = toBC(transConstraintXML.get('y'))                      # translation in 2 axis
                    zTrans = toBC(transConstraintXML.get('z'))                      # translation in 3 axis
                    loadBCLib[-1].update([['TransDOF', (xTrans, yTrans, zTrans)]])     # store DOF as tuple

                    logger.info("Rotation in x-dir = " + str(xRot) + '\n')
                    logger.info("Rotation in y-dir = " + str(yRot) + '\n')
                    logger.info("Rotation in z-dir = " + str(zRot) + '\n')
                    logger.info("Translation in x-dir = " + str(xTrans) + '\n')
                    logger.info("Translation in y-dir = " + str(yTrans) + '\n')
                    logger.info("Translation in z-dir = " + str(zTrans) + '\n')

                elif ET.iselement(pinConstraintXML):                            # if pin constraint found:
                    logger.info("Defining pin constraint" + '\n')

                    loadBCLib[-1].update([['Treatment', 'Pin']])                     # face is labeled "Pin"
                    axialDispXML = pinConstraintXML.find('AxialDisplacement')
                    pinDisp = toBC(axialDispXML.get('Property'))                    # get axial displacement parameter
                    loadBCLib[-1].update([['AxialDisp', pinDisp]])                   # store SymbolicConstant
                    axialRotXML = pinConstraintXML.find('AxialRotation')
                    pinRot = toBC(axialRotXML.get('Property'))                      # get axial rotation parameter
                    loadBCLib[-1].update([['AxialRot', pinRot]])                     # store SymbolicConstant

                    logger.info("Pin Dispacement = " + str(pinDisp) + '\n')
                    logger.info("Pin Rotation = " + str(pinRot) + '\n')

                geometryXML = constraint.find('Geometry')
                featuresXML = geometryXML.find('Features')                      # go to geometry features
                loadBCLib[-1].update([['GeometryType', featuresXML.get('GeometryType')]])
                loadBCLib[-1].update([['1stGeomQual', featuresXML.get('PrimaryGeometryQualifier')]])
                loadBCLib[-1].update([['2ndGeomQual', featuresXML.get('SecondaryGeometryQualifier')]])
                featureSetXML = featuresXML.getiterator('Feature')              # ^^store geometry settings
                loadBCLib[-1].update([['ComponentID', featureSetXML[0].get('ComponentID')]])
                pointNames = []
                for feature in featureSetXML:                                   # for each geometry feature:
                    pointNames.append(feature.get('Name'))                          # store new point name
                loadBCLib[-1].update([['Points', pointNames]])                   # store names of datum points
        loadSetXML = feaXML.getiterator('Load')
        accel = dict(x=0, y=0, z=0)
        for load in loadSetXML:                                         # for each load definition:
            loadBCLib.append({})                                            # add new dictionary
            forceMomentXML = load.find('ForceMoment')                       # look for ForceMoment load
            pressureXML = load.find('Pressure')                             # look for Pressure load
            accelXML = load.find('Acceleration')                            # look for Acceleration load
            if ET.iselement(forceMomentXML):                                # if ForceMoment load found:
                logger.info("Defining Force/Moment" + '\n')

                loadBCLib[-1].update([['Treatment', 'ForceMoment']])             # load is type "ForceMoment"
                forceXML = forceMomentXML.find('Force')
                fUnits = forceXML.get('Units')                                  # get force units
                xForce = float(forceXML.get('x'))*conv[fUnits]                  # force along 1 axis
                yForce = float(forceXML.get('y'))*conv[fUnits]                  # force along 2 axis
                zForce = float(forceXML.get('z'))*conv[fUnits]                  # force along 3 axis
                loadBCLib[-1].update([['Force', (xForce, yForce, zForce)]])        # store forces as tuple
                momentXML = forceMomentXML.find('Moment')
                mUnits = momentXML.get('Units')                                 # get moment units
                xMoment = float(momentXML.get('x'))*conv[mUnits]                # moment about 1 axis
                yMoment = float(momentXML.get('y'))*conv[mUnits]                # moment about 2 axis
                zMoment = float(momentXML.get('z'))*conv[mUnits]                # moment about 3 axis
                loadBCLib[-1].update([['Moment', (xMoment, yMoment, zMoment)]])    # store moments as tuple
                hasGeometry = True

                logger.info("Force in x-dir = " + str(xForce) + '\n')
                logger.info("Force in y-dir = " + str(yForce) + '\n')
                logger.info("Force in z-dir = " + str(zForce) + '\n')
                logger.info("Moment in x-dir = " + str(xMoment) + '\n')
                logger.info("Moment in y-dir = " + str(yMoment) + '\n')
                logger.info("Moment in z-dir = " + str(zMoment) + '\n')

            elif ET.iselement(pressureXML):                                 # if Pressure load found:
                logger.info("Defining pressure load" + '\n')

                loadBCLib[-1].update([['Treatment', 'Pressure']])                # load is type "Pressure"
                pUnits = 'MPa'                                                  # manually assigning units (not given)
                pMag = float(pressureXML.get('Value'))*conv[pUnits]             # magnitude of pressure
                loadBCLib[-1].update([['Pressure', pMag]])                       # store pressure value
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
                loadBCLib[-1].update([['GeometryType', featuresXML.get('GeometryType')]])
                loadBCLib[-1].update([['1stGeomQual', featuresXML.get('PrimaryGeometryQualifier')]])
                loadBCLib[-1].update([['2ndGeomQual', featuresXML.get('SecondaryGeometryQualifier')]])
                featureSetXML = featuresXML.getiterator('Feature')              # ^ store geometry settings ^
                # assumes all points in same component
                loadBCLib[-1].update([['ComponentID', featureSetXML[0].get('ComponentID')]])
                pointNames = []
                for feature in featureSetXML:                                   # for each geometry feature:
                    pointNames.append(feature.get('Name'))
                loadBCLib[-1].update([['Points', pointNames]])                   # store names of datum points
            else:
                loadBCLib.pop()

        if thermalSetXML is not None:
            generateThermalConstructs(thermalSetXML, loadBCLib, asminfo)
                
    except:
        cad_library.exitwitherror('Error in creating load and boundary condition dictionaries', -1, 'AbaqusDict')
    
    return loadBCLib, accel