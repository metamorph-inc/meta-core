"""

AbaqusGeometry.py

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>
           Robert Boyles <rboyles@isis.vanderbilt.edu>

    - Includes modules which take care of geometrical operations
      in the part and assembly level.

"""

import re
import math
from numpy import array, cross, transpose, vstack, dot
from abaqusConstants import *
import numpy.linalg as LA
import string as STR


def regexFriendly(inString):
    """ Clean up coordinates read from STEP file, prior to applying regular expressions. """
    outString = STR.replace(inString, '\'', '%')
    outString = STR.replace(outString, '(', '')
    outString = STR.replace(outString, ')', ',')
    return outString


def coordinate(stepString):
    """ Extract tuple of cartesian coordinates from STEP coordinate string. """
    e = re.compile(',\S+,,')                                # regular expression
    coordFind = e.search(stepString)                        # extract substring containing coordinates
    coordList = coordFind.group(0).strip(',').split(',')    # separate x, y, and z coordinates by commas
    coords = (float(coordList[0]), float(coordList[1]),
              float(coordList[2]))                          # convert coordinate strings to a tuple of floats
    return coords                                           # return the coordinate tuple
     
     
# calculates transformation matrix between two coordinate systems as defined in STEP
def get3DTransformArray(fromDir1, fromDir2, toDir1, toDir2):
    """ Calculate transformation matrix between two coordinate systems as defined in STEP. """
    fromDir1 = array(fromDir1)                          # convert u1 vector to an array object
    fromDir2 = array(fromDir2)                          # convert u2 vector to an array object
    fromDir3 = cross(fromDir1, fromDir2)                 # extrapolate u3 vector from u1 and u2
    toDir1 = array(toDir1)                              # convert v1 vector to an array object
    toDir2 = array(toDir2)                              # convert v2 vector to an array object
    toDir3 = cross(toDir1, toDir2)                       # extrapolate v3 vector from v1 and v2
    inva = LA.inv(transpose(vstack([fromDir1, fromDir2, fromDir3])))
    b = transpose(vstack([toDir1, toDir2, toDir3]))
    transformArray = dot(b, inva)
    return transformArray


def unv(center, planarA, planarB):
    """ Use vector operations to get unit normal vector, given a center coordinate and two planar coordinates. """
    center = array(center)
    planarA = array(planarA)
    planarB = array(planarB)
    vA = planarA - center
    vB = planarB - center
    xV = cross(vA, vB)
    return xV/LA.norm(xV)


def transCoord(fromCoord, transformArray, translationVector):
    """ Transform/translate a cartesian point from one coordinate system to another. """
    vprod = dot(transformArray, fromCoord)
    vprod = vprod + translationVector
    toCoord = tuple(vprod)
    return toCoord


def asmRecursion(asm, subAsms, asmParts):
    """ Recursively identifies parts in sub-assemblies, in the order they are imported from STEP. """
    parts = []
    try:
        for child in subAsms[asm]:
            if child in subAsms:
                parts.extend(asmRecursion(child, subAsms, asmParts))
            else:
                parts.extend(asmParts[child])
    except KeyError:
        pass
    if asm in asmParts:
        parts.extend(asmParts[asm])
    return parts


def coordTransform(localTMs, localTVs, asm, subAsms, asmParts, localCoords):
    """
    Iterate through sub-assemblies and top-level parts to transform/translate
    every datum point to assembly coordinates; uses transCoord()
    Note: Ignores top-level datums in highest assembly, which will not exist
          in a CyPhy assembly anyway
    """
    globalCoords = {}                                               # create dictionary object to hold new point library
    if asm in subAsms:                                              # if assembly has sub-assemblies:
        for subAsm in subAsms[asm]:                                 # for each sub-assembly in the assembly:
            subCoords = coordTransform(localTMs, localTVs, subAsm,  # get point library local to sub-assembly
                                       subAsms, asmParts, localCoords)
            for part in subCoords.keys():                           # for each component in chosen sub-assembly:
                globalCoords.update([[part, {}]])                   # create new entry in globalCoords
                for (point, coord) in subCoords[part].iteritems():  # for each point in part/sub-sub-assembly:
                    globalCoords[part].update([[point.upper(), transCoord(  # translate/transform point to globalCoords
                        array(coord), localTMs[subAsm], localTVs[subAsm])]])
            globalCoords.update([[subAsm, {}]])                     # create entry for sub-assembly in globalCoords
            for (point, coord) in localCoords[subAsm].iteritems():
                # for each point specified at top level of that sub-assembly:
                globalCoords[subAsm].update([[point.upper(), transCoord(  # translate/transform point to globalCoords
                    array(coord), localTMs[subAsm], localTVs[subAsm])]])
    if asm in asmParts:                                                      # if assembly has top-level parts:
        for part in asmParts[asm]:                                           # for each top-level part:
            globalCoords.update([[part, {}]])                                # create new entry in globalCoords
            for (point, coord) in localCoords[part].iteritems():             # for each point in part:
                globalCoords[part].update([[point.upper(), transCoord(       # translate/transform point to globalCoords
                    array(coord), localTMs[part], localTVs[part])]])
    return globalCoords


def myMask(idnums):
    """ Produce mask string for getSequenceFromMask(...) from a feature ID or set of IDs. """
    try:
        idnums = tuple(idnums)                            # make the input a tuple!
    except TypeError:                                     # if input is not iterable:
        idnums = (idnums,)                                # make it a tuple anyway!
    powersum = 0                                          # integer to hold mask number
    for num in idnums:                                    # iterating through input IDs:
        powersum += 2**num                                # add 2**ID to powersum
    rawmask = hex(powersum)[2:]                           # convert powermask to hexadecimal
    rawmask = STR.rstrip(rawmask, 'L')                    # strip "long" character, if necessary
    if max(idnums) < 32:                                  # if hex number is 8 digits or less:
        mask = '[#' + rawmask + ' ]'                      # create mask
    else:                                                 # if hex number is >8 digits:
        maskpieces = []                                   # container for fragments of hex string
        piececount = int(math.ceil(len(rawmask)/8))       # number of times to split hex string
        for i in range(piececount):                       # for each split needed:
            maskpieces.append(rawmask[-8:])               # append last 8 characters of hex string to fragment list
            rawmask = rawmask[:-8]                        # trim last 8 characters from hex string
        maskpieces.append(rawmask)                        # append remaining hex string to fragment list
        mask = '[#' + STR.join(maskpieces, ' #') + ' ]'   # join fragments, using the correct delimiters, to create mask
    return mask


def toBC(constraint):
    """ Translates a degree of freedom as read from the XML to the appropriate SymbolicConstant. """
    if constraint == 'FIXED':
        return 0
    elif constraint == 'FREE':
        return UNSET
    else:
        return float(constraint)