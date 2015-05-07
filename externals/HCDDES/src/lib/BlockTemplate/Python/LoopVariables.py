from BlockTemplate import Block
from collections import deque

variableDeque = deque()

def get( noVariables ):
    variables = []
    for ix in range( 0, noVariables ):
        if len( variableDeque ) > 0:
            variables.append( variableDeque.popleft() )
        else:
            variables.append( Block.createMatlabVariable( "ix" ) )
    return variables

def absorb( variables ):
    variableDeque.extend( variables )
