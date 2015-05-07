from types import *

def GetCoefficientAux( zp_list, offset, power ):

    if power == len( zp_list ):
        return zp_list[ offset ]
        
    result = 0
    power += 1
    for subOffset in range( offset + 1, power ):
        result += GetCoefficientAux( zp_list, subOffset, power )
    
    return zp_list[ offset ] * result


def GetCoefficient( zp_list, power ):

    if power == len( zp_list ):
        return 1
    
    result = 0
    power += 1
    for ix in range( 0, power ):
        result += GetCoefficientAux( zp_list, ix, power )
    
    return result


def GetCoefficients( zp_list ):
	
    if (  type( zp_list ) == str  ):
        new_zp_list = []
        for val in zp_list.replace( ",", " " ).split():
            if val.isdigit():
                new_zp_list.append(  int( val )  )
    elif type( zp_list ) != list:
        new_zp_list = [ zp_list ]
    else:
        new_zp_list = zp_list
 
    if len( new_zp_list ) == 0:
        return [1]
		
    result = []
	
    for ix in range(  0, len( new_zp_list ) + 1  ):
        result.insert(  0, GetCoefficient( new_zp_list, ix )  )
        
    return result