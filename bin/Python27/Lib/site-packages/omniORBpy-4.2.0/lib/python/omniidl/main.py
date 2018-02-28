#!/usr/bin/env python
# -*- python -*-
#                           Package   : omniidl
# main.py                   Created on: 1999/11/05
#                           Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2002-2012 Apasphere Ltd
#    Copyright (C) 1999      AT&T Laboratories Cambridge
#
#  This file is part of omniidl.
#
#  omniidl is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#  02111-1307, USA.
#
# Description:
#   
#   IDL compiler main function

"""IDL Compiler front-end main function"""

import sys, os

if sys.hexversion < 0x10502f0:
    sys.stderr.write("\n\n")
    sys.stderr.write("omniidl: WARNING!!\n\n")
    sys.stderr.write("omniidl: Python version 1.5.2 or later is required.\n")
    sys.stderr.write("omniidl: " + sys.executable + " is version " + \
                     sys.version + "\n")
    sys.stderr.write("omniidl: Execution is likely to fail.\n")
    sys.stderr.write("\n\n\n")
    sys.stderr.flush()

import _omniidl
import getopt, os, os.path

import idlast, idltype

cmdname = "omniidl"

StringType = type("")

def version():
    print "omniidl version 1.0"

def usage():
    global backends
    print "\nUsage:", cmdname, "-b<back_end> [flags] file1 file2 ..."
    print """
The supported flags are:

  -Dname[=value]  Define name for preprocessor
  -Uname          Undefine name for preprocessor
  -Idir           Include dir in search path for preprocessor
  -E              Run preprocessor only, print on stdout
  -Ycmd           Set command for the preprocessor
  -N              Do not run preprocessor
  -P              Add defines relevant to platform dependencies (internal use)
  -T              Use a temporary file, not a pipe, for preprocessor output
  -Wparg[,arg...] Send args to the preprocessor
  -bback_end      Select a back-end to be used. More than one permitted
  -Wbarg[,arg...] Send args to the back-end
  -nf             Do not warn about unresolved forward declarations
  -nc             Do not treat identifiers differing only in case as an error
  -k              Comments after declarations are kept for the back-ends
  -K              Comments before declarations are kept for the back-ends
  -Cdir           Change directory to dir before writing output
  -d              Dump the parsed IDL then exit
  -i              Enter interactive mode after parsing the IDL
  -pdir           Path to omniidl back-ends ($TOP/lib/python)
  -V              Print version info then exit
  -u              Print this usage message and exit
  -v              Trace compilation stages"""

    if backends == []:
        print """
You must select a target back-end with -b. For C++, use -bcxx:

  omniidl -bcxx file.idl

To see options specific to C++, use:

  omniidl -bcxx -u
"""

preprocessor_name = "omnicpp"
preprocessor_args = []
preprocessor_only = 0

if sys.platform == "win32":
    exe_suffix = ".exe"
else:
    exe_suffix = ""

if sys.platform != "OpenVMS":
    preprocessor_paths = []
    if hasattr(_omniidl, "__file__"):
        preprocessor_paths.append(os.path.dirname(_omniidl.__file__))

    preprocessor_paths.append(os.path.dirname(sys.argv[0]))

    preprocessor = preprocessor_name
    for preprocessor_path in preprocessor_paths:
        if os.path.isfile(os.path.join(preprocessor_path,
                                       preprocessor_name + exe_suffix)):
            preprocessor = os.path.join(preprocessor_path, preprocessor_name)
            break

    preprocessor_cmd  = preprocessor + " -lang-c++ -undef -D__OMNIIDL__=" + \
                        _omniidl.version
else:
    # OpenVMS
    if hasattr(_omniidl, "__file__"):
        preprocessor_path = os.path.dirname(os.path.abspath(_omniidl.__file__))
    else:
        preprocessor_path = os.path.dirname(os.path.abspath(sys.argv[0]))

    names = preprocessor_path.replace('.', '^.').split("/")

    preprocessor_options = ('-lang-c++ -undef "-D__OMNIIDL__=%s"' %
                            _omniidl.version)
    preprocessor_cmdfmt  = "mcr %s:[%s]%s %s"
    preprocessor_cmd     = preprocessor_cmdfmt % (names[1],
                                                  ".".join(names[2:]),
                                                  preprocessor_name,
                                                  preprocessor_options)

no_preprocessor   = 0
backends          = []
backends_args     = []
dump_only         = 0
cd_to             = None
verbose           = 0
quiet             = 0
print_usage       = 0
interactive       = 0
temp_file         = None

def parseArgs(args):
    global preprocessor_args, preprocessor_only, preprocessor_cmd
    global no_preprocessor, backend, backend_args, dump_only, cd_to
    global verbose, quiet, print_usage, interactive, temp_file

    paths = []

    try:
        opts,files = getopt.getopt(args, "D:I:U:EY:NW:b:n:kKC:dVuhvqp:iTP")
    except getopt.error, e:
        sys.stderr.write("Error in arguments: -" + e.opt + "\n")
        sys.stderr.write("Use '" + cmdname + " -u' for usage\n")
        sys.exit(1)

    for opt in opts:
        o,a = opt

        if o == "-D":
            preprocessor_args.append("-D" + a)

        elif o == "-I":
            if sys.platform != "OpenVMS":
                preprocessor_args.append('-I "%s"' % a)
            else:
                # It would be stupid, rude, and evil to put spaces in
                # a dirname on VMS
                preprocessor_args.append('-I%s' % a)

        elif o == "-U":
            preprocessor_args.append("-U" + a)

        elif o == "-E":
            preprocessor_only = 1

        elif o == "-Y":
            preprocessor_cmd = a

        elif o == "-N":
            no_preprocessor = 1

        elif o == "-W":
            if a[0] == "p":
                preprocessor_args.extend(a[1:].split(","))
            elif a[0] == "b":
                if len(backends) == 0:
                    if not quiet:
                        sys.stderr.write(cmdname + ": Error in arguments: "
                                         "no back-ends selected\n")
                    sys.exit(1)
                backends_args[-1].extend(a[1:].split(","))
            else:
                if not quiet:
                    sys.stderr.write("Error in arguments: option " + o + \
                                     " not recognized\n")
                    sys.stderr.write("Use " + cmdname + " -u for usage\n")
                sys.exit(1)

        elif o == "-C":
            if not os.path.isdir(a):
                if not quiet:
                    sys.stderr.write(cmdname + ": '" + a + \
                                     "' is not a directory\n")
                sys.exit(1)
            cd_to = a

        elif o == "-b":
            backends.append(a)
            backends_args.append([])

        elif o == "-n":
            if a == "f":
                _omniidl.noForwardWarning()
            elif a == "c":
                sys.stderr.write(cmdname + \
                                 ": Warning: -nc option means omniidl will " \
                                 "silently accept invalid IDL.\n")
                _omniidl.caseSensitive()
            else:
                if not quiet:
                    sys.stderr.write(cmdname + \
                                     ": unknown error suppresion option '" + \
                                     a + "'\n")
                    sys.stderr.write("Use " + cmdname + " -u for usage\n")
                sys.exit(1)

        elif o == "-k":
            preprocessor_args.append("-C")
            _omniidl.keepComments(0)

        elif o == "-K":
            preprocessor_args.append("-C")
            _omniidl.keepComments(1)

        elif o == "-d":
            dump_only = 1

        elif o == "-V":
            version()
            sys.exit(0)

        elif o == "-u" or o == "-h":
            print_usage = 1

        elif o == "-v":
            verbose = verbose + 1

        elif o == "-q":
            quiet = 1
            _omniidl.quiet()

        elif o == "-p":
            paths.append(a)

        elif o == "-i":
            interactive = 1

        elif o == "-T":
            temp_file = genTempFileName()

        elif o == "-P":
            preprocessor_args.extend(_omniidl.platformDefines())

    sys.path = paths + sys.path

    return files


def genTempFileName():
    try:
        import tempfile
        return tempfile.mktemp(".idl")
    except ImportError:
        # No tempfile module. Just use current directory...
        return "omniidl-tmp%s.idl" % os.getpid()


def my_import(name):
    mod = __import__(name)
    components = name.split(".")
    for comp in components[1:]:
        mod = getattr(mod, comp)
    return mod

def be_import(name):
    try:
        return my_import("omniidl_be." + name)

    except ImportError, ex:
        try:
            # Try to import from just "name" rather than "omniidl_be.name"
            return my_import(name)

        except ImportError:
            # Raise the original exception
            raise ex

def main(argv=None):
    global preprocessor_args, preprocessor_only, preprocessor_cmd
    global no_preprocessor, backend, backend_args, dump_only, cd_to
    global verbose, quiet, print_usage, interactive, temp_file

    if argv is None: argv = sys.argv

    files = parseArgs(argv[1:])

    if _omniidl.alwaysTempFile():
        temp_file = genTempFileName()

    if print_usage:
        usage()        

    elif len(files) == 0:
        if not quiet:
            sys.stderr.write(cmdname + ": No files specified. Use '" \
                             + cmdname + " -u' for usage.\n")
        sys.exit(1)

    # Import back-ends, and add any pre-processor arguments
    bemodules = []
    for backend in backends:
        if verbose:
            sys.stderr.write(cmdname + ": Importing back-end '" +\
                             backend + "'\n")
        try:
            be = be_import(backend)
        except ImportError, msg:
            if not quiet:
                sys.stderr.write(cmdname + \
                                 ": Could not import back-end '" + \
                                 backend + "'\n")
                sys.stderr.write(cmdname + \
                                 ": Maybe you need to use the -p option?\n")
                sys.stderr.write(cmdname + \
                                 ": (The error was '" + str(msg) + "')\n")
            sys.exit(1)

        if verbose:
            if hasattr(be, "__file__"):
                sys.stderr.write(cmdname + ": '" + backend + \
                                 "' imported from '" + be.__file__ + "'\n")
            else:
                sys.stderr.write(cmdname + ": '" + backend + \
                                 "' imported from unknown file\n")

        bemodules.append(be)
        if hasattr(be, "cpp_args"):
            preprocessor_args.extend(be.cpp_args)

        if print_usage and hasattr(be, "usage_string"):
            print "\nArguments for back-end '" + backend + "':\n"
            print be.usage_string

    if print_usage:
        sys.exit(0)

    if len(backends) == 0 and \
       not (quiet or dump_only or preprocessor_only or interactive):
        sys.stderr.write(cmdname + ": Warning: No back-ends specified; " \
                         "checking IDL for validity\n")

    for name in files:
        if name != "-" and not os.path.isfile(name):
            if not quiet:
                sys.stderr.write(cmdname + ": '" + name + "' does not exist\n")
            sys.exit(1)

        if sys.platform != 'OpenVMS' or len(preprocessor_args)==0:
            preproc_cmd = '%s %s "%s"' % (preprocessor_cmd,
                                          " ".join(preprocessor_args),
                                          name)
        else:
            preproc_cmd = '%s "%s" %s' % (preprocessor_cmd,
                                          '" "'.join(preprocessor_args),
                                          name)
        if not no_preprocessor:
            if verbose:
                sys.stderr.write(cmdname + ": Preprocessing '" +\
                                 name + "' with '" + preproc_cmd + "'\n")

            if preprocessor_only:
                err = os.system(preproc_cmd)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    sys.exit(1)
                sys.exit(0)

            if temp_file:
                if verbose:
                    sys.stderr.write(cmdname + \
                                     ": cpp output to temporary file '" + \
                                     temp_file + "'\n")
                err = os.system(preproc_cmd + " >" + temp_file)
                if err:
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    try:
                        os.remove(temp_file)
                    except:
                        pass
                    sys.exit(1)
                file = temp_file

            else:
                file = os.popen(preproc_cmd, "r")

        else: # No preprocessor
            file = name

        if verbose: sys.stderr.write(cmdname + ": Running front end\n")

        if dump_only:
            if verbose:
                sys.stderr.write(cmdname + ": Dumping\n")
            _omniidl.dump(file, name)
            if not isinstance(file, StringType):
                file.close()
            if temp_file: os.remove(temp_file)
        else:
            tree = _omniidl.compile(file, name)

            if not isinstance(file, StringType):
                if file.close():
                    if not quiet:
                        sys.stderr.write(cmdname + \
                                         ": Error running preprocessor\n")
                    sys.exit(1)

            if tree is None:
                sys.exit(1)

            if cd_to is not None:
                old_wd = os.getcwd()
                os.chdir(cd_to)
            
            i = 0
            for backend in backends:
                if verbose:
                    sys.stderr.write(cmdname + ": Running back-end '" +\
                                     backend + "'\n")

                bemodules[i].run(tree, backends_args[i])
                i = i + 1

            if interactive:
                if verbose:
                    sys.stderr.write(cmdname + ": Entering interactive loop\n")

                idlast.tree = tree
                _omniidl.runInteractiveLoop()
                del idlast.tree

            if cd_to is not None:
                os.chdir(old_wd)

            if temp_file and not no_preprocessor:
                os.remove(temp_file)

            idlast.clear()
            idltype.clear()
            _omniidl.clear()
