"""

AbaqusSTEP.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which manupilate and parse
      the STEP(CAD) file.

"""

import re
import string as STR
import logging
import cad_library
from AbaqusGeometry import *


def processStep(stepPath, testBenchName, uniqueSuffix, asmIdentifier):
    # Generate datum points from STEP raw data
    logger = logging.getLogger()
    logger.info("Generating datum points from STEP raw data" + '\n')
    localTMs = {}                               # dict of local transformation matrices
    localTVs = {}                               # dict of local translation vectors
    subAsms = {}                                # dict of sub-assemblies by parent assembly
    asmParts = {}                               # dict of parts by direct parent assembly
    inst2SR = {}
    pointsBySR = {}

    wtl = []                           # whole text list
    ld = {}                            # list index dictionary
    a = re.compile('#\d+(?==)')        # regular expression a gets portion of line left of the = sign, e.g. "#4507"
    b = re.compile('(?<==).+;')        # regular expression b gets portion of line right of the = sign
    lineB = ''                         # placeholder for concatenated string
    addLine = False                    # initial setting for string concatenation mode
    i = 0
    with open(stepPath, 'rb') as stepData:
        for line in stepData:
            starts = a.search(line)
            stops = re.search(';', line)
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
            ld.update([[key, i]])
            i += 1
    c = re.compile('(?<!\s)#\d+')
    d = re.compile('(\w+) with respect to (\w+)')

    topLevelAsm = STR.upper(testBenchName) + asmIdentifier
    instance_num = 0
    prev_parent = None
    logger.info('STEP File import: ')
    logger.info('CHILD' + '                         ' + 'PARENT')
    for data in wtl:
        if 'CONTEXT_DEPENDENT_SHAPE_REPRESENTATION' in data:
            instance_num += 1
            cdsrRE = c.findall(data)              # grab all STEP pointers in CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
            rr = wtl[ld[cdsrRE[0]]]               # find REPRESENTATION_RELATIONSHIP
            pds = wtl[ld[cdsrRE[1]]]              # find PRODUCT_DEFINITION_SHAPE
            pdsRE2 = d.search(pds)                # grab names of component and direct parent assembly

            logger.info(str(pdsRE2.group(1) + '                         ' + str(pdsRE2.group(2))))

            if pdsRE2.group(1)[-4:] == asmIdentifier:    # if component is a subassembly:
                # If parent of ASM is TLA, then all children of subassembly have been processed.
                # Add subAssembly to asmParts, but now replace non-unique subAssembly name with
                # instance number identifier.
                if str(pdsRE2.group(2)) == topLevelAsm:
                    if topLevelAsm in subAsms:
                        # create subAsms[parent]
                        subAsms[pdsRE2.group(2)].append(str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)))
                        if str(pdsRE2.group(1)) in subAsms:
                            subAsms[str(pdsRE2.group(1)+ uniqueSuffix + str(instance_num))] = subAsms.pop(prev_parent)
                    else:
                        subAsms.update([[pdsRE2.group(2), [str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))]]])
                        if str(pdsRE2.group(1)) in subAsms:
                            subAsms[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))] = subAsms.pop(prev_parent)
                        if prev_parent in asmParts:
                            asmParts[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))] = asmParts.pop(prev_parent)
                        else:
                            pass
                else:
                    # If parent is not the TLA, then a grandchild sub-assembly has ended, with the parent being
                    # the next level up's subassembly name. Do not update subAsms with a unique name yet.
                    if str(pdsRE2.group(1)) in subAsms:
                            subAsms[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))] = subAsms.pop(prev_parent)
                    if str(pdsRE2.group(2)) in subAsms:
                        subAsms[str(pdsRE2.group(2))].append(str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)))
                    else:
                        subAsms.update([[str(pdsRE2.group(2)), [str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))]]])

                    # If child is a sub-assembly whose only children is another subassembly, then
                    # there will be no entry in asmParts to modify.
                    if prev_parent in asmParts:
                        asmParts[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))] = asmParts.pop(prev_parent)
                    else:
                        pass

            elif 'ZONE' not in pdsRE2.group(1):
                if instance_num == 1:
                    # If parent is the top level assembly, current child is a part.
                    if str(pdsRE2.group(2)) == topLevelAsm:
                        asmParts.update([[str(pdsRE2.group(2)), [str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))]]])
                    else:
                        # If not, parent is a subassembly, and child is first component of that assembly
                        # asmParts key for now is a non-unique name to be renamed later.
                        asmParts.update([[str(pdsRE2.group(2)), [str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))]]])
                        prev_parent = str(pdsRE2.group(2))
                else:  # Similar pattern followed for both cases after first pass.
                    if str(pdsRE2.group(2)) == topLevelAsm:
                        asmParts[topLevelAsm].append(str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)))
                    elif str(pdsRE2.group(2)) == prev_parent:
                        asmParts[prev_parent].append(str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)))
                    else:
                        asmParts.update([[str(pdsRE2.group(2)), [str(pdsRE2.group(1) + uniqueSuffix + str(instance_num))]]])
                        prev_parent = str(pdsRE2.group(2))

            rrRE = c.findall(rr)                        # grab all STEP pointers in REPRESENTATION_RELATIONSHIP
            idt = wtl[ld[rrRE[2]]]                      # find ITEM_DEFINED_TRANSFORMATION
            idtRE = c.findall(idt)                      # grab all STEP pointers in ITEM_DEFINED_TRANSFORMATION

            # store SHAPE_RELATIONSHIP pointer, indexed by part/asm name
            inst2SR.update([[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)), rrRE[0]]])

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
                 # create transformation matrix from axisA to axisB
                localTMs.update([[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)),
                                  get3DTransformArray(dirA1, dirA2, dirB1, dirB2)]])
                # create translation vector from axisA to axisB
                localTVs.update([[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)), originB]])
            else:
                # create transformation matrix from axisA to axisB
                localTMs.update([[str(pdsRE2.group(1) + uniqueSuffix + str(instance_num)),
                                  get3DTransformArray(dirA1,dirA2,dirB1,dirB2)]])
                # create translation vector from axisA to axisB
                localTVs.update([[str(pdsRE2.group(1)+ uniqueSuffix + str(instance_num)), originB]])

            prev_parent = pdsRE2.group(2)

        elif 'CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP' in data:
            cgrrRE = c.findall(data)         # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION_RELATIONSHIP
            sr = cgrrRE[0]                   # STEP pointer for SHAPE_REPRESENTATION
            pointsBySR.update([[sr, {}]])    # new dict entry to store datums by SHAPE_REPRESENTATION pointer
            cgr = wtl[ld[cgrrRE[1]]]         # get CONSTRUCTIVE_GEOMETRY_REPRESENTATION
            cgrRE = c.findall(cgr)           # get STEP pointers in CONSTRUCTIVE_GEOMETRY_REPRESENTATION
            for datum in cgrRE[:-1]:         # for each STEP pointer in CGR, excluding last one:
                datumline = wtl[ld[datum]]                       # get datum entry at pointer
                if 'CARTESIAN_POINT' in datumline:               # if datum entry is CARTESIAN_POINT:
                    regexable = regexFriendly(datumline)         # replace/remove problem characters in line
                    tagFind = re.search('%.+%', regexable)       # get name of datum
                    tag = tagFind.group(0).strip('%')            # remove % characters
                    coords = coordinate(regexable)               # get coordinate tuple
                    pointsBySR[sr].update([[tag, coords]])       # record datum info in SHAPE_REPRESENTATION dict
    
    return pointsBySR, inst2SR, localTMs, localTVs, topLevelAsm, subAsms, asmParts
    
    
def generateLocalCoords(inst2SR, pointsBySR, asminfo, uniqueSuffix):
    # new dict to index points by part name, rather than by SHAPE_REPRESENTATION pointer
    localCoords = {}
    CGs = {}
    logger = logging.getLogger()
    try:
        for (key, value) in inst2SR.iteritems():
            localCoords.update([[key, pointsBySR[value]]])
    except IndexError:
        cad_library.exitwitherror('Error in creating the dictionary to index points by part name - ' +
                                  'Probably due to corrupted or erronoeus STEP file', -1, 'AbaqusSTEP.py')

    # The step order needs to be increased by at least 1 to account for the fact that the step order
    # starts at 0 and not 1.
    # If root parent has only one child, then the Creo assembly is an assembly with only 1 immediate child.
    # In this case, the top level parent in the metrics file will not match the top level in the Creo file.
    # ie, test bench name != creo parent name. Increase the step order by 2 in this case.
    for child in asminfo.componentsdict.values():
        if child.cyphyid == asminfo.root.cyphyid:
            continue  # Ignore root entry
        if not child.is_assembly():
            componentName = child.cadmodelname.upper() + uniqueSuffix + str(child.steporder + 1)
            CG = child.centerofgravity
            CGs.update([[componentName, (CG[0], CG[1], CG[2])]])
        else:
            pass

        for (key, value) in CGs.iteritems():
            localCoords[key].update([['CG_'+key, value]])

    return localCoords, CGs