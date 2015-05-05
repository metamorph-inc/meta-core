import sys
from optparse import OptionParser
from odbAccess import *
#from abaqus import *
from abaqusConstants import *
#import visualization


# ===================================================================================================
# Global Variables 
# ===================================================================================================
gFile = open('StressTensors.txt', 'w')


# ===================================================================================================


# ===================================================================================================
# Functions
#

def PrintStressTensors(fileName):
    global gFile

    odbFileName=fileName

    stress = 'S'

    # Shows how to print key()
    #elemset = assembly.elementSets.keys()
    #for item in elemset:
    #    Write2Log(str(item) +'\n')

    gFile.write('MaxPrincipal,MidPrincipal,MinPrincipal,Tresca,Mises,Press,Tensor')
    try:
        myOdb = openOdb(path=odbFileName)
        lastFrame = myOdb.steps['STEP_1_SOL_101_LC'].frames[-1]
        #stressFields = lastFrame.fieldOutputs[stress]
        stressFields = lastFrame.fieldOutputs[stress]
        print 'Available Invariants of Stress:'
        print stressFields.validInvariants
        for stressValue in stressFields.values:
            gFile.write('%10.4E,%10.4E,%10.4E,%10.4E,%10.4E,%10.4E \n' % (stressValue.maxPrincipal, stressValue.midPrincipal, stressValue.minPrincipal, stressValue.tresca, stressValue.mises, stressValue.press) )
            for data in stressValue.data:
                gFile.write(',%10.5E' % data)

            
        myOdb.close()           # close odb
        gFile.close()
            
    except KeyError:
        print 'Key Error'
        myOdb.close()
        gFile.close()
        sys.exit(0)
    except AbaqusException, value:
        print 'Error:', value
        myOdb.close()
        gFile.close()
        sys.exit(0)


  
# ===================================================================================================

# ===================================================================================================
# START
#

if __name__ == '__main__':
    odbName = None
    paramFile = None
    argList = sys.argv
    argc = len(argList)
    i = 0
    while (i < argc):
        if (argList[i][:2] == '-i'):
            i+=1
            odbName = argList[i]
        i+=1
    PrintStressTensors(odbName)

