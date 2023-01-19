import os
import sys
import csv
import json
import itertools
from six.moves import zip


def exitwithcode(code):
    print code
    exit(code)


def ParseArgs():
    infilepath = None
    checkfilepath = None
    argList = sys.argv
    argc = len(argList)
    i = 0
    while i < argc:
        if argList[i][:2] == '-i':
            i += 1
            infilepath = argList[i]
        elif argList[i][:2] == '-c':
            i += 1
            checkfilepath = os.path.join(os.getcwd(), argList[i])
        i += 1

    if not any([infilepath, checkfilepath]):
        exitwithcode(-1)

    return infilepath, checkfilepath
    

def check_csv(test, reference):
    with open(test, 'rb') as testcsv, open(reference, 'rb') as referencecsv:
        test_reader = csv.reader(testcsv)
        ref_reader = csv.reader(referencecsv)
        for lhs, rhs in zip(test_reader, ref_reader):
            if lhs != rhs:
                exitwithcode(-1)

                
if __name__ == '__main__':

    checker_functions = {'csv': check_csv, 'xml': 'check_xml', 'json': 'check_json'}

    infile, checkfile = ParseArgs()
    if not any([os.path.exists(infile), os.path.exists(checkfile)]):
        exitwithcode(-1)
    checker_functions[infile.rsplit('.')[1].lower()](infile, checkfile)
    exitwithcode(0)