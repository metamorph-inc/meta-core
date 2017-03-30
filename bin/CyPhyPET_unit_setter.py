"""Converts CyPhy units to OpenMDAO unit strings."""
import sys
# sys.path[0:0] = ['C:\\Users\\kevin\\Documents\\meta-tonka\\bin\\Python27\\lib\\site-packages']
import operator
import openmdao.units.units
import re
import itertools
PhysicalQuantity = openmdao.units.units.PhysicalQuantity
PhysicalUnit = openmdao.units.units.PhysicalUnit
_find_unit = openmdao.units.units._find_unit
_UNIT_LIB = openmdao.units.units._UNIT_LIB


def log(s):
    print s


def log_formatted(s):
    print s
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
    return
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


class InvalidGMEUnitException(ValueError):
    pass


# from OpenMDAO
def in_base_units(value, unit):
    new_value = value * unit.factor
    num = ''
    denom = ''
    for unit, power in zip(_UNIT_LIB.base_names, unit.powers):
        if power < 0:
            denom = denom + '/' + unit
            if power < -1:
                denom = denom + '**' + str(-power)
        elif power > 0:
            num = num + '*' + unit
            if power > 1:
                num = num + '**' + str(power)

    if len(num) == 0:
        num = '1'
    else:
        num = num[1:]

    if new_value != 1:
        return repr(new_value) + '*' + num + denom
    else:
        return num + denom


def reduce_none(op, items):
    return reduce(op, (item for item in items if item is not None))


def get_unit_for_gme(fco, exponent=1):
    if fco.MetaBase.Name == 'si_unit':
        sym = fco.GetStrAttrByNameDisp('Symbol')
        if sym == 'U':
            return None
            # n.b. ignore exponent
            return PhysicalUnit({}, 1.0, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 0.0)
        try:
            return _find_unit(str(sym)) ** exponent
        except:
            raise InvalidGMEUnitException()
    if fco.GetStrAttrByNameDisp('Symbol') in ('degF', 'degC'):
        return _find_unit(str(fco.GetStrAttrByNameDisp('Symbol')))
    if fco.MetaBase.Name == 'derived_unit':
        for exponent in (ref.GetFloatAttrByNameDisp('exponent') for ref in fco.ChildFCOs):
            if int(exponent) != exponent:
                raise ValueError('Only integer and inverse integer exponents allowed')
        return reduce_none(operator.mul, (get_unit_for_gme(ref.Referred, int(ref.GetFloatAttrByNameDisp('exponent'))) for ref in fco.ChildFCOs))
    # log('xxx' + fco.MetaBase.Name)
    if fco.MetaBase.Name == 'conversion_based_unit':
        return reduce_none(operator.mul, (get_unit_for_gme(ref.Referred) * ref.GetFloatAttrByNameDisp('conversion_factor') for ref in fco.ChildFCOs)) ** exponent
    raise ValueError(fco.MetaBase.Name)


def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)


def unit_eq(self, other):
    return isclose(self.factor, other.factor) and \
           self.offset == other.offset and \
           self.powers == other.powers


def convert_unit_symbol(symbol):
    symbol = re.sub('-(?!\\d)', '*', symbol)
    symbol = symbol.replace('^', '**')
    symbol = symbol.replace(u'\u00b5', 'u')
    symbol = re.sub(' */ *', '/', symbol)
    symbol = re.sub(' +', '*', symbol)
    symbol = re.sub('\\bin\\b', 'inch', symbol)
    symbol = re.sub('\\byd\\b', 'yard', symbol)
    # TODO acre?
    # TODO metric tonne?
    return symbol.encode('ascii', 'replace')


# This is the entry point
def invoke(focusObject, rootObject, componentParameters, udmProject, **kwargs):
    if focusObject is not None and focusObject.type.name == "ParameterStudy":
        focusObject = focusObject.parent
    if focusObject is None or focusObject.type.name != "ParametricExploration":
        raise CyPhyPython.ErrorMessageException("Run on ParametricExploration")

    mga_project = focusObject.convert_udm2gme().Project

    for index, gme_id in [(key, value) for key, value in componentParameters.iteritems() if key.startswith('unit_id_')]:
        unit_fco = mga_project.GetFCOByID(gme_id)

        def set_units(units):
            componentParameters[index + "_ret"] = units

        set_unit(unit_fco, set_units)


def set_unit(unit_fco, set_units):
    symbol = unit_fco.GetStrAttrByNameDisp('Symbol')
    debug_log(symbol)
    symbol = convert_unit_symbol(symbol)
    debug_log(symbol)
    try:
        gme_unit = get_unit_for_gme(unit_fco)
    except InvalidGMEUnitException:
            set_units('')
            return
    except TypeError as e:
        if 'cannot multiply units with non-zero offset' in e.message:
            # FIXME: investigate why .../degC always fails
            set_units('')
            return
        else:
            raise
    try:
        sym_unit = _find_unit(symbol)
        if unit_eq(sym_unit, gme_unit):
            debug_log('match ' + unit_fco.Name + ' ' + symbol)
            set_units(symbol)
        # elif unit_fco.MetaBase.Name == 'conversion_based_unit' and \
        #         isclose(round_to_single_precision(sym_unit.factor), round_to_single_precision(gme_unit.factor), rel_tol=1e-5) and \
        #         sym_unit.offset == gme_unit.offset and \
        #         sym_unit.powers == gme_unit.powers:
        #     debug_log('Close ' + unit_fco.Name + '  ' + repr(gme_unit.factor) + '   ' + repr(sym_unit.factor))
        #     child_fcos = unit_fco.ChildFCOs
        #     if child_fcos.Count > 1:
        #         debug_log('Close: too many children')
        #     else:
        #         child_fcos.Item(1).SetFloatAttrByNameDisp('conversion_factor', sym_unit.factor)
        else:
            debug_log('mismatch ' + unit_fco.Name + '  ' + repr(gme_unit) + '   ' + repr(sym_unit))
            raise ValueError('mismatch; using GME version')
    except:
        # log('exception ' + derived_unit.Name + '  ' + symbol)
        # import traceback
        # import cgi
        # log(cgi.escape(traceback.format_exc()))
        debug_log(in_base_units(1, gme_unit))
        set_units(in_base_units(1, gme_unit))


def round_to_single_precision(f):
    import array
    return array.array('f', (f,))[0]


def get_all_unit_fcos(project):
    def sort_gme(fcos):
        ret = list(fcos)
        ret.sort(key=operator.attrgetter('Name'))
        return ret
    # TODO: would it be more performant to crawl the model
    filter = project.CreateFilter()
    filter.Kind = "derived_unit"
    deriveds = sort_gme(project.AllFCOs(filter))
    filter = project.CreateFilter()
    filter.Kind = "si_unit"
    sis = sort_gme(project.AllFCOs(filter))
    filter = project.CreateFilter()
    filter.Kind = "conversion_based_unit"
    cbus = sort_gme(project.AllFCOs(filter))
    return itertools.chain(sis, deriveds, cbus)

# run under gme console: check all units
if __name__ == '__ax_main__':
    gme = gme

    def log(msg):
        # if msg.startswith('Close') or msg.startswith('mismatch'):
        gme.ConsoleMessage(unicode(msg), 1)
    debug_log = log

    reload(openmdao.units.units)
    assert _find_unit('lbf').factor == 4.4482216152605

    gme.MgaProject.BeginTransactionInNewTerr()
    try:
        for derived_unit in get_all_unit_fcos(gme.MgaProject):
            # for derived_unit in cbus:
            # if 'Radian' in derived_unit.Name:
            #     continue
            if 'Steradian' in derived_unit.Name:
                continue
            if derived_unit.Name in ('Pebi', 'Tebi', 'Zebi', 'Exbi', 'Kibi', 'Yotta', 'Yobi', 'Mebi', 'Gibi'):
                continue
            if derived_unit.GetStrAttrByNameDisp('Symbol') in ('U',):
                continue
            link(derived_unit)
            set_unit(derived_unit, log)
    finally:
        gme.MgaProject.CommitTransaction()
