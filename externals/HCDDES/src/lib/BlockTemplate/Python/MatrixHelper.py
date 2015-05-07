import re
import copy


def createMatrix( dimensions ):

    dimensionsAux = dimensions[:]
    dimensionsAux.reverse()
    retval = 0

    for ix in dimensionsAux:
        retval = [ retval ]
        for jx in range(  0, int( ix ) - 1  ):
            retval.append(  copy.deepcopy( retval[0] )  )

    return retval


def getDimensions( value ):

    if isinstance( value, list ):
        if len( value ) == 0:
            return []
        return [ len( value ) ] + getDimensions( value[0] )

    return []


def incrementCounter( counter, limits ):
    for ix in range(  0, len( counter )  ):
        counter[ ix ] += 1
        if counter[ ix ] < limits[ ix ]:
            break
        counter[ ix ] = 0
        

def setValue( marray, indexes, value ):
    if len( indexes ) == 1:
        marray[ indexes[ 0 ] ] = value
    else:
        setValue( marray[ indexes[0] ], indexes[1:], value )


def getValue( marray, indexes ):
    value = marray[ indexes[0] ]
    return getValue( value, indexes[1:] ) if len( indexes ) > 0 and isinstance( value, list ) else value


def parseMatlabMatrix( matrix ):

    retval = []

    if "reshape" in matrix:

        matrixParser = re.compile( 'reshape\\s*\\(\\s*\\[(.*?)\\]\\s*,\\s*(.*)\\)' )
        mpMatchObject = re.search( matrixParser, matrix )

        origMatrix = parseMatlabMatrix(  mpMatchObject.group( 1 )  )
        origDimensions = getDimensions( origMatrix )
        origCounter = [0] * len( origDimensions )

        newDimensions = parseMatlabMatrix(  mpMatchObject.group( 2 )  )
        newMatrix = createMatrix( newDimensions )
        newCounter = [0] * len( newDimensions )

        limit = reduce( lambda x,y : x*y, origDimensions )
        ix = 0
        while ix < limit:
            setValue(  newMatrix, newCounter, getValue( origMatrix, origCounter )  )
            incrementCounter( origCounter, origDimensions )
            incrementCounter( newCounter, newDimensions )
            ix += 1

        retval = newMatrix

    else:
        matrixAux = matrix.strip( ' \t[]' )
        rows = matrixAux.split( ';' )
        for row in rows:
            retval.append(   map(  lambda x : float( x ), re.findall( '[0-9.Ee+-]+', row )  )   )
        if len( rows ) == 1:
            retval = retval[0]

    return retval

        
def printMatlabMatrix( matrix ):

    outputMatrix = "[ "

    dimensions = getDimensions( matrix )
    if len( dimensions ) == 1:

        for value in matrix:
            outputMatrix += str( value ) + " "

    elif len( dimensions ) == 2:

        first = True
        for item in matrix:
            if first:
                first = False
            else:
                outputMatrix += "; "
            for value in item:
                outputMatrix += str( value ) + " "

    else:

        counter = [0] * len( dimensions )

        limit = reduce( lambda x,y: x*y, dimensions )

        for ix in range( 0, limit ):
            outputMatrix += str(  getValue( matrix, counter )  ) + " "
            incrementCounter( counter, dimensions )

    outputMatrix += "]"
    
    return outputMatrix if len( dimensions ) <= 2 else "reshape( " + outputMatrix + ", " + printMatlabMatrix( dimensions ) + " )"


def flatten2D( matrix ):
    if len(  getDimensions( matrix )  ) <= 2:
        return matrix

    return flatten2DAux( matrix )


def flatten2DAux( matrix ):
    retval = []
    if not isinstance( matrix[0][0], list ):
        for item in matrix:
            retval.append( item )
    else:
        for item in matrix:
            retval += flatten2DAux( item )

    return retval


def printMatrix( matrix ):

    if isinstance( matrix, list ):
        for item in matrix:
            printMatrix( item )
        print

    else:
        print matrix, 

mtx = [[[ 10, 1, 9, 2], [8, 3, 7, 4], [6, 5, 5, 6], [4, 7, 3, 8]], [[ 3.4, -2, 17, 6], [-4.5, -1.4, -3.3, -1.4], [9.8, 2.71, 3.14, 0], [25, -25, 25, -25]], [[1.2, 2.3, 3.4, 4.3], [-4.3, 20, -1, 4.56], [2.1, 6.667, -10, -17], [2.4, 9.1, 7.33, 9.0]], [[-7, -2, -1.6, 8], [-20, -10, -15.5, 50], [6.2, 2.1, -5.6, 17], [51, -2, 33, -3]]]

print printMatlabMatrix( mtx )
