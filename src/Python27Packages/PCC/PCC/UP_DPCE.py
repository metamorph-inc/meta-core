from numpy import *
import pearscdf
from mvncdf import mvstdnormcdf
import estimate_complexity
import itertools  # for fullfact function
import os
import subprocess
import multiprocessing


def UP_DPCE(problem, driver):
    # Uses the Dakota PCE method for UP

    # ----------------------  Setup  ---------------------------
    methd = 'DPCE'
    method = 6

    inpt = len(driver.inputs)
    krig = driver.krig
    limstate = driver.limstate
    order = driver.order
    otpt = len(driver.outputNames)
    output = driver.outputNames

# preprocess inputs, as Dakota won't let us specify them one at a time, only in groups according to distribution type
    norm = []
    lnorm = []
    beta = []
    unif = []
    for idx, stvar in enumerate(driver.stvars):
        if stvar.dist == 'NORM':
            norm.append(idx)
        elif stvar.dist == 'LNORM':
            lnorm.append(idx)
        elif stvar.dist == 'BETA':
            beta.append(idx)
        elif stvar.dist == 'UNIF':
            unif.append(idx)

    if len(driver.Wrapper) == 0:
        raise Exception('Must specify a path to the model wrapper file.')

    OldDir = os.getcwd()
    WorkDir = os.path.dirname(driver.Wrapper)
    if WorkDir != "":
        os.chdir(WorkDir)

    with open('parameters.csv', 'w') as f:  # write log file of inputs and outputs
        f.write(','.join(driver.inputNames)+','+','.join(driver.outputNames)+'\n')
        f.close()

    f = open('dakota_pce.in', 'w')
    f.write('strategy\n')  # look at dakota input summary
    f.write('    single_method\n')  # graphics
    f.write('method\n')
    f.write('    polynomial_chaos\n')
    f.write('        quadrature_order')
    for node in driver.nodes:
        f.write(' {0}'.format(node))
    f.write('\n        variance_based_decomp\n')  # univariate_effects
#    f.write('        num_response_levels =')
#    for x in range(otpt):
#        f.write(' 2')
#    f.write('\n')
    f.write('        response_levels')
    for limits in driver.limstate:
        f.write(' {0} {1}'.format(limits[0], limits[1]))
    f.write('\n')
#    f.write('        compute reliabilities\n') #default is probabilities
    f.write('        rng\n')
    f.write('            rnum2\n')
    f.write('        samples 10000\n')
#    f.write('        seed 12347\n')
    f.write('variables')

    if len(norm) > 0:
        f.write('\n    normal_uncertain {0}\n'.format(len(norm)))
#            v[j] = norm.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[0], stvars[j].param[1])
        f.write('        means')
        for idx in norm:
            f.write(' {0}'.format(driver.stvars[idx].param[0]))
        f.write('\n        std_deviations')
        for idx in norm:
            f.write(' {0}'.format(driver.stvars[idx].param[1]))
        f.write('\n        descriptors')
        for idx in norm:
            f.write(' \'{0}\''.format(driver.stvars[idx].name))
    if len(lnorm) > 0:
        f.write('\n    lognormal_uncertain {0}\n'.format(len(lnorm)))
#            v[j] = lognorm.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
        f.write('        lnuv_means')
        for idx in lnorm:
            f.write(' {0}'.format(driver.stvars[idx].param[1]))
        f.write('\n        lnuv_std_deviations')
        for idx in lnorm:
            f.write(' {0}'.format(exp(driver.stvars[idx].param[0])))
        f.write('\n        lnuv_descriptors')
        for idx in lnorm:
            f.write(' \'{0}\''.format(driver.stvars[idx].name))
    if len(beta) > 0:
        f.write('\n    beta_uncertain {0}\n'.format(len(beta)))
#            v[j] = beta.ppf(norm.cdf(value[j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
        f.write('        alphas')
        for idx in beta:
            f.write(' {0}'.format(driver.stvars[idx].param[0]))
        f.write('\n        betas')
        for idx in beta:
            f.write(' {0}'.format(driver.stvars[idx].param[1]))
        f.write('\n        lower_bounds')
        for idx in beta:
            f.write(' {0}'.format(driver.stvars[idx].param[2]))
        f.write('\n        upper_bounds')
        for idx in beta:
            f.write(' {0}'.format(driver.stvars[idx].param[3]))
        f.write('\n        descriptors')
        for idx in beta:
            f.write(' \'{0}\''.format(driver.stvars[idx].name))
    if len(unif) > 0:
        f.write('\n    uniform_uncertain {0}\n'.format(len(unif)))
        f.write('        lower_bounds')
        for idx in unif:
            f.write(' {0}'.format(driver.stvars[idx].param[0]))
        f.write('\n        upper_bounds')
        for idx in unif:
            f.write(' {0}'.format(driver.stvars[idx].param[1]))
        f.write('\n        descriptors')
        for idx in unif:
            f.write(' \'{0}\''.format(driver.stvars[idx].name))

    f.write('\ninterface\n')
#    f.write('    fork\n')
    f.write('    fork asynchronous evaluation_concurrency = {0}\n'.format(multiprocessing.cpu_count()))
#    f.write('        analysis_drivers \'python {0}\'\n'.format(driver.workflow.__iter__().next().getFile()))
    f.write('        analysis_drivers \'python {0}\'\n'.format(driver.Wrapper))
    f.write('        parameters_file =\'params.in\'\n')
    f.write('        results_file =\'results.out\'\n')
    f.write('        work_directory\n')
    f.write('        local_evaluation_static_scheduling\n')
    f.write('        directory_tag\n')
    f.write('        copy\n')
    f.write('        template_directory =\'{0}\'\n'.format(os.path.dirname(driver.Wrapper)))
    f.write('responses\n')
    f.write('    response_functions {0}\n'.format(otpt))  # number of outputs
    f.write('    no_gradients\n')  # leave as-is?
    f.write('    no_hessians\n')  # leave as-is?
    f.close()

    command = 'dakota dakota_pce.in | tee dakota_output.txt'
    print 'Calling "{0}" as a subprocess.'.format(command)
    return_code = subprocess.call(command, shell=True)

    f = open('dakota_output.txt', 'r')
    dakota_output = f.read().split()
    f.close()

    os.chdir(OldDir)

    G_mean = zeros(otpt)
    G_kurt = zeros(otpt)
    G_skew = zeros(otpt)

    fn_start = dakota_output.index('Mean') + 5  # index of response_fn_1
    for out in range(otpt):
        G_mean[out] = float(dakota_output[fn_start+2])
        G_skew[out] = float(dakota_output[fn_start+7])
        G_kurt[out] = float(dakota_output[fn_start+8])+3
        fn_start = fn_start + 9  # go to next response function, if any

    DPCC = [0]*otpt
    fn_start = dakota_output.index('(CDF)')  # index of CDF for response_fn_1
    for out in range(otpt):
        DPCC[out] = float(dakota_output[fn_start+19]) - float(dakota_output[fn_start+17])
        fn_start = fn_start + 23  # go to next response function, if any
    print 'Dakota PCCs:', DPCC

    CovarianceMatrix = zeros((otpt, otpt))
    covpos = dakota_output.index('[[')+1
    for y in range(otpt):
        for x in range(otpt):
            CovarianceMatrix[x,y] = float(dakota_output[covpos])
            covpos = covpos + 1

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
