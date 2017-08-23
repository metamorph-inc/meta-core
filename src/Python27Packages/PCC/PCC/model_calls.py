from numpy import array, asarray
import motorDesign

from openmdao.api import Group, InMemoryRecorder

UseCluster = False
UseParallel = False


class ListGen(Group):

    """
    DOEgenerator that returns rows in a list of inputs.

    Plugs into the DOEgenerator socket on a DOEdriver.
    """

#    inputs = List([], iotype='in', desc='List of parameters.')

    def __init__(self, *args, **kwargs):
        super(ListGen, self).__init__(*args, **kwargs)

    def __iter__(self):
        """Return an iterator over our sets of input values."""
        return self._next_row()

    def SetInputs(self, inputs):
        self.inputs = inputs

    def _next_row(self):
        nsims = len(self.inputs)
        for i, row in enumerate(self.inputs):
            print 'Running simulation', i + 1, 'of', nsims, 'with inputs', row
            yield row


def run_model(driver, input):
    driver.set_parameters(input)
    driver.run_iteration()
    output = driver.eval_objectives()
    return array(output)


def run_list(problem, driver, inputs):
    if driver.TestMotor:    # test the motor design instead of an external model
        outputs = []
        for i in inputs:
            outputs.append(motorDesign.motor(i, len(driver.outputNames)))
    elif driver.UseParametersCSV:
        outputs = []
        with open('parameters.csv', 'r') as f:  # read log file of inputs and outputs
            f.readline()  # skip past the line specifying the names of inputs and outputs
            for line in f:
                output = []
                for num in line.split(',')[-len(driver.outputNames):]:  # get list of outputs from line (in string form)
                    output.append(float(num))
                outputs.append(output)
            f.close()
    else:
        recorder = InMemoryRecorder()
        recorder.startup(driver.root)
        driver.recorders.append(recorder)
        problem.setup()
        driver.runlist = [zip(driver.inputNames, input) for input in inputs]

        if UseParallel:
            driver.sequential = False

        if UseCluster:
            print 'Using remote cluster.'
            driver.sequential = False
            # This is necessary more often than it should be.
            driver.ignore_egg_requirements = True
            # Force use of only cluster hosts by adding this requirement.
            driver.extra_resources = dict(allocator='PCCCluster')

        super(driver.__class__, driver).run(problem)

        outputs = []
        for c in recorder.iters:
            unknowns = c['unknowns']
            output = []
            missing = object()
            for name in driver.outputNames:
                path = name.split(".")
                if len(path) > 1 and path[0] in driver.subproblem_output_meta:
                    real_name = driver.subproblem_output_meta[path[0]][path[1]]
                    real_path = "{}.{}".format(path[0], real_name)
                else:
                    real_path = name
                unknown = unknowns.get(real_path, missing)
                if unknown is missing:
                    raise Exception('No outputs from simulator matching requested output \'{0}\'. Available outputs: {1}'.format(real_path, unknowns.keys()))
                if unknown is None:
                    # FIXME upgrade OpenMDAO, and the testbench should throw AnalysisException
                    raise Exception('No value from simulator matching requested output \'{0}\'. Perhaps the testbench failed')
                if not isinstance(unknown, float):
                    # FIXME should really be a float. TODO fix post_processing_class.py update_metrics_in_report_json in all models everywhere...
                    # warnings.warn('Unknown \'{0}\' produced from a TestBench is not a float'.format(name))
                    unknown = float(unknown)
                output.append(unknown)
            outputs.append(output)
        driver.recorders._recorders.remove(recorder)

    if not driver.UseParametersCSV:
        with open('parameters.csv', 'w') as f:  # write log file of inputs and outputs
            f.write(','.join(driver.inputNames)+','+','.join(driver.outputNames)+'\n')
            for x, i in enumerate(inputs):
                f.write(','.join(str(y) for y in inputs[x]))
                if x < len(outputs):
                    f.write(','+','.join(str(y) for y in outputs[x]))
                f.write('\n')
            f.close()

    if len(outputs) != len(inputs):
        raise Exception('Simulator returned only {0} results of a requested {1}. See parameters.csv for details.'.format(len(outputs), len(inputs)))

    return asarray(outputs)
