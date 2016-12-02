"""

AbaqusCAE_ADAMS.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules that are invoked if load inputs to
      FEA analysis are from ADAMS.

"""

from abaqus import *
from abaqusConstants import *
import jobMessage
import interaction
import regionToolset
import job
import mesh
import cad_library
import math
import re
import os
import operator

from AbaqusParse import parseLOD, parseInpTemp
from AbaqusCAE import meshInstances
from AbaqusGeometry import *


def constrainWithSoftSprings(key, myAsm):
    """ Constrain every vertice of the part in x, y and z directions with soft-springs. """
    vRange = len(myAsm.instances[key].vertices)  # Find all vertices of the part
    # Define a region which is composed of all those vertices
    region = regionToolset.Region(vertices=myAsm.instances[key].vertices[0:vRange])

    # Put soft-spring on x-direction at each vertice of the part
    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-1', region=region, orientation=None, dof=1, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)

    # Put soft-spring on x-direction at each vertice of the part
    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-2', region=region, orientation=None, dof=2, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)

    # Put soft-spring on x-direction at each vertice of the part
    myAsm.engineeringFeatures.SpringDashpotToGround(
        name='Spring-3', region=region, orientation=None, dof=3, 
        springBehavior=ON, springStiffness=1.0, dashpotBehavior=OFF, 
        dashpotCoefficient=0.0)


def readAndDefineMarkers(myAsm, lodFilePath):
    """ Read marker data from the LOD files and define them in the Abaqus assembly. """
    # Open the corresponding LOD file for the part
    i = 1
    with open(lodFilePath, 'r') as dataFile:
        for eachLine in dataFile:  # Look each line in the LOD file
            loadData = eachLine.split()
            try:
                if loadData[1] == 'Load' and loadData[2] == 'Point' and loadData[3]\
                        == 'Information' and loadData[4] == '(Global':
                    start = i+2  # Save the line number at which the marker data starts
            except IndexError:
                pass
            try:
                if loadData[1] == 'Load' and loadData[2] == 'Point' and loadData[3]\
                        == 'Information' and loadData[4] == '(FEA':
                    stop = i-1  # Save the line number at which the marker data ends
            except IndexError:
                pass
            i += 1

    refPointRef = {}
    i = 1
    k = 1
    with open(lodFilePath, 'r') as dataFile:
        for eachLine in dataFile:       # Again check each line in the LOD file
            loadData = eachLine.split()
            if start < i < stop:  # If the line includes the marker definitions
                markerName = 'M' + str(loadData[2])  # Grab the marker name
                refName = 'RP-'+str(k)                                                                  
                X = float(loadData[3])  # Grab the x-coordinate of the marker
                Y = float(loadData[4])  # Grab the y-coordinate of the marker
                Z = float(loadData[5])  # Grab the z-coordinate of the marker
                # Store reference point name and coordinate data in the refPointRef dictionary
                refPointRef[refName] = (X, Y, Z)
                myAsm.ReferencePoint(point=(X, Y, Z))       # Define the marker as a reference point inside the assembly
                refPnt = myAsm.referencePoints.findAt((X, Y, Z),)
                # Define a set by using the reference point for the set name use the marker name defined in the LOD file
                myAsm.Set(referencePoints=(refPnt,), name=markerName)
                k += 1
            i += 1

    return refPointRef


def getBoundingNodes(asminfo, key, instRef, myModel, myAsm):
    """ Get the nodes inside the bounding box of a part which is connected to the current part. """
    sortOtherComp = {}
    refPntOtherComp = {}  
    # Look for all Joint metrics inside CADAssembly_metrics.xml file
    for joint in asminfo.joints:
        jointLocA = joint.compfrom.cyphyid          # Store the first component as ComponentA
        jointLocB = joint.compto[0].cyphyid  # Store the second component as ComponentB

        refPntLoc = joint.location

        found = False
        keyID = instRef[key]['ComponentID']
        if jointLocA == keyID:                  # If ComponentA is the current part.
            otherComp = jointLocB               # Store ComponentB's ComponentID with otherComp variable
            found = True
        elif jointLocB == keyID:                # If ComponentB is the current part.
            otherComp = jointLocA               # Store ComponentA's ComponentID with otherComp variable
            found = True
        else:
            pass

        if found:                               # If the component (otherComp) connected to the current part is a...
            otherCompName = None                # ...valid part in the model
            for element in instRef:             # Loop through each valid part in the model
                if instRef[element]['ComponentID'] == otherComp:
                    otherCompName = element  # Store the name of the component (otherComp) connected to the current part
 
            if otherCompName is None:   # constrained to TLA - all nodes are contained by 'otherComp'
                test = myModel.parts[key].queryGeometry()['boundingBox']  # Get the bounding box of the connected part
                minimumX = test[0][0]                               # Store the minimum x coordinate of the bounding box
                minimumY = test[0][1]                               # Store the minimum y coordinate of the bounding box
                minimumZ = test[0][2]                               # Store the minimum z coordinate of the bounding box
                maximumX = test[1][0]                               # Store the maximum x coordinate of the bounding box
                maximumY = test[1][1]                               # Store the maximum y coordinate of the bounding box
                maximumZ = test[1][2]                               # Store the maximum z coordinate of the bounding box
                # Get the nodes of the current part which are inside the bounding box of the connected part (otherComp)
                boundingNodes = myAsm.instances[key].nodes.getByBoundingBox(
                    xMin=minimumX, yMin=minimumY, zMin=minimumZ, xMax=maximumX,
                    yMax=maximumY, zMax=maximumZ)

                boundingNodesNum = len(boundingNodes)  # Store the number of the nodes inside the bounding box

                # Store the name of the connected part and the number of nodes inside the bounding box
                sortOtherComp.update([[otherCompName, boundingNodesNum]])

                # Store the name and the x, y and z coordinates of the connected part.
                refPntOtherComp.update([[otherCompName, refPntLoc]])

            if otherCompName in instRef:  # If the part which is connected to the current part is in instRef dictionary
                # Get the bounding box of the connected part
                test = myModel.parts[otherCompName].queryGeometry()['boundingBox']
                minimumX = test[0][0]             # Store the minimum x coordinate of the bounding box
                minimumY = test[0][1]             # Store the minimum y coordinate of the bounding box
                minimumZ = test[0][2]             # Store the minimum z coordinate of the bounding box
                maximumX = test[1][0]             # Store the maximum x coordinate of the bounding box
                maximumY = test[1][1]             # Store the maximum y coordinate of the bounding box
                maximumZ = test[1][2]             # Store the maximum z coordinate of the bounding box
                # Get the nodes of the current part which are inside the bounding box of the connected part (otherComp)
                boundingNodes = myAsm.instances[key].nodes.getByBoundingBox(
                    xMin=minimumX, yMin=minimumY, zMin=minimumZ,
                    xMax=maximumX, yMax=maximumY, zMax=maximumZ)

                boundingNodesNum = len(boundingNodes)  # Store the number of the nodes inside the bounding box

                # Store the name of the connected part and the number of nodes inside the bounding box.
                sortOtherComp.update([[otherCompName, boundingNodesNum]])
                # Store the name and the x, y and z coordinates of the connected part.
                refPntOtherComp.update([[otherCompName, refPntLoc]])

    # Sort the dictionary of the connected parts (sortOtherComp) based on the nodes...
    #...of the current part inside the connected parts bounding box
    sortedOtherComp = sorted(sortOtherComp.iteritems(), key=operator.itemgetter(1))

    return refPntOtherComp, sortedOtherComp
  

def defineMPCs(sortedOtherComp, refPntOtherComp, refPointRef,
               instRef, myAsm, myModel, key):
    """ Define MPCs between the reference points (markers) and the corresponding surfaces of the part. """
    usedNodeLabelTuple = ()
    usedRP = []
    for i in range(len(sortedOtherComp)):  # Loop through the parts that are connected to the current part
        otherComp = sortedOtherComp[i][0]  # Start with the connected part which has the least amount of the nodes...
                                           # ...the current part inside its boundong box

        # Get the coordiantes x, y and z coordinates of the current connected part
        refPntLoc = refPntOtherComp[otherComp]
        X = float(refPntLoc[0])                          # Store the value of x coordinate
        Y = float(refPntLoc[1])                          # Store the value of y coordinate
        Z = float(refPntLoc[2])                          # Store the value of z coordinate

        dist = float('+inf')
        # Loop through the coordinate values inside the LOD files provided by Adams
        for (RP, coord) in refPointRef.iteritems():
            if RP not in usedRP:
                preciseX = coord[0]               # Store the value of x coordinate
                preciseY = coord[1]               # Store the value of y coordinate
                preciseZ = coord[2]               # Store the value of z coordinate
                # Determine the distance between the marker defined in the LOD file and marker...
                # ...defined in the CADAssmebly_metrics.xml file
                distTemp = math.sqrt((abs(X-preciseX))**2+(abs(Y-preciseY))**2+(abs(Z-preciseZ))**2)

                if distTemp < dist:
                    dist = distTemp
                    selectedCoord = coord
                    selectedRP = RP    # Map the the markers defined in the LOD file with the markers defined in the...
                                       # ...CADAssmebly_metrics.xml file
        usedRP.append(selectedRP)                                                                           
        X = selectedCoord[0]                     # Store the value of x coordinate
        Y = selectedCoord[1]                     # Store the value of y coordinate
        Z = selectedCoord[2]                     # Store the value of z coordinate

        # Find the reference point object which corresponds to the current marker
        # Define the naem of the MPC based on the name of the current connected part
        # If the current connected part is a valid part inside the CAD domain
        # Get the x, y and z coordinates of the two points defining the bounding box of the current connected part
        refPnt = myAsm.referencePoints.findAt((X, Y, Z),)
        MPCname = str(otherComp) + '_MPC'
        if otherComp in instRef:
            test = myModel.parts[str(otherComp)].queryGeometry()['boundingBox']

            minimumX = test[0][0]
            minimumY = test[0][1]
            minimumZ = test[0][2]
            maximumX = test[1][0]
            maximumY = test[1][1]
            maximumZ = test[1][2]
            # Get all the nodes of the current part inside the bounding box of the current connected part
            crudeNodes = myAsm.instances[key].nodes.getByBoundingBox(
                xMin=minimumX, yMin=minimumY, zMin=minimumZ,
                xMax=maximumX, yMax=maximumY, zMax=maximumZ)

            # Get all the faces of the current part inside the bounding box of the...
            # ...current connected part, if there is any
            try:
                crudeFaces = myAsm.instances[key].faces.getByBoundingBox(
                    xMin=minimumX, yMin=minimumY, zMin=minimumZ,
                    xMax=maximumX, yMax=maximumY, zMax=maximumZ)
            except:
                pass
            
            if crudeFaces:        # If there is/are face/s of the current part detected inside the bounding...
                                  # ...box of the current connected part
                instFaces = crudeFaces     # Store the faces
            else:                          # Otherwise
                instFaces = myAsm.instances[key].faces  # Store all faces of the current part
            
            faceLabelList = []
            crudeLabelList = []
            for j in range(len(instFaces)):                        # For each stored face of the current part
                instFaceNodes = instFaces[j].getNodes()            # Store the nodes on that surface
                for k in range(len(instFaceNodes)):                # For each node on that surface
                    faceLabel = instFaceNodes[k].label             # Grab the label of that node
                    faceLabelList.append(faceLabel)                # And store the label of that node
            # Loop through all the nodes of the current part inside the boundong box of the current connected part
            for n in range(len(crudeNodes)):

                myFaceLabel = crudeNodes[n].label    # Grab the label of the current node
                crudeLabelList.append(myFaceLabel)   # And store the label of the current node

            # Determine the nodes of the current part inside the boundong box of the...
            # ...current connected part and on the one of the faces detected...
            # ...previously. Additionally, the determined nodes should not be used...
            # ...before by another MPC.
            # Convert the detected set of nodes into a tuple
            # Convert the detected tuple of nodes into a Mesh Node Array
            nodeSequenceSet = set(faceLabelList) & set(crudeLabelList) - set(usedNodeLabelTuple)

            nodeSequenceTuple = tuple(nodeSequenceSet)
            nodes = crudeNodes.sequenceFromLabels(nodeSequenceTuple)
            myModel.MultipointConstraint(
                name=MPCname, surface=(nodes,), controlPoint=(refPnt,), mpcType=BEAM_MPC)

             # Connect the current refernce point (marker) with the created Mesh Node...
             # ...Array by using a MPC constraint
            # Add the detected tuple of nodes to the tuple called usedNodeLabelTuple...
            # ...to prevent multiple use of the same node for different MPCs
            # If the current connected part is not a valid part inside the CAD domain...
            # ..., then the current refernce point is the anchor point
            usedNodeLabelTuple = usedNodeLabelTuple + nodeSequenceTuple
        else:
            # Detect the face on which the anchor point is on
            faces = myAsm.instances[key].faces.findAt(coordinates=(X, Y, Z), printWarning=False)
            # Connect the detected face with the reference point representing the anchor point by using a MPC constraint
            myModel.MultipointConstraint(name=MPCname, surface=(faces,), controlPoint=(refPnt,),
                                         mpcType=BEAM_MPC)


def createINP(myAsm, key, myModel, parallelCores, ramAllocated):
    """ Create an INP file of the current part. """
    eRange = len(myAsm.instances[key].elements)  # Determine the number of mesh elements of the current part
    # Create a set form those elements and name it as wholeInstance
    myAsm.Set(elements=myAsm.instances[key].elements[0:eRange], name='wholeInstance')
    inpName = key + '_temp'  # Determine the name of the INP file based on the name of the current part
            
    try:
        myJob = mdb.Job(name=inpName, model=myModel.name,
                        description='Static FEA job', multiprocessingMode=DEFAULT,
                        numCpus=parallelCores, numDomains=1, memory=ramAllocated,
                        parallelizationMethodExplicit=DOMAIN)  # Create a job inside the Abaqus/CAE
        myJob.writeInput(consistencyChecking=ON)  # Create an input file from that job
    except:
        cad_library.exitwitherror('Error during creating the INP file', -1, 'AbaqusCAE_ADAMS.py')

    return inpName


def cleanModel(myModel, myAsm):
    """ Clean the current model of the Abaqus/CAE from the previous part itself and its constraints. """
    for constraint in myModel.constraints.keys():
        # Clean the current model of the Abaqus/CAE from the previous part itself
        del myModel.constraints[constraint]

    for feat in myAsm.features.keys():
        # Clean the current model of the Abaqus/CAE from the previous parts constraints
        del myAsm.features[feat]


def createFinalInp(inpFilePath, inpTempFile, lodFile, stopLine, startLine):
    """ Merge temporary INP file and LOD file to create the final INP file. """
    with open(inpFilePath, 'wb') as inpFile:  # Open the empty final INP file
        n = 1
        for eachLine in inpTempFile:  # Loop through each line in the temporary INP file
            if n <= stopLine:                                                                               
                # Write each line into the final INP file until the stopping condition is reached
                inpFile.write(eachLine)
            else:
                break                   
            n += 1
        o = 1
        for eachLine2 in lodFile:  # Loop through each line in the LOD file
            if o >= startLine:
                # If there is an <element label> block for gravity force inside the LOD file
                gravReplace = eachLine2.find("<element label>,GRAV,")
                # If there is an <element label> block for centrifugal force inside the LOD file
                centrifReplace = eachLine2.find("<element label>,CENTRIF,")
                # If there is an <element label> block for rotational force inside the LOD file
                rotaReplace = eachLine2.find("<element label>,ROTA,")
                if gravReplace != -1:
                    # Remove the <element label> block for gravity forces
                    eachLine2 = eachLine2.replace("<element label>,GRAV,", ",GRAV,")
                elif centrifReplace != -1:
                    # Replace the <element label> block with the set wholeInstance for centrifugal forces
                    eachLine2 = eachLine2.replace("<element label>,CENTRIF,", "wholeInstance,CENTRIF,")
                elif rotaReplace != -1:
                    # Replace the <element label> block with the set wholeInstance for rotational forces
                    eachLine2 = eachLine2.replace("<element label>,ROTA,", "wholeInstance,ROTA,")
                # Write each line into the final INP file after the starting condition is reached
                inpFile.write(eachLine2)
            o += 1
            

def assignSections_ADAMS(key, entry, myModel, myAsm):
    """ Assigning section and create an assembly instance from the current part. """
    sectionName = key
    # Create a section based on the material defined for the current part
    myModel.HomogeneousSolidSection(name=sectionName, material=entry['MaterialID'])
    p = myModel.parts[key]   # Find the current part inside the model
    region = (p.cells,)      # Define a region for the current part
    p.SectionAssignment(region=region, sectionName=sectionName)  # Assign section to the current part
    myAsm.Instance(name=key, dependent=OFF, part=p)  # Create instance from the part


def ADAMS_setup_INP(myAsm, key, asminfo, instRef, instAssemblyRef,
                    myModel, parallelCores, ramAllocated):
    """ Manipulates Abaqus model to provide correctly formatted INP file. """
    try:
        lodFileID = instRef[key]['ComponentID']
    except KeyError:
        try:
            lodFileID = instAssemblyRef[key]['ComponentID']
        except KeyError:
            cad_library.exitwitherror('LOD file not found for ' + str(key), -1, 'AbaqusCAE_ADAMS.py')
    lodFileName = 'LT_' + lodFileID + '.lod'
    lodFilePath = os.path.join(os.getcwd(), lodFileName)

    # Constrain every vertice of the part in x, y and z directions with soft-springs
    constrainWithSoftSprings(key, myAsm)

    # Read the markers from the LOD files and define them in the Abaqus assembly as reference points
    refPointRef = readAndDefineMarkers(myAsm, lodFilePath)

    # Get the nodes inside the bounding box of the part which is connected to the current part
    (refPntOtherComp, sortedOtherComp) = getBoundingNodes(asminfo, key,
                                                          instRef, myModel, myAsm)

    # Define MPCs between the reference points (markers) and the corresponding surfaces on the part
    defineMPCs(sortedOtherComp, refPntOtherComp, refPointRef,
               instRef, myAsm, myModel, key)

    inpName = createINP(myAsm, key, myModel, parallelCores, ramAllocated)   # Create an INP file of the current part

    # Clean the current model of the Abaqus/CAE from the previous part itself and its constraints
    cleanModel(myModel, myAsm)

    # Define the previously created INP file of the current part as the temporary INP file
    inpTempFileName = inpName + '.inp'
    # Define the location of the temporary INP file and store it
    inpTempFilePath = os.path.join(os.getcwd(), inpTempFileName)
    with open(inpTempFilePath, 'r') as inpTempFile:
        # Determine upto which point the temporary INP file needs to be merged with the final INP file
        stopLine = parseInpTemp(inpTempFile)
        with open(lodFilePath, 'r') as lodFile:
            # Determine from which point the LOD file generated by Adams needs to be merged with the final INP file
            startLine = parseLOD(lodFile)

    with open(inpTempFilePath, 'r') as inpTempFile:
        with open(lodFilePath, 'r') as lodFile:
            # Define the name of the final INP file by using the name of the current part
            inpFileName = key + '.inp'
            # Define the location of the final INP file and store it
            inpFilePath = os.path.join(os.getcwd(), inpFileName)
            # Create the final INP file by opening and closing it
            open(inpFilePath, 'a').close()
            # Merge temporary INP file and LOD file to create the final INP file
            createFinalInp(inpFilePath, inpTempFile, lodFile, stopLine, startLine)
    os.remove(inpTempFilePath) 
    
    
def AbaqusCAE_ADAMS(asminfo, instRef, includeAnchoredPart, anchoredPart, myModel,
                    myAsm, edgeSeedDensity, unitShort, feaXML,
                    parallelCores, ramAllocated, args, instAssemblyRef, asmParts):
    """ Entry point for Adams-to-Abaqus simulations. All CyPhy/CAD information has been parsed at this point. """
    skipped = []
    merging_asms = []
   
    parts_to_be_merged = []
    for asm in instAssemblyRef.keys():
        if instAssemblyRef[asm]['MergeChildren']:
            merge = []
            merging_asms.append(asm)
            for comp in asmParts[asm]:
                parts_to_be_merged.append(comp)
                myAsm.Instance(comp, dependent=OFF, part=myModel.parts[comp])
                merge.append(myAsm.instances[comp])
            myAsm.PartFromBooleanMerge(name=asm, instances=tuple(merge))
            for comp in asmParts[asm]:
                del myAsm.instances[comp]
                
    for (key, entry) in instRef.iteritems():  # Iterate through all parts
        if key in parts_to_be_merged:
            skipped.append([key, entry])
            continue
        else:
            if not includeAnchoredPart and key == anchoredPart:
                continue
            else:
                pass
                    
            assignSections_ADAMS(key, entry, myModel, myAsm)
            
            meshInstances(asminfo, edgeSeedDensity, unitShort, instRef,
                          instAssemblyRef, myAsm, feaXML, args, runAdams=True)
                
            ADAMS_setup_INP(myAsm, key, asminfo, instRef, instAssemblyRef,
                            myModel, parallelCores, ramAllocated)

    for j in merging_asms:
        a = asmParts[j]
        material = None
        for s in skipped:
            if s[0] in a:
                material = s[1]['MaterialID']
                a.remove(s[0])
        if len(a) > 0:
            raise Exception
        if material is not None:
            instAssemblyRef[j].update([['MaterialID', material]])
            instRef[j].update([['MaterialID', material]])
        cleanModel(myModel, myAsm) 
        assignSections_ADAMS(j, instAssemblyRef[j], myModel, myAsm)
        
        meshInstances(asminfo, edgeSeedDensity, unitShort, instRef,
                      instAssemblyRef, myAsm, feaXML, args, runAdams=True)
                
        ADAMS_setup_INP(myAsm, j, asminfo, instRef, instAssemblyRef,
                        myModel, parallelCores, ramAllocated)
