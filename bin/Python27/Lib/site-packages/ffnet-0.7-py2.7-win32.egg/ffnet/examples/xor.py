########################################################################
##  Copyright (C) 2006 by Marek Wojciechowski
##  <mwojc@p.lodz.pl>
##
##  Distributed under the terms of the GNU General Public License (GPL)
##  http://www.gnu.org/copyleft/gpl.html
########################################################################

### XOR problem example for ffnet ###

from ffnet import ffnet, mlgraph

# Generate standard layered network architecture and create network
conec = mlgraph((2,2,1))
net = ffnet(conec)

# Define training data
input = [[0.,0.], [0.,1.], [1.,0.], [1.,1.]]
target  = [[1.], [0.], [0.], [1.]]

# Train network
#first find good starting point with genetic algorithm (not necessary, but may be helpful)
print "FINDING STARTING WEIGHTS WITH GENETIC ALGORITHM..."
net.train_genetic(input, target, individuals=20, generations=500)
#then train with scipy tnc optimizer
print "TRAINING NETWORK..."
net.train_tnc(input, target, maxfun = 1000, messages=1)

# Test network
print
print "TESTING NETWORK..."
output, regression = net.test(input, target, iprint = 2)

# Save/load/export network
from ffnet import savenet, loadnet, exportnet
print "Network is saved..."
savenet(net, "xor.net")
print "Network is reloaded..."
net = loadnet("xor.net")
print "Network is tested again, but nothing is printed..."
output, regression = net.test(input, target, iprint = 0)
print
print "Exporting trained network to the fortran source..."
exportnet(net, "xor.f")
print "Done..."
print "Look at the generated xor.f file."
print "Note: you must compile xor.f along with the ffnet.f" 
print "file which can be found in ffnet sources."
