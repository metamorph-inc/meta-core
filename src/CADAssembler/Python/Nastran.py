import sys
import argparse
import shutil
from subprocess import call
import os
from _winreg import *

def exitwitherror(msg,code):
    f = open('_FAILED.txt','a')
    f.write(msg + '\n') # python will convert \n to os.linesep
    f.close() # you can omit in most cases as the destructor will call if
    exit(code)

parser = argparse.ArgumentParser(description='Run Nastran on input .nas Deck')

parser.add_argument('nas_filename', help='.nas File Name')
#parser.add_argument('paramFile', help='.xml ComputedValues File Name')
args = parser.parse_args()


if (not os.path.exists(args.nas_filename)):
    print("Error: %s file not found" % nas_filename)

try:
    aReg = ConnectRegistry(None,HKEY_LOCAL_MACHINE)

    aKey = OpenKey(aReg,"SOFTWARE\Wow6432Node\MSC.Software Corporation\MSC Nastran\Latest")
    Ver = str(QueryValueEx(aKey,None)[0])
    bKey = OpenKey(aReg,"SOFTWARE\Wow6432Node\MSC.Software Corporation\MSC Nastran\\" + Ver)
except WindowsError as e:
    exitwitherror('Nastran.py: Can\'t read NASTRAN path from registry. Do you have it installed?',-1)


NasPath = QueryValueEx(bKey,"Path")[0]

print "\nStarting Nastran \n"
retcode = call(NasPath + "\\bin\\Nastran " + "estimate " + args.nas_filename, shell=True)

if(retcode == 0):
    print "Nastran Execution Successful!!"

sys.exit(0)