from numpy import *
# import loadargs
import Hasofer
import Dist
from scipy.stats import norm
from mvncdf import mvstdnormcdf
from model_calls import run_list

def UP_MPP(problem, driver):
    # Uses the MPP method for UP    
    
    # This routine has been updated as part of refactoring code before the port
    # from MATLAB to Python/NumPy/SciPy.  Sections of PCC_Computation that apply
    # this method have been moved here.
    
    # ----------------------  Setup  ---------------------------
    methd = 'MPP'
    method = '3'
    
    delta   = driver.MPPdelta
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
    
    # ----------------------  Model  ---------------------------

    G_temp = lambda x: run_list(problem, driver, x)
    Tinv = lambda x: Dist.Dist(stvars, x, inpt)
   
    #USING THE HASOFER-LIND ALGORITHM  
    alpha = zeros((otpt, inpt))
    Xl = zeros((otpt, inpt))
    Xu = zeros((otpt, inpt))
    beta1 = zeros(otpt)
    beta2 = zeros(otpt)
    if otpt>1:
        PCC = [0]*(otpt+1)
    else:
        PCC = [0]*otpt
    for k in range(otpt):
        print 'Testing output',k+1,'of',otpt    
        if krig == 1:        
            load("dmodel")        
            if limstate[k][1] == inf:            
                cdist = 1            
                G = lambda x: limstate[k][0]- predictor(x, dmodel)            
            elif limstate[k][0] == -inf:            
                cdist = 2            
                G = lambda x: predictor(x, dmodel) - limstate[k][1]            
            else:            
                cdist = 3            
                G1 = lambda x: limstate[k][0] - predictor(x, dmodel)
                G2 = lambda x: predictor(x, dmodel) - limstate[k][2]
        else:
            if limstate[k][1] == inf:            
                cdist = 1            
                G = lambda x: limstate[k][0] - G_temp(x)            
            elif limstate[k][0] == -inf:            
                cdist = 2            
                G = lambda x: G_temp(x) - limstate[k][1]           
            else:            
                cdist = 3            
                G1 = lambda x: limstate[k][0] - G_temp(x)            
                G2 = lambda x: G_temp(x) - limstate[k][1]

        I_sigma=ones((inpt));
        if cdist == 1 or cdist == 2:        
            u = zeros((inpt))        
            diff = 1.0
            while diff > .005:
                uk, alp = Hasofer.Hasofer(G, u, Tinv, k, delta, I_sigma, inpt, otpt)
                diff = abs(linalg.norm(uk) - linalg.norm(u))            
                u = uk
                print 'X =', Tinv(u)
            beta1[k] = linalg.norm(u)        
            beta2[k] = inf
            if cdist == 1:            
                Xl[k] = Tinv(u)            
                Xu[k] = ones((inpt)) * inf            
                alpha[k] = alp            
            else:            
                Xl[k] = ones((inpt)) * -inf            
                Xu[k] = Tinv(u)            
                alpha[k] = -alp
            PCC[k] = norm.cdf(beta1[k])
            alpha[k] = alp        
        else:        
            u = zeros((inpt))        
            diff = 1.0
            try:
                while diff > .005:
                    uk, alp = Hasofer.Hasofer(G1, u, Tinv, k, delta, I_sigma, inpt, otpt)        
                    diff = abs(linalg.norm(uk) - linalg.norm(u))
                    u = uk
                    print 'X =', Tinv(u)
                beta1[k] = linalg.norm(u)        
                Xl[k] = Tinv(u)
            except ValueError:
                beta1[k] = inf        
                Xl[k] = nan
            u = zeros((inpt))        
            diff = 1.0
            try:        
                while diff > .005:
                    uk, alp = Hasofer.Hasofer(G2, u, Tinv, k, delta, I_sigma, inpt, otpt)            
                    diff = abs(linalg.norm(uk) - linalg.norm(u))            
                    u = uk
                    print 'X =', Tinv(u)
                beta2[k] = linalg.norm(u) 
                Xu[k] = Tinv(u)
                alpha[k] = -alp
            except ValueError:
                beta2[k] = inf  
                Xu[k] = nan
                alpha[k] = nan 
            PCC[k] = norm.cdf(beta2[k]) - norm.cdf(-beta1[k])
    
    corr_mat = ones((otpt,otpt))
    for j in range(otpt):    
        for k in range(j,otpt):  
            corr_mat[j, k] = matrix(alpha[j]) * matrix(alpha[k]).transpose()        
            corr_mat[k, j] = corr_mat[j, k]   
     
    # ----------------------  Analyze  ---------------------------
    
    # There's not much to analyze; that already happened.  Just print out the results

    if otpt > 1 and not 0 in PCC[0:otpt]: 
        PCC[otpt] = mvstdnormcdf(-beta1, beta2, corr_mat)  

    # requested feature: replace all "NaNs" with "None"
    Xu=where(isnan(Xu), None, Xu)
    Xl=where(isnan(Xl), None, Xl)
    corr_mat=where(isnan(corr_mat), None, corr_mat)
    
    Results = {'MPPUpperBound':Xu ,'MPPLowerBound':Xl,'CorrelationMatrix': corr_mat, 'PCC': PCC}
    
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
    


