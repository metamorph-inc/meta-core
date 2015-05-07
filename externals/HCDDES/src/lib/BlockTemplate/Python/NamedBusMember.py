class NamedBusMember:

    def __init__( self, *args ):
        self._name = ""
        self._memberType = args[ 0 ] if args else None
    
    def getName( self ):
        return self._name
    
    def setName( self, name ):
        self._name = name
        
    def prependToName( self, pre ):
        self._name = pre + self._name
    
    def getMemberType( self ):
        return self._memberType