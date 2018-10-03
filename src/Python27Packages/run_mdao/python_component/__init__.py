from __future__ import print_function
from __future__ import absolute_import
import sys
import os
import os.path
import imp
import six
from six.moves import reload_module

from openmdao.core.component import Component


def LoadPythonComponent(filename):
    # support loading adjacent .py files as dependencies
    sys.path.append(os.path.dirname(os.path.abspath(filename)))
    try:
        modname = os.path.splitext(os.path.basename(filename))[0]
        py_suffix = [suffix for suffix in imp.get_suffixes() if suffix[0] == os.path.splitext(filename)[1]][0]
        with open(filename, py_suffix[1]) as modfile:
            # n.b. "If the module was already imported, it is equivalent to a reload()!"
            mod = imp.load_module(modname, modfile, filename, py_suffix)
    finally:
        sys.path.pop()

    def is_component(val):
        return isinstance(val, type) and val.__module__ == modname and Component in val.__mro__
    vals = (getattr(mod, name) for name in dir(mod))
    components = [val for val in vals if is_component(val)]
    if len(components) == 0:
        raise ValueError('No openmdao.core.component.Component found in %s' % modname)
    if len(components) > 1:
        raise ValueError('{} contains {} openmdao.core.component.Components; it must contain exactly 1'.format(modname, len(components)))
    return components[0]

def PythonComponent(filename, kwargs={}):
    kwargs = {key: eval(value) for key, value in six.iteritems(kwargs)}
    return LoadPythonComponent(filename)(**kwargs)
