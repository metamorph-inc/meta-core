__author__ = 'curator'

import sys
import os
import argparse
import win32com
import glob
import subprocess

import gme

def run_importer(project, lib_root, acm_file):
    """ Runs the component importer. Project should not be in a transaction. acm_files is a list of acm or xml
        AVM component model file, which will be imported into the project.
    """
    print 'Running Component Importer'
    importer = win32com.client.DispatchEx("MGA.Interpreter.CyPhyComponentImporter")
    importer.Initialize(project)  # initialize GMEConsole and MGAGateway
    project.BeginTransactionInNewTerr()  # transaction is required by the ImportFiles function call
    
    importer.ImportFile(project, lib_root, acm_file)  # import components defined in the list of files
    
    project.CommitTransaction()  # save imported component in the project
    
def main(argv=sys.argv):    
    #try:
    xme_file = "InputModel.xme"
    # import xme file and open gme project using gmepy
    mga_file = gme.xme2mga(xme_file)  # import XME
    # get project directory
    project = gme.Project.open(mga_file)
    
    run_importer(project.project,".","InputModel.component.acm")
    
		
		
    return 0

if __name__ == "__main__":
    sys.exit(main())
