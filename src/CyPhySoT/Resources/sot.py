#!/bin/python

manifest_filename = 'manifest.sot.json'


import sys
import win32com
import json
import os

# BEGIN:  code is from gme.py

# Disable early binding: full of race conditions writing the cache files,
# and changes the semantics since inheritance isn't handled correctly
import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None

def create_project(project, connection, paradigm):
    return project.Create(connection, paradigm)

class Project():
    def __init__(self, com_project):
        self.project = com_project

    def __enter__(self):
        self.begin_transaction()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if self.project.ProjectStatus == 3 or self.project.ProjectStatus == 4:
            if exc_type:
                self.project.AbortTransaction()
            else:
                self.project.CommitTransaction()
            if self.territory:
                self.territory.Destroy()
                self.territory = None
        if self.project.ProjectStatus != 0:
            if exc_type:
                self.project.Close(True)
            else:
                self.project.Close()

    def get_fco(self, path):
        path_a = path.split("/")
        current = self.project.RootFolder
        for name in path_a[0:-1]:
            containers = list(filter(is_container, current.ChildFCOs))
            if current.ObjType == OBJTYPE_FOLDER:
                containers += list(filter(is_container, current.ChildFolders))
            matches = filter(lambda x: x.Name == name, containers)
            if matches:
                current = matches[0]
            else:
                raise Exception("Cant find %s in path %s" % (name, path))
        matches = array(filter(lambda x: x.Name == path_a[-1], current.ChildFCOs))
        if current.ObjType == OBJTYPE_FOLDER:
            matches += array(filter(lambda x: x.Name == path_a[-1], current.ChildFolders))
        if matches:
            return matches[0]
        else:
            raise Exception("Cant find %s in path %s" % (path_a[-1], path))

    def save(self, filename=None):
        if not filename:
            filename = self.file
        filename = os.path.abspath(filename)
        self.project.CommitTransaction()
        if self.territory:
            self.territory.Destroy()
            self.territory = None

        extension = os.path.splitext(filename)[1]
        if extension == ".mga":
            self.project.Save("MGA=" + filename)
        elif extension == ".xme":
            dumper = win32com.client.DispatchEx("Mga.MgaDumper")
            dumper.DumpProject(self.project, filename)
        else:
            raise Exception("Don't know how to save '%s'" % filename)
        self.territory = self.project.BeginTransactionInNewTerr()
    
    def begin_transaction(self):
        self.territory = self.project.BeginTransactionInNewTerr()

    def commit_transaction(self):
        self.project.CommitTransaction()
        if self.territory:
            self.territory.Destroy()
            self.territory = None
    
    def abort_transaction(self):
        self.project.AbortTransaction()
        if self.territory:
            self.territory.Destroy()
            self.territory = None
    
    def run_interpreter(self, interpreter, focusobj=None, selectedobj=None, param=0):
        if not selectedobj:
            selectedobj=win32com.client.DispatchEx("Mga.MgaFCOs")
        self.commit_transaction()
        try:
            launcher = win32com.client.DispatchEx("Mga.MgaLauncher")
            launcher.RunComponent(interpreter, self.project, focusobj, selectedobj, param)
        finally:
            self.begin_transaction()

    @staticmethod
    def create(mgafile, paradigm):
        project = win32com.client.DispatchEx("Mga.MgaProject")
        create_project(project, "MGA=" + mgafile, paradigm)
        p = Project(project)
        p.filename = mgafile
        p.mgafile = mgafile
        return p

    @staticmethod
    def open(file, mga_to_save=None):
        if not os.path.isfile(file):
            raise Exception("'" + file + "' not found")
        extension = os.path.splitext(file)[1]
        mga = None
        if extension == ".mga":
            mga = win32com.client.DispatchEx("Mga.MgaProject")
            mga.Open("MGA=" + file)
            mga_to_save = file
        elif extension == ".xme":
            xme = win32com.client.DispatchEx("Mga.MgaParser")
            (paradigm, parversion, parguid, basename, ver) = xme.GetXMLInfo(file)

            mga = win32com.client.DispatchEx("Mga.MgaProject")
            xme = win32com.client.DispatchEx("Mga.MgaParser")
            resolver = win32com.client.DispatchEx("Mga.MgaResolver")
            try:
                resolver.IsInteractive = False
                xme.Resolver = resolver
            except AttributeError:
                # older GME
                pass
            if mga_to_save == True:
                mga_to_save = os.path.splitext(file)[0] + ".mga"
            elif not mga_to_save:
                mga_to_save = tempfile.gettempdir() + "gmepy-%s.mga" % os.getpid()
            create_project(mga, "MGA=" + mga_to_save, paradigm)
            try:
                xme.ParseProject(mga, file)
            except:
                mga.Close(True)
                raise
        else:
            raise Exception("Don't know how to open '%s'" % file)
        p = Project(mga)
        p.filename = file
        p.mgafile = mga_to_save
        return p

# END: this code is from gme.py


def run_sot(sot):
    '''Sot has a project name and a id for the SoT object in the project'''
    #print sot
    
    project_filename = None
    
    if 'ProjectFileName' in sot:
        project_filename = os.path.abspath(sot['ProjectFileName'])
    else:
        'Project filename was not found'
        return 1
    
    print project_filename
    
    sot_id = None
    
    if 'SoTID' in sot:
        sot_id = sot['SoTID']
    else:
        'SoTID was not found'
        return 1
    
    print 'Current object: {0}'.format(sot_id)
    
    project = Project.open(project_filename)
    current_fco = None
    project.begin_transaction()
    try:
        current_fco = project.project.GetFCOByID(str(sot_id))
        print current_fco.AbsPath
    except:
        print 'Object was not found??'
    
    project.abort_transaction()
    
    # creating CyPhySOT interpreter
    sotInterpreter = win32com.client.DispatchEx("Mga.Interpreter.CyPhySOT")
    
    # initializing CyPhySOT
    sotInterpreter.Initialize(project.project)
    
    # TODO: pass parameters...
    # TODO: redirect GME console???
    # TODO: setup output base directory...
    sotInterpreter.set_ComponentParameter('output_dir', os.getcwd())
    sotInterpreter.set_ComponentParameter('do_config', 'false')
    sotInterpreter.set_ComponentParameter('run_silent', 'true')
    
    print 'Calling CyPhySOT interpreter ...'
    # calling CyPhySOT interpreter
    selectedobj = win32com.client.DispatchEx("Mga.MgaFCOs")
    sotInterpreter.InvokeEx(project.project, current_fco, selectedobj, 0)
    
    # TODO: Link generated summary.report.json files to dashboard.
    
    print 'Done.'
    return 0
    
def main():
    # TODO: change current working  directory to __file__'s directory
    # restore the working dir before return
    
    print 'Running sot... '
    
    if not os.path.exists(manifest_filename):
        print 'Sot manifest file does not exist.'
        safe_return(1)
        return 1
    
    sot = None
    
    with open(manifest_filename, 'r') as fp:
        sot = json.load(fp)
    
    return_code = 0
    
    if sot:
        return_code = run_sot(sot)
    
    return return_code

if __name__ == '__main__':
    sys.exit(main())