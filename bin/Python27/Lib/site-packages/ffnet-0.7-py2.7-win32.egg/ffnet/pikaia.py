########################################################################
##  Copyright (C) 2006 by Marek Wojciechowski
##  <mwojc@p.lodz.pl>
##
##  Distributed under the terms of the GNU General Public License (GPL)
##  http://www.gnu.org/copyleft/gpl.html
########################################################################
'''
Genetic algorithm based optimizer.
'''

# import raw module 
from fortran import _pikaia

# Wrap main pikaia routine
def pikaia (ff, n, ff_extra_args = (), \
            individuals = 100, \
            generations = 500, \
            digits = 6, \
            crossover = 0.85, \
            mutation = 2, \
            initrate = 0.005, \
            minrate = 0.0005, \
            maxrate = 0.25, \
            fitnessdiff = 1.0, \
            reproduction = 3, \
            elitism = 0, \
            verbosity = 0):

    """
    Pikaia version 1.2 - genetic algorithm based optimizer.
    
    Wrapped with f2py by Marek Wojciechowski
    <mwojc@p.lodz.pl>
    
    Original fortran code: Paul Charbonneau & Barry Knapp
    <paulchar@hao.ucar.edu>
    <knapp@hao.ucar.edu>
    
    Simplest usage:
        from pikaia import pikaia
        x = pikaia(ff, n)
    
    Function ff is a user-supplied scalar function of n vari-
    ables, which must have the calling sequence f = ff(x, [n], [args]),
    where x is a real parameter array of length n and [args] are
    optional parameters. This function must
    be written so as to bound all parameters to the interval [0,1];
    that is, the user must determine a priori bounds for the para-
    meter space, and ff must use these bounds to perform the appro-
    priate scalings to recover true parameter values in the
    a priori ranges.
    
    By convention, ff should return higher values for more optimal
    parameter values (i.e., individuals which are more "fit").
    For example, in fitting a function through data points, ff
    could return the inverse of chi**2.
    
    Additional ff parameters [args] can be passed to pikaia routine
    via keyword argument ff_extra_args which have to be a tuple
    of variables.
    
    Returning array x is the "fittest" (optimal) solution found,
    i.e., the solution which maximizes fitness function ff.
    
    Algorithm control parameters are set via keyword arguments:
    individuals  - number of individuals in a population (default
                   is 100)
    generations  - number of generations over which solution is
                   to evolve (default is 500)
    digits       - number of significant digits (i.e., number of
                   genes) retained in chromosomal encoding (default
                   is 6)  (Note: This number is limited by the
                   machine floating point precision.  Most 32-bit
                   floating point representations have only 6 full
                   digits of precision.  To achieve greater preci-
                   sion this routine could be converted to double
                   precision, but note that this would also require
                   a double precision random number generator, which
                   likely would not have more than 9 digits of
                   precision if it used 4-byte integers internally.)
    crossover    - crossover probability; must be  <= 1.0 (default
                   is 0.85). If crossover takes place, either one
                   or two splicing points are used, with equal
                   probabilities
    mutation     - 1/2/3/4/5 (default is 2)
                   1=one-point mutation, fixed rate
                   2=one-point, adjustable rate based on fitness
                   3=one-point, adjustable rate based on distance
                   4=one-point+creep, fixed rate
                   5=one-point+creep, adjustable rate based on fitness
                   6=one-point+creep, adjustable rate based on distance
    initrate     - initial mutation rate; should be small (default
                   is 0.005) (Note: the mutation rate is the proba-
                   bility that any one gene locus will mutate in
                   any one generation.)
    minrate      - minimum mutation rate; must be >= 0.0 (default
                   is 0.0005)
    maxrate      - maximum mutation rate; must be <= 1.0 (default
                   is 0.25)
    fitnessdiff  - relative fitness differential; range from 0
                   (none) to 1 (maximum).  (default is 1.)
    reproduction - reproduction plan; 1/2/3=Full generational
                   replacement/Steady-state-replace-random/Steady-
                   state-replace-worst (default is 3)
    elitism      - elitism flag; 0/1=off/on (default is 0)
                   (Applies only to reproduction plans 1 and 2)
    verbosity    - printed output 0/1/2=None/Minimal/Verbose
                   (default is 0)
    
    Thus more complex usage:
        x = pikaia( ff, n, ff_extra_args = (1, 0.5), \ 
                    individuals = 200, \ 
                    generations = 50, \ 
                    verbosity = 1 )
                    
    """
    
    # Initialize pikaia random number generator
    from random import randint
    _pikaia.rninit(randint(1, 999999999))
    del randint

    # Restore control array
    ctrl = [ individuals, generations, digits, crossover, mutation, initrate, \
             minrate, maxrate, fitnessdiff, reproduction, elitism, verbosity ]

    # Optimize
    x, f, status = _pikaia.pikaia(ff, n, ctrl, ff_extra_args)
    return x

def test():
    x = pikaia(_pikaia.twod, 2)
    print "Solution for twod:"
    print x

    