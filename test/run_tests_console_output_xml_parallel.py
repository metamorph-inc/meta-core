import os
import os.path
import time
from xml.etree import ElementTree
import subprocess
from multiprocessing.pool import ThreadPool
import sys
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

    args = parser.parse_args()

    _this_dir = os.path.dirname(os.path.abspath(__file__))
    xml_files = main(*[(xunit_file, os.path.abspath(os.path.join(os.path.dirname(xunit_file), 'results'))) for xunit_file in args.xunit_files])
    run_tests_console_output_xml.amalgamate_nunit_xmls([xml_file for xml_file in xml_files if xml_file is not None], os.path.join(_this_dir, 'nunit_results.xml'))

    if args.start_failed:
        from xml.etree import ElementTree
        for xml_file in xml_files:
            if int(ElementTree.parse(xml_file).getroot().get('failures')):
                subprocess.check_call(['cmd.exe', '/c', 'start', xml_file + '.html'])
