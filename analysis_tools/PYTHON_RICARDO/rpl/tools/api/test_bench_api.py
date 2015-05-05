""" Get component information from metrics file. """
import sys, os
import logging
import json
from os.path import join, dirname
import functools
from collections import defaultdict
import cStringIO as StringIO

import numpy as np
from lxml import etree

from rpl.tools.geometry.geom_utils import trans_mat_from_avm_xml
from rpl.tools.geometry.read_cad_metrics import matrix_from_nx_ny_ro
from rpl.tools.geometry.surface_io import open_geom

## Get rid of any existing handlers (e.g. if running interactively in ipython)
logging.getLogger().handlers = []

## Main logging: output everything, including litany of debug messages
logging.basicConfig(filename="test_bench_debug.log",
                    filemode="w",
                    format="%(levelname)s %(asctime)s %(message)s",
                    level=logging.DEBUG)

## Also make end-user friendly log file (showing informational messages and TB results)
log_full = logging.FileHandler('test_bench.log', mode='w')
log_full.setLevel(logging.INFO)
log_full.setFormatter(logging.Formatter("%(levelname)s %(asctime)s %(message)s"))
logging.getLogger("").addHandler(log_full)

## Also log DEBUG and above to the console
console = logging.StreamHandler()
console.setLevel(logging.DEBUG)
console.setFormatter(logging.Formatter("%(asctime)s %(message)s"))
logging.getLogger("").addHandler(console)


def excepthook(*args):
    """
    Ensure that any uncaught exception is logged
    """
    logging.getLogger().error('Uncaught exception: ', exc_info=args)


# Uncaught exceptions and assertions should write error to log
sys.excepthook = excepthook

## Conversion factors to "m" in case CAD has odd units
si_units = ["m", "kg", "Pa", "N", "rad"]
conv_dict = {"inch": 0.0254, "millimeter": 0.001, "mm": 0.001, "lbs": 0.453592,
             "g": 0.001, "slug": 14.5939029, "kg*mm^2": 1e-06,
             "deg": 0.01745}
conv_units_dict = {"inch": "m", "millimeter": "m", "mm": "m", "lbs": "kg",
                   "g": "kg", "slug": "kg", "kg*mm^2": "kg*m^2",
                   "deg": "rad"}

## Compiled XPath functions to find datums properties and primitives
xp_datum = etree.XPath(r"//Datum[@DatumName = $v]")
xp_metric = etree.XPath(r"//ModelMetric[@Name = $v]/Value/ValueExpression//Value")
xp_metric_data_type = etree.XPath(r"//ModelMetric[@Name = $v]/Value/@DataType")
xp_metric_unit = etree.XPath(r"//ModelMetric[@Name = $v]/Value/@Unit")
xp_propt = etree.XPath(r"//Value[@ID = concat('nv.',$v)]/ValueExpression/Value")
xp_propt_data_type = etree.XPath(r"//Value[@ID = concat('nv.',$v)]/@DataType")
xp_propt_unit = etree.XPath(r"//Value[@ID = concat('nv.',$v)]/@Unit")
xp_propt_default = etree.XPath(r"//Value[@ID = concat('nv.',$v)]/ValueExpression/Default/Value")
xp_propt_source = etree.XPath(r"//Value[@ID = concat('nv.',$v)]/ValueExpression/@ValueSource")


## Load in the predefined exclusion sets that can strip unneeded classes to make a model smaller
with open(join(dirname(__file__), "geometry_sets.js"), "r") as g_sets_json:
    ## Convert json lists to sets
    geom_sets = {k: set(v) for k, v in json.load(g_sets_json).items()}

id_to_class_map_file = ""
cad_metrics_xml_root = None
path_to_instance_xmls = ""
stl_loc = ""
instances_by_class = defaultdict(dict)
output_json_file = ""


def load_settings(settings_file):
    """
    Load in the json file ``settings`` and use it to build the internal instances by class 
    dictionary as well as set paths to various files.
    """
    global id_to_class_map_file, cad_metrics_xml_root, path_to_instance_xmls, stl_loc, output_json_file

    try:
        settings = json.load(open(settings_file, "r"))

        msg = StringIO.StringIO()
        with open(settings_file, "r") as set_out:
            for line in set_out:
                msg.write(line)
        logging.info("Read in the following settings from file {}:\n {}".format(
            settings_file, msg.getvalue()))

    except (IOError, ValueError) as err:
        msg = "Could not parse the json settings file, {}.".format(settings_file)
        msg = msg + " Check the file and please try again. Error: {}".format(err)
        raise ValueError(msg)

    for r in ["metrics_file", "instance_file", "path_to_instance_stls", "path_to_instance_xmls",
              "output_json_file"]:
        if r not in settings:
            msg = "Required setting '{}' not found in settings file.".format(r)
            logging.error(msg)
            raise ValueError(msg)

    try:
        cad_metrics_xml_root = etree.parse(settings["metrics_file"])
    except IOError:
        msg = "Cad metrics files '{}' could not be loaded.".format(settings["metrics_file"])
        logging.error(msg)
        raise ValueError(msg)

    id_to_class_map_file = settings["instance_file"]
    stl_loc = settings["path_to_instance_stls"]
    path_to_instance_xmls = settings["path_to_instance_xmls"]
    output_json_file = settings["output_json_file"]
    try:
        with open(id_to_class_map_file, "r") as json_data:
            data = json.load(json_data)
    except IOError:
        msg = "Id to class map file '{}' could not be loaded.".format(id_to_class_map_file)
        logging.error(msg)
        raise ValueError(msg)

    for d in data:
        ## Store the model path keyed by class then instance_id ( [1:-1] removes curly braces)
        instances_by_class[d["Classification"]][d["InstanceID"][1:-1]] = d["ModelPath"]

    return settings


def get_all_geom_set():
    """
    Returns a set of all component classes used in the assembly.
    """
    return set(instances_by_class.keys())


def get_data(parts_of_interest):
    """
    Given a dictionary of classes with sub-dicts of required properties, returns a dictionary keyed
    first by class and then by instance_id of those extracted properties.
    """
    component_dict = defaultdict(dict)

    ## verbose log entry detailing what the components and attributes the tet bench is looking for
    text_head = "This test bench will be looking for the following classes and attributes:\n"
    text_str = ""
    for comp_class, propt in parts_of_interest.items():
        text_str = text_str + "\t{}\n".format(comp_class)
        for prop_type, prop_value in propt.items():
            if prop_type.lower() == "required":
                min_num, max_num = get_class_req_num(prop_value)
                if max_num == 1e3:
                    max_num = "any"
                text_str = text_str + "\t\t{}: min: {}, max: {}\n".format(prop_type, min_num,
                                                                          max_num)
            else:
                text_str = text_str + "\t\t{}: {}\n".format(prop_type, prop_value)

    logging.info(text_head + text_str)

    ## Loop over each part class and get the attributes of interest
    for part_class, attr_intr in parts_of_interest.items():

        num_of_class_inst = 0
        try:
            min_num, max_num = get_class_req_num(attr_intr["Required"])
        except (KeyError, NameError):
            min_num = 1
            ## Set max_num to a high number so that any number of instances can be found
            max_num = 1e3
        #        min_num, max_num = get_class_req_num(attr_intr["Required"])

        for inst_num, inst in enumerate(instances_by_class[part_class]):
            num_of_class_inst = inst_num + 1

        if num_of_class_inst < min_num or num_of_class_inst > max_num:
            if max_num == 1e3:
                max_num = "any"
            msg = "Could not find the required number of instances for class {}.".format(part_class)
            msg = msg + " Minimum number required, {}. Maximum number required, {}.".format(
                min_num, max_num)
            msg = msg + " Found {}".format(num_of_class_inst)
            logging.error(msg)
            # raise ValueError(msg)

        ## Loop over each instance of the class getting the attributes of interest
        for instance_id, model_path in instances_by_class[part_class].items():
            part = _get_part(instance_id, model_path)
            component_dict[part_class][instance_id] = component(attr_intr,
                                                                part["trans"],
                                                                model_path,
                                                                part_class,
                                                                instance_id)

            component_dict[part_class][instance_id].update({"trans": part["trans"],
                                                            "name": part["name"]})

    return component_dict


def load_geometry(class_set, single_file=False):
    """
    Given a set of classes return a dictionary with instance_id keys each containing a surface
    dictionary with "x", "y", "z", and "tris" keys.

    If ``single_file == True`` all the surfaces are merged together and a dictionary containing the
    "x", "y", "z", and "tris" keys is returned (i.e. no instance_id keys used).
    
    The returned surface(s) are **always** transformed to be in their correct vehicle position.
    """
    surf = {} if not single_file else {"x": np.empty((0,), dtype=np.float32),
                                       "y": np.empty((0,), dtype=np.float32),
                                       "z": np.empty((0,), dtype=np.float32),
                                       "tris": np.empty((0, 3), dtype=np.int32)}

    ## Loop through each class of component asked for
    for part_class in class_set:
        if part_class not in instances_by_class:
            logging.warn("No parts of class '{}' to load in.".format(part_class))
            continue

        ## Loop over each component instance of the class
        for inst_id, model_path in instances_by_class[part_class].items():
            try:
                instance_data = _get_part(inst_id, model_path)
            except ValueError:
                ## For now we will carry on.  ACM file is unloadable (probably not present)
                ## Already logged error in _get_part()
                continue

            try:
                g_path = join(stl_loc, instance_data["stl_name"])
                geom = open_geom(g_path)
                transform_geometry(geom, instance_data["trans"], 0.001)
                logging.debug("Loaded instance {} stl_file {}".format(inst_id, g_path))
            except (KeyError, IOError) as e:
                ## For now we will log and carry on but this may mean results are wrong.
                msg = "Cannot load instance '{}' stl_file '{}'. Got: {}".format(inst_id, g_path, e)
                logging.error(msg)
                continue

            if single_file:
                ## Increment the node refs in the new tris to point to appended position
                geom["tris"] += len(surf["x"])
                surf = {k: np.concatenate((surf[k], geom[k])) for k in ["x", "y", "z", "tris"]}
            else:
                ## Store the new surface under the instance_id key
                surf[inst_id] = geom
                # Add in the part class explicitly; don't make TBs guess based on instance name.
                surf[inst_id]['part_class'] = part_class

    return surf


def transform_geometry(geom, trans_matrix=None, pre_trans_scale_factor=1.0):
    """
    Scale and translate x, y, and z coords of ``geom`` **in place**.
    
    The scaling in ``pre_trans_scale_factor`` is applied before the transform.
    """
    scaled = {k: geom[k] * pre_trans_scale_factor for k in "xyz"}

    if trans_matrix is None:
        geom.update(scaled)
    else:
        nodes = np.vstack((scaled["x"], scaled["y"], scaled["z"], np.ones_like(geom["x"])))
        nodes = np.dot(trans_matrix, nodes).T[:, :3]
        geom.update({k: nodes[:, i] for i, k in enumerate("xyz")})


def component(prop_dict, trans, model_path, part_class, instance_id):
    """
    Returns a dictionary of the properties requested in ``prop_dict for an instance of a class.
    
    ``trans`` is the transformation matrix to put this instance into its global vehicle position.
    ``model_path`` is the location of this instances .acm file.
    """
    ## Parse the XML file and extract the data fields using xpath
    xr = etree.parse(model_path)

    to_global = functools.partial(np.dot, trans)

    d = {"datums": {}, "properties": {}, "model_metrics": {}}

    try:
        for prop_type, props in prop_dict.items():
            if prop_type == "Datum":
                for k in props:
                    dat_loc = xp_datum(xr, v=k)
                    if len(dat_loc) > 0:
                        loc = trans_mat_from_avm_xml(dat_loc[0])
                    else:
                        loc = 0
                        msg = "Problem retrieving '{}' called '{}' from '{}'.".format(prop_type, k,
                                                                                      model_path)
                        logging.error(msg)
                        raise ValueError(msg)
                    d["datums"][k] = {"local": loc, "global": to_global(loc)}

            if prop_type == "Property" or prop_type == "PrimitiveProperty":
                for k in props:
                    data_type = _get_data_type(prop_type, k, xr)

                    if data_type == "Real" or data_type == "" or \
                                    data_type == "Integer":
                        unit = xp_propt_unit(xr, v=k)
                        if len(unit) > 0:
                            unit = unit[0]

                        if unit not in si_units:
                            try:
                                conv_fact = conv_dict[unit]
                            except:
                                logging.error("{}: {} in {} has an unknown unit: '{}'".format(
                                    prop_type, k, part_class, unit))
                                conv_fact = 1.0
                        else:
                            conv_fact = 1.0

                        try:
                            orig_value = float(xp_propt(xr, v=k)[0].text)
                        except:
                            try:
                                orig_value = float(xp_propt_default(xr, v=k)[0].text)
                            except:
                                value_source = xp_propt_source(xr, v=k)[0]
                                mm_name = value_source.split(".")[-1]
                                msg = "{}: {} in {} has no fixed value.".format(prop_type, k,
                                                                                part_class)
                                msg = msg + "Looking for a ModelMetric with name {}".format(mm_name)
                                logging.debug(msg)
                                orig_value, conv_fact = try_model_metric(prop_type, mm_name, xr,
                                                                         part_class)

                        converted_value = orig_value * conv_fact

                        if unit not in si_units:
                            if unit in conv_units_dict.keys():
                                converted_unit = conv_units_dict[unit]
                            else:
                                converted_unit = unit
                            msg = "In component '{}' for field '{}' found non-SI unit --> {}[{}] converted to {}[{}]" \
                                .format(instance_id, k, orig_value, unit, converted_value,
                                        converted_unit)
                            logging.debug(msg)

                        d["properties"][k] = converted_value

                    else:
                        logging.debug("Returning {}: {} in {} as a string as indicated by the "
                                      "DataType, {}.".format(prop_type, k, part_class, data_type))
                        try:
                            d["properties"][k] = xp_propt(xr, v=k)[0].text
                        except:
                            d["properties"][k] = xp_propt_default(xr, v=k)[0].text

            if prop_type == "ModelMetric":
                for k in props:
                    data_type = _get_data_type(prop_type, k, xr)
                    if data_type == "Real" or data_type == "" or \
                                    data_type == "Integer":

                        unit = xp_metric_unit(xr, v=k)
                        if len(unit) > 0:
                            unit = unit[0]

                        if unit not in si_units:
                            try:
                                conv_fact = conv_dict[unit]
                            except:
                                logging.error("{}: {} in {} has an unknown unit: '{}'".format(
                                    prop_type, k, part_class, unit))
                                conv_fact = 1.0
                        else:
                            conv_fact = 1.0

                        orig_value = float(xp_metric(xr, v=k)[0].text)
                        converted_value = orig_value * conv_fact

                        if unit not in si_units:
                            if unit in conv_units_dict.keys():
                                converted_unit = conv_units_dict[unit]
                            else:
                                converted_unit = unit
                            msg = "In component '{}' for field '{}' found non-SI unit --> {}[{}] converted to {}[{}]" \
                                .format(instance_id, k, orig_value, unit, converted_value,
                                        converted_unit)
                            logging.debug(msg)

                        d["model_metrics"][k] = converted_value
                    else:
                        logging.debug("Returning {}: {} in {} as a string as indicated by the "
                                      "DataType, {}.".format(prop_type, k, part_class, data_type))
                        d["model_metrics"][k] = xp_metric(xr, v=k)[0].text

    except KeyError:
        msg = "Problem retrieving '{}' called '{}' from '{}'.".format(prop_type, k, model_path)
        logging.error(msg)
        raise ValueError(msg)

    return d

## Pre-compiled xpath functions for extracting transform information
x_id = etree.XPath(r"//ChildMetric[@ComponentInstanceID = $inst]")
x_rot = etree.XPath(r"RotationMatrix//Column/@Value")
x_resource = etree.XPath(r"//ResourceDependency[@ID = 'cad.path']/@Name")
x_met_comp = etree.XPath(r"//MetricComponents//MetricComponent")
x_name = etree.XPath(r"@Name")


def _get_data_type(prop_type, k, xr):
    """
    get the data_type for properties, primitive properties, and ModelMetrics 
    if it exists. If no DataType is specified return a blank string
    """

    if prop_type == "Property":
        try:
            data_type = xp_propt_data_type(xr, v=k)[0]

        except:
            data_type = ""

    if prop_type == "PrimitiveProperty":
        try:
            data_type = xp_propt_data_type(xr, v=k)[0]
        except:
            data_type = ""

    if prop_type == "ModelMetric":
        try:
            data_type = xp_metric_data_type(xr, v=k)[0]
        except:
            data_type = ""

    return data_type


def _get_part(instance, model_path):
    """
    Return a list of part geometric information for ``instance`` with a .acm file at ``model_path``.
    """
    part_info = None

    try:
        x_root = etree.parse(model_path)
    except (KeyError, IOError) as error:
        msg = "Unreadable ACM file at '{}' for instance '{}' exception was {}".format(model_path,
                                                                                      instance,
                                                                                      error)
        logging.error(msg)
        raise ValueError(msg)

    stl_prefix = x_resource(x_root)[0][:-4]
    name = x_name(x_root)[0]

    #find where in the list component is
    for child_num, child in enumerate(cad_metrics_xml_root.xpath(r"//ChildMetric")):
        if child.get("ComponentInstanceID")[1:-1] == instance:
            inst_num = child.get("MetricID")

    for child_metric in x_met_comp(cad_metrics_xml_root):
        if child_metric.get("MetricID") == str(inst_num):
            length_unit = child_metric.xpath(r"Units/@Distance")[0]

            try:
                conv_factor = conv_dict[length_unit]
                if length_unit != "m" or length_unit != "kg" or length_unit != "N" \
                    or length_unit != "kg*m^1":
                    converted_unit = conv_units_dict[length_unit]
                    msg = "In component '{}' for field 'Distance' found ".format(stl_prefix)
                    msg = msg + "non-SI unit --> {} converted to {}." \
                        .format(length_unit, converted_unit)
                    logging.debug(msg)

            except KeyError:
                logging.error("{} has an unknown unit for length: '{}'".format(
                    stl_prefix, length_unit))
                conv_factor = 1.0

            stl_suffix = "_asm.stl" if child_metric.get("Type") == "ASSEMBLY" else "_prt.stl"

            part_dict = {"stl_name": child_metric.get("Name") + stl_suffix,
                         "instance": instance,
                         "conv_fac": conv_factor,
                         "name": name,
                         "index": inst_num}

            part_info = part_dict
            break

    if part_info is None:
        ## Couldn't find the instance in the metrics file
        msg = "No instance '{}' with cad name '{}' in metrics file.".format(instance, str(inst_num))
        logging.error(msg)
        raise ValueError(msg)

    for child_metric in x_id(cad_metrics_xml_root, inst="{" + instance + "}"):

        mat = np.eye(4, dtype=np.float32)

        flat = [float(f) for f in x_rot(child_metric)]
        mat[0, :3] = flat[0:3]
        mat[1, :3] = flat[3:6]
        mat[2, :3] = flat[6:9]

        ## Check if this is a genuine VU file
        if cad_metrics_xml_root.getroot().get("VersionInfo"):
            logging.debug("Transposing rotation matrix contained in genuine VU metrics XML file")

        ## Will recalculate a fresh matrix because the ones in the file usually include
        ## a scale component too
        n_x = np.dot(mat[:3, :3], np.array([1, 0, 0]))
        n_y = np.dot(mat[:3, :3], np.array([0, 1, 0]))

        scale = part_info["conv_fac"]

        tran = [float(child_metric.xpath(r"Translation/@" + k)[0]) * scale for k in "XYZ"]

        part_info.update({"trans": matrix_from_nx_ny_ro(n_x, n_y, tran)})

    return part_info


def get_class_req_num(required):
    """Assigned value for required number of classes to find. If not assigned set min_num to 0"""
#    required = attr_intr["Required"]
    try:
        required
    except NameError:
        min_num = 1
        ## Set max_num to a high number so that any number of instances can be found
        max_num = 1e3

    ## set min and max number of class to find in assembly
    if "*" in required:
        min_num = 0
        ## Set max_num to a high number so that any number of instances can be found
        max_num = 1e3
    elif "+" in required:
        min_num = 1
        ## Set max_num to a high number so that any number of instances above the min can be found
        max_num = 1e3
    elif "," in required:
        req_num = required.split(",")
        min_num = int(req_num[0].split("{")[1])
        max_num = int(req_num[1].split("}")[0])
    else:
        req_num = required.split("{")
        min_num = max_num = int(req_num[1].split("}")[0])

    return min_num, max_num


def try_model_metric(prop_type, k, xr, part_class):
    """
    If a property has no value and the attribute ValueSource is defined, 
    check for a ModelMetric witht he value
    """
    unit = xp_metric_unit(xr, v=k)
    if len(unit) > 0:
        unit = unit[0]

    if unit not in si_units:
        try:
            conv_fact = conv_dict[unit]
        except:
            logging.error("{}: {} in {} has an unknown unit: '{}'".format(
                prop_type, k, part_class, unit))
            conv_fact = 1.0
    else:
        conv_fact = 1.0

    orig_value = float(xp_metric(xr, v=k)[0].text)

    return orig_value, conv_fact


def write_results(results_dict):
    """
    Write ``results_dict`` to json file ``output_json_file`` and echo contents to log.
    """
    with open(output_json_file, "w") as out_metrics:
        json.dump(results_dict, out_metrics, indent=4)

    msg = StringIO.StringIO()
    with open(output_json_file, "r") as check:
        for line in check:
            msg.write(line)
    logging.info("Wrote to json file:\n{}".format(msg.getvalue()))


def get_ground_pts():
    """
    Using the supplied computed values xml, gets the three points in ground plane.
    """
    xml_name = r"ComputedValues.xml"
    if os.path.exists(xml_name):
        xml_root = etree.parse(xml_name)
        ground_pts = []

        for child in xml_root.xpath(r"//ComplexMetric[@SubType = 'GROUND']/Metric"):
            unit = child.get("Units")
            conv_factor = conv_dict[unit]
            plane_coords = child.get("ArrayValue").split(";")
            x = float(plane_coords[0]) * conv_factor
            y = float(plane_coords[1]) * conv_factor
            z = float(plane_coords[2]) * conv_factor
            ground_pts.append(np.array([x, y, z]))
        return ground_pts
    else:
        raise ValueError(
            "{} is missing. Cannot find up direction or vectors in the ground plane. The Test Bench must compute these values.".format(
                xml_name))


def get_up_vector():
    """
    Using the supplied computed values xml, gets the three points in the ground plane.
    Computes two vectors in the ground plane and determines the normal vector, aka "up direction"
    """
    ground_pts = get_ground_pts()

    plane_vecs = np.array([ground_pts[1] - ground_pts[0],
                           ground_pts[2] - ground_pts[0]])
    up_vec = np.cross(plane_vecs[0], plane_vecs[1])
    # Return normalized vector of unit length
    up_vec /= np.linalg.norm(up_vec)

    # If ComputedValues.xml doesn't exist, get_ground_pts() will raise an exception
    return up_vec, plane_vecs, ground_pts


def get_veh_mass():
    """Extract the vehicle mass from CAD assembly metrics file
    Will fail or give weird results if the overall assembly metrics are not printed out before
    info about indiv components.
    """
    # The whole-assembly section is always the first MetricComponent in the file (ID=1)
    # TODO: consolidate into single xpath expression
    assembly_node = cad_metrics_xml_root.xpath(r"//MetricComponent")[0]
    mass = assembly_node.xpath("./Scalars/Scalar[@Name='Mass']/@Value")[0]
    return float(mass)


def get_veh_cg():
    """Extract the vehicle CG from CAD assembly metrics file
    Will fail or give weird results if the overall assembly metrics are not printed out before
    info about indiv components."""
    assembly_node = cad_metrics_xml_root.xpath(r"//MetricComponent")[0]
    cg = assembly_node.xpath("./CG")[0]
    return np.array([float(cg.get("X")),
                     float(cg.get("Y")),
                     float(cg.get("Z"))])