from numpy import *
from scipy.stats import beta,norm,lognorm,uniform
import SRC_regress
from model_calls import run_list

def SA_FAST(driver):
    
    # First order indicies for a given model computed with Fourier Amplitude Sensitivity Test (FAST).
    # R. I. Cukier, C. M. Fortuin, Kurt E. Shuler, A. G. Petschek and J. H. Schaibly.
    # Study of the sensitivity of coupled reaction systems to uncertainties in rate coefficients.
    # I-III Theory/Applications/Analysis The Journal of Chemical Physics
    #
    # Input:
    # inpt : no. of input factors
    #
    # Output:
    # SI[] : sensitivity indices
    # Other used variables/constants:
    # OM[] : frequencies of parameters
    # S[] : search curve
    # X[] : coordinates of sample points
    # Y[] : output of model
    # OMAX : maximum frequency
    # N : number of sample points
    # AC[],BC[]: fourier coefficients
    # V : total variance
    # VI : partial variances

    # ----------------------  Setup  ---------------------------
    methd = 'FAST'
    method = '9'
    
    mu      = [inp.get_I_mu() for inp in driver.inputs]
    I_sigma = [inp.get_I_sigma() for inp in driver.inputs]
    inpt    = len(driver.inputs)
    input   = driver.inputNames
    krig    = driver.krig
    limstate= driver.limstate
    lrflag  = driver.lrflag
    n_meta  = driver.n_meta
    nEFAST  = driver.nEFAST
    nSOBOL  = driver.nSOBOL
    nMCS    = driver.nMCS
    nodes   = driver.nodes
    order   = driver.order
    otpt    = len(driver.outputNames)
    output  = driver.outputNames
    p       = driver.p
    plotf   = 0
    r       = driver.r
    simple  = driver.simple
    stvars  = driver.stvars
    
    # ----------------------  Model  ---------------------------
    
    #
    MI = 4#: maximum number of fourier coefficients that may be retained in
    # calculating the partial variances without interferences between the assigned frequencies
    #
    # Frequency assignment to input factors.
    OM = SETFREQ(inpt)
    # Computation of the maximum frequency
    # OMAX and the no. of sample points N.
    OMAX = int(OM[inpt-1])
    N = 2 * MI * OMAX + 1
    # Setting the relation between the scalar variable S and the coordinates
    # {X(1),X(2),...X(inpt)} of each sample point.
    S = pi / 2.0 * (2 * arange(1,N+1) - N-1) / N
    ANGLE = matrix(OM).T * matrix(S)
    X = 0.5 + arcsin(sin(ANGLE.T)) / pi
    # Transform distributions from standard uniform to general.

    for j in range(inpt):    
        if stvars[j].dist == 'NORM':
            X[:,j] = norm.ppf(uniform.cdf(X[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])        
        elif stvars[j].dist == 'LNORM':        
            X[:,j] = lognorm.ppf(uniform.cdf(X[:, j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
        elif stvars[j].dist == 'BETA':        
            X[:,j] = beta.ppf(uniform.cdf(X[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
        elif stvars[j].dist == 'UNIF':        
            X[:,j] = uniform.ppf(uniform.cdf(X[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])

    # Do the N model evaluations.
    Y = zeros((N, otpt))        
    if krig == 1:            
        load("dmodel")            
        Y = predictor(X, dmodel)            
    else:
        values = []            
        for p in range(N):
#            print 'Running simulation on test',p+1,'of',N
#            Y[p] = run_model(driver, array(X[p])[0])
            values.append(array(X[p])[0])
        Y = run_list(driver, values)

    # Computation of Fourier coefficients.
    AC = zeros((N, otpt))# initially zero
    BC = zeros((N, otpt))# initially zero
#    q = int(N / 2)-1
    q = (N-1)/2
    for j in range(2,N+1,2):    # j is even
#        print "Y[q]",Y[q]
#        print "matrix(cos(pi * j * arange(1,q+) / N))",matrix(cos(pi * j * arange(1,q+1) / N))
#        print "matrix(Y[q + arange(0,q)] + Y[q - arange(0,q)])",matrix(Y[q + arange(1,q+1)] + Y[q - arange(1,q+1)])
        AC[j-1] = 1.0 / N * matrix(Y[q] + matrix(cos(pi * j * arange(1,q+1) / N)) * matrix(Y[q + arange(1,q+1)] + Y[q - arange(1,q+1)]))
    for j in range(1,N+1,2):    # j is odd
        BC[j-1] = 1.0 / N * matrix(sin(pi * j * arange(1,q+1) / N)) * matrix(Y[q + arange(1,q+1)] - Y[q - arange(1,q+1)])

    # Computation of the general variance V in the frequency domain.
    V = 2 * (matrix(AC).T * matrix(AC) + matrix(BC).T * matrix(BC))
    # Computation of the partial variances and sensitivity indices.
    # Si=zeros(inpt,otpt);
    Si = zeros((otpt,otpt,inpt));
    for i in range(inpt):    
        Vi = zeros((otpt, otpt))    
        for j in range(1,MI+1): 
            idx = j * OM[i]-1     
            Vi = Vi + AC[idx].T * AC[idx] + BC[idx].T * BC[idx]
        Vi = 2. * Vi
        Si[:, :, i] = Vi / V

    if lrflag == 1:
        SRC, stat = SRC_regress.SRC_regress(X, Y, otpt, N)

    # ----------------------  Analyze  ---------------------------
    
    Sti = []# appears right after the call to this method in the original PCC_Computation.m
    
#    if plotf == 1:    
#        piecharts(inpt, otpt, Si, Sti, method, output)
    if simple == 1:
        Si_t = zeros((inpt,otpt))
        for p in range(inpt):        
            Si_t[p] = diag(Si[:, :, p])
        Si = Si_t.T

    Results = {'FirstOrderSensitivity': Si}
    if lrflag == 1:
        Results.update({'SRC': SRC, 'R^2': stat})
    return Results

# Selection of a frequency set. Done recursively as described in:
# A computational implementation of FAST [McRae et al.]

def SETFREQ(inpt):
    OMEGA = [0, 3, 1, 5, 11, 1, 17, 23, 19, 25, 41, 31, 23, 87, 67, 73, 85, 143, 149, 99, 119, 237, 267, 283, 151, 385, 157, 215, 449, 163, 337, 253, 375, 441, 673, 773, 875, 873, 587, 849, 623, 637, 891, 943, 1171, 1225, 1335, 1725, 1663, 2019]
    DN = [4, 8, 6, 10, 20, 22, 32, 40, 38, 26, 56, 62, 46, 76, 96, 60, 86, 126, 134, 112, 92, 128, 154, 196, 34, 416, 106, 208, 328, 198, 382, 88, 348, 186, 140, 170, 284, 568, 302, 438, 410, 248, 448, 388, 596, 216, 100, 488, 166, 0]
    OM = zeros(inpt)
    OM[0] = OMEGA[inpt-1]
    for i in range(1,inpt):
        OM[i] = OM[i - 1] + DN[inpt - 1  - i]
    # to use the same frequencies as SimLab...
    if inpt == 2:    
        OM = [5, 9]
    elif inpt == 3:    
        OM = [1, 9, 15]

    return OM

# Copyright (c) 2011.
# Developed with the sponsorship of the Defense Advanced Research Projects Agency (DARPA).

# Permission is hereby granted, free of charge, to any person obtaining a copy of this data, 
# including any software or models in source or binary form, as well as any drawings, 
# specifications, and documentation (collectively "the Data"), 
# to deal in the Data without restriction, including without limitation the rights to 
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Data, 
# and to permit persons to whom the Data is furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Data.

# THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
# IN NO EVENT SHALL THE AUTHORS, SPONSORS, DEVELOPERS, CONTRIBUTORS, 
# OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
# WITH THE DATA OR THE USE OR OTHER DEALINGS IN THE DATA.



