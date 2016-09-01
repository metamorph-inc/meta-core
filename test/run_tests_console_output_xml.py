import os
import sys
import errno
import os.path
import glob
import time
from xml.etree import ElementTree
import subprocess
import threading

_this_dir = os.path.dirname(os.path.abspath(__file__))
_lock = threading.Lock()

def fixup_nunit_xml(filename, name=None):
    if name is None:
        name = os.path.basename(filename)
    result = ElementTree.parse(filename)
    result_root = result.getroot()
    if result_root.tag == 'test-results':
        test_results = result_root
    else:
        test_results = ElementTree.Element('test-results')
        test_results.append(result_root)
    test_results.set('name', name)
    test_results.set('total', str(len(result_root.findall('.//test-case'))))
    test_results.set('failures', str(len(result_root.findall('.//test-case[@success="False"]'))))
    test_results.set('not-run', str(len(result_root.findall('.//test-case[@executed="False"]'))))
    test_results.set('date', time.strftime('%Y-%m-%d'))
    test_results.set('time', time.strftime(time.strftime('%H:%M:%S')))
    ElementTree.ElementTree(test_results).write(filename)

def run_test(filename, cwd, result_dir):
    result_xml = os.path.join(result_dir, os.path.basename(filename) + '_result.xml')
    if os.path.exists(result_xml):
        os.unlink(result_xml)
    try:
        os.makedirs(os.path.dirname(result_xml))
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise

    # xunit redirects .NET stdout/stderr. Send other output to a log
    with open(result_xml + '.log', 'wb') as logfile:
        process = subprocess.Popen([sys.executable, os.path.join(_this_dir, '..\\run_in_job_object.py'), # n.b. without this, if a test creates a child process which doesn't exit, we hang reading its stdout
                                os.path.join(_this_dir, r'..\3rdParty\xunit-1.9.1\xunit.console.clr4.x86.exe'),
                                filename, '/nunit', result_xml, '/html', result_xml + '.html'],
                                cwd=cwd, stdout=logfile, stderr=subprocess.STDOUT)
        # output, unused_err = process.communicate()
        retcode = process.wait()
    with _lock:
        if retcode != 0:
            # xunit exe returns 1 when a test fails
            if os.path.exists(result_xml):
                print "Test '" + filename + "' failed"
            else:
                print "Error while testing ", filename, " return code: ", retcode
        if os.path.exists(result_xml):
            fixup_nunit_xml(result_xml)
            return result_xml

        print "test warning: %s was not produced" % result_xml 
        with open(result_xml + '.log', 'rb') as logfile:
            print " xunit.exe output: " + logfile.read()
        return None

def get_test_assemblies(xunit_file):
    tree = ElementTree.parse(xunit_file)
    root = tree.getroot()
    return (os.path.join(os.path.dirname(os.path.abspath(xunit_file)), node.attrib['filename']) for node in root.findall('.//assembly'))

def amalgamate_nunit_xmls(xml_files, output_filename):
    test_results = ElementTree.Element('test-results')
    for xml_file in xml_files:
        result_root = ElementTree.parse(xml_file).getroot()
        for tag in ('test-suite', 'test-case'):
            for element in result_root.findall('./' + tag):
                test_results.append(element)
    
    ElementTree.ElementTree(test_results).write(output_filename)
    fixup_nunit_xml(output_filename)

def clean_result_dir(result_dir=None):
    try:
        os.mkdir(result_dir)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(result_dir):
            pass
        else:
            raise
    for file in glob.glob(result_dir + "/*_result.xml"):
        #print "Unlink " + file
        os.unlink(file)
    
def run_tests_only(xunit_file, cwd=None, result_dir=None):
    if result_dir is None:
        result_dir = os.path.join(os.path.dirname(os.path.abspath(xunit_file)), 'results')

    if cwd is None:
        cwd = os.path.dirname(os.path.abspath(xunit_file))
    assemblies = get_test_assemblies(xunit_file)

    test_xmls = []
    for filename in assemblies:
        test_xml = run_test(filename, cwd, result_dir)
        if test_xml is not None:
            test_xmls.append(test_xml)
    return test_xmls

def run_tests(xunit_file, cwd=None, result_dir=None):
    clean_result_dir(result_dir)
    return run_tests(xunit_file, cwd, result_dir)

if __name__ == '__main__':
    xunit_files = sys.argv[1:] if len(sys.argv) >= 2 else ['tests.xunit']
    test_xmls = []
    clean_result_dir(os.path.join(_this_dir, 'results'))
    for xunit_file in xunit_files:
        test_xmls.extend(run_tests_only(os.path.join(_this_dir, xunit_file)))
    amalgamate_nunit_xmls(test_xmls, 'nunit_results.xml')
    #import glob
    #amalgamate_nunit_xmls(glob.glob('results/*'), 'nunit_results.xml')
