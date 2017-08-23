from __future__ import division
from numpy import *
import LHS
from scipy.stats import beta, norm, lognorm, uniform, skew, kurtosis
import estimate_complexity
from model_calls import run_list


def UP_MCS(problem, driver):
    # Uses the MCS method for UP

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

    #*****************RANDOM DRAWS FROM INPUT DISTRIBUTIONS********************
    value = asarray(LHS.LHS(inpt, nMCS))

    for j in range(inpt):
        if stvars[j].dist == 'NORM':
            value[:, j] = norm.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1])
        elif stvars[j].dist == 'LNORM':
            value[:, j] = lognorm.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
        elif stvars[j].dist == 'BETA':
            value[:, j] = beta.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
        elif stvars[j].dist == 'UNIF':
            value[:, j] = uniform.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1])

    # ----------------------  Model  ---------------------------

    out = zeros((nMCS, otpt))
    if krig == 1:
        load("dmodel")
        out = predictor(value, dmodel)
    else:
#        for i in range(nMCS):
#            print 'Running simulation', i+1, 'of', nMCS, 'with inputs', value[i]
#            out[i] = run_model(driver, value[i])
        out = run_list(problem, driver, value)

    limstate = asarray(limstate)
    limstate1 = asarray(kron(limstate[:, 0], ones(nMCS))).reshape(otpt, nMCS).transpose()
    limstate2 = asarray(kron(limstate[:, 1], ones(nMCS))).reshape(otpt, nMCS).transpose()
    B = logical_and(greater_equal(out, limstate1), less_equal(out, limstate2))
    PCC = sum(B, 0) / nMCS
    B_t = B[sum(B, 1) == otpt]
    if otpt > 1 and not 0 in PCC[0:otpt]:
        PCC = append(PCC, len(B_t) / nMCS)

    #Moments
    CovarianceMatrix = matrix(cov(out, None, 0))#.transpose()
    Moments = {'Mean': mean(out, 0), 'Variance': diag(CovarianceMatrix), 'Skewness': skew(out), 'Kurtosis': kurtosis(out, fisher=False)}

    # combine the display of the correlation matrix with setting a var that will be needed below
    sigma_mat=matrix(sqrt(diag(CovarianceMatrix)))
    CorrelationMatrix= CovarianceMatrix/multiply(sigma_mat, sigma_mat.transpose())

    # ----------------------  Analyze  ---------------------------

    if any(Moments['Variance'] == 0):
        print "Warning: One or more outputs does not vary over given parameter variation."

    C_Y = [0]*otpt
    for k in range(0, otpt):
        if Moments['Variance'][k] != 0:
            C_Y[k] = estimate_complexity.with_samples(out[:, k], nMCS)

    sigma_mat = matrix(sqrt(diag(CovarianceMatrix)))
    seterr(invalid='ignore')    # ignore problems with divide-by-zero, just give us 'nan' as usual
    CorrelationMatrix = CovarianceMatrix/multiply(sigma_mat, sigma_mat.transpose())

    Distribution = {'Complexity': C_Y}

    CorrelationMatrix = where(isnan(CorrelationMatrix), None, CorrelationMatrix)

    Results = {'Moments': Moments, 'CorrelationMatrix': CorrelationMatrix,
    'CovarianceMatrix': CovarianceMatrix, 'Distribution': Distribution, 'PCC': PCC}

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
