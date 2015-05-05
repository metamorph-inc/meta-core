# encoding: utf-8

import re
import os
import sys
import getopt
import math
import csv
import json
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

    #always sets value to 1 if number of parameters does not match expectations
    
    # NORMAL DISTRIBUTION: loc, scale
    # Cut off in the negative by reject/accept method. Aborts after 100 rejections
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

    # UNIFORM DISTRIBUTION: loc, scale
    #Corrects range and displays warning if it lies partially in the negative.
    #Sets to 1 and displays warning, if it lies completely in the negative
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

    # BETA DISTRIBUTION: a,b, loc, scale        
    elif distribName == 'beta':
        if len(params)!=5:
            displaymessage("Beta distribution takes four parameters",end_execution=False)
            out=1
        a, b, loc, scale = params[1], params[2], float(params[3]), float(params[4])
        if dist:
            out = lambda x : beta.rvs(a, b, loc, scale)
        else:
            out = beta.rvs(a, b, loc, scale)

    # LOGNORM DISTRIBUTION: s, loc, scale        
    elif distribName == 'lognorm':
        if len(params)!=4:
            displaymessage("Lognorm distribution takes three parameters",end_execution=False)
            out=1
        s, loc, scale = params[1], params[2], float(params[3])
        if dist:
            out = lambda x : lognorm.rvs(s, loc, scale)
        else:
            out = lognorm.rvs(s, loc, scale)

    # TRIANGULAR DISTRIBUTION: c, loc, scale        
    elif distribName == 'triang':
        if len(params)!=4:
            displaymessage("Triangular distribution takes three parameters",end_execution=False)
            out=1
        c, loc, scale = params[1], params[2], float(params[3])
        if dist:
            out = lambda x : triang.rvs(c, loc, scale)
        else:
            out = triang.rvs(c, loc, scale)
    return out

# 'filename.csv' contains n lines, where the i-th line contains i, alpha_i
# for lazy=True alpha_i can be float as well as a (_->float) function
# returns: size of matrix n array
def component_complexity(filename, dist=False, sampleSize=200):
    f = open(filename,"rU")
    reader = csv.reader(f)
    Alpha = []
    alpha_samples = np.zeros(sampleSize)

    #these variables are continously updated in the function
    #with the smallest and largest ID's found
    min_i=99999;
    max_i=-99999;
    
    for row in reader:
        try:
            i = int(row[0+1]) #read ID
        except:
            displaymessage("Component ID not an integer in '%s'" % filename,end_execution=True)

        min_i = min(i,min_i)  #is ID smaller than smallest ID up to now?
        max_i = max(i,max_i)  #is ID larger than largest ID up to now?
        
        alpha_i = float(row[1+1]) #read value

        #is value a distribution?
        if alpha_i ==-1 and row[2+1] in ['norm', 'uniform', 'beta','lognorm','triang']:
            X=parse_distribution(row[2+1:], True)
            #print eval_variate(X),eval_variate(X)
            alpha_i = parse_distribution(row[2+1:], dist=True) #not necessary to save value, but perhaps for later use
            for k in range (sampleSize):
                alpha_samples[k]+=parse_distribution(row[2+1:], dist)
        #if not a distribution, just add the value
        else:
            for k in range (sampleSize):
                alpha_samples[k]+=alpha_i
                
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
    return [Alpha, alpha_samples]

# 'filename.csv' contains n × n lines in the format i, j, beta_ij
# returns: array
def interface_complexity(filename, Alpha, dist=False, sampleSize=200):
    f = open(filename,"rU")
    reader = csv.reader(f)
    rows = []
    for row in reader:
       rows.append(row)

    n = len(Alpha)   #determines size of matrix
    connectivity_A = np.zeros((n,n))
    #Beta = np.zeros((n,n))
    beta_array = np.zeros(sampleSize)
    
    for row in reader:
       rows.append(row)

    for row in rows:
        try:
            i = int(row[0+0]) - 1  # shift indexing from 1,2,3,... to 0,1,2,...
            j = int(row[1+0]) - 1
        except:
            displaymessage("Component ID not an integer in '%s'" % filename,end_execution=True)
        
        if (i>=n) | (j>=n):
            displaymessage("Component ID in %s larger than expected in '%s'" % filename,end_execution=True)
        else:
            connectivity_A[i][j]=1;

        beta_ij = float(row[2+0])
        
        if beta_ij < 0:
            if row[3+0] in ['norm','uniform','beta','lognorm','triang']:
                for k in range (sampleSize):
                    beta_array[k]+=parse_distribution(row[3+0:])*eval_variate(Alpha[i])*eval_variate(Alpha[j])
            else:
                displaymessage("Corrupt entry in '%s' or distribution not supported. Value set to 1." % filename,end_execution=False)
                for k in range (sampleSize):
                    beta_array[k]+=1*eval_variate(Alpha[i])*eval_variate(Alpha[j])
        else:
            for k in range (sampleSize):
                    beta_array[k]+=beta_ij*eval_variate(Alpha[i])*eval_variate(Alpha[j])
            
            
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

def update_complexity_metrics(complexity_value=-1):
    # check if summary report json file exits
    if not os.path.exists('testbench_manifest.json'):
        print 'ERROR! File does not exist: testbench_manifest.json'
        os.exit(0)
    else:
        print 'testbench_manifest.json found'

    # read current summary report, which contains the metrics
    result_json = {}
    with open('testbench_manifest.json','r') as file_in:
        result_json = json.load(file_in)
        print 'testbench_manifest.json loaded'

    # update metrics
    if 'Metrics' in result_json:
        print 'Metrics key found'
        metricsFound = False
        for metric in result_json['Metrics']:
            print 'Working on one metric key'
            metricsFound = True
            if 'Name' in metric and 'Value' in metric:
                print 'Name and Value keys do exist in the metric'
                if metric['Name'] == "ComplexityMetric":
                    print 'Metric with name ComplexityMetric found'
                    metric['Value'] = str(complexity_value)
                else:
                    print 'ERROR! Metric with name ComplexityMetric could not be found'
            else:
                print 'ERROR! Name and Value keys do not exist in the metric'
        if metricsFound == False:
            print 'ERROR! No metrics found in the summary report json file'
    else:
        print 'ERROR! Metrics key not found in the summary report json file'

    # save updated metrics in the summary report json file
    with open('testbench_manifest.json','wb') as file_out:
        json.dump(result_json, file_out)

# computes complexity and returns an array
# sampleSize = number of random samples
# 
def compute_complexity(fname_component, fname_interface,sampleSize=200):
    #alpha_samples is an array of length sampleSize containing the sum of the component complexities \sum_{i=1}^n \alpha_i
    #Alpha is essentially the raw data read from the file
    [Alpha,alpha_samples] = component_complexity(fname_component,sampleSize=sampleSize)
    #print "Alpha", alpha_array

    beta_samples = interface_complexity(fname_interface, Alpha,sampleSize=sampleSize)
    #print "Beta", beta_array
    
    samples = alpha_samples+beta_samples
    if min(samples)==max(samples):
        update_complexity_metrics(samples[0:1][0])
        return samples[0:1]    #if there are no distributions, return a scalar only

    #otherwise return the mean and the samples
    out = np.zeros(sampleSize+1)  #output line contains mean and separate values
    out[0] = sum(samples)/len(samples)
    out[1:] = samples
    return out



# needed for batch mode
# looks in 'directory' for all filenames in the form
# prefixX where X stands for up to 4 alphanumeric digits
# returns them as a list
def locate_files(directory, prefix):
    out = []
    for fname in os.listdir(directory):
        # use built-in regular expression
        if re.match(prefix + '[0-9A-Za-z]{0,3}.csv', fname):
            out.append(fname)
    return out



# writes results into a csv file
# an array that is either length 2 or 10
def save_results(fname, results):
    f = open(fname, 'w')
    for row in results:
        str_arr = [str(x) for x in row.tolist()]
        f.write(','.join(str_arr))
        f.write('\n')
    f.close()


def batch_mode(directory='',sampleSize=200):
    comp_files = locate_files(directory,'component_complexity')
    interf_files = locate_files(directory,'interface_complexity')

    results = []
    
    for fname in comp_files:
        suffix = fname[len('componentcomplexity')+1:]
        fname1 = directory+'/'+'component_complexity'+suffix
        fname2 = directory+'/'+'interface_complexity'+suffix

        print "\n\nProcessing file "+fname1+' and '+fname2
        X=compute_complexity(fname1, fname2, sampleSize)
            
        print "Mean and Samples:\n",X
        results.append(X)
        
    save_results('results.csv',results)    






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
        print compute_complexity(args[0], args[1], int(args[2]))
    else:
        batch_mode(directory='complexity',sampleSize=200)

    return    


if __name__ == "__main__":
    main()

