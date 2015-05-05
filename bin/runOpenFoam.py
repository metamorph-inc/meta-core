import sys
import os
import _winreg as winreg
with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
    meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
sys.path.append(os.path.join(meta_path, 'bin'))
import udm
import shutil

def log(s):
    print s
try:
    import CyPhyPython # will fail if not running under CyPhyPython
    import cgi
    def log(s):
        CyPhyPython.log(cgi.escape(s))
except ImportError:
    pass

def log_formatted(s):
    print s
try:
    import CyPhyPython # will fail if not running under CyPhyPython
    import cgi
    def log(s):
        CyPhyPython.log(s)
except ImportError:
    pass
	
	
def start_pdb():
    ''' Starts pdb, the Python debugger, in a console window
    '''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    import sys
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb; pdb.set_trace()


# This is the entry point    
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    output_dir = componentParameters['output_dir']
    if output_dir == '':
        output_dir = os.getcwd()
    
    #shutil.copy(os.path.join(meta_path, 'bin', './run_openfoam.sh'), output_dir)
    # n.b. JobManager doesn't run zip.py if source_data.zip already exists (which is does, since our output_dir is the same as CyPhy2CAD's)
    for filename in ('source_data.zip', 'results.zip', 'zip.py', 'consoleText0.txt'):
        filename = os.path.join(componentParameters['output_dir'], filename)
        if os.path.exists(filename):
            os.unlink(filename)
    filename = os.path.join(componentParameters['output_dir'], 'consoleText.txt')
    if os.path.exists(filename):
        os.rename(filename, os.path.join(componentParameters['output_dir'], 'consoleText0.txt'))
    componentParameters['runCommand'] = 'bash runOpenFoam.sh'
    componentParameters['labels'] = 'OpenFoam'
    # testing on windows:
    #componentParameters['labels'] = 'Windows'
    #componentParameters['runCommand'] = 'run_classifications.cmd'
