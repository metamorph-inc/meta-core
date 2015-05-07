# -*- Mode: Python -*-
#
# This script is designed to move temporary log file into 
# right filename when FAME_Possible_Faults testbench failed to execute correctly.


import sys, os

script_dir = os.path.dirname(os.path.realpath(__file__))
output_dir = os.path.abspath(os.path.join(script_dir,"../"))

if os.path.isfile(os.path.join(script_dir,"FaultSimulation.json")):
    os.remove(os.path.join(script_dir,'output'))
else:
    os.rename(os.path.join(output_dir,'output'),os.path.join(output_dir,'_FAILED.txt'))

    
