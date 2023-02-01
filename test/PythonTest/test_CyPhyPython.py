from __future__ import absolute_import
from __future__ import print_function
import sys
import os
import html
import os.path
import pythoncom
from pywintypes import com_error

#import site
#print(site.__file__)

# remove src/ from sys.path since we don't want src/CyPhyPython/ imported
_this_dir = os.path.dirname(os.path.abspath(__file__))
_test_dir = os.path.dirname(_this_dir)
_src_dir = os.path.abspath(os.path.join(_test_dir, '..', 'src'))
if _src_dir in sys.path:
    sys.path.remove(_src_dir)
print(sys.path)

def log_formatted(s):
    print(s)


def log(s):
    print(s)
try:
    import CyPhyPython  # will fail if not running under CyPhyPython

    def log(s):
        CyPhyPython.log(html.escape(s))

    def log_formatted(s):
        CyPhyPython.log(s)

except ImportError:
    pass


def start_pdb():
    '''Start pdb, the Python debugger, in a console window.'''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    sys.stdout = open('CONOUT$', 'wt')
    sys.stderr = sys.stdout
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


# This is the entry point
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    import CyPhyPET_unit_matcher
    import socket
    log(CyPhyPET_unit_matcher.__file__)
    log(focusObject.name + " from test_CyPhyPython\n")
    import __main__


    #setattr(__main__, 'asdf', focusObject.convert_udm2gme())
    #setattr(__main__, 'asdf2', focusObject)


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
        import ctypes
        # ctypes.windll.kernel32.DebugBreak()

        gmeInterpreter.InvokeEx(project, component, Dispatch("Mga.MgaFCOs"), 128)
        '''
        import gc
        gc.get_referents(gmeInterpreter)
        '''
        x = [gmeInterpreter]
        #del gmeInterpreter
        #import udm
        #path = udm.findPathToNearestRoot(x)
        #import pdb; pdb.set_trace()
    finally:
        project.Close()
        del project

if __name__ == '__main__':
    main()
    import threading
    t = threading.Thread(target=main)
    t.start()
    t.join()
    t = threading.Thread(target=main)
    t.start()
    t.join()
    print('ok')
    # del t

    # del sys.modules['test_CyPhyPython']
    # import gc
    # import CyPhyPython
    # import pprint
    # pprint.pprint(repr(gc.get_referrers(CyPhyPython)))
    # for i, ref_ in enumerate(gc.get_referrers(CyPhyPython)):
    #    if isinstance(ref_, dict):
    #        print(f' {i}  {ref_.get("__name__")}')
    # asdf2.convert_udm2gme()
r'''
import sys
sys.path.append(r'C:\Users\kevin\Documents\meta-core\test\PythonTest')
import test_CyPhyPython

test_CyPhyPython.main()

del test_CyPhyPython
del sys.modules['test_CyPhyPython']
del sys.modules['CyPhyPython']
import gc
gc.collect()

'''
