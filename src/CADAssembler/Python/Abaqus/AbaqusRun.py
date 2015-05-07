"""

AbaqusRun.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - The script which is responsible from running
      the created Abaqus deck.

"""
import sys, os, time
import string as STR
import traceback
import logging
import glob
from abaqus import *
from abaqusConstants import *
import job

from utility_functions import CopyOrDeleteResults
import _winreg
with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
    META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]


# read Abaqus MSG file to determine job ending condition
def read_msg_file(job_name, maxNumberIter=None):
    logger = logging.getLogger()
    logger.info("**********************************************************************************" + '\n')
    logger.info("Parsing Abaqus MSG file for job ending condition... \n")
    fname = str(job_name) + '.msg'
    try:
        msg = open(fname,'rb')
    except IOError:
        found = False
        if maxNumberIter is not None:   # Adaptive run that finished before maxNumberIterations
            while int(maxNumberIter) > 0:
                maxNumberIter -= 1
                try:
                    fname = job_name[:-1] + str(maxNumberIter) + '.msg'
                    logger.info(fname)
                    msg = open(fname,'rb')  # Try opening different iter
                    found = True
                    break
                except IOError:
                    continue
            if not found:
                err = 'Adaptive iteration message file could not be found!'
                logger.error(err)
                raise Exception(err)
    m = msg.read()
    # Check for divergence message along with ending error statement
    # (avoids case of initial divergence but convergence before job completion)
    if (m.find(" ***NOTE: THE SOLUTION APPEARS TO BE DIVERGING. CONVERGENCE IS JUDGED UNLIKELY.") != -1
              and m.find(" ***ERROR: TOO MANY ATTEMPTS MADE FOR THIS INCREMENT") != -1):
        logger.error("ENDING CONDITION: 'ERROR - Too many attempts made for this increment. \n" \
                        "The solution appears to be diverging. Convergence is judged unlikely.'\n")
        logger.error('\n\nFAILED.\n')
        raise Exception("NoConvergenceError")
    elif m.find("THE ANALYSIS HAS BEEN COMPLETED") != -1:
        logger.info("ENDING CONDITION: 'SUCCESS - 'The analysis has been completed.'\n")
    else:
        err = "UNKNOWN ENDING CONDITION: Please look at abaqus msg file.\n"
        logger.error(err)
        logger.error('\n\nFAILED.\n')
        raise Exception(err)
    msg.close()


def runAbaqusStandard(myModel, superRef, args, includeAnchoredPart, 
                      anchoredPart, analysisStepName):
    logger = logging.getLogger()
    if (includeAnchoredPart and anchoredPart) is not None:
        runAbaqusStandard_Adams(superRef, includeAnchoredPart, anchoredPart, analysisStepName)
        return None
    else:
        jobName = 'staticFEAAnalysis'
        logger.info("**********************************************************************************" + '\n')
        logger.info("Creating the static analysis job and submitting it" + '\n')
        instRef = superRef['instRef']
        try:
            myJob = mdb.Job(name=jobName, model=myModel.name,
                description='Static FEA job',multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores,numDomains=args.parallelCores,
                memory=args.ramAllocated,memoryUnits=PERCENTAGE,
                parallelizationMethodExplicit=DOMAIN)
        except Exception as e:
            logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
            logger.error('Error during creating and submitting the standard nonadaptive analysis process\n')
            raise
        try:
            myJob.submit()
            myJob.waitForCompletion()
        except:
            read_msg_file(jobName)
        read_msg_file(jobName)
        return jobName

def runAbaqusAdaptive(myModel, myAsm, superRef, analysisStepName, rigidPartPresent, 
                        includeAnchoredPart, anchoredPart, rigidParts, args, maxNumberIter):
    logger = logging.getLogger()
    if (includeAnchoredPart and anchoredPart) is not None:
        runAbaqusAdaptive_Adams(superRef, anchoredPart, includeAnchoredPart, maxNumberIter, analysisStepName)
        return None
    else:
        logger.info("**********************************************************************************" + '\n')
        logger.info("Creating and submitting the adaptive meshing process" + '\n')
        instRef = superRef['instRef']
        
        j = job.Job(model=myModel.name, type=ANALYSIS,
            description='Adaptive mesh', multiprocessingMode=DEFAULT,
            numCpus=args.parallelCores, numDomains=args.parallelCores,
            numGPUs=0, queue=None, memory=args.ramAllocated,
            memoryUnits=PERCENTAGE, echoPrint=OFF, 
            modelPrint=OFF, contactPrint=OFF, historyPrint=ON, 
            userSubroutine='', scratch='', explicitPrecision=SINGLE, 
            nodalOutputPrecision=SINGLE)
        processName = 'Adaptivity-1'
        mdb.AdaptivityProcess(jobPrefix='',job=j,name=processName,
            maxIterations=maxNumberIter)
        instList = myAsm.instances.keys()
        for i in range(len(myAsm.instances)):
            ruleName = 'RemeshingRule-%d' % i
            myInst = myAsm.instances[instList[i]]
            myInstName = myInst.name
            if rigidPartPresent:
                if myInstName in rigidParts:
                    pass
                else:
                    setName = 'Set-%d' % i
                    if instRef[instList[i]]['isShell']:
                        f1Mask = myInst.faces.getMask()
                        f1 = myInst.faces.getSequenceFromMask(mask=f1Mask)
                        region = myAsm.Set(faces=f1,name=setName)
                    else:
                        c1Mask = myInst.cells.getMask()                     # preferred for Abaqus 6.13
                        c1 = myInst.cells.getSequenceFromMask(mask=c1Mask)
                        region = myAsm.Set(cells=c1,name=setName)
                    mdb.models['Model-1'].RemeshingRule(name=ruleName, 
                        region=region, description='Remesh rule from element energy error indicator', 
                        stepName=analysisStepName, outputFrequency=LAST_INCREMENT, sizingMethod=DEFAULT,
                        variables=(args.errorIndicator,), errorTarget=0.0, coarseningFactor=NOT_ALLOWED,
                        maxSolutionErrorTarget=0.0, minSolutionErrorTarget=0.0, specifyMinSize=False,
                        specifyMaxSize=False, elementCountLimit=None, refinementFactor=DEFAULT_LIMIT)
            else:
                setName = 'Set-%d' % i
                if instRef[instList[i]]['isShell']:
                    f1Mask = myInst.faces.getMask()
                    f1 = myInst.faces.getSequenceFromMask(mask=f1Mask)
                    region = myAsm.Set(faces=f1,name=setName)
                else:
                    c1Mask = myInst.cells.getMask()                     # preferred for Abaqus 6.13
                    c1 = myInst.cells.getSequenceFromMask(mask=c1Mask)
                    region = myAsm.Set(cells=c1,name=setName)
                mdb.models['Model-1'].RemeshingRule(name=ruleName, 
                    region=region, description='Remesh rule from Mises stress error indicator', 
                    stepName=analysisStepName, outputFrequency=LAST_INCREMENT, sizingMethod=DEFAULT,
                    variables=(args.errorIndicator,), errorTarget=0.0, coarseningFactor=NOT_ALLOWED,
                    maxSolutionErrorTarget=0.0, minSolutionErrorTarget=0.0, specifyMinSize=False,
                    specifyMaxSize=False, elementCountLimit=None, refinementFactor=DEFAULT_LIMIT)
                
        lastJobName = processName + '-iter' + repr(maxNumberIter)
        mdb.adaptivityProcesses[processName].submit(waitForCompletion=True)
        read_msg_file(lastJobName, maxNumberIter)
    
        return lastJobName
    
    
def runAbaqusDynamic(myModel, args, superRef, includeAnchoredPart, anchoredPart, analysisStepName):
    logger = logging.getLogger()
    if (includeAnchoredPart and anchoredPart) is not None:
        runAbaqusDynamic_Adams(superRef, includeAnchoredPart, anchoredPart, analysisStepName)
        return None
    else:
        jobName = 'dynamicFEAAnalysis'
        logger.info("**********************************************************************************" + '\n')
        logger.info("Creating the dynamic analysis job and submitting it" + '\n')
        try:
            myJob = mdb.Job(name=jobName, model=myModel.name,
                description='Dynamic FEA job',multiprocessingMode=DEFAULT,
                numCpus=args.parallelCores,numDomains=args.parallelCores,
                memory=args.ramAllocated,memoryUnits=PERCENTAGE,
                parallelizationMethodExplicit=DOMAIN)
        except Exception as e:
            logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
            logger.error('Error during creating and submitting the dynamic analysis process\n')
            raise
        try:
            myJob.submit()
            myJob.waitForCompletion()
        except:
            read_msg_file(jobName)      # Check log file for issue
        read_msg_file(jobName)
        return jobName
    
    
def runAbaqusModal(myModel, args):
    logger = logging.getLogger()
    jobName = 'modalAnalysis'
    logger.info("**********************************************************************************" + '\n')
    logger.info("Creating the modal analysis job and submitting it" + '\n')
    try:
        myJob = mdb.Job(name=jobName, model=myModel.name,
            description='Modal Analysis job',multiprocessingMode=DEFAULT,
            numCpus=args.parallelCores,numDomains=args.parallelCores,
            memory=args.ramAllocated,memoryUnits=PERCENTAGE,
            parallelizationMethodExplicit=DOMAIN)
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error during creating and submitting the modal analysis process\n')
        raise
    try:
        myJob.submit()
        myJob.waitForCompletion()
    except:
        read_msg_file(jobName) 
    return jobName
    
    
def runAbaqusDispatch(myModel, myAsm, superRef, analysisStepName, rigidPartPresent,
                      rigidParts, args, includeAnchoredPart, anchoredPart, maxNumberIter):
    logger = logging.getLogger()
    if maxNumberIter > 1 and not args.modal:
        jobName = runAbaqusAdaptive(myModel, myAsm, superRef, analysisStepName, rigidPartPresent,
                                     includeAnchoredPart, anchoredPart, rigidParts, args, maxNumberIter)
    elif args.dynamicExplicit or args.dynamicImplicit:
        jobName = runAbaqusDynamic(myModel, args, superRef, includeAnchoredPart, anchoredPart, analysisStepName)
    elif args.modal:
        if maxNumberIter > 1:
            logger.info("WARNING: MaxNumberIterations is set to greater than 1. Adaptive remeshing for a " + \
                        "modal analysis is not supported. Currently running standard modal analysis on design.")
        jobName = runAbaqusModal(myModel, args)
    else:
        jobName = runAbaqusStandard(myModel, superRef, args, includeAnchoredPart, 
                                    anchoredPart, analysisStepName)
        
    return jobName


##### These functions are called if ADAMS output provided.
def runAbaqusStandard_Adams(superRef, includeAnchoredPart, anchoredPart, analysisStepName):
    logger = logging.getLogger()
    logger.info('Running standard FEA analyses using LCK files \n')
    instRef = superRef['instRef']

    for inp in glob.glob("*.inp"):
        key = inp.rsplit('.inp',1)[0]
        if not includeAnchoredPart and key == anchoredPart:
            continue
        else:
            pass
        
        timeout = 600
        sleepInt = 20
        attempts = 0

        jobName = key
        path_to_file = jobName + ".lck"
        logger.info('Running standard FEA analyses on ' + str(key))
        os.system("abaqus job=" + jobName + " background")

        time.sleep(sleepInt)
        while ( attempts < timeout and os.path.exists(path_to_file) and os.path.isfile(path_to_file) ):
            time.sleep(sleepInt)
            attempts = attempts + 1
        
        read_msg_file(jobName)
        # Invoke post processing
        os.system("abaqus cae noGUI=" + META_PATH + "\\bin\\cad\\ABQ_CompletePostProcess.py -- " + \
                      "-o " + jobName + ".odb " + "-p " + "..\\AnalysisMetaData.xml" + " -m " + \
                      "..\\..\\RequestedMetrics.xml " + "-j " + "..\\..\\testbench_manifest.json " + \
                      "-a")

# def runAbaqusAdaptive_Adams(superRef,anchoredPart,includeAnchoredPart,maxNumberIter,analysisStepName):
    # instRef = superRef['instRef']
    # for key in instRef:
        # inpName = str(key) + '.inp' 
        # mdb.ModelFromInputFile(name=key,inputFileName=inpName)
        # myJob = mdb.Job(name=key, model=key,description='Adaptive-Static FEA job')
        # #myJob = mdb.Job(name=key, model=mdb.models['Model-1'].name,description='Adaptive FEA job')
        # processName = key
        # mdb.AdaptivityProcess(job=myJob, name=processName, maxIterations=maxNumberIter)
        # for step in mdb.models[key].steps.keys():
            # if not step == 'Initial':
                # mdb.models[key].RemeshingRule(name=step, 
                    # stepName=step, variables=("MISESERI","ENDENERI",))
        # # for step in mdb.models['Model-1'].steps.keys():
            # # if not step == 'Initial':
                # # mdb.models['Model-1'].RemeshingRule(name=step, 
                    # # stepName=step, variables=("MISESERI","ENDENERI",))
        # mdb.adaptivityProcesses[processName].submit(waitForCompletion=True)
        # read_msg_file(key)
        # afterJob(key, superRef, analysisStepName)
        
        
def runAbaqusDynamic_Adams(superRef, includeAnchoredPart, anchoredPart, analysisStepName):
    logger = logging.getLogger()
    logger.info('Running Explicit Dynamic FEA analyses using LCK files \n')
    instRef = superRef['instRef']

    for inp in  glob.glob("*.inp"):
        key = inp.rsplit('.inp',1)[0]
        
        if not includeAnchoredPart and key == anchoredPart:
            continue
        else:
            pass
        
        timeout = 600
        sleepInt = 20
        attempts = 0

        jobName = key
        path_to_file = jobName + ".lck"
        logger.info('Running Explicit Dynamic FEA analyses on ' + str(key))
        os.system("abaqus job=" + jobName + " background")

        time.sleep(sleepInt)
        while ( attempts < timeout and os.path.exists(path_to_file) and os.path.isfile(path_to_file) ):
            time.sleep(sleepInt)
            attempts = attempts + 1
        
        read_msg_file(jobName)
        # Invoke post processing
        os.system("abaqus cae noGUI=" + META_PATH + "\\bin\\cad\\ABQ_CompletePostProcess.py -- " + \
                      "-o " + jobName + ".odb " + "-p " + "..\\AnalysisMetaData.xml" + " -m " + \
                      "..\\..\\RequestedMetrics.xml " + "-j " + "..\\..\\testbench_manifest.json " + \
                      "-a")
        #afterJob(key,superRef,analysisStepName)        
        #CopyOrDeleteResults(key, runAdams=True)
