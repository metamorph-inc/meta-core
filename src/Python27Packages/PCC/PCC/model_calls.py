from numpy import *
import sys
import motorDesign

from openmdao.lib.drivers.api import DOEdriver
from openmdao.lib.casehandlers.api import ListCaseRecorder

from openmdao.main.resource import ResourceAllocationManager as RAM
from openmdao.main.resource import ClusterAllocator, ClusterHost
from openmdao.main.mp_distributing import _SSH

from openmdao.main.api import Container
from openmdao.main.interfaces import implements, IDOEgenerator

UseCluster = False
UseParallel = False

class ListGen(Container):
    """
    DOEgenerator that returns rows in a list of inputs.
    Plugs into the DOEgenerator socket on a DOEdriver.
    """
    
    implements(IDOEgenerator)
    
#    inputs = List([], iotype='in', desc='List of parameters.')

    def __init__(self, *args, **kwargs):
        super(ListGen, self).__init__(*args, **kwargs)

    def __iter__(self):
        """ Return an iterator over our sets of input values. """
        return self._next_row()

    def SetInputs(self, inputs):
        self.inputs = inputs

    def _next_row(self):
        nsims = len(self.inputs)
        for i, row in enumerate(self.inputs):
            print 'Running simulation',i+1,'of',nsims,'with inputs',row
            yield row

def run_model(driver,input):
    driver.set_parameters(input)
    driver.run_iteration()
    output = driver.eval_objectives()
    return array(output)

def run_list(driver,inputs):
    if driver.TestMotor:    # test the motor design instead of an external model
        outputs = []
        for i in inputs:
            outputs.append(motorDesign.motor(i, len(driver.outputNames)))
    elif driver.UseParametersCSV:
        outputs = []
        with open('parameters.csv', 'r') as f:  # read log file of inputs and outputs
            f.readline()    #skip past the line specifying the names of inputs and outputs
            for line in f:
                output = []
                for num in line.split(',')[-len(driver.outputNames):]:  # get list of outputs from line (in string form)
                    output.append(float(num))
                outputs.append(output)
            f.close()
    else:
        driver.DOEgenerator.SetInputs(inputs)
        driver.recorders = [ListCaseRecorder(),]
        
        if UseParallel:
            driver.sequential = False

        if UseCluster:
            print 'Using remote cluster.'
            driver.sequential = False
            # This is necessary more often than it should be.
            driver.ignore_egg_requirements = True
            # Force use of only cluster hosts by adding this requirement.
            driver.extra_resources = dict(allocator='PCCCluster')

        super(driver.__class__, driver).execute()

        outputs = []
        for c in driver.recorders[0].get_iterator():
            output = []
            for n in driver.outputNames:
                if c.__contains__(n):
                    output.append(c.get_output(n))
                else:
                    raise Exception,'No outputs from simulator matching requested output \'{0}\'. Available outputs: {1}'.format(n,c.get_outputs())
            outputs.append(output)

    if not driver.UseParametersCSV:
        with open('parameters.csv', 'w') as f:  # write log file of inputs and outputs
            f.write(','.join(driver.inputNames)+','+','.join(driver.outputNames)+'\n')
            for x,i in enumerate(inputs):
                f.write(','.join(str(y) for y in inputs[x]))
                if x<len(outputs):
                    f.write(','+','.join(str(y) for y in outputs[x]))
                f.write('\n')
            f.close()

    if len(outputs) != len(inputs):
        raise Exception,'Simulator returned only {0} results of a requested {1}. See parameters.csv for details.'.format(len(outputs),len(inputs))

    return asarray(outputs)

def UseLocalParallel():
    global UseCluster, UseParallel
    UseCluster = False
    UseParallel = True

def InitializeCluster(hostnames, pydir, identity_filename=None):
    print 'Connecting to cluster...'
    machines = []

    for host in hostnames:
        machines.append(ClusterHost(
            hostname=host,
            python = pydir,
            tunnel_incoming=True, tunnel_outgoing=True,
            identity_filename=identity_filename))
    
    _SSH.extend(['-o', 'StrictHostKeyChecking=no'])   #somewhat dangerous, this automatically adds the host key to known_hosts
    cluster = ClusterAllocator('PCCCluster', machines, allow_shell=True)
    RAM.insert_allocator(0, cluster)
    print 'Servers connected on cluster:',cluster.max_servers({})[0]
    global UseCluster
    UseCluster = True
    
         
        
        
