#!/usr/bin/python
###
# This module provides the 'make CF card' function to OM
# The two functions "update_fs" and "prepare_fs" should
# be provided to the user of the OM so that the user 
# can perform either one at any time
#
# At the bottom there is an example of running the function.
# In the OM you'll need to have an option for the user to select
# which device enumeration to use (or input it through text)
###
import copy, os, shutil, subprocess, string, glob, fnmatch, shlex
import threading
import time
import sys
import cad_library

def scan_for_CAD_files(mypath):
    print "Starting test script for ExtractACM-XMLfromCASModules.exe"
        
    from os import listdir
    from os.path import isfile, join, getsize

    matches = []
    for root, dirs, files in os.walk(mypath):
        for filename in fnmatch.filter(files, '*.prt*') + fnmatch.filter(files, '*.asm*'):
            if not filename.endswith('.xml'):
                matches.append(os.path.join(root, filename))

    max_threads = 1
    threads = []
    for fn in matches:
        while count_alive_threads(threads) >= max_threads:
            time.sleep(1)
        newThread = threading.Thread(target=run_the_extractor, kwargs={"filename": fn})
        newThread.start()
        threads.append(newThread)

def count_alive_threads(thread_array):
    count = 0
    for t in thread_array:
        if t.isAlive():
            count += 1
    return count

def run_the_extractor(filename):
    print "converting " + filename
    outfilename = filename + '.xml'
    exe_path = cad_library.META_PATH + r'bin\CAD\Creo\bin\ExtractACM-XMLfromCreoModels.exe'
    arguments = ' -c "'+filename+'" -x "' + outfilename + '"'
    command = exe_path + arguments
    return_code = subprocess.call(command)
    if return_code:
        print " Error on converting file "+ filename + " (return code " + str(return_code) + ")"


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Syntax: testExtractACM <PathtoScan>"
        exit()
    mypath =  sys.argv[1]
    scan_for_CAD_files(mypath)
