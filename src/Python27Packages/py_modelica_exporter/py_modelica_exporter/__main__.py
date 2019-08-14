from __future__ import absolute_import

from __future__ import print_function
import py_modelica_exporter
from optparse import OptionParser
import sys
import os

from py_modelica_exporter.exporters import ComponentExporter, TreeExporter, PackageExporter, ParsingException, ComponentAssemblyExporter

import logging
import timeit

logger = logging.getLogger('py_modelica_exporter')
logger.setLevel(logging.DEBUG)

# create file handler which logs even debug messages
if not os.path.isdir('log'):
    os.mkdir('log')

fh = logging.FileHandler(os.path.join('log', 'py_modelica_exporter.log'))
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


# get class name to export
def main():
    parser = OptionParser()
    parser.add_option("-v", "--version", action="store_true", default=False,
                      help='Displays the version number of this package.')
    parser.add_option("-c", "--components", action="store", type="string",
                      help='A semicolon-separated list of components to export.')
    parser.add_option("-j", "--json", action="store_true", default=False,
                      help='Exports the component in json format.')
    parser.add_option("-t", "--tree", action="store", type="string",
                      help='Export tree starting from this component')
    parser.add_option("-p", "--packages", type="string", action="store",
                      help='Load semicolon-separated list of external package.mo paths')
    parser.add_option("-f", "--config", type="string", action="store",
                      help='Load list of external packages from config file')
    parser.add_option("-x", "--xml", action="store_true", default=False,
                      help='Exports the component in xml format.')
    parser.add_option("-a", "--assemblies", action="store", type="string",
                      help='A semicolon-separated list of component-assemblies to export.')
    parser.add_option("-s", "--standard", action="store_true", default=False,
                      help='List the models from the MSL')
    parser.add_option("-i", "--icons", action="store_true", default=False,
                      help='Export an icon.svg based on each model annotation.')

    (opts, args) = parser.parse_args()

    if args:
        parser.print_help()
        return 1
    if opts.version:
        print((py_modelica_exporter.__version__))
    elif opts.components:
        if opts.packages:
            external_packages = [p for p in opts.packages.split(';') if p]
        else:
            external_packages = []

        if opts.icons:
            component_exporter = ComponentExporter(external_packages, export_icons=True)
        else:
            component_exporter = ComponentExporter(external_packages, export_icons=False)

        components_to_export = [c for c in opts.components.split(';') if c]

        extracted_components = []
        for modelica_uri in components_to_export:
            if opts.json:
                start_time = timeit.default_timer()
                extracted_components.append(component_exporter.get_component_json(modelica_uri))
                stop_time = timeit.default_timer()

                total_time = stop_time - start_time
                logger.info('Extracted info for {0} - {1} seconds'.format(modelica_uri, total_time))

        if opts.json:
            import json
            with open('modelica_components.json', 'w') as f_out:
                json.dump(extracted_components, f_out)

    elif opts.tree:
        tree_to_export = opts.tree

        tree_exporter = TreeExporter(tree_to_export)

        if opts.json:
            #treeExporter.export_to_json(treeToExport + '.tree.json')
            tree_exporter.export_to_json('ModelicaPackages.tree.json')
        if opts.xml:
            tree_exporter.export_to_xml(tree_to_export + '.tree.xml')

    elif opts.packages:
        external_packages = [p for p in opts.packages.split(';') if p]

        package_exporter = PackageExporter(external_packages, load_msl=opts.standard)

        if opts.json:
            package_exporter.package_names.sort()
            package_exporter.export_to_json('ModelicaPackages.tree.json')
            #package_exporter.exportToJson("_".join(package_exporter.externalPackageNames) + '.tree.json')

    elif opts.standard:
        package_exporter = PackageExporter([], load_msl=opts.standard)

        if opts.json:
            package_exporter.package_names.sort()
            package_exporter.export_to_json('ModelicaPackages.tree.json')

    elif opts.config:
        external_package_file = opts.config
        logger.info('loading packages from "{0}" ... '.format(external_package_file))

        package_exporter = PackageExporter(external_package_file, load_msl=opts.standard)

        if opts.json:
            package_exporter.package_names.sort()
            package_exporter.export_to_json("_".join(package_exporter.package_names) + '.tree.json')

    elif opts.assemblies:

        if opts.packages:
            external_packages = [p for p in opts.packages.split(';') if p]
        else:
            external_packages = []
        assembly_uris = [c.strip() for c in opts.assemblies.split(';') if c]
        extracted_assemblies = []

        component_assembly_exporter = ComponentAssemblyExporter(external_packages)
        for modelica_uri in assembly_uris:
            extracted_assemblies.append(component_assembly_exporter.get_component_assembly_json(modelica_uri))

        if opts.json:
            import json
            with open('component_assemblies.json', 'w') as f_out:
                json.dump(extracted_assemblies, f_out)
    else:
        parser.print_help()
        return 1

    return 0


sys.exit(main())
