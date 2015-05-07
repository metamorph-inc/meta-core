class NamedArray:

    uniqueNo = 0
    
    def __init__( self, array ):
        self._name = ""
        self._array = array
        self._elementType = array.getElementType()
        self._dimensions = array.getDimensions()
        self._noElements = reduce( lambda x,y: x*y, self._dimensions, 1 )
        self._hashValue = self.getUniqueNo()
    
    @classmethod
    def getUniqueNo( cls ):
        retval = cls.uniqueNo
        cls.uniqueNo += 1
        return retval

    def __hash__( self ):
        return self._hashValue
    
    def getNoElements( self ):
        return self._noElements
    
    def getDimensions( self ):
        return self._dimensions

    def getName( self ):
        return self._name
    
    def setName( self, name ):
        self._name = name
        
    def prependToName( self, pre ):
        self._name = pre + self._name
    
    def getArray( self ):
        return self._array