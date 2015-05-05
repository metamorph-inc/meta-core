import os
import numpy as np
from scipy.io import loadmat
from scipy.io.matlab.mio4 import VarReader4

VARIABLE_INDICES = set()
VARIABLES = {}


def VARIABLE_read_sub_array(self, hdr, copy=False):
    """
    Modification of the scipy function read_sub_array,
    that is called from loadmat.

    The modification only collects the variables with indices
    given in VARIABLE_INDICES.

    (The read_sub_array is nested in scipy and inorder to
     avoid argument modifications of all involved functions,
     VARIABLES and VARIABLE_INDICES are defined globally)
    """
    dt = hdr.dtype
    dims = hdr.dims

    vars = dims[0]
    time_points = dims[1] * 1
    num_bytes = dt.itemsize * vars

    index_ar = np.zeros((vars * 1, 1))
    cnt = 0
    for key_index in VARIABLE_INDICES:
        cnt += 1
        index_ar[key_index] = cnt
    index_array_bool = index_ar > 0
    arr = np.zeros((cnt, time_points))

    for delta_t in range(0, time_points):
        arr[:, delta_t] = np.ndarray(shape=(vars, np.array(1)),
                                     dtype=dt,
                                     buffer=self.mat_stream.read(int(num_bytes)),
                                     order='F')[index_array_bool]

    sorted_variable_indices = list(VARIABLE_INDICES)
    sorted_variable_indices.sort()
    cnt = 0
    for key_index in sorted_variable_indices:
        VARIABLES[key_index] = arr[cnt, :]
        cnt += 1

        # end of VARIABLE_read_sub_array


class MatFile2Dict():
    """
    Reads the mat-file and puts the data into the defined fields.

    A new instance must be created for each load.

    Example:
        converter = MatFile2Dict("res.mat", filter = ["Velocity", "Speed"])
        result = converter.get_results()
    """
    data_file = ""
    variable_filter = []
    constants = {}
    variables = {}
    c_info = {}
    v_info = {}

    gather_desc = False
    has_converted = False

    def __init__(self, data_file, variable_filter=None, gather_desc=False):
        """
        Creates a new instance of this class. Make sure to set gather_desc to True if you
        would like to load the descriptions. (Otherwise, if you invoke get_descriptions,
        it will read the mat-file again)

        :param data_file:
        :param variable_filter:
        :param gather_desc:
        :return:
        """
        self.data_file = data_file
        if variable_filter:
            self.variable_filter = variable_filter
        else:
            self.variable_filter = []
        self.gather_desc = gather_desc

    def get_results(self):
        """ 
        Returns with the variables and constants in one merged dictionary of the form:
        (It expands the constants into time_series.)

        {{name1: [values1]}, ..., {nameN: [valuesN]}}
        """

        if not self.has_converted:
            self._convert()

        # expand constants values into time series
        array_c = {}
        for co in self.constants.iteritems():
            #print "\nConstant : [name : {0}, value : {1}]".format(co[0], co[1])
            array_c[co[0]] = np.ones(len(self.variables['time'])) * [co[1]]

        result = {}
        result.update(self.variables)
        result.update(array_c)

        return result

    def get_descriptions(self):
        """
        Returns with a dictionary containing descriptions for both constants and variables.

        {{name1: {type:"", unit:"", desc:""}},...,{nameN: {type:"", unit:"", desc:""}}}

        """
        if not self.gather_desc:
            print 'You ought to instantiate MatFile2Dict with gather_desc if you want such.'
            self.gather_desc = True
            self._convert()

        if not self.has_converted:
            self._convert()

        desc = {}
        for c in self.c_info:
            cc = {'type': 'constant', 'unit': self.c_info[c][2], 'desc': self.c_info[c][3]}
            desc.update({c: cc})
        for v in self.v_info:
            vv = {'type': 'variable', 'unit': self.v_info[v][2], 'desc': self.v_info[v][3]}
            desc.update({v: vv})

        return desc

    def get_variables(self):
        """ 
        Returns with the variable-dictionary in the form:
        
        {{name1: [values1]}, ..., {nameN: [valuesN]}}
        """
        if not self.has_converted:
            self._convert()

        return self.variables

    def get_constants(self):
        """ 
        Returns with the constant-dictionary in the form:
        
        {{name1: value1}, ..., nameN: valuesN}}
        """
        if not self.has_converted:
            self._convert()

        return self.constants

    def _convert(self):
        """
        Converts the mat_file into dictionaries
        """
        if not os.path.isfile(self.data_file):
            raise Exception('Mat_file : {0} Does not exist!'.format(self.data_file))

        c_info, v_info = self._load_info_from_mat()

        # save info if to gather descriptions
        if self.gather_desc:
            self.c_info = c_info
            self.v_info = v_info
        self.constants = self._load_parameters_from_mat(c_info)
        self.variables = self._load_variables_from_mat(v_info)

        global VARIABLE_INDICES, VARIABLES
        VARIABLE_INDICES.clear()
        VARIABLES.clear()
        self.has_converted = True

    def _load_info_from_mat(self):

        """
        Loads data such as name, sign, position of timeseries
        from the mat_file, the time series are not gathered at this stage.

        The information is returned in;

        parameter_info - {"name": (data_pos, data_sign, unit, description),..}
        variable_info. -                        --  || --

        """
        global root_node, desc

        VARIABLE_INDICES.clear()

        # return dictionaries
        parameter_info = {}
        variable_info = {}

        to_collect = ['name', 'dataInfo']
        if self.gather_desc:
            to_collect.append('description')

        data = loadmat(self.data_file, matlab_compatible=True, variable_names=to_collect)

        # name of parameter or variable
        names = data['name'].transpose()

        # if 1 -> parameter, if 2 -> variable
        data_type = data['dataInfo'][0]

        # position in respective data-set
        data_pos = np.abs(data['dataInfo'][1]) - 1

        # sign of data
        data_sign = np.sign(data['dataInfo'][1])

        # description of parameter of variable
        if self.gather_desc:
            desc = data['description'].transpose()

            # build up search tree if filter is given
            #if filter :
            #root_node = TreeNode("root_node")
            #root_node.populate_tree(filter)

        for i in xrange(names.shape[0]):
            # Most time is spent here...
            name = str(u''.join(e for e in names[i])).rstrip().replace(' ', '')

            success = True
            if self.variable_filter:
                success = name in self.variable_filter

            if name == 'Time' or name == 'time':
                name = 'time'
            elif not success:
                continue

            # Use dymola name convention for der-operator,
            # e.g. name = der(brake.phi) then becomes brake.der(phi)
            while name.startswith('der('):
                # make sure the variable is not at root level
                j = name.rfind('.')
                if j != -1:
                    name = name[4:j + 1] + 'der(' + name[j + 1:]
                else:
                    break
            description = ""
            unit = ""
            if self.gather_desc:
                description = ''.join([str(e) for e in desc[i]]).rstrip()
                unit_ind1 = description.rfind('[')
                unit_ind2 = description.rfind(']')
                if unit_ind1 > 0 and unit_ind2 > 0:
                    unit = description[unit_ind1 + 1:unit_ind2]
                    description = description[:unit_ind1]

            if data_type[i] == 1:
                parameter_info[name] = (data_pos[i], data_sign[i], unit, description)
            else:
                variable_info[name] = (data_pos[i], data_sign[i], unit, description)
                VARIABLE_INDICES.add(data_pos[i])

        return parameter_info, variable_info

    def _load_variables_from_mat(self, var_info):
        """
        Loads the time_series data from the mat file,
        uses variable indices from global the global set.

        """

        # Saves the default and overrides the inner standard loadmat function.
        default_read_sub_array = VarReader4.read_sub_array
        VarReader4.read_sub_array = VARIABLE_read_sub_array

        loadmat(self.data_file, matlab_compatible=True, variable_names=['data_2'])
        variables = {}
        for var in var_info.iteritems():
            index = var[1][0]
            sign = var[1][1]
            vv = {var[0]: VARIABLES[index] * sign}
            variables.update(vv)
            #variables[var[0]] = VARIABLES[index]*sign

            # Sets the loadmat function back to default.
        VarReader4.read_sub_array = default_read_sub_array

        return variables

    def _load_parameters_from_mat(self, parameter_info):
        """
        Loads the values of the parameters.
        """
        parameters = {}

        data = loadmat(self.data_file, matlab_compatible=True, variable_names=['data_1'])['data_1']

        for parameter in parameter_info:
            index = parameter_info[parameter][0]
            sign = parameter_info[parameter][1]
            value_0 = data[index][0] * sign
            pp = {parameter: value_0}
            parameters.update(pp)

        return parameters