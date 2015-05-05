#!/bin/python

from __future__ import with_statement

import re
import sys
import os.path
import win32com.client
import StringIO
import platform
import runpy
import subprocess
import itertools
import posixpath
import urllib
from urlparse import urlparse


# Disable early binding: full of race conditions writing the cache files,
# and changes the semantics since inheritance isn't handled correctly
import win32com.client.gencache
_savedGetClassForCLSID = win32com.client.gencache.GetClassForCLSID
win32com.client.gencache.GetClassForCLSID = lambda x: None

def get_last_modified(filename):
    local_file = urllib.urlopen(filename)
    last_modified = local_file.info()['Last-Modified']
    local_file.close()
    return last_modified

def urlretrieve(url, file=None):
    if file is None:
        file = posixpath.basename(urlparse(url).path)
    class NotModifiedException(Exception):
        pass
    class MyURLopener(urllib.FancyURLopener):
        def http_error_default(self, url, fp, errcode, errmsg, headers):
            void = fp.read()
            fp.close()
            if errcode == 304:
                raise NotModifiedException()
            else:
                raise Exception("Error downloading '" + url + "': " + str(errcode) + " " + errmsg)
    opener = MyURLopener()
    if os.path.isfile(file):
        opener.addheader('If-Modified-Since', get_last_modified(file))
    try:
        (file, msg) = opener.retrieve(url, file)
    except NotModifiedException:
        pass

# Elevation helpers
def execute_elevated(*args):
    # FIXME: support **kwargs
    from win32com.shell.shell import ShellExecuteEx
    from win32com.shell import shellcon
    import win32process, win32event
    import winxpgui
    import win32api
    import win32con
    try:
        hwnd = winxpgui.GetConsoleWindow()
    except winxpgui.error:
        hwnd = 0
    parameters = ""
    if not hasattr(sys, "frozen"):
        # Not running under py2exe exe
        parameters += "\"" + sys.argv[0] + "\" "
    parameters += " ".join(map(lambda x: "\"" + str(x) + "\"", args))
    print "Executing elevated with parameters " + parameters
    # TODO: capture output (maybe via named pipe)
    rc = ShellExecuteEx(hwnd=hwnd, fMask=shellcon.SEE_MASK_NOCLOSEPROCESS, lpVerb="runas", lpFile=sys.executable, 
        lpParameters=parameters, nShow=win32con.SW_SHOW)
    hproc = rc['hProcess']
    win32event.WaitForSingleObject(hproc, win32event.INFINITE)
    exit_code = win32process.GetExitCodeProcess(hproc)
    if exit_code:
        raise Exception("Error: subprocess failed (exit code %s)." % exit_code)

def is_elevated():
    import win32security
    import win32api
    hToken = win32security.OpenProcessToken(win32api.GetCurrentProcess(), win32security.TOKEN_QUERY)
    return win32security.GetTokenInformation(hToken, win32security.TokenElevation) != 0


def maybe_elevate():
    def decorator(func):
        if not hasattr(sys, "getwindowsversion") or sys.getwindowsversion()[0] < 6:
            wrap = func
        else:
            def wrap(*args, **kwargs):
                if not is_elevated():
                    execute_elevated(func.__name__, *args, **kwargs)
                else:
                    func(*args, **kwargs)
        setattr(sys.modules[__name__], func.__name__, wrap)
        return wrap
    return decorator

@maybe_elevate()
def elevated_check_call(*args):
    return subprocess.check_call(args)

# GME functions
def create_project(project, connection, paradigm):
    return project.Create(connection, paradigm)

# aka CreateMga.vbs
def xme2mga(xmefile, mgafile=None):
    # use abspath, since on GME x64-only, parser will be called out-of-proc (which won't share the same cwd)
    xmefile = os.path.abspath(xmefile)
    if not mgafile:
        mgafile = os.path.splitext(xmefile)[0] + ".mga"
    with Project.open(xmefile, mgafile) as project:
        project.save(project.mgafile)
        return project.mgafile

def run_interpreter(interpreter, file, focusobj=None, selectedobj=None, param=None, mga_to_save=None, save=True):
    if param is None:
        param = 128
    with Project.open(file, mga_to_save=mga_to_save) as project:
        project.run_interpreter(interpreter, focusobj, selectedobj, param)
        if not save:
            project.project.Close(True)
    del(project)
    import gc
    gc.collect()

def run_interpreter_with_focusobj(interpreter, file, focusobj=None, selectedobj=None, param=None, mga_to_save=None, save=True):
    if param is None:
        param = 128
    with Project.open(file, mga_to_save=mga_to_save) as project:
        if focusobj:
            focusobj = project.project.GetFCOByID(focusobj)
        project.run_interpreter(interpreter, focusobj, selectedobj, param)
        if not save:
            project.project.Close(True)

# MGAUTILLib.regaccessmode_enum.REGACCESS_BOTH = 3
def get_paradigm_file(paradigm, regaccess=3):
    "Returns the .mta file for a given registered paradigm"
    registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
    guid = registrar.GetParadigmGUIDStringDisp(regaccess, paradigm)
    import uuid
    if platform.system() != 'Java':
        buf = buffer(uuid.UUID(guid).bytes_le, 0, 16)
    else:
        buf = str(uuid.UUID(guid).bytes_le[0:16])
    (connstr, guid) = registrar.QueryParadigm(paradigm, None, buf, regaccess)
    # >>> constr
    # "MGA=....mta"
    return connstr[4:]


def _associate(progid, paradigm, regaccess):
    registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
    registrar.Associate(progid, paradigm, regaccess)

@maybe_elevate()
def _associate_elevated(*args):
    _associate(*args)

def associate(progid, paradigm, regaccess=1):
    "Associate a component with a paradigm"
    regaccess = int(regaccess)
    if regaccess != 1:
        _associate_elevated(progid, paradigm, regaccess)
    else:
        _associate(progid, paradigm, regaccess)


def is_registered(paradigm):
    registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
    paradigms = []
    # REGACCESS_USER = 1
    paradigms.extend(registrar.GetParadigmsDisp(1))
    # REGACCESS_SYSTEM = 2
    paradigms.extend(registrar.GetParadigmsDisp(2))
    return filter(lambda p: p == paradigm, paradigms)

REGISTER = 128
DONT_REGISTER = 0
def register_if_not_registered(file):
    "Register an xme or mga if it has not already been registered"
    if os.path.splitext(file)[1] == ".xmp":
        if not is_registered(os.path.basename(os.path.splitext(file)[0])) or (platform.system() != 'Java' and not os.path.isfile(get_paradigm_file(os.path.splitext(os.path.basename(file))[0]))):
            regxmp(file)
        return
    
    # if we don't give GME an absolute path, it registers the mta with a relative path (which is bad)
    with Project.open(os.path.abspath(file), mga_to_save=True) as project:
        # KMS FIXME: build systems need to run this before MetaInterpreter. a new build_customization is needed
        # project.run_interpreter("MGA.Interpreter.MetaGME2Uml", mgafile, None, None, 128)
        paradigm = project.project.RootFolder.Name
        if not is_registered(paradigm):
            project.run_interpreter("MGA.Interpreter.MetaInterpreter", param=REGISTER)
            print "Paradigm '%s' is now registered" % paradigm
        elif not os.path.isfile(get_paradigm_file(paradigm)):
            #FIXME: should we look for the .xmp?
            project.run_interpreter("MGA.Interpreter.MetaInterpreter", param=REGISTER)
            print "Paradigm '%s' had nonexistant .mta; it is now reregistered" % paradigm
        # TODO: can we check if it is up-to-date?
        # or os.path.getmtime(get_paradigm_file(paradigm)) < os.path.getmtime(file):
        else:
            print "Paradigm '%s' is already registered" % paradigm

def mga2xmp(mgafile, register=REGISTER):
    # if we don't give GME an absolute path, it registers the mta with a relative path (which is bad)
    run_interpreter("MGA.Interpreter.MetaInterpreter", os.path.abspath(mgafile), param=register, mga_to_save=True, save=False)

def xme2xmp(xmefile, register=REGISTER):
    mgafile = xme2mga(xmefile)
    mga2xmp(mgafile, register)
    return mgafile

def regmta(mtafile, regaccess=1):
    regaccess = int(regaccess)
    if regaccess != 1:
        _regxmp_elevated(mtafile, regaccess)
    else:
        _regxmp(mtafile, regaccess)

def _regxmp(xmpfile, regaccess):
    REG_USER = 1
    REG_SYSTEM = 2
    REG_BOTH = 3
    registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
    if os.path.splitext(xmpfile)[1].lower() == ".xmp":
        registrar.RegisterParadigmFromData("XML=" + os.path.abspath(xmpfile), "", regaccess)
    else:
        registrar.RegisterParadigmFromData("MGA=" + os.path.abspath(xmpfile), "", regaccess)

@maybe_elevate()
def _regxmp_elevated(xmpfile, regaccess):
    _regxmp(xmpfile, regaccess)

def regxmp(xmpfile, regaccess=1):
    regaccess = int(regaccess)
    if regaccess != 1:
        _regxmp_elevated(xmpfile, regaccess)
    else:
        _regxmp(xmpfile, regaccess)

def _reggmexmps(regaccess):
    regaccess = int(regaccess)
    for file in [ 'HFSM/HFSM.xmp', 'MetaGME/MetaGME.xmp', 'SF/SF.xmp', 'UML/UML.xmp' ]:
        regxmp(os.path.join(os.path.join(os.environ['GME_ROOT'], 'Paradigms'), file), regaccess)

@maybe_elevate()
def _reggmexmps_elevated(regaccess):
    _reggmexmps(regaccess)

def reggmexmps(regaccess=1):
    regaccess = int(regaccess)
    if regaccess != 1:
        _reggmexmps_elevated(regaccess)
    else:
        _reggmexmps(regaccess)

def mga2xme(mgafile, xmefile=None):
    if not xmefile:
        xmefile = os.path.splitext(mgafile)[0] + ".xme"

    with Project.open(mgafile) as project:
        project.save(xmefile)
    return xmefile

def register_component(file, warn_on_tlb_error=None):
    '''Register a GME component .dll'''
# TODO: on Vista or 7 we need to start an elevated registrar
    registrar = win32com.client.DispatchEx("Mga.MgaRegistrar")
    # REGACCESS_BOTH    = 3,
    registrar.RegisterComponentLibrary(file, 3)


# UDM functions
def meta2uml(mgafile, umlfile=None):
    if not os.path.isfile(mgafile):
        raise Exception("'" + mgafile + "' not found")
    
    # n.b. this uses the SxS config in gmepy-setup.py under gmepy.exe (but not gme.py)
    with Project.open(mgafile) as project:
        project.run_interpreter("MGA.Interpreter.MetaGME2Uml", None, None, 128)
        output_path = os.path.join(os.path.dirname(mgafile), project.project.RootFolder.Name + "_uml.mga")
        # project.project.Save("MGA=" + os.path.splitext(mgafile)[0] + "_after_MetaGME2Uml.mga")
        project.project.Close(True)
    if umlfile and os.path.normcase(os.path.abspath(umlfile)) != os.path.normcase(os.path.abspath(output_path)):
        import shutil
        try:
            os.remove(umlfile)
        except OSError:
            pass
        shutil.move(output_path, umlfile)
    #subprocess.check_call(["MetaGME2UML.exe", mgafile, umlfile])

# aka CreateUdmXml.vbs
def mga2udmxml(mgafile):
    run_interpreter("MGA.Interpreter.UML2XML", mgafile, param=128, save=False)

# GReAT functions
def RunGreatMasterInt(file):
    file = os.path.abspath(file)
    mtime = os.stat(file).st_mtime
    # The interpreter updates the GReAT config path, so we need to save the .mga
    run_interpreter("MGA.Interpreter.GReAT Master Interpreter", file, param=128, save=True)
    # Let's lie and modify the timestamp so incremental build behaves better
    os.utime(file, (mtime, mtime))

# Explorer context menu
def context_menu_reg():
    """Register explorer context menu options"""
    import _winreg
    if hasattr(sys, "frozen"):
        # Running under py2exe exe
        gmepydir = os.path.dirname(unicode(sys.executable, sys.getfilesystemencoding( )))
    else:
        gmepydir = os.path.dirname(__file__)
    # Windows won't let us start gme.py from the context menu, so use the exe
    gmepy = gmepydir + "\\gmepy.exe"

    mga = "mga"
    xme = "xme"
    xmp = "xmp"
    menus = [ (mga, "mga2xme"),
            (mga, "mga2xmp"),
            (mga, "mga2udmxml"),
            (xme, "xme2mga"),
            (xmp, "regxmp"),
            ]
    regname = gmepydir + "\\gmepy_context_menu.reg"
    with open(regname, "w") as reg:
        reg.write("Windows Registry Editor Version 5.00\n")
        for p in menus:
            try:
                key = _winreg.OpenKey(_winreg.ConnectRegistry(None, _winreg.HKEY_CLASSES_ROOT), "."+p[0])
                n,v,t = _winreg.EnumValue(key, 0)
                ext = v
                _winreg.CloseKey(key)
            except WindowsError:
                ext = "."+p[0]
            str = """[HKEY_CLASSES_ROOT\{ext}\shell]

[HKEY_CLASSES_ROOT\{ext}\shell\{name}]

[HKEY_CLASSES_ROOT\{ext}\shell\{name}\command]
@="\\"{gmepy}\\" {name} \\\"%1\\\""

""".format(ext=ext, name=p[1], gmepy=gmepy.replace("\\", "\\\\"))
            reg.write(str)
    elevated_check_call("regedit", regname)

OBJTYPE_MODEL = 1
OBJTYPE_ATOM = 2
OBJTYPE_REFERENCE = 3
OBJTYPE_CONNECTION = 4
OBJTYPE_SET = 5
OBJTYPE_FOLDER = 6
def is_container(fco):
    return fco.ObjType == OBJTYPE_MODEL or fco.ObjType == OBJTYPE_FOLDER

import tempfile
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
        def get_children(mga_object):
            import itertools
            children = mga_object.ChildFCOs
            if current.ObjType == OBJTYPE_FOLDER:
                children = itertools.chain(children, current.ChildFolders)
            return children
            
        path_a = path.split("/")
        current = self.project.RootFolder
        for name in path_a[0:-1]:
            containers = filter(is_container, get_children(current))
            matches = list(filter(lambda x: x.Name == name, containers))
            if matches:
                current = matches[0]
            else:
                raise Exception("Cant find %s in path %s" % (name, path))
        
        matches = [child for child in get_children(current) if child.Name == path_a[-1]]
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


def print_paradigm(xmefile):
    "Print the input file and paradigm of a given xme"
    xme = win32com.client.DispatchEx("Mga.MgaParser")
    (paradigm, parversion, parguid, basename, ver) = xme.GetXMLInfo(xmefile)
    print xmefile
    print paradigm

def run_module(name, *args):
    sys.path.append('.')
    sys.argv[1:] = args
    runpy.run_module(name, run_name='__main__')

def usage():
    gme_dict = sys.modules[__name__].__dict__
    names = []
    names.extend(gme_dict.keys())
    for name in filter(lambda name: type(gme_dict[name]) == type(print_paradigm), names):
        if gme_dict[name].__doc__ and name.find('_') != 0:
            print name
            print "\t" + gme_dict[name].__doc__
    sys.exit(2)

import traceback
if __name__ == '__main__':
    try:
        if len(sys.argv) < 2 or sys.argv[1] not in dir():
            usage()
        else:
            # TRY:
            # sys.modules[__name__].__getattribute__(sys.argv[1]).__call__(*sys.argv[2:])
            eval("%s(*sys.argv[2:])" % sys.argv[1])
    except:
        traceback.print_exc(file=sys.stderr)
        sys.stdin.readline()
        sys.exit(1)
