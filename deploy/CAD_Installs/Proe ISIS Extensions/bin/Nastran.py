import sys
import argparse
import shutil
from subprocess import call
import os
from _winreg import *

parser = argparse.ArgumentParser(description='Run Nastran on input .nas Deck')

parser.add_argument('nas_filename', help='.nas File Name')
#parser.add_argument('paramFile', help='.xml ComputedValues File Name')
args = parser.parse_args()

if (not os.path.exists(args.nas_filename)):
	print("Error: %s file not found" % nas_filename)
	
aReg = ConnectRegistry(None,HKEY_LOCAL_MACHINE)

aKey = OpenKey(aReg,"SOFTWARE\Wow6432Node\MSC.Software Corporation\MSC Nastran\Latest")
Ver = str(QueryValueEx(aKey,None)[0])
bKey = OpenKey(aReg,"SOFTWARE\Wow6432Node\MSC.Software Corporation\MSC Nastran\\" + Ver)


NasPath = QueryValueEx(bKey,"Path")[0]

print "\nStarting Nastran \n"
retcode = call(NasPath + "\\bin\\Nastran " + "estimate " + args.nas_filename, shell=True)

if(retcode == 0):
	print "Nastran Execution Successful!!"