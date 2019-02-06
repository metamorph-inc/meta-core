import sys
import os
import cgi
import os.path
import pythoncom
from pywintypes import com_error


def log_formatted(s):
    print s


def log(s):
    print s
try:
    import CyPhyPython  # will fail if not running under CyPhyPython

    def log(s):
        CyPhyPython.log(cgi.escape(s))

    def log_formatted(s):
        CyPhyPython.log(s)

except ImportError:
    pass


def start_pdb():
    '''Start pdb, the Python debugger, in a console window.'''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


# This is the entry point
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    log(focusObject.name + "\n")


def main():
    from win32com.client import Dispatch
    pythoncom.CoInitialize()
    project = Dispatch("Mga.MgaProject")
    project.Create("MGA=" + "tmp.mga", "CyPhyML")
    try:
        gmeInterpreter = Dispatch("MGA.Interpreter.CyPhyPython")
        project.BeginTransactionInNewTerr()
        components_meta = project.RootMeta.RootFolder.GetDefinedFolderByNameDisp("Components", True)
        component_meta = project.RootMeta.RootFolder.GetDefinedFCOByNameDisp("Component", True)
        components = project.RootFolder.CreateFolder(components_meta)
        component = components.CreateRootObject(component_meta)
        component.Name = component_meta.Name
        project.CommitTransaction()
        invoke_id = gmeInterpreter._oleobj_.GetIDsOfNames(u'ComponentParameter')
        gmeInterpreter._oleobj_.Invoke(invoke_id, 0, 4, 0, 'script_file', os.path.basename(__file__))
        gmeInterpreter.InvokeEx(project, component, Dispatch("Mga.MgaFCOs"), 128)
    finally:
        project.Close()

if __name__ == '__main__':
    main()
    import threading
    t = threading.Thread(target=main)
    t.start()
    t.join()
    t = threading.Thread(target=main)
    t.start()
    t.join()
