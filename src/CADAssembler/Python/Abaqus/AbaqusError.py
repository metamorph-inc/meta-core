"""

AbaqusError.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Robert Boyles <rboyles@isis.vanderbilt.edu>

    - User defined error class

"""

import logging

class Error(Exception):
    pass

class ConstraintException(Error):
    # #def __init__(self, value):
        # #self.expr = expr
        # #self.msg = msg
    pass 
    
class ThermalConstraintException(Error):
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return repr(self.msg)
        
        
class InitialTempException(Error):
    def __init__(self):
        logger = logging.getLogger()
        logger.error('Initial Temperature is set to be applied to ' + \
                     'specific face rather than an entire component/assembly. ' + \
                     'This is not a valid case. Please revise your test bench.')
                     

class InitialTempMissingException(Error):
    def __init__(self):
        logger = logging.getLogger()
        logger.error('No initial temperature specified. Invalid thermal analysis.')
        

class ConflictingThermalConstraintException(Error):
    def __init__(self):
        logger = logging.getLogger()
        logger.error('Conflicting constraints are present!')