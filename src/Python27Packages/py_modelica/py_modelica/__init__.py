#!/usr/bin/env python

"""
This package provides a common API for dealing with Modelica models in an automated
way using different Modelica tools. Currently Dymola and OpenModelica are supported.
DymolaDemo has not been maintained and the user might experience inconveniences
when using this.

Future tool support include JModelica.org.
"""

__authors__ = ["Patrik Meijer", "Zsolt Lattmann"]
__author__ = __authors__[0]

__credits__ = __authors__

__version__ = "14.13" #Versioning: http://www.python.org/dev/peps/pep-0386/

__maintainer__ = __author__
__contact__ = __author__
__email__ = "patrik85@isis.vanderbilt.edu"

__status__ = "Production"
__url__ = 'https://svn.isis.vanderbilt.edu/META/trunk/src/Python27Packages/' + __name__

__copyright__ = "Copyright (C) 2011-2013, Vanderbilt University"
#__license__ = "MIT"

from py_modelica.exception_classes import ModelicaError
from py_modelica.modelica_simulation_tools.om_class import OpenModelica
from py_modelica.modelica_simulation_tools.dymola_class import Dymola
from py_modelica.modelica_simulation_tools.jmodelica import JModelica
from py_modelica.utility_functions import instantiate_logger, run_post_scripts, \
    get_simscript_opts_and_args, write_out_tool_statistics
from py_modelica.report_functions import get_parameters_from_report_json, \
    update_analysis_status, update_metrics_and_check_limits, check_for_invalid_numbers