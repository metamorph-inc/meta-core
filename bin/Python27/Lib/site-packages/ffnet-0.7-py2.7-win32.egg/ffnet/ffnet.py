########################################################################
##  Copyright (C) 2006 by Marek Wojciechowski
##  <mwojc@p.lodz.pl>
##
##  Distributed under the terms of the GNU General Public License (GPL)
##  http://www.gnu.org/copyleft/gpl.html
########################################################################

'''
Module containing ffnet class and utility functions.
'''

from _version import version
from scipy import zeros, ones, random, optimize, sqrt, ndarray, array
import networkx as NX
from fortran import _ffnet as netprop
from pikaia import pikaia
import sys

def mlgraph(arch, biases = True):
    '''
    Creates standard multilayer network with full connectivity list.
    '''
    nofl = len(arch)
    conec = []
    if nofl: trg = arch[0]
    for l in xrange(1, nofl):
        layer = arch[l]
        player = arch[l-1]
        srclist = range(trg-player+1, trg+1)
        if biases: srclist += [0]
        for i in xrange(layer):
            trg += 1
            for src in srclist:
                conec.append((src, trg))
    return conec

def imlgraph(arch, biases = True):
    '''
    Creates special layered network where outputs are
    independent from each other.
    Exemplary architecture definition:
    arch = (3, [(4,), (), (6, 3)], 3).
    With such an arch, imlgraph builds three independent
    multilayer graphs: 3-4-1, 3-1, 3-6-3-1
    and merges them into one graph with common input nodes.
    
    Simplified version of the above architecture syntax is:
    arch = (3, 3, 3)  #exactly as in the mlgraph
    Three nets: 3-3-1, 3-3-1, 3-3-1 are merged in this case.
    '''
    #Checks of the architecture
    if len(arch) < 3:
        raise TypeError("Wrong architecture definition (at least 3 layers needed).")
    if isinstance(arch[1], int):
        arch = tuple(arch)
        arch = arch[:1] + tuple([[ arch[1:-1] ] * arch[-1]]) + arch[-1:]
    elif isinstance(arch[1], (list, tuple)):
        if len(arch[1]) != arch[2]:
            raise TypeError("Length of arch[1] should be equal to arch[2].")
    else: raise TypeError("Wrong architecture definition.")
    
    #Merging function
    def merge(conec, conec_tmp, nofi):
        from scipy import array, where
        try:
            trans = array(conec).max() - nofi
            tmp = array(conec_tmp)
            tmp = where(tmp > nofi, tmp + trans, tmp)
            conec_tmp = [ tuple(c) for c in tmp ]
            return conec + conec_tmp
        except ValueError:
            return conec_tmp
        
    nofi = arch[0]
    inps = arch[:1]
    outs = (1,)
    conec = []
    for hids in arch[1]:
        arch_tmp = tuple(inps) + tuple(hids) + tuple(outs)
        conec_tmp = mlgraph(arch_tmp, biases=biases)
        conec = merge(conec, conec_tmp, nofi)
    return conec

def tmlgraph(arch, biases = True):
    '''
    Creates multilayer network full connectivity list,
    but now layers have connections with all preceding layers
    (not only "the nearest" one).
    '''
    nofl = len(arch)
    conec = []; srclist = []
    if biases: srclist = [0]
    if nofl: trg = arch[0]
    for l in xrange(1, nofl):
        layer = arch[l]
        player = arch[l-1]
        srclist += range(trg-player+1, trg+1)
        for i in xrange(layer):
            trg += 1
            for src in srclist:
                conec.append((src, trg))
    return conec

def _dependency(G, source):
    """
    Returns subgraph of G connecting source with all sinks.
    """
    H = G.copy()
    node_removal = 1
    while node_removal:
        node_removal = 0
        for node in H.nodes():
            if not H.in_degree(node) and node != source:
                H.remove_node(node)
                node_removal = 1
    return H

def _linear(a, b, c, d):
    '''
    Returns coefficients of linear map from range (a,b) to (c,d)
    '''
    #if b == a: raise ValueError("Mapping not possible due to equal limits")
    if b == a: 
        c1 = 0.0
        c2 = ( c + d ) / 2.
    else:
        c1 = ( d - c ) / ( b - a )
        c2 = c - a * c1
    return c1, c2

def _norms(inarray, lower = 0., upper = 1.):
    '''
    Gets normalization information from an array, for use in ffnet class.
    (lower, upper) is a range of normalization.
    inarray is 2-dimensional, normalization parameters are computed
    for each column...
    '''
    limits = []; en = []; de = []
    inarray = array(inarray).transpose()
    for col in inarray:
        maxarr = max(col)
        minarr = min(col)
        limits += [(minarr, maxarr)]
        en += [_linear(minarr, maxarr, lower, upper)]
        de += [_linear(lower, upper, minarr, maxarr)]
    return array(limits), array(en), array(de)
    
def _normarray(inarray, coeff):
    ''' 
    Normalize 2-dimensional array linearly column by column 
    with provided coefficiens.
    '''
    #if coeff is not None:
    inarray = array(inarray).transpose()
    coeff = array(coeff)
    i = inarray.shape[0]
    for ii in xrange(i):
        inarray[ii] = inarray[ii] * coeff[ii,0] + coeff[ii,1]
    return inarray.transpose()
    #else: print "Lack of normalization parameters. Nothing done."

def _ffconec(conec):
    """
    Checks if conec is acyclic, sorts it if necessary and returns tuple:
    (conec, inno, hidno, outno) where:
    conec - sorted input connectivity
    inno/hidno/outno  - lists of input/hidden/ouput units
    """
    if len(conec) == 0: raise ValueError("Empty connectivity list")
    graph = NX.DiGraph()
    graph.add_edges_from(conec)
    snodes = NX.topological_sort(graph) # raises NetworkXUnfeasible if cycles are found
    conec = []; inno = []; hidno = []; outno = []
    for node in snodes:
        ins = graph.in_edges(node)
        outs = graph.out_edges(node)
        if not ins and node != 0 :  # biases handling
            inno += [node]
        else:
            conec += ins   #Maybe + [(0,node)] i.e. bias
            if not outs: outno += [node]
            else: 
                if node != 0: hidno += [node] #bias handling again
    return graph, conec, inno, hidno, outno

def _bconec(conec, inno):
    """
    Returns positions of edges of reversed graph in conec (for backprop). 
    Conec is assumed to be acyclic.
    """
    bgraph = NX.DiGraph()
    bgraph.add_edges_from(conec)
    bgraph = bgraph.reverse()
    bgraph.remove_nodes_from(inno)
    try: bgraph.remove_node(0) #handling biases
    except: pass
    bsnodes = NX.topological_sort(bgraph)
    bconecno = []
    for bnode in bsnodes:
        for bedge in bgraph.in_edges(bnode):
            edge = (bedge[1], bedge[0])
            idx = conec.index(edge) + 1
            bconecno.append(idx)
    return bgraph, bconecno
    
def _dconec(conec, inno):
    """
    Return positions of edges (in conec) of graphs for
    derivative calculation, all packed in one list (dconecno). Additionaly
    beginings of each graph in this list is returned (dconecmk)
    """
    dgraphs = []; dconecno = []; dconecmk = [0]
    for idx, i in enumerate(inno):
        dgraph = NX.DiGraph()
        dgraph.add_edges_from(conec)
        dgraph = _dependency(dgraph, i) 
        dsnodes = NX.topological_sort(dgraph)
        for dnode in dsnodes:
            for dedge in dgraph.in_edges(dnode):
                idx = conec.index(dedge) + 1
                dconecno.append(idx)
        dgraphs.append(dgraph)
        dconecmk.append(len(dconecno))
    return dgraphs, dconecno, dconecmk


class ffnet:
    """
    Feed-forward neural network main class.
    
    NETWORK CREATION:
    Creation of the network consist in delivering list of neuron 
    connections:
        conec = [[1, 3], [2, 3], [0, 3] 
                 [1, 4], [2, 4], [0, 4] 
                 [3, 5], [4, 5], [0, 5]]
        net = ffnet(conec)
    0 (zero) in conec is a special unit representing bias. If there is
    no connection from 0, bias is not considered in the node.
    Only feed-forward directed graphs are allowed. Class makes check
    for cycles in the provided graph and raises TypeError if any.
    All nodes (exept input ones) have sigmoid activation function.

    Although generation of conec is left to the user, there are several
    functions provided to facilitate this task. See description of 
    the following functions: mlgraph, imlgraph, tmlgraph.
    More architectures may be provided in the future.
    
    Weights are automatically initialized at the network creation. They can
    be reinitialized later with 'randomweights' method.
    
    TRAINING NETWORK:
    There are several training methods included, currently:
    train_momentum, train_rprop, train_genetic, train_cg, 
    train_bfgs, train_tnc.
    The simplest usage is, for example:
        net.train_tnc(input, target)
    where 'input' and 'target' is data to be learned. Class performs data
    normalization by itself and records encoding/decoding information 
    to be used during network recalling.
    Class makes basic checks of consistency of data.
    
    For information about training prameters see appropriate 
    method description.
    
    RECALLING NETWORK:
    Usage of the trained network is as simple as function call:
        ans = net(inp)
    or, alternatively:
        ans = net.call(inp)
    where 'inp' - list of network inputs and 'ans' - list of network outputs
    There is also possibility to retrieve partial derivatives of 
    output vs. input at given input point:
        deriv = n.derivative(inp)
    'deriv' is an array of the form:
        | o1/i1, o1/i2, ..., o1/in |
        | o2/i1, o2/i2, ..., o2/in |
        | ...                      |
        | om/i1, om/i2, ..., om/in |
    
    LOADING/SAVING/EXPORTING NETWORK
    There are three helper functions provided with this class:
    savenet, loadnet and exportnet.
    Basic usage:
        savenet(net, filename)   --> pickles network
        net = loadnet(filename)  --> loads pickled network
        exportnet(net, filename) --> exports network to fortran source
    """
    def __init__(self, conec, lazy_derivative = True):
        graph, conec, inno, hidno, outno = _ffconec(conec)
        self.graph = graph        
        self.conec = array(conec)
        self.inno = array(inno)
        self.hidno = array(hidno)
        self.outno = array(outno)        
        
        bgraph, bconecno = _bconec(conec, self.inno)
        self.bgraph = bgraph
        self.bconecno = array(bconecno)
        
        # Ommit creating data for derivatives here (which is expensive for large nets)
        if lazy_derivative:
            self.dgraphs = None
            self.dconecno = None
            self.dconecmk = None
        else:
            self._set_dconec()

        #max(graph) below needed if graph lacks some numbers
        self.units = zeros(max(graph), 'd')
        # initialize weights
        self.randomweights()
        # set initial normalization parameters
        self._setnorm()
        
    def __repr__(self):
        info = "Feed-forward neural network: \n" + \
               "inputs:  %4i \n" %(len(self.inno)) + \
               "hiddens: %4i \n" %(len(self.hidno)) + \
               "outputs: %4i \n" %(len(self.outno)) + \
               "connections and biases: %4i" %(len(self.conec)) 
        return info

    def __call__(self, inp):
        ## Something more sophisticated is needed here?
        return self.call(inp)

    def _set_dconec(self):
        conec = [tuple(c) for c in self.conec] # maybe rather some changes in _dconec?
        dgraphs, dconecno, dconecmk = _dconec(conec, self.inno)
        self.dgraphs = dgraphs
        self.dconecno = array(dconecno)
        self.dconecmk = array(dconecmk)
        
    def call(self, inp):
        """
        Returns network answer (numpy array) to input sequence 
        (for one input sample or 2D array of input samples)
        """
        if not isinstance(inp, ndarray): inp = array(inp, 'd')
        if inp.ndim == 1:
            output = netprop.normcall(self.weights, self.conec, self.units, \
                            self.inno, self.outno, self.eni, self.deo, inp)
            return output
        if inp.ndim == 2:
            output = netprop.normcall2(self.weights, self.conec, self.units, \
                            self.inno, self.outno, self.eni, self.deo, inp)
            return output
        raise TypeError("Input is not valid")

    def derivative(self, inp):
        """
        Returns partial derivatives of the network's 
        output vs its input at given input point 
        (for one input sample or 2D array of input samples)
        in the following array:
            | o1/i1, o1/i2, ..., o1/in |
            | o2/i1, o2/i2, ..., o2/in |
            | ...                      |
            | om/i1, om/i2, ..., om/in |
        """
        if self.dconecno is None:  #create dconecno (only od demand)
            self._set_dconec()
           
        if not isinstance(inp, ndarray): inp = array(inp, 'd')
        if inp.ndim == 1:
            deriv = netprop.normdiff(self.weights, self.conec, self.dconecno, self.dconecmk, \
                            self.units, self.inno, self.outno, self.eni, self.ded, inp)
            return deriv 
        if inp.ndim == 2:
            deriv = netprop.normdiff2(self.weights, self.conec, self.dconecno, self.dconecmk, \
                            self.units, self.inno, self.outno, self.eni, self.ded, inp)
            return deriv
        raise TypeError("Input is not valid")

    def sqerror(self, input, target):
        """
        Returns 0.5*(sum of squared errors at output)
        for input and target arrays being first normalized.
        Might be slow in frequent use, because data normalization is
        performed at each call.
        """
        input, target = self._setnorm(input, target)
        err  = netprop.sqerror(self.weights, self.conec, self.units, \
                               self.inno, self.outno, input, target)
        return err
    
    def sqgrad(self, input, target):
        """
        Returns gradient of sqerror vs. network weights.
        Input and target arrays are first normalized.
        Might be slow in frequent use, because data normalization is
        performed at each call.
        """
        input, target = self._setnorm(input, target) 
        g  = netprop.grad(self.weights, self.conec, self.bconecno, self.units, \
                          self.inno, self.outno, input, target)
        return g
    
    def randomweights(self):
        """
        Randomize weights due to Bottou proposition.
        """
        nofw = len(self.conec)
        weights = zeros(nofw, 'd')
        for w in xrange(nofw):
            trg = self.conec[w,1]
            n = len(self.graph.predecessors(trg))
            bound = 2.38 / sqrt(n)
            weights[w] = random.uniform(-bound, bound)
        self.weights = weights
        self.trained = False

    def _testdata(self, input, target):
        """
        Tests input and target data.
        """
        # Test conversion
        try: 
            if not isinstance(input, ndarray): input = array(input, 'd')
            #input = array(input, 'd')
        except: raise ValueError("Input cannot be converted to numpy array")
        try: 
            if not isinstance(target, ndarray): target = array(target, 'd')
            #target = array(target, 'd')
        except: raise ValueError("Target cannot be converted to numpy array")
        
        #if input.dtype.char != 'd': input = array(input, 'd')
        
        #Convert 1-d arrays to 2-d (this allows to put 1-d arrays
        #for training if we have one input and/or one output
        if len(self.inno) == 1 and len(input.shape) == 1:
            input = input.reshape( (input.shape[0], 1) )
        if len(self.outno) == 1 and len(target.shape) == 1:
            target = target.reshape( (target.shape[0], 1) )        

        #Test some sizes
        numip = input.shape[0]; numop = target.shape[0]
        if numip != numop:
            raise ValueError \
            ("Data not aligned: input patterns %i, target patterns: %i" %(numip, numop))
        numi = len(self.inno); numiv = input.shape[1]
        if numiv != numi:
            raise ValueError \
            ("Inconsistent input data, input units: %i, input values: %i" %(numi, numiv))
        numo = len(self.outno); numov = target.shape[1]
        if numov != numo:
            raise ValueError \
            ("Inconsistent target data, target units: %i, target values: %i" %(numo, numov))
        
        return input, target

    def _setnorm(self, input = None, target = None):
        """
        Retrieves normalization info from training data and normalizes data.
        This method sets self.renormalize attribute to control normalization.
        """
        numi = len(self.inno); numo = len(self.outno)
        if input is None and target is None:
            self.inlimits  = array( [[0.15, 0.85]]*numi ) #informative only
            self.outlimits = array( [[0.15, 0.85]]*numo ) #informative only
            self.eni = self.dei = array( [[1., 0.]] * numi )
            self.eno = self.deo = array( [[1., 0.]] * numo )
            self.ded = ones((numo, numi), 'd')
            self.renormalize = True  # this is set by __init__
        else:
            input, target = self._testdata(input, target)
            
            # Warn if any input or target node takes a one single value
            # I'm still not sure where to put this check....
            for i, col in enumerate(input.transpose()):
                if max(col) == min(col):
                    print "Warning: %ith input node takes always a single value of %f." %(i+1, max(col))

            for i, col in enumerate(target.transpose()):
                if max(col) == min(col):
                    print "Warning: %ith target node takes always a single value of %f." %(i+1, max(col))
            
            #limits are informative only, eni,dei/eno,deo are input/output coding-decoding
            if self.renormalize: 
                self.inlimits, self.eni, self.dei = _norms(input, lower=0.15, upper=0.85)
                self.outlimits, self.eno, self.deo = _norms(target, lower=0.15, upper=0.85)
                self.ded = zeros((numo,numi), 'd')
                for o in xrange(numo):
                    for i in xrange(numi):
                        self.ded[o,i] = self.eni[i,0] * self.deo[o,0]
                self.renormalize = False
                
            return _normarray(input, self.eni), _normarray(target, self.eno)

    def train_momentum(self, input, target, eta = 0.2, momentum = 0.8, \
                        maxiter = 10000, disp = 0):
        """
        Simple backpropagation training with momentum.
    
        Allowed parameters:
        eta             - descent scaling parameter (default is 0.2)
        momentum        - momentum coefficient (default is 0.8)
        maxiter         - the maximum number of iterations (default is 10000)
        disp            - print convergence message if non-zero (default is 0)
        """
        input, target = self._setnorm(input, target)
        if disp:
            err  = netprop.sqerror(self.weights, self.conec, self.units, \
                                   self.inno, self.outno, input, target)
            print "Initial error --> 0.5*(sum of squared errors at output): %.15f" %err
        self.weights = netprop.momentum(self.weights, self.conec, self.bconecno, \
                                        self.units, self.inno, self.outno, input, \
                                        target, eta, momentum, maxiter)
        if disp:
            err  = netprop.sqerror(self.weights, self.conec, self.units, \
                                   self.inno, self.outno, input, target)
            print "Final error   --> 0.5*(sum of squared errors at output): %.15f" %err

    def train_rprop(self, input, target, \
                    a = 1.2, b = 0.5, mimin = 0.000001, mimax = 50., \
                    xmi = 0.1, maxiter = 10000, disp = 0):
        """
        Rprop training algorithm.
        
        Allowed parameters:
        a               - training step increasing parameter (default is 1.2)
        b               - training step decreasing parameter (default is 0.5)
        mimin           - minimum training step (default is 0.000001)
        mimax           - maximum training step (default is 50.)
        xmi             - vector containing initial training steps for weights;
                          if 'xmi' is a scalar then its value is set for all
                          weights (default is 0.1)
        maxiter         - the maximum number of iterations (default is 10000)
        disp            - print convergence message if non-zero (default is 0)
        
        Method updates network weights and returns 'xmi' vector 
        (after 'maxiter' iterations).
        """
        input, target = self._setnorm(input, target)
        if type(xmi).__name__ in ['float', 'int']:
            xmi = [ xmi ]*len(self.conec)
        
        if disp:
            err  = netprop.sqerror(self.weights, self.conec, self.units, \
                                   self.inno, self.outno, input, target)
            print "Initial error --> 0.5*(sum of squared errors at output): %.15f" %err
        self.weights, xmi = netprop.rprop(self.weights, self.conec, self.bconecno, \
                                          self.units, self.inno, self.outno, input, \
                                          target, a, b, mimin, mimax, xmi, maxiter)
        if disp:
            err  = netprop.sqerror(self.weights, self.conec, self.units, \
                                   self.inno, self.outno, input, target)
            print "Final error   --> 0.5*(sum of squared errors at output): %.15f" %err
        return xmi

    def train_genetic(self, input, target, **kwargs):
        """
        Global weights optimization with genetic algorithm.
    
        Allowed parameters:
        lower        - lower bound of weights values (default is -25.)
        upper        - upper bound of weights values (default is 25.)
        individuals  - number of individuals in a population (default
                       is 20)
        generations  - number of generations over which solution is
                       to evolve (default is 500)
        crossover    - crossover probability; must be  <= 1.0 (default
                       is 0.85). If crossover takes place, either one
                       or two splicing points are used, with equal
                       probabilities
        mutation     - 1/2/3/4/5 (default is 2)
                       1=one-point mutation, fixed rate
                       2=one-point, adjustable rate based on fitness
                       3=one-point, adjustable rate based on distance
                       4=one-point+creep, fixed rate
                       5=one-point+creep, adjustable rate based on fitness
                       6=one-point+creep, adjustable rate based on distance
        initrate     - initial mutation rate; should be small (default
                       is 0.005) (Note: the mutation rate is the proba-
                       bility that any one gene locus will mutate in
                       any one generation.)
        minrate      - minimum mutation rate; must be >= 0.0 (default
                       is 0.0005)
        maxrate      - maximum mutation rate; must be <= 1.0 (default
                       is 0.25)
        fitnessdiff  - relative fitness differential; range from 0
                       (none) to 1 (maximum).  (default is 1.)
        reproduction - reproduction plan; 1/2/3=Full generational
                       replacement/Steady-state-replace-random/Steady-
                       state-replace-worst (default is 3)
        elitism      - elitism flag; 0/1=off/on (default is 0)
                       (Applies only to reproduction plans 1 and 2)
        verbosity    - printed output 0/1/2=None/Minimal/Verbose
                       (default is 0)
    
        Note: this optimization routine is a python wrapper for fortran pikaia code.
    
        For more info see pikaia homepage and documentation:
        http://www.hao.ucar.edu/modeling/pikaia/pikaia.php
        """
        input, target = self._setnorm(input, target)
        lower = -25.
        upper =  25.
        if 'lower' in kwargs: lower = kwargs['lower']; del kwargs['lower']
        if 'upper' in kwargs: upper = kwargs['upper']; del kwargs['upper']
        if lower >= upper: raise ValueError("Wrong weights range: (%f, %f)" %(lower, upper))
        if 'individuals' not in kwargs: kwargs['individuals'] = 20
        func = netprop.pikaiaff
        n = len(self.weights)
        extra_args = (self.conec, self.units, self.inno,
                      self.outno, input, target, lower, upper)
        self.weights = pikaia(func, n, extra_args, **kwargs)
        self.weights = netprop.vmapa(self.weights, 0., 1., lower, upper)
        self.trained = 'genetic'
    
    def train_cg(self, input, target, **kwargs):
        """
        Train network with conjugate gradient algorithm.
        scipy.optimize.fmin_cg keyword arguments are accepted, except:
            args
            fprime 
        
        NOTE: This procedure does produce no output by default.
        
    Documentation of scipy.optimize.fmin_cg:
    ----------------------------------------
    """
        if 'maxiter' not in kwargs: kwargs['maxiter'] = 10000
        input, target = self._setnorm(input, target)
        func = netprop.func
        fprime = netprop.grad
        extra_args = (self.conec, self.bconecno, self.units, \
                           self.inno, self.outno, input, target)
        self.weights = optimize.fmin_cg(func, self.weights, fprime=fprime, \
                                        args=extra_args, **kwargs)
        self.trained = 'cg'
    train_cg.__doc__ += optimize.fmin_cg.__doc__ # paste __doc__

    def train_bfgs(self, input, target, **kwargs):
        """
        Train network with constrained version of BFGS algorithm.
        scipy.optimize.fmin_l_bfgs_b keyword arguments are accepted, except:
            args
            fprime 
        Bounds are set for all weights as (-100., 100.) by default.
        
        NOTE: This procedure does produce no output by default.
        
    Documentation of scipy.optimize.fmin_l_bfgs_b:
    ------------------------------------------
    """
        if sys.platform.startswith('aix'): return
        
        input, target = self._setnorm(input, target)
        if 'bounds' not in kwargs: kwargs['bounds'] = ((-100., 100.),)*len(self.conec)
        func = netprop.func
        fprime = netprop.grad
        extra_args = (self.conec, self.bconecno, self.units, \
                           self.inno, self.outno, input, target)
        self.weights = optimize.fmin_l_bfgs_b(func, self.weights, fprime=fprime, \
                                              args=extra_args, **kwargs)[0]
        self.trained = 'bfgs'
    train_bfgs.__doc__ += optimize.fmin_l_bfgs_b.__doc__ # paste __doc__

    def train_tnc(self, input, target, nproc = 1, **kwargs):
        """
        Train network with constrained truncated Newton algorithm (TNC).
        scipy.optimize.fmin_tnc keyword arguments are accepted, except:
            args
            fprime 
        Bounds are set for all weights as (-100., 100.) by default.
        
        NOTE: This procedure does produce no output by default.
        
    Documentation of scipy.optimize.fmin_tnc:
    -----------------------------------------
    """        
        input, target = self._setnorm(input, target)
        if 'messages' not in kwargs: kwargs['messages'] = 0
        if 'bounds' not in kwargs: kwargs['bounds'] = ((-100., 100.),)*len(self.conec)
       
        # multiprocessing version if nproc > 1
        if (isinstance(nproc, int) and nproc > 1) or nproc in (None, 'ncpu'):
            if nproc == 'ncpu': nproc = None
            self._train_tnc_mp(input, target, nproc = nproc, **kwargs)
            return
            
        # single process version
        func = netprop.func
        fprime = netprop.grad       
        extra_args = (self.conec, self.bconecno, self.units, \
                           self.inno, self.outno, input, target)
        res = optimize.fmin_tnc(func, self.weights, fprime=fprime, \
                                         args=extra_args, **kwargs)
        self.weights = array( res[0] )
        self.trained = 'tnc'
    train_tnc.__doc__ += optimize.fmin_tnc.__doc__ # paste __doc__
    
    def _train_tnc_mp(self, input, target, nproc = None, **kwargs):        
        # register training data at mpprop module level
        # this have to be done *BEFORE* creating pool
        import _mpprop as mpprop
        try: key = max(mpprop.nets) + 1
        except ValueError: key = 0  # uniqe identifier for this training
        mpprop.nets[key] = self 
        mpprop.inputs[key] = input
        mpprop.targets[key] = target

        # create processing pool
        from multiprocessing import Pool, cpu_count
        if nproc is None: nproc = cpu_count()
        if sys.platform.startswith('win'):
            # we have to initialize processes in pool on Windows, because
            # each process reimports mpprop thus the registering
            # made above is not enough
            # WARNING: this might be slow and memory hungry
            # (no shared memory, all is serialized and copied)
            initargs = [key, self, input, target]
            pool = Pool(nproc, initializer = mpprop.initializer, initargs=initargs)
        else:
            pool = Pool(nproc)
        # generate splitters for training data
        splitters = mpprop.splitdata(len(input), nproc)
        
        # train
        func = mpprop.mpfunc
        fprime = mpprop.mpgrad
        
        #if 'messages' not in kwargs: kwargs['messages'] = 0
        #if 'bounds' not in kwargs: kwargs['bounds'] = ((-100., 100.),)*len(self.conec)
        res = optimize.fmin_tnc(func, self.weights, fprime = fprime, \
                                args = (pool, splitters, key), **kwargs)
        self.weights = res[0]
        
        # remove references from mpprop
        del mpprop.nets[key] 
        del mpprop.inputs[key]
        del mpprop.targets[key]
        pool.terminate()
        del pool

    def test(self, input, target, iprint = 1, filename = None):
        """
        Calculates output and parameters of regression line of targets vs. outputs.
        
        Returns: (output, regress)
        
        where regress contains regression line parameters for each output node. These
        parameters are: 
        
        (slope, intercept, r-value, p-value, stderr-of-slope, stderr-of-estimate).
        
        Optional parameters:
        iprint   - verbosity level:
                   0 - print nothing
                   1 - print regression parameters for each output node
                   2 - print additionaly general network info and all targets vs. outputs
                       (default is 1)
        filename - string with path to the file; if given, output is redirected 
                   to this file (default is None)
        """
        # Check if we dump stdout to the file
        if filename:
            import sys
            file = open(filename, 'w')
            saveout = sys.stdout
            sys.stdout = file
        # Print network info
        if iprint == 2:
            print self
            print
        # Test data and get output
        input, target = self._testdata(input, target)
        nump = len(input)
        output = self(input) #array([self(inp) for inp in input])
        # Calculate regression info
        from scipy.stats import linregress
        numo = len(self.outno)
        target = target.transpose()
        output = output.transpose()
        regress = []
        if iprint: print "Testing results for %i testing cases:" % nump
        for o in xrange(numo):
            if iprint:
                print "OUTPUT %i (node nr %i):" %(o+1, self.outno[o])
            if iprint == 2:    
                print "Targets vs. outputs:"
                for p in xrange(nump):
                    print "%4i % 13.6f % 13.6f" %(p+1, target[o,p], output[o,p])
            x = target[o]; y = output[o]
            r = linregress(x, y)
            # linregress calculates stderr of the slope instead of the estimate, even 
            # though the docs say something else. we calculate the thing here manually
            sstd = r[-1]
            estd = sstd * sqrt( ( ( x-x.mean() )**2 ).sum() )
            r += (estd,)
            if iprint:
                print "Regression line parameters:"
                print "slope         = % f" % r[0] 
                print "intercept     = % f" % r[1]
                print "r-value       = % f" % r[2]
                print "p-value       = % f" % r[3]
                print "slope stderr  = % f" % r[4]
                print "estim. stderr = % f" % r[5]
            regress.append(r)
            if iprint: print
        # Close file and restore stdout
        if filename: 
            file.close()
            sys.stdout = saveout
            
        return output.transpose(), regress

def savenet(net, filename):
    """
    Saves network to a file using cPickle module.
    """
    import cPickle
    file = open(filename, 'w')
    cPickle.dump(net, file)
    file.close()
    return
    
def loadnet(filename):
    """
    Loads network pickled previously with 'savenet'. 
    """    
    import cPickle
    file = open(filename, 'r')
    net = cPickle.load(file)
    return net

def _exportfortran(net, filename, name, derivative = True):
    import _py2f as py2f
    f = open( filename, 'w' )
    f.write( py2f.fheader( net, version = version ) )
    f.write( py2f.fcomment() )
    f.write( py2f.ffnetrecall(net, name) )
    if derivative:
        if net.dconecno is None: net._set_dconec() #set on demand
        f.write( py2f.fcomment() )
        f.write( py2f.ffnetdiff(net, 'd' + name) )
    f.close()
    
def exportnet(net, filename, name = 'ffnet', lang = None, derivative = True):
    """
    Exports network to a compiled language source code.
    Currently only fortran is supported.

    There are two routines exported. First one, for
    recalling the network, is named as indicated by keyword argument
    'name'. The second one, for calculating partial derivatives, have
    the same name with 'd' prefix. 'ffnet' and 'dffnet' are exported
    by default.
    
    NOTE: You need 'ffnet.f' file distributed with ffnet
          sources to get the exported routines to work.
    """

    # Determine language if not specified
    if not lang:
        import os.path
        fname, ext = os.path.splitext(filename)
        if ext in ['.f', '.for', '.f90']:
            lang = 'fortran'

    if lang == 'fortran':
        _exportfortran(net, filename, name, derivative = derivative)
    else:
        if lang:
            raise TypeError("Unsupported language " + lang)
        else:
            raise TypeError("Unspecified language")
    return

def readdata(filename, **kwargs):
    """
    Reads arrays from ASCII files. Just calls numpy.loadtxt passing
    all keyword arguments.
    
    Documentation of numpy.loadtxt:
    -------------------------------
    """
    from numpy import loadtxt
    data = loadtxt(filename, **kwargs)
    return data
import numpy
readdata.__doc__ += numpy.loadtxt.__doc__ # paste __doc__
