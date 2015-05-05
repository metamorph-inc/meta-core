# encoding: utf-8

import sys
import getopt
import math
import csv
import numpy as np
from types import FunctionType
from scipy.stats import norm
from scipy.stats import uniform
from scipy.stats import beta



# param contains the type of the distribution and its parameters
# returns:
# for dist=False: returns float random variate
# for dist=True: returns (_->float) function, which gives
# a random variate when called
def parse_distribution(params, dist=False):
    distribName = params[0]
    params[1], params[2] = float(params[1]), float(params[2])
    if distribName == 'norm':
        if len(params)!=3:
            displaymessage("Normal distribution takes two parameters",end_execution=False)
            out=1
        loc, scale = params[1], params[2]
        if dist:
            out = lambda x : norm.rvs(loc, scale)
        else:
            loop_counter=0;
            out=norm.rvs(loc,scale)
            while (out<0):
                out=norm.rvs(loc,scale)
                loop_counter+=1
                if loop_counter>100:
                    displaymessage("Distribution too far in the negative. Set to default 1.",end_execution=False)
                    out=1
                    break
            
    elif distribName == 'uniform':   #constant between loc and loc+scale
        if len(params)!=3:
            displaymessage("Uniform distribution takes two parameters",end_execution=False)
            out=1
        loc, scale = params[1], params[2]
        if (loc+scale <= 0):
            displaymessage("Distribution lies entirely to the left of the y-axis. Changed to default value 1.",end_execution=False)
            out=1
        elif loc<0:
            displaymessage("Uniform distribution takes negative values.",end_execution=False)
            loc = max(loc,0)
        else:
            if dist:
                out = lambda x : uniform.rvs(loc, scale)
            else:
                out = uniform.rvs(loc, scale)
            
    elif distribName == 'beta':
        if len(params)!=5:
            displaymessage("Normal distribution takes four parameters",end_execution=False)
            out=1
        a, b, loc, scale = params[1], params[2], float(params[3]), float(params[4])
        if dist:
            out = lambda x : beta.rvs(a, b, loc, scale)
        else:
            out = beta.rvs(a, b, loc, scale)
    return out

# 'filename.csv' contains n lines, where the i-th line contains i, alpha_i
# for lazy=True alpha_i can be float as well as a (_->float) function
# returns: size of matrix n array
def component_complexity(filename, dist=False, sampleSize=1):
    f = open(filename)
    reader = csv.reader(f)
    Alpha = []
    alpha_array = np.zeros(sampleSize)

    #these variables are continously updated in the function
    #with the smallest and largest ID's found
    min_i=99999;
    max_i=-99999;
    
    for row in reader:
        try:
            i = int(row[0]) #read ID
        except:
            displaymessage("Component ID not an integer in '%s'" % filename,end_execution=True)

        min_i = min(i,min_i)  #is ID smaller than smallest ID up to now?
        max_i = max(i,max_i)  #is ID larger than largest ID up to now?
        
        alpha_i = float(row[1]) #read value

        #is value a distribution?
        if alpha_i ==-1 and row[2] in ['norm', 'uniform', 'beta']:
            alpha_i = parse_distribution(row[2:], dist) #not necessary to save value, but perhaps for later use
            for k in range (sampleSize):
                alpha_array[k]+=parse_distribution(row[2:], dist)
        #if not a distribution, just add the value
        else:
            for k in range (sampleSize):
                alpha_array[k]+=alpha_i
                
        #technically not necessary to save
        Alpha.append((i, alpha_i))
    
    # [..,(1,b),...(2,c),...(0,a)...]  -->  [(0,a),(1,b),(2,c),..] 
    Alpha.sort()
    
    # [(0,a),(1,b),(2,c),..]  -->  [a,b,c]
    Alpha = [x[1] for x in Alpha]
    
    f.close()
    
    if min_i != 1:
          displaymessage("Smallest component ID is below 1 in '%s'" % filename,end_execution=True)

    if max_i != len(Alpha):
          displaymessage("Component ID's inconsistent in '%s'" % filename,end_execution=True)
        
    #return np.array(Alpha)
    return [len(Alpha), alpha_array]

# 'filename.csv' contains n × n lines in the format i, j, beta_ij
# returns: array
def interface_complexity(filename, n, dist=False, sampleSize=1):
    f = open(filename)
    reader = csv.reader(f)
    rows = []
    for row in reader:
       rows.append(row)

    connectivity_A = np.zeros((n,n));
    #Beta = np.zeros((n,n))
    beta_array = np.zeros(sampleSize);
    
    for row in reader:
       rows.append(row)

    for row in rows:
        try:
            i = int(row[0]) - 1  # shift indexing from 1,2,3,... to 0,1,2,...
            j = int(row[1]) - 1
        except:
            displaymessage("Component ID not an integer in '%s'" % filename,end_execution=True)
        
        if (i>=n) | (j>=n):
            displaymessage("Component ID in %s larger than expected in '%s'" % filename,end_execution=True)
        else:
            connectivity_A[i][j]=1;

        beta_ij = float(row[2])
        
        if beta_ij < 0:
            if row[3] in ['norm', 'uniform', 'beta']:
                for k in range (sampleSize):
                    beta_array[k]+=parse_distribution(row[3:])
            else:
                displaymessage("Corrupt entry in '%s' or distribution not supported. Value set to 1." % filename,end_execution=False)
                for k in range (sampleSize):
                    beta_array[k]+=1
        else:
            for k in range (sampleSize):
                    beta_array[k]+=beta_ij
            
            
        #Beta[i][j] = beta_ij
    f.close()

    svd = sum(np.linalg.svd(connectivity_A)[1])

    return beta_array*svd/n



# can be a float or a function returning a float
# returns: float
def eval_variate(var):
    if isinstance(var, FunctionType):
        return var(0)
    else:
        return var

# displays 'message_text' as warning or error message
# if end_execution=True, then program will be aborted
# THIS FUNCTION CAN BE ADAPTED
# FOR INTEGRATION WITH THE DISPLAY MODULE
def displaymessage(message_text,end_execution=False):
    print message_text
    if end_execution:
        exit()
    return


# computes complexity and returns an array
# sampleSize = number of random samples
# 
def compute_complexity(fname_component, fname_interface,sampleSize=1):
    
    [n,alpha_array] = component_complexity(fname_component,sampleSize=sampleSize)
    #print "Alpha", alpha_array

    beta_array = interface_complexity(fname_interface, n,sampleSize=sampleSize)
    #print "Beta", beta_array
    
    return alpha_array+beta_array


help_message = '''
Main.py [component_complexity.csv] [interface_complexity.csv]
'''


class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg


def main(argv=None):
    if argv is None:
        argv = sys.argv
    try:
        try:
            opts, args = getopt.getopt(argv[1:], "ho:v", ["help", "output="])
        except getopt.error, msg:
            raise Usage(msg)
    
        # option processing
        for option, value in opts:
            if option == "-v":
                verbose = True
            if option in ("-h", "--help"):
                raise Usage(help_message)
            if option in ("-o", "--output"):
                output = value
    
    except Usage, err:
        print >> sys.stderr, sys.argv[0].split("/")[-1] + ": " + str(err.msg)
        return 2
    if len(args) >= 3:
        print compute_complexity(args[0], args[1], args[2])
    else:
        #print compute_complexity('component_complexity.csv', 'interface_complexity.csv')

        sampleSize=10
        X=compute_complexity('component_complexity1.csv', 'interface_complexity1.csv',sampleSize)
        print "Example 1:"
        print X
        print "mean: ",sum(X)/len(X)

        sampleSize=10
        X=compute_complexity('component_complexity2.csv', 'interface_complexity2.csv',sampleSize)
        print "Example 2:"
        print X
        print "mean: ",sum(X)/len(X)

        sampleSize=10
        X=compute_complexity('component_complexity3.csv', 'interface_complexity3.csv',sampleSize)
        print "Example 3:"
        print X
        print "mean: ",sum(X)/len(X)

    return    


if __name__ == "__main__":
    main()
