from numpy import *
from scipy.stats import beta,norm,lognorm,uniform

def Dist(stvars, value, inpt):
    v = zeros(inpt)
    for j in range(inpt):
        if stvars[j].dist == 'NORM':
            v[j] = norm.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[0], stvars[j].param[1])
        elif stvars[j].dist == 'LNORM':        
            v[j] = lognorm.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
        elif stvars[j].dist == 'BETA':        
            v[j] = beta.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
        elif stvars[j].dist == 'UNIF':        
            v[j] = uniform.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[0], stvars[j].param[1])

    return v
