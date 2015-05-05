"""
compare_materials.py
Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2013

Launch the cost and leadtime testbench in a special mode to evaluate
all available materials.
"""

import cltestbench
import sys

def cli(argv):
    """
    append --exhaust-materials to argv and launch the testbench
    """
    argv.append("--exhaust-materials")
    cltestbench.cli(argv)

if __name__ == "__main__":
    cli(sys.argv)




# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai:colorcolumn=74:number
