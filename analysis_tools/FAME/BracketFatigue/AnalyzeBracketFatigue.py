import argparse
import sys
import os
import math
from collections import OrderedDict
import json
import csv

# Test this with "python -m doctest -v AnalyzeBracketFatigue.py"

#from optparse import OptionParser


# Input need to be following range
#Steel: 0.4" < ta  < 0.8" and  0.15" < tg < 0.35"
#Aluminum: 0.7" < ta  < 0.9" and 0.5" < tg < 0.7"

# constant comes from Mission assumption and simulation to develop damage parameter map
MilesInDamageParameterMap = 28800.0
MilesPerMission = 116.0
criticalDamage = 1.0

# flag used to be in command line input
verbFlag = 0
failureProb = 0.01

# setting up the path for data directory
current_dir = os.path.dirname(__file__)
data_filepath = os.path.join(current_dir, "Data")

def error(msg):
    "Output an error message"
    
    sys.stderr = open('_FAILED.txt', 'w')
    sys.stderr.write("Error running Bracket Fatigue Analysis\n")
    sys.stderr.write(msg + "\n")
    sys.stderr.close()


# function to interpolate:
def interp1d(x_values, y_values, x,logFlag):
    indx = 0
    for i in range(len(x_values)):
        if x_values[i] < x:
            indx = indx + 1
        else:
            break
    if indx > 0:
        indx = indx - 1 #index if off by 1 because count start of 1 rather than 0
    if logFlag:
        try:
            y_val0 = math.log(y_values[indx])
            y_val1 = math.log(y_values[indx+1])
            y_interp = y_val0+(x- x_values[indx])*(y_val1-y_val0)/(x_values[indx+1]-x_values[indx]) 
            y_interp =math.exp(y_interp) 
        except:
            y_val0 = math.log(y_values[indx-1])
            y_val1 = math.log(y_values[indx])
            y_interp = y_val0+(x- x_values[indx])*(y_val1-y_val0)/(x_values[indx+1]-x_values[indx]) 
            y_interp =math.exp(y_interp) 
    else:
        try:
            y_interp = y_values[indx]+(x- x_values[indx])*(y_values[indx+1]-y_values[indx])/(x_values[indx+1]-x_values[indx])  
        except:
            y_interp = y_values[indx-1]+(x- x_values[indx-1])*(y_values[indx]-y_values[indx-1])/(x_values[indx]-x_values[indx-1])
    return y_interp

# function of compute ASF value given material, size, angle thickness, and gasset thickness
def computeASF(material,size,ta,tg):
    # equations come from ACEI and BAE.
    if material == 1:
        if size == 1:
            ASFa = 3.6809*math.pow(ta,2)+6.5981*ta-1.4364
            ASFg = 0.8826*math.pow(ta,2)+2.4154*ta-14.684*math.pow(tg,2)+12.309*tg-1.1335
        elif size == 2:
            ASFa = 8.3978*math.pow(ta,2)+2.1907*ta-0.3430
            ASFg = -1.2146*math.pow(ta,2)+5.2028*ta-9.9946*math.pow(tg,2)+9.5069*tg-1.3816
        elif size == 3:
            ASFa = 9.7941*math.pow(ta,2)+0.9701*ta-0.0174
            ASFg = -2.5429*math.pow(ta,2)+7.2021*ta-11.855*math.pow(tg,2)+10.758*tg-2.0322
        else:
            error("unknown size class %s" % (size,))
            return None
    elif material == 2:
        if size == 1:
            ASFa = 2.2602*math.pow(ta,2)-0.5297*ta-0.3439
            ASFg = 0.3465*math.pow(ta,2)+1.0904*ta-0.5426*math.pow(tg,2)+1.3467*tg-0.4674
        elif size == 2:
            ASFa = 2.1064*math.pow(ta,2)-0.3078*ta+0.3265
            ASFg = -0.5988*math.pow(ta,2)+2.7302*ta-0.1807*math.pow(tg,2)+1.0464*tg-0.9423
        elif size == 3:
            ASFa = 2.2189*math.pow(ta,2)-0.4252*ta+0.4228
            ASFg = -0.4846*math.pow(ta,2)+3.2506*ta-0.0997*math.pow(tg,2)+0.9813*tg-1.2962
        else:
            error("unknown size class %s" % (size,))
            return None
    else:
        error("unknown material type %s" % (material,))
        return None

    ASF = min([ASFa,ASFg])
    return ASF
  
# function of compute probability of failure given ASF value and material
def computeProbFail(material,ASF):

    global criticalDamage
    
    # loading list of probability of failures
    probFailListFileName = os.path.join(data_filepath, "probFailureList.csv")
    probFailListFile = open(probFailListFileName,"rbU")
    probFailList = []
    for line in probFailListFile:
        probFailList.append(float(line))
            
    # loading list of ASF values corresponding particular material
    if material == 1:
        ASFListFileName = os.path.join(data_filepath, "ASF_Steel.csv")
    elif material == 2:
        ASFListFileName = os.path.join(data_filepath, "ASF_Alum.csv")
    else:
        raise RuntimeError("Unknown material code " + str(material))
    ASFListFile = open(ASFListFileName,"rbU")
    ASFList = []
    for line in ASFListFile:
        ASFList.append(float(line))
    
    """
    if ASFList[0] > ASF:
        print ASF        
        sys.stderr.write("ASF value is too small\n")
        return "None"
    
    elif ASFList[-1] < ASF:
        sys.stderr.write("ASF value is too large\n")
        return "None"
    """
    
    # loading list of Damage Data corresponding particular material
    if material == 1:
        DataFileName = os.path.join(data_filepath, "Bracket_LCF_17-4PH900.csv")
    elif material == 2:
        DataFileName = os.path.join(data_filepath, "Bracket_LCF_Al6061T6.csv")
    DataFile = open(DataFileName,"rbU")
    csvDataReader = csv.reader(DataFile,delimiter=",")
    damageList = []
    for dataLine in csvDataReader:
        for i in range(len(dataLine)):
            dataLine[i] = float(dataLine[i])
        fval = interp1d(ASFList, dataLine,ASF,1)
        damageList.append(fval)
    
    if min(damageList) > 1.0:
        return 1.0
    elif max(damageList) < 1.0:
        return 0.0001
    probFailureVal = interp1d(damageList[::-1],probFailList[::-1],1.0,0)
    return probFailureVal
        

# function to put the result into json format  
def output(perfVal,failureProb,ta,tg,material,size):

    # creating ordered dictionary to be outputted in testbench.json format
    data = OrderedDict()
    data["$id"] = "1"
    data["Name"] = "Bracket_Fatigue"
    
    MetricDict = OrderedDict()
    MetricDict["$id"] = "2"
    MetricDict["Requirement"] = failureProb
    MetricDict["Name"] = "Bracket_Fatigue"
    MetricDict["Unit"] = "probability"
    MetricDict["Value"] = perfVal
    data["Metric"] = [MetricDict]
    
    ParamDict1 = OrderedDict()
    ParamDict1["$id"] = "5"
    ParamDict1["Name"] = "Angular Thickness (Ta)"
    ParamDict1["Unit"] = "mm"
    ParamDict1["Value"] = ta
    
    ParamDict2 = OrderedDict()
    ParamDict2["$id"] = "6"
    ParamDict2["Name"] = "Gusset Thickness (Tg)"
    ParamDict2["Unit"] = "mm"
    ParamDict2["Value"] = tg
    
    ParamDict3 = OrderedDict()
    ParamDict3["$id"] = "3"
    ParamDict3["Name"] = "Material"
    ParamDict3["Unit"] = ""
    if int(material) == 1:
        ParamDict3["Value"] = "Steel 17-4PH "
    if int(material) == 2:
        ParamDict3["Value"] = "Aluminum 6061-T6"
    
    ParamDict4 = OrderedDict()
    ParamDict4["$id"] = "4"
    ParamDict4["Name"] = "Size"
    ParamDict4["Unit"] = "mm"
    if int(size) == 1:
        ParamDict4["Value"] = '76.2 x 76.2 x 76.2'
    if int(size) == 2:
        ParamDict4["Value"] = '101.6 x 101.6 x 101.6'
    if int(size) == 3:
        ParamDict4["Value"] = '127 x 127 x 127'
    data["Parameters"] = [ParamDict3,ParamDict4,ParamDict1,ParamDict2]
    data["Requirement"] = []
    with open('Bracket_Fatigue.testbench.json','w') as outfile:
        json.dump(data,outfile, indent=2,sort_keys=False)
    
def main():

    parser = argparse.ArgumentParser(description='Determine Probability of Failrue caused by Bracket Fatigue given Bracket Design in json file')
    parser.add_argument("InputJson",help="json file containing braket design information")
    args = parser.parse_args()
    InputFileName = os.path.abspath(args.InputJson)
    #InputFileName = os.path.join(current_dir, args.InputJson)
    try:
        InputFile = open(InputFileName,"rU")
        Input = json.load(InputFile)
    except:
        error("Couldn't open input json file " + InputFileName + "\n")
        return 1
        
    
    # Interpreting json file.
    if (Input["Material"].lower()).find("st") >= 0: 
        material = 1
    elif (Input["Material"].lower()).find("al") >= 0: 
        material = 2
    else:
        error("unknown material type")
        return 1
        
    try:   
        size = int(Input["Size"])
        if size < 1 or size > 3:
            error("size need to be integer [1,2,3]")
            return 1
    except:
        error("size need to be integer [1,2,3]")
        return 1
    
    try:
        ta = float(Input["Angular Thickness"])
    except:
        error("Angular Thickness need to be float")
        return 1
    
    try:
        tg = float(Input["Gusset Thickness"])
    except:
        error("Gusset Thickness need to be float")
        return 1
    
    if (material == 1 and ta < 10.16) or (material == 2 and ta < 17.78):
        error("ta is lower than lower bound\n")
        return 1      

    elif (material == 1 and ta > 20.32) or (material == 2 and ta > 22.86):
        error("ta is greater than upper bound")
        return 1

    elif (material == 1 and tg < 3.81) or (material == 2 and tg < 12.7):
        error("tg is lower than lower bound")
        return 1

    elif (material == 1 and tg > 8.89) or (material == 2 and tg > 17.78):
        error("tg is greater than upper bound")
        return 1
     
    ASF = computeASF(material,size,ta/25.4,tg/25.4)
    if verbFlag:
        print "ASF is: ", ASF
    if ASF is None:
        return 1

    probFailure = computeProbFail(material,ASF)
    if verbFlag:
        print "probFailure is: ", probFailure
    
    output(probFailure,failureProb,ta,tg,material,size)
    return 0

def test(material, size, ta, tg):
    """
    >>> test('Steel', 1, 10.16, 3.81)
    1.489836 0.922342
    >>> test('Steel', 1, 20.32, 8.89)
    3.873044 0.000176
    >>> test('Steel', 1, 11, 4)
    1.652333 0.794925
    >>> test('Steel', 1, 20, 8.5)
    3.790329 0.000224
    >>> test('Steel', 1, 10.16, 8.89)
    1.791784 0.621595
    >>> test('Steel', 1, 20.32, 3.81)
    2.879644 0.007641
    >>> test('Aluminum', 1, 17.78, 12.7)
    0.392808 0.696768
    >>> test('Aluminum', 1, 22.86, 17.78)
    1.010132 0.038028
    >>> test('Aluminum', 1, 17.78, 17.78)
    0.392808 0.696768
    >>> test('Aluminum', 1, 22.86, 12.7)
    1.010132 0.038028
    >>> test('Aluminum', 1, 20, 15)
    0.64034 0.097872
    >>> test('Steel', 2, 10.16, 3.81)
    1.706341 0.732438
    >>> test('Steel', 2, 20.32, 8.89)
    4.106372 0.000101
    >>> test('Steel', 2, 11, 4)
    1.893065 0.18994
    >>> test('Steel', 2, 20, 8.5)
    4.02421 0.000121
    >>> test('Steel', 2, 10.16, 8.89)
    1.876928 0.192067
    >>> test('Steel', 2, 20.32, 3.81)
    3.204452 0.001848
    >>> test('Aluminum', 2, 17.78, 12.7)
    1.143176 0.011416
    >>> test('Aluminum', 2, 22.86, 17.78)
    1.673789 0.0001
    >>> test('Aluminum', 2, 17.78, 17.78)
    1.143176 0.011416
    >>> test('Aluminum', 2, 22.86, 12.7)
    1.507877 0.000274
    >>> test('Aluminum', 2, 20, 15)
    1.390108 0.000839
    >>> test('Steel', 3, 10.16, 3.81)
    1.788739 0.625942
    >>> test('Steel', 3, 20.32, 8.89)
    4.415086 0.0001
    >>> test('Steel', 3, 11, 4)
    2.010066 0.170759
    >>> test('Steel', 3, 20, 8.5)
    4.334647 0.0001
    >>> test('Steel', 3, 10.16, 8.89)
    1.937696 0.183342
    >>> test('Steel', 3, 20.32, 3.81)
    3.448986 0.000819
    >>> test('Aluminum', 3, 17.78, 12.7)
    1.207491 0.005915
    >>> test('Aluminum', 3, 22.86, 17.78)
    1.837429 0.0001
    >>> test('Aluminum', 3, 17.78, 17.78)
    1.212421 0.005649
    >>> test('Aluminum', 3, 22.86, 12.7)
    1.702539 0.0001
    >>> test('Aluminum', 3, 20, 15)
    1.463718 0.000416
    """

    mat = {"Steel": 1, "Aluminum": 2}.get(material)
    ASF = computeASF(mat, size, ta/25.4, tg/25.4)
    # we round to 6 significant digits here to avoid floating-point mismatches
    print round(ASF, 6), round(computeProbFail(mat, ASF), 6)

def testJSON(material, size, ta, tg):
    """
    >>> testJSON('Steel', 1, 10.16, 3.81)
    0.922342
    >>> testJSON('Steel', 1, 20.32, 8.89)
    0.000176
    >>> testJSON('Steel', 1, 11, 4)
    0.794925
    >>> testJSON('Steel', 1, 20, 8.5)
    0.000224
    >>> testJSON('Steel', 1, 10.16, 8.89)
    0.621595
    >>> testJSON('Steel', 1, 20.32, 3.81)
    0.007641
    >>> testJSON('Aluminum', 1, 17.78, 12.7)
    0.696768
    >>> testJSON('Aluminum', 1, 22.86, 17.78)
    0.038028
    >>> testJSON('Aluminum', 1, 17.78, 17.78)
    0.696768
    >>> testJSON('Aluminum', 1, 22.86, 12.7)
    0.038028
    >>> testJSON('Aluminum', 1, 20, 15)
    0.097872
    >>> testJSON('Steel', 2, 10.16, 3.81)
    0.732438
    >>> testJSON('Steel', 2, 20.32, 8.89)
    0.000101
    >>> testJSON('Steel', 2, 11, 4)
    0.18994
    >>> testJSON('Steel', 2, 20, 8.5)
    0.000121
    >>> testJSON('Steel', 2, 10.16, 8.89)
    0.192067
    >>> testJSON('Steel', 2, 20.32, 3.81)
    0.001848
    >>> testJSON('Aluminum', 2, 17.78, 12.7)
    0.011416
    >>> testJSON('Aluminum', 2, 22.86, 17.78)
    0.0001
    >>> testJSON('Aluminum', 2, 17.78, 17.78)
    0.011416
    >>> testJSON('Aluminum', 2, 22.86, 12.7)
    0.000274
    >>> testJSON('Aluminum', 2, 20, 15)
    0.000839
    >>> testJSON('Steel', 3, 10.16, 3.81)
    0.625942
    >>> testJSON('Steel', 3, 20.32, 8.89)
    0.0001
    >>> testJSON('Steel', 3, 11, 4)
    0.170759
    >>> testJSON('Steel', 3, 20, 8.5)
    0.0001
    >>> testJSON('Steel', 3, 10.16, 8.89)
    0.183342
    >>> testJSON('Steel', 3, 20.32, 3.81)
    0.000819
    >>> testJSON('Aluminum', 3, 17.78, 12.7)
    0.005915
    >>> testJSON('Aluminum', 3, 22.86, 17.78)
    0.0001
    >>> testJSON('Aluminum', 3, 17.78, 17.78)
    0.005649
    >>> testJSON('Aluminum', 3, 22.86, 12.7)
    0.0001
    >>> testJSON('Aluminum', 3, 20, 15)
    0.000416
    """

    import tempfile, subprocess, os, json

    fname = tempfile.mktemp(suffix=".json")
    try:
        fp = open(fname, "w")
        json.dump({
            "Material": material,
            "Size": size,
            "Angular Thickness": ta,
            "Gusset Thickness": tg,
            }, fp)
        fp.close()
        if os.path.exists("Bracket_Fatigue.testbench.json"):
            os.unlink("Bracket_Fatigue.testbench.json")
        subprocess.call(['python', 'AnalyzeBracketFatigue.py', fname])
        if not os.path.exists("Bracket_Fatigue.testbench.json"):
            return None
        results = json.load(open("Bracket_Fatigue.testbench.json"))
        # we round to 6 significant digits here to avoid floating-point mismatches
        print round(results["Metric"][0]["Value"], 6)
        if os.path.exists("Bracket_Fatigue.testbench.json"):
            os.unlink("Bracket_Fatigue.testbench.json")
    finally:
        if os.path.exists(fname):
            os.unlink(fname)

if __name__ == "__main__":
    main()
