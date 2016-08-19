import os
import sys
import time
import logging

try:
    import py_modelica as pym
    print 'Found py_modelica in virtual python environment'
except ImportError as err:
    print err.message
    print 'Use META virtual python environment'

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", "--driver", dest="driver",
                  help="Name of the driver to be executed")

if __name__=='__main__':
    if not os.path.isdir('log'):
        os.mkdir('log')
    log = pym.instantiate_logger(os.path.join('log', 'debug.log'))
    (options, args) = parser.parse_args()
    driver_name = options.driver
    driver = __import__(driver_name)
    driver.main()
