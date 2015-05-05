from numpy import *
import pearscdf
from mvncdf import mvstdnormcdf
import estimate_complexity
import Dist
import gaussquad
import itertools  #for fullfact function
import scipy #for misc.factorial function
import xvector
import hermite
from sympy import N, symbols
from model_calls import run_list

def UP_PCE(driver):
    # Uses the PCE method for UP
    
    # This routine has been updated as part of refactoring code before the port
    # from MATLAB to Python/NumPy/SciPy.  Sections of PCC_Computation that apply
    # this method have been moved here.
    
    # ----------------------  Setup  ---------------------------
    methd = 'PCE'
    method = 6

    inpt    = len(driver.inputs)
    krig    = driver.krig
    limstate= driver.limstate
    nodes   = driver.nodes
    order   = driver.order
    otpt    = len(driver.outputNames)
    output  = driver.outputNames
    stvars  = driver.stvars
    numbins = driver.numbins
    
    #current settings for these two vars
    ii = 0
    jj = 0
    
    # ----------------------  Model  ---------------------------
    mu_g = zeros(inpt)
    sigma_g = ones(inpt)
    node_t = zeros((inpt,nodes[0]))
    weight_t = zeros((inpt,nodes[0]))
    for i in range(inpt):
        node_t[i], weight_t[i] = gaussquad.gaussquad(nodes[i], 'NORM', mu_g[i], sigma_g[i])

    x=[]
    for i in range(inpt):
        x.append(symbols('x'+str(i)))
    x=array(x)

    j=fullfact(nodes);

    pts = shape(j)[0]
    node=zeros((pts,inpt))
    wj=zeros((pts,inpt))
    for y in range(pts):
        for i in range(inpt):
            node[y][i] = node_t[i][j[y][i]]
            wj[y][i] = weight_t[i][j[y][i]]
    weight=prod(wj,1);
    
    P = zeros(order)
    P[0] = 1
    for p in range(1,order):    
        term2 = 0
        for s in range(1,p+1):
            term1 = 1        
            for r in range(s):
                term1 = term1 * (inpt + r)
            term2 = term2 + (1.0 / int(scipy.misc.factorial(s))) * term1
        if p == 1:        
            P[p] = term2        
        else:        
            P[p] = term2 - sum(P[range(1,p+1)])
    
    G_s = zeros((pts, otpt))
    if krig == 1:    
        t = strcat('SS_K', num2str(ii), num2str(jj))    
        load(char(t))    
        for j in range(pts):       
            #Rosenblatt Transformation
            T_L = Dist.Dist(stvars, node[j], inpt)       
            G_s[j] = predictor(T_L, dmodel)
    else:
        values = []    
        for j in range(pts):     
            #Rosenblatt Transformation
#            print 'Running simulation',j+1,'of',pts
            T_L = Dist.Dist(stvars, node[j], inpt)         
#            G_s[j] = run_model(driver, T_L)
            values.append(T_L)
        G_s = run_list(driver, values)

    indx = 0
    bn = zeros((sum(P), otpt))
    bd = zeros(sum(P))
    for k in range(order):    
        vec = xvector.xvector(k, inpt)
        for j in range(int(P[k])):   
            for i in range(pts):
                L=node[i]       
                if k == 0:
                    bn[indx] = bn[indx] + weight[i] * G_s[i]
                    bd[indx] = bd[indx] + weight[i]
                else:                

                    h, h_sym = hermite.hermite(k, vec[j], L, x)
                    bn[indx] += weight[i] * G_s[i] * h          
                    bd[indx] += weight[i] * (h ** 2)
            indx+=1

    b = zeros((sum(P),otpt))
    for l in range(otpt):    
        b[:, l] = bn[:, l] / bd
    
    indx = 0
    U_sum = 0
    for k in range(order):    
        vec = xvector.xvector(k, inpt)
        for j in range(int(P[k])):        
            if k == 0:            
                U_sum = b[0]
            else:       
                h, h_sym = hermite.hermite(k, vec[j], L, x)
                U_sum = U_sum + b[indx] * N(h_sym)
            indx+=1
    U = U_sum
    
    U_s = zeros((pts,otpt))
    G_mean = zeros(otpt)
    G_kurt = zeros(otpt)
    G_skew = zeros(otpt)
    covar_m = zeros((otpt,otpt))
    for i in range(pts):
        for k in range(otpt):
            U_s[i][k] = U[k].subs(dict(zip(x, node[i])))
    for k in range(otpt):
#        G_mean[k] = sum(matrix(weight) * matrix(U_s[:, k]).transpose())
        G_mean[k] = sum(weight * U_s[:, k])
    for k in range(otpt):    
        for j in range(k,otpt):    
            covar_m[k, j] = sum(weight * (U_s[:, k] - G_mean[k]) * (G_s[:, j] - G_mean[j]))        
            covar_m[j, k] = covar_m[k, j]
        G_skew[k] = sum(weight * (U_s[:, k] - G_mean[k]) ** 3) / covar_m[k, k] ** 1.5    
        G_kurt[k] = sum(weight * (U_s[:, k] - G_mean[k]) ** 4) / covar_m[k, k] ** 2
    
    CovarianceMatrix = covar_m.transpose()
    Moments = {'Mean': G_mean, 'Variance': diag(CovarianceMatrix), 'Skewness': G_skew, 'Kurtosis': G_kurt}
    
    # ----------------------  Analyze  ---------------------------

    if any(Moments['Variance']==0):
        print "Warning: One or more outputs does not vary over given parameter variation."

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

    for k in range(otpt):
        PCC[k],dtype[k],Inv1[k],m1[k],m2[k],a1[k],a2[k],alph[k],beta[k],lo[k],hi[k] =\
          pearscdf.pearscdf(limstate[k], Moments['Mean'][k], sqrt(CovarianceMatrix[k, k]), Moments['Skewness'][k], Moments['Kurtosis'][k], methd, k, output)
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



