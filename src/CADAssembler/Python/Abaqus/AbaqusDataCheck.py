"""

AbaqusDataCheck.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar   <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - This script checks the Abaqus model for constraint errors
        & attempts to resolve any issues automatically.

"""

import os
import logging
import cad_library
from abaqus import *
from abaqusConstants import *
import caeModules
import jobMessage
import odbAccess
import interaction
import visualization
import regionToolset
import job


def CreateOverlapPNG(jobName, overlapCheck, root):
    """ Generate PNG files displaying locations of overlapping nodes relative to entire design. """
    try:
        myOdb = odbAccess.openOdb(path=jobName + '.odb')

        resultsDir = os.path.join(root, jobName)
        if not os.path.exists(resultsDir):
            os.mkdir(resultsDir)
        
        mainDir = os.path.join(root, jobName, "Contour_and_BC_plots")
        if not os.path.exists(mainDir):
            os.mkdir(mainDir)
        os.chdir(mainDir)

        myViewport = session.viewports['Viewport: 1']
        myViewport.setValues(displayedObject=myOdb)

        for k in range(len(overlapCheck.elements)):
            overlappingElems = overlapCheck.elements[k]
            for i in range(len(overlappingElems)):
                overlappingElem = overlappingElems[i]
                highlight(overlappingElem)

        myViewport.view.setValues(session.views['Iso'])
        myViewport.view.zoom(0.8)
        session.printToFile("Overlapping_Elements_1", PNG, (myViewport,))

        myViewport.view.setValues(cameraPosition=(987.505, -35.8282, 7.68834), cameraUpVector=(0, 1, 0))
        myViewport.view.fitView()
        session.printToFile("Overlapping_Elements_2", PNG, (myViewport,))

        session.viewports['Viewport: 1'].view.setValues(nearPlane=757.99, farPlane=1200.36, width=542.18,
                                                        height=365.72, cameraPosition=(0.996667, 36.7201, -977.094),
                                                        cameraUpVector=(0.0, 1.0, 0.0),
                                                        cameraTarget=(-0.87486, -35.267, 7.11584))
        myViewport.view.fitView()
        session.printToFile("Overlapping_Elements_3", PNG, (myViewport,))

        session.viewports['Viewport: 1'].view.setValues(nearPlane=759.096, farPlane=1215.06, width=542.971,
                                                        height=366.255, cameraPosition=(-91.9079, -1009.75, -32.4658),
                                                        cameraUpVector=(-1.0, 0.0, 0.0),
                                                        cameraTarget=(1.67948, -27.8817, -0.616374))
        myViewport.view.fitView()
        session.printToFile("Overlapping_Elements_4", PNG, (myViewport,))
        
    except:
        cad_library.exitwitherror('Error in creating overlap PNG files.', -1, 'AbaqusDataCheck.py')
    os.chdir(root)  


def dataCheck_final(myModel, args):
    """ Submit an input file data check analysis to Abaqus. """
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Checking the model for possible constraint errors (final check)." + '\n')
    dataCheckJobName3 = 'DataCheckFinal'
    myDataCheckJob3 = mdb.Job(name=dataCheckJobName3, model=myModel.name,
                              description='Final Data Check job', multiprocessingMode=DEFAULT,
                              parallelizationMethodExplicit=DOMAIN, numCpus=args.parallelCores,
                              numDomains=args.parallelCores)
    myDataCheckJob3.submit(datacheckJob=True)
    myDataCheckJob3.waitForCompletion()
    logger.info("Final data check completed with success" + '\n')


def dataCheck_two(myModel, args):
    """ Submit a data check analysis reflecting modified Abaqus model. """
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Checking the model for possible constraint errors (2nd check)." + '\n')
    dataCheckJobName2 = 'DataCheck2'
    myDataCheckJob2 = mdb.Job(name=dataCheckJobName2, model=myModel.name,
                              description='2nd Data Check job', multiprocessingMode=DEFAULT,
                              parallelizationMethodExplicit=DOMAIN, numCpus=args.parallelCores,
                              numDomains=args.parallelCores)
    myDataCheckJob2.submit(datacheckJob=True)
    myDataCheckJob2.waitForCompletion()
    logger.info("2nd data check completed with success" + '\n')


def tryModifyingConstraints(myModel, problemConstraints, errorNodes, first_attmpt):
    """ Attempt to resolve constraint issues by swapping master/slave pairing on each offending instance. """
    logger = logging.getLogger()
    logger.info("Attempting to modify constraints" + '\n')
    for key in errorNodes.instanceNames:
        stringKey = str(key)
        for surf in myModel.rootAssembly.allSurfaces.keys():
            surfKey = str(surf)
            if stringKey in surfKey:
                logger.info("Modifying nodes on surface " + str(surfKey) + " on instance " + str(stringKey) + '\n')
                CPswitchKey = surfKey.replace(stringKey, "").strip()
                for swap in myModel.constraints.keys():
                    if swap.startswith(CPswitchKey):
                        logger.info("Modifying constraint " + str(swap) + '\n')
                        if first_attmpt:
                            myModel.constraints[swap].swapSurfaces()
                            problemConstraints.append(CPswitchKey)
                        else:
                            if CPswitchKey in problemConstraints:
                                myModel.constraints[swap].swapSurfaces()


def dataCheck_one(myModel, args):
    """ Submit an input file data check analysis to Abaqus. """
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Checking the model for possible constraint errors (1st check)." + '\n')
    dataCheckJobName = 'DataCheck'
    myDataCheckJob = mdb.Job(name=dataCheckJobName, model=myModel.name,
                             description='1st Data Check job', multiprocessingMode=DEFAULT,
                             parallelizationMethodExplicit=DOMAIN, numCpus=args.parallelCores,
                             numDomains=args.parallelCores)
    myDataCheckJob.submit(datacheckJob=True)
    myDataCheckJob.waitForCompletion()
    fControl = True
    logger.info("1st data check completed with success" + '\n')
    
    return fControl

    
def checkOverlap(odb_inst, jobName):
    """ Check ODB file for overlapping nodes (components) and exit if any exist. """
    logger = logging.getLogger()
    overlapCheck = False
    try:
        overlapCheck = odb_inst.rootAssembly.elementSets['ErrElemVolSmallNegZero']
    except:
        pass
    if overlapCheck:
        logger.info("There are elements in the model with zero or negative volume" + '\n')
        logger.info("Possibly parts are overlapping each other excessively" + '\n')
        logger.info("Unable to run the analyis, please fix your CAD model" + '\n')
        logger.info("Terminating" + '\n')
        
        CreateOverlapPNG(jobName, overlapCheck, os.getcwd())
        logger.info("Check \"results/abcdef/Analysis/Abaqus/Contour_and_BC_plots/Overlapping_Elements_#.png\" "
                    "files to determine problematic parts/elements" + '\n')
        cad_library.exitwitherror('Overlapping parts in thed design. Please see log file.', -1, 'AbaqusDataCheck.py')
    

def checkConstraintErrors(myModel, args):
    """
        Submit a series of data check jobs in Abaqus to analyze input deck representing model.
        If constraint issues are present, attempt to resolve issues and submit again.
        If parts are overlapping, or subsequent data check fails, exit program.
    """
    logger = logging.getLogger()
    try:
        fControl = dataCheck_one(myModel, args)
    except:
        fControl = False
        logger.info("1st data check failed" + '\n')
        logger.info("Constraint issues might be present" + '\n')
    
    problemConstraints = []
    if not fControl:
        jobName = 'DataCheck'
        odb = odbAccess.openOdb(path=jobName + '.odb')
        try:
            errorNodes = odb.rootAssembly.nodeSets['ErrNodeOverconTieSlave']
            tryModifyingConstraints(myModel, problemConstraints, errorNodes, True)
        except:
            logger.info("WARNING: Cannot modify constraints" + '\n')
            logger.info("WARNING: Turning ON the option for adjusting initial surface "
                        "positions and specifying initial clearances" + '\n')
            for key in myModel.constraints.keys():
                if key[:5] == 'Rigid':
                    continue
                myModel.constraints[key].setValues(adjust=ON)
                if key[0:3] == 'CC-':
                    myModel.constraints[key].setValues(tieRotations=OFF)
    
        try:
            dataCheck_two(myModel, args)
        except:
            logger.info("2nd data check failed" + '\n')
            jobName2 = 'DataCheck2'
            odb2 = odbAccess.openOdb(path=jobName2 + '.odb')
            
            try:
                errorNodes = odb2.rootAssembly.nodeSets['ErrNodeOverconTieSlave']
                tryModifyingConstraints(myModel, problemConstraints, errorNodes, True)
            except:
                pass
            checkOverlap(odb2, jobName2) 
    try:
        dataCheck_final(myModel, args)
    except:
        logger.info("Final data check failed" + '\n')
        jobName3 = 'DataCheckFinal'
        odb3 = odbAccess.openOdb(path=jobName3 + '.odb')
        checkOverlap(odb3, jobName3)
        
    logger.info("**********************************************************************************" + '\n')
