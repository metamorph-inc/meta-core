from __future__ import print_function
from __future__ import absolute_import
from openmdao.core.component import Component
from collections import Counter
import sys
import os
import six


class EnumMapper(Component):
    def __init__(self, config, param_name='input'):
        super(EnumMapper, self).__init__()

        self.config = config
        self.param_name = param_name
        # Check for consistency in all of the dictionaries
        first_key = None
        for key, val in six.iteritems(config):
            if first_key is None:
                # First run
                first_key = key
                first_map = val
                first_map_counter = Counter(val.keys())

            else:
                # Check for key consistency
                # TODO: Check for type consistency
                same = first_map_counter == Counter(val.keys())
                if same is False:
                    raise

        # Use the first config entry to set up the defaults
        self.add_param(param_name, val=first_key)
        for key, value in six.iteritems(first_map):
            self.add_output(key, val=value, pass_by_obj=True)

    def solve_nonlinear(self, params, unknowns, resids):
        input_value = params[self.param_name]
        try:
            lookup = self.config[input_value]
        except KeyError:
            print("ERROR: EnumMapper isn't configured to handle an input value of \"%s\"" % input_value)
            raise

        for key, value in six.iteritems(lookup):
            unknowns[key] = value


# USAGE EXAMPLE

# config = dict()
# config["Ia"] = { "AverageWindSpeed": 10.0, "ExtremeGust": 70.0 }
# config["Ib"] = { "AverageWindSpeed": 10.0, "ExtremeGust": 70.0 }
# config["IIa"] = { "AverageWindSpeed": 8.5, "ExtremeGust": 59.5 }
# config["IIb"] = { "AverageWindSpeed": 8.5, "ExtremeGust": 59.5 }
# config["IIIa"] = { "AverageWindSpeed": 7.5, "ExtremeGust": 52.5 }
# config["IIIb"] = { "AverageWindSpeed": 7.5, "ExtremeGust": 52.5 }
# config["IV"] = { "AverageWindSpeed": 6.0, "ExtremeGust": 42.0 }
# root.add('em', EnumMapper(config))
