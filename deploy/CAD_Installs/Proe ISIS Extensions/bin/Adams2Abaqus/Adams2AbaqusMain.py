"""

Adams2AbaqusMain.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - The main script which takes care of pre-processing,
      solving and post-processing.

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
from Adams2AbaqusParse import *
from Adams2AbaqusDict import *
from Adams2AbaqusRun import *
from Adams2AbaqusStep import *
from Adams2AbaqusCAE import *


MAIN = os.getcwd()                                                                                          # Initial working directory
LOGDIR = os.path.join(MAIN, "log", "CyPhy2AbaqusCmd.log")                                                      # Path to log file
LOGFILE = open(LOGDIR, "a")                                                                                 # Open the log file

LIBRARY_MANAGER = parseMaterialLibrary()                                                                    # Parse the material library to get necessary material information

LOGFILE.close()

# the function that is executed in the command line
def primary():

    f_p = open(LOGDIR, "w")                                                                                 # Create a log file

    now = time.strftime("%c")
    
    f_p.write('\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    f_p.write("**************************** STARTING FEA ANALY" + \
        "SIS *******************************" + '\n')
    f_p.write("*********************************** v1.0.0 ****" + \
        "***********************************" + '\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    f_p.write('\n')
    f_p.write("Initializing Abaqus/CAE" + '\n')
    f_p.write('\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    f_p.write('\n')
    f_p.write(now + '\n')
    f_p.write('\n')
    f_p.write("***********************************************" + \
        "***********************************" + '\n')
    
    # Take command line arguments
    usage = "usage: abaqus cae script=CyPhy2AbaqusCmd.py -- [options]"
    parser = OptionParser(usage=usage)
    parser.add_option("-f","--standard", default=False, action="store_true",
        help="run a standard FEA analysis")
    parser.add_option("-d","--dynamic", default=False, action="store_true",
        help="run a dynamic explicit FEA analysis")


    e = re.compile(',\S+,,')                                                                                # regular expression

    # Assign variable to the blank model produced at startup
    myModel = mdb.models['Model-1']
    myAsm = myModel.rootAssembly

    # Assign values to the constants that will be used throughout the FEA analysis
    edgeSeedDensity = 40                                                                                    # Coefficient inversely proportional to seed size
    parallelCores = 1                                                                                       # Number of CPU cores to use in solver
    ramAllocated = 90                                                                                       # Value for deciding the allocated RAM for the analysis

    # Parse necessary information from the XML files  
    (xmlName, feaXML, cadAssemblyXML, metricSetXML, metricsComponentSetXML,
         maxNumberIter) = parseCADAssemblyXML()                                                             # Get necessary data from CADAssembly.xml file
    (stepPath, testBenchName, step) = parseStep(cadAssemblyXML)                                             # Parse necessary information from CADAssembly.xml for the Step file
    
    (xmlMetricsName, metricComponentsXML, jointsMBDXML) = parseCADAssemblyMetricsXML()                      # Get necessary data from CADAssembly_metrics.xml file
    unitLength = getUnitLength(metricComponentsXML)                                                         # Get unit length information from CADAssembly_metrics.xml file

    (anchorID, anchorPointID) = parseKinComputedValuesXML()                                                    # Get necessary data from ComputedValues.xml file

    (unitScale, unitShort) = calcGeoScaleFac(unitLength)                                                    # Calculate the geometry scale factor

    # Generate necessary general dictionaries
    (instRef, instIndex) = generateInstRef(metricComponentsXML, xmlMetricsName,
        xmlName)                                                                                            # Generate "instRef" dictionary

    mtrlRef = generateMtrlRef(LIBRARY_MANAGER, metricComponentsXML, unitScale)                              # Generate "mtrlRef" dictionary

    metricRef = generateMetricRef(metricSetXML, instIndex)                                                  # Generate "metricRef" dictionary

    superRef = generateSuperRef(instRef, mtrlRef, metricRef, instIndex)                                     # Generate "superRef" dictionary

    # Process the data obtained from the step file
    (pointsBySR, inst2SR, localTMs, localTVs, topLevelAsm, subAsms,
         asmParts) = processStep(stepPath, testBenchName)                                                   # Get the analysis points from the step file

    # Generate necessary geometric dictionaries
    localCoords = generateLocalCoords(inst2SR, pointsBySR)                                                  # Generate "localCoords" dictionary

    datumPointDict = coordTransform(localTMs,localTVs,topLevelAsm,
        subAsms,asmParts,localCoords)                                                                       # Generate "datumPointDict" dictionary

    # Start building Abaqus model
    createParts(myModel, step)                                                                              # Create parts in Abaqus by importing the top level assembly Step file

    deleteInvalidParts(myModel)                                                                             # Delete the invalid parts that came with the Step file

    defineStaticStep(myModel)                                                                               # Define a static analysis FEA step 

    defineMaterials(myModel, mtrlRef)                                                                       # Define a materials inside the Abaqus model

    (includeAnchoredPart, anchoredPart) = decideAnchoredPart(jointsMBDXML,
        anchorID, anchorPointID, instRef, myModel)                                                          # Decide if the anchored part will be included in the analysis

    try:
        for (key,entry) in instRef.iteritems():                                                             # Iterate through all parts
            
            if not includeAnchoredPart and key == anchoredPart:
                continue
            else:
                pass

            assignSections(key, entry, myModel, myAsm)                                                      # Assigning section and create an assembly instance from the current part

            meshInstances(edgeSeedDensity, unitShort, instRef, myAsm)                                       # Mesh the current part

            constrainWithSoftSprings(key, myAsm)                                                            # Constrain every vertice of the part in x, y and z directions with soft-springs

            refPointRef = readAndDefineMarkers(key, MAIN, myAsm)                                            # Read the markers from the LOD files and define them in the Abaqus assembly as reference points

            (refPntOtherComp, sortedOtherComp) = getBoundingNodes(jointsMBDXML,
                key, instRef,myModel, myAsm)                                                                # Get the nodes inside the bounding box of the part which is connected to the current part

            defineMPCs(sortedOtherComp, refPntOtherComp, refPointRef,
                instRef, myAsm, myModel, key)                                                               # Define MPCs between the reference points (markers) and the corresponding surfaces on the part

            inpName = createINP(myAsm, key, myModel, parallelCores, ramAllocated, mdb)                      # Create an INP file of the current part

            cleanModel(myModel, myAsm)                                                                      # Clean the current model of the Abaqus/CAE from the previous part itself and its constraints
                                                                   
            inpTempFileName = inpName + '.inp'                                                              # Define the previously created INP file of the current part as the temporary INP file
            inpTempFilePath = os.path.join(MAIN, inpTempFileName)                                           # Define the location of the temporary INP file and store it
            inpTempFile = open(inpTempFilePath, 'r')                                                        # Open the temporary INP file

            stopLine = parseInpTemp(inpTempFile)                                                            # Determine upto which point the temporary INP file needs to be merged with the final INP file

            lodFileName = 'LT_' + key + '.lod'                                                              # Define the name of the LOD file by using the name of the current part
            lodFilePath = os.path.join(MAIN, lodFileName)                                                   # Define the location of the LOD file and store it
            lodFile = open(lodFilePath, 'r')                                                                # Open the LOD file

            startLine = parseLOD(lodFile)                                                                   # Determine from which point the LOD file generated by Adams needs...
                                                                                                            # ...to be merged with the final INP file
            inpTempFile.close()                                                                             # Close the temporary INP file (essential)
            lodFile.close()                                                                                 # Close the LOD file (essential)
            
            inpTempFile = open(inpTempFilePath, 'r')                                                        # Open the temporary INP file again (essential)
            lodFile = open(lodFilePath, 'r')                                                                # Open the LOD file again (essential)
            
            inpFileName = key + '.inp'                                                                      # Define the name of the final INP file by using the name of the current part
            inpFilePath = os.path.join(MAIN, inpFileName)                                                   # Define the location of the final INP file and store it
            open(inpFilePath, 'a').close()                                                                  # Create the final INP file by opening and closing it

            inpFile = createFinalInp(inpFilePath, inpTempFile, lodFile, stopLine,
                startLine)                                                                                  # Merge temporary INP file and LOD file to create the final INP file
                       
            inpFile.close()                                                                                 # Close the final INP file
            inpTempFile.close()                                                                             # Close the temporary INP file

            os.remove(inpTempFilePath)                                                                      # Delete the temporary INP file
    
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: ...\n')
        raise

    f_p.write('\n')
    f_p.write("**********************************************************************************" + '\n')
    f_p.write('\n')

    runAbaqus(superRef,anchoredPart,includeAnchoredPart)                                                    # Run the Abaqus solver with the created input decks...
                                                                                                            # ...for each part seperately
   
##    if maxNumberIter == 1 or maxNumberIter == 0:
##        runAbaqus(superRef,anchoredPart,includeAnchoredPart)
##    else:
##        runAbaqusAdaptive(superRef,anchoredPart,includeAnchoredPart,maxNumberIter)                        ### Need to find a way to make adaptivity work with orphan mesh

    f_p.close()

if __name__ == "__main__":
    main()
