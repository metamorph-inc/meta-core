from __future__ import division
from numpy import *
from scipy import stats
from scipy.stats import norm,f,gamma,t
import cmath
import math
from scipy.special import gammaln,betainc
from mpmath import hyp2f1

def pearscdf(X, mu, sigma, skew, kurt, method, k, output):

# pearspdf
#   [p,type,coefs] = pearspdf(X,mu,sigma,skew,kurt)
#
#   Returns the probability distribution denisty of the pearsons distribution
#   with mean `mu`, standard deviation `sigma`, skewness `skew` and
#   kurtosis `kurt`, evaluated at the values in X.
#
#   Some combinations of moments are not valid for any random variable, and in
#   particular, the kurtosis must be greater than the square of the skewness
#   plus 1.  The kurtosis of the normal distribution is defined to be 3.
#
#   The seven distribution types in the Pearson system correspond to the
#   following distributions:
#
#      Type 0: Normal distribution
#      Type 1: Four-parameter beta
#      Type 2: Symmetric four-parameter beta
#      Type 3: Three-parameter gamma
#      Type 4: Not related to any standard distribution.  Density proportional
#              to (1+((x-a)/b)^2)^(-c) * exp(-d*arctan((x-a)/b)).
#      Type 5: Inverse gamma location-scale
#      Type 6: F location-scale
#      Type 7: Student's t location-scale
#
#   Examples
#
#   See also
#       pearspdf pearsrnd mean std skewness kurtosis
#


#   References:
#      [1] Johnson, N.L., S. Kotz, and N. Balakrishnan (1994) Continuous
#          Univariate Distributions, Volume 1,  Wiley-Interscience.
#      [2] Devroye, L. (1986) Non-Uniform Random Variate Generation,
#          Springer-Verlag.

    otpt = len(output)
#    outClass = superiorfloat(mu, sigma, skew, kurt)

    if X[1] == inf:
        cdist = 1
        limstate = X[0]
    elif X[0] == -inf:
        cdist = 2
        limstate = X[1]
    else:
        cdist = 3
        limstate = X

    if sigma == 0:
        print "Warning: The standard deviation of output distribution",k,"is zero. No distribution or correlation can be calculated for it."
        if mu>=X[0] and mu<=X[1]:   #mean is in the limits
            return 1, None, inf, None, None, None, None, None, None, None, None
        else:                       #mean is outside the limits
            return 0, None, inf, None, None, None, None, None, None, None, None

    X = (X - mu) / sigma    # Z-score

    if method == 'MCS':
        beta1 = 0
        beta2 = 3
        beta3 = sigma ** 2
    else:
        beta1 = skew ** 2
        beta2 = kurt
        beta3 = sigma ** 2

    # Return NaN for illegal parameter values.
    if (sigma < 0) or (beta2 <= beta1 + 1):
        p = zeros(otpt)+nan
        #p = zeros(sizeout)+nan
        dtype = NaN
        coefs = zeros((1,3))+nan
        print 'Illegal parameter values passed to pearscdf! (sigma:',sigma,'  beta1:',beta1,'  beta2:', beta2,')'
        return

    #% Classify the distribution and find the roots of c0 + c1*x + c2*x^2
    c0 = (4 * beta2 - 3 * beta1)# ./ (10*beta2 - 12*beta1 - 18);
    c1 = skew * (beta2 + 3)# ./ (10*beta2 - 12*beta1 - 18);
    c2 = (2 * beta2 - 3 * beta1 - 6)# ./ (10*beta2 - 12*beta1 - 18);

    if c1 == 0:    # symmetric dist'ns
        if beta2 == 3:
            dtype = 0
            a1 = 0
            a2 = 0
        else:
            if beta2 < 3:
                dtype = 2
            elif beta2 > 3:
                dtype = 7
            a1 = -sqrt(abs(c0 / c2))
            a2 = -a1        # symmetric roots
    elif c2 == 0:    # kurt = 3 + 1.5*skew^2
        dtype = 3
        a1 = -c0 / c1    # single root
        a2 = a1
    else:
        kappa = c1 ** 2 / (4 * c0 * c2)
        if kappa < 0:
            dtype = 1
        elif kappa < 1 - finfo(float64).eps:
            dtype = 4
        elif kappa <= 1 + finfo(float64).eps:
            dtype = 5
        else:
            dtype = 6
        # Solve the quadratic for general roots a1 and a2 and sort by their real parts
        csq=c1 ** 2 - 4 * c0 * c2
        if c1 ** 2 - 4 * c0 * c2 < 0:
            tmp = -(c1 + sign(c1) * cmath.sqrt(c1 ** 2 - 4 * c0 * c2)) / 2
        else:
            tmp = -(c1 + sign(c1) * sqrt(c1 ** 2 - 4 * c0 * c2)) / 2
        a1 = tmp / c2
        a2 = c0 / tmp
        if (real(a1) > real(a2)):
            tmp = a1;
            a1 = a2;
            a2 = tmp;

    denom = (10 * beta2 - 12 * beta1 - 18)

    if abs(denom) > sqrt(finfo(double).tiny):
        c0 = c0 / denom
        c1 = c1 / denom
        c2 = c2 / denom
        coefs = [c0, c1, c2]
    else:
        dtype = 1    # this should have happened already anyway
        # beta2 = 1.8 + 1.2*beta1, and c0, c1, and c2 -> Inf.  But a1 and a2 are
        # still finite.
        coefs = zeroes((1,3))+inf

    if method == 'MCS':
        dtype = 8

    #% Generate standard (zero mean, unit variance) values
    if dtype == 0:
        # normal: standard support (-Inf,Inf)
        #     m1 = zeros(outClass);
        #     m2 = ones(outClass);
        m1 = 0
        m2 = 1
        p = norm.cdf(X[1], m1, m2) - norm.cdf(X[0], m1, m2)
        lo= norm.ppf( 3.39767E-06, mu,sigma ); 
        hi= norm.ppf( 0.999996602, mu,sigma );
        Inv1 = norm.ppf(p, 0, 1)
        #     Inv1=norm.ppf( normcdf(X[0],m1,m2), 0,1 );
        #Inv2 = norm.ppf(normcdf(X[1], m1, m2), 0, 1)

    elif dtype == 1:
        # four-parameter beta: standard support (a1,a2)
        if abs(denom) > sqrt(finfo(double).tiny):
            m1 = (c1 + a1) / (c2 * (a2 - a1))
            m2 = -(c1 + a2) / (c2 * (a2 - a1))
        else:
            # c1 and c2 -> Inf, but c1/c2 has finite limit
            m1 = c1 / (c2 * (a2 - a1))
            m2 = -c1 / (c2 * (a2 - a1))
        #     r = a1 + (a2 - a1) .* betarnd(m1+1,m2+1,sizeOut);
        X = (X - a1) / (a2 - a1)    # Transform to 0-1 interval
        #     lambda = -(a2-a1)*(m1+1)./(m1+m1+2)-a1;
        #     X = (X - lambda - a1)./(a2-a1);

        alph=m1+1
        beta=m2+1
        if alph < 1.001 and beta < 1.001:
            alph=1.001
            beta=1.001

        mode=(alph-1)/(alph+beta-2)

        if mode < 0.1: 
            if alph > beta:
                alph = max(2.0,alph)
                beta = (alph-1)/0.9 - alph + 2
            elif beta > alph:
                beta = max(2.0,beta)
                alph = (0.1*(beta -2) +1)/(1 - 0.1)
        elif mode > 0.9:
            if alph > beta:
                alph = max(2.0,alph)
                beta =(alph-1)/0.9 - alph + 2
            elif beta > alph:
                beta = max(2.0,beta);
                alph = (0.1*(beta -2) +1)/(1 - 0.1)

        p = stats.beta.cdf(X[1], alph, beta) - stats.beta.cdf(X[0], alph, beta)
        lo=a1*sigma+mu;
        hi=a2*sigma+mu;
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( beta.cdf(X[0],m1+1,m2+1), 0,1 );
        #Inv2 = norm.ppf(beta.cdf(X[1], m1 + 1, m2 + 1), 0, 1)

        #     X = X*(a2-a1) + a1;         % Undo interval tranformation
        #     r = r + (0 - a1 - (a2-a1).*(m1+1)./(m1+m2+2));
    elif dtype == 2:
        # symmetric four-parameter beta: standard support (-a1,a1)
        m = (c1 + a1) / (c2 * 2 * abs(a1))
        m1 = m
        m2 = m
        X = (X - a1) / (2 * abs(a1))
        #     r = a1 + 2*abs(a1) .* betapdf(X,m+1,m+1);

        alph=m+1;
        beta=m+1;
        if alph < 1.01: 
            alph=1.01
            beta=1.01

        p = stats.beta.cdf(X[1], alph, beta) - stats.beta.cdf(X[0], alph, beta)
        lo=a1*sigma+mu;
        hi=a2*sigma+mu;
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( beta.cdf(X[0],m+1,m+1), 0,1 );
        #Inv2 = norm.ppf(beta.cdf(X[1], m + 1, m + 1), 0, 1)

        #     X = a1 + 2*abs(a1).*X;
    elif dtype == 3:
        # three-parameter gamma: standard support (a1,Inf) or (-Inf,a1)
        m = (c0 / c1 - c1) / c1
        m1 = m
        m2 = m
        X = (X - a1) / c1
        #     r = c1 .* gampdf(X,m+1,1,sizeOut) + a1;

        p = gamma.cdf(X[1], m + 1, 1) - gamma.cdf(X[0], m + 1, 1)
        lo=(gamma.ppf( 3.39767E-06, m+1, scale=1 )*c1+a1)*sigma+mu; 
        hi=(gamma.ppf( 0.999996602, m+1, scale=1 )*c1+a1)*sigma+mu;
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( gamcdf(X[0],m+1,1), 0,1 );
        #Inv2 = norm.ppf(gamcdf(X[1], m + 1, 1), 0, 1)

        #     X = c1 .* X + a1;
    elif dtype == 4:
        # Pearson IV is not a transformation of a standard distribution: density
        # proportional to (1+((x-lambda)/a)^2)^(-m) * exp(-nu*arctan((x-lambda)/a)),
        # standard support (-Inf,Inf)
        X = X * sigma + mu
        r = 6 * (beta2 - beta1 - 1) / (2 * beta2 - 3 * beta1 - 6)
        m = 1 + r / 2
        nu = -r * (r - 2) * skew / sqrt(16 * (r - 1) - beta1 * (r - 2) ** 2)
        a = sqrt(beta3 * (16 * (r - 1) - beta1 * (r - 2) ** 2)) / 4
        _lambda = mu - ((r - 2) * skew * sigma) / 4    # gives zero mean
        m1 = m
        m2 = nu
        #     X = (X - lambda)./a;
        if cdist == 1:
            p = 1 - pearson4cdf(X[0], m, nu, a, _lambda, mu, sigma)
        elif cdist == 2:
            p = pearson4cdf(X[1], m, nu, a, _lambda, mu, sigma)
        elif cdist == 3:
            p = pearson4cdf(X[1], m, nu, a, _lambda, mu, sigma) - pearson4cdf(X[0], m, nu, a, _lambda, mu, sigma)
        lo=norm.ppf( 3.39767E-06, mu,sigma );   
        hi=norm.ppf( 0.999996602, mu,sigma );
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( pearson4cdf(X[0],m,nu,a,lambda,mu,sigma), 0,1 );
        #Inv2 = norm.ppf(pearson4cdf(X[1], m, nu, a, _lambda, mu, sigma), 0, 1)

        #     C = X.*a + lambda;
        #     C = diff(C);
        #     C= C(1);
        #     p = p./(sum(p)*C);
    elif dtype == 5:
        # inverse gamma location-scale: standard support (-C1,Inf) or
        # (-Inf,-C1)
        C1 = c1 / (2 * c2)
        #     r = -((c1 - C1) ./ c2) ./ gampdf(X,1./c2 - 1,1) - C1;
        X = -((c1 - C1) / c2) / (X + C1)
        m1 = c2
        m2 = 0
        p = gamma.cdf(X[1], 1. / c2 - 1, scale=1) - gamma.cdf(X[0], 1. / c2 - 1, scale=1)
        lo=(-((c1-C1)/c2)/gamma.ppf( 3.39767E-06, 1/c2 - 1, scale=1 )-C1)*sigma+mu; 
        hi=(-((c1-C1)/c2)/gamma.ppf( 0.999996602, 1/c2 - 1, scale=1 )-C1)*sigma+mu; 
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( gamcdf(X[0],1./c2 - 1,1), 0,1 );
        #Inv2 = norm.ppf(gamcdf(X[1], 1. / c2 - 1, 1), 0, 1)

        #     X = -((c1-C1)./c2)./X-C1;
    elif dtype == 6:
        # F location-scale: standard support (a2,Inf) or (-Inf,a1)
        m1 = (a1 + c1) / (c2 * (a2 - a1))
        m2 = -(a2 + c1) / (c2 * (a2 - a1))
        # a1 and a2 have the same sign, and they've been sorted so a1 < a2
        if a2 < 0:
            nu1 = 2 * (m2 + 1)
            nu2 = -2 * (m1 + m2 + 1)
            X = (X - a2) / (a2 - a1) * (nu2 / nu1)
            #         r = a2 + (a2 - a1) .* (nu1./nu2) .* fpdf(X,nu1,nu2);

            p = fcdf(X[1], nu1, nu2) - fcdf(X[0], nu1, nu2)
            lo=(f.ppf( 3.39767E-06, nu1,nu2)+a2)*sigma+mu
            hi=(f.ppf( 0.999996602, nu1,nu2)+a2)*sigma+mu
            Inv1 = norm.ppf(p, 0, 1)
            #             Inv1=norm.ppf( fcdf(X[0],nu1,nu2), 0,1 );
            #Inv2 = norm.ppf(fcdf(X[1], nu1, nu2), 0, 1)

            #         X = a2 + (a2-a1).*(nu1./nu2).*X
        else:        # 0 < a1
            nu1 = 2 * (m1 + 1)
            nu2 = -2 * (m1 + m2 + 1)
            X = (X - a1) / (a1 - a2) * (nu2 / nu1)
            #         r = a1 + (a1 - a2) .* (nu1./nu2) .* fpdf(X,nu1,nu2);

            p = -fcdf(X[1], nu1, nu2) + fcdf(X[0], nu1, nu2)
            hi=(-f.ppf( 3.39767E-06, nu1,nu2)+a1)*sigma+mu;
            lo=(-f.ppf( 0.999996602, nu1,nu2)+a1)*sigma+mu; 
            Inv1 = norm.ppf(p, 0, 1)
            #             Inv1=norm.ppf( fcdf(X[0],nu1,nu2), 0,1 );
            #Inv2 = norm.ppf(fcdf(X[1], nu1, nu2), 0, 1)

            #         X = a1 + (a1-a2).*(nu1./nu2).*X;
    elif dtype == 7:
        # t location-scale: standard support (-Inf,Inf)

        nu = 1. / c2 - 1
        X = X / sqrt(c0 / (1 - c2))
        m1 = nu
        m2 = 0
        p = t.cdf(X[1], nu) - t.cdf(X[0], nu)
        lo=t.ppf( 3.39767E-06, nu )*sqrt(c0/(1-c2))*sigma+mu
        hi=t.ppf( 0.999996602, nu )*sqrt(c0/(1-c2))*sigma+mu
        Inv1 = norm.ppf(p, 0, 1)
        #         Inv1=norm.ppf( tcdf(X[0],nu), 0,1 );
        #Inv2 = norm.ppf(tcdf(X[1], nu), 0, 1)

        #     p = sqrt(c0./(1-c2)).*tpdf(X,nu);
        #     X = sqrt(c0./(1-c2)).*X;
    else:
        print "ERROR: Unknown data type!"
#    elif dtype == 8:
        #Monte Carlo Simulation Histogram
#        out = kurt
#        p = skew
#        m1 = 0
#        m2 = 0

    # scale and shift
    # X = X.*sigma + mu; % Undo z-score

    if dtype != 1 and dtype != 2:
        mu_s=(mu-lo)/(hi-lo);
        sigma_s=sigma ** 2/(hi-lo) ** 2;
        alph = ((1-mu_s)/sigma_s -1/mu_s)*mu_s ** 2;
        beta = alph*(1/mu_s - 1);

    if alph >70 or beta>70:
        alph=70;
        beta=70;
        lo=mu-11.87434*sigma
        hi=2*mu-lo

    return p, dtype, Inv1, m1, m2, a1, a2, alph, beta, lo, hi

def pearson4cdf(X, m, nu, a, _lambda, mu, sigma):
    # pearson4pdf
    #   p = pearson4pdf(X,m,nu,a,lambda)
    #
    #   Returns the pearson type IV probability density function with
    #   parameters m, nu, a and lambda at the values of X.
    #
    #   Example
    #
    #   See also
    #       pearson4pdf betapdf normpdf
    #       pearspdf pearsrnd
    #
    Xx = (X - _lambda) / a
    
    if Xx < -sqrt(3):    
        p1 = fx(X, m, nu, a, _lambda) * a / (2 * m - 1) * (1j - Xx) * hyp2f1(1, m + nu / 2 * 1j, 2 * m, 2 / (1 - 1j * Xx))    
        p = float(p1.real)   
    elif Xx > sqrt(3):    
        p1 = 1 - fx(-X, m, -nu, a, -_lambda) * a / (2 * m - 1) * (1j + Xx) * hyp2f1(1, m - nu / 2 * 1j, 2 * m, 2 / (1 + 1j * Xx))  
        p = float(p1.real)    
    elif Xx < 0 and Xx > -sqrt(3) and abs(nu) < (4 - 2 * sqrt(3)) * m:    
        p1 = norm.cdf(X, mu, sigma)    
        p = float(p1.real)   
    elif Xx < 0 and Xx > -sqrt(3) and abs(nu) > (4 - 2 * sqrt(3)) * m:    
        p1 = (1 - exp(-(nu + 1j * 2 * m) * pi)) ** (-1) - (1j * a * fx(X, m, nu, a, _lambda)) / (1j * nu - 2 * m + 2) * (1 + Xx ** 2) * hyp2f1(1, 2 - 2 * m, 2 - m + 1j * nu / 2, (1 + 1j * Xx) / 2)    
        p = float(p1.real)   
    elif Xx > 0 and Xx < sqrt(3) and abs(nu) < (4 - 2 * sqrt(3)) * m:    
        p1 = norm.cdf(X, mu, sigma)    
        p = float(p1.real)  
    else:     
        p1 = 1 - (1 - exp(-(-nu + 1j * 2 * m) * pi)) ** (-1) + (1j * a * fx(-X, m, -nu, a, -_lambda)) / (1j * (-nu) - 2 * m + 2) * (1 + (-Xx) ** 2) * hyp2f1(1,2-2*m,2-m-1j*nu/2,(1-1j*Xx)/2)    
        p = float(p1.real)
    
    return p


def fx(X=None, m=None, nu=None, a=None, _lambda=None):
    #Compute the pdf of pearson4
    Xx = (X - _lambda) / a
    k = -0.5 * log(pi) - log(a) - gammaln(m - 0.5) + 2 * (real(gammaln(m + (nu / 2) * 1j))) - gammaln(m)
    pearspdf = exp(k - m * log(1 + Xx ** 2) - nu * math.atan(Xx))
    
    return pearspdf

#This is a work-around for a bug in f.cdf() that returns "nan" for values of the second parameter < 1.0
def fcdf(x, d1, d2):
    result = f.cdf(x, d1, d2)
    if isnan(result):
        return betainc(d1/2., d2/2., d1*x*1./(d1*x+d2))
    return result


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



