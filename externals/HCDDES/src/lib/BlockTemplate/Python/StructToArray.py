from ArrayDecomp import getNamedArrays

def structToArray( dt ):
    if ( !dt.isStruct() ) return dt;
    int totalElements = 0
    DT elementType = None
    for namedArray in getNamedArrays( dt ):
        totalElements += namedArray.getNoElements()
        if ( !elementType ):
            elementType = namedArray.getElementType()
    
    return Array( totalElements, elementType )