#!/usr/bin/env python
#
# Testing script for the scheduler - run from Cygwin
#

import sys, os
import datetime

if sys.platform == "win32":
    from time import clock
else:
    from time import time as clock

schedtool_path = "/cygdrive/c/Documents\ and\ Settings/Administrator/Desktop/HCDDES/trunk/bin/SchedToolD.exe"

test_list = [
    ( 'simplesched.scs', 'Single task - this one is easy!' ),
    ( 'simpletask_bad.scs', 'Single task - not schedulable.' ),
    ( 'twotasks_samerate.scs', 'Two tasks at the same rate.' ),
    ( 'twotasks_tworates.scs', 'Two tasks at different rates.' ),
    ( 'twotasks_twoprocs.scs', 'Two procs and one bus.' ),
    ( 'twotasks_twoprocs_twobuses.scs', 'Two procs and two buses.' ),
    ( 'twotasks_latency.scs', 'Three tasks, one latency spec.' ),
    ( 'testfile.scs', 'Two procs, one bus, and latencies.' ),
    ( 'lots_of_tasks.scs', 'Lots of tasks -- scheduler timing test.' ),
    ( 'rp_autopilot_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_cruise_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_landing_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ( 'rp_takeoff_mode.scs', 'Scheduling in TT sys under mode changes.' ),
    ]

err_msgs = [
    'Duration is too large for frequency' 
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
# Create an html page for the output and display it

def runTests(args):
    dirname = "results_" + datetime.datetime.now().strftime("%a_%m%d%Y_%H%M%S")
    os.mkdir( dirname )
    for t in test_list:
        print t[0] + ": " + t[1]
        logfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.log'
        rsltfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.rslt'
        os.system( 'cp Test/' + t[0] + ' ' + dirname )
        exec_str = schedtool_path + " -f "
        exec_str = exec_str + dirname + "/" + t[0] 
        exec_str = exec_str + " -o " + rsltfilename
        exec_str = exec_str + " > " + logfilename + " 2>&1"
        print "Executing command: " + exec_str
        start = clock()
        os.system( exec_str )
        runtime = clock() - start  # approximate runtime
        print "It took " + str(runtime) + " seconds."

        # See if we had any solutions...
        grepfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.grp'
        os.system( 'grep "No solutions found" ' + logfilename + ' > ' + grepfilename )
        
        statinfo = os.stat( grepfilename )
        if ( statinfo[6] > 0 ):
            print "No solutions found!"

        # See if we had any errors...
        for err in err_msgs:
            errfilename = dirname + '/' + t[0][:(t[0].rfind('.'))] + '.err'
            os.system( 'grep "' + err + '" ' + logfilename + ' >> ' + errfilename )
        statinfo = os.stat( errfilename )
        if ( statinfo[6] > 0 ):
            print "Schedule determination error! (see log file)"

        print

if __name__ == "__main__":
   runTests(sys.argv)

