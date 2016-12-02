import os
import sys
import glob
import json
import shutil
import logging
from logging.handlers import RotatingFileHandler
from time import gmtime, strftime


def right_trim(input,suffix):
    fileName = None
    pos = input.find(suffix)
    if (pos == -1):
        fileName = input
    else:
        fileName = input[0:pos]
    return fileName
    
   
def CopyOrDeleteResults(fileName, reportPath, runAdams=False):
    logger = logging.getLogger()
    #reportFile = 'testbench_manifest.json'
    #reportPath = os.path.join(os.getcwd(),'..','..', reportFile)


    mainDir = os.path.join(os.getcwd(),fileName)

    with open(reportPath, 'r') as json_data:
        data = json.load(json_data)
    usrInput = data["CopyTestResults"]
    try:
        for lck in glob.glob("*.lck"):
            os.remove(lck)
    except:
        logger.info('WARNING: Unable to delete LCK file')
    
    rm_types = ['*.odb','*.023','*.com','*.dat','*.mdl',
                    '*.msg','*.prt','*.sim','*.stt']#.jnl
    m_types = ['*.sta','*.csv','*.bdf'] #cae, inp
    list_types = ['*.log']

    for hgx in glob.glob('*'):
        # Data check artifacts should be deleted regardless
        if 'DataCheck' in hgx or 'DataCheck2' in hgx or 'DataCheckFinal' in hgx:
            os.remove(hgx)
    
    if not usrInput:
        # Delete the following files
        for ext in rm_types:
            for hgx in glob.glob(ext):
                try:
                    os.remove(hgx)
                except WindowsError:
                    continue
                logger.info('Deleting file ' + str(hgx) + '\n')
        for ext in list_types:    
            for hgx in glob.glob(ext):
                if runAdams:
                    if os.path.splitext(hgx)[0] == fileName:
                        try:
                            os.remove(hgx)
                        except WindowsError:
                            continue
                        logger.info('Deleting file ' + str(hgx) + '\n')
                else:
                    if os.path.splitext(hgx)[0]:
                        try:
                            os.remove(hgx)
                        except WindowsError:
                            continue
                        logger.info('Deleting file ' + str(hgx) + '\n')
    else:
        # Move these files to Abaqus directory
        for ext in rm_types:
            for hgx in glob.glob(ext):
                try:
                    shutil.move(hgx,mainDir)
                except WindowsError:
                    continue
                logger.info('Moving file ' + str(hgx) + '\n')
        for ext in list_types:
            for hgx in glob.glob(ext):
                if runAdams:
                    if os.path.splitext(hgx)[0] == fileName:
                        try:
                            shutil.move(hgx,mainDir)
                        except WindowsError:
                            continue
                        logger.info('Moving file ' + str(hgx) + '\n')
                else:
                    if os.path.splitext(hgx)[0]:
                        try:
                            shutil.move(hgx,mainDir)
                        except WindowsError:
                            continue
                        logger.info('Moving file ' + str(hgx) + '\n')
    
    for ext in m_types:
        # These files are moved to Abaqus dir regardless
        for hgx in glob.glob(ext):
            if ext == '*.csv' and runAdams:
                if fileName in hgx or "OutMetrics" in hgx:
                    # Only move CSV file matching current key
                    shutil.move(hgx,mainDir)
                else:
                    continue
            else:
                shutil.move(hgx,mainDir)
            logger.info('Moving file ' + str(hgx) + '\n')
        if os.path.exists(os.path.join(os.getcwd(),'OutMetrics.xml')):
            shutil.move(os.path.join(os.getcwd(),'OutMetrics.xml'),mainDir)