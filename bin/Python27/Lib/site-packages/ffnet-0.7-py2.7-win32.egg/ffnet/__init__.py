'''
ffnet is a fast and easy-to-use feed-forward neural 
network training solution for python.
 
Unique features present in ffnet
--------------------------------
1. Any network connectivity without cycles is allowed.
2. Training can be performed with use of several optimization 
   schemes including: standard backpropagation with momentum, rprop,
   conjugate gradient, bfgs, tnc (with multiprocessing)
   and genetic alorithm based optimization.
3. There is access to exact partial derivatives of network outputs 
   vs. its inputs.
4. Automatic normalization of data.
 
Basic assumptions and limitations
---------------------------------
1. Network has feed-forward architecture.
2. Input units have identity activation function, 
   all other units have sigmoid activation function.
3. Provided data are automatically normalized, both input and output, 
   with a linear mapping to the range (0.15, 0.85).
   Each input and output is treated separately (i.e. linear map is 
   unique for each input and output).
4. Function minimized during training is a sum of squared errors 
   of each output for each training pattern.
 
Performance
-----------
Excellent computational performance is achieved implementing core
functions in fortran 77 and wrapping them with f2py. ffnet outstands
in performance pure python training packages and is competitive to
'compiled language' software. Incorporation of multiprocessing capabilities
(tnc algorithm so far) makes ffnet ideal for large scale (really!)
problems. Moreover, a trained network can be exported to fortran sources, 
compiled and called from many programming languages.
 
Usage
-----
Basic usage of the package is outlined below. 
See description of ffnet module and its functions 
(and especially ffnet class) for detailed explanations.
 
>>> from ffnet import ffnet, mlgraph, savenet, loadnet, exportnet
>>> conec = mlgraph( (2,2,1) )
>>> net = ffnet(conec)
>>> input = [ [0.,0.], [0.,1.], [1.,0.], [1.,1.] ]
>>> target  = [ [1.], [0.], [0.], [1.] ]
>>> net.train_tnc(input, target, maxfun = 1000)
>>> net.test(input, target, iprint = 2)
>>> savenet(net, "xor.net")
>>> exportnet(net, "xor.f")
>>> net = loadnet("xor.net")
>>> answer = net( [ 0., 0. ] )
>>> partial_derivatives = net.derivative( [ 0., 0. ] )
 
Usage examples with full description can be found in 
examples directory of the source distribution.
'''

from _version import version
import fortran
import ffnet as ffnetmodule
from ffnet import ffnet, \
                  mlgraph, \
                  tmlgraph, \
                  imlgraph, \
                  savenet, \
                  loadnet, \
                  exportnet, \
                  readdata
from pikaia import pikaia
import _tests
