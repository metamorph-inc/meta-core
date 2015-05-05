""" Structural Completeness Testbench
    Copyright 2013-2014 The Pennsylvania State University
"""
from xml.etree import ElementTree
import os
import sys
import json
import argparse
import itertools
import uuid
import traceback
import urllib

def id_in_path(id, path):
    for entry in path:
        if entry.id == id:
            return True
    return False

class Node():
    def __init__(self):
        self.parent = None

class Connector(Node):
    def __init__(self, id, name, definition, join_id, connector_composition):
        self.id = id
        self.name = name
        self.definition = definition
        self.join_id = join_id
        self.connector_composition = connector_composition
        self.default_join = None

    def find_endpoints(self, path=[]):
        if self.connector_composition is "" or self.parent is None:
            return {}
        
        path = list(path) # create a clone of the path
        path.append(self)

        result = {}
        ids = [id for id in self.connector_composition.split(" ") if not id_in_path(id, path)]

        # internal connections
        for id in ids:
            for connector in self.parent.connectors_with_id(id):
                result.update(connector.find_endpoints(path))

        # external connections
        if self.parent.parent is not None:
            for id in ids:
                for connector in self.parent.parent.connectors_with_id(id):
                    result.update(connector.find_endpoints(path))

        if len(result) == 0:
            return {self.parent : path}
        else:
            return result

class ConnectorInstance(Node):
    def __init__(self, id, name, component_model_id, join_id, connector_composition):
        self.id = id
        self.name = name
        self.component_model_id = component_model_id
        self.join_id = join_id
        self.connector_composition = connector_composition
        self.default_join = None

    def find_endpoints(self, path=[]):
        if self.connector_composition is "" or self.parent is None or self.parent.parent is None:
            return {}

        path = list(path) # create a clone of the path
        path.append(self)

        result = {}
        ids = [id for id in self.connector_composition.split(" ") if not id_in_path(id, path)]

        for id in ids:
            for connector in self.parent.parent.connectors_with_id(id):
                result.update(connector.find_endpoints(path))

        if len(result) == 0:
            return {self.parent : path}
        else:
            return result

class ComponentInstance(Node):
    def __init__(self, id, name, component_id):
        self.id = id
        self.name = name
        self.component_id = component_id
        self.connectors = []

    def add_connector(self, connector):
        connector.parent = self
        self.connectors.append(connector)

    def connectors_with_id(self, id):
        return [connector for connector in self.connectors if connector.id == id]

class Container(Node):
    def __init__(self, name):
        self.name = name
        self.connectors = []
        self.components = []
        self.containers = []

    def add_connector(self, connector):
        connector.parent = self
        self.connectors.append(connector)

    def add_component(self, component):
        component.parent = self
        self.components.append(component)

    def add_container(self, container):
        container.parent = self
        self.containers.append(container)

    def list_components(self):
        result = []
        result.extend(self.components)

        for container in self.containers:
            result.extend(container.list_components())

        return result

    def connectors_with_id(self, id):
        result = [connector for connector in self.connectors if connector.id == id]

        for component in self.components:
            result.extend([connector for connector in component.connectors if connector.id == id])

        for container in self.containers:
            result.extend([connector for connector in container.connectors if connector.id == id])

        return result

    def components_with_name(self, name):
        result = [component for component in self.components if component.name == name]

        for container in self.containers:
            result.extend(container.components_with_name(name))

        return result

class RootContainer(Container):
    def __init__(self):
        Container.__init__(self, "RootContainer")
        self.join_map = {}
        self.parent = None

    def add_join(self, join_data):
        self.join_map[join_data.id] = join_data

    def lookup_join(self, join_id):
        return self.join_map[join_id]

    def joins_on_path(self, path):
        result = [entry.join_id for entry in path if entry.join_id is not ""]

        if len(result) == 0:
            result = [entry.default_join for entry in path if entry.default_join is not None]

        return result

    def join_warnings_on_path(self, path):
        warnings = []
        join_ids = self.joins_on_path(path)

        if len(join_ids) == 0:
            warnings.append("Connection missing join data")

        for join_id in join_ids:
            if join_id in self.join_map:
                warnings.extend(self.join_map[join_id].get_warnings())
            else:
                warnings.append("References a JoinData element that does not exist")

        return warnings

class JoinData():
    def __init__ (self, join_data):
        self.id = join_data.get("{iFAB}id")
        self.join_data = join_data
        
    def is_complete(self):
        return len(self.get_warnings()) == 0

    def get_warnings(self):
        """ Returns string indicating what caused the invalid join. """
        join_data = self.join_data
        count = 0
        warnings = []

        for element in join_data.iter():
            if element.tag == "mechanical" or element.tag == "brazed" or element.tag == "soldered" or element.tag == "glued" or element.tag == "welded" or element.tag == "incidentalContact":
                count = count + 1
                warnings.extend(self.get_join_warnings(element))

        if count == 0:
            warnings.append("Connection is associated with a JoinData element, but it does not contain any valid join information")

        return warnings

    def get_join_warnings(self, join_data):
        """ Returns string indicating what caused the invalid join. """
        type = join_data.tag
        warnings = []

        if type == "mechanical":
            if join_data.find("fasteningMethod") is None:
                return ["Mechanical join missing fastening method attribute"]
            if join_data.find("fasteningMethod").text not in ["Bolted", "Bolted (blind)", "Machined Screw", "Press Fit", "Snap Fit", "Crimp/Clamp Fit"]:
                warnings.append("Fastening method is not valid (must be Bolted, Bolted (blind), Machined Screw, Press Fit, Snap Fit, or Crimp/Clamp Fit)")
        elif type == "brazed" or type == "soldered":
            if join_data.find("length") is None or join_data.find("fillerMaterial") is None or join_data.find("fluxMaterial") is None:
                return ["Brazed join missing length, filler material, or flux material attributes"]
            if float(join_data.find("length").text) <= 0.0:
                warnings.append("Brazed join has length 0")
        elif type == "glued":
            if join_data.find("length") is None or join_data.find("volume") is None or join_data.find("material") is None:
                return ["Glued join missing length, volume, or material attributes"]
            if float(join_data.find("length").text) <= 0.0 or float(join_data.find("volume").text) <= 0.0:
                warnings.append("Glued join length and/or volume is 0")
        elif type == "welded":
            if join_data.find("length") is None or join_data.find("jointType") is None or join_data.find("weldType") is None or join_data.find("weldPenetration") is None or join_data.find("twoSided") is None or join_data.find("inspectionRequirement") is None or join_data.find("part1Thickness") is None or join_data.find("part1Material") is None or join_data.find("part2Thickness") is None or join_data.find("part2Material") is None:
                return ["Welded join missing length, joint type, weld type, weld penetration, two sided, inspection requirement, or part thicknesses/materials attributes"]
            if float(join_data.find("length").text) <= 0.0:
                warnings.append("Welded join has length 0")
            if join_data.find("jointType").text not in ["Butt", "Corner", "Edge", "Lap", "Tee"]:
                warnings.append("Join type is not valid (must be Butt, Corner, Edge, Lap, or Tee)")
            if join_data.find("weldType").text not in ["Seam", "Stitch", "Spot"]:
                warnings.append("Weld type is not valid (must be Seam, Stitch, or Spot)")
            if join_data.find("weldPenetration").text not in ["Full", "Partial"]:
                warnings.append("Weld penetration is not valid (must be Full or Partial)")
            if join_data.find("inspectionRequirement").text not in ["Visual", "X-Ray"]:
                warnings.append("Inspection requirement is not valid (must be Visual or X-Ray)")
        elif type == "incidentalContact":
            return []
        else:
            return ["Unknown join type " + str(type)]

        return warnings

def load_element(element):
    if element.tag == "JoinData":
        return JoinData(element)
    if element.tag == "ComponentInstance":
        component = ComponentInstance(element.get("ID"), element.get("Name"), element.get("ComponentID"))

        for connector in element.findall("ConnectorInstance"):
            component.add_connector(load_element(connector))

        return component
    elif element.tag == "Container" or element.tag == "RootContainer":
        if element.tag == "RootContainer":
            container = RootContainer()

            for join_element in element.findall("JoinData"):
                container.add_join(load_element(join_element))
        else:
            container = Container(element.get("Name"))

        for nested_container in element.findall("Container"):
            container.add_container(load_element(nested_container))

        for component in element.findall("ComponentInstance"):
            container.add_component(load_element(component))

        for connector in element.findall("Connector"):
            container.add_connector(load_element(connector))

        return container
    elif element.tag == "Connector":
        return Connector(element.get("ID"), element.get("Name"), element.get("Definition"), element.get("ApplyJoinData"), element.get("ConnectorComposition"))
    elif element.tag == "ConnectorInstance":
        return ConnectorInstance(element.get("ID"), element.get("Name"), element.get("IDinComponentModel"), element.get("ApplyJoinData"), element.get("ConnectorComposition"))
    else:
        raise IOException("Unknown tag " + element.tag)

def load(file):
    tree = ElementTree.parse(file)
    design = tree.getroot()
    root_container = load_element(design.find("RootContainer"))

    # load default join info from ACM files
    try:
        with open(os.path.join(os.path.dirname(file), "component_index.json")) as input_file:
            component_index = json.load(input_file)

        for component in root_container.list_components():
            acm_file = None

            for entry in component_index:
                if entry["InstanceID"] == component.id:
                    acm_file = os.path.join(os.path.dirname(file), entry["ModelPath"])
                    break

            if acm_file is None:
                continue

            # Not sure if this is still needed, but early version of the component_index.json
            # file used URL-like %00 encodings
            if not os.path.exists(acm_file):
                acm_file = urllib.unquote(acm_file)

            if not os.path.exists(acm_file):
                continue

            tree = ElementTree.parse(acm_file)
            design = tree.getroot()

            for default_connector in design.iter("Connector"):
                if default_connector.find("DefaultJoin") is not None:
                    default_join_data = JoinData(default_connector.find("DefaultJoin"))
                    root_container.add_join(default_join_data)

                    for connector in component.connectors_with_id(default_connector.get("ID")):
                        connector.default_join = default_join_data.id
    except Exception:
        traceback.print_exc()

    return root_container

def update_result_file(result_file, field_name, value, unit="", successful=True):
    modified = False

    try:
        with open(result_file, "r") as file:
            data = json.load(file)

        for metric in data["Metrics"]:
            if metric["Name"] == str(field_name):
                metric["Value"] = str(value)
                metric["Unit"] = unit
                modified = True

        if successful:
            data["AnalysisStatus"] = "OK"
        else:
            data["AnalysisStatus"] = "FAILED"

        with open(result_file, "w") as file:
            json.dump(data, file, indent=4)
    except Exception:
        modified = False

    if not modified:
        print "Unable to find or update result file!  Please ensure the file exists and"
        print "contains the", field_name, "metric."
        sys.exit(-1)

def calculate_completeness(file):
    try:
        root_container = load(file)
        number_of_components = 0
        number_of_connections = 0
        number_of_free_components = 0
        incomplete_join_data = 0
        missing_join_data = 0
        output = { "details" : [] }

        for component in root_container.list_components():
            output_entry = { "id" : component.id, "name" : component.name, "connections" : [] }
            number_of_components = number_of_components + 1
            connections = 0

            for connector in component.connectors:
                endpoints = connector.find_endpoints()

                for key in endpoints.keys():
                    if isinstance(key, Container):
                        continue

                    joins_on_path = len(root_container.joins_on_path(endpoints[key]))
                    warnings = root_container.join_warnings_on_path(endpoints[key])
                    output_entry["connections"].append({ "id" : key.id, "name" : key.name, "warnings" : warnings })

                    if joins_on_path == 0:
                        missing_join_data = missing_join_data + 1
                    elif len(warnings) > 0:
                        incomplete_join_data = incomplete_join_data + 1
                    connections = connections + 1

            if connections == 0:
                number_of_free_components = number_of_free_components + 1
                output_entry["connections"].append({ "id" : "N/A", "name" : "N/A", "warnings" : ["Not connected to any other component"] })

            number_of_connections = number_of_connections + connections
            output["details"].append(output_entry)

        if number_of_connections == 0:
            if number_of_components > 1:
                completeness_metric = 0.0
            else:
                completeness_metric = 100.0
        else:
            completeness_metric = 100 * float(number_of_connections - incomplete_join_data - missing_join_data) / number_of_connections  

        print completeness_metric

        output["number_of_components"] = number_of_components
        output["number_of_free_components"] = number_of_free_components
        output["number_of_connections"] = number_of_connections / 2
        output["missing_join_data"] = missing_join_data / 2
        output["incomplete_join_data"] = incomplete_join_data / 2
        output["completeness_metric"] = completeness_metric

        try:
            with open(os.path.join(os.path.dirname(file), "completeness_output.json"), "w") as output_file:
                json.dump(output, output_file, indent=4)
        except Exception:
            traceback.print_exc()

        update_result_file(os.path.join(os.path.dirname(file), "testbench_manifest.json"), "Structural_Completeness", completeness_metric, "%", True)
    except Exception:
        traceback.print_exc()
        update_result_file(os.path.join(os.path.dirname(file), "testbench_manifest.json"), "Structural_Completeness", 0.0, "%", False)

if __name__ == "__main__":
    file = None

    if file is None:
        if len(sys.argv) < 2:
            file = "."
        else:
            file = sys.argv[1]

    if not os.path.exists(file):
        print file, "does not exist!"
        sys.exit(-1)

    if os.path.isdir(file):
        directory = file
        file = None

        for filename in os.listdir(directory):
            if filename.upper().endswith(".ADM"):
                file = os.path.join(directory, filename)
                break

        if file is None or not os.path.exists(file):
            print directory, "does not contain an ADM file!"
            update_result_file(os.path.join(directory, "testbench_manifest.json"), "Structural_Completeness", 0.0, "%", False)
            sys.exit(-1)

    calculate_completeness(file)
