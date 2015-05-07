"""

AbaqusCAE.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which use functions of Abaqus/CAE to
      build the Finite Element Model.

"""

from abaqus import *
from abaqusConstants import *
import caeModules, jobMessage, odbAccess
import interaction, visualization, regionToolset, job, mesh

import csv
import glob
import math, re, sys, json, os
import operator
from optparse import OptionParser
import shutil
import string as STR
import traceback
import time
import uuid, ctypes
import xml.etree.ElementTree as ET
import _winreg
import logging

from AbaqusGeometry import *
from AbaqusError import *
from AbaqusThermal import *

def createParts(myModel, step, CGs): 
    logger = logging.getLogger()
    logger.info('Importing parts into the model...\n')
    i = 0
    j = 0
    newPart = 'I love my job.'
    try: 
        while newPart: 
            i += 1
            newPart = myModel.PartFromGeometryFile(        # create part from file
                name="part" + '-%d' % i,geometryFile=step,
                bodyNum=i,combine=False,dimensionality=THREE_D,
                type=DEFORMABLE_BODY,usePartNameFromFile=True)
            if len(myModel.parts[newPart.name].cells) != 0:
                j = 0
                des_comp = [key for key, value in CGs.items() if newPart.name.lower() in key.lower()]
                des_comp = [c for c in des_comp if (c[0:c.rfind('__')].lower() == newPart.name.lower())]    
                # Imported part has non-unique name. Rename so internal part is unique.
                # Need to rename correct instance - eg, ASM could have 10 JACKS.
                # Imported order is based on model tree, so order is known.
                while True:
                    try:
                        logger.info('Changing name from ' + newPart.name + ' to ' + des_comp[j])
                        myModel.parts.changeKey(fromName=newPart.name,toName=des_comp[j])
                        break
                    except:
                        j += 1
                        if j > 100: # Arbitrary value. Need to increase if more than 100 instances
                            raise Exception('Error occurred on renaming/importing of parts.')
    except:
        pass
        

def deleteInvalidParts(myModel):        
    logger = logging.getLogger()
    logger.info("**********************************************************************************\n")
    for key in myModel.parts.keys(): 
        currentPart = myModel.parts[key] 
        if len(currentPart.cells) != 0:
            logger.info('Importing part ' + str(key))
        else:    # if part has no volume, delete
            try:
                del myModel.parts[key]
            except:
                logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
                logger.error('Error during the deletion of the invalid part\n')
                raise
    logger.info("**********************************************************************************\n")

    
def defineStaticStep(myModel):
    logger = logging.getLogger()
    try:
        logger.info("Defining static step \n")
        myStep = myModel.StaticStep(name='staticLoading',previous='Initial',
            timePeriod=1.0,nlgeom=ON,maxNumInc=100000,
            initialInc=0.01,minInc=0.000001)                                                               
        myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
            'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
            frequency=LAST_INCREMENT)                                                                      
        myModel.TabularAmplitude(name='staticAmplitude',
            data=((0,0),(1,1)))                                                                            
        amp = 'staticAmplitude'
        analysisStepName = 'staticLoading'
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during static analysis step creation\n')
        raise     
    logger.info("**********************************************************************************\n")
    
    return (myStep, amp, analysisStepName)
    

def defineModalStep(myModel):
    logger = logging.getLogger() 
    try:
        logger.info("Defining modal analysis step \n")
        myStep = myModel.FrequencyStep(name='modalAnalysis',
            previous='Initial',eigensolver=LANCZOS,numEigen=30) # Parameters may need to change
        myModel.TabularAmplitude(name='dummyAmplitude',
            data=((0,0),(1,1)))
        amp = 'dummyAmplitude'
        analysisStepName = 'modalAnalysis'
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during modal analysis step creation\n')
        raise 
    logger.info("**********************************************************************************\n")
    
    return (myStep, amp, analysisStepName)
    
    
def defineDynamicStep(myModel, runAdams):
    logger = logging.getLogger()
    try:
        if not runAdams:
            logger.info("Defining implicit dynamic step \n")
            myStep = myModel.ImplicitDynamicsStep(name='dynamicImplicitLoading',
                previous='Initial',timePeriod=15.0,nlgeom=ON,maxNumInc=100000,
                initialInc=0.01,minInc=0.000001,maxInc=1)   ### parameters may need to change
            myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
                'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
                frequency=1)
            myModel.TabularAmplitude(name='dynamicImplicitAmplitude',
                data=((0,0),(5,1),(10,1),(15,0)))
            amp = 'dynamicImplicitAmplitude'
            analysisStepName = 'dynamicImplicitLoading'
        else:
            logger.info("Defining explicit dynamic step \n")
            myStep = myModel.ExplicitDynamicsStep(name='dynamicExplicitLoading',
                previous='Initial',timePeriod=15.0,nlgeom=ON)
            myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
                'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
                frequency=1)
            myModel.TabularAmplitude(name='dynamicExplicitAmplitude',
                data=((0,0),(5,1),(10,1),(15,0)))
            amp = 'dynamicExplicitAmplitude'
            analysisStepName = 'dynamicExplicitLoading'
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during dynamic analysis step creation\n')
        raise 
    logger.info("**********************************************************************************\n")
    
    return (myStep, amp, analysisStepName)


def defineCoupledThermalDisplacementStep(myModel):
    logger = logging.getLogger()
    logger.info("Defining coupled temperature-displacement step" + '\n')
    myStep = myModel.CoupledTempDisplacementStep(name='coupledTempDispl',
        previous='Initial',response=STEADY_STATE, timePeriod=1.0,nlgeom=ON,
        maxNumInc=100000, initialInc=0.01,minInc=0.000001,deltmx=0.000001)                            ### parameters may need to change
    myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
        'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF','TEMP'),
        frequency=LAST_INCREMENT)                                                   # setting field variables for the Field Output Request
    myModel.TabularAmplitude(name='staticAmplitude',
        data=((0,0),(1,1)))
    amp = 'staticAmplitude'
    analysisStepName = 'coupledTempDispl'
    logger.info("**********************************************************************************\n")
    
    return (myStep, amp, analysisStepName)


    
def defineMaterials(myModel, mtrlRef):                                                                      # Define a materials inside the Abaqus model
    logger = logging.getLogger()

    # create materials (rough)
    try:
        for (key,mtrl) in mtrlRef.iteritems():                                                              # For each material described in mtrlRef:
            myMaterial = myModel.Material(name=key)                                                         # Declare a new material object
            elasticP = (mtrl['mechanical__modulus_elastic'],
                mtrl['mechanical__ratio_poissons'])                                                         # Grab modulus of elasticity and poissons ratio values for the current material
            myMaterial.Elastic(table=(elasticP,))                                                           # Define a material object for the current material
            if 'density' in mtrl:                                                                           # If density value is present in mtrlRef for the current material
                densityP = (mtrl['density'],)
                myMaterial.Density(table=(densityP,))                                                       # Define the density value for that material object
            if 'thermal__capacity_specific_heat' in mtrl:                                                                      # If heat capacity value is present in mtrlRef for the current material
                heatCapP = (mtrl['thermal__capacity_specific_heat'],)
                myMaterial.SpecificHeat(table=(heatCapP,))
            if 'thermal__coefficient_expansion_linear' in mtrl:                                                     # If coefficient of thermal expansion value is present in mtrlRef for the current material
                expandP = (mtrl['thermal__coefficient_expansion_linear'],)
                myMaterial.Expansion(table=(expandP,))                                                      # Define the coefficient of thermal expansion value for that material object
            if 'thermal__conductivity' in mtrl:                                                               # If thermal conductivity value is present in mtrlRef for the current material
                conductP = (mtrl['thermal__conductivity'],)
                myMaterial.Conductivity(table=(conductP,))                                                  # Define the thermal conductivity value for that material object
            logger.info("Creating material " + str(key))
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during creating materials inside the model\n')
        raise                                                                            
    logger.info("**********************************************************************************\n")
    
    
def decideAnchoredPart(jointsMBDXML, anchorID, anchorPointID, instRef, myModel):                            # Decide if the anchored part will be included in the analysis
    logger = logging.getLogger()
    try:
        refPntLocSplit = re.findall ('[^;]+', anchorPointID)                                                # Seperate the anchor point coordiate into its X, Y and Z components

        anchorX = float(refPntLocSplit[0])                                                                  # Store the x-coordinate value
        anchorY = float(refPntLocSplit[1])                                                                  # Store the y-coordinate value
        anchorZ = float(refPntLocSplit[2])                                                                  # Store the z-coordinate value

        for key in instRef:                                                                                 # Loop through each part in the model
            if anchorID == instRef[key]['ComponentID']:                                                     # If the current part is the anchored part
                anchoredPart = key                                                                          # Store the name of the anchored part

                anchorFace = myModel.parts[key].faces.findAt(coordinates=(anchorX,                          # In the Abaqus model, find out if the anchor point is on the face of a part or not
                    anchorY,anchorZ),printWarning=True)

                if anchorFace:                                                                              # If the anchor point is on a face, include that part in FEA analysis
                    includeAnchoredPart = True
                else:
                    includeAnchoredPart = False    
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during detection of the anchored part.\n')
        raise

    return(includeAnchoredPart, anchoredPart)           
    

def assignSections(instRef, myModel, myAsm, subAsms, asmParts):  
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
            pass
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
            myAsm.Instance(name=key,dependent=OFF,part=p)   


def EliminateOverlaps(instRef, rigidParts, myAsm, myModel):
    logger = logging.getLogger()
    count = 0
    noOverlap = True
    logger.info("**********************************************************************************\n")
    logger.info("Checking for overlapping (buy) parts")
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
                    logger.info("Eliminating overlap between " + str(rigid) + ' and ' + str(key) + '\n')
                    noOverlap = False
                except:
                    pass
                count+=1
    if noOverlap:
        logger.info("None of the (buy) parts are overlapping" + '\n')
    logger.info("**********************************************************************************\n")

        
def defineContactInteractions(myModel):
    logger = logging.getLogger()
    logger.info("Defining general contact interactions" + '\n')
    
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
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('ERROR: Error during creating general contact behavior\n')
        raise


def defineRigidBodyConstraints(instRef, Jan24_deactivate_rigidity,
                               instIndex, myAsm, myModel):
###  if part is flagged as rigid, make that part a rigid body
####  Rigid Bodies will results in no value for summary.  Summary will need to be updated to allow this.  Currently it prodeces an error.
    logger = logging.getLogger()
    try:
        refPointKeys = []
        refPointLocation = []
        for (key,entry) in instRef.iteritems():
            sectionName = key
            if entry['isRigid'] and Jan24_deactivate_rigidity:  
                instName = instIndex[entry['ComponentID']]
                myInstcell = myAsm.instances[instName].cells        
                point = myAsm.ReferencePoint(datumPointDict[sectionName]['CG_'+sectionName]) 
                cells1 = myInstcell.getSequenceFromMask(mask=('[#1 ]', ), )
                region2=regionToolset.Region(cells=cells1)
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
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during defining rigid body constraints\n')
        raise

def apply_flat_face_load(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                         instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):    
    logger = logging.getLogger()
    regionMask = myMask(entry['FaceIDs'])
    try:
        instName = instIndex[entry['ComponentID']]
    except KeyError:
        instName = instAssemblyIndex[entry['ComponentID']]
    logger.info("Preparing geometry for load/BC on part " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
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
                    follower=OFF,resultant=ON,amplitude=amp) 
    logger.info("Creating load/BC " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')

    
def apply_polygon(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                  instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):    
    logger = logging.getLogger()
    edgeIndex = []
    entry.update([['Edges',[]]])
    try:
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
            for faceSet in faceIndex[1:]:
                if face not in faceSet:
                    break   # Continue outer loop
            entry.update([['FaceIDs',(face,)]])
            entry.update([['RegionName','Face_'+faceName+str(faceCount)]])
            faceCount=faceCount+1
        if entry['1stGeomQual'] != 'BoundaryOnly':        
            apply_flat_face_load(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                                 instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount)
    except:
        raise ConstraintException
            
def apply_cylinder(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                   instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):
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
 
    if entry['1stGeomQual'] == 'BoundaryOnly':
        # initial region stuff
        logger.info("Preparing geometry for load/BC on part " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
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
        logger.info("Creating load/BC " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')

 
def apply_circle(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                 instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):
    logger = logging.getLogger()
    #try:
    for key in myAsm.instances.keys():
        circleCenter = partDatums[entry['Points'][0]]
        myInst2 = myAsm.instances[key]
        circleFaces = myInst2.faces.findAt(
            coordinates=circleCenter,printWarning=True)
        logger.info(str(myInst2.faces))
        logger.info(str(circleCenter))
        logger.info(str(circleFaces))
        if circleFaces:
            myInst = myInst2
            instance = key
            entry.update([['FaceIDs',(circleFaces.index,)]])
            entry.update([['RegionName','Face_%d' % uniqueName]])
            entry.update([['Instance',key]])
    if entry['1stGeomQual'] != 'BoundaryOnly':        
        apply_flat_face_load(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                             instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount)
    #except:
    #    raise ConstraintException
 
def apply_face(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
               instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):
    logger = logging.getLogger()
    try:
        for key in myAsm.instances.keys():
            facePoint = partDatums[entry['Points'][0]]
            myInst2 = myAsm.instances[key]
            selectedFace = myInst2.faces.findAt(
                coordinates=facePoint,printWarning=True)
            if selectedFace:
                myInst = myInst2
                instance = key
                entry.update([['FaceIDs',(selectedFace.index,)]])
                entry.update([['RegionName','PointFace_%d' % uniqueName]])
                entry.update([['Instance',key]])
                selectedFaceEdges = selectedFace.getEdges()
                selectedFaceEdge = myInst2.edges[selectedFaceEdges[0]].pointOn
                selectedFaceEdgeCoord = selectedFaceEdge[0]
        if 'FaceIDs' not in entry:
            logger.error("No geometric entity was found at " + str(facePoint) + \
                         ". Please check CAD model for floating points.")
            raise
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
        logger.info("Preparing geometry for load/BC on part " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
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
                curved = None
                try:  # Check for curved face.
                    curved = myInst.faces[entry['FaceIDs'][0]].getRadius()
                except:
                    pass
                if curved is None:
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
                                magnitude=tractionMag,directionVector=((0.0,0.0,0.0),tupleC),
                                distributionType=UNIFORM,traction=GENERAL,
                                follower=OFF,resultant=ON,amplitude=amp)                            # apply traction load
                        except:
                            logger.error('Can not apply traction component of the force to the surface.\n')
                            pass
                else:
                    vectorF = array(entry['Force'])
                    tractionMag = float(LA.norm(vectorF))
                    tractionName = regionName
                    myModel.SurfaceTraction(name=tractionName,
                        createStepName=myStep.name,region=region,
                        magnitude=tractionMag,directionVector=((0.0,0.0,0.0),tuple(vectorF)),
                        distributionType=UNIFORM,traction=GENERAL,
                        follower=OFF,resultant=ON,amplitude=amp)
                        
        elif entry['Treatment'] in ['SpecifiedTemperature','ConvectionHeat','HeatFlux']:
            createThermalConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName)
        
        logger.info("Creating load/BC " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
    except:
        #raise ConstraintException
        raise

def apply_concentric_circles(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                             instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):
    logger = logging.getLogger()
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
        logger.error("Uncommon concentric circle geometry, e.g. hemitoroid" + '\n')
        raise ValueError("Uncommon concentric circle geometry, e.g. hemitoroid")
    if entry['1stGeomQual'] != 'BoundaryOnly':    
        apply_flat_face_load(entry, instIndex, instAssemblyIndex, myAsm, datumPointDict, myModel, args)
            

def apply_sphere(myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                 instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount):
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
    if entry['1stGeomQual'] == 'BoundaryOnly':        
        logger.info("Preparing geometry for load/BC on part " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
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
        logger.info("Creating load/BC " + instName + ' with geometry type ' + str(entry['GeometryType']) + ' and treatment ' + str(entry['Treatment']) + '\n')
                
def apply_acceleration(myModel, myAsm, myStep, instRef, accel, amp, args, Jan24_deactivate_rigidity):
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    if accel['x'] or accel['y'] or accel['z']:
        if not args.modal:
            for (key,entry) in instRef.iteritems():
                loadName = str(key)
                try:
                    myRegion = (myAsm.instances[key].cells,)
    #                        if not entry['isRigid']:
                except KeyError:
                    continue
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
            logger.info("Creating acceleration\n")  
        
def apply_loads_and_bcs(myModel, myAsm, myStep, instRef, loadBCLib, instIndex, instAssemblyIndex,
                        datumPointDict, accel, amp, args, Jan24_deactivate_rigidity, thermalSetXML):
    logger = logging.getLogger()
    faceCount = 0
    uniqueName = 0
    logger.info("**********************************************************************************" + '\n')
    for entry in loadBCLib:
        if 'Treatment' in entry:
            if entry['Treatment'] == 'AmbientTemperature':
                continue
        try:
            instName = instIndex[entry['ComponentID']]
        except KeyError:
            instName = instAssemblyIndex[entry['ComponentID']]
        partDatums = datumPointDict[instName]
        try:
            myInst = myAsm.instances[instName]
        except:
            myInst = None
        if 'Component' in entry and entry['Component'] == True:
            # Only thermal constraints are applied to all faces of component.
            createThermalConstraint(myModel, myAsm, entry, myStep, amp, instName, regionName=None)
        try:
            # Dispatch function based on geometry type.
            if 'Component' not in entry or entry['Component'] == False:
                # Only thermal constraints have Component/Geometry distinction
                globals()[str('apply_' + entry['GeometryType'].lower())](myModel, myAsm, myStep, partDatums, instIndex, args, amp,
                                                                         instAssemblyIndex, entry, uniqueName, datumPointDict, faceCount)
        except KeyError:
            logger.error('Geometry type %s not supported.' % entry['GeometryType'])
            raise
        except ConstraintException:
            logger.error('Error attempting to generate load or boundary condition! \n')
            logger.error('Error occured on: \n' + \
                          '\n            ComponentID ' + entry['ComponentID'] + \
                          '\n            Geometry construct ' + str(entry['GeometryType']) + \
                          '\n            Which uses datum point(s) ' + str(entry['Points']) +'.\n')
            logger.error('Check your testbench and CAD model for any invalidities, misplaced points or ' + \
                         ' illegal/incorrect use of CyPhy constructs. \n')
            raise ConstraintException
        uniqueName = uniqueName + 1  # Name of loads, not instances.
          
    apply_acceleration(myModel, myAsm, myStep, instRef, accel, amp, args, Jan24_deactivate_rigidity)

    if thermalSetXML:
        initT = False
        for thermal in thermalSetXML:
            if thermal.get('LoadType') == 'InitialTemperature':
                setInitialTemperature(thermal, myAsm, instName)
                initT = True
        if initT == False:
            raise InitialTempMissingException

            
def get_mesh_statistics(entry, myAsm, myInst, full_asm_stats, mesh_fname):
    with open(mesh_fname, 'a') as mq_csv:
        csv_writer = csv.writer(mq_csv)
        csv.field_size_limit(sys.maxint)
        # Generate Mesh Statistics META-3027
        meshStats = myAsm.getMeshStats(regions=(myInst,))
        statistics = [stat for stat in dir(meshStats) if not \
                        stat.startswith('__') and not stat.endswith('__')]
        qual = myAsm.verifyMeshQuality(criterion=ANALYSIS_CHECKS,regions=(myInst,))
        
        temp = []
        for s in statistics:
            temp.append(getattr(meshStats,s))
        name = myInst.name                    
        row = [ #myInst.name[0:name.rfind('__')],   
                myInst.name,    # Return modified name for user to reference in CAE file
                entry['ComponentID'],
                qual['numElements'],
                [warn.label for warn in qual['warningElements']],
                [fail.label for fail in qual['failedElements']],
                [na.label for na in qual['naElements']] ]
        row.extend(temp)
        # Store part values to use for calculating full assembly mesh stats
        full_asm_stats.extend([row[2],len(row[3]),len(row[4]),len(row[5])])
        full_asm_stats.extend(temp)
        csv_writer.writerow(row)
        
    return full_asm_stats


def get_asm_mesh_statistics(metricComponentsXML, metricAssemblyXML, full_asm_stats, mesh_fname):
    mq_list = []
            
    # Read csv and overwrite assembly stat placeholder with full_asm_stats data
    with open(mesh_fname, 'rb') as mq_csv:
        reader = csv.reader(mq_csv)
        mq_list.extend(reader)
    # Assembly name/id *always* first instance found in these trees
    asm_name = metricComponentsXML.find('MetricComponent').get('Name')
    asm_id = metricAssemblyXML.find('CADComponent').get('ComponentInstanceID')
    asm_line_override = {1:[asm_name,asm_id,sum(full_asm_stats[0::15]),sum(full_asm_stats[1::15]),
                            sum(full_asm_stats[2::15]),sum(full_asm_stats[3::15]),sum(full_asm_stats[4::15]),
                            sum(full_asm_stats[5::15]),sum(full_asm_stats[6::15]),sum(full_asm_stats[7::15]),
                            sum(full_asm_stats[8::15]),sum(full_asm_stats[9::15]),sum(full_asm_stats[10::15]),
                            sum(full_asm_stats[11::15]),sum(full_asm_stats[12::15]),
                            sum(full_asm_stats[13::15]),sum(full_asm_stats[14::15])]}
    with open(mesh_fname, 'wb') as mq_csv:
        writer = csv.writer(mq_csv)
        for line, row in enumerate(mq_list):
            data = asm_line_override.get(line,row)
            writer.writerow(data)
    
    
def meshInstances(edgeSeedDensity, unitShort, instRef, instAssemblyRef, myAsm, feaXML,
                   metricComponentsXML, metricAssemblyXML, args,runAdams=None):                                              # Mesh the current part
    logger = logging.getLogger()
    
    # define global element size as distance between opposite corners of assembly bounding box
    logger.info("Meshing the model & grabbing quality statistics" + '\n')
    logger.info("Mesh size will be determined by using the distance between " + \
                "opposite corners of each parts bounding box" + '\n')
        
    max_elem_size = float(feaXML.get('MaxElementSize'))
    if max_elem_size < 0.0:
        logger.info('\nWARNING: Maximum element size is specified as negative value in assembly XML. ' \
                    'Assuming correct value is abs(MaxElemSize). Make sure to correct GME model.')
        max_elem_size = abs(max_elem_size)

    try:
        # mesh instances
        logger.info("Writing mesh quality statistics to MeshQuality.csv." + '\n')
        if runAdams:
            mesh_fname = "MeshQuality_" + str(myAsm.instances.keys()[0]) + ".csv"
        else:
            mesh_fname = "MeshQuality.csv"
        with open(mesh_fname, 'wb') as mq_csv:
            csv_writer = csv.writer(mq_csv)
            csv.field_size_limit(sys.maxint)
            csv_writer.writerow(['Full Assembly','ComponentID','Total Elements','Warning Elements','Failed Elements',
                                 'NA Elements','Hex Elements','Line Elements','Meshed Regions',
                                 'Total Nodes','Point Elements','Pyramid Elements','Quad Elements',
                                 'Tet Boundary Elements','Tet Elements','Tri Elements','Wedge Elements'])
            csv_writer.writerow('')  # Full Assembly data placeholder
            csv_writer.writerow('')
            csv_writer.writerow(['Part','ComponentID','Total Elements','Warning Elements','Failed Elements',
                                 'NA Elements','Hex Elements','Line Elements','Meshed Regions',
                                 'Total Nodes','Point Elements','Pyramid Elements','Quad Elements',
                                 'Tet Boundary Elements','Tet Elements','Tri Elements','Wedge Elements'])
        full_asm_stats = []
        for key in myAsm.instances.keys():
            myInst = myAsm.instances[key]
            try:
                entry = instRef[key]
            except KeyError:
                entry = instAssemblyRef[key]
            region = myInst.cells
            regionBox = region.getBoundingBox()
            boxCorner1 = array(regionBox['low'])
            boxCorner2 = array(regionBox['high'])
            seedSize = LA.norm(boxCorner2-boxCorner1)/edgeSeedDensity
            if max_elem_size != 0.0 and seedSize > max_elem_size:
                logger.info((str(key) + ': Overriding computed seed size of ' + str(seedSize) + 
                            ' with specified maximum element size of ' + str(abs(max_elem_size))))
                seedSize = abs(max_elem_size)
            else:
                logger.info(str(key) + " element size: " + str(seedSize) + " " + str(unitShort))
            regionMask = region.getMask()                               
            regionPick = region.getSequenceFromMask(mask=regionMask)
            if entry['isShell']:
                if runAdams and args.dynamicExplicit:
                    elemType = mesh.ElemType(elemCode=S4R,elemLibrary=EXPLICIT)
                else:
                    elemType = mesh.ElemType(elemCode=S4R,elemLibrary=STANDARD)
                myAsm.setMeshControls(regions=regionPick,elemShape=QUAD_DOMINATED,technique=FREE,
                    sizeGrowthRate=1.05)
                myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType,))
                seedSize = seedSize/2
            elif feaXML.find('ThermalElements') is not None:
                defineThermalElements(myAsm, regionPick, args)
            else:
                if runAdams and args.dynamicExplicit:
                    elemType = mesh.ElemType(elemCode=C3D10,elemLibrary=EXPLICIT)
                else:
                    elemType = mesh.ElemType(elemCode=C3D10,elemLibrary=STANDARD)
                myAsm.setMeshControls(regions=regionPick,elemShape=TET,technique=FREE,
                    sizeGrowthRate=1.05)
                myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType,))
            myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER)
            myAsm.generateMesh(regions=(myInst,),meshTechniqueOverride=ON,
                seedConstraintOverride=ON)
            full_asm_stats = get_mesh_statistics(entry, myAsm, myInst, full_asm_stats, mesh_fname)
        get_asm_mesh_statistics(metricComponentsXML, metricAssemblyXML, full_asm_stats, mesh_fname)
    
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during seeding and meshing - ' + \
                'Unmeshable parts or parts with invalid geometry might be present\n')
        raise
        

def connectParts(myModel, myAsm, instRef, instAssemblyRef, cadAssemblyXML,
                 asmParts, analysisStepName, args): 
    logger = logging.getLogger()
    logger.info("Connecting parts" + '\n')
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
            logger.info("Creating the constraint: " + str(key) + ' ')
            if key[:5] == 'Rigid':
                continue
            myModel.constraints[key].setValues(adjust=OFF)
            if key[0:3] == 'CC-':
                myModel.constraints[key].setValues(tieRotations=OFF)
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during defining tie constraints\n')
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
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during defining rigid body constraints\n')
        raise
        
        
def createCAE(root, testBenchName, args):
    logger = logging.getLogger()
    logger.info('Creating CAE file\n')
    
    caePath = os.path.join(root,testBenchName) + '.cae'
    mdb.saveAs(pathName=caePath)                                        # save it as <testBenchName>.cae
    if not args.meshOnly:                                     # if .cae file is requested:
        caeCopyName = 'AbaqusMeshAndAssembly.cae'
    else:
        caeCopyName = 'AbaqusMesh.cae'
    try:
        shutil.copy2(testBenchName + '.cae', caeCopyName)    
    except:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during saving the CAE file\n')
        raise

    logger.info('Creating' + str(caeCopyName) + '\n')
    logger.info("**********************************************************************************" + '\n')
 

def exportNastranDeck():
    logger = logging.getLogger()
    logger.info("Exporting the Nastran deck" + '\n')
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
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during exporting the Nastran deck\n')
        raise
    logger.info("Exporting NastranModel.bdf" + '\n')
    
    
def mergeAssemblyComponents(myAsm, instAssemblyRef, asmParts):
    logger = logging.getLogger()
    for asm in instAssemblyRef.keys():
        if instAssemblyRef[asm]['MergeChildren']:
            logger.info('Merging all leaf components of ' + \
                str(instAssemblyRef[asm]['ComponentID']) + ' into one part instance.')
            merge = []
            for comp in asmParts[asm]:
                merge.append(myAsm.instances[comp])
            myAsm.PartFromBooleanMerge(name=asm,instances=tuple(merge))
            myAsm.Instance(name=asm,dependent=OFF,part=mdb.models['Model-1'].parts[asm])
            for comp in asmParts[asm]:
                del myAsm.instances[comp]
                

def CreateLOADSBCPNG(PreProcessFileName, myAsm, analysisStepName, root):
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Creating the PNG files of the loads and BC's" + '\n')

    mainDir = os.path.join(root,"Contour_and_BC_plots")
    if not os.path.exists(mainDir):
        os.mkdir(mainDir)

    os.chdir(mainDir)

    save2fileName=PreProcessFileName
	#  Print pictures of load and BCs
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
    logger.info("Creating PNG file " + 'Boundary Conditions and Loads_1')
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON, loads=OFF, 
        bcs=OFF, predefinedFields=OFF, connectors=OFF)
    session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
        meshTechnique=ON)	
    session.printToFile(fileName='Model Initial Mesh_1', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    logger.info("Creating PNG file " + 'Model Initial Mesh_1')
    session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF, loads=ON, 
        bcs=ON, predefinedFields=ON, connectors=ON)
    session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
        meshTechnique=OFF)		
    session.viewports['Viewport: 1'].view.setValues(cameraPosition=(987.505, 
    -35.8282, 7.68834), cameraUpVector=(0, 1, 0))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_2', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    logger.info("Creating PNG file " + 'Boundary Conditions and Loads_2')
    session.viewports['Viewport: 1'].view.setValues(nearPlane=757.99, 
        farPlane=1200.36, width=542.18, height=365.72, cameraPosition=(0.996667, 
        36.7201, -977.094), cameraUpVector=(0.0, 1.0, 0.0), 
        cameraTarget=(-0.87486, -35.267, 7.11584))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_3', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    logger.info("Creating PNG file " + 'Boundary Conditions and Loads_3')
    session.viewports['Viewport: 1'].view.setValues(nearPlane=759.096, 
        farPlane=1215.06, width=542.971, height=366.255, cameraPosition=(-91.9079, 
        -1009.75, -32.4658), cameraUpVector=(-1.0, 0.0, 0.0), 
        cameraTarget=(1.67948, -27.8817, -0.616374))
    session.viewports['Viewport: 1'].view.fitView()
    session.printToFile(fileName='Boundary Conditions and Loads_4', format=PNG, canvasObjects=(
        session.viewports['Viewport: 1'], ))
    logger.info("Creating PNG file " + 'Boundary Conditions and Loads_4')

    os.chdir(root)
            
 
 
 
 