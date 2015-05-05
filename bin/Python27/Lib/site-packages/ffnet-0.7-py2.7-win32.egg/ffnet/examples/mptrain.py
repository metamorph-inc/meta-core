# -*- coding: utf-8 -*-
########################################################################
##  Copyright (C) 2011 by Marek Wojciechowski
##  <mwojc@p.lodz.pl>
##
##  Distributed under the terms of the GNU General Public License (GPL)
##  http://www.gnu.org/copyleft/gpl.html
########################################################################
from ffnet import ffnet, mlgraph
from scipy import rand

# Generate random training data (large)
input = rand(10000, 10)
target = rand(10000, 1)

# Define net (large one)
conec = mlgraph((10,300,1))
net = ffnet(conec)

# Test training speed-up
# Note that the below *if* is necessary only on Windows
if __name__=='__main__':    
    stored_weights = net.weights.copy()
    
    print "Training in single process:"
    from time import time
    t0 = time()
    net.train_tnc(input, target, nproc = 1, maxfun=50, messages=1) 
    t1 = time()
    single_time = t1-t0

    print

    from multiprocessing import cpu_count
    print "Trainig in %s processes:" %cpu_count()
    net.weights = stored_weights # Just to start from the same point
    t0 = time()
    net.train_tnc(input, target, nproc = 'ncpu', maxfun=50, messages=1) # THAT'S IT!
    t1 = time()
    allproc_time = t1-t0

    print
    print 'Train time, single process:', single_time
    print 'Train time, %s processes:' %cpu_count(), allproc_time

