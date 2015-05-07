from NamedArray import NamedArray

# DECOMPOSES A STRUCTURE OR LIST OF STRUCTURES INTO A LIST OF ARRAYS
# ONE ARRAY FOR EACH LEAF IN THE STRUCTURE
def getNamedArrayList( argDeclBase ):
    retval = []
    for namedArray in getNamedArrays( argDeclBase ):
        retval.append( namedArray )
    return retval

def getNamedArrays( argDeclBase ):
    if type( argDeclBase ) in ( list, tuple ):
        for adb in argDeclBase:
            for namedArray in getNamedArraysAux( adb ):
                yield namedArray
    else:
        for namedArray in getNamedArraysAux( argDeclBase ):
            yield namedArray

def getNamedArraysAux( argDeclBase ):
    if argDeclBase.isNull() or argDeclBase.type.isNull():
        return

    name = argDeclBase.name;
    
    for namedArray in getNamedArraysOfType( argDeclBase.type ):
        namedArray.prependToName( name )
        yield namedArray

def getNamedArraysOfType( dt ):
    if dt.isStruct():
        return getNamedArraysOfStruct( dt )
    elif dt.isArray():
        return getNamedArraysOfArray( dt )
    elif dt.isBasicType():
        return getNamedArraysOfBasicType( dt )
    else:
        return

def getNamedArraysOfStruct( dt ):
    for member in dt.getMembers():
        for namedArray in getNamedArrays( member ):
            namedArray.prependToName( "." )
            yield namedArray
           
def getNamedArraysOfArray( dt ):

    elementType = dt.getElementType()

    if elementType.isStruct():
    
        dimensions = dt.getDimensions()
        indexes = [0]*len( dimensions )

        iterations = reduce( lambda x, y: x*y, dimensions )
        
        for namedArray in getNamedArraysOfStruct( elementType ):
            for ix in xrange( 0, iterations ):
                newNamedArray = namedArray
                newNamedArray.prependToName(   "(" + ",".join(  str( jx ) for jx in indexes  ) + ")"   )
                yield newNamedArray
                
                for ix in reversed(   range(  0, len( dimensions )  )   ):
                    indexes[ ix ] += 1
                    if indexes[ ix ] < dimensions[ ix ]:
                        break
                    indexes[ ix ] = 0

        return
        
    yield NamedArray( dt )
    return

def getNamedArraysOfBasicType( dt ):
    yield NamedArray( dt )
    return
