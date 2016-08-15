from numpy import *
import logging

# Calculate nodes and weights for Guassian quadrature using probability densities.
# Adapted from Netlib routine gaussq.f
def gaussquad(n=None, dist=None, param1=None, param2=None):
    if n < 0:
        logging.error('need non-negative number of nodes')
        raise ValueError,'need non-negative number of nodes'
    elif n == 0:
        nodes = 0
        weights = 0
    elif n == 1:
        if dist == 'UNIF':
            x = (param1 + param2) / 2
        elif dist == 'BETA':
            x = param1 / (param1 + param2)
        elif dist == 'NORM':
            x = param1
        elif dist == 'LNORM':
            x = exp(param1)
        else:
            logging.error('Unexpected distribution type')
            raise ValueError,'Unexpected distribution type'
        nodes = x
        weights = 1
    else:
        if dist=='BETA' and param1 == 0.5 and param2 == 0.5:
            dist = 'beta1'
        if dist=='BETA' and param1 == 1.5 and param2 == 1.5:
            dist = 'beta2'

        i = arange(1.0,n+1)
        i1 = arange(1.0,n)
        if dist == 'UNIF':
            a = zeros((n, 1))
            b = i1 / sqrt(4 * i1 ** 2 - 1)
        elif dist == 'beta1':
            a = zeros((n, 1))
            b = 0.5 * ones((n - 1, 1))
            b[0] = sqrt(0.5)
        elif dist == 'beta2':
            a = zeros((n, 1))
            b = 0.5 * ones((n - 1, 1))
        elif dist == 'NORM':
            a = zeros((n, 1))
            b = sqrt(i1 / 2.)
        elif dist == 'LNORM':
            a = zeros((n, 1))
            b = sqrt(i1 / 2.)
        elif dist == 'BETA':
            ab = param1 + param2
            a = i
            a[0] = param1 - param2 / ab
            i2 = arange(2,n+1)
            abi = ab - 2 + 2 * i2
            a[1:n] = ((param1 - 1) ** 2 - (param2 - 1) ** 2) / (abi - 2) / abi
            b = i1
            b[0] = sqrt(4 * param1 * param2 / ab ** 2. / (ab + 1))
            i2 = i1[1:(n - 1)]
            abi = ab - 2 + 2 * i2
            b[1:(n - 1)] = sqrt(4. * i2 * (i2 + param1 - 1) * (i2 + param2 - 1) * (i2 + ab - 2) / (abi ** 2 - 1) / abi ** 2)
        else:
            logging.error('Unexpected distribution type.')
            raise ValueError,'Unexpected distribution type.'
        A = zeros((n * n, 1))
        for j in range(0,n):
            A[(n + 1) * j] = a[j]
        for j in range(0,n - 1):
            A[(n + 1) * j + 1] = b[j]
            A[(n + 1) * (j+1)-1] = b[j]
        A = A.reshape(n, n)
        x, V = linalg.eig(A)
        w = V[0,:]
        w = w**2
        if dist == 'UNIF':
            x = param1 + (param2 - param1) * (x + 1) / 2.
        elif dist == 'BETA':
            x = (x + 1) / 2.
        elif dist == 'beta1':
            x = (x + 1) / 2.
        elif dist == 'beta2':
            x = (x + 1) / 2.
        elif dist == 'NORM':
            x = param1 + sqrt(2) * param2 * x
        elif dist == 'LNORM':
            x = exp(param1 + sqrt(2) * param2 * x)
        nodes = x
        weights = w

    return nodes, weights
