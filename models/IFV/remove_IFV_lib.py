
lib_xme = "CompLib_DesignSpace_andDesigns.xme"
input = r"..\..\generated\IFV\models\IFV.mga"
output = "IFV_no_lib"

import win32com.client

MGAPREF_NO_NESTED_TX = 0x00000080

project = win32com.client.DispatchEx("Mga.MgaProject")

project.Open("MGA=" + input)
try:
    # BUG: GME overwrites the RootFolder name on ParserProject
    project.BeginTransactionInNewTerr()
    rf_name = project.RootFolder.Name
    project.Preferences = MGAPREF_NO_NESTED_TX | project.Preferences
    project.CommitTransaction()

    parser = win32com.client.DispatchEx("Mga.MgaParser")
    parser.ParseProject(project, lib_xme)
    
    project.BeginTransactionInNewTerr()
    libroot = [f for f in project.RootFolder.ChildFolders if f.LibraryName.find('CompLib') != -1][0]
    libfolders = sorted(libroot.ChildFolders, key=lambda x: x.Name)
    folders = []
    for libfolder in libfolders:
        fs = [f for f in project.RootFolder.ChildFolders if f.Name == libfolder.Name]
        if len(fs) != 1:
            raise Exception("No equivalent for lib folder " + libfolder.Name)
        folders.append(fs[0])
    print 'Updating ' + ", ".join([f.Name for f in folders])
    switcher = win32com.client.DispatchEx("MGA.Interpreter.ReferenceSwitcher")
    switcher.SwitchReferences(libfolders, folders)
    project.RootFolder.Name = rf_name
    project.Name = rf_name
    # FlushUndoQueue, or oom
    project.CommitTransaction()
    project.FlushUndoQueue()
    project.BeginTransactionInNewTerr()
    libroot.DestroyObject()
    project.CommitTransaction()
    project.Save("MGA=" + output + ".mga")
    project.FlushUndoQueue()
    dumper = win32com.client.DispatchEx("Mga.MgaDumper")
    dumper.DumpProject(project, output + ".xme")
finally:
    project.Close(True)

