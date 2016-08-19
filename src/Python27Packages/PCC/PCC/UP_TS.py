from numpy import *
from mvncdf import mvstdnormcdf
import pearscdf
import estimate_complexity
import taylorseries
from model_calls import run_list

def UP_TS(driver):
    # Uses the TS method for UP

    # ----------------------  Setup  ---------------------------

    methd = 'TS'
    method = '2'

    delta   = driver.TSdelta
    mu      = [inp.get_I_mu() for inp in driver.inputs]
    sigma   = [inp.get_I_sigma() for inp in driver.inputs]
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

    rho = identity(inpt)

    # ----------------------  Model  ---------------------------

    if krig == 1:
        load("dmodel")
        G_mean = predictor(mu, dmodel).cT
        G_k = lambda x: predictor(x, dmodel)
        F1 = taylorseries.taylorseries(G_k, mu, delta*sigma, inpt, otpt)
    else:
#        G_mean = run_model(driver, mu)
#        G = lambda x: run_model(driver, x)
        values = [mu]
        values.extend(taylorseries.pretaylorseries(mu, delta*array(sigma), inpt))
        out = iter(run_list(driver, values))
        G_mean = out.next()
        G = lambda x: out.next()
        F1 = taylorseries.taylorseries(G, mu, delta*array(sigma), inpt, otpt)

    print 'Taylor Series:\n',F1

    covar_m = zeros((otpt, otpt))
    for j in range(otpt):
        for k in range(j,otpt):
            for l in range(inpt):
                for m in range(inpt):
                    covar_m[j, k] = covar_m[j, k] + F1[l, j] * F1[m, k] * sigma[l] * sigma[m] * rho[l, m]
                covar_m[k, j] = covar_m[j, k]

    CovarianceMatrix = covar_m.transpose()
    print 'Covariance Matrix:\n',CovarianceMatrix
    Moments = {'Mean': G_mean, 'Variance': diag(CovarianceMatrix), 'Skewness': zeros((otpt, 1)), 'Kurtosis': 3 * ones((otpt, 1))}

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

    # ----------------------  Analyze  ---------------------------

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
