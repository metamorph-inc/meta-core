"""
Exception classes for Modelica executions.
The message is customly written in the code, whereas
sp_msg is the output from a failed subprocess call.
"""


class ModelicaError(Exception):
    """
    Base class for all exceptions in py_modelica
    """

    def __init__(self, message, sp_msg=''):
        super(ModelicaError, self).__init__(message)
        self.message = message
        self.sp_msg = sp_msg


class ModelicaInstantiationError(ModelicaError):
    """
    Raised whenever the instantiation of the simulation tool fails
    """

    def __init__(self, message, sp_msg=''):
        ModelicaError.__init__(self, message, sp_msg)


class ModelicaCompilationError(ModelicaError):
    """
    Raised whenever the compilation of the Modelica code fails.
    """

    def __init__(self, message, sp_msg=''):
        ModelicaError.__init__(self, message, sp_msg)


class ModelicaSimulationError(ModelicaError):
    """
    Raised whenever the simulation of the compiled Modelica code fails.
    """

    def __init__(self, message, sp_msg=''):
        ModelicaError.__init__(self, message, sp_msg)
