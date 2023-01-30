# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# __init__.py                Created on: 1999/07/19
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2002-2019 Apasphere Ltd
#    Copyright (C) 1999 AT&T Laboratories Cambridge
#
#    This file is part of the omniORBpy library
#
#    The omniORBpy library is free software; you can redistribute it
#    and/or modify it under the terms of the GNU Lesser General
#    Public License as published by the Free Software Foundation;
#    either version 2.1 of the License, or (at your option) any later
#    version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library. If not, see http://www.gnu.org/licenses/
#
#
# Description:
#    omniORB module -- omniORB specific things

"""
omniORB module -- omniORB specific features
"""

import sys, types, imp, os, os.path, tempfile

try:
    import threading
except ImportError:
    sys.stderr.write("""
Error: your Python executable was not built with thread support.
       omniORBpy requires threads. Sorry.
""")
    raise ImportError("Python executable has no thread support")

import _omnipy

_coreVersion = _omnipy.coreVersion()
__version__  = _omnipy.__version__


# Make sure _omnipy submodules are in sys.modules, and have not been
# damaged. This can happen if someone has messed with sys.modules, or
# the interpreter has been stopped and restarted.
reinit = 0
for k, v in list(_omnipy.__dict__.items()):
    if k[-5:] == "_func" and isinstance(v, types.ModuleType):
        sub = "_omnipy." + k
        if sub not in sys.modules:
            reinit = 1
            sys.modules[sub] = v
        del sub
del k, v

if reinit:
    _omnipy.ensureInit()
del reinit


# Add path to COS stubs if need be
_cospath = os.path.join(os.path.dirname(__file__), "COS")
if _cospath not in sys.path:
    sys.path.append(_cospath)
del _cospath


# Public functions

def coreVersion():
    """coreVersion()

Return a string containing the version number of the omniORB core, of
the form major.minor.micro. Versions from 3.0.0 up support the full
POA functionality."""
    return _coreVersion


_omniidl_args = []

def omniidlArguments(args):
    """omniidlArguments(list)

Set default omniidl arguments for importIDL() and importIDLString().
e.g. omniidlArguments(["-I/my/include", "-DMY_DEFINE"])"""

    global _omniidl_args

    if not isinstance(args, list):
        raise TypeError("argument must be a list of strings")

    for arg in args:
        if not isinstance(arg, str):
            raise TypeError("argument must be a list of strings")

    _omniidl_args = args


# Import an IDL file by forking the IDL compiler and processing the
# output
def importIDL(idlname, args=None, inline=1):
    """importIDL(filename [, args ] [, inline ]) -> tuple

Run the IDL compiler on the specified IDL file, and import the
resulting stubs. If args is present, it must contain a list of strings
used as arguments to omniidl. If args is not present, uses the default
set with omniidlArguments().

Normally imports the definitions for #included files as well as the
main file. Set inline to 0 to only import definitions for the main
file.

Returns a tuple of Python module names corresponding to the IDL module
names declared in the file. The modules can be accessed through
sys.modules."""

    import subprocess

    if not os.path.isfile(idlname):
        raise ImportError("File " + idlname + " does not exist")

    if args is None:
        args = _omniidl_args

    modname = os.path.basename(idlname).replace(".", "_")

    cmd = ["omniidl", "-bpython", "-Wbstdout"]

    if inline:
        cmd.append("-Wbinline")

    cmd.extend(args)
    cmd.append(idlname)

    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    try:
        tempname  = tempfile.mktemp()
        tempnamec = tempname + "c"
        while os.path.exists(tempnamec):
            tempname  = tempfile.mktemp()
            tempnamec = tempname + "c"

        mod    = imp.load_source(modname, tempname, proc.stdout)
        errors = proc.stderr.read()
        status = proc.wait()

    finally:
        # Get rid of byte-compiled file
        if os.path.isfile(tempnamec):
            os.remove(tempnamec)

    if status:
        if not isinstance(errors, str):
            errors = errors.decode("utf-8")

        raise ImportError(errors)

    try:
        mod.__file__ = idlname
        mods = mod._exported_modules

        for mod in mods:
            for m in (mod, skeletonModuleName(mod)):
                if m in _partialModules:
                    if m in sys.modules:
                        updateModuleDict(sys.modules[m], _partialModules[m])
                    else:
                        sys.modules[m] = _partialModules[m]
                    del _partialModules[m]

        return mods

    except (AttributeError, KeyError):
        sys.modules.pop(modname, None)
        raise ImportError("Invalid output from omniidl")


def importIDLString(str, args=None, inline=1):
    """importIDLString(string [, args ] [, inline ]) -> tuple

Run the IDL compiler on the given string, and import the resulting
stubs. If args is present, it must contain a list of strings used as
arguments to omniidl. If args is not present, uses the default set
with omniidlArguments().

Normally imports the definitions for #included files as well as the
main file. Set inline to 0 to only import definitions for the main
file.

Returns a tuple of Python module names corresponding to the IDL module
names declared in the file. The modules can be accessed through
sys.modules."""

    tfn = tempfile.mktemp()
    tf  = open(tfn, "w")
    tf.write(str)
    tf.close()
    try:
        ret = importIDL(tfn, args, inline)
    finally:
        os.remove(tfn)
    return ret


def cdrMarshal(tc, data, endian=-1):
    """cdrMarshal(TypeCode, data [,endian]) -> binary string

Marshal data with the given type into a CDR encapsulation. The data
can later be converted back into Python objects with cdrUnmarshal().
The encapsulation is language, platform, and ORB independent.

If the endian boolean is provided, it represents the endianness to
marshal with: True for little endian; false for big endian. The
resulting string in this case is the raw marshalled form, not a CDR
encapsulation. To unmarshal it, the endianness must be known.

CORBA.ORB_init must be called before using this function.

Throws CORBA.BAD_PARAM if the data does not match the TypeCode or
CORBA.BAD_INV_ORDER if the ORB has not been initialised."""

    if orb is None:
        raise CORBA.BAD_INV_ORDER(BAD_INV_ORDER_ORBNotInitialised,
                                  CORBA.COMPLETED_NO)

    if not isinstance(tc, CORBA.TypeCode):
        raise TypeError("Argument 1 must be a TypeCode")

    return _omnipy.cdrMarshal(tc._d, data, endian)


def cdrUnmarshal(tc, encap, endian=-1):
    """cdrUnmarshal(TypeCode, string [,endian]) -> data

Unmarshal a CDR stream created with cdrMarshal() or equivalent. The
encapsulation must adhere to the given TypeCode.

If the endian boolean is provided, it represents the endianness to
unmarshal with: True for little endian; false for big endian. In this
case, the string should be the raw marshalled form, not a CDR
encapsulation. If the endianness does not match that used for
marshalling, invalid data may be returned, or exceptions raised.

CORBA.ORB_init must be called before using this function.

Throws CORBA.MARSHAL if the binary string does not match the TypeCode
or CORBA.BAD_INV_ORDER if the ORB has not been initialised."""

    if orb is None:
        raise CORBA.BAD_INV_ORDER(BAD_INV_ORDER_ORBNotInitialised,
                                  CORBA.COMPLETED_NO)

    if not isinstance(tc, CORBA.TypeCode):
        raise TypeError("Argument 1 must be a TypeCode")

    return _omnipy.cdrUnmarshal(tc._d, encap, endian)



WTHREAD_CREATED = 0
WTHREAD_DELETED = 1

def addWThreadHook(hook):
    """addWThreadHook(hook) -> None

Arrange to call "hook(WTHREAD_{CREATED,DELETED}, wt)" on the new thread
whenever the runtime creates or deletes a Python "omniORB.WorkerThread"
"wt" (for instance as a result of a new incoming connection).  There is
no concurrency control: "addWThreadHook()" must be called before the
runtime creates any "WorkerThread"s.
"""
    WorkerThread.hooks.append(hook)


def importIRStubs():
    """importIRStubs() -> None

Make stubs for the Interface Repository appear in the CORBA module"""
    import omniORB.ir_idl
    CORBA._d_Object_interface = ((),(CORBA._d_InterfaceDef,),None)



# Import omniORB API functions. This provides:
#
#   installTransientExceptionHandler()
#   installTimeoutExceptionHandler()
#   installCommFailureExceptionHandler()
#   installSystemExceptionHandler()
#   traceLevel
#   traceInvocations
#   traceInvocationReturns
#   traceThreadId
#   traceTime
#   log
#   nativeCharCodeSet
#   fixed
#   minorCodeToString
#   setClientCallTimeout
#   setClientThreadCallTimeout
#   setClientConnectTimeout
#   myIPAddresses
#   setPersistentServerIdentifier
#   locationForward

from _omnipy.omni_func import *

# More public things at the end


# Private things

# ORB:
orb     = None
rootPOA = None
lock    = threading.Lock()

# Maps for object reference classes and IDL-defined types:
objrefMapping       = {}
skeletonMapping     = {}
typeMapping         = {}
typeCodeMapping     = {}
valueFactoryMapping = {}

# List of policy creation functions
policyMakers = []


def registerObjref(repoId, objref):
    objrefMapping[repoId] = objref

def registerSkeleton(repoId, skel):
    skeletonMapping[repoId] = skel

def registerType(repoId, desc, tc):
    typeMapping[repoId]     = desc
    typeCodeMapping[repoId] = tc

def findType(repoId):
    return typeMapping.get(repoId)

def findTypeCode(repoId):
    return typeCodeMapping.get(repoId)

def registerValueFactory(repoId, factory):
    old = valueFactoryMapping.get(repoId)
    valueFactoryMapping[repoId] = factory
    return old

def unregisterValueFactory(repoId):
    del valueFactoryMapping[repoId]

def findValueFactory(repoId):
    return valueFactoryMapping.get(repoId)


# Map of partially-opened modules
_partialModules = {}


# Function to return a Python module for the required IDL module name
def openModule(mname, fname=None):
    if mname == "CORBA":
        mod = sys.modules["omniORB.CORBA"]

    elif mname in sys.modules:
        mod = sys.modules[mname]

        if mname in _partialModules:
            pmod = _partialModules[mname]
            updateModuleDict(mod, pmod)
            del _partialModules[mname]
            
    elif mname in _partialModules:
        mod = _partialModules[mname]

    else:
        mod = newModule(mname)

    if not hasattr(mod, "__doc__") or mod.__doc__ is None:
        mod.__doc__ = "omniORB IDL module " + mname + "\n\n" + \
                      "Generated from:\n\n"

    if fname is not None:
        mod.__doc__ = mod.__doc__ + "  " + fname + "\n"

    return mod

# Function to create a new module, and any parent modules which do not
# already exist
def newModule(mname):
    mlist   = mname.split(".")
    current = ""
    mod     = None

    for name in mlist:
        current = current + name

        if current in sys.modules:
            mod = sys.modules[current]

        elif current in _partialModules:
            mod = _partialModules[current]

        else:
            newmod = imp.new_module(current)
            _partialModules[current] = mod = newmod

        current = current + "."

    return mod

def updateModule(mname):
    """
    updateModule(mname) -- update a module with a partial module
    stored in the partial module map.
    """
    if mname in _partialModules:
        pmod = _partialModules[mname]
        mod  = sys.modules[mname]
        updateModuleDict(mod, pmod)
        del _partialModules[mname]


def updateModuleDict(dest, source):
    dd = dest.__dict__
    sd = source.__dict__
    
    for k, v in sd.items():
        if not (k.startswith("__") and k.endswith("__")):
            dd[k] = v


def promotePartialModule(mname):
    """
    promotePartialModule(mname) -- convert partial module to full
    module in sys.modules.
    """
    sys.modules[mname] = _partialModules[mname]
    del _partialModules[mname]


def skeletonModuleName(mname):
    l = mname.split(".")
    l[0] = l[0] + "__POA"
    return ".".join(l)



# Function to create a new empty class as a scope place-holder
def newEmptyClass():
    class __dummy(object): pass
    return __dummy

 
# Docstring setting
def setDocString(obj, doc):
    obj.__doc__ = doc


# Classes to support IDL type mapping

class EnumItem(object):
    def __init__(self, name, value):
        self._n = name
        self._v = value
        return

    def __str__(self):
        return self._n

    def __repr__(self):
        return self._n

    def __cmp__(self, other):
        try:
            if isinstance(other, EnumItem):
                if other._parent_id == self._parent_id:
                    return cmp(self._v, other._v)
                else:
                    return cmp(self._parent_id, other._parent_id)
            else:
                return cmp(id(self), id(other))
        except:
            return cmp(id(self), id(other))

    def __hash__(self):
        return hash(self._parent_id + "/" + self._n)

class AnonymousEnumItem (EnumItem):
    def __init__(self, value):
        self._n = ""
        self._v = value
    
    def __repr__(self):
        return "anonymous enum item"


class Enum(object):
    def __init__(self, repoId, items):
        self._NP_RepositoryId = repoId
        self._items = items
        for i in items:
            i._parent_id = repoId

    def _item(self, n):
        return self._items[n]


class StructBase(object):
    _NP_RepositoryId = None
    _NP_ClassName = None
    
    def __repr__(self):
        cname = self._NP_ClassName
        if cname is None:
            cname = "%s.%s" % (self.__module__, self.__class__.__name__)

        desc = findType(self._NP_RepositoryId)
        if desc is None:
            # Type is not properly registered
            return "<%s instance at 0x%x>" % (cname, id(self))
        vals = []
        for i in range(4, len(desc), 2):
            attr = desc[i]
            try:
                val = getattr(self, attr)
                vals.append("%s=%s" % (attr,repr(val)))
            except AttributeError:
                vals.append("%s=<not set>" % attr)

        return "%s(%s)" % (cname, ", ".join(vals))

    def _tuple(self):
        desc = findType(self._NP_RepositoryId)
        if desc is None:
            # Type is not properly registered
            raise CORBA.BAD_PARAM(BAD_PARAM_IncompletePythonType,
                                  CORBA.COMPLETED_NO)
        vals = []
        for i in range(4, len(desc), 2):
            attr = desc[i]
            vals.append(getattr(self, attr))
        return tuple(vals)


class Union(object):
    _NP_ClassName = None
    _def_m = None

    def __init__(self, *args, **kw):
        if len(args) == 2:
            self._d = args[0]
            self._v = args[1]
        else:
            ks = list(kw.keys())
            if len(args) != 0 or len(ks) != 1:
                raise TypeError("require 2 arguments or one keyword argument.")
            k = ks[0]
            self.__setattr__(k, kw[k])

    def __getattr__(self, mem):
        if mem[0] == "_":
            raise AttributeError(mem)

        try:
            cmem = self._d_to_m[self._d]
            if mem == cmem:
                return self._v
            else:
                if mem == self._def_m or mem in self._m_to_d:
                    raise CORBA.BAD_PARAM(BAD_PARAM_WrongUnionMemberSelected,
                                          CORBA.COMPLETED_NO)
                else:
                    raise AttributeError(mem)
        except KeyError:
            if mem == self._def_m:
                return self._v
            else:
                if mem in self._m_to_d:
                    raise CORBA.BAD_PARAM(BAD_PARAM_WrongUnionMemberSelected,
                                          CORBA.COMPLETED_NO)
                else:
                    raise AttributeError(mem)

    def __setattr__(self, mem, val):
        if mem[0] == "_":
            self.__dict__[mem] = val
        else:
            try:
                disc = self._m_to_d[mem]
                self.__dict__["_d"] = disc
                self.__dict__["_v"] = val
            except KeyError:
                if mem == self._def_m:
                    self.__dict__["_d"] = self._def_d
                    self.__dict__["_v"] = val
                else:
                    raise AttributeError(mem)

    def __repr__(self):
        cname = self._NP_ClassName
        if cname is None:
            cname = "%s.%s" % (self.__module__, self.__class__.__name__)

        try:
            return "%s(%s = %s)" % (cname, self._d_to_m[self._d],
                                    repr(self._v))
        except KeyError:
            return "%s(%s, %s)" % (cname, repr(self._d), repr(self._v))


# Import sub-modules
from . import CORBA, tcInternal, omniPolicy
tcInternal.CORBA = CORBA

def createUnknownStruct(repoId, members):

    class UnknownStruct (StructBase):
        def __init__(self, *args):
            if len(args) != len(self._members):
                raise TypeError("__init__() takes exactly %d arguments "
                                "(%d given)" %
                                (len(self._members) + 1, len(args) + 1))

            self._values = args

            for i in range(len(args)):
                if self._members[i] != "":
                    setattr(self, self._members[i], args[i])

        def __repr__(self):
            vals = []
            for i in range(len(self._values)):
                attr = self._members[i]
                val  = self._values[i]
                if attr:
                    vals.append("%s=%s" % (attr, repr(val)))
                else:
                    vals.append(repr(val))

            return "UnknownStruct<%s>(%s)" % (self._NP_RepositoryId,
                                              ", ".join(vals))
        def _tuple(self):
            return tuple(self._values)

    UnknownStruct._NP_RepositoryId = repoId
    UnknownStruct._members         = members
    return UnknownStruct

def createUnknownUnion(repoId, def_used, members):

    class UnknownUnion (Union):
        pass

    UnknownUnion._NP_RepositoryId = repoId
    UnknownUnion._NP_ClassName    = "UnknownUnion<%s>" % repoId
    UnknownUnion._d_to_m          = {}
    UnknownUnion._m_to_d          = {}

    for i in range(len(members)):
        if i == def_used:
            UnknownUnion._def_d = members[i][0]
            UnknownUnion._def_m = members[i][1]
        else:
            UnknownUnion._d_to_m[members[i][0]] = members[i][1]
            UnknownUnion._m_to_d[members[i][1]] = members[i][0]

    return UnknownUnion

def createUnknownUserException(repoId, members):

    class UnknownUserException (CORBA.UserException):
        def __init__(self, *args):
            if len(args) != len(self._members):
                raise TypeError("__init__() takes exactly %d arguments "
                                "(%d given)" %
                                (len(self._members) + 1, len(args) + 1))

            self._values = args

            for i in range(len(args)):
                if self._members[i] != "":
                    setattr(self, self._members[i], args[i])

        def __repr__(self):
            vals = []
            for i in range(len(self._values)):
                attr = self._members[i]
                val  = self._values[i]
                if attr:
                    vals.append("%s=%s" % (attr, repr(val)))
                else:
                    vals.append(repr(val))

            return "UnknownUserException<%s>(%s)" % (self._NP_RepositoryId,
                                                     ", ".join(vals))

    UnknownUserException._NP_RepositoryId = repoId
    UnknownUserException._members         = members
    return UnknownUserException


class UnknownValueBase (CORBA.ValueBase):
    pass


def createUnknownValue(repoId, base_desc):

    if base_desc == tcInternal.tv_null:
        class UnknownValue (UnknownValueBase):
            pass
    else:
        base_cls = base_desc[1]
        if isinstance(base_cls, UnknownValueBase):
            class UnknownValue (base_cls):
                pass
        else:
            class UnknownValue (UnknownValueBase, base_cls):
                pass

    UnknownValue._NP_RepositoryId = repoId
    return UnknownValue


# Function to coerce an Any value with a partially-specified
# descriptor to a value with an equivalent, fully-specified
# descriptor.

def coerceAny(v, fd, td):
    if fd == td:
        return v

    if not tcInternal.equivalentDescriptors(fd, td):
        return None

    if not (isinstance(fd, tuple) and isinstance(td, tuple)):
        return None

    while fd[0] == tcInternal.tv_alias:
        fd = fd[3]

    while td[0] == tcInternal.tv_alias:
        td = td[3]

    try:
        if fd == td:
            return v

        elif fd[0] == tcInternal.tv_objref:
            return _omnipy.narrow(v, td[1])

        elif fd[0] == tcInternal.tv_struct:
            l = list(v._values)

            # Coerce each member
            for i in range(len(l)):
                l[i] = coerceAny(l[i], fd[i*2 + 5], td[i*2 + 5])
            
            return td[1](*l)

        elif fd[0] == tcInternal.tv_union:
            return td[1](v._d, coerceAny(v._v, fd[6][v._d], td[6][v._d]))

        elif fd[0] == tcInternal.tv_enum:
            return td[3][v._v]

        elif fd[0] == tcInternal.tv_sequence:
            l = v[:]
            for i in range(len(l)):
                l[i] = coerceAny(v[i], fd[1], td[1])
            return l

        elif fd[0] == tcInternal.tv_array:
            l = v[:]
            for i in range(len(l)):
                l[i] = coerceAny(v[i], fd[1], td[1])
            return l

        elif fd[0] == tcInternal.tv_except:
            l = list(v._values)

            # Coerce each member
            for i in range(len(l)):
                l[i] = coerceAny(l[i], fd[i*2 + 5], td[i*2 + 5])
            
            return td[1](*l)

        elif fd[0] == tcInternal.tv__indirect:
            return coerceAny(v, fd[1][0], td[1][0])

    except:
        return None

    return None


# Support for _is_a()
def static_is_a(cls, repoId):
    try:
        if cls._NP_RepositoryId == repoId:
            return 1

    except AttributeError:
        return 0
    
    for b in cls.__bases__:
        if static_is_a(b, repoId):
            return 1

    return 0


# Fixed point type

class fixedConstructor(object):
    def __init__(self, repoId, digits, scale):
        self._NP_RepositoryId = repoId
        self.digits           = digits
        self.scale            = scale

    def __call__(self, arg):
        try:
            return fixed(self.digits, self.scale, arg)
        except TypeError:
            raise TypeError("Invalid type for fixed argument")

    def __repr__(self):
        return "omniORB fixed<%d,%d> constructor" % (self.digits, self.scale)


# WorkerThread class used to make the threading module happy during
# operation dispatch.
# *** Depends on threading module internals ***

_thr_init = threading.Thread.__init__

try:
    _thr_id = threading._get_ident
except AttributeError:
    _thr_id = threading.get_ident

_thr_act  = threading._active
_thr_acq  = threading._active_limbo_lock.acquire
_thr_rel  = threading._active_limbo_lock.release

def_id = id

class WorkerThread(threading.Thread):

    hooks = []

    def __init__(self):
        id = _thr_id()
        _thr_init(self, name="omniORB-%d" % id)

        if hasattr(self, "_started"):
            self._started.set()
        elif hasattr(self._Thread__started, 'set'):
            self._Thread__started.set()
        else:
            self._Thread__started = 1

        self.id = id
        _thr_acq()

        if id in _thr_act:
            self.add = 0
        else:
            self.add = 1
            _thr_act[id] = self

        _thr_rel()
        if self.add:
            for hook in self.hooks:
                hook(WTHREAD_CREATED, self)

    def delete(self):
        if self.add:
            for hook in self.hooks:
                hook(WTHREAD_DELETED, self)
            _thr_acq()
            try:
                del _thr_act[self.id]
            finally:
                _thr_rel()

    def _set_daemon(self): return 1
    def join(self):        assert 0, "cannot join an omniORB WorkerThread"


# omniThreadHook is used to release a dummy omni_thread C++ object
# associated with a threading.Thread object when the thread stops.

class omniThreadHook(object):
    def __init__(self, target):
        self.target            = target

        try:
            self.target_stop       = target._Thread__stop
            target._Thread__stop   = self.omni_thread_stop

        except AttributeError:
            self.target_stop = target._stop
            target._stop     = self.omni_thread_stop

    def omni_thread_stop(self):
        try:
            delattr(self.target, "__omni_thread")
            try:
                del self.target._Thread__stop
            except AttributeError:
                del self.target._stop
        except AttributeError:
            pass
        self.target_stop()


# System exception mapping.

sysExceptionMapping = {}

cd = CORBA.__dict__
for exc in _omnipy.system_exceptions:
    cls = cd[exc]
    sysExceptionMapping[cls._NP_RepositoryId] = cls

del cd, exc, cls

# Reserved word mapping:

keywordMapping = {}
try:
    import keyword
    for word in keyword.kwlist:
        keywordMapping[word] = "_" + word

    keywordMapping["None"] = "_None"

    del keyword
except ImportError:
    pass

# Exception minor codes. See include/omniORB4/minorCode.h

def omniORBminorCode(c):
    return 0x41540000 | c

def OMGminorCode(c):
    return 0x4f4d0000 | c

from omniORB.minorCodes import *


# More public things, which depend on the CORBA module

# LOCATION_FORWARD exception
class LOCATION_FORWARD (Exception):
    """LOCATION_FORWARD(objref, permanent=0)

This exception may be thrown inside any operation implementation. It
causes the ORB the return a LOCATION_FORWARD message to the caller, so
the invocation is retried on the given object reference. If permanent
is set to 1, a permanent location forward is requested."""

    _NP_RepositoryId = "omniORB.LOCATION_FORWARD" # Not really a CORBA type

    def __init__(self, objref, perm=0):
        if not isinstance(objref, CORBA.Object):
            raise CORBA.BAD_PARAM(BAD_PARAM_WrongPythonType,
                                  CORBA.COMPLETED_NO)

        self._forward = objref
        self._perm    = perm

    def __str__(self):
        return "omniORB.LOCATION_FORWARD exception"

# "Static" objects required by the _omnipy module. They are here so
# memory management works correctly if the omniORB modules are
# unloaded.

_emptyTuple      = ()
_servantAttr     = "__omni_svt"
_objAttr         = "_obj"
_NP_RepositoryId = "_NP_RepositoryId"


# Register this module and the threading module with omnipy:
import omniORB, omniORB.PortableServer
_omnipy.registerPyObjects(omniORB)

# Import CORBA module stubs
from omniORB import corbaidl_idl
sys.modules["corbaidl_idl"]  = corbaidl_idl

from omniORB import boxes_idl
sys.modules["boxes_idl"]     = boxes_idl

from omniORB import pollable_idl
sys.modules["pollable_idl"]  = pollable_idl

from omniORB import messaging_idl
sys.modules["messaging_idl"] = messaging_idl

# Import the Interface Repository stubs if necessary
if "OMNIORBPY_IMPORT_IR_STUBS" in os.environ:
    importIRStubs()

promotePartialModule("CORBA__POA")
promotePartialModule("Messaging")
promotePartialModule("Messaging__POA")

del omniORB
