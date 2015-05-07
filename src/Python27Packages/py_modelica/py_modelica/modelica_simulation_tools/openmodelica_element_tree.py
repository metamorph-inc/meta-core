import logging
from xml.etree import ElementTree
import numpy as np


class OpenModelicaElementTree():
    """
    Class containing an ElementTree with methods defined to change
    the *_init.xml associated with a compiled OpenModelica model.

    """

    ## instance variables
    the_etree = None

    def __init__(self, xml_file):
        """
        Instantiates the ElementTree based on the xml_file

        """
        self.the_etree = ElementTree.parse(xml_file)

        self.xml_file = xml_file

    # end of __init__

    def write(self, new_xml_file=None):
        """
        Write the element tree instance to a xml file. Added by KMoon, 8/6/2012
        """
        if new_xml_file is None:
            self.the_etree.write(self.xml_file)
        else:
            self.the_etree.write(new_xml_file)
            self.xml_file = new_xml_file
        # end of write

    def change_experiment(self,
                          start_time='',
                          stop_time='',
                          step_size='',
                          tolerance='',
                          solver='',
                          output_format='',
                          variable_filter=''):
        """
        Change the default experiment values in a *_init.xml OM input file.

        """
        log = logging.getLogger()
        changed = False

        e_root = self.the_etree.getroot()

        e_experiment = e_root.find('DefaultExperiment')

        if start_time:
            if change_attrib(e_experiment, 'startTime', start_time):
                changed = True

        if stop_time:
            if change_attrib(e_experiment, 'stopTime', stop_time):
                changed = True

        if step_size:
            if change_attrib(e_experiment, 'stepSize', step_size):
                changed = True

        if tolerance:
            if change_attrib(e_experiment, 'tolerance', tolerance):
                changed = True

        if solver:

            # list of accepted solvers
            solvers = ['dassl',
                       'dassl2',
                       'euler',
                       'rungekutta',
                       'dopri5',
                       'inline-euler',
                       'inline-rungekutta']

            use_solver = 'dassl'

            if solver in solvers:
                use_solver = solver
            else:
                log.warning('Given solver ({0}) was not found, using \
                                dassl instead'.format(use_solver))

            if change_attrib(e_experiment, 'solver', use_solver):
                changed = True

        if output_format:
            if change_attrib(e_experiment, 'outputFormat', output_format):
                changed = True

        if variable_filter:
            if change_attrib(e_experiment, 'variableFilter', variable_filter):
                changed = True

        return changed

    # end of change_experiment

    def change_parameter(self, change_dict):
        """
        Find parameters in the *_init.xml OM input file that match the keys in
        change_dict and change them to the value in change_dict.

        """

        log = logging.getLogger()
        changed = False

        # Make a set copy so that any parameters not found can be reported
        change_set = set(change_dict)
        if not change_set:
            return changed

        # Make a dictionary to store any attempts to
        # change non parameter variables
        not_params = {}

        # All the variables, including parameters,
        # are in element 'ModelVariables'
        e_root = self.the_etree.getroot()
        e_variables = e_root.find('ModelVariables')
        for var in e_variables.getchildren():
            ## All the variable elements are just called <ScalarVariable> so we
            ## need to extract the name from the attributes
            var_name = var.attrib['name']
            if var_name in change_set:
                ## Check if it is actually a parameter before changing it
                if var.attrib['variability'] != 'parameter':
                    not_params[var_name] = var.attrib['variability']
                else:
                    ## Get the value element (Real, Integer or Boolean)
                    change_val = change_dict[var_name]
                    change_type = type(change_val)
                    var_elem = get_value_elem(var, change_type)
                    if var_elem is None:
                        raise ValueError('Did not find Real, Integer\
                                         or Boolean')
                    try:
                        current_val = change_type(var_elem.attrib['start'])
                    except KeyError:
                        current_val = None

                    if current_val and current_val == change_val:
                        log.info('parameter {0} is already equal to {1}'.format(var_name, current_val))
                    else:
                        # Print the change details and do it
                        log.debug('changing parameter {0} from {1} to {2}'.format(var_name, current_val,
                                                                                  str(change_dict[var_name])))

                        var_elem.attrib['start'] = str(change_dict[var_name])
                        changed = True

                # Remove a found variable from the input set copy
                change_set.remove(var_name)

        if change_set:
            log.warning('Could not find the following parameter variables:')
            for var in change_set:
                log.warning('{0}, tried to set to {1}'.format(var, change_dict[var]))

        if not_params:
            log.warning('The following variables are not parameters:')
            for var in not_params:
                log.warning('{0}, variability is {1}'.format(var, not_params[var]))

        return changed
    # end of change_parameter

    def get_statistics(self):
        """
        Returns statistics, such as number of state-, algebraic- and
        alias-variables.
        OpenModelica: *_init.xml is parsed
        JModelica.org: model_description.xml is parsed

        """
        statistics = {}
        log = logging.getLogger()
        log.debug("Retrieving model data from *.xml")

        e_root = self.the_etree.getroot()

        statistics['numberOfContinuousStates'] = e_root.attrib['numberOfContinuousStates']
        statistics['numberOfRealAlgebraicVariables'] = e_root.attrib['numberOfRealAlgebraicVariables']
        statistics['numberOfRealAlgebraicAliasVariables'] = e_root.attrib['numberOfRealAlgebraicAliasVariables']
        return statistics
        # end of get_statistics

    ## helper methods
# end of OpenModelicaElementTree


def change_attrib(elem, name, value):
    """
    Change an attribute and return True only if new value differs from old.
    Otherwise don't change and return False.

    :param elem:
    :param name:
    :param value:
    """

    log = logging.getLogger()

    value_type = type(value)
    if value_type(elem.attrib[name]) == value:
        log.warning('{0} in {1} already equal to {2}'.format(name, str(elem), value))
        return False
    else:
        log.info('Changed {0} in {1} from {2} to {3}'.format(name, str(elem), elem.attrib[name], value))
        elem.attrib[name] = str(value)
        return True
    # end of change_attrib


def get_value_elem(elem, var_type):
    """
    Search for and return either a Real, Integer, Boolean or String etree
    element based on the Python type that is intended to be assigned (float,
    int, bool or str).  Returns None if nothing is found that matches.
    :param elem:
    :param var_type:
    """

    if var_type is float or var_type is np.float64:
        val_elem = elem.find('Real')

    elif var_type is int:
        val_elem = elem.find('Integer')
        # Allow for assigning an int to a Real
        if val_elem is None:
            val_elem = elem.find('Real')

    elif var_type is bool:
        val_elem = elem.find('Boolean')

    elif var_type is str:
        val_elem = elem.find('String')

    else:
        raise ValueError('Unrecognized Python type = {0}'.format(var_type))

    return val_elem
# end of get_value_elem

