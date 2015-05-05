"""
cost_and_lead_time.py
Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2013

Compute cost and lead time for a design using its
manufacturing models plus cad metrics for the hull.
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
import time
import StringIO
import _winreg

def query_analysis_tools():
    analysis_tools_key_name = r'SOFTWARE\Wow6432Node\META\AnalysisTools'

    analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                         _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY)

    number_of_keys =  _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys

    info = {}
    for sub_key_id in range(0, number_of_keys):
        sub_key_name = _winreg.EnumKey(analysis_tools_key, sub_key_id)
        sub_key = _winreg.OpenKey(analysis_tools_key, sub_key_name)
        info[sub_key_name] = {}
        number_of_values = _winreg.QueryInfoKey(sub_key)[1]
        for value_id in range(0, number_of_values):
            value_tuple = _winreg.EnumValue(sub_key, value_id)
            value_name = value_tuple[0]
            value = value_tuple[1]
            info[sub_key_name][value_name] = value

    return info

tools = query_analysis_tools()
sys.path.append(os.path.join(tools['conceptual_mfg']['InstallLocation']))

import psu_python_library.component_cost as component_cost
import psu_python_library.hull_cost_model as hcm
import psu_python_library.hull_data_extractor as hde

#import component_cost as component_cost
#import hull_cost_model as hcm
#import hull_data_extractor as hde

class CostModelError(Exception):
    pass

def get_args(argv):
    name = argv.pop(0)
    parser = argparse.ArgumentParser(name)
    parser.add_argument("-d", "--directory", 
                        default=".",
                        help="directory where design can be found")
    parser.add_argument("--dry-run", action="store_true",
                        help="print proposed file changes rather than "\
                             "writing them to disk")
    parser.add_argument("--material", help="override hull plate material")
    parser.add_argument("--thickness", type=float,
                        help="override hull plate thickness")
    parser.add_argument("--exhaust-materials", action="store_true",
                        help="run the model for all materials to explore"\
                             "the space")
    return parser.parse_args(argv)

def evaluate_components(directory, **kwargs):
    designfiles = []

    for extension in ["*.adm", "*.metadesign.xml", "*.metadesign.json"]:
        pattern = os.path.join(directory, extension)
        matches = glob.glob(pattern)
        designfiles.extend(matches)

    if len(designfiles) < 1:
        msg = "No designs found, reporting zero component cost.\n"
        sys.stderr.write(msg)
        return {}
    elif len(designfiles) > 1:
        msg = "Multiple designs found, using {0}\n"
        sys.stderr.write(msg.format(designfiles[0]))

    return component_cost.evaluate(designfiles[0], "report")

def find_hull(directory):
    # look for a componentACMs directory
    patterns = [
            {"pattern": "ComponentACMs", "flags": 0},
            {"pattern": "componentacms", "flags": re.I},
            {"pattern": "acm", "flags": re.I}
            ]

    potential_acm_directories = []
    listing = [os.path.join(directory, d) 
                for d in os.listdir(directory)
                if os.path.isdir(os.path.join(directory,d))]

    for pat in patterns:
        potential_acm_directories.extend([
            d for d in listing 
            if re.search(pat["pattern"], d, flags=pat["flags"])
            ])
        if len(potential_acm_directories) > 0:
            break

    if len(potential_acm_directories) == 0:
        potential_acm_directories.append(directory)

    design_acms = []
    for dirname in potential_acm_directories:
        pattern = os.path.join(dirname, "*acm")
        acms = glob.glob(pattern)
        design_acms.extend(acms)

    hull_acms = []

    # iterate through componentacms looking for hulls
    for filename in design_acms:
        try:
            eltree = ET.parse(filename)
        except ET.ParseError:
            try: # fallback on us-ascii parser
                eltree = ET.parse(filename, 
                           parser=ET.XMLParser(encoding="us-ascii"))
            except ET.ParseError: # can't read the file, skip it
                continue
        classifications = eltree.getroot().find(".//Classifications")
        if classifications is not None and classifications.text is not None:
            if re.search("hull", classifications.text, flags=re.I):
                hull_acms.append(eltree)

    hull_filenames = []

    for acm in hull_acms:
        deps = acm.findall(".//ResourceDependency")
        for dep in deps:
            ident = dep.get("ID", "") 
            name = dep.get("Name", None)
            if ident == "cad_stp.path" and name is not None:
                fixedname = re.sub("_gus\.stp$", "_gus_asm.stp", name)
                hull_filenames.append(os.path.basename(fixedname))

    # assume step files within the same directory tree as the design
    possible_hulls = []
    for dirpath, _, filenames in os.walk(directory):
        ph = [os.path.join(dirpath, filename) for filename in filenames 
                if filename in hull_filenames]
        possible_hulls.extend(ph)

    fellback = False
    if len(possible_hulls) == 0:
        fellback = True
        msg = "WARNING: Unable to find hull model using ComponentACMs .\n"\
              "         Falling back on search for 'e137_*.stp\n"
        sys.stderr.write(msg)
        for dirpath, _, filenames in os.walk(directory):
            ph = [os.path.join(dirpath, filename)
                  for filename in filenames
                  if  filename.lower().startswith("e137")
                  and filename.lower().endswith(".stp")]
            possible_hulls.extend(ph)

    if len(possible_hulls) < 1:
        fellback = True
        msg = "WARNING: Unable to find e137_*.stp\n"\
              "         Falling back on *hull*.stp\n"
        sys.stderr.write(msg)
        for dirpath, _, filenames in os.walk(directory):
            ph = [os.path.join(dirpath, filename)
                  for filename in filenames
                  if  "hull" in filename.lower()
                  and filename.lower().endswith(".stp")]
            possible_hulls.extend(ph)

#    if len(possible_hulls) < 1:
#        fellback = True
#        msg = "WARNING: Unable to find *hull*.stp\n"\
#              "         Falling back on *.stp\n"
#        sys.stderr.write(msg)
#        for dirpath, _, filenames in os.walk(directory):
#            ph = [os.path.join(dirpath, filename)
#                  for filename in filenames
#                  if filename.endswith(".stp")]
#            possible_hulls.extend(ph)

    if len(possible_hulls) < 1:
        msg = "WARNING: No step model found for the hull! "\
              "Reporting zero cost.\n"
        sys.stderr.write(msg)
        raise CostModelError()

    # for now, assume the largest hull model we find is the one we want
    sizes = [os.stat(fn).st_size for fn in possible_hulls]
    biggest = possible_hulls[sizes.index(max(sizes))]

    if len(possible_hulls) > 1:
        msg = "WARNING: multiple possible hull models found, "\
              "using {0}\n"
        sys.stderr.write(msg.format(biggest))
    elif fellback:
        msg = "         Fell back on {0}\n".format(biggest)
        sys.stderr.write(msg)

    return biggest
    
def evaluate_hull(directory, **kwargs):
    with open("hull_cost_data.json", "r") as fp:
        tables = json.load(fp)

    try:
        filename = find_hull(directory)
    except CostModelError:
        return {}

    with open(filename, 'r') as stepfile:
        plates = hde.extract(stepfile)

    ###################################################################
    # WARNING! THIS IS THE WRONG WAY TO SPECIFY MATERIAL TYPE. I'VE   #
    # ADDED THIS CODE FOR EXPLORATORY ANALYSIS, NOT BECAUSE THIS IS   #
    # AN APPROPRIATE WAY TO SPECIFY MATERIAL TYPE.                    #
    # THE VEHICLE DESIGN SHOULD SPECIFY THE MATERIAL TYPE INSTEAD.    #
    ###################################################################
    for plate in plates: # override default
        plate["material"] = kwargs.get("material", plate["material"])
        plate["thickness"] = kwargs.get("thickness", plate["thickness"])

    return hcm.evaluate(plates, tables)

def evaluate(directory, **kwargs):
    components = evaluate_components(directory, **kwargs)
    hull = evaluate_hull(directory, **kwargs)

    hullcost = hull.get("Total Cost", 0)
    hulltime = hull.get("Total Time", 0)
    hullmass = hull.get("Total Mass", 0)

    if hullcost > 0 and hulltime > 0:
        componentlist = components.get("detail", [])
        componentlist = [c for c in componentlist 
                         if (c.get("cost", "no manufacturing model") != "no manufacturing model")
                         and (c.get("leadtime", "no manufacturing model") != "no manufacturing model")]
        componentlist = [c for c in componentlist if re.search(
                    "hull", c.get("name", ""), flags=re.I) is None]

        componentcost = sum([c.get("cost", 0) for c in componentlist])
        componenttime = max([c.get("leadtime", 0) for c in componentlist] + [0])
    else:
        componentcost = components.get("cost", 0)
        componenttime = components.get("leadtime", 0)

    totalcost = componentcost + hullcost
    totaltime = componenttime + hulltime # conservative estimate

    summary = [ {"Name": 'Manufacturing_Lead_Time', 
                 'Value': str(totaltime), 'Unit': 'Days'},
                {"Name": 'Vehicle_Unit_Cost',
                 'Value': str(totalcost), 'Unit': 'Dollars'} ]

    totals = {
            "total lead time (days)"    : totaltime,
            "total cost (dollars)"      : totalcost,
            "hull lead time (days)"     : hulltime,
            "hull cost (dollars)"       : hullcost,
            "hull mass (kilograms)"     : hullmass,
            "component lead time (days)": componenttime,
            "component cost (dollars)"  : componentcost
            }
    verbose = { "components": components, "hull": hull,
                "totals": totals}
    return verbose, summary

def print_results(verbose, summary):
    print json.dumps({"verbose": verbose, "summary": summary}, indent=4)

def write_summaryfile(filename, summary):
    try:
        with open(filename, 'r') as sfp:
            globalsummary = json.load(sfp)
    except IOError:
        msg = "Could not open {0} for reading".format(filename)
        raise CostModelError(msg)
    except ValueError:
        msg = "Could not interpret {0} as json data".format(filename)
        raise CostModelError(msg)

    if not isinstance(globalsummary, dict):
        msg = "Expected to find a dictionary in {0}, found {1} instead"
        raise CostModelError(msg.format(filename, type(globalsummary)))

    try:
        metrics = globalsummary["Metrics"]
    except KeyError:
        msg = 'No "Metrics" key found in {0}'.format(filename)
        raise CostModelError(msg)

    names = [item["Name"] for item in summary]
    indices = []
    for _ in names:
        indices.append([])

    # make a list of indices into the metrics list where each name
    # has a record. the lists "names" and "indices" are indexed the same
    for ii in range(len(metrics)):
        name = metrics[ii].get("Name", None)
        if name in names:
            indices[names.index(name)].append(ii)

    for name, index_list, metric in zip(names, indices, summary):
        if len(index_list) < 1:
            msg = 'WARNING: Cost Model produced the metric "{0}", '\
                  "which is not in {1}\n".format(name, filename)
            sys.stderr.write(msg)
            # raise CostModelError(msg)
            continue

        if len(index_list) > 1:
            msg = "Metric {0} has multiple entries in {1}, refusing "\
                  "to update".format(name, filename)
            raise CostModelError(msg)

        # check if record is OK 
        record = metrics[index_list[0]]
        
        for key in ["Name", "Value", "Unit"]:
            try:
                record.get(key)
            except KeyError:
                msg = 'Record {0} under "Metrics" in {1} is missing '\
                      'key {2}'.format(indices[name], filename, key)
                raise CostModelError(msg)

        # replace the record, updating globalsummary
        metrics[index_list[0]] = metric

    globalsummary["TierLevel"] = 1
    globalsummary["TestBench"] = "Foundry"
    globalsummary["AnalysisStatus"] = "OK"

    with open(filename, 'w') as sfp:
        sfp.write(json.dumps(globalsummary, indent=4))

def write_results(directory, verbose, summary):
    """
    lots of defensive stuff in here to try to enforce and make explicit
    the assumptions embodied in the Vanderbilt code
    """
    summaryfile = os.path.join(directory, "testbench_manifest.json")
    try:
        write_summaryfile(summaryfile, summary)
    except CostModelError as cme:
        msg = "WARNING: Failed to write summary.  "\
                "Message follows:\n\t{0}\n"
        sys.stderr.write(msg.format(cme))

    detailfile = os.path.join(directory, "conceptual_cost.json")
    with open(detailfile, 'w') as dfp:
        dfp.write(json.dumps(verbose, indent=4))

def exhaust_materials(directory):
    """
    evaluate a design repeatedly with all of the material options
    """
    with open("hull_cost_data.json", 'r') as fp:
        hcd = json.load(fp)

    materials = hcd["materials"][1:]
    toprint = ["Material\tThickness (in)\tTotal Cost ($)\tTotal Time (d)\tTotal Mass (lbs)"]
    err = StringIO.StringIO()
    realstderr = sys.stderr

    try:
        for material in materials:
            verbose, summary = evaluate(directory, 
                                        material=material[0][0], 
                                        thickness=material[0][1])
            hull = verbose["hull"]
            report = "\t".join([
                material[0][0], 
                "{0:.3f}".format(material[0][1]).rjust(9, " "), 
                "{0:.2f}".format(hull["Total Cost"]).rjust(9," "),
                "{0:.1f}".format(hull["Total Time"]).rjust(9, " "),
                "{0:.0f}".format(hull["Total Mass"]).rjust(9, " ")
                ])
            toprint.append( report)
            sys.stderr = err # mask sys.stderr after the first iteration
    finally:
        sys.stderr = realstderr

    return "\n".join(toprint)

def cli(argv):
    args = get_args(argv)

    if args.exhaust_materials:
        report = exhaust_materials(args.directory)
        sys.stderr.write("\n\n")
        sys.stderr.flush()
        sys.stdout.flush()
        time.sleep(1)
        print(report)
        exit()

    if args.material is not None and args.thickness is not None:
        verbose, summary = evaluate(args.directory, 
                                    material=args.material, 
                                    thickness=args.thickness)
    elif args.thickness is not None:
        verbose, summary = evaluate(args.directory, 
                                    thickness=args.thickness)
    elif args.material is not None:
        verbose, summary = evaluate(args.directory, 
                                    material=args.material)
    else:
        verbose, summary = evaluate(args.directory) 

    if args.dry_run:
        print_results(verbose, summary)
    else:
        write_results(args.directory, verbose, summary)


if __name__ == "__main__":
    argv = sys.argv
    #argv.append("-d")
    cli(argv)

# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai:colorcolumn=74:number
