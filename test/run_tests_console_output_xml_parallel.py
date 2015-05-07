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
    _this_dir = os.path.dirname(os.path.abspath(__file__))
    xml_files = main((os.path.join(_this_dir, 'tests.xunit'), os.path.join(_this_dir, 'results')))
    run_tests_console_output_xml.amalgamate_nunit_xmls([xml_file for xml_file in xml_files if xml_file is not None], os.path.join(_this_dir, 'nunit_results.xml'))

