from Cheetah.Template import Template
from BlockTemplate import Block, Array
from ArrayDecomp import getNamedArrayList
from ArrayCoordinator import ArrayCoordinator

# threshold CONTROLS WHEN A LOOP IS USED TO COPY STRUCTURE-MEMBER ELEMENTS INTO AN ARRAY.
# IF THE NUMBER OF ELEMENTS IN THE STRUCTURE MEMBER IS LESS THAN THE THRESHOLD, THEN
# INDIVIDUAL ASSIGNMENTS ARE USED.  OTHERWISE, A LOOP IS USED.
def performOperation( varMap, template, threshold = 4 ):

    if template[-1] != "\n":
        template += "\n"
    
    # THE STRING VALUE RETURNED (FOR THE mfile2SFC COMPILER)
    retval = ''
    
    namedArrayListMap = {}  # MAPS TEMPLATE TOKENS TO LIST OF NAMED ARRAYS
    namedArrayOffsets = {}  # THE OFFSET INTO EACH NAMED ARRAY LIST
    
    prototypeArrayList = None
    missingType = []
    
    # GET NamedArrayList FOR EVERY STRUCTURE MEMBER OF THE VARIABLES IN varMap
    for ( key, value ) in varMap.items():
        namedArrayList = getNamedArrayList( value )
        namedArrayListMap[ key ] = namedArrayList
        if namedArrayList:
            if not prototypeArrayList:
                prototypeArrayList = namedArrayList
        else:
            missingType.append( key )
        namedArrayOffsets[ key ] = 0

	# IF ONE OR MORE VARIABLES IN varMap HAVE NO TYPE (I.E. AN EMPTY namedArrayList)
	# THEY ARE GIVEN THE FLATTENED TYPE OF THE FIRST VARIABLE THAT HAS A TYPE
    if missingType:
        if not prototypeArrayList:
            return ""
        totalElements = 0
        basicType = None
        for namedArray in prototypeArrayList:
            totalElements += namedArray.getNoElements()
            if not basicType:
                basicType = namedArray.getArray().getElementType()
        newType = Array( totalElements, basicType )
        for key in missingType:
            value = varMap[ key ]
            if type( value ) in ( list, tuple ):
                for item in value:
                    item.type = newType
            else:
                value.type = newType
            namedArrayListMap[ key ] = getNamedArrayList( value )
            

	# THE FOLLOWING CODE MAPS A SINGLE INPUT TO MULTIPLE OUTPUTS
    maxItems = 0
    totalItemsMap = {}
    singletonNamedArrayKeys = []
    for ( key, namedArrayList ) in namedArrayListMap.items():
        itemCount = 0
        for namedArray in namedArrayList:
            itemCount += namedArray.getNoElements()
        if itemCount > maxItems:
            maxItems = itemCount
        if itemCount == 1:
            singletonNamedArrayKeys.append( key )
    
    if maxItems > 1:
        for key in singletonNamedArrayKeys:
            namedArrayListMap[ key ] = [ namedArrayListMap[ key ][0] ]*maxItems
            

    arrayCoordinator = ArrayCoordinator()
    
    initializations = ''
    for ( key, value ) in namedArrayListMap.items():
        initializations += arrayCoordinator.addNamedArray( key, value[ 0 ] )
        
    loopVar = Block.createMatlabVariable( "ix" );
    
    notDone = True
    while notDone:
        items = arrayCoordinator.getSpan()
        if items < threshold:
            for ix in range( 0, items ):
                nameMap = dict()
                for key in namedArrayListMap.keys():
                    nameMap[ key ] = arrayCoordinator.genElem( key )
                arrayCoordinator.update( 1 )
                templateInstance = Template( template, [nameMap] )
                retval += templateInstance.respond()
        else:
            retval += initializations
            nameMap = dict()
            for key in namedArrayListMap.keys():
                nameMap[ key ] = arrayCoordinator.genDeref( key )
            arrayCoordinator.update( items )

            retval += loopVar + " = 0\n"
            retval += "while " + loopVar + " < " + str( items ) + "\n"
            templateInstance = Template( template, [nameMap] )
            retval += "    " + templateInstance.respond()
            retval += arrayCoordinator.genIncrements( '    ' )
            retval += "    " + loopVar + " = " + loopVar + " + 1\n"
            retval += "end\n"
            
        initializations = ''
        termArrays = arrayCoordinator.getTermArrays()        
        for key in termArrays:
            namedArrayList = namedArrayListMap[ key ]
            namedArrayOffsets[ key ] += 1
            offset = namedArrayOffsets[ key ]
            if offset >= len( namedArrayList ):
                notDone = False
                break
            initializations += arrayCoordinator.addNamedArray( key, namedArrayList[ offset ] )

    return retval
