from ArrayManager import ArrayManager

class ArrayCoordinator:

    def __init__( self ):
        self._arrayManagers = []
        self._arrayManagerMap = dict()
        self._arrayNameMap = dict()
        
    def addNamedArray( self, ref, namedArray ):
        retval = ''
		
        arrayManager = ArrayManager( namedArray )
        existingArrayManager = None
        for am in self._arrayManagers:
            if am == arrayManager:
                existingArrayManager = am
                break
        if existingArrayManager:
            arrayManager = existingArrayManager
        else:
            retval += arrayManager.start()
            self._arrayManagers.append( arrayManager )
        self._arrayManagerMap[ ref ] = arrayManager
        self._arrayNameMap[ ref ] = namedArray.getName()
        
        return retval

    def update( self, items ):
        for aam in self._arrayManagers:
            aam.update( items )

    def getSpan( self ):
        return min(  map( lambda x: x.getItemsLeft(), self._arrayManagers )  )
        
    def genDeref( self, ref ):
        return self._arrayManagerMap[ ref ].genDereference( self._arrayNameMap[ ref ] )
    
    def genElem( self, ref ):
        return self._arrayManagerMap[ ref ].genElement( self._arrayNameMap[ ref ] )

    def genIncrements( self, indent = '' ):
        retval = ''
        for aam in self._arrayManagers:
            retval += aam.genIncrement( indent )
        return retval
        
    def getTermArrays( self ):
        newArrayManagers = []
        for aam in self._arrayManagers:
            if aam.getItemsLeft() > 0:
                newArrayManagers.append( aam )
            else:
                aam.end()
        self._arrayManagers = newArrayManagers        

        retval = []
        for ( ref, aam ) in self._arrayManagerMap.items():
            if aam.getItemsLeft() == 0:
                retval.append( ref )        
        for ref in retval:
            del( self._arrayManagerMap[ ref ] )
            del( self._arrayNameMap[ ref ] )
        
        return retval
