from numpy import *
import params
import pearscdf
import jsonout
from mvncdf import mvstdnormcdf
import estimate_complexity
import itertools  #for fullfact function
from model_calls import run_list

def UP_FFNI(driver):

    # Uses the FFNI method for UP

    # ----------------------  Setup  ---------------------------
    methd = 'FFNI'
    method = 4

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
    numbins = driver.numbins
    nodes   = driver.nodes
    order   = driver.order
    otpt    = len(driver.outputNames)
    output  = driver.outputNames
    p       = driver.p
    plotf   = 0
    r       = driver.r
    simple  = driver.simple
    stvars  = driver.stvars

    node,w = params.params(method, nodes, inpt, stvars)    # Do I need to transpose these matrices?
    #[quadpts] = params(method, nodes, inpt, stvars)

    # ----------------------  Model  ---------------------------

    # Create full factorial experiment from individual nodes and weights
    j = fullfact(nodes)
    pts = shape(j)[0]
    x=zeros((pts,inpt))
    wj=zeros((pts,inpt))
    for y in range(pts):
        for i in range(inpt):
            x[y][i] = node[i][j[y][i]]
            wj[y][i] = w[i][j[y][i]]
    weight = prod(wj, 1)

    if krig == 1:
        load("dmodel")
        G_s = predictor(x, dmodel)
    else:
#        G_s = zeros((pts, otpt))
#        for i in range(pts):
#            print 'Running simulation',i+1,'of',pts
#            G_s[i] = run_model(driver, x[i])
            # G_s[i] = modelica.RunModelica(x[i], modelname, properties)
        G_s = run_list(driver, x)

    G_mean = zeros(otpt)
    G_kurt = zeros(otpt)
    G_skew = zeros(otpt)
    covar_m = zeros((otpt,otpt))
    for k in range(otpt):
        G_mean[k] = sum(weight * G_s[:, k])
    for k in range(otpt):
        for j in range(otpt):
            covar_m[k, j] = sum(weight * (G_s[:, k] - G_mean[k]) * (G_s[:, j] - G_mean[j]))
            covar_m[j, k] = covar_m[k, j]
        G_skew[k] = sum(weight * (G_s[:, k] - G_mean[k]) ** 3) / covar_m[k, k] ** 1.5
        G_kurt[k] = sum(weight * (G_s[:, k] - G_mean[k]) ** 4) / covar_m[k, k] ** 2

    CovarianceMatrix = covar_m.transpose()
    Moments = {'Mean': G_mean, 'Variance': diag(CovarianceMatrix), 'Skewness': G_skew, 'Kurtosis': G_kurt}

    # ----------------------  Analyze  ---------------------------

    # Calculate the PCC for the FFNI method
    if otpt>1:
        PCC = [0]*(otpt+1)
    else:
        PCC = [0]*otpt
    dtype = [0]*otpt
    Inv1 = [0]*otpt
    Inv2 = [0]*otpt
    m1 = [0]*otpt
    m2 = [0]*otpt
    a1 = [0]*otpt
    a2 = [0]*otpt
    alph = [0]*otpt
    beta = [0]*otpt
    lo = [0]*otpt
    hi = [0]*otpt
    C_Y_pdf = [0]*otpt

    if any(Moments['Variance']==0):
        print "Warning: One or more outputs does not vary over given parameter variation."

    for k in range(0,otpt):
        PCC[k],dtype[k],Inv1[k],m1[k],m2[k],a1[k],a2[k],alph[k],beta[k],lo[k],hi[k] = pearscdf.pearscdf(limstate[k], Moments['Mean'][k], sqrt(CovarianceMatrix[k, k]), Moments['Skewness'][k], Moments['Kurtosis'][k], methd, k, output)
        if dtype[k] != None:
            if iscomplex(a1[k]):
                a1[k] = [a1[k].real, a1[k].imag]
            if iscomplex(a2[k]):
                a2[k] = [a2[k].real, a2[k].imag]

            C_Y_pdf[k] = estimate_complexity.with_distribution(dtype[k],limstate[k],Moments['Mean'][k],Moments['Variance'][k],numbins)

    sigma_mat=matrix(sqrt(diag(CovarianceMatrix)))
    seterr(invalid='ignore')    #ignore problems with divide-by-zero, just give us 'nan' as usual
    CorrelationMatrix= CovarianceMatrix/multiply(sigma_mat,sigma_mat.transpose())
    Distribution = {'PearsonType': dtype, 'm1': m1, 'm2': m2, 'a1': a1, 'a2': a2, 'Complexity': C_Y_pdf}
    Plotting = {'alpha': alph, 'beta': beta, 'lo': lo, 'hi': hi}

    CorrelationMatrix=where(isnan(CorrelationMatrix), None, CorrelationMatrix)

    if otpt > 1 and not 0 in PCC[0:otpt]:
        lower = zeros(otpt)-inf
        PCC[otpt] = mvstdnormcdf(lower, Inv1, CorrelationMatrix)

    Results = {'Moments': Moments, 'CorrelationMatrix': CorrelationMatrix,
    'CovarianceMatrix': CovarianceMatrix, 'Distribution': Distribution, 'Plotting': Plotting, 'PCC': PCC}

    return Results

#duplicates matlab fullfact function
def fullfact(levels):
    args = []
    for l in levels:
        args.append(range(0,l))
    ff = itertools.product(*args)
    return array(list(ff))

# if we're supposed to close the plot window autoomatically on
# completion, quit.
# if autoClose == 1

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



