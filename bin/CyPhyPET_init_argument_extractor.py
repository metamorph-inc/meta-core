"""Gets the __init__ arguments from an OpenMDAO Component"""
from __future__ import absolute_import
from __future__ import print_function
import json
import inspect
import run_mdao.python_component


def log(s):
    print(s)


def log_formatted(s):
    print(s)


try:
    import CyPhyPython  # will fail if not running under CyPhyPython
    import cgi

    def log_formatted(s):
        CyPhyPython.log(s)

    def log(s):
        CyPhyPython.log(cgi.escape(s))
except ImportError:
    pass


def debug_log(s):
    # return
    log(s)


def link(fco):
    gme.ConsoleMessage('<a href="mga:{}">{}</a>'.format(fco.ID, fco.Name), 1)


def start_pdb():
    """Start pdb, the Python debugger, in a console window."""
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    import sys
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


# This is the entry point
def invokeGME(project, rootObject, focusObject, componentParameters, **kwargs):
    mga_project = project

    component_class = run_mdao.python_component.LoadPythonComponent(componentParameters['openmdao_py'])

    # FIXME: defaults may have values that are not json-serializable
    componentParameters['ret'] = json.dumps(inspect.getargspec(component_class.__init__))
    componentParameters['icon_path'] = getattr(component_class, 'icon_path')
