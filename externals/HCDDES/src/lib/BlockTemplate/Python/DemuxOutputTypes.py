from ArrayDecomp import getNamedArrays
from BusMemberDecomp import getNamedBusMembers
import re

def getDemuxOutputTypes( inputs, outputs, parameters ):

    if inputs.type.isNull():
        return

    # GET ALL ATOMIC DATUMS FROM INPUT

    output = outputs[ 0 ]

    outputTypes = []
    negOneItems = 0
    if parameters.BusSelectionMode == "on":
        namedBusMembers = getNamedBusMembers( output )
        for namedBusMember in namedBusMembers:
            outputTypes.append( nameBusMember.getMemberType() )
            negOneItems += 1
    else:
        for namedArray in getNamedArrays( output ):
            array = namedArray.getArray()
            if len( outputTypes ) == 0:
                outputTypes.append( array.getElementType() )#slurp
            negOneItems += array.getNoElements();


    # GET SPEC FOR OUTPUTS

    noDatumsPerOutput = []
    negOnePositions = []
    caseRE = re.compile( "[+-]?\\d+" )
    start = 0
    match = caseRE.match( parameters.Outputs.value[start:] )
    while match:
        number = int(  match.group( 0 )  )
        if number > 0:
            negOneItems -= number
        else:
            negOnePositions.append(  len( noDatumsPerOutput )  )
        noDatumsPerOutput.append( number )
        start += match.end( 0 )
        match = caseRE.match( parameters.Outputs.value[start:] )

    if len( noDatumsPerOutput ) == 1:
        noNegOnes = noDatumsPerOutput[ 0 ]
        noDatumsPerOutput = [-1]*noNegOnes
        negOnePositions = range( 0, noNegOnes )


    # ACCOUNT FOR -1'S IN OUTPUT SPEC

    noNegOnes = len( negOnePositions )
    if noNegOnes > 0:
        datumsPerNegOne = int( negOneItems / noNegOnes )
        noOutputMore = negOneItems - datumsPerNegOne * noNegOnes
        for negOnePosition in negOnePositions:
            noDatumsPerOutput[ negOnePosition ] = datumsPerNegOne + 1 if noOutputMore > 0 else datumsPerNegOne
            noOutputMore -= 1


    # DOLE OUT ATOMIC DATUMS TO OUTPUTS

    if parameters.BusSelectionMode == "on":
        start = 0
        for ( out, noDatums ) in zip( outputs, noDatumsPerOutput ):
            out.type = Struct( outputTypes[start:start + noDatums] )
            start += noDatums
    else:
        for ( out, noDatums ) in zip( outputs, noDatumsPerOutput ):
            out.type = Array( noDatums, outputTypes[ 0 ] )
            start += noDatums
