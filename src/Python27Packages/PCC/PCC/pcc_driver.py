from run_mdao import PredeterminedRunsDriver

import os             # more Python environment and OS-type stuff
import math           # for sqrt
import numpy as np    # numerics and n-dimensional array, needed by scipy
from scipy import stats  # statistical distributions
from collections import namedtuple
import json
import logging

from UP_MCS import UP_MCS
from UP_TS import UP_TS
from UP_MPP import UP_MPP
from UP_FFNI import UP_FFNI
from UP_UDR import UP_UDR
from UP_PCE import UP_PCE
from SA_SOBOL import SA_SOBOL
# from SA_MORRIS import SA_MORRIS
from SA_FAST import SA_FAST
from SA_EFAST import SA_EFAST
from UP_DPCE import UP_DPCE

import jsonout


class PCCdriver(PredeterminedRunsDriver):

    def __init__(self, Configurations, **kwargs):
        super(PCCdriver, self).__init__(**kwargs)
        self.use_restart = False

        self._raw_data = {}     # Keep original outputs of the component model
        self._json_tree = {}    # Store json data into here
        self._model_dir = ''    # Model file location
        self.autoClose = True        # Optional: regression test doesn't need extra windows popping up
        self.waitForClose = True     # Optional: If this is not true, you may be very confused because Python will return as soon as it calls MATLAB

        # Optional: should results be plotted?  Defaults to False
        self.showplot = False

        # In code below, we read in the name of a statistical distribution and
        # some parameters to characterize that distribution.  This is the
        # number of parameters for each distribution.
        self.paramsForDist = {'NORM': 2, 'LNORM': 2, 'BETA': 4, 'UNIF': 2}

        # DEFAULT IS 1280 for all these
        self.numbins = 1280             # Number of bins to use for complexity estimation in non-sampled methods
        self.nMCS = 1280                # Number of Monte Carlo Simulation points
        self.nSOBOL = 1280              # Number of Monte Carlo Simulation points for the Sobol Method
        self.nEFAST = 1280              # Number of sample points for the EFAST Method

        # Set p and r for Morris method. It is recommended to use p=4 & r=10
        # p MUST BE EVEN
        self.p = 4
        self.r = 10

        # Regression Flag 0=No Regression, 1=Yes Regression
        # This is regression in the statistical sense, not the
        # nightly build and test sense.
        self.lrflag = 1

        # Number of Latin Hypercube samples for the Kriging Metamodel
        self.n_meta = 100

        # Use the Kriging Metamodel
        # Note that method 0 does nothing but the Kriging model.  This variable
        # does not apply there.  When set, the other methods predict the model
        # outputs rather than actually calling the model.
        self.krig = 0

        # Delta x value for Finite Difference Approximation of derivatives
        # (Central Difference Approx Used)
        self.MPPdelta = 0.1
        self.TSdelta = 0.1
        # self.delta = 0.0001

        # Number of Gaussian Quadrature Points for FFNI, UDR, and PCE
        # DEFAULT IS 5
        self.numOfNodes = 5

        # Order of Polynomial for PCE Surrogate Model
        self.order = 4

        # I think this tells the SA_ methods to simplify their outputs, i.e. toss
        # data that may be superfluous
        self.simple = 1

        # Other things that will be filled after reading a json file
        self.outputNames = []
        self.inputNames = []
        self.limstate = []
        self.inputs = []

        # Defined in the execution routine
        self.nodes = 0
        self.stvars = []

#    def add_model_parameters(self, model_name):
#        if self.inputNames == []:
#            sys.exit('Load PCC json file first.')

#        for inp in self.inputNames:
#            target_str = model_name + '.' + str(inp)
#            self.add_parameter(target_str, low = -1e99, high = 1e99)


#    def add_model_outputs(self, model_name):
#        if self.outputNames == []:
#            sys.exit('Load PCC json file first.')


#        for out in self.outputNames:
#            target_str = model_name + '.' + str(out)
#            self.add_objective(target_str)

#        # Add time data at the end
#        self.add_objective(model_name+'.time')

        self._json_tree = {'Configurations': Configurations}

        try:
            outputdict = self._json_tree['Configurations']['Configuration']['PCCInputArguments']['PCCMetrics']
        except Exception, err:
            logging.exception('Could not find PCCMetrics in Configurations.')
            raise err

        if len(outputdict) == 0:
            logging.exception('No outputs found in PCCMetrics.')
            raise ValueError('No outputs found in PCCMetrics.')

        outputNames = []     # list of output names
        limstate = []      # list of output requirements
        for out in outputdict:
            outputNames.append(str(out['Name']))
            limstate.append([out['Limits']['Min'], out['Limits']['Max']])
        self.outputNames = outputNames
        self.limstate = limstate
        # remember the number of outputs
        # otpt = len(outputNames)

        #  ********************* PROCESS INPUT PARAMETERS HERE ******************
        # ************************************************************************

        try:
            inputdict = self._json_tree['Configurations']['Configuration']\
                             ['PCCInputArguments']['StochasticInputs']\
                             ['InputDistributions']
        except Exception, err:
            logging.exception('Could not find InputDistributions in Configurations')
            raise err

        if len(inputdict) == 0:
            logging.exception('No inputs found in PCCMetrics.')
            raise ValueError('No inputs found in PCCMetrics.')

        inputs = []
        for inp in inputdict:
            input = Input(str(inp['Name']))
            inputs.append(input)
            distName = str(inp['Distribution'])
            params = []
            try:
                params.append(inp['Param1'])
                params.append(inp['Param2'])
            except Exception, err:
                logging.exception('Could not read Param1 and Param2 for input %s:', input)
                raise err
            # "designVariable" comes from run_mdao
            self.inputNames.append("designVariable." + str(inp['Name']))

            # Now that we have the distribution's parameters, get that
            # distribution's mean and standard deviations.
            if distName == 'NORM':
                I_mu, I_sigma = stats.norm.stats(params[0], params[1])
            elif distName == 'LNORM':
                I_mu, I_sigma = stats.lognorm.stats(params[0], params[1])
            elif distName == 'BETA':
                try:
                    params.append(inp['Param3'])
                    params.append(inp['Param4'])
                except Exception, err:
                    logging.exception('Could not read Param3 and Param4 for input %s:', input)
                    raise err
                I_mu, I_sigma = stats.beta.stats(params[0], params[1])
                # the following scaling code was in PCC_Computation.m
                I_mu = I_mu * (params[3] - params[2]) + params[2]
                I_sigma = I_sigma * ((params[3] - params[2])**2)
            elif distName == 'UNIF':
                raw_I_mu, raw_I_sigma = \
                    stats.uniform.stats(params[0], params[1] - params[0])  # .astype(float)
                # these show up as numpy.ndarray, which is very confusing
                I_mu = float(raw_I_mu)
                I_sigma = float(raw_I_sigma)
            else:
                print 'distribution', distName, ' is not supported'
                logging.error('distribution %s is not supported', distName)
                raise IndexError()

            if math.isnan(I_sigma):
                logging.error("Sigma must be greater than zero for input '%s'.", str(inp['Name']))
                raise ValueError("Sigma must be greater than zero for input '{0}'.".format(str(inp['Name'])))

            # we really want the standard deviation, not the variance
            I_sigma = math.sqrt(I_sigma)

            if isinstance(I_mu, np.ndarray):
                I_mu = I_mu.item()
            if isinstance(I_sigma, np.ndarray):
                I_sigma = I_sigma.item()

            input.set_distribName(distName)
            for par in params:
                input.set_param(par)
            input.set_I_mu(I_mu)
            input.set_I_sigma(I_sigma)

        self.inputs = inputs
        # remember the number of outputs
        # inpt = len(inputs)

        self.numbins = self.ReadSample('numbins', 1, self.numbins)
        self.nMCS = self.ReadSample('nMCS', 2, self.nMCS)
        self.nSOBOL = self.ReadSample('nSOBOL', 2+len(inputs), self.nSOBOL)
        self.nEFAST = self.ReadSample('nEFAST', 65*len(inputs), self.nEFAST)
        self.lrflag = self.ReadFlag('lrflag', self.lrflag)
        self.n_meta = self.ReadSample('n_meta', 1, self.n_meta)
        self.krig = self.ReadFlag('krig', self.krig)
        self.MPPdelta = self.ReadFloat('MPP delta', 0.000001, 0.1, self.MPPdelta)
        self.TSdelta = self.ReadFloat('TS delta', 0.000001, 0.1, self.TSdelta)
        self.numOfNodes = self.ReadnumOfNodes(self.numOfNodes)
        self.order = self.ReadOrder(self.order)
        self.simple = self.ReadFlag('simple', self.simple)
        self.Wrapper = self.ReadString('Wrapper', "")
        self.TestMotor = self.ReadBool('TestMotor', False)
        self.UseParametersCSV = self.ReadBool('UseParametersCSV', False)

    def _build_runlist(self):
        # self.runlist is precomputed elsewhere
        return self.runlist

    def save_output_to_json(self):
        # TODO: Assumption there is exactly one _model_config_file
        '''
        import glob
        model_config_file = glob.glob('*_model_config.json')[0]
        print 'Reading model config file:', model_config_file
        if (os.path.exists(model_config_file)):
            with open(model_config_file) as f:
                model_config = json.load(f)
        model_file_dir = os.path.dirname(model_config['model_file_name'])
        summary_report_path = os.path.join(model_file_dir, 'testbench_manifest.json')
        '''
        # print 'Writing PCC results to:', summary_report_path
        jsonout.jsonout(os.path.join(os.getcwd(), 'testbench_manifest.json'), self.results, self.method_name,
                        self._json_tree)
        # Edited by Patrik Meijer @ ISIS - Put summary.testresults.json one folder higher up.

#        new_path = os.path.abspath(os.path.join(model_file_dir, '..', 'summary.testresults.json'))
#        shutil.copy(summary_report_path, new_path)

    def run(self, driver):
##        ## ******************** SET UP THE MODEL HERE ****************************
##        #*************************************************************************
##        #
##        # Compile the model here.  The UP_* and SA_* methods call it a lot.
##        # One of the side effects of compiling the model is that it produces a file
##        # that contains the default input values in <modelName>_init.txt.  The
##        # code that sets up parameters before each call to the model edits this file
##        # (well, really it makes a copy of the file and changes the parameters of
##        # interest on the way).
##
##        omcompile.compile(library, modelname, modelloc)
##
##        # tidy up after the compilation
##        omcleanup.cleanup(modelname)
##
##        # get the omparameter object that we'll use to set the parameters
##        omParams = omparameter.omParameterList(modelname)
##
##        #FOR OPENMODELICA 1.6 AND HIGHER ONLY--OTHERWISE COMMENT OUT THIS
##        #STATEMENT.  Set the Modelica Output format. The script assumes
##        #the output is written to a .plt file
##        omParams.setExperimentParameter('outputFormat','plt');
##
##        # dump these to the file.  Normally, we'd do this immediately
##        # before running the model, but we won't run the model until
##        # we get into the code for each UP_ or SA_ method.
##        omParams.dumpParamsToFile()

        inpt = len(self.inputs)

        # (From loadargs.loadargs)
        # stvars is tricky.  stvars is an array of structures.  Each
        # structure has two nodes, dist and param.  dist is a string.
        # param is a variable length array of numbers.

        stvars = []

        for i in range(inpt):
            stvar = namedtuple("stvar", "name dist param")
            stvar.name = self.inputs[i].get_inputName()
            stvar.dist = self.inputs[i].get_distribName()
            stvar.param = self.inputs[i].get_params()
            stvars.append(stvar)
        self.stvars = stvars

        # Convert the scalar number of gaussian quadrature nodes defined
        # into a vector of the same length as our input vector. The weird
        # '*' notation says we're going to create a new list (possibly the
        # type of thing created is set by the type of the thing that the
        # '*' is operating on) and the new list will have inpt elements.
        # The value of each element is numOfNodes.

        self.nodes = inpt*[self.numOfNodes]

##        # dump all the arguments that will be needed by the individual UP_
##        # and SA_ procedures.  Eventually, these will be read back in by
##        # each procedure after it's started from the command line.
##        dumpargs.dump_args(modelname, self.delta, inpt,
##                           inputs, self.krig, limstate, self.lrflag, self.n_meta, self.nEFAST,
##                           self.nSOBOL, self.nMCS, nodes, self.order, otpt,
##                           outputNames, self.p, self.showplot, self.r, self.simple)

        # loop through whatever methods were chosen above
        # skip method 6 until we get the right MATLAB toolkit (or port it to Python!)
        for method in self._json_tree['Configurations']['Configuration']\
                                     ['PCCInputArguments']['Methods']:
            print('------------------  Starting method {} -------------------'.format(method))
            # Here is where the real work is done.  The UP methods Propagate
            # Uncertainty through the component to calculate the probability
            # of the component working.  The SA methods estimate the
            # sensitivity of the selected outputs to inputs
##            if method == 0:     # KRINGING metamodel saved as a .mat file for use with any MATLAB method.
##                runMethod.invokeMethod('KRIGING', modelname, autoClose, waitForClose, useCompiledMethod);

            if method >= 7 and len(self.inputs) < 2:   # if we are using a sensitivity analysis (SA) method, must have at least 2 inputs
                logging.exception('Must use at least two inputs for method %d (tried to use %d).', method, len(self.inputs))
                raise ValueError('Must use at least two inputs for Sensitivity Analysis.')

            if method == 1:   # MCS: Monte Carlo Simulation
                self.method_name = 'MCS'
                self.results = UP_MCS(self)
            elif method == 2:   # TS: Taylor Series Approximation
                self.method_name = 'TS'
                self.results = UP_TS(self)
            elif method == 3:   # MPP: Most Probable Point Method
                self.method_name = 'MPP'
                self.results = UP_MPP(self)
            elif method == 4:   # FFNI: Full Factorial Numerical Integration
                self.method_name = 'FFNI'
                self.results = UP_FFNI(self)
            elif method == 5:   # UDR: Univariate Dimension Reduction Method
                self.method_name = 'UDR'
                self.results = UP_UDR(self)
            elif method == 6:   # PCE: Polynomial Chaos Expansion
                self.method_name = 'PCE'
                self.results = UP_PCE(self)
            elif method == 7:   # Sobols Method (SOBOL)
                self.method_name = 'SOBOL'
                self.results = SA_SOBOL(self)
            # elif method == 8:   # Morris Screening Method (MORRIS)
                # self.method_name = 'MORRIS'
                # self.results = SA_MORRIS(self)
            elif method == 9:   # FAST Method (FAST)
                self.method_name = 'FAST'
                self.results = SA_FAST(self)
            elif method == 10:   # Extended FAST Method (EFAST)
                self.method_name = 'EFAST'
                self.results = SA_EFAST(self)
            elif method == 11:   # DPCE: Dakota implementation of Polynomial Chaos Expansion
                self.method_name = 'DPCE'
                self.results = UP_DPCE(self)
            else:
                print 'attempting to execute illegal method', method
                logging.error('attempting to execute illegal method %d', method)
                raise Exception

            # Print the results
            # import pdb; pdb.set_trace()

            np.set_printoptions(suppress=True)  # prettier printing. Supresses scientific notation
            if 'MPPUpperBound' in self.results.keys():
                print 'Most Probable Points of Failure (upper bound):\n', self.results['MPPUpperBound']
                print 'Most Probable Points of Failure (lower bound):\n', self.results['MPPLowerBound']
            if 'CorrelationMatrix' in self.results.keys():
                print 'Correlation:\n', self.results['CorrelationMatrix']
            if 'Moments' in self.results.keys():
                print 'Moments:\n',
                print "  Mean =", self.results['Moments']['Mean']
                print "  Variance =", self.results['Moments']['Variance']
                print "  Skewness =", self.results['Moments']['Skewness']
                print "  Kurtosis =", self.results['Moments']['Kurtosis']
            if 'dtype' in self.results.keys():
                print 'Pearson Distribution Type =', self.results['dtype']
            if 'PCC' in self.results.keys():
                print 'PCC:', self.results['PCC']
            if 'Distribution' in self.results.keys():
                if 'Complexity' in self.results['Distribution'].keys():
                    print 'Complexity estimates:', self.results['Distribution']['Complexity']
            if 'FirstOrderSensitivity' in self.results.keys():
                print 'First order sensitivity:\n', self.results['FirstOrderSensitivity']
            if 'TotalEffectSensitivity' in self.results.keys():
                print 'Total effect sensitivity:\n', self.results['TotalEffectSensitivity']
            if 'SRC' in self.results.keys():
                print 'Standardized regression coefficients:\n', self.results['SRC']
                print 'R^2:\n', self.results['R^2']

            self.save_output_to_json()
            # should be called here rather than in PCCDriver_component.py to allow results from more than one method to be saved
            # Now it is called here :)

        # That's it!
        print('Done!')

    def ReadString(self, name, default):
        settings = self._json_tree['Configurations']['Configuration']
        if name in settings:
            if type(settings[name]) is unicode:
                return settings[name]
            else:
                logging.error('%s must be of type unicode. (attempted to set %s to %s)', name, name, settings[name])
        return default

    def ReadBool(self, name, default):
        settings = self._json_tree['Configurations']['Configuration']
        if name in settings:
            if type(settings[name]) is bool:
                return settings[name]
            else:
                logging.error('%s must be boolean. (attempted to set %s to %s)', name, name, settings[name])
        return default

    def ReadFlag(self, name, default):
        settings = self._json_tree['Configurations']['Configuration']
        if name in settings:
            var = settings[name]
            if type(var) is int and (var == 0 or var == 1):
                return settings[name]
            else:
                logging.error('%s must be 0 or 1. (attempted to set %s to %s)', name, name, settings[name])
        return default

    def ReadSample(self, name, smin, default):
        settings = self._json_tree['Configurations']['Configuration']
        if name in settings:
            var = settings[name]
            if type(var) is int:
                if var < smin:
                    logging.error('%s must be >= %d. (attempted to set %s to %d)', name, smin, name, var)
                    return default
                elif var < 100:
                    logging.warning('Sample sizes less than 100 are likely to cause innaccurate results. (set %s to %d)', name, var)
                return var
            else:
                logging.error('%s must be an integer. (attempted to set %s to %s)', name, name, settings[name])
        return default

    def ReadFloat(self, name, fmin, fmax, default):
        settings = self._json_tree['Configurations']['Configuration']
        if name in settings:
            var = settings[name]
            if type(var) is float or type(var) is int:
                if var < fmin or var > fmax:
                    logging.error('%s must be between %f and %f. (attempted to set %s to %f)', name, fmin, fmax, name, var)
                    return default
                return var
            else:
                logging.error('%s must be a number. (attempted to set %s to %s)', name, name, settings[name])
        return default

    def ReadnumOfNodes(self, default):
        settings = self._json_tree['Configurations']['Configuration']
        if 'numOfNodes' in settings:
            var = settings['numOfNodes']
            if type(var) is int and var >= 3 and var % 2 == 1:
                return var
            else:
                logging.error('numOfNodes must be odd and >= 3. (attempted to set numOfNodes to %s)', var)
        return default

    def ReadOrder(self, default):
        settings = self._json_tree['Configurations']['Configuration']
        if 'order' in settings:
            var = settings['order']
            if type(var) is int and var >= 2 and var <= 10:
                return var
            else:
                logging.error('order must be >= 2 and <= 10. (attempted to set order to %s)', var)
        return default


class Input:

    """Hold data about component inputs."""

    def __init__(self, inputName):
        self.inputName = inputName
        self.params = []

    def get_inputName(self):
        return self.inputName

    def set_drivenBy(self, drivenBy):
        self.drivenBy = drivenBy

    def get_drivenBy(self):
        return self.drivenBy

    def set_distribName(self, distribName):
        self.distribName = distribName

    def get_distribName(self):
        return self.distribName

    def set_param(self, param):
        self.params.append(param)

    def get_param(self, paramNum):
        return self.params[paramNum]

    def get_params(self):
        return self.params

    # associated with each input is also a mean and standard deviation.
    def set_I_mu(self, mean):
        self.I_mu = mean

    def get_I_mu(self):
        return self.I_mu

    def set_I_sigma(self, stddev):
        self.I_sigma = stddev

    def get_I_sigma(self):
        return self.I_sigma


# #perform indicated operation on given range and ordered dictionary of values, returning the result of the operation
# #currently valid operations: last, min, max, avg. Default is 'last'.
# def PerformOp(values, op, minrange, maxrange):

#    if op == None or op == 'last':  #just take the last item before maxrange. This is the default operation.
#        for key, val in values.iteritems():
#            if maxrange != None and key > maxrange:
#                break
#            out = val
#    else:
#        it = values.iteritems()
#        if minrange == None:     #find the iterator to the first element >= to minrange. Otherwise it's the first item.
#            firstkey, out = it.next()
#        else:
#            for firstkey, out in it:
#                if firstkey >= minrange:
#                    break

#        if op == 'max':
#            for key, val in it:
#                if maxrange != None and key > maxrange:
#                    break
#                if val > out:
#                    out = val

#        elif op == 'min':
#            for key, val in it:
#                if maxrange != None and key > maxrange:
#                    break
#                if val < out:
#                    out = val

#        elif op == 'avg':
#            lastkey = firstkey
#            for key, val in it:
#                if maxrange != None and key > maxrange:
#                    break
#                out += val*(key-lastkey)  #value of this contribution is weighted by differences in time index
#                lastkey = key
#            out /= lastkey-firstkey+1  #take average

#        else:
#            print "Unknown operator: ", op

#    print "ending time:", key
#    return out
