# to run with XML output:
# CurrentDirectory>c:\Python27\python.exe __main__.py -x

import sys
import os
import os.path
import unittest
import unittest.loader

if sys.version <= (2.7):
    raise Exception("Python versions less than 2.7 are unsupported")

def run_xmlrunner(tests, output_filename):
    import os.path
    import xmlrunner
    results = []
    with open(output_filename, "w") as output:
        output.write("<testsuites>")
        for test in tests:
            runner = xmlrunner.XMLTestRunner(output)
            runner.run(test)
        output.write("</testsuites>")


if __name__=='__main__':
    tests = unittest.loader.defaultTestLoader.discover(os.path.dirname(os.path.abspath(__file__)))
    import optparse
    parser = optparse.OptionParser()
    parser.add_option("-x", "--xml", action="store_true", dest="xml")
    parser.add_option("-v", action="store_true", dest="verbose")

    (options, args) = parser.parse_args()
    if options.xml:
        run_xmlrunner(tests, 'tests.xml')
    else:
        runner = unittest.TextTestRunner(verbosity=2 if options.verbose else 1)
        runner.run(tests)
