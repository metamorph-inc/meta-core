## Chelsea He and Emily Clements, MIT
## estimate_complexity.py
## June 26, 2012
##
## This function estimates the complexity of a random variable Z
## based on either the pdf of Z or samples of Z.  The complexity 
## metric used is exponential entropy as defined by Campbell (1966)

# Import libraries
from numpy import *
import scipy.stats.kde as kde
from scipy.stats import norm

def with_distribution(dist,limits,mean,variance,numbins):

    if limits[0] > -inf:
        lb = limits[0]-3*math.sqrt(variance)
    else:
        lb = mean-5*math.sqrt(variance)
    if limits[1] < inf:
        ub = limits[1]+3*math.sqrt(variance)
    else:
        ub = mean+5*math.sqrt(variance)

    bins = linspace(lb,ub,numbins)

    # Generate Gaussian pdf
    f_z = norm.pdf(bins, mean, math.sqrt(variance))

    # Estimate complexity based on pdf
    return with_pdf(bins,f_z)


############### Method I: supply pdf ############### 
def with_pdf(bins,f_z):

    # Compute bin size
    binsize = bins[1]-bins[0]

    # Initialize entropy value with log(binsize) -- correction term for discretizing pdf
    entsum = log(binsize)

    # Compute differential entropy and complexity     
    for fz in f_z:

        # Consider only terms where f_z > 0 (otherwise log(f_z) --> log(0) will cause trouble)
        if fz*binsize > 1e-320:
            entsum = entsum - fz*binsize*log(fz*binsize) 
            
    entropy =  entsum   
    complexity = exp(entropy)
    return complexity

####### Method II: supply Monte Carlo samples #######
def with_samples(Z,numbins):

    # Turn list into array
    z = array(Z)
    
    # Density estimation, discretized into bins
    bins = linspace(min(z),max(z),numbins)
    binsize = bins[1]-bins[0]
    f_z = kde.gaussian_kde(z).evaluate(bins) 

    # Initialize entropy value with log(binsize) -- correction term for discretizing pdf
    entsum = log(binsize)

    # Compute differential entropy and complexity 
    for fz in f_z:
        
        # Consider only terms where f_z > 0 (otherwise log(f_z) --> log(0) will cause trouble)
        if fz > 0:
            entsum = entsum - fz*binsize*log(fz*binsize)
            
    entropy =  entsum 
    complexity = exp(entropy)
    return complexity
