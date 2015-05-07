import glob
import logging
from optparse import OptionParser
import os
import sys
import json
from lxml import etree
from collections import Counter

# ------------------ setting up logger ------------------------------
# create logger with 'python_sample_analysis_tool'
logger = logging.getLogger('python_sample_analysis_tool')
logger.setLevel(logging.DEBUG)

# create file handler which logs even debug messages
if not os.path.isdir('log'):
    os.mkdir('log')

fh = logging.FileHandler(os.path.join('log', 'python_sample_analysis_tool.log'))
fh.setLevel(logging.DEBUG)

# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.WARNING)

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(fh)
logger.addHandler(ch)


# ------------------ functionality implementation ------------------

# find adm file/files
def transform_adm(adm_file):

    # FIXME: check if file exists

    # read adm file
    tree = etree.parse(adm_file)

    logger.debug("transforming: %s", adm_file)

    num_components = 0
    component_classes = Counter()

    # get all component instances at every level
    for component in tree.findall('.//ComponentInstance'):
        num_components += 1
        component_classes[component.get('ComponentID')] += 1

    return {
        'count_components': num_components,
        'component_classes': component_classes
    }


# perform analysis
def count_components(adm_json):

    return adm_json['count_components']


def bill_of_materials(adm_json):

    return {'number_of_component_classes': len(adm_json['component_classes'].keys())}


# report results
def update_manifest_json(metric_name, value):
    with open('testbench_manifest.json', 'r') as f_p:
        test_bench_manifest = json.load(f_p)

    found = False

    for metric in test_bench_manifest['Metrics']:
        if metric['Name'] == metric_name:
            metric['Value'] = str(value)
            found = True
            break

    if found:
        with open('testbench_manifest.json', 'w') as f_p:
            json.dump(test_bench_manifest, f_p)
        logger.info('Metric was found and updated %s, %s', metric_name, str(value))
        return 0
    else:
        logger.error('Metric was NOT found %s, %s', metric_name, str(value))
        return 1

# ------------------ initialize variables --------------------------
def main():
    parser = OptionParser()
    parser.add_option("-a", "--adm", type='string', action="store",
                      help='AVM design model file. If it is not defined, runs on the first ADM file in the directory.')
    parser.add_option("-b", "--bom", action="store_true", default=False,
                      help='Exports bill of material component list.')
    parser.add_option("-c", "--components", action="store_true", default=False,
                      help='Counts number of components used in the design.')

    (opts, args) = parser.parse_args()

    working_dir = os.getcwd()

    logger.debug("working directory: %s", working_dir)

    if opts.adm:
        adm_file = opts.adm
    else:
        adm_file = glob.glob('*.adm')[0]

    logger.debug("given adm file: %s", adm_file)

    adm_data = transform_adm(adm_file)

    with open('adm_analysis_results.json', 'w') as f_p:
        json.dump(adm_data, f_p)

    if opts.bom:
        logger.info("reporting bill of material")
        bom = bill_of_materials(adm_data)
        return update_manifest_json('NumberOfComponentClasses', bom['number_of_component_classes'])

    if opts.components:
        logger.info("reporting number of components")
        number_of_components = count_components(adm_data)
        return update_manifest_json('NumberOfComponents', number_of_components)

    parser.print_help()
    parser.error("Incorrect number of arguments --bom or --components has to be defined. BOM has precedence.")

# ------------------ call main -------------------------------------
if __name__ == '__main__':
    sys.exit(main())