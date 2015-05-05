import unittest
import os
import glob
import win32api
import win32con
import win32com.client
import pythoncom
import json
import ctypes
import comtypes.client
import comtypes.automation



def get_ctypes_dispatch_from_win32com(disp):
	# http://mail.python.org/pipermail/python-win32/2008-April/007302.html
	import win32com.client.dynamic
	import ctypes
	import comtypes
	disp = win32com.client.dynamic.DumbDispatch(disp)
	x = disp._oleobj_
	addr = int(repr(x).split()[-1][2:-1], 16)
	#print hex(addr)
	
	pnt = ctypes.POINTER(comtypes.automation.IDispatch)()
	ctypes.cast(ctypes.byref(pnt), ctypes.POINTER(ctypes.c_void_p))[0] = addr
	pnt.AddRef()
	return pnt

class TestMETA(unittest.TestCase):
    """Class for testing various interpreters on different models."""
    def _test_run_interpreters(self, test_case):
        try:
            # import model
            # ASSUMPTION: MUST BE XME
            project_com = self._test_import_project(test_case['model_path'])
                        
            project = get_ctypes_dispatch_from_win32com(project_com)
            
            # create interpreter
            interpreter = self._test_create_interpreter(test_case['progid'])
            # TODO: check if progid is really an interpreter
                       
            if interpreter:
            
                # run interpreter
                interpreter.Initialize(project)
                interpreter.InteractiveMode = False
                #print interpreter.InteractiveMode
                #print interpreter.ComponentName
                #print interpreter.ComponentType
                #print interpreter.Paradigm
                #print interpreter.ComponentProgID
                #print interpreter.get_ComponentParameter('automation')
                #print dir(interpreter)
                #print dir(interpreter.ComponentParameter)
                automation = interpreter.ComponentParameter['automation']
                if automation == 'false':
                    interpreter.ComponentParameter['automation'] = 'true'
                    #print interpreter.ComponentParameter['automation']
                    fcos = comtypes.client.CreateObject("Mga.MgaFCOs")
                    #fcos = win32com.client.DispatchEx("Mga.MgaFCOs")
                    interpreter.InvokeEx(project.QueryInterface(comtypes.gen.MGALib.IMgaProject), None, fcos, 128)
                    # TODO: run post processing
                    # TODO: save results
                    
                else:
                    self.fail('{0} does not support automation'.format(test_case['progid']))

            else:
                self.skipTest('Invalid interpreter {0}'.format(test_case['progid']))
        except pythoncom.com_error as err:
            self.fail('{0} failed to run. {1}'.format(test_case, err))

    def _test_create_interpreter(self, progid):
        interpreter = None
        try:
            o = pythoncom.CoCreateInstance(
                                            progid,
                                            None,
                                            pythoncom.CLSCTX_ALL,
                                            pythoncom.IID_IUnknown)

            
            registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
            type = None
            desc = None
            (type, desc) = registrar.QueryComponent(progid, type, desc, 3) # 3 = regaccess.BOTH            
            #print '{0} {1}'.format(progid, type)
            #print desc
            
            if not o and not type:
                self.skipTest('{0} is not registered.'.format(progid))
            
            if type == 1: # 1 = interpreter         
                import _ctypes
                # FIXME: possible race with multiple gme.pys
                # need to delete the cache since we don't properly version the typelibs, e.g. GMESRC r947
                import comtypes.client._code_cache
                for root, dirs, files in os.walk(comtypes.client._code_cache._find_gen_dir()):
                                for f in files:
                                                os.unlink(os.path.join(root, f))
                comtypes.client.GetModule(['{270B4F86-B17C-11D3-9AD1-00AA00B6FE26}', 1, 0]) # MGALib
                comtypes.client.GetModule(["{461F30AE-3BF0-11D4-B3F0-005004D38590}", 1, 0]) # MgaUtilLib/MgaUTILLib
                #TODO: check for GME version compatibility
                #TODO: CreateMgaComponent supports IDispatch wrapper ComponentProxy. do the same here
                # GME VS2010 moved IMgaComponentEx from MGAUTILLib(now MGAUtilLib) to MGALib
                if hasattr(comtypes.gen, "MGAUTILLib") and hasattr(comtypes.gen.MGAUTILLib, "IMgaComponentEx"):
                                _IMgaComponentEx = comtypes.gen.MGAUTILLib.IMgaComponentEx
                else:
                                _IMgaComponentEx = comtypes.gen.MGALib.IMgaComponentEx
                interpreter = comtypes.client.CreateObject(progid, interface=_IMgaComponentEx)
                
                #interpreter = win32com.client.Dispatch(progid)
                
                del(o)
                
        except pythoncom.com_error as err:
            self.fail('{0} cannot be created. {1}'.format(progid, err))
        
        return interpreter
     

    def _test_import_project(self, xme_filename):
        if not os.path.exists(xme_filename):
            self.fail('{0} does not exist.'.format(xme_filename))
        try:
            xme = win32com.client.DispatchEx("Mga.MgaParser")
            (paradigm, parversion, parguid, basename, ver) = xme.GetXMLInfo(xme_filename)
            #print paradigm
            #print parversion
            mga = win32com.client.DispatchEx("Mga.MgaProject")
            resolver = win32com.client.DispatchEx("Mga.MgaResolver")
            resolver.IsInteractive = False
            mga.Create("MGA=" + os.path.join(xme_filename + "_test_.mga"), paradigm)
            xme.Resolver = resolver
            #import pdb; pdb.set_trace()
            xme.ParseProject(mga, xme_filename)

            
            mga.Save()
            mga.Close()
        except pythoncom.com_error as err:
            self.fail('{0} is not compatible with the meta model. {1}'.format(xme_filename, err))
        del(xme)
        return mga
        
def main():
    pass
# determine user/developer machine
# get the directory for the models
user_machine = False
meta_doc_path = os.path.join(os.environ["PUBLIC"], "Documents", "META Documents")

model_dir = ''

if os.path.exists(meta_doc_path):
    print 'User machine'
    model_dir = meta_doc_path
    user_machine = True
else:
    print 'Developer machine'
    hKey = win32api.RegOpenKey (win32con.HKEY_LOCAL_MACHINE, r"SOFTWARE\META")
    value, type = win32api.RegQueryValueEx (hKey, "META_PATH")
    model_dir = os.path.join(value, 'models')
    if not os.path.exists(model_dir):
        raise Exception('Model directory cannot be located')

# model dir exists

# TODO: read input file and configuration
test_cases = []

config_file = os.path.abspath('InterpreterTestCasesConfig.json')
print config_file
if os.path.exists(config_file):
    if os.path.isfile(config_file):
        with open(config_file, 'r') as f_p:
            test_cases = json.load(f_p)

        #print test_cases        
    else:
        print 'config file is not a file'
else:
    print 'config file does not exist'



# test_cases = [
    # {
        # "progid":"MGA.Interpreter.CyPhyMasterInterpreter",
        # "name": "MasetTestOnMSD1",
        # "modelPath": "model",
    # },
    # {
        # "progid":"MGA.Interpreter.CyPhyDynamics",
        # "name": "MasetTestOnMSD2",
        # "modelPath": "model",
    # }
# ]

_progids = (
#  grep \<ProgId  ../deploy/*wxi ../deploy/*wxs | perlsed 's/.*Id=.([^'"'"'"]*).*?$/"$1",/'
"MGA.Interpreter.CAD2CyPhy",
"MGA.AddOn.CyPhyAddOn",
"MGA.Interpreter.CyPhyConnectionUtility",
"MGA.Addon.CyPhyDecoratorAddon",
"MGA.Interpreter.CyPhyFormulaEvaluator",
"MGA.Interpreter.CyPhyDynamics",
"MGA.Addon.CyPhyMdaoAddOn",
"MGA.Interpreter.CyPhyMetricEvaluator",
"MGA.Interpreter.CyPhyMasterInterpreter",
"MGA.Interpreter.CyphyDSEConverter",
"MGA.Interpreter.DesignSpaceHelper",
"MGA.Interpreter.ReferenceSwitcher",
"MGA.Interpreter.Run_ESMoL_toolchain",
#"MGA.Decorator.Workflow",
#"MGA.CPMDecorator",
)

# TODO: get all models

# TODO: try to import all models



# generate methods for each test case based on the input definition
for progid in _progids:
    setattr(
        TestMETA,
        'test_create_' + progid.replace(".", "_"),
        lambda self, progid=progid: self._test_create_interpreter(progid))

for test_case in test_cases:
    setattr(
        TestMETA,
        'test_run_' + test_case['name'],
        lambda self, test_case=test_case: self._test_run_interpreters(test_case))

# test project import functionality
print model_dir
i = 0
xme_files = []
for r,d,f in os.walk(model_dir):
    for files in f:
        if files.endswith(".xme"):
            xme_files.append(os.path.join(r,files))

print xme_files
for xme_file in xme_files:
    i += 1
    # setattr(
            # TestMETA,
            # 'test_import_{0}_{1}'.format(i, os.path.basename(xme_file).replace('.','_')),
            # lambda self, xme_file=xme_file: self._test_import_project(xme_file))
                     
# setattr(
       # TestMETA,
       # 'test_import_' + 'MassSpringDamper',
       # lambda self: self._test_import_project(os.path.join(model_dir, 'MassSpringDamper', 'Mass_Spring_Damper_Design.xme')))


if __name__=='__main__':  
    main()
    # run unit tests
    unittest.main()

