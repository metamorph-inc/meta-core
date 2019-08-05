from __future__ import absolute_import
from __future__ import print_function
import six
from six.moves import map

__author__ = 'Zsolt'

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


class Import(object):
    """
    A Python wrapper for the Modelica 'Import' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.full_name = None
        self.id = None
        self.kind = None

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['fullName'] = self.full_name
        json_result['id'] = self.id
        json_result['kind'] = self.kind

        return json_result

    def xml(self):
        """
        return an xml representation

        """
        xml_result = etree.Element("Import")
        xml_result.set("FullName", self.full_name)
        xml_result.set("Id", self.id)
        xml_result.set("Kind", self.kind)

        return xml_result


class Parameter(object):
    """
    A Python wrapper for the Modelica 'Parameter' type

    """

    def __init__(self):
        """
        Create an instance

        """

        self.name = None
        self.full_name = None
        self.value = None
        self.description = ''
        self.modifiers = {}
        self.is_public = True
        self.dimension = 1  # '0' indicates empty string, -1 indicates table

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['name'] = self.name
        json_result['fullName'] = self.full_name
        json_result['value'] = self.value
        json_result['description'] = self.description
        json_result['modifiers'] = self.modifiers
        json_result['isPublic'] = self.is_public
        json_result['dimension'] = self.dimension

        return json_result

    def xml(self):
        """
        return an xml representation

        """

        xml_result = etree.Element("Parameter")
        xml_result.set("Name", self.name)
        xml_result.set("FullName", self.full_name)
        xml_result.set("Value", str(self.value))
        xml_result.set("Description", self.description)
        # [xml_result.append(xml_node.xml()) for xml_node in self.modifiers]

        return xml_result


class RedeclareParameter(Parameter):
    """
    A Python wrapper for the Modelica 'RedeclareParameter' type

    """

    def __init__(self):
        """
        Create an instance

        """

        super(RedeclareParameter, self).__init__()
        self.redeclare_type = None
        self.modifiers = {}

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['redeclare_type'] = self.redeclare_type
        json_result['modifiers'] = self.modifiers
        json_result['name'] = self.name
        json_result['value'] = self.value

        return json_result

    def xml(self):
        """
        return an xml representation

        """
        xml_result = etree.Element("RedeclareParameter")
        xml_result.set("RedeclareType", self.redeclare_type)

        # [xml_result.append(xml_node.xml()) for xml_node in self.modifiers]

        return xml_result


class Extend(object):
    """
    A Python wrapper for the Modelica 'Extend' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.full_name = None
        self.parameters = list()  # Parameter
        self.redeclare_parameters = list()  # RedeclareParameter
        self.modifiers = {}

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['fullName'] = self.full_name
        json_result['parameters'] = [json_value.json() for json_value in self.parameters]
        json_result['redeclare_parameters'] = [json_value.json() for json_value in self.redeclare_parameters]
        json_result['modifiers'] = self.modifiers

        return json_result

    def xml(self):
        """
        return an xml representation

        """
        xml_result = etree.Element("Extend")
        xml_result.set("FullName", self.full_name)

        [xml_result.append(xml_node.xml()) for xml_node in self.parameters]
        [xml_result.append(xml_node.xml()) for xml_node in self.redeclare_parameters]
        # [xml_result.append(xml_node.xml()) for xml_node in self.modifiers]

        return xml_result


class Connector(object):
    """
    A Python wrapper for the Modelica 'Connector' type

    """

    def __init__(self):
        """
        Create an instance

        """

        self.name = None
        self.full_name = None
        self.description = ''
        self.parameters = list()  # Parameter
        self.redeclare_parameters = list()  # RedeclareParameter
        self.modifiers = {}
        self.variables = []
        self.connectors = []

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['name'] = self.name
        json_result['fullName'] = self.full_name
        json_result['parameters'] = [json_value.json() for json_value in self.parameters]
        json_result['redeclare_parameters'] = [json_value.json() for json_value in self.redeclare_parameters]
        json_result['modifiers'] = self.modifiers
        json_result['variables'] = self.variables
        json_result['connectors'] = list(map(Connector.json, self.connectors))

        return json_result

    def xml(self):
        """
        return an xml representation

        """
        xml_result = etree.Element("Connector")
        xml_result.set("Name", self.name)
        xml_result.set("FullName", self.full_name)

        [xml_result.append(xml_node.xml()) for xml_node in self.parameters]
        [xml_result.append(xml_node.xml()) for xml_node in self.redeclare_parameters]
        # [xml_result.append(xml_node.xml()) for xml_node in self.modifiers]

        return xml_result


class Package(object):
    """
    A Python wrapper for the Modelica 'Package' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.value = None
        self.name = None

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['name'] = self.name
        json_result['value'] = self.value

        return json_result


class Component(object):
    """
    A Python wrapper for the Modelica 'Component' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.full_name = None
        self.comment = None
        self.type = None
        self.parameters = list()  # Parameter
        self.redeclare_parameters = list()  # RedeclareParameter
        self.connectors = list()  # Connector
        self.extends = list()  # Extends
        self.imports = list()
        self.components = []

        # JK - need some feedback from Z/P
        self.packages = list()

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['fullName'] = self.full_name
        json_result['comment'] = self.comment
        json_result['type'] = self.type
        json_result['parameters'] = [json_value.json() for json_value in self.parameters]
        json_result['redeclare_parameters'] = [json_value.json() for json_value in self.redeclare_parameters]
        json_result['connectors'] = [json_value.json() for json_value in self.connectors]
        json_result['extends'] = [json_value.json() for json_value in self.extends]
        json_result['imports'] = [json_value.json() for json_value in self.imports]
        json_result['components'] = [json_value.json() for json_value in self.components]
        # JK - need some feedback from Z/P
        json_result['packages'] = [json_value.json() for json_value in self.packages]

        return json_result

    def xml(self):
        """
        return an xml representation

        """
        xml_result = etree.Element("Component")
        xml_result.set("FullName", self.full_name)

        [xml_result.append(xml_node.xml()) for xml_node in self.parameters]
        [xml_result.append(xml_node.xml()) for xml_node in self.redeclare_parameters]
        [xml_result.append(xml_node.xml()) for xml_node in self.connectors]
        [xml_result.append(xml_node.xml()) for xml_node in self.extends]
        [xml_result.append(xml_node.xml()) for xml_node in self.imports]

        return xml_result


class ComponentAssembly(object):
    """
    A Python wrapper for the Modelica 'ComponentAssembly' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.name = None
        self.full_name = None
        self.comment = None
        self.connectors = list()  # Connector
        self.connections = list()
        self.component_shells = {}

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['name'] = self.name
        json_result['fullName'] = self.full_name
        json_result['comment'] = self.comment
        json_result['connectors'] = [json_value.json() for json_value in self.connectors]
        json_result['connections'] = [json_value.json() for json_value in self.connections]
        json_result['component_shells'] = [{k: v.json()} for k, v in six.iteritems(self.component_shells)]

        return json_result


class ComponentShell(object):
    """
    A Python wrapper for the Modelica 'ComponentShell' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.name = None
        self.connectors = list()

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['name'] = self.name
        json_result['connectors'] = [json_value.json() for json_value in self.connectors]

        return json_result


class Connection(object):
    """
    A Python wrapper for the Modelica 'Connection' type

    """

    def __init__(self):
        """
        Create an instance

        """
        self.src_parent = ""
        self.src_name = ""
        self.dst_parent = ""
        self.dst_name = ""

    def json(self):
        """
        return an dictionary representation

        """
        json_result = dict()
        json_result['src_parent'] = self.src_parent
        json_result['src_name'] = self.src_name
        json_result['dst_parent'] = self.dst_parent
        json_result['dst_name'] = self.dst_name

        return  json_result
