"""

Adams2AbaqusPost.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - Includes modules which take care of the
      post-processing.

"""

from abaqus import *
from abaqusConstants import *
from caeModules import *
from numpy import *
from jobMessage import *
from optparse import *
from operator import xor
import multiprocessing
import math, re, sys, json, os
import interaction, visualization, regionToolset, job, mesh
import numpy.linalg as LA
import xml.etree.ElementTree as ET
import string as STR
import uuid, ctypes
import csv
import _winreg
import odbAccess
import shutil
from collections import defaultdict
import os
import glob
import traceback
from subprocess import Popen
import pickle

root = os.getcwd()                                                       # initial working directory
logDir = os.path.join(root,"log","CyPhy2AbaqusCmd.log")                      # path to log file


####  Rigid Bodies will results in no value for summary.  Summary will need to be updated to allow this.  Currently it prodeces an error.
# callback function used to monitor job status, export results, execute post-processing
def afterJob(jobName,superRef):

    analysisDir = os.path.join(root,"Analysis", "Abaqus")
    if not os.path.exists(analysisDir):
        os.makedirs(analysisDir)
    f = open(logDir, "a")                       # open the log file

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write('Job complete.\n')

    instRef = superRef['instRef']                        
    mtrlRef = superRef['mtrlRef']                           
    metricRef = superRef['metricRef']                     
                
    try:
        odb = odbAccess.openOdb(path=jobName + '.odb')
        f.write('Reporting results of %s.\n' % jobName)
    except Exception as e:
        f.write('ERROR: Error in reporting results of %s.\n' % jobName)
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        raise

    mySteps = odb.steps
    numSteps = len(mySteps)
    
    for i in range(numSteps):
        stepKey = mySteps.keys()[i]
        step = mySteps[stepKey]

        try:
            sout = step.frames[-1].fieldOutputs['S'].values                                                                 # stress field output
            maxStress = -1
            for j in range(len(sout)):                                                                            # for each value in the stress field output:
                try:                                                                                              # get name of part instance
                    if sout[j].mises > maxStress:                                                                 # if the Mises stress is greater than maxStress for that part:
                        maxStress = sout[j].mises                                                                 # set the part's maxStress equal to that Mises stress
                        maxStressStep = i                                                                         # store the frame number of the maximum stress
                except AttributeError:
                    pass
        except:
            pass
 
    try:
        mtrlID = instRef[jobName]['MaterialID']            # get the Material ID
        failFatigueStress = mtrlRef[mtrlID]\
            ['mechanical__strength_fatigue']                        # get the Fatigue Strength material property
        failYieldStress = mtrlRef[mtrlID]\
            ['mechanical__strength_tensile_yield']                  # get the Yield Tensile Strength material property
        failUltimateStress = mtrlRef[mtrlID]\
            ['mechanical__strength_tensile_ultimate']               # get the Ultimate Tensile Strength material property
        fosFatigue = failFatigueStress/maxStress           # store the factor of safety for fatigue strength
        fosYield = failYieldStress/maxStress               # store the factor of safety for yield strength
        fosUltimate = failUltimateStress/maxStress         # store the factor of safety for ultimate strength
        fos = failFatigueStress/maxStress
    except:
        maxStress = 'Err'
        failFatigueStress = 'Err'
        failYieldStress = 'Err'
        failUltimateStress = 'Err'
        fosFatigue = 'Err'
        fosYield = 'Err'
        fosUltimate = 'Err'
        fos = 'Err'
        f.write("Insufficient data for FactorOfSafety!\n")
        pass

    headers = ['Part Name','Unique ID','Fatigue Strength (MPa)','Yield Strength (MPa)','Ultimate Tensile Strength (MPa)','Maximum Stress (MPa)', 
        'Factor of Safety for Fatigue Strength','Factor of Safety for Yield Strength','Factor of Safety for Ultimate Tensile Strength']

    output = [jobName,instRef[jobName]['ComponentID'],failFatigueStress,failYieldStress,failUltimateStress,maxStress,
        fosFatigue,fosYield,fosUltimate]
    
    report_file = jobName + '.csv'
    with open(report_file, 'wb') as fp:
        a = csv.writer(fp, delimiter=',')
        data = [headers,output]
        a.writerows(data)

    f.close()
    
    try:
        CreateViewportPNG(odb, jobName, maxStressStep)
    except:
        pass

    odb.close()

    os.chdir(root)

    CopyOrDeleteResults(jobName)


def CopyOrDeleteResults(fileName):

    f = open(logDir, "a")                                       # open the log file  
    
    reportFile = 'testbench_manifest.json'                      # name of metric JSON
    reportPath = os.path.join(root, reportFile)
    mainDir = os.path.join(root,"Analysis","Abaqus",fileName)

    try:
        with open(reportPath, 'r') as json_data:
            data = json.load(json_data)
        usrInput = data["CopyTestResults"]
        if not usrInput:
            f.write('\n')
            f.write("**********************************************************************************" + '\n')
            f.write('\n')
            f.write('Deleting result files\n')
            f.write('\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            for hgx in glob.glob("*.odb"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.023"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.com"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.dat"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.inp"):
                if os.path.splitext(hgx)[0] == fileName:
                    os.remove(hgx)
                    f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.mdl"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.msg"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.prt"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sim"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.stt"):
                os.remove(hgx)
                f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                if os.path.splitext(hgx)[0] == fileName:
                    os.remove(hgx)
                    f.write('Deleting file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sta"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.csv"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
        else:
            f.write('\n')
            f.write("**********************************************************************************" + '\n')
            f.write('\n')
            f.write('Moving result files\n')
            f.write('\n')
            os.chdir(root)
            try:
                for lck in glob.glob("*.lck"):
                    os.remove(lck)
            except:
                pass
            for hgx in glob.glob("*.odb"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.023"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.com"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.dat"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.inp"):
                if os.path.splitext(hgx)[0] == fileName:
                    shutil.move(hgx,mainDir)
                    f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.mdl"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.msg"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.prt"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sim"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.stt"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.log"):
                if os.path.splitext(hgx)[0] == fileName:
                    shutil.move(hgx,mainDir)
                    f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.sta"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')
            for hgx in glob.glob("*.csv"):
                shutil.move(hgx,mainDir)
                f.write('Moving file ' + str(hgx) + '\n')

    except:
        f.write('WARNING: Can not move/delete Abaqus artifacts\n')
        pass

    f.close()

def CreateViewportPNG(myOdb, fileName, maxStressStep):

    f = open(logDir, "a")             # open the log file

    f.write('\n')
    f.write("**********************************************************************************" + '\n')
    f.write('\n')
    f.write("Creating the PNG files of the results" + '\n')
    try:

        partDir = os.path.join(root,"Analysis","Abaqus",fileName)
        if not os.path.exists(partDir):
            os.mkdir(partDir)
        
        mainDir = os.path.join(root,"Analysis","Abaqus",fileName,"Stress_and_Displ_Contours")
        if not os.path.exists(mainDir):
            os.mkdir(mainDir)

        os.chdir(mainDir)
        
        myViewport=session.Viewport(name='Contour Plot', origin=(0, 0), width=200, height=100)
        #myOdb = visualization.openOdb(path=odbName)
        myViewport.setValues(displayedObject=myOdb)

        mySteps = myOdb.steps
        numSteps = len(mySteps)

        t = myOdb.userData.Text(name='Text-1', text='MPa',offset=(24.1042, 185.322),\
            font='-*-arial-medium-r-normal-*-*-80-*-*-p-*-*-*')
        myViewport.plotAnnotation(annotation=t)
        
        session.printOptions.setValues(rendition=COLOR,
                                       vpDecorations=OFF, vpBackground=OFF)
        
        stepKey = mySteps.keys()[maxStressStep]
        step = mySteps[stepKey]

        save2fileName=fileName.replace(' ','') + "_"
                    
        #temporary solution until write3DXMLFile issue fixed-------------------------------------------------------------
        displacement = step.frames[-1].fieldOutputs['U']
        myViewport.odbDisplay.setPrimaryVariable(field=displacement,
                                                 outputPosition=NODAL,
                                                 refinement=(INVARIANT, 'Magnitude'))                   
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
        session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
        #temporary solution end--------------------------------------------------------------------------------------
                    
        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
        session.viewports['Contour Plot'].makeCurrent()
        session.viewports['Contour Plot'].maximize()            
        session.writeVrmlFile(save2fileName+"DEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"Displacement_DEF", False, (myViewport,))

        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
        session.viewports['Contour Plot'].makeCurrent()
        session.viewports['Contour Plot'].maximize()            
        session.writeVrmlFile(save2fileName+"UNDEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"Displacement_UNDEF", False, (myViewport,))
        
       
##            #get displacement data
##            f.write("Grabbing displacement data" + '\n')
##            displacement = step.frames[-1].fieldOutputs['U']
##            myViewport.odbDisplay.setPrimaryVariable(field=displacement,
##                                                     outputPosition=NODAL,
##                                                     refinement=(INVARIANT, 'Magnitude'))                   
##            myViewport.odbDisplay.contourOptions.setValues(numIntervals=10)
##            myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_DEF))
##            myViewport.view.setValues(session.views['Iso'])
##            myViewport.view.zoom(0.8)
##            session.printToFile(save2fileName+"Displacement_DEF", PNG, (myViewport,))
        

        #get stress data
        f.write("Grabbing stress data" + '\n')
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
        session.writeVrmlFile(save2fileName+"Stress_DEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"Stress_DEF", False, (myViewport,))

        myViewport.odbDisplay.display.setValues(plotState=(CONTOURS_ON_UNDEF))
        session.viewports['Contour Plot'].makeCurrent()
        session.viewports['Contour Plot'].maximize()            
        session.writeVrmlFile(save2fileName+"Stress_UNDEF", False, (myViewport,))
        session.write3DXMLFile(save2fileName+"Stress_UNDEF", False, (myViewport,))

        #--------------------------------------------------------------------------------------------------------------------


        myViewport.view.setValues(session.views['Iso'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Iso_UNDEF_Meshed", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Left'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Left_UNDEF_Meshed", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Right'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Right_UNDEF_Meshed", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Back'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Back_UNDEF_Meshed", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Front'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Front_UNDEF_Meshed", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Top'])
        myViewport.view.zoom(0.7)
        session.printToFile(save2fileName+"Stress_Top_UNDEF_Meshed", PNG, (myViewport,))         

        myViewport.view.setValues(session.views['Bottom'])
        myViewport.view.zoom(0.7)
        session.printToFile(save2fileName+"Stress_Bottom_UNDEF_Meshed", PNG, (myViewport,))

        #--------------------------------------------------------------------------------------------------------------------

        myViewport.odbDisplay.commonOptions.setValues(
        visibleEdges=FEATURE)

        #--------------------------------------------------------------------------------------------------------------------

        myViewport.view.setValues(session.views['Iso'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Iso_UNDEF", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Left'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Left_UNDEF", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Right'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Right_UNDEF", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Back'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Back_UNDEF", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Front'])
        myViewport.view.zoom(0.8)
        session.printToFile(save2fileName+"Stress_Front_UNDEF", PNG, (myViewport,))

        myViewport.view.setValues(session.views['Top'])
        myViewport.view.zoom(0.7)
        session.printToFile(save2fileName+"Stress_Top_UNDEF", PNG, (myViewport,))         

        myViewport.view.setValues(session.views['Bottom'])
        myViewport.view.zoom(0.7)
        session.printToFile(save2fileName+"Stress_Bottom_UNDEF", PNG, (myViewport,))

            
            
    except KeyError:
        f.write('\n')
        f.write('ERROR: Key Error' + '\n')
        print 'Key Error'
        odb.close()
        exit(0)
    except (AbaqusException), value:
        f.write('ERROR:' + value + '\n')
        print 'ERROR:', value
        odb.close()
        exit(0)

    f.close()
