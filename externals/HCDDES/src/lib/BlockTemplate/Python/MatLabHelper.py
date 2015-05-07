import re


def MatLabStringToNumList( input ):

    new_list = []
    for val in input.replace( ",", " " ).replace( "[", "" ).replace( "]", "" ).split():
        testval = val
        if testval[0] == "-" or testval[0] == "+":
            testval = testval[1:]
        if testval.count( "." ) <= 1 and testval.replace( ".", "" ).isdigit():
            new_list.append(  float( val )  )

    return new_list


def multiportSwitchParseSpecifiedIndices( input ):
	
    specifiedIndicesStringList = re.findall( "\\[.*?\\]|[0-9]+", input )

    retval = []
    for item in specifiedIndicesStringList:
        if item.startswith( '[' ):
            retval.append(   map(  lambda x: int( x ), re.findall( "[0-9]+", item )  )   )
        else:
            retval.append(  int( item )  )

    return retval


