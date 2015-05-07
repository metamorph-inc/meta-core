#!/usr/bin/env python
#
# Run the test as follows:

# Setup
# 1. Copy the binary over

# Running
# For each input data file:
# 1. Copy the file over
# 2. Run the test on the newly transferred file
# 3. Record the running time
# 4. Collect the result file and the running time

import sys, os, re
from glob import glob
import pexpect

target = "root@192.168.0.195"

def auth(cmd):
	os.system( cmd )
	#child = pexpect.spawn( cmd )
	#child.expect( "root@192.168.0.195's password: " )
	#child.sendline( "gumstix" )

def setup():
	auth( 'scp TestOuterLoop ' + target + ":oltest/")
	auth( 'ssh ' + target + ' "chmod u+x oltest/TestOuterLoop"' )
	

def runloop():
	infiles = glob( '../inputs/*.dat' );
	for inf in infiles:
		(pre, dirname, infname) = inf.split( '/' );
                outfname = infname.replace( 'sin', 'rslt', 1 )
		logfname = infname.replace( 'sin', 'log', 1 )
	 	auth( 'scp ' + inf + ' ' + target + ':oltest/' )
		auth( 'ssh ' + target + ' "cd oltest; time ./TestOuterLoop ' + infname + ' > log 2>&1"' )
		auth( 'scp ' + target + ':oltest/rslt.dat ../ol_outputs_pf/' + outfname )
		auth( 'scp ' + target + ':oltest/log ../ol_outputs_pf/' + logfname )
		auth( 'ssh ' + target + ' "rm -f oltest/' + infname + ' oltest/rslt.dat oltest/log"' )

if __name__ == "__main__":
	if ( sys.argv[1] == 'init' or sys.argv[1] == 'setup' ):
		setup()
	elif ( sys.argv[1] == 'run' ):
		runloop()
	else:
		print "Usage:"
		print "   test_ol.py <init | run>"

