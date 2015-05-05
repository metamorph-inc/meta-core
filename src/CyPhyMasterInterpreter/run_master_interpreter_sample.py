import win32com.client

# Disable early binding: full of race conditions writing the cache files,
# and changes the semantics since inheritance isn't handled correctly
import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None

project = win32com.client.DispatchEx("Mga.MgaProject")
project.Open("MGA=" + r'D:\Projects\META\development\models\DynamicsTeam\MasterInterpreter\MasterInterpreter.mga')

# config_light = win32com.client.DispatchEx("CyPhyMasterInterpreter.ConfigurationSelectionLight")

# # GME id, or guid, or abs path or path to Test bench or SoT or PET
# config_light.ContextId = '{6d24a596-ec4f-4910-895b-d03a507878c3}'

# print config_light.SelectedConfigurationIds
# config_light.SetSelectedConfigurationIds(['id-0065-000000f1'])

# #config_light.KeepTemporaryModels = True
# #config_light.PostToJobManager = True

# master = win32com.client.DispatchEx("CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI")
# master.Initialize(project)
# results = master.RunInTransactionWithConfigLight(config_light)


# It works only this way and does not worth the time to figure out the other way.
# will run ALL configurations.
focusobj = None

try:
    project.BeginTransactionInNewTerr()
    focusobj = project.GetObjectByID('id-0065-00000635')
finally:
    project.AbortTransaction()
    
selectedobj=win32com.client.DispatchEx("Mga.MgaFCOs")

interpreter = "MGA.Interpreter.CyPhyMasterInterpreter"
launcher = win32com.client.DispatchEx("Mga.MgaLauncher")
launcher.RunComponent(interpreter, project, focusobj, selectedobj, 128)

project.Close()