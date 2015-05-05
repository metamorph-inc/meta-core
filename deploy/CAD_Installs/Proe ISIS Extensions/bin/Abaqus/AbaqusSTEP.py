"""

AbaqusSTEP.py, version 1.3.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which manupilate and parse
      the STEP(CAD) file.

"""

import re, sys, os
import string as STR
import traceback
import logging
from AbaqusGeometry import *


def processStep(stepPath, testBenchName):
    # Generate datum points from STEP raw data
    logger = logging.getLogger()
    logger.info("Generating datum points from STEP raw data" + '\n')
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
        instance_num = 0
        prev_parent = None
        bump = False
        bump_down = False
        for data in wtl:
            if 'CONTEXT_DEPENDENT_SHAPE_REPRESENTATION' in data:
                instance_num += 1
                cdsrRE = c.findall(data)                    # grab all STEP pointers in CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
                rr = wtl[ld[cdsrRE[0]]]                     # find REPRESENTATION_RELATIONSHIP
                pds = wtl[ld[cdsrRE[1]]]                    # find PRODUCT_DEFINITION_SHAPE
                pdsRE = c.findall(pds)                      # grab all STEP pointers in PRODUCT_DEFINITION_SHAPE
                pdsRE2 = d.search(pds)                      # grab names of component and direct parent assembly
                if pdsRE2.group(1)[-4:] == '_ASM':          # if component is a subassembly:
                    # THIS CODE SHOULD NEVER HIT IN DEPTH FIRST
                    # if pdsRE2.group(2) in subAsms:              # and if parent assembly has entry in subAsms:
                        # subAsms[pdsRE2.group(2)].append(
                            # str(pdsRE2.group(1)+'__Z'+str(instance_num)))                        # add subassembly to subAsms[parent]
                    # else:                                       # if parent has no entry in subAsms yet:
                    if str(pdsRE2.group(2)) == topLevelAsm:  
                        if topLevelAsm in subAsms:
                            subAsms[pdsRE2.group(2)].append(prev_par_rename)                    # create subAsms[parent]
                        else:
                            subAsms.update([[pdsRE2.group(2),[prev_par_rename]]])
                        bump_down = True  # don't increase instance_num as the subAsm has already been accounted for
                    else:
                        subAsms.update([[str(pdsRE2.group(2)+'__Z'+str(instance_num)),[prev_par_rename]]])
                        bump = True

                elif 'ZONE' not in pdsRE2.group(1):
                    if instance_num == 1:
                        if str(pdsRE2.group(2)) == topLevelAsm:
                            asmParts.update([[str(pdsRE2.group(2)),[str(pdsRE2.group(1)+'__Z' + str(instance_num))]]])
                        else:
                            asmParts.update([[str(pdsRE2.group(2)+'__Z1'),[str(pdsRE2.group(1)+'__Z' + str(instance_num+1))]]])
                            prev_par_rename = str(pdsRE2.group(2)+'__Z1')
                            bump = True
                    else:                                               # Similar pattern followed for both cases after first pass.
                        if str(pdsRE2.group(2)) == topLevelAsm:
                            asmParts[topLevelAsm].append(str(pdsRE2.group(1)+'__Z' + str(instance_num)))
                        elif str(pdsRE2.group(2)) == prev_parent:
                            asmParts[prev_par_rename].append(str(pdsRE2.group(1)+'__Z' + str(instance_num)))
                        else:
                            asmParts.update([[str(pdsRE2.group(2)+'__Z'+str(instance_num)),[str(pdsRE2.group(1)+'__Z' + str(instance_num+1))]]])
                            bump = True

                rrRE = c.findall(rr)                        # grab all STEP pointers in REPRESENTATION_RELATIONSHIP
                idt = wtl[ld[rrRE[2]]]                      # find ITEM_DEFINED_TRANSFORMATION
                idtRE = c.findall(idt)                      # grab all STEP pointers in ITEM_DEFINED_TRANSFORMATION
                if str(pdsRE2.group(1)) == prev_parent:
                    inst2SR.update([[prev_par_rename,rrRE[0]]]) # store SHAPE_RELATIONSHIP pointer, indexed by part/asm name
                else:
                    if bump == True:
                        inst2SR.update([[str(pdsRE2.group(1)+'__Z' + str(instance_num+1)),rrRE[0]]])
                    else:
                        inst2SR.update([[str(pdsRE2.group(1)+'__Z' + str(instance_num)),rrRE[0]]])
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
                if str(pdsRE2.group(1)) == prev_parent:
                    localTMs.update([[prev_par_rename,get3DTransformArray(
                        dirA1,dirA2,dirB1,dirB2)]])             # create transformation matrix from axisA to axisB
                    localTVs.update([[
                        prev_par_rename,originB]])              # create translation vector from axisA to axisB
                else:
                    if bump == True:
                        localTMs.update([[str(pdsRE2.group(1)+'__Z' + str(instance_num+1)),get3DTransformArray(
                            dirA1,dirA2,dirB1,dirB2)]])             # create transformation matrix from axisA to axisB
                        localTVs.update([[
                            str(pdsRE2.group(1)+'__Z' + str(instance_num+1)),originB]])              # create translation vector from axisA to axisB
                    else:
                        localTMs.update([[str(pdsRE2.group(1)+'__Z' + str(instance_num)),get3DTransformArray(
                            dirA1,dirA2,dirB1,dirB2)]])             # create transformation matrix from axisA to axisB
                        localTVs.update([[
                            str(pdsRE2.group(1)+'__Z' + str(instance_num)),originB]])              # create translation vector from axisA to axisB

                if str(pdsRE2.group(2)) == prev_parent or str(pdsRE2.group(2)) == topLevelAsm:
                    pass
                else:
                    prev_par_rename = str(pdsRE2.group(2)+'__Z'+str(instance_num))
                prev_parent = pdsRE2.group(2)
                
                if bump == True:
                    instance_num += 1
                    bump = False
                if bump_down == True:
                    instance_num -= 1
                    bump_down = False
                
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
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('ERROR: Error during parsing the STEP file - ' + \
                     '       Probably due to corrupted or erronoeus STEP raw data\n')
        raise 
    logger.info("**********************************************************************************" + '\n')
    
    return (pointsBySR, inst2SR, localTMs, localTVs, topLevelAsm, subAsms, asmParts)
    
    
def generateLocalCoords(inst2SR, pointsBySR, metricCADComponentsXML, metricComponentsXML):
    # new dict to index points by part name, rather than by SHAPE_REPRESENTATION pointer
    logger = logging.getLogger()
    localCoords = {}
    CGs = {}
    try:
        for (key,value) in inst2SR.iteritems():
            localCoords.update([[key,pointsBySR[value]]])  
    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in creating the dictionary to index points by part name - ' \
                     'Probably due to corrupted or erronoeus STEP file\n')
        raise   
    try:
        instance_num = 0
        for cadcomp in metricCADComponentsXML.findall('CADComponent'): 
            for component in metricComponentsXML.findall('MetricComponent'):
                if component.get('MetricID') == cadcomp.get('MetricID'):
                    instance_num += 1
                    inst = str('__Z' + str(instance_num))
                    if component.get('Type') == 'PART':
                        componentName = component.get('Name').upper() + inst    # This will be the same number as before as components are looped through in same order
                        centerOfGravity = component.find('CG')
                        CGX = float(centerOfGravity.get('X'))
                        CGY = float(centerOfGravity.get('Y'))
                        CGZ = float(centerOfGravity.get('Z'))
                        CGs.update([[componentName,(CGX, CGY, CGZ)]]) 
                else:
                    pass

        for (key,value) in CGs.iteritems():
            localCoords[key].update([['CG_'+key,value]])

    except Exception as e:
        logger.error(STR.join(traceback.format_exception(*sys.exc_info())))
        logger.error('Error in grabbing the CG of the parts from CADAssembly_metrics.xml file.\n')
        raise 

    return (localCoords, CGs) 