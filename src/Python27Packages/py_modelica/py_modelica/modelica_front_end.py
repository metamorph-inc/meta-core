"""
This should be the file that the user should call if he/she wants to use an interface
to different simulation tools.

Still under construction.

"""

import json
from py_modelica.modelica_simulation_tools.om_class import OpenModelica
from py_modelica.modelica_simulation_tools.dymola_class import Dymola
from py_modelica.modelica_simulation_tools.dymola_demo_class import DymolaDemo
from py_modelica.mat_file_functions.mat_file_to_dict import MatFile2Dict


class ModelicaFrontEnd():
    """
    Class where the simulation tool is specified by tool_config,
    which either is a .json-file or dictionary.

    All methods, e.g. compile_model, run_simulation, are invoked
    independently of the simulation tool used.
    """

    tool = None
    model_config = {}
    tool_config = {}
    tool_name = ''
    om_home = ''
    dymola_home = ''

    def __init__(self, model_config, tool_config):
        """
        Extract configurations, either directly as given dictionaries
        or deserialize the .json file.

        """

        if isinstance(model_config, str):
            with open(model_config) as file_model:
                self.model_config = json.load(file_model)
        elif isinstance(model_config, dict):
            self.model_config = model_config
        else:
            msg = 'model_config must be a .json file name or a dictionary.'
            raise ValueError(msg)

        if isinstance(tool_config, str):
            with open(tool_config) as file_model:
                self.tool_config = json.load(file_model)
        elif isinstance(tool_config, dict):
            self.tool_config = tool_config
        else:
            msg = 'model_config must be a .json file name or a dictionary.'
            raise ValueError(msg)

        if self.tool_config['tool_name'] == "Dymola":
            dymola_home = self.tool_config['dymola_home']
            if dymola_home:
                self.tool = Dymola(self.model_config, dymola_home)
            else:
                self.tool = Dymola(self.model_config)
        elif self.tool_config['tool_name'] == "DymolaDemo":
            dymola_home = self.tool_config['dymola_home']
            if dymola_home:
                self.tool = DymolaDemo(self.model_config, dymola_home)
            else:
                self.tool = DymolaDemo(self.model_config)
        elif self.tool_config['tool_name'] == "OpenModelica":
            om_home = self.tool_config['om_home']
            self.tool = OpenModelica(self.model_config, om_home)
    # end of __init__

    def compile_model(self):
        """
        Compiles the model using the defined tool.
        """

        return self.tool.compile_model()
    # end of compile_model

    def change_experiment(self,
                          start_time='0',
                          stop_time='1',
                          increment='',
                          n_interval='500',
                          tolerance='1e-5',
                          max_fixed_step='',
                          solver='dassl',
                          output_format='',
                          variable_filter=''):
        """
        Changes the simulation setup.
        See each simulation tool for more info...
        """

        return self.tool.change_experiment(start_time, stop_time, increment, n_interval, tolerance,
                                           max_fixed_step, solver, output_format, variable_filter)
    # end of change_experiment

    def change_parameter(self, change_dict):
        """
        Changes the parameters of the simulation.
        change_dict is of the form
        {p_name1 : value1,...,p_nameN : valueN}

        See each tool for more info...
        """

        return self.tool.change_parameter(change_dict)
    # end of change_parameter

    def run_simulation(self, variable_filter=None, gather_desc=False):
        """
        Simulates the model and returns with the two dictionaries;
        first one with parameters(constants) and second one with variables .

        Filter should be a list of strings with names of variables/parameters
        to include. If None all are returned.

        """
        if self.tool.simulate_model():
            result_file = self.tool.result_mat
            mat_converter = MatFile2Dict(result_file, variable_filter, gather_desc)
            result = mat_converter.get_results()

            desc = None
            if gather_desc:
                desc = mat_converter.get_descriptions()

            return result, desc
        else:
            return None, None
    # end of run_simulation






