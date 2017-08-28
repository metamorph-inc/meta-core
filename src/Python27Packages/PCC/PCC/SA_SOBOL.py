from __future__ import division # tells python to use "true" division throughout
from numpy import *
from scipy.stats import beta,norm,lognorm,uniform
import SRC_regress
import LHS
from model_calls import run_list
from copy import copy, deepcopy

def SA_SOBOL(problem, driver):
    # Uses the Sobel Method for SA.
    # Input:
    # inpt : no. of input factors
    # N: number of Sobel samples
    #
    # Output:
    # SI[] : sensitivity indices
    # STI[] : total effect sensitivity indices
    # Other used variables/constants:
    # V : total variance
    # VI : partial variances

    # ----------------------  Setup  ---------------------------

    methd = 'SOBOL'
    method = '7'

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
    value = asarray(LHS.LHS(2*inpt, nSOBOL))

    for j in range(inpt):
        if stvars[j].dist == 'NORM':
            value[:,j] = norm.ppf(uniform.cdf(value[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])
            value[:,j+inpt] = norm.ppf(uniform.cdf(value[:,j+inpt], 0, 1), stvars[j].param[0], stvars[j].param[1])
        elif stvars[j].dist == 'LNORM':
            value[:,j] = lognorm.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
            value[:,j+inpt] = lognorm.ppf(uniform.cdf(value[:, j+inpt], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
        elif stvars[j].dist == 'BETA':
            value[:,j] = beta.ppf(uniform.cdf(value[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
            value[:,j+inpt] = beta.ppf(uniform.cdf(value[:, j+inpt], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
        elif stvars[j].dist == 'UNIF':
            value[:,j] = uniform.ppf(uniform.cdf(value[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])
            value[:,j+inpt] = uniform.ppf(uniform.cdf(value[:,j+inpt], 0, 1), stvars[j].param[0], stvars[j].param[1])

    values = []
    XMA = value[0:nSOBOL, 0:inpt]
    XMB = value[0:nSOBOL, inpt:2 * inpt]
    YXMA = zeros((nSOBOL, otpt))
    YXMB = zeros((nSOBOL, otpt))
    if krig == 1:
        load("dmodel")
        YXMA = predictor(XMA, dmodel)
        YXMB = predictor(XMB, dmodel)
    else:
        values.extend(list(XMA))
        values.extend(list(XMB))

    YXMC = zeros((inpt, nSOBOL, otpt))
    for i in range(inpt):
        XMC = deepcopy(XMB)
        XMC[:, i] = deepcopy(XMA[:, i])
        if krig == 1:
            YXMC[i] = predictor(XMC, dmodel)
        else:
            values.extend(list(XMC))

    if krig != 1:
        out = iter(run_list(problem, driver, values))
        for i in range(nSOBOL):
            YXMA[i] = out.next()
        for i in range(nSOBOL):
            YXMB[i] = out.next()
        for i in range(inpt):
            for j in range(nSOBOL):
                YXMC[i, j] = out.next()

    f0 = mean(YXMA,0)
    if otpt==1:
        V = cov(YXMA,None,0,1)
    else:  #multiple outputs
        V = diag(cov(YXMA,None,0,1))
    Vi = zeros((otpt, inpt))
    Vci = zeros((otpt, inpt))
    for i in range(inpt):
        for p in range(otpt):
            Vi[p,i] = 1.0/nSOBOL*sum(YXMA[:,p]*YXMC[i,:,p])-f0[p]**2;
            Vci[p,i]= 1.0/nSOBOL*sum(YXMB[:,p]*YXMC[i,:,p])-f0[p]**2;

    Si = zeros((otpt,inpt));
    Sti = zeros((otpt,inpt));
    for j in range(inpt):
        Si[:, j] = Vi[:, j] / V
        Sti[:, j] = 1 - Vci[:, j] / V

    if lrflag == 1:
        SRC, stat = SRC_regress.SRC_regress(XMA, YXMA, otpt, nSOBOL)

# ----------------------  Analyze  ---------------------------

    Results = {'FirstOrderSensitivity': Si, 'TotalEffectSensitivity': Sti}
    if lrflag == 1:
        Results.update({'SRC': SRC, 'R^2': stat})
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



