__author__ = 'Zsolt'

import logging
import json
import os
import sys

from omc_session import OMCSession
from generate_icons import IconExporter
from modelica_classes import *

try:
    from lxml import etree
    #print("running with lxml.etree")
except ImportError:
    try:
        # Python 2.5
        import xml.etree.cElementTree as etree
        #print("running with cElementTree on Python 2.5+")
    except ImportError:
        try:
            # Python 2.5
            import xml.etree.ElementTree as etree
            #print("running with ElementTree on Python 2.5+")
        except ImportError:
            try:
                # normal cElementTree install
                import cElementTree as etree
                #print("running with cElementTree")
            except ImportError:
                try:
                    # normal ElementTree install
                    import elementtree.ElementTree as etree
                    #print("running with ElementTree")
                except ImportError as ex:
                    print("Failed to import ElementTree from any known place")
                    raise ex


class ParsingException(Exception):
    def __init__(self):
        self.what_is_this = False


class ComponentExporter(object):

    def __init__(self, external_packages=None, export_icons=False, msl_version='3.2'):
        """
        Creates a new instance of ComponentExporter. Opens up a OMCSession and loads
        all necessary Modelica packages.
        """

        self.external_package_paths = make_paths_safe_for_omc(external_packages)

        self.logger = logging.getLogger('py_modelica_exporter.ComponentExporter')
        self.logger.setLevel(logging.NOTSET)

        self.logger.info('Initializing ComponentExporter({0})'.format(external_packages))

        # start om session
        self.omc = OMCSession()

        # load all packages
        self.loadPackages(self.external_package_paths)

        self.export_icons = export_icons

        if self.export_icons:
            icon_dir_name = 'Icons'
            if os.path.isdir(icon_dir_name) == False:
                os.mkdir(icon_dir_name)
            self.icon_exporter = IconExporter(self.omc, icon_dir_name)

    def loadPackages(self, external_package_paths):

        self.omc.loadModel('Modelica')

        for package_path in external_package_paths:
            if os.path.isfile(package_path) == True:                        # make sure the file exists
                if self.omc.loadFile(package_path) == True:         # try to load the package file
                    self.logger.info('Library loaded from : {0}'.format(package_path))
                else:
                    self.logger.warning('Failed to load: {0}'.format(package_path))
            else:
                self.logger.warning('File does not exist! Failed to load: {0}'.format(package_path))

    def get_component_json(self, modelica_uri):
        """
        Extracts component and returns a dictionary
        """
        components = []
        json_result = {'exportedComponentClass': modelica_uri,
                       'icon_path': None,
                       'components': []}

        if self.export_icons and os.path.isdir('Icons'):
            json_result['icon_path'] = self.icon_exporter.export_icon(modelica_uri)

        try:
            self.extract_component_content(modelica_uri, components)
            [json_result['components'].append(component.json()) for component in components]

        except Exception as exception:
            component = Component()
            component.full_name = 'Exception'
            component.comment = exception.message
            json_result['components'].append(component.json())
            self.logger.info('Could not get information for {0}'.format(modelica_uri))

        return json_result

    def get_icon_only(self, modelica_uri):

        if self.export_icons and os.path.isdir('Icons'):
            self.icon_exporter.export_icon(modelica_uri)

    def get_component_xml(self, modelica_uri):
        """
        Extracts component and returns a xml tree
        """
        components =[]
        self.extract_component_content(modelica_uri, components)

        xml_result = etree.Element("Root")
        xml_result.set("ExportedComponent", modelica_uri)
        [xml_result.append(xml_node.xml()) for xml_node in components]

        return xml_result

    def extract_component_content(self, modelica_uri, components):
        """
        Recursively populates components with extracted_components,
        starting from modelica_uri and goes through the extends.
        """
        component = Component()
        component.full_name = modelica_uri
        component.comment = self.omc.getClassComment(modelica_uri)

        components.append(component)

        try:
            #mo_packages = self.omc.getPackages(modelica_uri)
            mo_replaceables = self.omc.getReplaceables(modelica_uri)
            mo_extends_packages = self.omc.getPackagesFromExtends(modelica_uri)

            for replaceable_package in mo_replaceables:
                package = Package()
                package.name = replaceable_package['name']
                package.value = replaceable_package['value']
                component.packages.append(package)

            for extends_class, extends_package in mo_extends_packages.iteritems():
                package = Package()
                package.name = extends_package['name']
                package.value = extends_package['value']
                component.packages.append(package)

        except ValueError as ex:
            if ex.args[0] == 'Could not parse OMC response.':
                self.logger.warning('Could not parse OMC response for getReplaceables({0}) or getPackagesFromExtends()'.format(modelica_uri))
                raise ParsingException

        try:
            mo_components = self.omc.getComponents(modelica_uri)
        except ValueError as ex:
            if ex.args[0] == 'Could not parse OMC response.':
                self.logger.warning('Could not parse OMC response for getComponents({0})'.format(modelica_uri))
                raise ParsingException

        for (mo_type, mo_name, mo_annotation, mo_modification, v1, v2, v3, isReplaceable, component_type, v5, v6, v7) in mo_components:

            if self.omc.isConnector(mo_type):
                connector = Connector()
                connector.full_name = mo_type
                connector.name = mo_name
                connector.description = mo_annotation

                modifier_names = self.omc.getComponentModifierNames(modelica_uri, mo_name)
                modifiers = dict()
                modifiers['modifications'] = mo_modification
                for modifier_name in modifier_names:
                    modifier_value = self.omc.getComponentModifierValue(modelica_uri, '{0}.{1}'.format(mo_name, modifier_name))
                    modifiers[modifier_name] = modifier_value
                    connector.modifiers = modifiers

                if (len(component.packages) != 0) and ('Modelica.Fluid.Interfaces.FluidPort_' in mo_type):
                    portRedeclare = self.omc.getPortRedeclares(modelica_uri, mo_type, mo_name)
                    if portRedeclare:
                        for package in component.packages:
                            if portRedeclare[1] == package.name:
                                redeclare_parameter = RedeclareParameter()
                                redeclare_parameter.name = portRedeclare[0]
                                redeclare_parameter.value = portRedeclare[1]
                                #redeclare_parameter.value = package.value

                                connector.redeclare_parameters.append(redeclare_parameter)
                                break

                component.connectors.append(connector)

            elif component_type == 'parameter':
                parameter = Parameter()
                if mo_modification != 'public':
                    parameter.is_public = False

                parameter.name = mo_name
                parameter.description = mo_annotation
                parameter.full_name = mo_type
                parameter.value = self.omc.getParameterValue(modelica_uri, mo_name)

                value_type = type(parameter.value)

                if value_type == tuple:
                    parameter.dimension = len(parameter.value)  # log the length of the tuple
                elif value_type == str:
                    if parameter.value == '':
                        parameter.dimension = 0  # 0 indicates an empty string
                    elif '[' in parameter.value:
                        semicolon_split = parameter.value.split(';')
                        if len(semicolon_split) > 1:
                            # this means a table with dimension (n,m); should we actually log the values? or only '-1'?
                            parameter.dimension = -1
                        else:
                            # if there are no semicolons, then this is a 1-d array (?)
                            comma_split = parameter.value.split(',')
                            parameter.dimension = len(comma_split)

                for modifier_name in self.omc.getComponentModifierNames(modelica_uri, mo_name):
                    modifier_value = self.omc.getComponentModifierValue(modelica_uri, '{0}.{1}'.format(mo_name, modifier_name))
                    parameter.modifiers.update({modifier_name: modifier_value})

                if self.omc.isType(mo_type):
                    parameter.full_name = get_parameter_base_type_and_modifiers(self.omc, mo_type, parameter.modifiers)

                component.parameters.append(parameter)

            else:
                pass # the object is not a connector or a parameter

        mo_inheritance_count = self.omc.getInheritanceCount(modelica_uri)

        for i in range(1, mo_inheritance_count + 1):
            extend = Extend()
            mo_extend_class_name = self.omc.getNthInheritedClass(modelica_uri, i)

            if mo_extend_class_name in mo_extends_packages:
                redeclare_parameter = RedeclareParameter()
                redeclare_parameter.name = mo_extends_packages[mo_extend_class_name]['name']
                redeclare_parameter.redeclare_type = 'package'
                redeclare_parameter.value = mo_extends_packages[mo_extend_class_name]['value']
                extend.redeclare_parameters.append(redeclare_parameter)

            mo_extend_class_modifiers = self.omc.getExtendsModifierNames(modelica_uri, mo_extend_class_name)

            extend.modifiers = {}

            for mo_extend_class_modifier in mo_extend_class_modifiers:
                if '.' in mo_extend_class_modifier:
                    # this is a modifier
                    extend.modifiers[mo_extend_class_modifier] = self.omc.getExtendsModifierValue(modelica_uri, mo_extend_class_name, mo_extend_class_modifier)
                else:
                    # this is a parameter
                    extend_parameter = Parameter()
                    extend_parameter.name = mo_extend_class_modifier
                    extend_parameter.value = self.omc.getExtendsModifierValue(modelica_uri, mo_extend_class_name, mo_extend_class_modifier)
                    extend.parameters.append(extend_parameter)

            extend.full_name = mo_extend_class_name

            component.extends.append(extend)

            self.extract_component_content(mo_extend_class_name, components)

        # TODO: Shouldn't this be done for all components?
        mo_import_count = self.omc.getImportCount(modelica_uri)
        for i in range(1, mo_import_count + 1):
            import_item = Import()

            mo_import = self.omc.getNthImport(modelica_uri, i)

            import_item.full_name = mo_import[0]
            import_item.id = mo_import[1]
            import_item.kind = mo_import[2]

            component.imports.append(import_item)


class TreeExporter(object):

    def __init__(self, className):
        self.classNames = list()
        self.classNames.append(className)
        self.classDetails = list()

        self.logger = logging.getLogger('py_modelica_exporter.TreeExporter')
        self.logger.setLevel(logging.NOTSET)

        self.logger.info('Initializing TreeExporter({0})'.format(className))

        self.omc = OMCSession()

        # load all packages
        success = self.omc.loadModel('Modelica')
        # TODO: load all external packages

        self.parse_tree(className)

    def parse_tree(self, className):

        classNames = self.omc.getClassNames(className, recursive=True, sort=True)

        # filter and export only blocks and models
        for c_name in classNames:
            if (self.omc.isModel(c_name)) or (self.omc.isBlock(c_name)):
                class_details = dict()
                class_details['ComponentName'] = c_name
                class_details['Description'] = self.omc.getClassComment(c_name)
                self.classDetails.append(class_details)

    def json(self):
        json_result = dict()
        json_result['topLevelPackages'] = self.classNames
        json_result['classDetails'] = self.classDetails
        return json_result

    def xml(self):
        raise NotImplementedError

    def export_to_json(self, filename):
        json_result = self.json()

        with open(filename, 'w') as f_p:
            json.dump(json_result, f_p, indent=4)

        return json_result

    def export_to_xml(self, filename):
        raise NotImplementedError


class PackageExporter(object):

    def __init__(self, external_packages, load_MSL=True):

        self.externalPackagePaths = make_paths_safe_for_omc(external_packages)
        self.packageNames = list()
        self.failedLoadPackageNames = list()
        self.classDetails = list()

        self.logger = logging.getLogger('py_modelica_exporter.PackageExporter')
        self.logger.setLevel(logging.NOTSET)
        self.logger.info('Initializing PackageExporter({0})'.format(self.externalPackagePaths))

        self.omc = OMCSession()
        self.loadPackages(self.externalPackagePaths, load_MSL=load_MSL)
        self.getClassDetails(self.packageNames)

    def parseArgument(self, externalPackages):
        # this will update self.externalPackagePaths and self.externalPackageNames

        for potentialPath in externalPackages:
            if os.path.exists(potentialPath):
                self.externalPackagePaths.append(potentialPath)

                if os.path.basename(potentialPath) == 'package.mo':     # check if the file name is 'package.mo'
                    packageDir = os.path.dirname(potentialPath)         # get the path to the last directory
                    packageName = os.path.basename(packageDir)          # get the name of the last directory
                    self.packageNames.append(packageName)       # store that name
                else:
                    file_path, file_extension = os.path.splitext(potentialPath)
                    if file_extension == '.mo':                         # make sure it is a '.mo' file
                        packageName = os.path.basename(file_path)       # get the name of the file
                        self.packageNames.append(packageName)   # store that name
            else:
                pass  # should log that this package path is invalid?

    def loadPackages(self, external_package_paths, load_MSL=True):

        if load_MSL:
            if self.omc.loadModel('Modelica') == True:
                comment = self.omc.getClassComment('Modelica')

                import re
                modelica_version_pattern = '.*Version ([\d\.]*)'
                regex_modelica_version = re.findall(modelica_version_pattern, comment)

                if regex_modelica_version[0]:
                    self.packageNames.append('Modelica ' + regex_modelica_version[0])
                else:
                    self.packageNames.append('Modelica')

        for package_path in external_package_paths:
            if os.path.isfile(package_path) == True:                # make sure the file exists
                if os.path.basename(package_path) == 'package.mo':  # check if the file name is 'package.mo'
                    package_dir = os.path.dirname(package_path)     # get the path to the last directory
                    package_name = os.path.basename(package_dir)    # get the name of the last directory (package_name)
                    if self.omc.loadFile(package_path) == True:     # try to load the package file
                        self.packageNames.append(package_name)  # log successful load
                        self.logger.info('Library loaded from : {0}'.format(package_path))
                    else:
                        self.failedLoadPackageNames.append("FAILED_" + package_name)    # log failure
                        self.logger.warning('Failed to load: {0}'.format(package_path))
                else:
                    file_path, file_extension = os.path.splitext(package_path)
                    if file_extension == '.mo':                             # make sure it is a '.mo' file
                        package_name = os.path.basename(file_path)          # get the name of the file
                        if self.omc.loadFile(package_path) == True:         # try to load the package file
                            self.packageNames.append(package_name)  # log successful load
                            self.logger.info('Library loaded from : {0}'.format(package_path))
                        else:
                            self.failedLoadPackageNames.append("FAILED_" + package_name)    # log failure
                            self.logger.warning('Failed to load: {0}'.format(package_path))
            else:
                file_path, file_extension = os.path.splitext(package_path)
                if file_extension == '.mo':                            # make sure it is a '.mo' file
                    package_name = os.path.basename(file_path)          # get the name of the file
                    if package_name == 'package':
                        package_name = os.path.basename(os.path.dirname(file_path))

                    self.failedLoadPackageNames.append(package_name)
                    self.logger.warning('Failed to load: {0}'.format(package_path))

    def getClassDetails(self, packageNames):

        for packageName in packageNames:
            if 'Modelica' in packageName:
                classNames = self.omc.getClassNames('Modelica', recursive=True, sort=True)
            else:
                classNames = self.omc.getClassNames(packageName, recursive=True, sort=True)

            for c_name in classNames:
                if (self.omc.isModel(c_name)) or (self.omc.isBlock(c_name)): # only blocks and models are exported
                    class_details = dict()
                    class_details['ComponentName'] = c_name
                    class_details['Description'] = self.omc.getClassComment(c_name)
                    self.classDetails.append(class_details)

    def json(self):
        json_result = dict()
        json_result['topLevelPackages'] = self.packageNames + self.failedLoadPackageNames
        json_result['classDetails'] = self.classDetails
        return json_result

    def exportToJson(self, filename):
        json_result = self.json()

        with open(filename, 'w') as f_p:
            json.dump(json_result, f_p, indent=4)

        return json_result


class ComponentAssemblyExporter(object):

    def __init__(self, external_packages=None, msl_version='3.2'):
        self.logger = logging.getLogger('py_modelica_exporter::ComponentExporter')
        self.logger.setLevel(logging.DEBUG)
        # create console handler with a higher log level
        self.logger_console_handler = logging.StreamHandler()
        self.logger_console_handler.setLevel(logging.INFO)

        # create formatter and add it to the handlers
        self.logger_formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        self.logger_console_handler.setFormatter(self.logger_formatter)

        # add the handlers to the logger
        self.logger.addHandler(self.logger_console_handler)

        # start om session
        self.omc = OMCSession()
        # start om session
        self.omc = OMCSession()

        # load all packages
        self.omc.loadModel('Modelica, {{"{0}"}}'.format(msl_version))
        self.logger.info('Modelica {0} loaded.'.format(msl_version))

        if external_packages:
            self.load_external_packages(external_packages)

        self.logger.debug('ComponentAssemblyExporter __init__ finished.')

    def load_external_packages(self, external_packages):

        for package_path in (os.path.abspath(p.strip()).replace("\\", "/") for p in external_packages):
            file_name = os.path.basename(package_path)

            if file_name == 'package.mo':
                package_name = os.path.basename(os.path.dirname(package_path))
            else:
                package_name, dot_mo = os.path.splitext(file_name)

            if self.omc.loadFile(package_path):
                self.logger.info('Loaded {0} at {1}'.format(package_name, package_path))
            else:
                self.logger.error('Failed loading {0} from {1}!'.format(package_name, package_path))

    def get_component_assembly_json(self, modelica_uri):

        ca = ComponentAssembly()
        ca.name = modelica_uri.split('.')[-1]
        ca.full_name = modelica_uri
        try:
            mo_components = self.omc.getComponents(modelica_uri)
        except ValueError as ex:
            if ex.args[0] == 'Could not parse OMC response.':
                raise ParsingException

        for (mo_type, mo_name, mo_annotation, mo_modification, v5, v6, v7, isReplaceable, component_type, v10, v11, v12) in mo_components:

            if self.omc.isModel(mo_type) or self.omc.isBlock(mo_type):
                if v10 in ['inner', 'outer']:
                    self.logger.debug('Skipping {0} - it is {1}'.format(mo_name, v10))
                    continue
                component_shell = ComponentShell()
                component_shell.name = mo_name
                ca.component_shells.update({mo_name: component_shell})
            elif self.omc.isConnector(mo_type):
                connector = Connector()
                connector.full_name = mo_type
                connector.name = mo_name
                connector.description = mo_annotation
                ca.connectors.append(connector)

        for n in range(1, self.omc.getConnectionCount(modelica_uri) + 1):
            src, dst, v3 = self.omc.getNthConnection(modelica_uri, n)
            connection = Connection()
            src_name, src_parent = self._get_connector_and_parent(src)
            dst_name, dst_parent = self._get_connector_and_parent(dst)

            try:
                if src_parent:
                    connection.src_parent = src_parent
                    component_shell = ca.component_shells[src_parent]
                    assert isinstance(component_shell, ComponentShell)
                    connector = Connector()
                    connector.name = src_name
                    component_shell.connectors.append(connector)
                if dst_parent:
                    connection.dst_parent = dst_parent
                    component_shell = ca.component_shells[dst_parent]
                    assert isinstance(component_shell, ComponentShell)
                    connector = Connector()
                    connector.name = src_name
                    component_shell.connectors.append(connector)
            except KeyError as err:
                self.logger.debug('Skipping connections to {0}'.format(err.message))
                continue

            connection.src_name = src_name
            connection.dst_name = dst_name
            ca.connections.append(connection)

        return ca.json()

    def _get_connector_and_parent(self, connected):
        pieces = connected.split('.')
        parent = ""
        if len(pieces) == 2:
            parent = pieces[0]
            connector = pieces[1]
        else:
            connector = connected

        return connector, parent


def get_parameter_base_type_and_modifiers(omc, class_type, modifiers):
    # Assumption Types can only inherit from one class (at a time)
    base_type = class_type
    # Extract inherited modifiers
    for i in range(1, omc.getInheritanceCount(class_type) + 1):
        base_type = omc.getNthInheritedClass(class_type, i)
        for mod_name in omc.getExtendsModifierNames(class_type, base_type):
            if mod_name not in modifiers:
                value = omc.getExtendsModifierValue(class_type, base_type, mod_name)
                modifiers.update({mod_name: value})
        # recursion
        if omc.isType(base_type):
            base_type = get_parameter_base_type_and_modifiers(omc, base_type, modifiers)

    return base_type

def make_paths_safe_for_omc(path_list):

    python_version_major = sys.version_info[0]

    safe_paths = list()

    for path in path_list:
        path.strip()

        if python_version_major >= 3:
            omc_safe_path = path.encode('unicode-escape').replace('\\\\', '/').replace('\\', '/')
            safe_paths.append(omc_safe_path)
        else:
            omc_safe_path = path.encode('string-escape').replace('\\\\', '/').replace('\\', '/')
            safe_paths.append(omc_safe_path)

    return safe_paths
