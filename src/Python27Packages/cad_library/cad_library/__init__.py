#!/usr/bin/env python

"""
Common API and functionality to be used in the CAD python scripts (FEA, Kinematics, etc.)
"""

__authors__ = ["Sandor Nyako", "Robert Owens", "Robert Boyles"]
__author__ = __authors__[0]

__credits__ = __authors__

__version__ = "14.13"  # Versioning: http://www.python.org/dev/peps/pep-0386/

__maintainer__ = __author__
__contact__ = __author__
__email__ = "snyako@isis.vanderbilt.edu"

__status__ = "Production"
__url__ = 'https://svn.isis.vanderbilt.edu/META/trunk/src/Python27Packages/' + __name__

__copyright__ = "Copyright (C) 2011-2014, Vanderbilt University"
#__license__ = "MIT"

import six.moves.winreg

# Retrieve META_PATH
with six.moves.winreg.OpenKey(six.moves.winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0,
                     six.moves.winreg.KEY_READ | six.moves.winreg.KEY_WOW64_32KEY) as key:
    META_PATH = six.moves.winreg.QueryValueEx(key, 'META_PATH')[0]

from cad_library.__main__ import exitwitherror
from cad_library.__main__ import setuplogger
from cad_library.cadapi import AssemblyInfo
from cad_library.cadapi import CalculixResults
from cad_library.cadapi import Vec3
from cad_library.cadapi import Mat4x4
from cad_library.cadapi import ComponentData
from cad_library.cadapi import CADMetrics
