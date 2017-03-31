"""Converts OpenMDAO unit strings to a matching CyPhy unit FCO."""
# sys.path[0:0] = ['C:\\Users\\kevin\\Documents\\meta-tonka\\bin\\Python27\\lib\\site-packages']
from __future__ import absolute_import
from __future__ import print_function
import json
import operator
import re
import itertools

import CyPhyPET_unit_setter
import run_mdao.python_component
import run_mdao.python_component.get_params_and_unknowns

import openmdao.units.units
import six
PhysicalQuantity = openmdao.units.units.PhysicalQuantity
PhysicalUnit = openmdao.units.units.PhysicalUnit
_find_unit = openmdao.units.units._find_unit
_UNIT_LIB = openmdao.units.units._UNIT_LIB


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
def invoke(focusObject, rootObject, componentParameters, udmProject, **kwargs):
    mga_project = focusObject.convert_udm2gme().Project

    c = run_mdao.python_component.PythonComponent(componentParameters['openmdao_py'])

    all_units = []

    def set_units(symbol):
        if symbol is None or symbol == '':
            return
        all_units.append((_find_unit(symbol), unit_fco))
    for unit_fco in CyPhyPET_unit_setter.get_all_unit_fcos(mga_project):
        if unit_fco.Name in ('Pebi', 'Tebi', 'Zebi', 'Exbi', 'Kibi', 'Yotta', 'Yobi', 'Mebi', 'Gibi'):
            continue
        if unit_fco.GetStrAttrByNameDisp('Symbol') in ('U',):
            continue
        CyPhyPET_unit_setter.set_unit(unit_fco, set_units)

    for param_name, metadata in itertools.chain(six.iteritems(c._init_params_dict), six.iteritems(c._init_unknowns_dict)):
        unit_expr = metadata.get('units')
        if not unit_expr:
            continue
        # debug_log(repr(unit_expr))
        openmdao_unit = _find_unit(unit_expr)

        gme_unit_id = None
        for unit, fco in all_units:
            if unit == openmdao_unit:
                if gme_unit_id is None:
                    gme_unit_id = fco.ID
                # prefer a unit with Symbol that matches (e.g. m^3 vs stere)
                if CyPhyPET_unit_setter.convert_unit_symbol(fco.GetStrAttrByNameDisp('Symbol')) == unit_expr:
                    gme_unit_id = fco.ID
                    break
        if gme_unit_id is not None:
            metadata['gme_unit_id'] = gme_unit_id
        # else: TODO create a new unit

    componentParameters['ret'] = json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict},
        default=run_mdao.python_component.get_params_and_unknowns.json_default)
    # import cgi
    # debug_log(cgi.escape(componentParameters['ret']))
