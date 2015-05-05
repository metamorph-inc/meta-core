import os
import sys
import shutil
import zipfile
import logging
import subprocess
import win32com.client

## Setup a logger
# Create logger with 'spam_application'.
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

# Create file handler which logs even debug messages.
if not os.path.isdir('log'):
    os.mkdir('log')

fh = logging.FileHandler(os.path.join('log', 'export-to-webgme.log'))
fh.setLevel(logging.DEBUG)

# Create console handler with a higher log level.
ch = logging.StreamHandler()
ch.setLevel(logging.INFO)

# Create formatter and add it to the handlers.
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)

# Add the handlers to the logger.
logger.addHandler(fh)
logger.addHandler(ch)


## Obtain the root directory for the META-tools.
# Get the running meta-python path.
sys_pieces = sys.executable.split(os.path.sep)
# Drop the 'bin/Python27/Scripts/python.exe' part.
if len(sys_pieces) < 4:
    logger.error('Python script must be called using the META-python virtual env!')
    sys.exit(1)
sys_pieces = sys_pieces[:-4]
# Make sure to get the slashes after e.g. "C:".
if sys_pieces[0].endswith(':'):
    sys_pieces[0] = sys_pieces[0] + os.path.sep
# Join the pieces.
META_DIR = os.path.join(*sys_pieces)

# Disable early binding: full of race conditions writing the cache files,
# and changes the semantics since inheritance isn't handled correctly
import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None


def parse_xme_and_save_to_mga(file_name='empty.xme'):
    """
    Imports the xme project and saves it to a mga-file with the same name.
    (Will overwrite any existing mga with same path.)

    returns : mga_path : path to newly created mga
    """
    mga_file = file_name[:-3] + 'mga'
    mga_path = os.path.abspath(mga_file)
    logger.debug("About to parse .xme, mga will be saved to " + mga_path)
    parser = win32com.client.Dispatch('Mga.MgaParser')
    (paradigm, paradigm_v, paradigm_guid, basename, version) = parser.GetXMLInfo(file_name)
    logger.debug('Xme info :')
    logger.debug('  paradigm     : {0}'.format(paradigm))
    logger.debug('  paradigm_v   : {0}'.format(paradigm_v))
    logger.debug('  paradigm_guid: {0}'.format(paradigm_guid))
    logger.debug('  basename     : {0}'.format(basename))
    logger.debug('  version      : {0}'.format(version))
    if paradigm != 'CyPhyML':
        raise IOError("Given xme file must be using CyPhyML as paradigm, not {0}".format(paradigm))

    project = win32com.client.Dispatch('Mga.MgaProject')
    project.Create('MGA={0}'.format(mga_path), paradigm)
    try:
        parser.ParseProject(project, file_name)
        project.Save()
        logging.debug("Mga saved to " + mga_file)
    finally:
        project.Close(True)

    return mga_path


def export_components(mga_path, output_dir='acms'):
    if os.path.isdir(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)
    project_conn_str = 'MGA={0}'.format(mga_path)
    project = win32com.client.Dispatch('Mga.MgaProject')
    project.Open(project_conn_str)
    interpreter = win32com.client.Dispatch('MGA.Interpreter.CyPhyComponentExporter')
    interpreter.Initialize(project)
    interpreter.OutputDir = output_dir
    interpreter.InvokeEx(project, None, None, 128)
    project.Close(False)


def export_designs(mga_path, output_dir='adms'):
    if os.path.isdir(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)
    project_conn_str = 'MGA={0}'.format(mga_path)
    project = win32com.client.Dispatch('Mga.MgaProject')
    project.Open(project_conn_str)
    interpreter = win32com.client.Dispatch('MGA.Interpreter.CyPhyDesignExporter')
    interpreter.Initialize(project)
    interpreter.OutputDir = output_dir
    interpreter.InvokeEx(project, None, None, 128)
    project.Close(False)


def main(xme_file_name, acm_folder='acms', adm_folder='adms'):
    mga_file = parse_xme_and_save_to_mga(xme_file_name)
    logger.info('(1) Mga created...')
    export_components(mga_file, acm_folder)
    logger.info('(2) Components exported...')
    export_designs(mga_file, adm_folder)
    logger.info('(3) Designs exported...')

if __name__ == '__main__':
    try:
        xme_file_name = [f for f in os.listdir('.') if f.endswith('.xme')][0]
    except IndexError:
        logger.error('Could not find an xme file at {0}'.format(os.getcwd()))
        sys.exit(1)
    main(xme_file_name)
