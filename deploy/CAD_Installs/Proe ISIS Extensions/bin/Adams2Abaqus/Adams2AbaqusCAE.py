"""

Adams2AbaqusCAE.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - Includes modules which use functions of Abaqus/CAE to
      build the Finite Element Model.

"""

import abaqus
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

from Adams2AbaqusGeometry import *

MAIN = os.getcwd()                                                                                          # Initial working directory
LOGDIR = os.path.join(MAIN, "log", "CyPhy2AbaqusCmd.log")                                                      # Path to log file


def createParts(myModel, step):                                                                             # Create parts in Abaqus by importing the top level assembly Step file
    f_p = open(LOGDIR, "w")

    i = 0
    newPart = 'I love my job.'                                                                              # Create a new dummy part (essential)
    try:                                                                                                    # STEP part import stage
        while newPart:                                                                                      # While part exists
            i += 1
            genericPartName = "part" + '-%d' % i                                                            # Change the part name to a unique part name for this loop
            newPart = myModel.PartFromGeometryFile(                                                         
                name=genericPartName,geometryFile=step,
                bodyNum=i,combine=False,dimensionality=THREE_D,
                type=DEFORMABLE_BODY,usePartNameFromFile=True)                                              # Create part from file
		
    except Exception:                                                                                       # If exception occurs
        pass                                                                                                # Do nothing

    f_p.close()


def deleteInvalidParts(myModel):                                                                            # Delete the invalid parts that came with the Step file
    f_p = open(LOGDIR, "w")

    try:
        for key in myModel.parts.keys():                                                                    # Loop through each part in the model
            currentPart = myModel.parts[key]                                                                # Get the current part object
            f_p.write('Importing part ' + str(key) + '\n')
            if len(currentPart.cells) == 0:                                                                 # If part has no volume:
                del myModel.parts[key]                                                                      # Delete the part
    except:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))                                    
        f_p.write('ERROR: Error during the deletion of the invalid part\n')
        raise                                                                                               

    f_p.close()


def defineStaticStep(myModel):                                                                              # Define a static analysis FEA step
    f_p = open(LOGDIR, "w")

    # define static step
    try:
        f_p.write("Defining static step" + '\n')
        myStep = myModel.StaticStep(name='staticLoading',previous='Initial',
            timePeriod=1.0,nlgeom=ON,maxNumInc=100000,
            initialInc=0.01,minInc=0.000001)                                                                # Define the static step                                                                
        myModel.fieldOutputRequests['F-Output-1'].setValues(variables=(
            'S','MISESMAX','PE','PEEQ','PEMAG','LE','U','RF'),
            frequency=LAST_INCREMENT)                                                                       # Setting field variables for the Field Output Request
        myModel.TabularAmplitude(name='staticAmplitude',
            data=((0,0),(1,1)))                                                                             # Define an amplitude for the loads for a smoother convergence
        amp = 'staticAmplitude'
        analysisStepName = 'staticLoading'
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during analysis step creation\n')
        raise                                                                             

    f_p.close()


def defineMaterials(myModel, mtrlRef):                                                                      # Define a materials inside the Abaqus model
    f_p = open(LOGDIR, "w")

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
            if 'HeatCapacity' in mtrl:                                                                      # If heat capacity value is present in mtrlRef for the current material
                heatCapP = (mtrl['HeatCapacity'],)
                myMaterial.SpecificHeat(table=(heatCapP,))
            if 'CoefficientOfThermalExpansion' in mtrl:                                                     # If coefficient of thermal expansion value is present in mtrlRef for the...
                                                                                                            # ...current material
                expandP = (mtrl['CoefficientOfThermalExpansion'],)
                myMaterial.Expansion(table=(expandP,))                                                      # Define the coefficient of thermal expansion value for that material object
            if 'ThermalConductivity' in mtrl:                                                               # If thermal conductivity value is present in mtrlRef for the current material
                conductP = (mtrl['ThermalConductivity'],)
                myMaterial.Conductivity(table=(conductP,))                                                  # Define the thermal conductivity value for that material object
            f_p.write("Creating material " + str(key) + '\n')
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during creating materials inside the model\n')
        raise                                                                            

    f_p.close()


def decideAnchoredPart(jointsMBDXML, anchorID, anchorPointID, instRef, myModel):                            # Decide if the anchored part will be included in the analysis
    f_p = open(LOGDIR, "w")

    try:

        refPntLocSplit = re.findall ('[^;]+', anchorPointID)                                                # Seperate the anchor point coordiate into its X, Y and Z components

        anchorX = float(refPntLocSplit[0])                                                                  # Store the x-coordinate value
        anchorY = float(refPntLocSplit[1])                                                                  # Store the y-coordinate value
        anchorZ = float(refPntLocSplit[2])                                                                  # Store the z-coordinate value

        for key in instRef:                                                                                 # Loop through each part in the model
            if anchorID == instRef[key]['ComponentID']:                                                     # If the current part is the anchored part
                anchoredPart = key                                                                          # Store the name of the anchored part

                anchorFace = myModel.parts[key].faces.findAt(coordinates=(anchorX,                          # In the Abaqus model, find out if the anchor point is on the face of a part or not
                    anchorY,anchorZ),printWarning=False)

                if anchorFace:                                                                              # If the anchor point is on a face, include that part in FEA analysis
                    includeAnchoredPart = True
                else:
                    includeAnchoredPart = False    

    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during detection of the anchored part.\n')
        raise

    f_p.close()

    return(includeAnchoredPart, anchoredPart)          


def assignSections(key, entry, myModel, myAsm):                                                             # Assigning section and create an assembly instance from the current part
    f_p = open(LOGDIR, "w")

    sectionName = key
    myModel.HomogeneousSolidSection(name=sectionName,
        material=entry['MaterialID'])                                                                       # Create a section based on the material defined for the current part

    p = myModel.parts[key]                                                                                  # Find the current part inside the model
    region = (p.cells,)                                                                                     # Define a region for the current part
    p.SectionAssignment(region=region,
        sectionName=sectionName)                                                                            # Assign section to the current part
    myAsm.Instance(name=key,dependent=OFF,part=p)                                                           # Create instance from the part

    f_p.close()


def meshInstances(edgeSeedDensity, unitShort, instRef, myAsm):                                              # Mesh the current part
    f_p = open(LOGDIR, "w")
    
    # define global element size as distance between opposite corners of assembly bounding box
    f_p.write("Meshing the model" + '\n')
    f_p.write("Mesh size will be determined by using the distance between " + \
        "opposite corners of each parts bounding box" + '\n')

    try:
        # mesh instances
        f_p.write("Meshing each part" + '\n')
        f_p.write('\n')
        for key in myAsm.instances.keys():
            myInst = myAsm.instances[key]
            entry = instRef[key]
            region = myInst.cells
            regionBox = region.getBoundingBox()
            boxCorner1 = array(regionBox['low'])
            boxCorner2 = array(regionBox['high'])
            seedSize = LA.norm(boxCorner2-boxCorner1)/edgeSeedDensity
            f_p.write(str(key) + " element size: " +str(seedSize) + " " + str(unitShort) + "\n")
            regionMask = region.getMask()                               
            regionPick = region.getSequenceFromMask(mask=regionMask)
            elemType = mesh.ElemType(elemCode=C3D10,elemLibrary=STANDARD)
            myAsm.setMeshControls(regions=regionPick,elemShape=TET,technique=FREE)
            myAsm.setElementType(regions=(regionPick,),elemTypes=(elemType,))
            myAsm.seedPartInstance(regions=(myInst,),size=seedSize,constraint=FINER)
            myAsm.generateMesh(regions=(myInst,),meshTechniqueOverride=ON,
                seedConstraintOverride=ON)
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during seeding and meshing\n')
        f_p.write('       Unmeshable parts or parts with invalid geometry might be present\n')
        raise

    f_p.close()


def constrainWithSoftSprings(key, myAsm):                                                                   # Constrain every vertice of the part in x, y and z directions with soft-springs
    f_p = open(LOGDIR, "w")

    vRange=len(myAsm.instances[key].vertices)                                                               # Find all vertices of the part
    region=regionToolset.Region(vertices=myAsm.instances[key].vertices[0:vRange])                           # Define a region which is composed of all those vertices                   

    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-1', region=region, orientation=None, dof=1, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)                                                                             # Put soft-spring on x-direction at each vertice of the part

    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-2', region=region, orientation=None, dof=2, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)                                                                             # Put soft-spring on x-direction at each vertice of the part

    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-3', region=region, orientation=None, dof=3, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)                                                                             # Put soft-spring on x-direction at each vertice of the part

    loadFileName = 'LT_' + str(key) + '.lod'
    loadFilePath = os.path.join(MAIN, loadFileName)

    f_p.close()


def readAndDefineMarkers(key, MAIN, myAsm):                                                                 # Read marker data from the LOD files and define them in the Abaqus assembly
    f_p = open(LOGDIR, "w")

    loadFileName = 'LT_' + str(key) + '.lod'
    loadFilePath = os.path.join(MAIN, loadFileName)

    dataFile = open(loadFilePath, 'r')                                                                      # Open the corresponding LOD file for the part 
    i=1
    
    for eachLine in dataFile:                                                                               # Look each line in the LOD file
        loadData = eachLine.split()
        try:
            if loadData[1] == 'Load' and loadData[2] == 'Point' and loadData[3]\
            == 'Information' and loadData[4] == '(Global':
                start=i+2                                                                                   # Save the line number at which the marker data starts
        except:
            pass
        try:
            if loadData[1] == 'Load' and loadData[2] == 'Point' and loadData[3]\
            == 'Information' and loadData[4] == '(FEA':
                stop=i-1                                                                                    # Save the line number at which the marker data ends
        except:
            pass
        i+=1


    dataFile = open(loadFilePath, 'r')
    refPointRef = {}
    i=1
    k=1
    
    for eachLine in dataFile:                                                                               # Again check each line in the LOD file
        loadData = eachLine.split()
        if i<stop and i>start:                                                                              # If the line includes the marker definitions
            markerName = 'M' + str(loadData[2])                                                             # Grab the marker name
            refName = 'RP-'+str(k)                                                                  
            X = float(loadData[3])                                                                          # Grab the x-coordinate of the marker
            Y = float(loadData[4])                                                                          # Grab the y-coordinate of the marker
            Z = float(loadData[5])                                                                          # Grab the z-coordinate of the marker
            refPointRef[refName]=(X,Y,Z)                                                                    # Store reference point name and coordinate data in the refPointRef dictionary
            myAsm.ReferencePoint(point=(X, Y, Z))                                                           # Define the marker as a reference point inside the assembly
            refPnt=myAsm.referencePoints.findAt((X, Y, Z),)
            myAsm.Set(referencePoints=(refPnt,), name=markerName)                                           # Define a set by using the reference point for the set name use the marker name...
                                                                                                            # ...defined in the LOD file           
            k+=1
        i+=1

    f_p.close()

    return (refPointRef)


def getBoundingNodes(jointsMBDXML, key, instRef, myModel, myAsm):                                           # Get the nodes inside the bounding box of a part which is connected to the...
                                                                                                            # ...current part
    f_p = open(LOGDIR, "w")
   
    sortOtherComp = {}
    refPntOtherComp = {}  

    for joint in jointsMBDXML.findall('Joint'):                                                             # Look for all Joint metrics inside CADAssembly_metrics.xml file
        jointLocA = joint.get('AssembledComponentInstanceID')                                               # Store the first component as ComponentA
        ConstrainedToComponents = joint.find('ConstrainedToComponents')                                
        ConstrainedToComponent = ConstrainedToComponents.find('ConstrainedToComponent')
        jointLocB = ConstrainedToComponent.get('ComponentInstanceID')                                       # Store the second component as ComponentB

        coordinateSys = joint.find('GlobalCoordinateSystem')
        location = coordinateSys.find('Location')
        refPntLoc = location.get('ArrayValue')                                                              # Store x, y and z coordinates of the joint                                                              

        found = False
        keyID = instRef[key]['ComponentID']
        if jointLocA == keyID:                                                                              # If ComponentA is the current part.
            otherComp = jointLocB                                                                           # Store ComponentB's ComponentID with otherComp variable
            found = True
        elif jointLocB == keyID:                                                                            # If ComponentB is the current part.
            otherComp = jointLocA                                                                           # Store ComponentA's ComponentID with otherComp variable
            found = True
        else:
            pass

        if found:                                                                                           # If the component (otherComp) connected to the current part is a...
                                                                                                            #...valid part in the model
            for element in instRef:                                                                         # Loop through each valid part in the model
                if instRef[element]['ComponentID'] == otherComp:
                    otherCompName = element                                                                 # Store the name of the component (otherComp) connected to the current part
 
            if otherCompName in instRef:                                                                    # If the part which is connected to the current part is in instRef dictionary
                test = myModel.parts[otherCompName].queryGeometry()['boundingBox']                          # Get the bounding box of the connected part
                minimumX = test[0][0]                                                                       # Store the minimum x coordinate of the bounding box
                minimumY = test[0][1]                                                                       # Store the minimum y coordinate of the bounding box
                minimumZ = test[0][2]                                                                       # Store the minimum z coordinate of the bounding box
                maximumX = test[1][0]                                                                       # Store the maximum x coordinate of the bounding box
                maximumY = test[1][1]                                                                       # Store the maximum y coordinate of the bounding box
                maximumZ = test[1][2]                                                                       # Store the maximum z coordinate of the bounding box
                boundingNodes = myAsm.instances[key].nodes.getByBoundingBox(xMin = minimumX,
                    yMin = minimumY, zMin = minimumZ, xMax = maximumX, yMax = maximumY,
                    zMax = maximumZ)                                                                        # Get the nodes of the current part which are inside the bounding box of the...
                                                                                                            # ...connected part (otherComp)
                                                                                                        
                boundingNodesNum = len(boundingNodes)                                                       # Store the number of the nodes inside the bounding box

                sortOtherComp.update([[otherCompName,boundingNodesNum]])                                    # Store the name of the connected part and the number of nodes inside the...
                                                                                                            # ...bounding box.
                                                                                                            
                refPntOtherComp.update([[otherCompName,refPntLoc]])                                         # Store the name and the x, y and z coordinates of the connected part.


    sortedOtherComp = sorted(sortOtherComp.iteritems(), key=operator.itemgetter(1))                         # Sort the dictionary of the connected parts (sortOtherComp) based on the nodes...
                                                                                                            #...of the current part inside the connected parts bounding box
    f_p.close()

    return (refPntOtherComp, sortedOtherComp)


def defineMPCs(sortedOtherComp, refPntOtherComp, refPointRef,
               instRef, myAsm, myModel, key):                                                               # Define MPCs between the reference points (markers) and the corresponding...
                                                                                                            # ...surfaces of the part
    f_p = open(LOGDIR, "w")

    usedNodeLabelTuple = ()
    usedRP = []
    
    for i in range(len(sortedOtherComp)):                                                                   # Loop through the parts that are connected to the current part
        otherComp = sortedOtherComp[i][0]                                                                   # Start with the connected part which has the least amount of the nodes...
                                                                                                            # ...the current part inside its boundong box
        refPntLocArray = refPntOtherComp[otherComp]                                                         # Get the coordiantes x, y and z coordinates of the current connected part 
        
        refPntLocSplit = re.findall ('[^;]+', refPntLocArray)                                               # Split the coordintes array into 3 seperate values

        X = float(refPntLocSplit[0])                                                                        # Store the value of x coordinate
        Y = float(refPntLocSplit[1])                                                                        # Store the value of y coordinate
        Z = float(refPntLocSplit[2])                                                                        # Store the value of z coordinate

        dist = float('+inf')
        for (RP,coord) in refPointRef.iteritems():                                                          # Loop through the coordinate values inside the LOD files provided by Adams
            if RP not in usedRP:
                preciseX = coord[0]                                                                         # Store the value of x coordinate
                preciseY = coord[1]                                                                         # Store the value of y coordinate
                preciseZ = coord[2]                                                                         # Store the value of z coordinate
                distTemp = math.sqrt((abs(X-preciseX))**2+(abs(Y-preciseY))**2+(abs(Z-preciseZ))**2)        # Determine the distance between the marker defined in the LOD file and marker...
                                                                                                            # ...defined in the CADAssmebly_metrics.xml file
                if distTemp < dist:
                    dist = distTemp
                    selectedCoord = coord
                    selectedRP = RP                                                                         # Map the the markers defined in the LOD file with the markers defined in the...
                                                                                                            # ...CADAssmebly_metrics.xml file        
        usedRP.append(selectedRP)                                                                           
        X = selectedCoord[0]                                                                                # Store the value of x coordinate
        Y = selectedCoord[1]                                                                                # Store the value of y coordinate
        Z = selectedCoord[2]                                                                                # Store the value of z coordinate

        refPnt=myAsm.referencePoints.findAt((X, Y, Z),)                                                     # Find the reference point object which corresponds to the current marker                                                                                                         

        MPCname = str(otherComp) + '_MPC'                                                                   # Define the naem of the MPC based on the name of the current connected part

        if otherComp in instRef:                                                                            # If the current connected part is a valid part inside the CAD domain
            test = myModel.parts[str(otherComp)].queryGeometry()['boundingBox']                             # Get the x, y and z coordinates of the two points defining the bounding...
                                                                                                            # ...box of the current connected part
            minimumX = test[0][0]
            minimumY = test[0][1]
            minimumZ = test[0][2]
            maximumX = test[1][0]
            maximumY = test[1][1]
            maximumZ = test[1][2]
            crudeNodes = myAsm.instances[key].nodes.getByBoundingBox(xMin = minimumX,
                yMin = minimumY, zMin = minimumZ, xMax = maximumX, yMax = maximumY,
                zMax = maximumZ)                                                                            # Get all the nodes of the current part inside the bounding box of the...
                                                                                                            # ...current connected part
            try:
                crudeFaces = myAsm.instances[key].faces.getByBoundingBox(xMin = minimumX,
                    yMin = minimumY, zMin = minimumZ, xMax = maximumX, yMax = maximumY,
                    zMax = maximumZ)                                                                        # Get all the faces of the current part inside the bounding box of the...
                                                                                                            # ...current connected part, if there is any
            except:
                pass
            
            if crudeFaces:                                                                                  # If there is/are face/s of the current part detected inside the bounding...
                                                                                                            # ...box of the current connected part
                instFaces = crudeFaces                                                                      # Store the faces
            else:                                                                                           # Otherwise
                instFaces = myAsm.instances[key].faces                                                      # Store all faces of the current part
            
            faceLabelList = []
            crudeLabelList = []
            for j in range(len(instFaces)):                                                                 # For each stored face of the current part
                instFaceNodes = instFaces[j].getNodes()                                                     # Store the nodes on that surface
                for k in range(len(instFaceNodes)):                                                         # For each node on that surface
                    faceLabel = instFaceNodes[k].label                                                      # Grab the label of that node
                    faceLabelList.append(faceLabel)                                                         # And store the label of that node                                                         
            for i in range(len(crudeNodes)):                                                                # Loop through all the nodes of the current part inside the boundong box of...
                                                                                                            # ...the current connected part
                myFaceLabel = crudeNodes[i].label                                                           # Grab the label of the current node
                crudeLabelList.append(myFaceLabel)                                                          # And store the label of the current node

            nodeSequenceSet = set(faceLabelList) & set(crudeLabelList) - set(usedNodeLabelTuple)            # Determine the nodes of the current part inside the boundong box of the...
                                                                                                            # ...current connected part and on the one of the faces detected...
                                                                                                            # ...previously. Additionally, the determined nodes should not be used...
                                                                                                            # ...before by another MPC.
            nodeSequenceTuple = tuple(nodeSequenceSet)                                                      # Convert the detected set of nodes into a tuple

            nodes = crudeNodes.sequenceFromLabels(nodeSequenceTuple)                                        # Convert the detected tuple of nodes into a Mesh Node Array
            myModel.MultipointConstraint(name=MPCname, surface=(nodes,),
                controlPoint=(refPnt,), mpcType=BEAM_MPC)                                                   # Connect the current refernce point (marker) with the created Mesh Node...
                                                                                                            # ...Array by using a MPC constraint
            usedNodeLabelTuple = usedNodeLabelTuple + nodeSequenceTuple                                     # Add the detected tuple of nodes to the tuple called usedNodeLabelTuple...
                                                                                                            # ...to prevent multiple use of the same node for different MPCs
        else:                                                                                               # If the current connected part is not a valid part inside the CAD domain...
                                                                                                            # ..., then the current refernce point is the anchor point
            faces = myAsm.instances[key].faces.findAt(coordinates=(anchorX,anchorY,anchorZ),
                printWarning=False)                                                                         # Detect the face on which the anchor point is on
            myModel.MultipointConstraint(name=MPCname, surface=(faces,), controlPoint=(refPnt,),
                mpcType=BEAM_MPC)                                                                           # Connect the detected face with the reference point representing the anchor...
                                                                                                            # ...point by using a MPC constraint
    f_p.close()


def createINP(myAsm, key, myModel, parallelCores, ramAllocated, mdb):                                       # Create an INP file of the current part

    f_p = open(LOGDIR, "w")

    eRange=len(myAsm.instances[key].elements)                                                               # Determine the number of mesh elements of the current part 
    myAsm.Set(elements=myAsm.instances[key].elements[0:eRange], name='wholeInstance')                       # Create a set form those elements and name it as wholeInstance
    inpName = key + '_temp'                                                                                 # Determine the name of the INP file based on the name of the current part
            
    try:
        myJob = mdb.Job(name=inpName, model=myModel.name,
            description='Static FEA job',multiprocessingMode=DEFAULT,
            numCpus=parallelCores,numDomains=1,memory=ramAllocated,
            parallelizationMethodExplicit=DOMAIN)                                                           # Create a job inside the Abaqus/CAE
        myJob.writeInput(consistencyChecking=ON)                                                            # Create an input file from that job
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during creating the INP file')
        raise

    f_p.close()

    return(inpName)


def cleanModel(myModel, myAsm):                                                                             # Clean the current model of the Abaqus/CAE from the previous part itself and...
                                                                                                            # ...its constraints
    f_p = open(LOGDIR, "w")
    
    for constraint in myModel.constraints.keys():
        del myModel.constraints[constraint]                                                                 # Clean the current model of the Abaqus/CAE from the previous part itself

    for feat in myAsm.features.keys():
        del myAsm.features[feat]                                                                            # Clean the current model of the Abaqus/CAE from the previous parts constraints

    f_p.close()
    

def createFinalInp(inpFilePath, inpTempFile, lodFile, stopLine, startLine):                                 # Merge temporary INP file and LOD file to create the final INP file

    f_p = open(LOGDIR, "w")
                                                                                                            
    with open(inpFilePath, 'wb') as inpFile:                                                                # Open the empty final INP file                                                       
        n=1
        for eachLine in inpTempFile:                                                                        # Loop through each line in the temporary INP file
            if n <= stopLine:                                                                               
                inpFile.write(eachLine)                                                                     # Write each line into the final INP file until the stopping condition is reached
            else:
                break                   
            n+=1
        o=1
        for eachLine2 in lodFile:                                                                           # Loop through each line in the LOD file
            if o >= startLine:
                gravReplace = eachLine2.find("<element label>,GRAV,")                                       # If there is an <element label> block for gravity force inside the LOD file
                centrifReplace = eachLine2.find("<element label>,CENTRIF,")                                 # If there is an <element label> block for centrifugal force inside the LOD file
                rotaReplace = eachLine2.find("<element label>,ROTA,")                                       # If there is an <element label> block for rotational force inside the LOD file
                if gravReplace != -1:
                    eachLine2 = eachLine2.replace("<element label>,GRAV,"
                        ,",GRAV,")                                                                          # Remove the <element label> block for gravity forces
                elif centrifReplace != -1:
                    eachLine2 = eachLine2.replace("<element label>,CENTRIF,"
                        ,"wholeInstance,CENTRIF,")                                                          # Replace the <element label> block with the set wholeInstance for centrifugal forces
                elif rotaReplace != -1:
                    eachLine2 = eachLine2.replace("<element label>,ROTA,"
                        ,"wholeInstance,ROTA,")                                                             # Replace the <element label> block with the set wholeInstance for rotational forces
                inpFile.write(eachLine2)                                                                    # Write each line into the final INP file after the starting condition is reached 
            o+=1                                                                          

    f_p.close()

    return (inpFile)
    
