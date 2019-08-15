# coding: utf-8
from __future__ import absolute_import

from __future__ import print_function
__author__ = 'Zsolt'

__license__ = """
 This file is part of OpenModelica.

 Copyright (c) 1998-CurrentYear, Vanderbilt University; Metamorph Inc; Open Source Modelica Consortium (OSMC),
 c/o Linköpings universitet, Department of Computer and Information Science,
 SE-58183 Linköping, Sweden.

 All rights reserved.

 THIS PROGRAM IS PROVIDED UNDER THE TERMS OF THE BSD NEW LICENSE OR THE
 GPL VERSION 3 LICENSE OR THE OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2.
 ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3,
 ACCORDING TO RECIPIENTS CHOICE.

 The OpenModelica software and the OSMC (Open Source Modelica Consortium)
 Public License (OSMC-PL) are obtained from OSMC, either from the above
 address, from the URLs: http://www.openmodelica.org or
 http://www.ida.liu.se/projects/OpenModelica, and in the OpenModelica
 distribution. GNU version 3 is obtained from:
 http://www.gnu.org/copyleft/gpl.html. The New BSD License is obtained from:
 http://www.opensource.org/licenses/BSD-3-Clause.

 This program is distributed WITHOUT ANY WARRANTY; without even the implied
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, EXCEPT AS
 EXPRESSLY SET FORTH IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE
 CONDITIONS OF OSMC-PL.

 Version: 1.1
"""

import os
import sys
import time
import logging
import uuid
import subprocess
import tempfile
import pyparsing
import re
import struct
import platform


try:
    from functools import lru_cache
except ImportError:
    from backports.functools_lru_cache import lru_cache


if sys.platform == 'darwin':
    # on Mac let's assume omc is installed
    # OMPython packages are coming from here
    sys.path.append('/opt/local/lib/python2.7/site-packages/')
    # omc is here
    #sys.path.append('/opt/local/bin')

# TODO: replace this with the new parser
from OMPython import OMTypedParser, OMParser


def is_64bit_exe(exe):
    if not os.path.isfile(exe):
        return False

    with open(exe, 'rb') as pe:
        # IMAGE_DOS_HEADER.e_lfanew
        pe.seek(60)
        pe_offset = pe.read(4)
        if len(pe_offset) != 4:
            return False
        pe_offset = struct.unpack("I", pe_offset)[0]
        pe.seek(pe_offset + 4)
        # _IMAGE_FILE_HEADER.Machine
        machine = pe.read(2)
        if len(machine) != 2:
            return False
        return struct.unpack("H", machine)[0] == 0x8664


class OMCSession(object):

    def _start_server(self):
        self._server = subprocess.Popen(self._omc_command, shell=True, stdout=self._omc_log_file,
                                        stderr=self._omc_log_file)
        return self._server

    def _set_omc_corba_command(self, omc_path='omc'):
        #  +d=failtrace +d=interactivedump
        self._omc_command = "{0} +d=interactiveCorba +c={1} --showErrorMessages".format(omc_path, self._random_string)
        return self._omc_command

    def _start_omc(self):
        self._omc_command = None
        if platform.system() == 'Linux':
            self._set_omc_corba_command('omc')
            self._start_server()
            return
        if sys.platform == 'darwin':
            # FIXME: what is this case? are we looking at platform specifics? or different versions of OpenModelica?
            try:
                self._set_omc_corba_command('/opt/local/bin/omc')
                self._start_server()
            except:
                self.logger.exception("The OpenModelica compiler did not start")
                raise
            return
        try:
            self.omhome = os.environ['OPENMODELICAHOME']
            # add OPENMODELICAHOME\lib to PYTHONPATH so python can load omniORB libraries
            if (platform.architecture()[0] == '64bit') != is_64bit_exe(os.path.join(self.omhome, "lib", "python", "_omnipy.pyd")):
                # we can't load OpenModelica's 64-bit _omnipy.pyd. So use the bin\Python27 one
                # this needs an OpenModelica >1.9.7, but there's no x64 release <=1.9.7, so we are ok
                index = len(sys.path)
            elif sys.version_info[0] >= 3:
                # OpenModelica's omniORB is older and does not work with Python 3
                index = len(sys.path)
            else:
                index = 0
            sys.path.insert(index, os.path.join(self.omhome, 'lib'))
            sys.path.insert(index, os.path.join(self.omhome, 'lib', 'python'))
            # n.b. use OpenModelica bundled idl stubs. ours has _omnipy.checkVersion(3,0, __file__), but:
            # OpenModelica 1.11, 1.12 have version 4.2
            # OpenModelica 1.9.2, 1.9.7 have version 3.0
            sys.path.insert(0, os.path.join(self.omhome, 'share/omc/scripts/PythonInterface'))
            # add OPENMODELICAHOME\bin to path so python can find the omniORB binaries
            pathVar = os.getenv('PATH')
            pathVar += os.pathsep
            pathVar += os.path.join(self.omhome, 'bin')
            os.putenv('PATH', pathVar)
            self._set_omc_corba_command(os.path.join(self.omhome, 'bin', 'omc'))
            self._start_server()
        except:
            self.logger.exception("Exception")
            # FIXME: what is this case? are we looking at platform specifics? or different versions of OpenModelica?
            import OMConfig

            PREFIX = OMConfig.DEFAULT_OPENMODELICAHOME
            self.omhome = os.path.join(PREFIX)
            self._set_omc_corba_command(os.path.join(self.omhome, 'bin', 'omc'))
            self._start_server()

    def _connect_to_omc(self):
        # import the skeletons for the global module
        from omniORB import CORBA
        from OMPythonIDL import _OMCIDL
        # Locating and using the IOR
        if sys.platform == 'win32':
            self._ior_file = "openmodelica.objid." + self._random_string
        else:
            self.currentUser = os.environ['USER']
            if not self.currentUser:
                self.currentUser = "nobody"

            self._ior_file = "openmodelica." + self.currentUser + ".objid." + self._random_string
        self._ior_file = os.path.join(self._temp_dir, self._ior_file)
        self._omc_corba_uri = "file:///" + self._ior_file
        # See if the omc server is running
        attempts = 0
        while True:
            if not os.path.isfile(self._ior_file):
                time.sleep(0.25)
                attempts += 1
                if attempts == 50:
                    self.logger.error("OMC Server is down. Please start it!")
                    raise Exception("OMC Server is down. Please start it!")
            else:
                self.logger.info("OMC Server is up and running at {0}".format(self._omc_corba_uri))
                break

        #initialize the ORB with maximum size for the ORB set
        sys.argv.append("-ORBgiopMaxMsgSize")
        sys.argv.append("2147483647")
        self._orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
        # Read the IOR file
        with open(self._ior_file, 'r') as f_p:
            self._ior = f_p.readline()

        # Find the root POA
        self._poa = self._orb.resolve_initial_references("RootPOA")
        # Convert the IOR into an object reference
        self._obj_reference = self._orb.string_to_object(self._ior)
        # Narrow the reference to the OmcCommunication object
        self._omc = self._obj_reference._narrow(_OMCIDL.OmcCommunication)
        # Check if we are using the right object
        if self._omc is None:
            self.logger.error("Object reference is not valid")
            raise Exception("Object reference is not valid")

    def __init__(self, readonly=False):
        self.readonly = readonly
        self._omc = None
        self._omc_log_file = None
        self._server = None

        self._REGEX_PATTERN_getComponents = r'{?{?([\w\d\.]+),([\w\d\.]+),"([^\"]*)", "([\w]+)", ([\w]+), ([\w]+), ([\w]+), ([\w]+), "([\w]+)", "([\w]+)", "([\w]+)",{(([\d]*|:)(,([\d]*|:))*)'
        self._REGEX_getComponents = re.compile(self._REGEX_PATTERN_getComponents)

        #{{(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*),(true|false|[\-0-9.]*)}}
        _T_F_float = r'(true|false|[\-0-9.]*)'
        _T_F_float_repeat = (_T_F_float + ',')*14  + _T_F_float
        self._REGEX_PATTERN_getNthComponentAnnotation = r'{?{?' + _T_F_float_repeat + '}?}?'
        self._REGEX_getNthComponentAnnotation = re.compile(self._REGEX_PATTERN_getNthComponentAnnotation)

        self._REGEX_PATTERN_getReplaceables = r'  replaceable (package|model|function|block) (\w+) = (\S+)'
        self._REGEX_getReplaceables = re.compile(self._REGEX_PATTERN_getReplaceables)

        # r'  extends ([\w.]+)\(([\w\W]*?redeclare package)\W+(\w*)\W*=\W*([\w.]+)[\)|,]'
        self._REGEX_PATTERN_getPackagesFromExtends = r'  extends ([\w.]+)\(([\w\W]*?redeclare package)\W+(\w*)\W*=\W*([\w.]+)[\)|,]'
        self._REGEX_getPackagesFromExtends = re.compile(self._REGEX_PATTERN_getPackagesFromExtends)

        # r'  Modelica.Fluid.Interfaces.FluidPort_b port_b\([.\r\n\s\S]*?redeclare package Medium = ([\w|\d|\.]*)[\)|,]'
        # r'  Modelica.Fluid.Interfaces.FluidPort_[a|b] (\w+)\([\w\W]*?redeclare package[^\w]+(\w+)[^\w]*=[^\w]*(\w+)[\)|,]'
        #self._REGEX_PATTERN_getPortRedeclares = r'  {0} {1}\([.\r\n\s\S]*?redeclare {2} (\w+) = ([\w|\d|\.]*)[\)|,]'
        self._REGEX_PATTERN_getPortRedeclares = r'  {0} {1}(?:\[[\w\s]*\])?\([\w\W]*?redeclare\s+(?:each\s+)?(?:final\s+)?{2}[^\w]*(\w+)[^\w]*=[^\w]*(\w+)[\)|,]'  # {0} is class, {1} is portName, {2} is redeclareType

        self._REGEX_PATTERN_checkParameterArrayValue = r'(\[[.,; e\-\d\r\n]+\])'
        self._REGEX_checkParameterArrayValue = re.compile(self._REGEX_PATTERN_checkParameterArrayValue)

        self.logger = logging.getLogger('py_modelica_exporter.OMCSession')
        self.logger.setLevel(logging.NOTSET)

        self._temp_dir = tempfile.gettempdir()
        # this file must be closed in the destructor
        self._omc_log_file = open(os.path.join(self._temp_dir, "openmodelica.omc.output.OMPython"), 'w')

        # generate a random string for this session
        self._random_string = uuid.uuid4().hex

        # start up omc executable, which is waiting for the CORBA connection
        self._start_omc()

        # connect to the running omc instance using CORBA
        self._connect_to_omc()
        if readonly:
            self.sendExpression_cached = lru_cache(maxsize=2048)(self.sendExpression_cached)

    def sendExpression(self, command):
        if command == 'getErrorString':
            return self._omc.sendExpression(command)
        return self.sendExpression_cached(command)

    def sendExpression_cached(self, command):
        return self._omc.sendExpression(command)

    def __del__(self):
        if self._omc:
            self._omc.sendExpression("quit();")  # FIXME: does not work in a virtual python environment
        if self._omc_log_file:
            self._omc_log_file.close()
        # kill self._server process if it is still running/exists
        if self._server is not None and self._server.returncode is None:
            self._server.kill()

    # TODO: this method will be replaced by the new parser
    def execute(self, command):
        result = self.sendExpression(command)
        return OMTypedParser.parseString(result)

        # try:
        #     answer = OMTypedParser.parseString(result)
        #     return answer
        # except Exception as e:
        #     file_name = 'C:\Users\James\Downloads\OMParserError\OMC_calls_and_responses.txt'
        #
        #     with open(file_name, 'a') as f_out:
        #         f_out.write('=====================================================\n')
        #         f_out.write(command + '\n')
        #         f_out.write('result = {0}\n'.format(result))
        #         f_out.write('OMTypedParser.parseString(result) caused Exception: {0}\n'.format(e))
        #         f_out.write('=====================================================\n')
        #
        #     raise e

    def ask(self, question, opt=None, parsed=True):
        if opt:
            expression = '{0}({1})'.format(question, opt)
        else:
            expression = question

        try:
            if parsed:
                res = self.execute(expression)
            else:
                res = self.sendExpression(expression)
        except Exception as e:
            # self.logger.debug("Failed: {0}({1}, parsed={2})".format(question, opt, parsed))
            raise
        return res

    # TODO: Open Modelica Compiler API functions. Would be nice to generate these.
    def loadFile(self, filename):
        response = self.ask('loadFile', '"{0}"'.format(filename))

        return response

    def loadModel(self, className):
        response = self.ask('loadModel', className)

        return response

    def isModel(self, className):
        try:
            return self.ask('isModel', className)
        except:
            return False
    def isPackage(self, className):
        return self.ask('isPackage', className)

    def isPrimitive(self, className):
        return self.ask('isPrimitive', className, parsed=False) == 'true\n'

    def isConnector(self, className):
        try:
            return self.ask('isConnector', className)
        except pyparsing.ParseException as ex:
            raw_text = self.ask('isConnector', className, parsed=False)

            if 'true' in raw_text:
                return True
            elif 'True' in raw_text:
                return True
            else:
                return False

    def isRecord(self, className):
        return self.ask('isRecord', className)

    def isBlock(self, className):
        try:
            return self.ask('isBlock', className)
        except:
            return False
    def isType(self, className):
        return self.ask('isType', className)

    def isFunction(self, className):
        return self.ask('isFunction', className)

    def isClass(self, className):
        return self.ask('isClass', className)

    def isParameter(self, className):
        return self.ask('isParameter', className)

    def isConstant(self, className):
        return self.ask('isConstant', className)

    def isProtected(self, className):
        return self.ask('isProtected', className)

    def getIconAnnotation(self, className):
        return self.ask('getIconAnnotation', className, parsed=False)

    # def getPackages(self):
    #     return self.ask('getPackages')

    def getPackages(self, className=None):
        if className is None:
            return self.ask('getPackages')
        else:
            try:
                result = self.ask('getPackages', className)
                return result
            except:
                result = self.ask('getPackages', className, parsed=False)
                return result

    def getReplaceables(self, className, namedPackage=None):

        raw_text = self.ask('list', className, parsed=False)
        regex_findall = self._REGEX_getReplaceables.findall(raw_text)

        regex_matches = []

        for regex_match in regex_findall:
            if (regex_match[0] == 'package'):
                match_dict = {}
                match_dict['name'] = regex_match[1]
                match_dict['value'] = regex_match[2]
                regex_matches.append(match_dict)

        return regex_matches

    def getPackagesFromExtends(self, className):

        raw_text = self.ask('list', className, parsed=False)
        regex_findall = self._REGEX_getPackagesFromExtends.findall(raw_text)

        regex_matches = {}

        for regex_match in regex_findall:
            split = regex_match[1].split('(')
            if len(split) > 1:
                if (')' in split[-1]):
                    pass
                else:
                    continue

            match_dict = {}
            #match_dict['extends_class'] = regex_match[0]
            match_dict['name'] = regex_match[2]
            match_dict['value'] = regex_match[3]
            regex_matches[regex_match[0]] = match_dict

        return regex_matches

    def getPortRedeclares(self, className, portType, portName, redeclareType='package'):

        raw_text = self.ask('list', className, parsed=False)
        # FIXME portType may not be fully-qualified in raw_text
        custom_pattern = self._REGEX_PATTERN_getPortRedeclares.format(portType, portName, redeclareType)

        redeclare = None
        regex_findall = re.findall(custom_pattern, raw_text)

        if regex_findall:
            redeclare = regex_findall[0]
        else:
            print('Could not get port redeclare for {}'.format(className))

        return redeclare

    def getClassRestriction(self, className):
        return self.ask('getClassRestriction', className)

    def getDerivedClassModifierNames(self, className):
        return self.ask('getDerivedClassModifierNames', className)

    def getDerivedClassModifierValue(self, className, modifierName):
        return self.ask('getDerivedClassModifierValue', '{0}, {1}'.format(className, modifierName))

    def typeNameStrings(self, className):
        return self.ask('typeNameStrings', className)

    def getComponents(self, className, parsed=True):
        try:
            return self.ask('getComponents', className, parsed=parsed)

        except pyparsing.ParseException as ex:

            unparsed_result = self.ask('getComponents', className, parsed=False)
            component_details = unparsed_result.split('}},{')
            regex_results = []
            failed_to_match = []

            for component_detail in component_details:
                regex_match = self._REGEX_getComponents.match(component_detail)
                if regex_match:
                    regex_result = (regex_match.group(1),
                                    regex_match.group(2),
                                    regex_match.group(3),
                                    regex_match.group(4),
                                    regex_match.group(5) == 'true',
                                    regex_match.group(6) == 'true',
                                    regex_match.group(7) == 'true',
                                    regex_match.group(8) == 'true',
                                    regex_match.group(9),
                                    regex_match.group(10),
                                    regex_match.group(11),
                                    ())
                    regex_results.append(regex_result)
                else:
                    failed_to_match.append(component_detail)

            # debug_file = 'C:\Users\James\Downloads\OMParserError\{0}.txt'.format(className)
            # with open(debug_file, 'a') as f_out:
            #     f_out.write('Calling: "getComponents({0})"\n'.format(className))
            #     f_out.write('=====================================================\n')
            #     f_out.write('unparsed_result: "{0}"\n'.format(unparsed_result))
            #     f_out.write('=====================================================\n')
            #     f_out.write('regex_result: "{0}"\n'.format(regex_results))
            #     f_out.write('failed_to_match: "{0}"\n'.format(failed_to_match))

            return regex_results

    def getClassComment(self, className):
        try:
            return self.ask('getClassComment', className)
        except pyparsing.ParseException as ex_parse:
            return 'No description available'
        except Exception as ex_gen:
            return 'No description available (ex)'

    def getComponentComment(self, className):
        try:
            return self.ask('getComponentComment', className)
        except:
            return 'No description available'

    def getNthComponent(self, className, comp_id):
        """ returns with (type, name, description) """
        return self.ask('getNthComponent', '{0}, {1}'.format(className, comp_id))

    def getNthComponentAnnotation(self, className, comp_id):
        try:
            omc_response = self.ask('getNthComponentAnnotation', '{0}, {1}'.format(className, comp_id))

            comp_annotation = []
            if omc_response and (len(omc_response) == 1):
                comp_annotation = list(omc_response[0])

            return comp_annotation

        except pyparsing.ParseException as ex:

            raw_text = self.ask('getNthComponentAnnotation', '{0}, {1}'.format(className, comp_id), parsed=False)

            regex_findall = self._REGEX_getNthComponentAnnotation.findall(raw_text)

            comp_annotation = []
            if regex_findall and (len(regex_findall) == 1):
                for value in regex_findall[0]:
                    try:
                        comp_annotation.append(float(value))
                    except:
                        if value == 'true':
                            comp_annotation.append(True)
                        elif value == 'false':
                            comp_annotation.append(False)
                        else: # need to check if the value is a tuple, and maybe unpack the tuple
                            comp_annotation.append(value)

            return comp_annotation

    def getImportCount(self, className):
        return self.ask('getImportCount', className)

    def getNthImport(self, className, importNumber):
        # [Path, id, kind]
        return self.ask('getNthImport', '{0}, {1}'.format(className, importNumber))

    def getInheritanceCount(self, className):
        return self.ask('getInheritanceCount', className)

    def getNthInheritedClass(self, className, inheritanceDepth):
        return self.ask('getNthInheritedClass', '{0}, {1}'.format(className, inheritanceDepth))

    def getParameterNames(self, className):
        try:
            return self.ask('getParameterNames', className)
        except KeyError as ex:
            self.logger.warning('OMPython error when calling "ask(getParameterNames, {0})": {1}'.format(className, ex.message))
            # FIXME: OMC returns with a different structure for empty parameter set
            return []

    def getParameterValue(self, className, parameterName):
        try:
            raw_text = self.ask('getParameterValue', '{0}, {1}'.format(className, parameterName), parsed=False)
            return OMTypedParser.parseString(raw_text)
        except pyparsing.ParseException as ex:

            regex_findall = self._REGEX_checkParameterArrayValue.findall(raw_text)

            if regex_findall and (len(regex_findall) == 1):
                return regex_findall[0]
            else:
                return ""

    def getComponentModifierNames(self, className, componentName):
        return self.ask('getComponentModifierNames', '{0}, "{1}"'.format(className, componentName))

    def getComponentModifierValue(self, className, componentName):
        try:
            # FIXME: OMPython exception UnboundLocalError exception for 'Modelica.Fluid.Machines.ControlledPump'
            return self.ask('getComponentModifierValue', '{0}, {1}'.format(className, componentName))
        except pyparsing.ParseException as ex:
            result = self.ask('getComponentModifierValue', '{0}, {1}'.format(className, componentName), parsed=False)
            try:
                answer = OMParser.check_for_values(result)
                OMParser.result = {}
                return answer[2:]
            except (TypeError, UnboundLocalError) as ex:
                self.logger.info('OMParser error: {0} when calling "OMParser.check_for_values({1})"'.format(ex.message, result))
                return result
            except SystemExit as sys_exit:
                self.logger.warning('sys.exit() called in "getComponentModifierValue()"')
                return 'sys.exit()'
        except SystemExit as sys_exit:
            self.logger.warning('sys.exit() called in "getComponentModifierValue()"')
            return 'sys.exit()'

    def getExtendsModifierNames(self, className, componentName):
        return self.ask('getExtendsModifierNames', '{0}, {1}'.format(className, componentName))

    def getExtendsModifierValue(self, className, extendsName, modifierName):
        try:
            # FIXME: OMPython exception UnboundLocalError exception for 'Modelica.Fluid.Machines.ControlledPump'
            return self.ask('getExtendsModifierValue', '{0}, {1}, {2}'.format(className, extendsName, modifierName))
        except pyparsing.ParseException as ex:
            result = self.ask('getExtendsModifierValue', '{0}, {1}, {2}'.format(className, extendsName, modifierName), parsed=False)
            try:
                answer = OMParser.check_for_values(result)
                OMParser.result = {}
                return answer[2:]
            except (TypeError, UnboundLocalError) as ex:
                self.logger.info('OMParser error: {0} when calling "OMParser.check_for_values({1})"'.format(ex.message, result))
                return result
            except SystemExit as sys_exit:
                self.logger.warning('sys.exit() called in "getExtendsModifierValue()"')
                return 'sys.exit()'
        except SystemExit as sys_exit:
            self.logger.warning('sys.exit() called in "getExtendsModifierValue()"')
            return 'sys.exit()'

    def getNthComponentModification(self, className, comp_id):
        # FIXME: OMPython exception Results KeyError exception

        # get {$Code(....)} field
        # \{\$Code\((\S*\s*)*\)\}
        value = self.ask('getNthComponentModification', '{0}, {1}'.format(className, comp_id), parsed=False)
        value = value.replace("{$Code(", "")
        return value[:-3]
        #return self.re_Code.findall(value)

    def getClassInformation(self, className):
        answer = self.ask('getClassInformation', className)
        return answer

    # function getClassNames
    #   input TypeName class_ = $Code(AllLoadedClasses);
    #   input Boolean recursive = false;
    #   input Boolean qualified = false;
    #   input Boolean sort = false;
    #   input Boolean builtin = false "List also builtin classes if true";
    #   input Boolean showProtected = false "List also protected classes if true";
    #   output TypeName classNames[:];
    # end getClassNames;
    def getClassNames(self, className=None, recursive=False, qualified=False, sort=False, builtin=False,
                      showProtected=False):
        try:
            if className:
                opt = '{0}, recursive={1}, qualified={2}, sort={3}, builtin={4}, showProtected={5}'.format(
                    className,
                    str(recursive).lower(),
                    str(qualified).lower(),
                    str(sort).lower(),
                    str(builtin).lower(),
                    str(showProtected).lower())
                value = self.ask('getClassNames', opt)
            else:
                opt = 'recursive={0}, qualified={1}, sort={2}, builtin={3}, showProtected={4}'.format(
                    str(recursive).lower(),
                    str(qualified).lower(),
                    str(sort).lower(),
                    str(builtin).lower(),
                    str(showProtected).lower())
                value = self.ask('getClassNames', opt)
            return value
        except:
            return list()

    def getConnectionCount(self, className):
        return self.ask('getConnectionCount', className)

    def getNthConnection(self, className, n):
        return self.ask('getNthConnection', '{0}, {1}'.format(className, n))
