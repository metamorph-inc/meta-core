"""

Adams2AbaqusRun.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - The script which is responsible from running
      the created Abaqus deck.

"""
import sys, os, time
from Adams2AbaqusPost import *

def runAbaqus(superRef,anchoredPart,includeAnchoredPart):

    instRef = superRef['instRef']

    for key in instRef:
        
        if not includeAnchoredPart and key == anchoredPart:
            continue
        else:
            pass
        
        timeout = 600
        sleepInt = 20
        attempts = 0

        jobName = key
        path_to_file = jobName + ".lck"

        os.system("abaqus job=" + jobName + " background")

        time.sleep(sleepInt)
        while ( attempts < timeout and os.path.exists(path_to_file) and os.path.isfile(path_to_file) ):
            time.sleep(sleepInt)
            attempts = attempts + 1

        afterJob(key,superRef)

def runAbaqusAdaptive(superRef,anchoredPart,includeAnchoredPart,maxNumberIter):

    instRef = superRef['instRef']

    for key in instRef:

        inpName = str(key) + '.inp'
        
        mdb.ModelFromInputFile(name=key,inputFileName=inpName)

        for step in mdb.models[key].steps.keys():
            if not step == 'Initial':
                mdb.models[key].RemeshingRule(name=step, 
                    stepName=step, variables=("MISESERI","ENDENERI",))

        myJob = mdb.Job(name=key, model=key,description='Static FEA job')
        processName = key
        mdb.AdaptivityProcess(job=myJob, name=processName, maxIterations=maxNumberIter)

##        try:
##            mdb.adaptivityProcesses[processName].submit(waitForCompletion=True)
##        except:
##            pass


        mdb.adaptivityProcesses[processName].submit(waitForCompletion=True)
        #afterJob(key,superRef)


