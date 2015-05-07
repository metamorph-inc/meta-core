class LoopHelper:

    indentBase = '    '
    
    def __init__( self, block, dimensions, indent = '' ):

        self._indexes = ''
        self._loopVars = []
        self._dimensions = dimensions
        self._indent = indent if len( dimensions ) > 0 else ''

        firstIndex = True
        for dimension in self._dimensions:
            self._loopVars.append(  block.createVariable( "ix", "int" )  )
            if firstIndex:
                self._indexes = '( ' + self._loopVars[ -1 ]
                firstIndex = False
            else:
                self._indexes += ', ' + self._loopVars[ -1 ]
        if self._dimensions:
            self._indexes += ' )'

    def indent( self ):
        return self._indent

    def loopVars( self ):
        return self._loopVars

    def indexes( self ):
        return self._indexes
        
    def startLoops( self ):
        retval = ''
        for loopVar,dimension in zip( self._loopVars, self._dimensions ):
            retval += self._indent + loopVar + " = 0\n" + \
                      self._indent + "while " + loopVar + " < " + str( dimension ) + "\n"
            self._indent += LoopHelper.indentBase
        return retval
    

    def endLoops( self ):
        retval = ''
        self._loopVars.reverse()
        for loopVar in self._loopVars:
            retval += self._indent + loopVar + " = " + loopVar + " + 1\n"
            self._indent = self._indent[:-len( LoopHelper.indentBase )]
            retval += self._indent + "end" + "\n"
        return retval

