"""

AbaqusProcessData.py, version 1.3.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar   <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - This script takes cares of all data pre/post processing.
    - Includes BC/Load/Stress PNG creation & CSV creation

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

from AbaqusThermal import thermalPostProcessing, CreateThermalViewportPNG


def CopyOrDeleteResults(root, fileName, runAdams=False):
    logger = logging.getLogger()
    reportFile = 'testbench_manifest.json'                      # name of metric JSON
    reportPath = os.path.join(root, reportFile)
    mainDir = os.path.join(root,"Analysis","Abaqus",fileName)

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        usrInput = data["CopyTestResults"]
        if not usrInput:
            logger.info("**********************************************************************************" + '\n')
            logger.info('Deleting result files\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            rm_types = ['*.odb','*.023','*.com','*.dat','*.mdl',
                        '*.msg','*.prt','*.sim','*.stt']
            for ext in rm_types:
                for hgx in glob.glob(ext):
                    os.remove(hgx)
                    logger.info('Deleting file ' + str(hgx) + '\n')
            
            for hgx in glob.glob("*.inp"):
                if os.path.splitext(hgx)[0] == fileName:
                    os.remove(hgx)
                    logger.info('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                if os.path.splitext(hgx)[0] == fileName:
                    os.remove(hgx)
                    logger.info('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sta"):
                shutil.move(hgx,mainDir)
                logger.info('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.csv"):
                if runAdams:
                    if fileName in hgx or "stress" in hgx:
                        shutil.move(hgx,mainDir)    # Only move CSV file matching current key
                        logger.info('Moving file ' + str(hgx) + '\n')
                else:
                    shutil.move(hgx,mainDir)
                    logger.info('Moving file ' + str(hgx) + '\n')
        else:
            logger.info("**********************************************************************************" + '\n')
            logger.info('Moving result files\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            m_types = ['*.odb','*.023','*.com','*.dat','*.mdl',
                        '*.msg','*.prt','*.sim','*.stt','*.sta','*.csv']
            for ext in m_types:
                for hgx in glob.glob(ext):
                    logger.info(str(hgx))
                    shutil.move(hgx,mainDir)
                    logger.info('Moving file ' + str(hgx) + '\n')
            
            
            
            for hgx in glob.glob("*.inp"):
                if os.path.splitext(hgx)[0] == fileName:
                    shutil.move(hgx,mainDir)
                    logger.info('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                if os.path.splitext(hgx)[0] == fileName:
                    shutil.move(hgx,mainDir)
                    logger.info('Moving file ' + str(hgx) + '\n')
    except:
        logger.info('WARNING: Can not move/delete Abaqus artifacts\n')
        pass

        
def CreateViewportPNG(myOdb, fileName, myViewport, step):
    logger = logging.getLogger()
    
    stressDir = os.path.join(os.getcwd(),'Stress_Contours')
    if not os.path.exists(stressDir):
        os.mkdir(stressDir)
    os.chdir(stressDir)
    
    save2fileName=fileName.replace(' ','') + "_"
    
    #temporary solution until write3DXMLFile issue fixed-------------------------------------------------------------
    displacement = step.frames[-1].fieldOutputs['U']
    myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                             outputPosition=NODAL,
                                             refinement=(INVARIANT, 'Magnitude'))                   
    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
    session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
    
    logger.info("Creating PNG file " + save2fileName+"Displacement_DEF" + '\n')
    
    #temporary solution end--------------------------------------------------------------------------------------
    
    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
    session.viewports['Contour Plot'].makeCurrent()
    session.viewports['Contour Plot'].maximize()
    session.viewports['Contour Plot'].viewportAnnotationOptions.setValues(
        legendFont='-*-verdana-medium-r-normal-*-*-120-*-*-p-*-*-*')
    session.writeVrmlFile(save2fileName+"DEF", False, (myViewport,))
    session.write3DXMLFile(save2fileName+"Displacement_DEF", False, (myViewport,))

    logger.info("Creating 3DXML file " + save2fileName+"Displacement_DEF" + '\n')

    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
    session.viewports['Contour Plot'].makeCurrent()
    session.viewports['Contour Plot'].maximize()            
    session.writeVrmlFile(save2fileName+"UNDEF", False, (myViewport,))
    session.write3DXMLFile(save2fileName+"Displacement_UNDEF", False, (myViewport,))

    logger.info("Creating 3DXML file " + save2fileName+"Displacement_UNDEF" + '\n')
    
    #get stress data
    logger.info("Grabbing stress data" + '\n')
    stress = step.frames[-1].fieldOutputs['S']
    myViewport.odbDisplay.setPrimaryVariable(field=stress,
                                             outputPosition=INTEGRATION_POINT,
                                             refinement=(INVARIANT, 'Mises'))
    myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
    session.viewports['Contour Plot'].makeCurrent()
    session.viewports['Contour Plot'].maximize()
    session.viewports['Contour Plot'].viewportAnnotationOptions.setValues(
        legendFont='-*-verdana-medium-r-normal-*-*-120-*-*-p-*-*-*')
    session.writeVrmlFile(save2fileName+"Stress_DEF", False, (myViewport,))
    session.write3DXMLFile(save2fileName+"Stress_DEF", False, (myViewport,))

    logger.info("Creating 3DXML file " + save2fileName+"Stress_DEF" + '\n')

    myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
    session.viewports['Contour Plot'].makeCurrent()
    session.viewports['Contour Plot'].maximize()            
    session.writeVrmlFile(save2fileName+"Stress_UNDEF", False, (myViewport,))
    session.write3DXMLFile(save2fileName+"Stress_UNDEF", False, (myViewport,))

    logger.info("Creating 3DXML file " + save2fileName+"Stress_UNDEF" + '\n')

    #--------------------------------------------------------------------------------------------------------------------


    myViewport.view.setValues(session.views['Iso'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Iso_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Iso_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Left'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Left_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Left_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Right'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Right_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Right_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Back'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Back_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Back_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Front'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Front_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Front_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Top'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Stress_Top_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Top_UNDEF_Meshed" + '\n')

    myViewport.view.setValues(session.views['Bottom'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Stress_Bottom_UNDEF_Meshed", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Bottom_UNDEF_Meshed" + '\n')

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.odbDisplay.commonOptions.setValues(
    visibleEdges=FEATURE)

    #--------------------------------------------------------------------------------------------------------------------

    myViewport.view.setValues(session.views['Iso'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Iso_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Iso_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Left'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Left_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Left_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Right'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Right_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Right_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Back'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Back_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Back_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Front'])
    myViewport.view.zoom(0.8)
    session.printToFile(save2fileName+"Stress_Front_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Front_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Top'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Stress_Top_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Top_UNDEF" + '\n')

    myViewport.view.setValues(session.views['Bottom'])
    myViewport.view.zoom(0.7)
    session.printToFile(save2fileName+"Stress_Bottom_UNDEF", PNG, (myViewport,))

    logger.info("Creating PNG file " + save2fileName+"Stress_Bottom_UNDEF" + '\n')
    os.chdir('..')
    
def SetupViewportPNG(myOdb, fileName, thermal, structural, maxStressStep=None):
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Creating the PNG files of the results" + '\n')
    root = os.getcwd()
    try:
        mainDir = os.path.join(root,"Analysis","Abaqus",fileName,"Stress_and_Displ_Contours")
        if not os.path.exists(mainDir):
            os.mkdir(mainDir)

        os.chdir(mainDir)
        
        myViewport=session.Viewport(name='Contour Plot', origin=(0, 0), width=200, height=100)
        myViewport.setValues(displayedObject=myOdb)

        mySteps = myOdb.steps
        numSteps = len(mySteps)

        session.printOptions.setValues(rendition=COLOR, vpDecorations=OFF, vpBackground=OFF)

        if structural:
            t = myOdb.userData.Text(name='Text-1', text='MPa',offset=(34.1042, 179.422),\
                font='-*-arial-medium-r-normal-*-*-120-*-*-p-*-*-*')
            myViewport.plotAnnotation(annotation=t)
            try:
                stepKey = mySteps.keys()[maxStressStep]
            except IndexError:
                logger.debug('WARNING: maxStressStep key in mySteps.keys() does not exist! ' +\
                             'Will try creating viewport using old method.')
                for i in range(numSteps):
                    myViewport.odbDisplay.commonOptions.setValues(visibleEdges=EXTERIOR)
                    stepKey = mySteps.keys()[i]
                    step = mySteps[stepKey]
                    CreateViewportPNG(myOdb, fileName, myViewport, step)
            else:
                myViewport.odbDisplay.commonOptions.setValues(visibleEdges=EXTERIOR)
                step = mySteps[stepKey]
                CreateViewportPNG(myOdb, fileName, myViewport, step)
            
        if thermal:
            t = myOdb.userData.Text(name='Text-1', text='K',offset=(34.1042, 179.422),\
                    font='-*-arial-medium-r-normal-*-*-120-*-*-p-*-*-*')
            myViewport.plotAnnotation(annotation=t)
            for i in range(numSteps):
                myViewport.odbDisplay.commonOptions.setValues(visibleEdges=EXTERIOR)    #Turn mesh plot on
                CreateThermalViewportPNG(myOdb, fileName, myViewport, mySteps, i)
            
        
               
    except KeyError:
        logger.error('Key Error' + '\n')
        odb.close()
        raise
    except (AbaqusException), value:
        logger.error(value + '\n')
        odb.close()
        raise      
        

def CreateLOADSBCPNG(PreProcessFileName, myAsm, analysisStepName, root):
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Creating the PNG files of the loads and BC's" + '\n')

    analysisDir = os.path.join(root,"Analysis", "Abaqus")
    if not os.path.exists(analysisDir):
        os.makedirs(analysisDir)

    mainDir = os.path.join(root,"Analysis","Abaqus","Contour_and_BC_plots")
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
    
    
def CreateOverlapPNG(jobName, overlapCheck, root):
    logger = logging.getLogger()
    try:
        myOdb = odbAccess.openOdb(path=jobName + '.odb')
        save2fileName=jobName.replace(' ','') + "_"

        resultsDir = os.path.join(root,"Analysis","Abaqus", jobName)
        if not os.path.exists(resultsDir):
            os.mkdir(resultsDir)
        
        mainDir = os.path.join(root,"Analysis","Abaqus", jobName, "Contour_and_BC_plots")
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

        session.viewports['Viewport: 1'].view.setValues(nearPlane=757.99, farPlane=1200.36, width=542.18, height=365.72, cameraPosition=(0.996667, 
            36.7201, -977.094), cameraUpVector=(0.0, 1.0, 0.0), cameraTarget=(-0.87486, -35.267, 7.11584))
        myViewport.view.fitView()

        session.printToFile("Overlapping_Elements_3", PNG, (myViewport,))

        session.viewports['Viewport: 1'].view.setValues(nearPlane=759.096, farPlane=1215.06, width=542.971, height=366.255, cameraPosition=(-91.9079, 
            -1009.75, -32.4658), cameraUpVector=(-1.0, 0.0, 0.0), cameraTarget=(1.67948, -27.8817, -0.616374))
        myViewport.view.fitView()

        session.printToFile("Overlapping_Elements_4", PNG, (myViewport,))
        
    except:
        logger.error('Error in creating overlap PNG files. \n')
        pass
    os.chdir(root)  


####  Rigid Bodies will results in no value for summary.  Summary will need to be updated to allow this.  Currently it prodeces an error.
# callback function used to monitor job status, export results, execute post-processing
def afterJob(jobName, superRef, analysisStepName, runAdams, thermalSetXML=None):
    logger = logging.getLogger()
    root = os.getcwd()
    analysisDir = os.path.join(root,"Analysis", "Abaqus", jobName)
    if not os.path.exists(analysisDir):
        os.makedirs(analysisDir)
    logger.info("**********************************************************************************" + '\n')
    logger.info('Job complete\n')

    check = 0
    if jobName.startswith("Adaptivity-1-iter"):
        lastIterStr = jobName.replace("Adaptivity-1-iter","")
        lastIter = int(lastIterStr)

        while lastIter > 0 and check == 0:
            try:
                AdaptiveJobName = "Adaptivity-1-iter" + str(lastIter)
                checkOdb = odbAccess.openOdb(path=AdaptiveJobName + '.odb')
                frameCheck = checkOdb.steps[analysisStepName].frames[1]
                check = 1
                jobName = AdaptiveJobName
            except:
                logger.info('ERROR: Error in reading results of %s.\n' % AdaptiveJobName)
                lastIter = lastIter - 1

    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        logger.info('Reporting results of %s.\n' % jobName)
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in reporting results of %s.\n' % jobName)
        raise

    instRef = superRef['instRef']                         # identify refs in superRef
    instAssemblyRef = superRef['instAssemblyRef']
    mtrlRef = superRef['mtrlRef']                         # ''
    metricRef = superRef['metricRef']                     # ''
    metricRef2 = superRef['metricRef2']
    metricRef3 = superRef['metricRef3']
    lengthUnit = superRef['lengthUnit']                   # ''
    allAssembly = superRef['allAssembly']

    maxStress = {}
    maxTemp = {}
    maxDisp = {}
    fosFatigue = {}
    fosYield = {}
    fosUltimate = {}
    fos = {}
    maxTemp = None
    adamsInst = None
    mySteps = odb.steps
    numSteps = len(mySteps)
    maxStressStep = None
    
    if thermalSetXML is not None:
        (maxTemp, minTemp) = thermalPostProcessing(odb, mySteps, numSteps, jobName, superRef, analysisStepName)
    
    if 'Mises' in metricRef:        # Stress analysis
        try:
            if jobName in instRef.keys() or \
                    jobName in instAssemblyRef.keys():       # This is hit if Adams2Abaqus - otherwise jobName's != any key
                maxStress.update([[jobName,0]])                     # create a maxStress entry
                maxDisp.update([[jobName,0]])                       # create a maxDisp entry
                fosFatigue.update([[jobName,0]])                    # create a fos entry for fatigue strength
                fosUltimate.update([[jobName,0]])                   # create a fos entry for ultimate strength
                fos.update([[jobName,0]])
            else:
                for key in instRef.keys():                      # for each part/subasm instance:
                    if instRef[key]['isRigid']:
                        continue
                    maxStress.update([[key,0]])                     # create a maxStress entry
                    maxDisp.update([[key,0]])                       # create a maxDisp entry
                    fosFatigue.update([[key,0]])                    # create a fos entry for fatigue strength
                    fosUltimate.update([[key,0]])                   # create a fos entry for ultimate strength
                    fos.update([[key,0]])
        except Exception as e:
            logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
            logger.error('Error in creating entires for post-processing\n')
            raise
            
        for j in range(numSteps):
            stepKey = mySteps.keys()[j]
            step = mySteps[stepKey]
            sout = step.frames[-1].fieldOutputs['S'].values
            for i in range(len(sout)):                      # for each value in the stress field output:
                try:
                    inst = sout[i].instance.name                    # get name of part instance
                    if instRef[inst]['isRigid']:
                        continue
                    if sout[i].mises > maxStress[inst]:             # if the Mises stress is greater than maxStress for that part:
                        maxStress[inst] = sout[i].mises             # set the part's maxStress equal to that Mises stress
                        maxStressStep = j
                except AttributeError:
                    pass

        if allAssembly == False:
            if 'Mises' in metricRef:                        # if Mises stress is a metric in the XML:
                for inst in maxStress.keys():                   # iterating through key+value pairs in maxStress
                    if inst in metricRef['Mises']:
                        metricRef['Mises'][inst] = maxStress[inst]
            try:
                for inst in maxStress.keys():                   # for each entry in maxStress:
                    mtrlID = instRef[inst]['MaterialID']            # get the Material ID
                    failFatigueStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_fatigue']                        # get the Fatigue Strength material property
                    failYieldStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_tensile_yield']                  # get the Yield Tensile Strength material property
                    failUltimateStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_tensile_ultimate']               # get the Ultimate Tensile Strength material property
                    fosFatigue[inst] = failFatigueStress/maxStress[inst]        # store the factor of safety for fatigue strength
                    fosYield[inst] = failYieldStress/maxStress[inst]           # store the factor of safety for fatigue strength
                    fosUltimate[inst] = failUltimateStress/maxStress[inst]      # store the factor of safety for ultimate strength
                    fos[inst] = failFatigueStress/maxStress[inst]
                for inst in fos.keys():                                         # for each entry in fos:
                    if inst in metricRef['FactorOfSafety']:
                        metricRef['FactorOfSafety'][inst] = fos[inst]
            except:
                logger.error("Insufficient data for FactorOfSafety\n")
                raise
                
            try:
                for j in range(numSteps):
                    stepKey = mySteps.keys()[j]
                    step = mySteps[stepKey]
                    uout = step.frames[-1].fieldOutputs['U'].values           # displacement field output
            except:
                logger.error('Error in reading the displacement data\n')
                raise
       
                for i in range(len(uout)):                      # for each value in the disp. field output:
                    try:
                        inst = uout[i].instance.name
                        if uout[i].magnitude > maxDisp[inst]:           # if disp. magnitude is greater than maxDisp:
                            maxDisp[inst] = uout[i].magnitude           # set maxDisp equal to that disp. magnitude
                    except AttributeError:
                        continue
            if 'Displacement' in metricRef:                 # if displacement is a metric in the XML:
                for inst in maxDisp.keys():                     # for each entry in maxDisp:
                    if inst in metricRef['Displacement']:
                        metricRef['Displacement'][inst] = maxDisp[inst]

            idArray = []
            reportFile = 'testbench_manifest.json'         # name of metric JSON
            reportPath = os.path.join(root, reportFile)

            logger.info("Updating the testbench_manifest.json file" + '\n')

            try:
                with open(reportPath, 'r') as json_data:
                    data = json.load(json_data)
                ourResults = data["Metrics"]
                for rs in ourResults:
                    idArray.append(rs["GMEID"])
                for component in idArray:
                    for component2 in metricRef2:
                        if component == component2:
                            for component3 in maxStress:
                                if metricRef3[component] == instRef[component3]['ComponentID']:
                                    if metricRef2[component] == 'Mises':
                                        for cs in ourResults:
                                            if cs["GMEID"] == component:
                                                cs["Value"] = maxStress[component3]
                                                cs["Unit"] = 'MPa'
                                    elif metricRef2[component] == 'FactorOfSafety':
                                        for cs in ourResults:
                                            if cs["GMEID"] == component:
                                                cs["Value"] = fos[component3]
                                                cs["Unit"] = 'None'
                minFosFatigue = -1
                for inst in fosFatigue.keys():
                    if fosFatigue[inst] < minFosFatigue or minFosFatigue == -1:
                        minFosFatigue = fosFatigue[inst]

                minFosYield = -1
                for inst in fosYield.keys():
                    if fosYield[inst] < minFosYield or minFosYield == -1:
                        minFosYield = fosYield[inst]

                minFosUltimate = -1
                for inst in fosUltimate.keys():
                    if fosUltimate[inst] < minFosUltimate or minFosUltimate == -1:
                        minFosUltimate = fosUltimate[inst]

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Fatigue_Strength_Factor_of_Safety':
                        cs["Value"] = minFosFatigue
                        cs["Unit"] = 'None'

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Yield_Strength_Factor_of_Safety':
                        cs["Value"] = minFosYield
                        cs["Unit"] = 'None'

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Ultimate_Tensile_Strength_Factor_of_Safety':
                        cs["Value"] = minFosUltimate
                        cs["Unit"] = 'None'

                with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
                    json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary       

            except:
                logger.error('Error in exporting data to %s.\n' % reportFile)
                raise

        else:
            try:
                for inst in maxStress.keys():                   # for each entry in maxStress:
                    mtrlID = instRef[inst]['MaterialID']            # get the Material ID
                    failFatigueStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_fatigue']                        # get the Fatigue Strength material property
                    failYieldStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_tensile_yield']                  # get the Yield Tensile Strength material property
                    failUltimateStress = mtrlRef[mtrlID]\
                        ['mechanical__strength_tensile_ultimate']               # get the Ultimate Tensile Strength material property
                    fosFatigue[inst] = failFatigueStress/maxStress[inst]        # store the factor of safety for fatigue strength
                    fosYield[inst] = failYieldStress/maxStress[inst]            # store the factor of safety for yield strength
                    fosUltimate[inst] = failUltimateStress/maxStress[inst]      # store the factor of safety for ultimate strength
                    fos[inst] = failFatigueStress/maxStress[inst]  
            except:
                logger.error("Insufficient data for FactorOfSafety\n")
                raise

            idArray = []
            reportFile = 'testbench_manifest.json'         # name of metric JSON
            reportPath = os.path.join(root, reportFile)

            logger.info("Updating the testbench_manifest.json file" + '\n')

            try:
                with open(reportPath, 'r') as json_data:
                    data = json.load(json_data)
                ourResults = data["Metrics"]
                minFosFatigue = -1
                for inst in fosFatigue.keys():
                    if fosFatigue[inst] < minFosFatigue or minFosFatigue == -1:
                        minFosFatigue = fosFatigue[inst]

                minFosYield = -1
                for inst in fosYield.keys():
                    if fosYield[inst] < minFosYield or minFosYield == -1:
                        minFosYield = fosYield[inst]

                minFosUltimate = -1
                for inst in fosUltimate.keys():
                    if fosUltimate[inst] < minFosUltimate or minFosUltimate == -1:
                        minFosUltimate = fosUltimate[inst]

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Fatigue_Strength_Factor_of_Safety':
                        cs["Value"] = minFosFatigue
                        cs["Unit"] = 'None'

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Yield_Strength_Factor_of_Safety':
                        cs["Value"] = minFosYield
                        cs["Unit"] = 'None'

                for cs in ourResults:
                    if cs["Name"] == 'Minimum_Ultimate_Tensile_Strength_Factor_of_Safety':
                        cs["Value"] = minFosUltimate
                        cs["Unit"] = 'None'               
        
                with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
                    json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary
            except:
                logger.error('Error in exporting data to %s.\n' % reportFile)
                raise
    
    # END if stress analysis
            
    try:    
        logger.info("Creating the CSV file" + '\n')

        tempInstRef = {}
        headers = {}
        val = {}
        forCSVlist = []   
    
        headers['Part Name'] = {'fatigueStr': 'Fatigue Strength (MPa)', 'yieldStr': 'Yield Strength (MPa)', 'ultimateStr': 'Ultimate Tensile Strength (MPa)', 'fosFatigue': 'Factor of Safety for Fatigue Strength', 
            'fosYield': 'Factor of Safety for Yield Strength', 'fosUltimate': 'Factor of Safety for Ultimate Tensile Strength', 'maxStress': 'Maximum Stress (MPa)', 'MaterialID': 'IloveMyJob', 'order': 1,
            'isPart': 'NA', 'ComponentID': 'Unique ID', 'maxTemp': 'Maximum Temperature (K)', 'minTemp': 'Minimum Temperature (K)'}
        tempInstRef = instRef

        orderNum = 2
        for key in tempInstRef:
            matID = tempInstRef[key]['MaterialID']
            if key in maxStress:
                try:
                    tempInstRef[key]['maxStress'] = maxStress[key]
                    tempInstRef[key]['fosFatigue'] = fosFatigue[key]
                    tempInstRef[key]['fosYield'] = fosYield[key]
                    tempInstRef[key]['fosUltimate'] = fosUltimate[key]
                    tempInstRef[key]['fatigueStr'] = mtrlRef[matID]['mechanical__strength_fatigue']
                    tempInstRef[key]['yieldStr'] = mtrlRef[matID]['mechanical__strength_tensile_yield']
                    tempInstRef[key]['ultimateStr'] = mtrlRef[matID]['mechanical__strength_tensile_ultimate']
                except:
                    tempInstRef[key]['maxStress'] = 'Err'
                    tempInstRef[key]['fosFatigue'] = 'Err'
                    tempInstRef[key]['fosYield'] = 'Err'
                    tempInstRef[key]['fosUltimate'] = 'Err'
            else:
                if jobName in instRef.keys() and key != jobName:    # ADAMS run, but not on analyzed component
                    continue
                else:
                    tempInstRef[key]['maxStress'] = 'N/A'
                    tempInstRef[key]['fosFatigue'] = 'N/A'
                    tempInstRef[key]['fosYield'] = 'N/A'
                    tempInstRef[key]['fosUltimate'] = 'N/A'
        
            if maxTemp and minTemp:
                tempInstRef[key]['maxTemp'] = maxTemp[key]
                tempInstRef[key]['minTemp'] = minTemp[key]
        
            tempInstRef[key]['order'] = orderNum
            orderNum = orderNum + 1        

        forCSV = dict(headers, **tempInstRef)
        
        if maxTemp and minTemp:
            for d in forCSV:
                try:
                    if len(maxStress) == 0:     # Thermal analysis only
                        temp = (d, forCSV[d]['ComponentID'], forCSV[d]['maxTemp'], forCSV[d]['minTemp'], forCSV[d]['order'])
                    else:       # Coupled analysis
                        temp = (d, forCSV[d]['ComponentID'], forCSV[d]['fatigueStr'], forCSV[d]['yieldStr'], forCSV[d]['ultimateStr'], forCSV[d]['maxStress'],
                                forCSV[d]['fosFatigue'],forCSV[d]['fosYield'],forCSV[d]['fosUltimate'], forCSV[d]['maxTemp'],
                                forCSV[d]['minTemp'], forCSV[d]['order'])  
                except KeyError:
                    continue    # ADAMS run - no data for this key available yet.
                except IndexError:
                    logger.info("Error adding stress data to CSV file! Most likely caused by running test bench " + \
                                "without FEAComputation blocks. Continuing test bench.")
                    continue
                forCSVlist.append(temp)
        else:   # Structural analysis only
            for d in forCSV:
                try:
                    temp = (d, forCSV[d]['ComponentID'], forCSV[d]['fatigueStr'], forCSV[d]['yieldStr'], forCSV[d]['ultimateStr'], forCSV[d]['maxStress'],
                        forCSV[d]['fosFatigue'],forCSV[d]['fosYield'],forCSV[d]['fosUltimate'], forCSV[d]['order'])
                except KeyError:
                    continue
                except IndexError:
                    logger.info("Error adding stress data to CSV file! Most likely caused by running test bench " + \
                                "without FEAComputation blocks. Continuing test bench.")
                    continue
                forCSVlist.append(temp)
        forCSVlist.sort(key=lambda x: float(x[-1]))  # Sort by orderNum - independent of structural or thermal analysis
      
        report_file2 = 'stressOutput.csv'
        try:
            with open(report_file2,'wb') as f:
                writer = csv.writer(f)
                for d in range(0, orderNum-1):
                    try:
                        if maxTemp and minTemp:
                            if len(maxStress) == 0:
                                val = (forCSVlist[d][0], forCSVlist[d][1], forCSVlist[d][2], forCSVlist[d][3])
                            else:
                                val = (forCSVlist[d][0], forCSVlist[d][1], forCSVlist[d][2], forCSVlist[d][3], forCSVlist[d][4],
                                       forCSVlist[d][5], forCSVlist[d][6], forCSVlist[d][7], forCSVlist[d][8], forCSVlist[d][9], forCSVlist[d][10])
                        else:
                            val = (forCSVlist[d][0], forCSVlist[d][1], forCSVlist[d][2], forCSVlist[d][3], forCSVlist[d][4],
                                   forCSVlist[d][5], forCSVlist[d][6], forCSVlist[d][7], forCSVlist[d][8])
                    except IndexError:
                        logger.info("Error adding stress data to CSV file! Most likely caused by running test bench " + \
                                    "without FEAComputation blocks. Continuing test bench.")
                        continue
                    writer.writerow(val)
        except:
            logger.error('Error in exporting data to %s.\n' % report_file2)
            raise
    except:
        logger.error('Error in creating the CSV file\n')
        raise

    if analysisStepName in ['staticLoading', 'coupledTempDispl']:
        if maxTemp: 
            thermal = True
        else:
            thermal = False
        if len(maxStress) > 0: 
            structural = True
        else:
            structural = False
        try:
            SetupViewportPNG(odb, jobName, thermal, structural, maxStressStep)
        except Exception as e:
            logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
            logger.error('Error\n')
            raise
        

    odb.close()
    os.chdir(root)
    try:
        CopyOrDeleteResults(root, jobName, runAdams)
    except:
        logger.error('Error in copying or deleting result files to users machine\n')
        pass


    logger.info("**********************************************************************************" + '\n')
    logger.info('Success\n')
    logger.info("**********************************************************************************" + '\n')
    
    
def afterJobModal(jobName,analysisStepName):
    logger = logging.getLogger()
    odb = odbAccess.openOdb(path=jobName + '.odb')
    logger.info("**********************************************************************************" + '\n')
    logger.info('Job complete\n')
    root = os.getcwd()
    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        headers = (('Mode Number','Frequency(Hz)'))
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in opening %s.\n' % jobName)
        raise
    try:
        for histReg in odb.steps[analysisStepName].historyRegions.keys():
            eigenFrequencies = odb.steps[analysisStepName].historyRegions[histReg].historyOutputs['EIGFREQ'].data
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in reading eigenfrequencies\n')
        raise
        
    forCSV = (headers,) + eigenFrequencies
    logger.info("Creating the CSV file" + '\n')
    
    report_file2 = 'modalOutput.csv'
    try:
        with open(report_file2,'wb') as f2:
            writer = csv.writer(f2)
            for (number,frequency) in forCSV:
                val = (number, frequency)
                writer.writerow(val)                    
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in exporting data to %s.\n' % report_file2)
        pass
    
    idArray = []
    reportFile = 'testbench_manifest.json'                              # name of metric JSON
    reportPath = os.path.join(os.getcwd(), reportFile)
    logger.info("Updating the testbench_manifest.json file" + '\n')

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        ourResults = data["Metrics"]

        minMode = -1
        for (key,entry) in eigenFrequencies:
            if entry < minMode or minMode == -1:
                   minMode = entry
        if minMode == 0:
            logger.info('WARNING: Rigid body modes are present, model has not been constrained properly' + '\n')
        for cs in ourResults:
            if cs["Name"] == 'Minimum_Mode':
                cs["Value"] = minMode
                cs["Unit"] = 'Hz'
    
        with open(reportPath, 'wb') as file_out:                        # open JSON file in write mode...
            json.dump(data, file_out, indent=4)                         # and overwrite with new dictionary

    except:
        logger.error('Error in exporting data to %s.\n' % reportFile)
        raise

    odb.close()
    os.chdir(root)
    try:
        CopyOrDeleteResults(root, jobName)
    except:
        logger.error('Error in copying or deleting result files to users machine\n')
        pass

    logger.info("**********************************************************************************" + '\n')
    logger.info('Success\n')
    logger.info("**********************************************************************************" + '\n')

    
