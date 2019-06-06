from __future__ import absolute_import
from __future__ import print_function
import six
from six.moves import range

__author__ = 'Zsolt'

import logging
import json
import os
import sys
import collections

from .omc_session import OMCSession
from .modelica_classes import Connector, Extend, Parameter, RedeclareParameter, Package, Import, Connection, ComponentAssembly, Component, ComponentShell

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

ModelicaComponent = collections.namedtuple('ModelicaComponent', ('mo_type', 'mo_name', 'mo_annotation', 'mo_modification', 'v_1',
    'v_2', 'v_3', 'isReplaceable', 'component_type', 'v_5', 'v_6', 'v_7'))


class ParsingException(Exception):
    """
    Special exception to be used/handled in this module only
    """
    def __init__(self):
        """
        Create an instance of ParsingException
        """
        self.dummy_variable = False


class ComponentExporter(object):
    """
    Class ComponentExporter is an interface with the OpenModelicaCompiler to facilitate extracting class information
    in a single 'Component' format
    """
    def __init__(self, external_packages=None, export_icons=False, msl_version='3.2'):
        """
        Creates a new instance of ComponentExporter. Opens up a OMCSession and loads
        all necessary Modelica packages.
        """

        self.external_package_paths = make_paths_safe_for_omc(external_packages)

        self.logger = logging.getLogger('py_modelica_exporter.ComponentExporter')
        self.logger.setLevel(logging.NOTSET)

        self.logger.info('Initializing ComponentExporter({0})'.format(external_packages))
        self.connectors = {}

        # start om session
        self.omc = OMCSession()

        # load all packages
        self.load_packages(self.external_package_paths)

        self.export_icons = export_icons

        if self.export_icons:
            icon_dir_name = 'Icons'
            if not os.path.isdir(icon_dir_name):
                os.mkdir(icon_dir_name)
            from .generate_icons import IconExporter
            self.icon_exporter = IconExporter(self.omc, icon_dir_name)

    def load_packages(self, external_package_paths):
        """
        load all external packages into the OpenModelicaCompiler so individual classes may be explored

        @param external_package_paths:
        @return:
        """

        self.omc.loadModel('Modelica')

        for package_path in external_package_paths:
            if os.path.isfile(package_path):                        # make sure the file exists
                if self.omc.loadFile(package_path):         # try to load the package file
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
            component.comment = getattr(exception, 'message', 'unknown error')
            json_result['components'].append(component.json())
            self.logger.info('Could not get information for {0}'.format(modelica_uri))

        return json_result

    def get_icon_only(self, modelica_uri):
        """
        Extracts the icon from the Modelica class
        """
        if self.export_icons and os.path.isdir('Icons'):
            self.icon_exporter.export_icon(modelica_uri)

    def get_component_xml(self, modelica_uri):
        """
        Extracts component and returns a xml tree
        """
        components = []
        self.extract_component_content(modelica_uri, components)

        xml_result = etree.Element("Root")
        xml_result.set("ExportedComponent", modelica_uri)
        [xml_result.append(xml_node.xml()) for xml_node in components]

        return xml_result

    def create_connector(self, modelica_uri, c):
        connector = Connector()
        connector.full_name = c.mo_type
        connector.name = c.mo_name
        connector.description = c.mo_annotation

        modifier_names = self.omc.getComponentModifierNames(modelica_uri, c.mo_name)
        modifiers = dict()
        modifiers['modifications'] = c.mo_modification
        for modifier_name in modifier_names:
            modifier_value = self.omc.getComponentModifierValue(modelica_uri,
                                                                '{0}.{1}'.format(c.mo_name, modifier_name))
            modifiers[modifier_name] = modifier_value
            connector.modifiers = modifiers

        for connector_component in self.omc.getComponents(c.mo_type):
            connector_component = ModelicaComponent(*connector_component)
            if self.omc.isConnector(connector_component.mo_type):
                connector.connectors.append(self.create_connector(c.mo_type, connector_component))
            elif c.component_type == 'parameter':
                connector.parameters.append(self.create_parameter(c.mo_type, connector_component))
            else:
                connector.variables.append({'type': connector_component.mo_type, 'name': connector_component.mo_name})
        return connector

    def create_parameter(self, modelica_uri, c):
        parameter = Parameter()
        if c.mo_modification != 'public':
            parameter.is_public = False

        parameter.name = c.mo_name
        parameter.description = c.mo_annotation
        parameter.full_name = c.mo_type
        parameter.value = self.omc.getParameterValue(modelica_uri, c.mo_name)

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

        for modifier_name in self.omc.getComponentModifierNames(modelica_uri, c.mo_name):
            modifier_value = self.omc.getComponentModifierValue(modelica_uri,
                                                                '{0}.{1}'.format(c.mo_name, modifier_name))
            parameter.modifiers.update({modifier_name: modifier_value})

        if self.omc.isType(c.mo_type):
            parameter.full_name = get_parameter_base_type_and_modifiers(self.omc, c.mo_type, parameter.modifiers)
        return parameter

    def extract_component_content(self, modelica_uri, components):
        """
        Recursively populates components with extracted_components,
        starting from modelica_uri and goes through the extends.
        """
        component = Component()
        component.full_name = modelica_uri
        component.comment = self.omc.getClassComment(modelica_uri)
        component.type = self.omc.getClassInformation(modelica_uri)[0]

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

            for extends_class, extends_package in six.iteritems(mo_extends_packages):
                package = Package()
                package.name = extends_package['name']
                package.value = extends_package['value']
                component.packages.append(package)

        except ValueError as value_error_exception:
            if value_error_exception.args[0] == 'Could not parse OMC response.':
                e_msg = 'Could not parse OMC response for getReplaceables({0}) or getPackagesFromExtends()'.format(
                    modelica_uri)
                self.logger.warning(e_msg)
                raise ParsingException

        try:
            mo_components = self.omc.getComponents(modelica_uri)
        except ValueError as value_error_exception:
            if value_error_exception.args[0] == 'Could not parse OMC response.':
                self.logger.warning('Could not parse OMC response for getComponents({0})'.format(modelica_uri))
                raise ParsingException()
        if mo_components == 'Error':
            self.logger.warning('OMC error for getComponents({0})'.format(modelica_uri))
            raise ParsingException()
        for c in mo_components:
            c = ModelicaComponent(*c)

            if self.omc.isConnector(c.mo_type):
                #     pass
                # elif False:
                connector = self.create_connector(modelica_uri, c)

                if (len(component.packages) != 0) and ('Modelica.Fluid.Interfaces.FluidPort_' in c.mo_type):
                    port_redeclare = self.omc.getPortRedeclares(modelica_uri, c.mo_type, c.mo_name)
                    if port_redeclare:
                        for package in component.packages:
                            if port_redeclare[1] == package.name:
                                redeclare_parameter = RedeclareParameter()
                                redeclare_parameter.name = port_redeclare[0]
                                redeclare_parameter.value = port_redeclare[1]
                                # redeclare_parameter.value = package.value

                                connector.redeclare_parameters.append(redeclare_parameter)
                                break

                component.connectors.append(connector)

            elif c.component_type == 'parameter':
                parameter = self.create_parameter(modelica_uri, c)
                component.parameters.append(parameter)

            else:
                pass  # the object is not a connector or a parameter

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

            for extend_class_modifier in mo_extend_class_modifiers:
                if '.' in extend_class_modifier:
                    # this is a modifier
                    extend.modifiers[extend_class_modifier] = self.omc.getExtendsModifierValue(modelica_uri,
                                                                                               mo_extend_class_name,
                                                                                               extend_class_modifier)
                else:
                    # this is a parameter
                    extend_parameter = Parameter()
                    extend_parameter.name = extend_class_modifier
                    extend_parameter.value = self.omc.getExtendsModifierValue(modelica_uri, mo_extend_class_name,
                                                                              extend_class_modifier)
                    extend.parameters.append(extend_parameter)

            extend.full_name = mo_extend_class_name

            component.extends.append(extend)

            self.extract_component_content(mo_extend_class_name, components)

        mo_import_count = self.omc.getImportCount(modelica_uri)
        for i in range(1, mo_import_count + 1):
            import_item = Import()

            mo_import = self.omc.getNthImport(modelica_uri, i)

            import_item.full_name = mo_import[0]
            import_item.id = mo_import[1]
            import_item.kind = mo_import[2]

            component.imports.append(import_item)


class TreeExporter(object):
    """
    Class TreeExporter is an interface with the OpenModelicaCompiler to facilitate extraction of all contained classes
    """

    def __init__(self, class_name):
        """
        creates an instance of TreeExporter
        """
        self.class_names = list()
        self.class_names.append(class_name)
        self.class_details = list()

        self.logger = logging.getLogger('py_modelica_exporter.TreeExporter')
        self.logger.setLevel(logging.NOTSET)

        self.logger.info('Initializing TreeExporter({0})'.format(class_name))

        self.omc = OMCSession()

        # load all packages
        success = self.omc.loadModel('Modelica')

        self.parse_tree(class_name)

    def parse_tree(self, class_name):
        """
        gets all classes from within 'class_name'
        """

        class_names = self.omc.getClassNames(class_name, recursive=True, sort=True)

        # filter and export only blocks and models
        for c_name in class_names:
            # if (self.omc.isModel(c_name)) or (self.omc.isBlock(c_name)):
            class_details = dict()
            class_details['ComponentName'] = c_name
            class_details['Description'] = "None available"  # self.omc.getClassComment(c_name)
            self.class_details.append(class_details)

    def json(self):
        """
        Creates a dictionary from of all the classes contained in the Tree
        """
        json_result = dict()
        json_result['topLevelPackages'] = self.class_names
        json_result['classDetails'] = self.class_details
        return json_result

    def xml(self):
        raise NotImplementedError

    def export_to_json(self, filename):
        """
        Write out a json representation of the Tree
        """
        json_result = self.json()

        with open(filename, 'w') as f_p:
            json.dump(json_result, f_p, indent=4)

        return json_result

    def export_to_xml(self, filename):
        raise NotImplementedError


class PackageExporter(object):
    """
    Class PackageExporter is an interface with the OpenModelicaCompiler to facilitate extraction of all classes
    contained within 'external_packages' (and the MSL)
    """

    def __init__(self, external_packages, load_msl=True):
        """
        Create an instance of the PackageExporter
        """
        self.external_package_paths = make_paths_safe_for_omc(external_packages)
        self.package_names = list()
        self.failed_load_package_names = list()
        self.class_details = list()

        self.logger = logging.getLogger('py_modelica_exporter.PackageExporter')
        self.logger.setLevel(logging.NOTSET)
        self.logger.info('Initializing PackageExporter({0})'.format(self.external_package_paths))

        self.omc = OMCSession()
        self.load_packages(self.external_package_paths, load_msl=load_msl)
        self.get_class_details(self.package_names)

    def parse_argument(self, external_packages):
        """
        This will update self.external_package_paths and self.package_names
        """
        for potential_path in external_packages:
            if os.path.exists(potential_path):
                self.external_package_paths.append(potential_path)

                if os.path.basename(potential_path) == 'package.mo':     # check if the file name is 'package.mo'
                    package_dir = os.path.dirname(potential_path)         # get the path to the last directory
                    package_name = os.path.basename(package_dir)          # get the name of the last directory
                    self.package_names.append(package_name)       # store that name
                else:
                    file_path, file_extension = os.path.splitext(potential_path)
                    if file_extension == '.mo':                         # make sure it is a '.mo' file
                        package_name = os.path.basename(file_path)       # get the name of the file
                        self.package_names.append(package_name)   # store that name
            # else:
            #     pass  # should log that this package path is invalid?

    def load_packages(self, external_package_paths, load_msl=True):
        """
        load all the external packages using OpenModelicaCompiler
        """

        if load_msl:
            if self.omc.loadModel('Modelica'):
                comment = self.omc.getClassComment('Modelica')

                import re

                modelica_version_pattern = '.*Version ([\d\.]*)'
                regex_modelica_version = re.findall(modelica_version_pattern, comment)

                if regex_modelica_version[0]:
                    self.package_names.append('Modelica ' + regex_modelica_version[0])
                else:
                    self.package_names.append('Modelica')

        for package_path in external_package_paths:
            if os.path.isfile(package_path):                # make sure the file exists
                if os.path.basename(package_path) == 'package.mo':  # check if the file name is 'package.mo'
                    package_dir = os.path.dirname(package_path)     # get the path to the last directory
                    package_name = os.path.basename(package_dir)    # get the name of the last directory (package_name)
                    if self.omc.loadFile(package_path):     # try to load the package file
                        self.package_names.append(package_name)  # log successful load
                        self.logger.info('Library loaded from : {0}'.format(package_path))
                    else:
                        self.failed_load_package_names.append("FAILED_" + package_name)    # log failure
                        self.logger.warning('Failed to load: {0}'.format(package_path))
                else:
                    file_path, file_extension = os.path.splitext(package_path)
                    if file_extension == '.mo':                             # make sure it is a '.mo' file
                        package_name = os.path.basename(file_path)          # get the name of the file
                        if self.omc.loadFile(package_path):         # try to load the package file
                            self.package_names.append(package_name)  # log successful load
                            self.logger.info('Library loaded from : {0}'.format(package_path))
                        else:
                            self.failed_load_package_names.append("FAILED_" + package_name)    # log failure
                            self.logger.warning('Failed to load: {0}'.format(package_path))
            else:
                file_path, file_extension = os.path.splitext(package_path)
                if file_extension == '.mo':                            # make sure it is a '.mo' file
                    package_name = os.path.basename(file_path)          # get the name of the file
                    if package_name == 'package':
                        package_name = os.path.basename(os.path.dirname(file_path))

                    self.failed_load_package_names.append(package_name)
                    self.logger.warning('Failed to load: {0}'.format(package_path))

    def get_class_details(self, package_names):
        """
        Get the class details and write out a json file
        """

        for package_name in package_names:
            get_comments = False  # self.omc.getClassNames randomly (?) takes a long time to execute

            if 'Modelica' in package_name:
                class_names = self.omc.getClassNames('Modelica', recursive=True, sort=True)
                # get_comments = True

                # blacklist = ['.Utilities.',
                #              '.UsersGuide.',
                #              '.Examples.',
                #              '.UnitConversions.',
                #              '.Media.']

                # for c_name in class_names:
                #     # skip_it = False
                #     # for bad_name in blacklist:
                #     #     if bad_name in c_name:
                #     #         skip_it = True
                #     #         self.logger.info(c_name + " is black-listed (not a model or block)")
                #     #         break
                #     # if skip_it:
                #     #     continue
                #
                #     class_details = dict()
                #     class_details['ComponentName'] = c_name
                #     class_details['Description'] = ""  # self.omc.getClassComment(c_name)
                #     self.class_details.append(class_details)

            else:
                class_names = self.omc.getClassNames(package_name, recursive=True, sort=True)
                # get_comments = True

            for c_name in class_names:

                class_details = dict()
                class_details['ComponentName'] = c_name
                if get_comments:
                    class_details['Description'] = self.omc.getClassComment(c_name)
                else:
                    class_details['Description'] = "None available"

                self.class_details.append(class_details)

                # model_type = self.omc.getClassRestriction(c_name)
                # if model_type in ['model', 'block']:
                # # if (self.omc.isModel(c_name)) or (self.omc.isBlock(c_name)):  # only blocks and models are exported
                #     class_details = dict()
                #     class_details['ComponentName'] = c_name
                #     class_details['Description'] = self.omc.getClassComment(c_name)
                #     self.class_details.append(class_details)

    def json(self):
        """
        Generate a dictionary from the Packages
        """
        json_result = dict()
        json_result['topLevelPackages'] = self.package_names + self.failed_load_package_names
        json_result['classDetails'] = self.class_details
        return json_result

    def export_to_json(self, filename):
        """
        Write a json file of the Packages
        """
        json_result = self.json()

        with open(filename, 'w') as f_p:
            json.dump(json_result, f_p, indent=4)

        return json_result


class ComponentAssemblyExporter(object):
    """
    Class ComponentAssemblyExporter is an interface with the OpenModelicaCompiler to facilitate
    extracting class information in a 'ComponentAssembly' format
    """

    def __init__(self, external_packages=None, msl_version='3.2'):
        """
        Create an instance of the ComponentAssemblyExporter
        """
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
        """
        Load all the external packages
        """

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
        """
        create a dictionary description of the Modelica class, enumerating all the contained classes
        """
        ca = ComponentAssembly()
        ca.name = modelica_uri.split('.')[-1]
        ca.full_name = modelica_uri
        try:
            mo_components = self.omc.getComponents(modelica_uri)
        except ValueError as value_error_exception:
            if value_error_exception.args[0] == 'Could not parse OMC response.':
                raise ParsingException

        for (mo_type, mo_name, mo_annotation, mo_modification, v5, v6, v7, isReplaceable, component_type, v10, v11,
             v12) in mo_components:

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
        """
        Get the connection details
        """
        pieces = connected.split('.')
        parent = ""
        if len(pieces) == 2:
            parent = pieces[0]
            connector = pieces[1]
        else:
            connector = connected

        return connector, parent


def get_parameter_base_type_and_modifiers(omc, class_type, modifiers):
    """
    Gets all pertinent information from super ('extends') classes

    """
    # Assumption: Types can only inherit from one class (at a time)
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
    """
    this method takes paths defined by the caller and ensures that they are compatible with the OpenModelicaCompiler


    """
    python_version_major = sys.version_info[0]

    safe_paths = list()

    for path in path_list:
        path.strip()

        if python_version_major >= 3:
            omc_safe_path = path.replace('\\\\', '/').replace('\\', '/')
            safe_paths.append(omc_safe_path)
        else:
            omc_safe_path = path.encode('string-escape').replace('\\\\', '/').replace('\\', '/')
            safe_paths.append(omc_safe_path)

    return safe_paths
