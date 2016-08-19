from numpy import array, zeros

def taylorseries(G, x, delta, inpt, otpt):
    grad = zeros((inpt, otpt))
    for i in range(inpt):
        xvecPLUS = array(x)
        xvecMINS = array(x)
        xvecPLUS[i] += delta[i]
        xvecMINS[i] -= delta[i]
#        xvecPLUS = Tinv(xvecPLUS)
#        xvecMINS = Tinv(xvecMINS)
        grad[i] = (G(xvecPLUS) - G(xvecMINS)) / (2.0 * delta[i])

    return grad

# preprocess taylor series
def pretaylorseries(x, delta, inpt):
    values = []
    for i in range(inpt):
        xvecPLUS = array(x)
        xvecMINS = array(x)
        xvecPLUS[i] += delta[i]
        xvecMINS[i] -= delta[i]
        values.append(xvecPLUS)
        values.append(xvecMINS)

    return values

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
