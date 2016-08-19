from numpy import *
from scipy.stats import beta,norm,lognorm,uniform
import random
import SRC_regress
from model_calls import run_list
import logging

def SA_EFAST(driver):

    #[SI,STI] = EFAST(K,WANTEDN)
    # First order and total effect indices for a given model computed with
    # Extended Fourier Amplitude Sensitivity Test (EFAST).
    # Andrea Saltelli, Stefano Tarantola and Karen Chan. 1999
    # A quantitative model-independent method for global sensitivity analysis of model output.
    # Technometrics 41:39-56
    #
    # Input:
    # inpt : no. of input factors
    # WANTEDN : wanted no. of sample points
    #
    # Output:
    # SI[] : first order sensitivity indices
    # STI[] : total effect sensitivity indices
    # Other used variables/constants:
    # OM[] : vector of inpt frequencies
    # OMI : frequency for the group of interest
    # OMCI[] : set of freq. used for the compl. group
    # X[] : parameter combination rank matrix
    # AC[],BC[]: fourier coefficients
    # FI[] : random phase shift
    # V : total output variance (for each curve)
    # VI : partial var. of par. i (for each curve)
    # VCI : part. var. of the compl. set of par...
    # AV : total variance in the time domain
    # AVI : partial variance of par. i
    # AVCI : part. var. of the compl. set of par.
    # Y[] : model output
    # N : no. of runs on each curve

    # ----------------------  Setup  ---------------------------
    methd = 'EFAST'
    method = '10'

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

    NR = 1#: no. of search curves
    MI = 4#: maximum number of fourier coefficients that may be retained in calculating
    # the partial variances without interferences between the assigned frequencies
    #
    # Computation of the frequency for the group of interest OMi and the no. of sample points N.
    OMi = int(floor((nEFAST / NR - 1) / (2 * MI) / inpt))
    N = 2 * MI * OMi + 1
    total_sims = N*NR*inpt
    sim = 0
    if (N * NR < 65):
        logging.error('sample size must be >= 65 per factor.')
        raise ValueError,'sample size must be >= 65 per factor.'

    # Algorithm for selecting the set of frequencies. OMci(i), i=1:inpt-1, contains
    # the set of frequencies to be used by the complementary group.
    OMci = SETFREQ(N - 1, OMi / 2 / MI)
    # Loop over the inpt input factors.
    Si = zeros((otpt,otpt,inpt));
    Sti = zeros((otpt,otpt,inpt));
    for i in range(inpt):
        # Initialize AV,AVi,AVci to zero.
        AV = 0
        AVi = 0
        AVci = 0
        # Loop over the NR search curves.
        for L in range(NR):
            # Setting the vector of frequencies OM for the inpt factors.
            cj = 1
            OM = zeros(inpt)
            for j in range(inpt):
                if (j == i):
                    # For the factor of interest.
                    OM[i] = OMi
                else:
                    # For the complementary group.
                    OM[j] = OMci[cj]
                    cj = cj + 1
            # Setting the relation between the scalar variable S and the coordinates
            # {X(1),X(2),...X(inpt)} of each sample point.
            FI = zeros(inpt)
            for j in range(inpt):
                FI[j] = random.random() * 2 * pi        # random phase shift
            S_VEC = pi * (2 * arange(1,N+1) - N - 1) / N
            OM_VEC = OM[range(inpt)]
            FI_MAT = transpose(array([FI]*N))
            ANGLE = matrix(OM_VEC).T*matrix(S_VEC) + matrix(FI_MAT)
            X = 0.5 + arcsin(sin(ANGLE.T)) / pi
            # Transform distributions from standard uniform to general.

            for j in range(inpt):
                if stvars[j].dist == 'NORM':
                    X[:,j] = norm.ppf(uniform.cdf(X[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])
                elif stvars[j].dist == 'LNORM':
                    X[:,j] = lognorm.ppf(uniform.cdf(X[:, j], 0, 1), stvars[j].param[1], 0, exp(stvars[j].param[0]))
                elif stvars[j].dist == 'BETA':
                    X[:,j] = beta.ppf(uniform.cdf(X[:, j], 0, 1), stvars[j].param[0], stvars[j].param[1], stvars[j].param[2], stvars[j].param[3] - stvars[j].param[2])
                elif stvars[j].dist == 'UNIF':
                    X[:,j] = uniform.ppf(uniform.cdf(X[:,j], 0, 1), stvars[j].param[0], stvars[j].param[1])

            # Do the N model evaluations.
            Y = zeros((N, otpt))
            if krig == 1:
                load("dmodel")
                Y = predictor(X, dmodel)
            else:
                values = []
                for p in range(N):
#                    sim += 1
#                    print 'Running simulation on test',sim,'of',total_sims
#                    Y[p] = run_model(driver, array(X[p])[0])
                    values.append(array(X[p])[0])
                Y = run_list(driver, values)

            # Subtract the average value.
            Y = Y - kron(mean(Y,0), ones((N, 1)))

            # Fourier coeff. at [1:OMi/2].
            NQ = int(N / 2)-1
            N0 = NQ + 1
            COMPL = 0
            Y_VECP = Y[N0+1:] + Y[NQ::-1]
            Y_VECM = Y[N0+1:] - Y[NQ::-1]
#            AC = zeros((int(ceil(OMi / 2)), otpt))
#            BC = zeros((int(ceil(OMi / 2)), otpt))
            AC = zeros((OMi * MI, otpt))
            BC = zeros((OMi * MI, otpt))
            for j in range(int(ceil(OMi / 2))+1):
                ANGLE = (j+1) * 2 * arange(1,NQ+2) * pi / N
                C_VEC = cos(ANGLE)
                S_VEC = sin(ANGLE)
                AC[j] = (Y[N0] +matrix(C_VEC)*matrix(Y_VECP)) / N
                BC[j] = matrix(S_VEC) * matrix(Y_VECM) / N
                COMPL = COMPL + matrix(AC[j]).T * matrix(AC[j]) + matrix(BC[j]).T * matrix(BC[j])
            # Computation of V_{(ci)}.
            Vci = 2 * COMPL
            AVci = AVci + Vci
            # Fourier coeff. at [P*OMi, for P=1:MI].
            COMPL = 0
# Do these need to be recomputed at all?
#            Y_VECP = Y[N0 + range(NQ)] + Y[N0 - range(NQ)]
#            Y_VECM = Y[N0 + range(NQ)] - Y[N0 - range(NQ)]
            for j in range(OMi, OMi * MI + 1, OMi):
                ANGLE = j * 2 * arange(1,NQ+2) * pi / N
                C_VEC = cos(ANGLE)
                S_VEC = sin(ANGLE)
                AC[j-1] = (Y[N0] + matrix(C_VEC)*matrix(Y_VECP)) / N
                BC[j-1] = matrix(S_VEC) * matrix(Y_VECM) / N
                COMPL = COMPL + matrix(AC[j-1]).T * matrix(AC[j-1]) + matrix(BC[j-1]).T * matrix(BC[j-1])
            # Computation of V_i.
            Vi = 2 * COMPL
            AVi = AVi + Vi
            # Computation of the total variance in the time domain.
            AV = AV +  matrix(Y).T * matrix(Y) / N
        # Computation of sensitivity indicies.
        AV = AV / NR
        AVi = AVi / NR
        AVci = AVci / NR
        Si[:, :, i] = AVi / AV
        Sti[:, :, i] = 1 - AVci / AV

    if lrflag == 1:
        SRC, stat = SRC_regress.SRC_regress(X, Y, otpt, N)

    # ----------------------  Analyze  ---------------------------

#    if plotf == 1:
#        piecharts(inpt, otpt, Si, Sti, methd, output)
    if simple == 1:
        Si_t = zeros((inpt,otpt))
        for p in range(inpt):
            Si_t[p] = diag(Si[:, :, p])
        Si = Si_t.T
    if simple == 1:
        Sti_t = zeros((inpt,otpt))
        for p in range(inpt):
            Sti_t[p] = diag(Sti[:, :, p])
        Sti = Sti_t.T
    Results = {'FirstOrderSensitivity': Si, 'TotalEffectSensitivity': Sti}
    if lrflag == 1:
        Results.update({'SRC': SRC, 'R^2': stat})
    return Results

# Algorithm for selection of a frequency set for the complementary group. Done
# recursively as described in: Appendix of Sensitivity Analysis [Saltelli et al.]
def SETFREQ(Kci, OMciMAX):
    if Kci == 1:
        OMci = 1
    elif OMciMAX == 1:
        OMci = ones(Kci)
    else:
        if (OMciMAX < Kci):
            INFD = OMciMAX
        else:
            INFD = Kci
        ISTEP = int(round((OMciMAX - 1) / (INFD - 1)))
        if (OMciMAX == 1):
            ISTEP = 0
        print 'ISTEP=',ISTEP
        OTMP = range(1, int(INFD * ISTEP) + ISTEP, ISTEP)
        fl_INFD = int(floor(INFD))
        OMci = zeros(Kci)
        for i in range(Kci):
            j = mod(i - 1, fl_INFD)
            OMci[i] = OTMP[j]

    return OMci

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



