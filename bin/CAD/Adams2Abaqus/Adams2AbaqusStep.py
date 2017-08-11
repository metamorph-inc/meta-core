"""

Adams2AbaqusMain.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - Includes modules which manupilate and parse
      the STEP(CAD) file.

"""

import abaqus
from abaqusConstants import *
import caeModules, jobMessage, odbAccess
import interaction, visualization, regionToolset, job, mesh

import csv
import glob
import math, re, sys, json, os
import operator
from optparse import OptionParser
import shutil
import string as STR
import traceback
import time
import uuid, ctypes
import xml.etree.ElementTree as ET
import _winreg

from Adams2AbaqusGeometry import *

MAIN = os.getcwd()                                                           # initial working directory
LOGDIR = os.path.join(MAIN, "log", "CyPhy2AbaqusCmd.log")                    # path to log file

def processStep(stepPath, testBenchName):
    f_p = open(LOGDIR, "w")

    # generate datum points from STEP raw data
    f_p.write("Generating datum points from STEP raw data" + '\n')
    try:
        wtl = []                                    # whole text list
        ld = {}                                     # list index dictionary
        a = re.compile('#\d+(?==)')                 # regular expression a gets portion of line left of the = sign, e.g. "#4507"
        b = re.compile('(?<==).+;')                 # regular expression b gets portion of line right of the = sign
        lineB = ''                                  # placeholder for concatenated string
        addLine = False                             # initial setting for string concatenation mode
        i = 0
        with open(stepPath, 'rb') as stepData:
            for line in stepData:
                starts = a.search(line)
                stops = re.search(';',line)
                if not addLine and not starts:              # case 1: header
                    continue                                    # goto next line
                elif not addLine and stops:                 # case 2: '#num=...;'
                    lineB = line                                # save current line
                elif not addLine:                           # case 3: '#num=...\n'
                    lineB = line                                # save current line
                    lineB = lineB.strip('\n')                   # strip standard newline char
                    lineB = lineB.strip('\r')                   # strip Mac-style newline char
                    addLine = True                              # start adding lines
                    continue                                    # goto next line
                elif not stops:                             # case 4: '...\n'
                    lineB += line                               # add current line to previous line(s)
                    lineB = lineB.strip('\n')                   # strip standard newline char
                    lineB = lineB.strip('\r')                   # strip Mac-style newline char
                    continue                                    # goto next line
                else:                                       # case 5: '...;'
                    lineB += line                               # add current line to previous line(s)
                    addLine = False                             # stop adding lines
                aResult = a.search(lineB)                   # obtain pointer string
                bResult = b.search(lineB)                   # obtain value string
                key = aResult.group(0)
                data = bResult.group(0)
                wtl.append(data)
                ld.update([[key,i]])
                i += 1
        c = re.compile('(?<!\s)#\d+')
        d = re.compile('(\w+) with respect to (\w+)')
        localTMs = {}                               # dict of local transformation matrices
        localTVs = {}                               # dict of local translation vectors
        subAsms = {}                                # dict of sub-assemblies by parent assembly
        asmParts = {}                               # dict of parts by direct parent assembly
        i = 1
        inst2SR = {}
        pointsBySR = {}
        topLevelAsm = STR.upper(testBenchName) + '_ASM'
        for data in wtl:
            if 'CONTEXT_DEPENDENT_SHAPE_REPRESENTATION' in data:
                cdsrRE = c.findall(data)                    # grab all STEP pointers in CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
                rr = wtl[ld[cdsrRE[0]]]                     # find REPRESENTATION_RELATIONSHIP
                pds = wtl[ld[cdsrRE[1]]]                    # find PRODUCT_DEFINITION_SHAPE
                pdsRE = c.findall(pds)                      # grab all STEP pointers in PRODUCT_DEFINITION_SHAPE
                pdsRE2 = d.search(pds)                      # grab names of component and direct parent assembly
                if pdsRE2.group(1)[-4:] == '_ASM':          # if component is a subassembly:
                    if pdsRE2.group(2) in subAsms:              # and if parent assembly has entry in subAsms:
                        subAsms[pdsRE2.group(2)].append(
                            pdsRE2.group(1))                        # add subassembly to subAsms[parent]
                    else:                                       # if parent has no entry in subAsms yet:
                        subAsms.update([[pdsRE2.group(2),
                            [pdsRE2.group(1)]]])                    # create subAsms[parent]
                elif 'ZONE' not in pdsRE2.group(1):
                    if pdsRE2.group(2) in asmParts:
                        asmParts[pdsRE2.group(2)].append(pdsRE2.group(1))
                    else:
                        asmParts.update([[pdsRE2.group(2),[pdsRE2.group(1)]]])
                rrRE = c.findall(rr)                        # grab all STEP pointers in REPRESENTATION_RELATIONSHIP
                idt = wtl[ld[rrRE[2]]]                      # find ITEM_DEFINED_TRANSFORMATION
                idtRE = c.findall(idt)                      # grab all STEP pointers in ITEM_DEFINED_TRANSFORMATION
                inst2SR.update([[pdsRE2.group(1),rrRE[0]]]) # store SHAPE_RELATIONSHIP pointer, indexed by part/asm name
                axisA = wtl[ld[idtRE[0]]]                   # find first AXIS2_PLACEMENT_3D
                axisB = wtl[ld[idtRE[1]]]                   # find second AXIS2_PLACEMENT_3D
                axisARE = c.findall(axisA)                  # grab all STEP pointers in first AXIS2_PLACEMENT_3D
                axisBRE = c.findall(axisB)                  # grab all STEP pointers in second AXIS2_PLACEMENT_3D
                originA = coordinate(regexFriendly(
                    wtl[ld[axisARE[0]]]))                   # get CARTESIAN_POINT of axisA
                dirA1 = coordinate(regexFriendly(
                    wtl[ld[axisARE[1]]]))                   # get first DIRECTION of axisA
                dirA2 = coordinate(regexFriendly(
                    wtl[ld[axisARE[2]]]))                   # get second DIRECTION of axisA
                originB = coordinate(regexFriendly(
                    wtl[ld[axisBRE[0]]]))                   # get CARTESIAN_POINT of axisB
                dirB1 = coordinate(regexFriendly(
                    wtl[ld[axisBRE[1]]]))                   # get first DIRECTION of axisB
                dirB2 = coordinate(regexFriendly(
                    wtl[ld[axisBRE[2]]]))                   # get second DIRECTION of axisB
                localTMs.update([[pdsRE2.group(1),get3DTransformArray(
                    dirA1,dirA2,dirB1,dirB2)]])             # create transformation matrix from axisA to axisB
                localTVs.update([[
                    pdsRE2.group(1),originB]])              # create translation vector from axisA to axisB
            elif 'CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP' in data:
                cgrrRE = c.findall(data)                    # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP
                sr = cgrrRE[0]                              # STEP pointer for SHAPE_REPRESENTATION
                pointsBySR.update([[sr,{}]])                # new dict entry to store datums by SHAPE_REPRESENTATION pointer
                cgr = wtl[ld[cgrrRE[1]]]                    # get CONSTRUCTIVE_GEOMETRY_REPRESENTATION
                cgrRE = c.findall(cgr)                      # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION
                for datum in cgrRE[:-1]:                    # for each STEP pointer in CGR, excluding last one:
                    datumline = wtl[ld[datum]]                  # get datum entry at pointer
                    if 'CARTESIAN_POINT' in datumline:          # if datum entry is CARTESIAN_POINT:
                        regexable = regexFriendly(datumline)        # replace/remove problem characters in line
                        tagFind = re.search('%.+%',regexable)       # get name of datum
                        tag = tagFind.group(0).strip('%')           # remove % characters
                        coords = coordinate(regexable)              # get coordinate tuple
                        pointsBySR[sr].update([[tag,coords]])       # record datum info in SHAPE_REPRESENTATION dict

    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error during parsing the STEP file\n')
        f_p.write('       Probably due to corrupted or erronoeus STEP raw data\n')
        raise 

    f_p.write('\n')
    f_p.write("**********************************************************************************" + '\n')
    f_p.write('\n')

    f_p.close()

    return (pointsBySR, inst2SR, localTMs, localTVs, topLevelAsm, subAsms, asmParts)

def generateLocalCoords(inst2SR, pointsBySR):
    f_p = open(LOGDIR, "w")

    # new dict to index points by part name, rather than by SHAPE_REPRESENTATION pointer
    localCoords = {}
    try:
        for (key,value) in inst2SR.iteritems():
            localCoords.update([[key,pointsBySR[value]]])
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error in creating the dictionary to index points by part name\n')
        f_p.write('       Probably due to corrupted or erronoeus STEP file\n')
        raise

    f_p.close()

    return (localCoords)    
