"""

AbaqusMain.py, version 2.3.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar   <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - The main script which takes care of pre-processing,
      solving and post-processing.
      
    - Can be run with the following parameters:
        1) Mesh Only - Build the assembly and generate mesh with CAE file
        2) Mesh & Boundary Conditions - Create mesh and PNG files of
                boundary conditions applied.
        3) Static/Standard analysis
        4) Explicit Dynamic analysis
        5) Implicit Dynamic analysis
        6) Modal analysis
    
    - Static jobs can be run with adaptive remeshing functionality.
        - Dynamic analyses (Implicit or Explicit) can not use adaptive
          remeshing. This is a restriction on Abaqus, not this script.
    - FEA can be kicked off to run as standalone application
        - Build model and submit jobs through CAE.
    OR
    - FEA can be kicked off by SOT with ADAMS LOD files as an input.
        - Create INP files and run analysis on those.
        
    - ADAMS-to-Abaqus can run with options (1),(3),(4).
        - No adaptive re-meshing (8/29/14)
        - (1) only generates mesh quality CSV files as no CAE is used.
    - Standalone Abaqus can use (1),(2),(3),(5), and (6).
"""

import xml.etree.ElementTree as ET
from xml.dom import minidom
import _winreg
import odbAccess
import shutil
import glob
import logging
from abaqus import *
from abaqusConstants import *
from caeModules import *
from jobMessage import *
from optparse import *
import sys
import json
import os


with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0,
                     _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
    META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]
    sys.path.append(os.path.join(META_PATH, 'bin', 'Python27', 'Lib', 'site-packages'))

import cad_library
import cad_library.unit_utils as unit_utils

abaqusDir = os.path.join(cad_library.META_PATH, 'bin', 'CAD', 'Abaqus')
ppDir = os.path.join(cad_library.META_PATH, 'bin', 'CAD')
sys.path.append(abaqusDir)
sys.path.append(ppDir)

logger = logging.getLogger()

from AbaqusParse import *
from AbaqusGeometry import *
from AbaqusDataCheck import checkConstraintErrors
from AbaqusDict import *
from AbaqusSTEP import *
from AbaqusCAE import *
from AbaqusRun import runAbaqusDispatch
from AbaqusCAE_ADAMS import AbaqusCAE_ADAMS
import ABQ_CompletePostProcess
import ComputedMetricsSummary


def check_for_adams(runAdams, reportFile, kinComValFile, computedValuesXML, cadMetricsXML,
                    root, abqDir):
    """ Scan testbench_manifest.json to look for kinematic dependency. """
    if not os.path.exists(abqDir):
        os.mkdir(abqDir)
    os.chdir(abqDir)
    reportPath = os.path.join(root, reportFile)
    kinComValPath = os.path.join(root, kinComValFile)
    with open(reportPath, 'r') as json_data:
        data = json.load(json_data)
    try:
        adamsData = data["Dependencies"]
        adamsPath = adamsData[0]["Directory"]
        runAdams = True
    except IndexError:
        # Thrown if adamsData is an empty string.
        logger.info('No ADAMS analysis load file found, executing standard analysis! \n')
    # If Adams dependency detected, copy ADAMS load files and ComputedValues.XML.
    if runAdams:
        os.chdir(adamsPath)
        for hgx in glob.glob("*.lod"):  # For each file with extension 'lod'
            shutil.copy(hgx, abqDir)  # Copy lod file to Abaqus simulation directory
        for hgx in glob.glob("*.xml"):
            if hgx == computedValuesXML:
                shutil.copy(hgx, kinComValPath)
            if hgx == cadMetricsXML:
                shutil.copy(hgx, os.path.join(root, cadMetricsXML))
        logger.info('ADAMS analysis load files found! \n')
    os.chdir(root)
    logger.info("**********************************************************************************" + '\n')

    return runAdams


def recurseTree(node, asminfo, uniqueSuffix, asmIdentifier):
    """ Recursively search assembly tree as represented by CAD metrics file. """
    for n in node:  # Iterate through each node in the meta data XML file
        name = n.attrib.get('Name')
        # Find (in CADAssembly_metrics.xml) the section that corresponds to the part whose
        # name matches the one for the node in the meta data XML file.
        model = [child for child in asminfo.componentsdict.values() if child.cadmodelname == name][0]
        if n.attrib.get('Type') == 'PART':
            # Set the unique FEAElementID to the part's name plus associated order number in the metrics file.
            n.set('FEAElementID', str(name + uniqueSuffix + str(model.steporder + 1)))
        else:
            # If the node's associated type is an assembly:
            n.set('FEAElementID', str(name + asmIdentifier + uniqueSuffix + str(model.steporder + 1)))
            recurseTree(n, asminfo, uniqueSuffix, asmIdentifier)
    return


def modifyMetaDataFile(xmlname, asminfo, uniqueSuffix, asmIdentifier):
    """ Update meta data file with internal unique Abaqus instance names. """
    try:
        tree = ET.ElementTree()
        tree.parse(xmlname)
    except Exception, inst:
        cad_library.exitwitherror('Unexpected error opening ' + xmlname + str(inst), 1, 'AbaqusMain.py')

    rootNode = tree.getroot()  # CADAnalysisMetaData

    assemblyNode = rootNode.find('Assemblies/Assembly')
    componentNodes = assemblyNode.findall('./Component/Component')
    # Recurse through the meta data file to update each component with it's unique Abaqus name.
    recurseTree(componentNodes, asminfo, uniqueSuffix, asmIdentifier)
    # Format the updated meta data file string to look 'pretty' in the standard format
    #   Avoids the entire file being written on one line.
    pretty = minidom.parseString(ET.tostring(rootNode, encoding='utf-8')).toprettyxml(indent='    ')
    with open(xmlname, 'w') as data:
        # Open the meta data file for writing and dump the XML formatted modified contents to it.
        data.write(pretty)


def initializeConstants():
    """ Initialize variables to be used throughout program. """
    # File Constants
    cadAssemblyXML = 'CADAssembly.xml'
    cadMetricsXML = 'CADAssembly_metrics.xml'
    requestedMetricsXML = 'RequestedMetrics.xml'
    computedValuesXML = 'ComputedValues.xml'
    kinComputedValuesXML = 'kinComputedValues.xml'
    analysisMetaDataXML = 'AnalysisMetaData.xml'
    testbench_manifest = 'testbench_manifest.json'
    postprocessingScript = 'ABQ_CompletePostProcess.py'

    # Directory Constants
    resultsDir = os.getcwd()
    solverDir = os.path.join(resultsDir, 'Analysis', 'Abaqus')
    stepDir = os.path.join(resultsDir, 'AP203_E2_SINGLE_FILE')

    # Abaqus Constants
    myModel = mdb.models['Model-1']  # Model object within Abaqus. Contains all information related to model.
    myAsm = myModel.rootAssembly  # Assembly object within Abaqus. Contains all information related to assembly.
    uniqueSuffix = '__Z'  # Added to each component for unique identifying purposes (__Z#)
    asmIdentifier = '_ASM'  # Designates between a part and assembly within Abaqus.
    structuralOutputs = ('S', 'MISESMAX', 'PE', 'PEEQ', 'PEMAG', 'LE', 'U', 'RF')  # Possible metric ID codes
    thermalOutputs = ('TEMP',)  # Possible metric ID codes

    return (cadAssemblyXML, cadMetricsXML, requestedMetricsXML, computedValuesXML, kinComputedValuesXML,
            analysisMetaDataXML, testbench_manifest, postprocessingScript, resultsDir, solverDir,
            stepDir, myModel, myAsm, uniqueSuffix, asmIdentifier, structuralOutputs, thermalOutputs)


def main():
    logger.info("**********************************************************************************" + '\n')
    logger.info("**********************************************************************************" + '\n')
    logger.info("*************************   STARTING FEA MODEL-BASED   ***************************" + '\n')
    logger.info("**********************************************************************************" + '\n')
    logger.info("**********************************************************************************" + '\n')
    logger.info("Initializing Abaqus/CAE" + '\n')
    logger.info("**********************************************************************************" + '\n')
    logger.info("**********************************************************************************" + '\n')

    # take command line arguments
    usage = "usage: abaqus cae script=AbaqusMain.py -- [options]"
    parser = OptionParser(usage=usage)
    parser.add_option("-o", "--meshOnly", default=False, action="store_true",
                      help="""disable assembly of faces from datum points and creation
                      of loads and BCs (necessary for META 13.13)""")
    parser.add_option("-b", "--meshAndBC", default=False, action="store_true",
                      help="Create mesh and generate loads/boundary conditions. Do not run analysis.")
    parser.add_option("-s", "--standard", default=False, action="store_true",
                      help="run a standard FEA analysis")
    parser.add_option("-i", "--dynamicImplicit", default=False, action="store_true",
                      help="run a dynamic implicit FEA analysis")
    parser.add_option("-e", "--dynamicExplicit", default=False, action="store_true",
                      help="run a dynamic explicit FEA analysis")
    parser.add_option("-m", "--modal", default=False, action="store_true",
                      help="run a modal analysis")
    parser.add_option("-p", "--parallelCores", default=1, type=int,
                      help="number of CPU cores to use in solver")
    parser.add_option("-r", "--ramAllocated", default=90, type=int,
                      help="integer amount of memory allocated to solver, defined in units memoryUnits")
    parser.add_option("-u", "--memoryUnits", default="PERCENTAGE",
                      choices=["PERCENTAGE", "MEGA_BYTES", "GIGA_BYTES"],
                      help="units of memory allocated to solver")
    parser.add_option("-t", "--separationTolerance", default=1E-3, type=float,
                      help="tolerance for separation between faces when creating tie constraints")
    parser.add_option("-q", "--errorIndicator", default="ENDENERI",
                      choices=["MISESERI", "ENDENERI", "CPRESSERI", "CSHEARERI", "PEEQUERI",
                               "PEERI", "CEERI", "HFLERI", "EFLERI", "EPGERI"],
                      help="""error indicator variable for adaptive meshing;
                      not all variables are supported yet
                      (see Section 4.1.4 of the Abaqus Analysis Manual)""")
    parser.add_option("-v", "--rigidParts", default=True, action="store_true",
                      help="use Rigid parts as specified in CADAssembly.xml")

    if sys.argv[2] == '-noGUI':
        (args, testBenchArg) = parser.parse_args(sys.argv[8:])
    else:
        (args, testBenchArg) = parser.parse_args(sys.argv[6:])
    
    # Initialize constants to be used throughout the analysis.
    (cadAssemblyXML, cadMetricsXML, requestedMetricsXML, computedValuesXML, kinComputedValuesXML,
     analysisMetaDataXML, testbench_manifest, postprocessingScript, resultsDir, solverDir,
     stepDir, myModel, myAsm, uniqueSuffix, asmIdentifier, structuralOutputs, thermalOutputs) \
        = initializeConstants()

    # Determine if ADAMS analysis preceeded this FEA run
    # Looks for dependency notifications in the testbench_manifest.json file.
    runAdams = check_for_adams(False, testbench_manifest, kinComputedValuesXML,
                               computedValuesXML, cadMetricsXML, resultsDir, solverDir)

    # Parse necessary information from the XML files
    (feaXML, cadAssemblyXMLTree, maxNumberIter, analysisConstraintSetXML, thermalSetXML) \
        = parseCADAssemblyXML(cadAssemblyXML, resultsDir, args)

    # TODO: remove runAdams constraint
    if maxNumberIter > 1 and any([args.dynamicImplicit, args.dynamicExplicit, runAdams]):
        cad_library.exitwitherror("Adaptive remeshing requested with invalid analysis type. Only static, "
                                  "thermal, and coupled thermal/displacement analyses are supported. FEA "
                                  "analyses launched from a ADAMS kinematic test bench are also currently "
                                  "invalid. Please change your GME test bench settings.", 1, 'AbaqusMain.py')

    elif runAdams and args.meshAndBC or runAdams and args.modal:
        cad_library.exitwitherror("ADAMS-to-Abaqus runs can not have the Abaqus analysis set to "
                                  "MeshAndBoundaryConditions or Modal.", 1, 'AbaqusMain.py')

    if thermalSetXML and any([args.modal, runAdams]):
        cad_library.exitwitherror("Coupled Thermal analysis not supported for modal or Adams-to-Abaqus"
                                  "analyses.", 1, 'AbaqusMain.py')

    # Parse STEP file for raw assembly data
    (stepPath, testBenchName, step) = parseStep(cadAssemblyXMLTree, stepDir)

    # Populate assembly information from CADAssembly_metrics.xml
    asminfo = cad_library.AssemblyInfo()
    asminfo.read_metrics_file(cadMetricsXML)

    # Determine unit length of CAD files    
    unitLength = unit_utils.getUnitLength(asminfo)

    if runAdams:
        # Parse kinematic computed values (ADAMS data)
        (anchorID, anchorPointID) = parseKinComputedValuesXML(kinComputedValuesXML)

    # Calculate scale factor (with respect to meters)
    (unitScale, unitShort) = unit_utils.calcGeoScaleFac(unitLength)
    # Create dictionary of all possibly used units throughout the analysis along with their conversion factors.
    conv = unit_utils.generateConvDict(unitScale)

    # Generate necessary general dictionaries
    # These contain information parsed from the various input XML files.
    # InstRef creates a dictionary entry for each component.
    (instRef, instIndex, instAssemblyRef, instAssemblyIndex,
     rigidPartMasses, rigidPartVolumes, rigidPartDensities,
     rigidParts, rigidPartPresent, Jan24_deactivate_rigidity) = \
        generateInstRef(asminfo, cadAssemblyXML, uniqueSuffix, asmIdentifier, runAdams, args)
    
    # Generate a dictionary containing material properties for each component in the assembly.
    mtrlRef = generateMtrlRef(asminfo, thermalSetXML, rigidParts, rigidPartDensities, conv)

    if not runAdams:
        # If no Adams dependency is found, generate dictionary of loads and boundary conditions placed on each part.
        (loadBCLib, accel) = generateLoadBCLib(analysisConstraintSetXML, feaXML, thermalSetXML, conv, asminfo)
    else:
        # If Adams dependency exists, loads are defined in the LOD files output from Adams. Boundary conditions
        # are applied at a later point.
        (loadBCLib, accel) = ([], [])

    # Process the data obtained from the step file
    (pointsBySR, inst2SR, localTMs, localTVs, topLevelAsm, subAsms,
     asmParts) = processStep(stepPath, testBenchName, uniqueSuffix, asmIdentifier)

    # Generate necessary geometric dictionaries
    (localCoords, CGs) = generateLocalCoords(inst2SR, pointsBySR, asminfo, uniqueSuffix)
    logger.info("Creating a new dictionary with points translated into the global coordinate system" + '\n')
    datumPointDict = coordTransform(localTMs, localTVs, topLevelAsm, subAsms, asmParts, localCoords)
    logger.info("**********************************************************************************" + '\n')
    
    # Directory where analysis output files and post-processing files will be written to.
    if not os.path.exists(solverDir):
        os.mkdir(solverDir)
    os.chdir(solverDir)

    # Start building Abaqus model
    createParts(myModel, step, CGs)
    deleteInvalidParts(myModel)

    # Define step - standard 'static' default
    if thermalSetXML:
        if args.dynamicExplicit:
            (myStep, amp, analysisStepName) = \
                defineDynamicExplicitCoupledThermalDisplacementStep(myModel, structuralOutputs+thermalOutputs)
        else:
            (myStep, amp, analysisStepName) = defineCoupledThermalDisplacementStep(myModel, args.dynamicImplicit,
                                                                                   structuralOutputs+thermalOutputs)
    else:
        if args.dynamicImplicit or args.dynamicExplicit:
            (myStep, amp, analysisStepName) = defineDynamicStep(myModel, args.dynamicExplicit, structuralOutputs)
        elif args.modal:
            (myStep, amp, analysisStepName) = defineModalStep(myModel)
        else:
            (myStep, amp, analysisStepName) = defineStaticStep(myModel, structuralOutputs)
    
    # Assign materials to each component.
    defineMaterials(myModel, mtrlRef)

    if not runAdams:
        assignSections(instRef, myModel, myAsm, subAsms, asmParts)
        mergeAssemblyComponents(myModel, myAsm, instAssemblyRef, asmParts)
        defineContactInteractions(myModel, args.dynamicExplicit)
        defineRigidBodyConstraints(instRef, Jan24_deactivate_rigidity,
                                   instIndex, myAsm, myModel)
        EliminateOverlaps(instRef, rigidParts, myAsm, myModel)

        apply_loads_and_bcs(myModel, myAsm, myStep, instRef, loadBCLib, instIndex, instAssemblyIndex,
                            datumPointDict, accel, amp, args, Jan24_deactivate_rigidity, thermalSetXML)

        meshInstances(asminfo, 40.0, unitShort, instRef,
                      instAssemblyRef, myAsm, feaXML,  args)

    if runAdams:
        EliminateOverlaps(instRef, rigidParts, myAsm, myModel)

        (includeAnchoredPart, anchoredPart) = decideAnchoredPart(anchorID, anchorPointID, instRef, myModel)

        AbaqusCAE_ADAMS(asminfo, instRef, includeAnchoredPart, anchoredPart, myModel,
                        myAsm, 40.0, unitShort, feaXML, args.parallelCores,
                        args.ramAllocated, args, instAssemblyRef, asmParts)

    else:
        includeAnchoredPart, anchoredPart = None, None

    logger.info("**********************************************************************************" + '\n')

    if not runAdams:
        if not args.meshOnly:
            connectParts(myModel, myAsm, args)
            try:
                CreateLOADSBCPNG(myAsm, analysisStepName, solverDir)
            except:
                logger.info('ERROR: Error during creating BC PNG files - Program will keep executing \n')
                pass
            createCAE(solverDir, testBenchName, args)

            checkConstraintErrors(myModel, args)

            exportNastranDeck()

    modifyMetaDataFile(os.path.join(solverDir, '..', analysisMetaDataXML), asminfo, uniqueSuffix, asmIdentifier)

    if not args.meshOnly and not args.meshAndBC:
        jobName = runAbaqusDispatch(myModel, myAsm, instRef, analysisStepName, rigidPartPresent,
                                    rigidParts, args, includeAnchoredPart, anchoredPart, maxNumberIter,
                                    postprocessingScript, analysisMetaDataXML, requestedMetricsXML, testbench_manifest)

    if not runAdams:
        if args.dynamicImplicit or args.dynamicExplicit or args.standard:
            check = 0
            # Adaptive runs can complete before the MaxNumberIterations,
            # so multiple checks need to be done. Start with assuming it went to MaxNumberIter
            if jobName.startswith("Adaptivity-1-iter"):
                lastIterStr = jobName.replace("Adaptivity-1-iter", "")
                lastIter = int(lastIterStr)

                while lastIter > 0 and check == 0:
                    try:
                        AdaptiveJobName = "Adaptivity-1-iter" + str(lastIter)
                        odbAccess.openOdb(path=AdaptiveJobName + '.odb')
                        check = 1
                        jobName = AdaptiveJobName
                    except:
                        # Error thrown because file does not exist. If lastIter > 1, this means the analysis
                        # completed before the max number allowed. Decrease lastIter and try opening again.
                        logger.info('ERROR: Error in reading results of %s.\n' % AdaptiveJobName)
                        lastIter -= 1

            os.chdir(solverDir)
            # Invoke post processing
            # Spawn new Python process by calling the post-processing script along with necessary arguments.
            os.system("abaqus cae noGUI=" + os.path.join(cad_library.META_PATH, "bin", "CAD", postprocessingScript) +
                      " -- " + "-o " + jobName + ".odb " + "-p " + "..\\" + analysisMetaDataXML + " -m " +
                      "..\\..\\" + requestedMetricsXML + " -j " + "..\\..\\" + testbench_manifest)
        elif args.modal:
            # Modal analyses use a different, older post-processing method as the output ODB is analyzed different.
            ABQ_CompletePostProcess.afterJobModal(jobName, analysisStepName)


if __name__ == "__main__":
    logger = cad_library.setuplogger('AbaqusCmd')  # Initialize logger
    try:
        main()
    except:
        # Overarching try, catch. Used to handle any unexpected errors.
        cad_library.exitwitherror('An unknown exception occurred.', -1, 'AbaqusMain.py')