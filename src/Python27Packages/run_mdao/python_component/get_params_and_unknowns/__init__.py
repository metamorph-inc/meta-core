from __future__ import absolute_import
import numpy
from openmdao.api import FileRef


def json_default(obj):
    if isinstance(obj, FileRef):
        return repr(obj)
    if isinstance(obj, numpy.ndarray):
        return repr(obj)
    if isinstance(obj, type):
        return obj.__name__
    raise TypeError(repr(obj) + " is not JSON serializable")
