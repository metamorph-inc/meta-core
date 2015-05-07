import MatrixHelper

def getCoefficients( inputs, values ):

    hn_m1 = inputs[ 1 ] - inputs[ 0 ]
    ydiff_m1 = values[ 1 ] - values[ 0 ]

    inputs_diff = [ hn_m1 ]
    values_diff = [ ydiff_m1 ]

    matrix_cprime = [ 0.0 ]
    matrix_dprime = [ 0.0 ]

    for idx in range(  1, len( inputs ) - 1  ):

        hn = inputs[ idx + 1 ] - inputs[ idx ]
        ydiff = values[ idx + 1 ] - values[ idx ]

        inputs_diff.append( hn )
        values_diff.append( ydiff )

        matrix_a = hn_m1
        matrix_b = 2.0 * ( hn_m1 + hn )
        matrix_c = hn
        matrix_d =  3.0* ( ydiff / hn - ydiff_m1 / hn_m1 )

        matrix_cprime.append(  matrix_c / ( matrix_b - matrix_cprime[-1] * matrix_a )  )
        matrix_dprime.append(  ( matrix_d - matrix_dprime[-1] * matrix_a ) / ( matrix_b - matrix_cprime[-2] * matrix_a )  )

        hn_m1 = hn
        ydiff_m1 = ydiff

    matrix_cprime.append( 0 )
    matrix_dprime.append( 0 )

    coefficients = map( lambda x : [x], values )
    
    coefficients[-1].append( matrix_dprime[-1] )
    for idx in range(  len( values ) - 2, -1, -1  ):
        coefficients[ idx ].append( matrix_dprime[ idx ] - matrix_cprime[ idx ]*coefficients[ idx + 1 ][1] )


    for idx in range( 0, len( values ) - 1 ):
        coefficients[ idx ].insert( -1, values_diff[ idx ] / inputs_diff[ idx ] - inputs_diff[ idx ] * ( coefficients[ idx + 1 ][-1] + 2.0 * coefficients[ idx ] [-1] ) / 3.0  )
        coefficients[ idx ].append(  ( coefficients[ idx + 1 ][-1] - coefficients[ idx ][-1] ) / ( 3.0 * inputs_diff[ idx ] )  )
    
    coefficients = coefficients[0:-1]

    return coefficients



def getAllCoefficients( input, matrix ):

    if isinstance( matrix[0], list ):
        retval = []
        for item in matrix:
            retval.append(  getAllCoefficients( input, item )  )
        return retval

    return getCoefficients( input, matrix )



#inputs = [ -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 ]
#values = [ 6.816824, 8.753734, 21.219733, 14.763267, 15.248821, 0.205778, 4.503538, 3.275157, -2.750000, 1.780267 ]


#coefficients = getCoefficients( inputs, values )


#print coefficients

#value = 0.25
#print (  ( coefficients[3][3] * value + coefficients[3][2] ) * value + coefficients[3][1]  ) * value + coefficients[3][0]

