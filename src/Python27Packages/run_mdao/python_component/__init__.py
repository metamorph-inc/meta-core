from __future__ import print_function
from __future__ import absolute_import
import sys
import os
import os.path
import importlib
from six.moves import reload_module

from openmdao.core.component import Component


def PythonComponent(filename):
    sys.path.insert(0, os.path.dirname(os.path.abspath(filename)))
    try:
        modname = os.path.splitext(os.path.basename(filename))[0]
        mod = importlib.import_module(modname)
        reload_module(mod)

        def is_component(val):
            return isinstance(val, type) and val.__module__ == modname and Component in val.__mro__
        vals = (getattr(mod, name) for name in dir(mod))
        components = [val for val in vals if is_component(val)]
        if len(components) == 0:
            raise ValueError('No openmdao.core.component.Component found in %s' % modname)
        if len(components) > 1:
            raise ValueError('{} contains {} openmdao.core.component.Components; it must contain exactly 1'.format(modname, len(components)))
        component = components[0]
        return component()
    finally:
        sys.path[:1] = []
