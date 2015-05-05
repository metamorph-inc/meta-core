class Args(object):
    def __init__(self, **kwargs):
        self.__dict__.update(kwargs)

IFV_typelib = Args(lib_mga="..\\TypeLibrary\\TypeLibrary.mga", input_file="IFV.mga", outputfilename="IFV_updatedTypeLibrary", old_lib_name='TypeLibrary')
wps = "..\\WheeledPerformanceSurrogates\\WheeledVehiclePerformanceSurrogate_Library"
WPS_typelib = Args(lib_mga="..\\TypeLibrary\\TypeLibrary.mga", input_file=wps + ".mga", outputfilename=wps + "_updatedTypeLibrary", old_lib_name='TypeLibrary')
IFV_WPS = Args(lib_mga=wps + "_updatedTypeLibrary.mga", input_file="IFV_updatedTypeLibrary.mga", outputfilename="IFV_updatedWPS", old_lib_name='WheeledPerformanceSurrogates')

#updates = (IFV_typelib, WPS_typelib, IFV_WPS)
updates = (WPS_typelib,)

import win32com.client
# Disable early binding: full of race conditions writing the cache files,
# and changes the semantics since inheritance isn't handled correctly
import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None


def RefreshLibrary(args):
	project = win32com.client.DispatchEx("Mga.MgaProject")

	output = None
	#output = open("log", "w")
	project.Open("MGA=" + args.input_file)
	try:
	    project.BeginTransactionInNewTerr(2)
	    oldlib = [f for f in project.RootFolder.ChildFolders if f.LibraryName.find(args.old_lib_name) != -1][0]
	    newlib = project.RootFolder.AttachLibrary("MGA=" + args.lib_mga)
	    newlib.LibraryName = args.old_lib_name

	    switcher = win32com.client.DispatchEx("MGA.Interpreter.ReferenceSwitcher")
	    switcher.SwitchReferences([oldlib], [newlib])
	    # FlushUndoQueue, or oom
	    project.CommitTransaction()
	    project.FlushUndoQueue()
	    project.BeginTransactionInNewTerr(2)
	    if output:
	        import collections
	        q = collections.deque()
	        q.append(oldlib)
	        while len(q) != 0:
	            o = q.pop()
	            if o.ObjType == 6:
	                q.extend(o.ChildFolders)
	            if o.ObjType == 1 or o.ObjType == 6:
	                q.extend(o.ChildFCOs)
	            # print o.Name
	            if o.ObjType != 6:
	                for ref in o.ReferencedBy:
	                    output.write(ref.AbsPath)
	                    output.write("\n")
	    oldlib.DestroyObject()
	    project.CommitTransaction()
	    #project.Close(True)
	    project.Save("MGA=" + args.outputfilename + ".mga")
	    print "Saved " + args.outputfilename + ".mga"
	    project.FlushUndoQueue()
	    dumper = win32com.client.DispatchEx("Mga.MgaDumper")
	    dumper.DumpProject(project, args.outputfilename + ".xme")
	    print "Saved " + args.outputfilename + ".xme"
	finally:
	    project.Close(True)

for update in updates:
	RefreshLibrary(update)
