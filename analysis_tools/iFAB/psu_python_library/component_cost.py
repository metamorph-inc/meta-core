"""
component_cost.py
Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2013

Compile a list of the components in a design and determine
their costs from the manufacturing models.
"""
# For every part in the design
# look up its manufacturing model
# extract price and lead time from MM

import glob
import json
import os
import xml.etree.ElementTree as ET
import re
import sys
import argparse
import traceback
import urllib

class CostModelError(Exception):
    """ error in the cost model """
    pass

def get_args(argv):
    """ extract arguments from argv """
    parser = argparse.ArgumentParser()
    parser.add_argument("design", help="design file name")
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="print a verbose report")
    return parser.parse_args(argv)

def evaluate(designfile, verbosity):
    """ extract components and produce a score """
    design = read_design(designfile)
    components = components_in(design.containers())
    path = os.path.dirname(designfile)
    manfmanifest = read_component_index(os.path.join(path, "component_index.json"))
    manfmanifest.update(read_manufacturing_manifest(os.path.join(path, "manufacturing.manifest.json")))
    score = score_components(path, components, manfmanifest)
    return score.report(verbosity)

def cli(argv):
    """ score the file given as an argument, print result """
    args = get_args(argv)
    verbosity = "summary"
    if args.verbose:
        verbosity = "report"
    report = evaluate(args.design, verbosity)
    print json.dumps(report, indent=4)

def read_design(designfile):
    """
    read the design from the design file and return an appropriate
    design object
    """
    designtype = None
    if re.search(r"\.adm$", designfile, flags=re.I) is not None:
        designtype = XMLDesign
    elif re.search(r"\.xml$", designfile, flags=re.I) is not None:
        designtype = XMLDesign
    elif re.search(r"\.json$", designfile, flags=re.I) is not None:
        designtype = JSONDesign
    design = designtype(designfile)
    return design

def read_component_index(component_index_file):
    result = {}

    with open(component_index_file, 'r') as fp:
        component_index = json.load(fp)

        for component in component_index:
            filename = os.path.join(os.path.dirname(component_index_file), component["ModelPath"])

            if not os.path.exists(filename):
                filename = urllib.unquote(filename)

            if os.path.exists(filename):
                try:
                    manfmodel = ET.parse(filename)
                except ET.ParseError:
                    try:
                        manfmodel = ET.parse(filename, parser=ET.XMLParser(encoding="us-ascii"))
                    except ET.ParseError:
                        # give up. no manf model
                        continue

                result[component["InstanceID"]] = {"id":component["InstanceID"], "model":manfmodel}

    return result

def read_manufacturing_manifest(manifestfile):
    with open(manifestfile, 'r') as fp:
        manifest = json.load(fp)
        component_list = manifest["ComponentManufactureList"]
        result = {}

        for component in component_list:
            if "ManufacturingModel" in component:
                filename = os.path.join(os.path.dirname(manifestfile), component["ManufacturingModel"])

                if os.path.exists(filename):
                    try:
                        manfmodel = ET.parse(filename)
                    except ET.ParseError:
                        try:
                            manfmodel = ET.parse(filename,
                                   parser=ET.XMLParser(encoding="us-ascii"))
                        except ET.ParseError:
                            # give up. no manf model
                            continue

                    if "InstanceGUID" in component:
                        result[component["InstanceGUID"]] = {"id":component["InstanceGUID"].strip("{}"), "model":manfmodel}
                    elif "id" in component:
                        result[component["id"]] = {"id":component["id"].strip("{}"), "model":manfmodel}

        return result        

class XMLComponent(object):
    """ a component derived from an XML design """
    def __init__(self, component):
        """ just save a ref to the component element """
        self.component = component

    def name(self):
        """ extract the component name """
        return self.component.get("Name", "")

    def ident(self):
        """ extract the component id """
        return self.component.get("ID", "")

class JSONComponent(object):
    """ a component derived from a JSON design """
    def __init__(self, component):
        """ just save a reference to the component """
        self.component = component

    def name(self):
        """ extract the component name """
        return self.component.get("Name", "")

    def ident(self):
        """ extract the component id """
        text = self.component.get("id", "")
        # strip surrounding curly braces from id
        return re.sub("[{}]", "", text)

class XMLContainer(object):
    """ a container derived from an XML design """
    def __init__(self, container):
        """ just save a reference to the container element """
        self.container = container

    def components(self):
        """ return a list of components in the container """
        children = self.container.findall("ComponentInstance")
        return [XMLComponent(c) for c in children]

    def containers(self):
        """
        This is tricky because I don't have an example design
        with nested containers.  I don't even know what they're
        supposed to be called, but I'm going to guess "Container".
        """
        children = self.container.findall("Container")
        return [XMLContainer(c) for c in children]

class JSONContainer(object):
    """ a container derived from a JSON design """
    def __init__(self, container):
        """ just save a reference to the container """
        self.container = container

    def components(self):
        """ return a list of components in the container """
        return [JSONComponent(c) for c
                in self.container.get("ComponentInstances", [])]

    def containers(self):
        """ return a list of containers in the container"""
        return [JSONContainer(c) for c
                in self.container.get("Containers", [])]

class XMLDesign(object):
    """ an XML design file """
    def __init__(self, designfile):
        try:
            self.design = ET.parse(designfile)
        except ET.ParseError:
            # just let it raise an error if we can't parse it now
            parser = ET.XMLParser(encoding="us-ascii")
            self.design = ET.parse(designfile, parser=parser)

    def containers(self):
        """
        Return a list of containers, although unlike the JSON
        designs, I think the XML designs must have a single
        RootContainer. Although I could be wrong about that.
        Also, all of the XML designs I've seen are flat, but
        I'm not sure if that's guaranteed.
        """
        rootcontainers = self.design.getroot().findall("RootContainer")
        return [XMLContainer(c) for c in rootcontainers]

class JSONDesign(object):
    """ a JSON design file """
    def __init__(self, designfile):
        """ read in the JSON file """
        with open(designfile, 'r') as fp:
            self.design = json.load(fp)

    def containers(self):
        """ return a list of containers """
        return [JSONContainer(c) for c
                in self.design.get("Containers", [])]

def components_in(containers):
    """ given a list of containers, return the components within """
    components = []
    for container in containers:
        instances = container.components()
        components.extend(instances)
        subcontainers = container.containers()
        components.extend(components_in(subcontainers))

    return components

# $ USD per kilogram
material_library = {"GOLD_MASS": 47619.0, "TITANIUM_MASS": 6.15, "STEEL_MASS": 0.209}

def to_kg(value, unit="kg"):
    if unit == "oz":
        return value*0.0283495
    elif unit == "lb":
        return value*0.453592
    elif unit == "ton":
        return value*907.185
    elif unit == "mg":
        return value*0.000001
    elif unit == "g":
        return value*0.001
    elif unit == "kg":
        return value
    elif unit == "tonne":
        return value * 1000
    else:
        print "Unrecognized unit", unit
        return value

def lookup_weight(id, path):
    metrics = ET.parse(os.path.join(path, "CADAssembly_metrics.xml"))
    root = metrics.getroot()
    metricId = None

    # first find the metric id
    for component in root.find("Assemblies").iter("CADComponent"):
        if component.get("ComponentInstanceID").strip("{}") == id:
            metricId = component.get("MetricID")
            break

    # then find the mass
    for component in root.find("MetricComponents").iter("MetricComponent"):
        if component.get("MetricID") == metricId:
            for scalar in component.find("Scalars").iter("Scalar"):
                if scalar.get("Name") == "Mass":
                    return to_kg(float(scalar.get("Value")), scalar.get("Unit", "kg"))

    return None
        
def manufacturing_cost_model(id, name, model, path):
    """ compute the cost and lead time of a component """
    if model is None:
        return ("no manufacturing model", "no manufacturing model")

    try:
        # the manufacturing info may be wrapped in a <part> element
        root = model.getroot()

        if root.tag == "part":
            root = root.find("manufacturingDetails")

        if root.tag == "manufacturingDetails":
            root = list(root)[0]

        if root.tag == "{avm}Component":
            is_production = True
            production_cost = "no manufacturing model"
            production_leadTime = "no manufacturing model"
            prototype_cost = "no manufacturing model"
            prototype_leadTime = "no manufacturing model"
            for parameter in root.iter("Parameter"):
                if parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_production__part_cost_per_orderable_unit__value":
                    if parameter.find("Value").find("ValueExpression").find("Value").text != "None":
                        production_cost = float(parameter.find("Value").find("ValueExpression").find("Value").text)
                elif parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_production__part_leadtime_per_orderable_unit" or parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_production__part_leadtime":
                    if parameter.find("Value").find("ValueExpression").find("Value").text != "None":
                        production_leadTime = to_days(float(parameter.find("Value").find("ValueExpression").find("Value").text), parameter.find("Value").get("Unit", "s"))
                elif parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_prototype__part_cost_per_orderable_unit__value":
                    if parameter.find("Value").find("ValueExpression").find("Value").text != "None":
                        prototype_cost = float(parameter.find("Value").find("ValueExpression").find("Value").text)
                elif parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_prototype__part_leadtime_per_orderable_unit" or parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_prototype__part_leadtime":
                    if parameter.find("Value").find("ValueExpression").find("Value").text != "None":
                        prototype_leadTime = to_days(float(parameter.find("Value").find("ValueExpression").find("Value").text), parameter.find("Value").get("Unit", "s"))
                elif parameter.get("Name") == "procurement__supplier_specific_part_info__part_info__cost_leadtime_production__production_or_prototype":
                    if parameter.find("Value").find("ValueExpression").find("Value").text != "None":
                        is_production = True if parameter.find("Value").find("ValueExpression").find("Value").text.lower() == "production" else False

            if is_production:
                if production_cost == "no manufacturing model" or production_leadTime == "no manufacturing model":
                    return (prototype_cost, prototype_leadTime)
                else:
                    return (production_cost, production_leadTime)
            else:
                if prototype_cost == "no manufacturing model" or prototype_leadTime == "no manufacturing model":
                    return (production_cost, production_leadTime)
                else:
                    return (prototype_cost, prototype_leadTime)
        elif root.tag == "purchased":
            root = root.find("supplier")
            cost = float(root.find("price").text)
            leadTime = to_days(float(root.find("leadTime").text), root.find("leadTime").attrib.get("unit", "day"))

            # Hack for MSD, since the procurement information is not changing with different weights
            if name.startswith("Mass_"):
                part_weight = lookup_weight(id, path)
                if name.endswith("Gold"):
                    cost = material_library.get("GOLD_MASS", 1.0)*part_weight
                elif name.endswith("Titanium"):
                    cost = material_library.get("TITANIUM_MASS", 1.0)*part_weight
                elif name.endswith("Steel"):
                    cost = material_library.get("STEEL_MASS", 1.0)*part_weight

            return (cost, leadTime)
        else:
            # estimate manufacturing cost based on mass
            if root.find("material") is None or root.find("material").find("alloySteel") is not None:
                material = "STEEL_MASS"
            else:
                material = root.find("material").text

            material_cost = material_library.get(material, 0.209)
            part_weight = lookup_weight(id, path)
            time = 30 + part_weight*2

            return (round(material_cost*part_weight + 50*(time/60),2), round(4 + time/(8*60),1))
    except:
        traceback.print_exc()
        return ("no manufacturing model", "no manufacturing model")

def score_components(path, components, manfmanifest):
    """ compute a score given a list of components """
    score = Score()

    for component in components:
        name = component.name()
        ident = component.ident()
        entry = manfmanifest.get(ident, None)

        if entry is None or "model" not in entry:
            cost = "no manufacturing model"
            time = "no manufacturing model"
        else:
            (cost, time) = manufacturing_cost_model(entry["id"], name, entry["model"], path)

            try:
                cost = float(cost) + 600 # add labor cost for ordering, receiving, & inspection
            except ValueError:
                cost = "no manufacturing model"
            try:
                time = float(time) + 2 # add shipping time
            except ValueError:
                time = "no manufacturing model"

        score.tally(ident, name, cost, time)

    return score

class Score(object):
    """ keep score on cost and lead time """
    def __init__(self):
        """ prepare aligned lists for tally """
        self.idents = []
        self.names = []
        self.costs = []
        self.leadtimes = []

    def tally(self, ident, name, cost, leadtime):
        """ add info to each list """
        self.idents.append(ident)
        self.names.append(name)
        self.costs.append(cost)
        self.leadtimes.append(leadtime)

    def report(self, kind="summary"):
        """ construct a table and compute the score """
        header = ["identity", "name",
                  "cost", "leadtime"]
        data = zip(self.idents, self.names,
                   self.costs, self.leadtimes)
        cost = 0.0
        leadtime = 0.0
        defective = 0
        for _, _, price, lead in data:
            try:
                cost += float(price)
                leadtime = max(float(lead), leadtime)
            except TypeError:
                defective += 1
            except ValueError:
                defective += 1

        if kind == "report":
            verbose = [dict(zip(header, record)) for record in data]
            report = {"detail": verbose,
                      "cost": cost, "leadtime": leadtime,
                      "defective": defective}
        else:
            report = {"cost": cost, "leadtime": leadtime,
                      "defective": defective}

        return report

def to_days(astring, unit):
    """ No weeks in QUDT unit library: days or years """
    factors = {
            "week":     7.0,        "weeks":    7.0,
            "Week":     7.0,        "Weeks":    7.0,
            "day":      1.0,        "Day":      1.0,
            "days":     1.0,        "Days":     1.0,
            "hour":     1.0/24.0,   "hours":    1.0/24.0,
            "Hour":     1.0/24.0,   "Hours":    1.0/24.0,
            "minute":   1.0/1440.0, "Minute":   1.0/1440.0,
            "minutes":  1.0/1440.0, "Minutes":  1.0/1440.0,
            "s": 1.0/86400.0
            }
    factor = factors.get(unit, 0.0)
    try:
        value = float(astring)
    except ValueError:
        return None

    return factor * value

if __name__ == "__main__":
    cli(sys.argv[1:])

# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai:colorcolumn=74:number
