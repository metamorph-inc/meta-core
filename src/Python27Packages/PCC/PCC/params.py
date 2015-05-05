from collections import namedtuple      #here so we can use a "structure"-like entity
from numpy import *
import gaussquad

#*****************COMPUTATION OF QUADRATURE NODES AND WEIGHTS**************
def params(method=None, m=None, inpt=None, stvars=None):
    
    node = zeros((inpt,max(m)))
    weight = zeros((inpt,max(m)))

    if method==4 or method==5:    
        for i in range(0,inpt):
            node[i], weight[i] = gaussquad.gaussquad(m[i], stvars[i].dist, stvars[i].param[0], stvars[i].param[1])
            if stvars[i].dist == 'BETA':     
                node[i] = node[i] * (stvars[i].param[3] - stvars[i].param[2]) + stvars[i].param[2]    
    
    return node,weight
    
    
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


