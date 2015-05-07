#!/usr/bin/env python
#
# Testing script for the scheduler - run from Cygwin
#

from __future__ import with_statement

import sys, os
import datetime
import unittest
import optparse

if sys.platform == "win32":
    from time import clock
else:
    from time import time as clock

schedtool_path = os.path.dirname(__file__) + "/../../bin/SchedTool.exe"

def no_solution():
    pass

test_list = [
    ( 'simplesched.scs', 'Single task - this one is easy!' ),
    ( 'simpletask_bad.scs', 'Single task - not schedulable.', no_solution ),
    ( 'twotasks_samerate.scs', 'Two tasks at the same rate.' ),
    ( 'twotasks_tworates.scs', 'Two tasks at different rates.' ),
    ( 'twotasks_twoprocs.scs', 'Two procs and one bus.' ),
    ( 'twotasks_twoprocs_twobuses.scs', 'Two procs and two buses.' ),
    ( 'twotasks_latency.scs', 'Three tasks, one latency spec.' ),
    ( 'testfile.scs', 'Two procs, one bus, and latencies.' ),
#RED( 'lots_of_tasks.scs', 'Lots of tasks -- scheduler timing test.' ),
    ( 'rp_autopilot_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_cruise_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_landing_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_takeoff_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ]

err_msgs = [
    'Duration is too large for frequency' ,
    ]

# Make directory with current date/time stamp
# Go over the test list
#   Print the test name, and its description
#   Start timer
#   Invoke the scheduler to run it, redirecting all output
#   Record time
#   Collect results (if any)
#   Add a line to the output structure:
#      Test name
#      Result status (no schedule, schedule filename, err)
#      Run time
#
# If --xml switch provied, create an JUnit compatible XML page summarizing the results

class SchedToolTest(unittest.TestCase):
    def __init__(self, test, t):
        unittest.TestCase.__init__(self, test)
        self.t = t

    def id(self):
        class_name = "%s" % self.__class__
        class_name = class_name.rsplit('.', 1)[1].split('\'',1)[0]
	test_name = self.t[0].split('.', 1)[0]
        return "%s.%s" % (class_name, test_name)

    def test(self):
        t = self.t
        print "<TEST>"
        print t[0] + ": " + t[1]
        logfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.log'
        rsltfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.rslt'
        import shutil
        shutil.copyfile(t[0], os.path.join(dirname, t[0]))
        exec_str = schedtool_path + " -f "
        exec_str = exec_str + dirname + "/" + t[0] 
        exec_str = exec_str + " -o " + rsltfilename
        exec_str = exec_str + " > " + logfilename + " 2>&1"
        print "\tExecuting command: " + exec_str
        start = clock()
        os.system( exec_str )
        runtime = clock() - start  # approximate runtime
        print "\tThis test took " + str(runtime) + " seconds."

        # See if we had any solutions...
        grepfilename = os.path.join(dirname, os.path.splitext(t[0])[0]) + ".grp"
        with open(logfilename) as logfile:
            log = logfile.readlines()

        nosolutions = filter(lambda x: x.find("No solutions found") != -1, log)
        if len(t) > 2 and t[2] == no_solution:
            if not nosolutions:
                raise Exception("Solution found but not expected!")
        else:
            if nosolutions:
                raise Exception("No solutions found!")
            
        # See if we had any errors...
        errors = []
        for err in err_msgs:
            errors.extend(filter(lambda x: x.find(err) != -1, log))
        if errors:
            raise Exception("\n".join(errors))
        print "</TEST>"
        print

if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option("-x", "--xml", action="store_true", dest="xml")

    (options, args) = parser.parse_args()


    dirname = "results"
    if not os.path.isdir(dirname):
        os.mkdir( dirname )
    suite = unittest.TestSuite()
    for t in test_list:
        test = SchedToolTest("test", t)
        suite.addTest(test)

    if options.xml:
        import xmlrunner
        with open("tests.xml", "w") as output:
            output.write("<testsuites>")
            for test in suite._tests:
                runner = xmlrunner.XMLTestRunner(output)
                runner.run(test)
            output.write("</testsuites>")
    else:
        runner = unittest.TextTestRunner()
        runner.run(suite)
