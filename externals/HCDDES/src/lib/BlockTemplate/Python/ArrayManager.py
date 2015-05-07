import LoopVariables

class ArrayManager:

    def __init__( self, namedArray ):
        self._array = namedArray.getArray()
        self._dimensions = namedArray.getDimensions()
        self._totalItems = namedArray.getNoElements()
        self._itemsUsed = 0
        self._loopVariables = LoopVariables.get(  len( self._dimensions )  )
        self._genInit = True

    def __eq__( self, rhs ):
        return self._dimensions == rhs._dimensions and self._itemsUsed == rhs._itemsUsed

    def __ne__( self, rhs ):
        return not( self == rhs )

    def getArray( self ):
        return self._array

    def getItemsLeft( self ):
        return self._totalItems - self._itemsUsed

    def getIndexes( self ):
        elementNo = self._itemsUsed
        indexes = []
        if self._dimensions:
            for dimension in self._dimensions[0:-1]:
                indexes.append( elementNo % dimension )
                elementNo /= dimension
            indexes.append( elementNo )
        return indexes

    def getGenInit( self ):
        return self._genInit

    def start( self, indent = '' ):
        retval = ''
        if self._genInit:
            for ( var, initValue ) in zip( self._loopVariables, self.getIndexes() ):
                retval += indent + str( var ) + " = " + str( initValue ) + "\n"
            self._genInit = False
        return retval

    def genDereference( self, name, indent = '' ):
        retval = indent + name
        if self._loopVariables:
            retval += "(" + ",".join(   map(  lambda x: str( x ), self._loopVariables  )   ) + ")"

        return retval

    def genElement( self, name, indent = '' ):
        elementNo = self._itemsUsed
        retval = indent + name
        if self._dimensions:
            indexes = self.getIndexes()
            retval += "(" + ",".join(  map(  lambda x: str( x ), indexes  )   ) + ")" 
        _genInit = True
        return retval
        
    def genIncrement( self, indent = '' ):
        retval = ''
        if len( self._dimensions ) <= 2:
            for ix in reversed(   range(  1, len( self._dimensions )  )   ):
                var = str( self._loopVariables[ ix ] )
                retval += indent + var + " = " + var + " + 1\n"
                retval += indent + "if " + var + " >= " + str( self._dimensions[ ix ] ) + "\n"
                indent += '    '
                retval += indent + var + " = 0\n"
            var = str( self._loopVariables[ 0 ] )
            retval += indent + var + " = " + var + " + 1\n"
            for ix in range(  1, len( self._dimensions )  ):
                indent = indent[ 0:-4 ]
                retval += indent + "end\n"
        return retval

    def update( self, items ):
        self._itemsUsed += items

    def end( self ):
        LoopVariables.absorb( self._loopVariables )

