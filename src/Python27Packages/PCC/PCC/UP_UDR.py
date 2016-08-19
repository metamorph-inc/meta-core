from numpy import *
import pearscdf
import params
import jsonout
from mvncdf import mvstdnormcdf
import sys
import estimate_complexity
from model_calls import run_list
import logging

def UP_UDR(driver):
# Uses the UDR method for UP

    methd = 'UDR'
    method = 5

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

    node,weight = params.params(method, nodes, inpt, stvars)

# ----------------------  Model  ---------------------------
    # set_printoptions(precision=4)
    # set_printoptions(suppress=True)

    x = kron(mu, ones((inpt * nodes[0], 1)))
    for ii in range(0,inpt):
        k = ii * nodes[ii]
        l = (ii+1) * nodes[ii]
        x[k:l, ii] = node[ii,:]

#    G_mu = run_model(driver, mu)
    values = [mu]

    G_s = zeros((inpt,max(nodes),otpt))
    for k,inputname in enumerate(driver._json_tree['Configurations']['Configuration']['PCCInputArguments']['StochasticInputs']['InputDistributions']):
        if krig == 1:
            load('dmodel')
            for j in  range(0,nodes[k]):
                var = k * nodes[k] + j
                X = x[var, :]
                G_s[k, j] = predictor(X, dmodel)
        else:
            for j in  range(0,nodes[k]):
                var = k * nodes[k] + j
                X = x[var, :]
#                print 'Running simulation on node',j,'of input',inputname['Name']
#                G_s[k, j] = run_model(driver, X)
                values.append(X)

    out = iter(run_list(driver, values))
    G_mu = out.next()
    for k,inputname in enumerate(driver._json_tree['Configurations']['Configuration']['PCCInputArguments']['StochasticInputs']['InputDistributions']):
        for j in  range(0,nodes[k]):
            G_s[k, j] = out.next()

    G_mean = zeros(otpt)
    G_kurt = zeros(otpt)
    G_skew = zeros(otpt)
    G_sigma = zeros(otpt)
    covar_m = zeros((otpt,otpt))
    gs = zeros(otpt)
    gk = zeros(otpt)
    moms = []
    for l in range(0,otpt):
        moms.append(newmoment(inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l]))
        G_mean[l] = moment(1, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l])
    for l in range(0,otpt):
        moms.append(newmoment(inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l]))
        G_sigma[l] = moment(2, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l])
        gs[l] = moment(3, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l])
        G_skew[l] = moment(3, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l]) / G_sigma[l] ** 1.5
        gk[l] = moment(4, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l])
        G_kurt[l] = moment(4, inpt, nodes[0], weight, G_s[:, :, l], G_mu[l], G_mean[l]) / G_sigma[l] ** 2
        for j in range(l,otpt):
            covar_m[l, j] = moment2(1, inpt, nodes[0], weight, G_s[:, :, l], l, G_s[:, :, j], j, G_mu, G_mean)
            covar_m[j, l] = covar_m[l, j]

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

def binomial(n,k):
    accum = 1
    for m in range(1,k+1):
        accum = accum*(n-k+m)/m
    return accum

def newmoment(NN, n, weight, G_s, G_mu, G_mean):

    sn=[]
    for i in range(0,5):
        sn.append(SN(i, NN-1, n, weight, G_s, G_mean))

    ml = [0]*4
    for l in range(1,5):
        for i in range(0,l+1):
            ml[l-1] += binomial(l, i) * sn[i] * ((-(NN - 1) * (G_mu - G_mean)) ** (l - i))

    return ml

def SN(i, num, n, weight, G_s, G_mean):

    mom = 0
    if num == 0:
        for j in range(0,n):
            mom = mom + weight[num, j] * (G_s[num, j] - G_mean) ** i
    else:
        for k in range(0,i+1):
            for j in range(0,n):
                mom = mom + binomial(i, k) * SN(k, num - 1, n, weight, G_s, G_mean) * weight[num, j] * (G_s[num, j] - G_mean) ** (i - k)
    return mom

def moment(l, NN, n, weight, G_s, G_mu, G_mean):

    ml = 0
    for i in range(0,l+1):
        ml = ml + binomial(l, i) * SN(i, NN-1, n, weight, G_s, G_mean) * ((-(NN - 1) * (G_mu - G_mean)) ** (l - i))

    return ml

def moment2(l, NN, n, weight, G_1, p, G_2, q, G_mu, G_mean):
    ml = 0
    for i in range(0,l+1):
        ml = ml + binomial(l, i) * SN2(i, NN-1, n, weight, G_1, p, G_2, q, G_mean) * ((-(NN - 1) * (G_mu[p] - G_mean[p]))) ** (l - i) * ((-(NN - 1) * (G_mu[q] - G_mean[q]))) ** (l - i)

    return ml

def SN2(i, num, n, weight, G_1, p, G_2, q, G_mean):

    mom = 0
    if num == 0:
        for j in range(0,n):
            mom = mom + weight[num, j] * (G_1[num, j] - G_mean[p]) ** i * (G_2[num, j] - G_mean[q]) ** i
        ret = mom
    else:
        for k in range(0,i+1):
            for j in range(0,n):
                mom = mom + binomial(i, k) * SN2(k, num - 1, n, weight, G_1, p, G_2, q, G_mean) * weight[num, j] * (G_1[num, j] - G_mean[p]) ** (i - k) * (G_2[num, j] - G_mean[q]) ** (i - k)

    return mom

def predictor(x=None, dmodel=None):
#PREDICTOR  Predictor for y(x) using the given DACE model.
#
# Call:   y = predictor(x, dmodel)
#         [y, or] = predictor(x, dmodel)
#         [y, dy, mse] = predictor(x, dmodel)
#         [y, dy, mse, dmse] = predictor(x, dmodel)
#
# Input
# x      : trial design sites with n dimensions.
#          For mx trial sites x:
#          If mx = 1, then both a row and a column vector is accepted,
#          otherwise, x must be an mx*n matrix with the sites stored
#          rowwise.
# dmodel : Struct with DACE model; see DACEFIT
#
# Output
# y    : predicted response at x.
# or   : If mx = 1, then or = gradient vector/Jacobian matrix of predictor
#        otherwise, or is an vector with mx rows containing the estimated
#                   mean squared error of the predictor
# Three or four results are allowed only when mx = 1,
# dy   : Gradient of predictor; column vector with  n elements
# mse  : Estimated mean squared error of the predictor;
# dmse : Gradient vector/Jacobian matrix of mse

# hbn@imm.dtu.dk
# Last update August 26, 2002

    or1 = NaN
    or2 = NaN; print or2
    dmse = NaN
    # Default return values
    if isnan(dmodel.beta):
        y = NaN
        logging.error('DMODEL has not been found')
        raise ValueError,'DMODEL has not been found'

    [m, n] = size(dmodel.S)# number of design sites and number of dimensions
    sx = size(x)# number of trial sites and their dimension
    if min(sx) == logical_and(1, n > 1):    # Single trial point
        nx = max(sx)
        if nx == n:
            mx = 1
            x = x[:].T
    else:
        mx = sx(1)
        nx = sx(2)
    if nx != n:
        logging.error('Dimension of trial sites should be %d', n)
        raise ValueError

    # Normalize trial sites
    x = (x - repmat(dmodel.Ssc[1, :], mx, 1)) / repmat(dmodel.Ssc[2, :], mx, 1)
    q = size(dmodel.Ysc, 2)# number of response functions
    y = zeros((mx, q))# initialize result

    if mx == 1:    # one site only
        dx = repmat(x, m, 1) - dmodel.S    # distances to design sites
        if nargout > 1:        # gradient/Jacobian wanted
            [f, df] = feval(dmodel.regr, x)
            [r, dr] = feval(dmodel.corr, dmodel.theta, dx)
            # Scaled Jacobian
            dy = (df * dmodel.beta).T + dmodel.gamma * dr
            # Unscaled Jacobian
            or1 = dy * repmat(dmodel.Ysc[2, :].cT, 1, nx) / repmat(dmodel.Ssc[2, :], q, 1)
            if q == 1:
                # Gradient as a column vector
                or1 = or1.cT
            if nargout > 2:            # MSE wanted

                rt = dmodel.C; print rt
                r

                u = dmodel.Ft.T * rt - f.T
                v = dmodel.G; print v
                u

                or2 = repmat(dmodel.sigma2, mx, 1) *elmul* repmat((1 + sum(v **elpow** 2) - sum(rt **elpow** 2)).cT, 1, q)

                if nargout > 3:                # gradient/Jacobian of MSE wanted
                    # Scaled gradient as a row vector
                    Gv = dmodel.G.cT; print Gv
                    v

                    -(df * Gv).cT
                    # Unscaled Jacobian
                    dmse = repmat(2 * dmodel.sigma2.cT, 1, nx) * repmat(g /eldiv/ dmodel.Ssc[2, :], q, 1)
                    if q == 1:
                        # Gradient as a column vector
                        dmse = dmse.cT

        else:        # predictor only
            f = feval(dmodel.regr, x)
            r = feval(dmodel.corr, dmodel.theta, dx)

        # Scaled predictor
        sy = f * dmodel.beta + (dmodel.gamma * r).T
        # Predictor
        y = (dmodel.Ysc[1, :] + dmodel.Ysc[2, :] * sy).cT

    else:    # several trial sites
        # Get distances to design sites
        dx = zeros(mx * m, n)
        kk = mslice[1:m]; print kk

        for k in mslice[1:mx]:
            dx[kk, :] = repmat(x[k, :], m, 1) - dmodel.S
            kk = kk + m
        # Get regression function and correlation
        f = feval(dmodel.regr, x)
        r = feval(dmodel.corr, dmodel.theta, dx)
        r = reshape(r, m, mx)

        # Scaled predictor
        sy = f * dmodel.beta + (dmodel.gamma * r).T
        # Predictor
        y = repmat(dmodel.Ysc[1, :], mx, 1) + repmat(dmodel.Ysc[2, :], mx, 1) * sy

        if nargout > 1:        # MSE wanted
            rt = dmodel.C; print rt
            r

            u = dmodel.G(dmodel.Ft.T * rt - f.T)
            or1 = repmat(dmodel.sigma2, mx, 1) *elmul* repmat((1 + colsum(u **elpow** 2) - colsum(rt **elpow** 2)).cT, 1, q)
            if nargout > 2:
                disp('WARNING from PREDICTOR.  Only  y  and  or1=mse  are computed')
    #end of several sites

    return y,or1,or2,dmse

# >>>>>>>>>>>>>>>>   Auxiliary function  ====================
def colsum(x=None):
    # Columnwise sum of elements in  x
    if size(x, 1) == 1:
        s = x; print s
    else:
        s = sum(x); print s
    return s

# Copyright (c) 2011, Christopher Hoyle.
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


