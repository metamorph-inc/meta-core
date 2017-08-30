import os
import os.path
import time
from xml.etree import ElementTree
import subprocess
from multiprocessing.pool import ThreadPool
import sys
import glob
import run_tests_console_output_xml

def star_test(args):
    return run_tests_console_output_xml.run_test(*args)

def main(*xunitfile_and_result_dirs):
    tests = []
    for xunit_filename, result_dir in xunitfile_and_result_dirs:
        test_dir = os.path.dirname(os.path.abspath(xunit_filename))

        tree = ElementTree.parse(xunit_filename)
        root = tree.getroot()
        assemblies = root.findall('.//assembly')

        for filename in (node.attrib['filename'] for node in assemblies):
            tests.append((filename, test_dir, result_dir))

    threads = ThreadPool()
    return threads.map_async(star_test, tests).get()

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Run xunit in parallel.')
    parser.add_argument('xunit_files', nargs='*', default=['tests.xunit'])
    parser.add_argument('--start-failed', action='store_true')
    parser.add_argument('--rerun-failed', action='store_true')

    args = parser.parse_args()

    _this_dir = os.path.dirname(os.path.abspath(__file__))
    if args.rerun_failed:
        xunit_xml = ElementTree.Element('xunit')
        xunit_assemblies = ElementTree.SubElement(xunit_xml, 'assemblies')
        for result_file in (f for f in glob.glob(_this_dir + '/results/*.xml') if not f.endswith('.xunit.xml')):
            tree = ElementTree.parse(result_file)
            root = tree.getroot()
            assemblies = root.findall('./test-suite[@success="False"]')
            for f in assemblies:
                assembly = ElementTree.SubElement(xunit_assemblies, 'assembly')
                assembly.attrib['filename'] = f.attrib['name']
                assembly.attrib['shadow-copy'] = 'true'

        with open('failed.xunit', 'wb') as out_xml:
            out_xml.write(ElementTree.tostring(xunit_xml))

        args.xunit_files = ['failed.xunit']

    xml_files = main(*[(xunit_file, os.path.abspath(os.path.join(os.path.dirname(xunit_file), 'results'))) for xunit_file in args.xunit_files])
    run_tests_console_output_xml.amalgamate_nunit_xmls([xml_file for xml_file in xml_files if xml_file is not None], os.path.join(_this_dir, 'nunit_results.xml'))
    run_tests_console_output_xml.transform_xunit_xmls([xml_file + '.xunit.xml' for xml_file in xml_files if xml_file is not None], os.path.join(_this_dir, 'junit_results.xml'))

    if args.start_failed:
        from xml.etree import ElementTree
        for xml_file in xml_files:
            if xml_file is None:
                continue
            if int(ElementTree.parse(xml_file).getroot().get('failures')):
                subprocess.check_call(['cmd.exe', '/c', 'start', xml_file + '.html'])
    # exit with number of missing results (e.g. xunit.exe crash)
    sys.exit(xml_files.count(None))
