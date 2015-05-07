datastore = {}

def ancestors( block ):
    currentBlock = block
    while 1:
        if currentBlock.isNull():
			return
			
        yield currentBlock
        currentBlock = currentBlock.parent
        
def create( block ):
    varname = block.parameters[ "DataStoreName" ].value
    var = block.createGlobalVariable( varname, block.parameters[ "InitialValue" ] )
    datastore[ ( block.parent, varname ) ] = var
    return None
 
def retrieve( block ):
    varname = block.parameters[ "DataStoreName" ].value
    for ancestor in ancestors( block ):
        if ( ancestor, varname ) in datastore:
            return datastore[ ( ancestor, varname ) ]
    return None
