########################################################################
##  Copyright (C) 2006 by Marek Wojciechowski
##  <mwojc@p.lodz.pl>
##
##  Distributed under the terms of the GNU General Public License (GPL)
##  http://www.gnu.org/copyleft/gpl.html
########################################################################

"""
Functions to create fortran code.
Needed for exporting trained network to fortran source.
"""

def flines(string = ' '):
    if string == '': string == ' '
    lines = string.splitlines()
    fstr = ''
    for line in lines:
        fstr += ' ' * 6 + line + '\n'
    return fstr

def fcomment(string = ' ', lstrip = False):
    if string == '': string == ' '
    lines = string.splitlines()
    fstr = ''
    for line in lines:
        if lstrip: line = line.lstrip()
        fstr += 'c' + ' ' * 5 + line + '\n'
    return fstr

def addfline(string=' '):  # not very smart solution (should be in flines?)
    if string == '': string == ' '
    return ' '*5 + '& ' + string + '\n'
    
def f2pyline(string=' '):  # not very smart solution (should be in flines?)
    if string == '': string == ' '
    return 'cf2py ' + string + '\n'

def fval2string(val, ftyp):
    if 'INTEGER' in ftyp: return '%i' %val
    if 'REAL' in ftyp: return '%+.7e' %val
    if 'DOUBLE' in ftyp: return ('%+.15e' %val).replace('e', 'd')

def farray(arr, fname):
    shp = arr.shape
    if len(shp) == 1: fshp = '(%i)' %shp[0]
    else: fshp = str(shp)

    from numpy import dtype
    if arr.dtype == dtype('int'): ftyp = 'INTEGER '
    elif arr.dtype == dtype('float32'): ftyp = 'REAL '
    elif arr.dtype == dtype('float64'): ftyp = 'DOUBLE PRECISION '
    else: raise TypeError("Unsupported array type: %s" (arr.dtype,))
    
    declaration = flines ( ftyp + fname + fshp )
    
    indexes = [ [ i ] for i in range(shp[0]) ]
    findexes = [ [ i + 1 ] for i in range(shp[0]) ]
    try:
    #if 1:
        for idx in shp[1:]:
            newindexes = []
            newfindexes = []
            for i in range(idx):
                for j in xrange( len(indexes) ):
                    newindexes += [ indexes[j] + [i] ]
                    newfindexes += [ findexes[j] + [i + 1] ]
            indexes = newindexes[:]
            findexes = newfindexes[:]
    except: pass
    indexes = [ tuple(i) for i in indexes ] 
    if len(shp) == 1: 
        findexes = [ '(%i)' %idx[0] for idx in findexes ]
    else:
        findexes = [ str(tuple(idx)) for idx in findexes ]
    
    definition = ''
    for i in xrange( len(indexes) ):
        fval = fval2string(arr[indexes[i]], ftyp)
        definition += flines ( fname + findexes[i] + ' = ' + fval )
        
    return declaration, definition

def fnumber(number, fname):
    typ = type(number).__name__
    if typ == 'int': ftyp = 'INTEGER '
    elif typ == 'float': ftyp = 'DOUBLE PRECISION '
    else: raise TypeError("Unsupported variable type")

    fval = fval2string(number, ftyp)
    
    declaration = flines ( ftyp + fname )
    definition = flines ( fname + ' = ' + fval )
    
    return declaration, definition
    
def fstring(string, fname):
    typ = type(string).__name__
    if typ == 'str': 
        length = len(string)
        ftyp = 'CHARACTER*%i ' %(length)
    else: raise TypeError("Provide a string")
        
    declaration = flines ( ftyp + fname )
    definition = flines ( fname + ' = ' + "'" + string + "'" )
    
    return declaration, definition
    
def fexport(variable, fname):
    if type(variable).__name__ == 'ndarray':
        return farray(variable, fname)
    elif type(variable).__name__ in ['int', 'float']:
        return fnumber(variable, fname)
    elif type(variable).__name__ == 'str':
        return fstring(variable, fname)

def ffnetrecall(net, fname):
    """
    Takes ffnet network instance and returns string representing
    fortran source of the recalling routine.
    """
    # Network routine definition
    netroutine = flines ( 'SUBROUTINE ' + fname + '(input, output)' )
    
    # Short description
    descr =  net.call.__doc__
    descr += "Arguments:\n"
    descr += "    input - 1-d array of length %i\n"%len(net.inno)
    descr += "    output - 1-d array of length %i\n"%len(net.outno)
    descr = fcomment(descr, lstrip = True)

    # Network variables export
    declarations = ''
    definitions = ''
    # array lengths
    n = len(net.conec); u = len(net.units); i = len(net.inno); o = len(net.outno)
    # defined arrays
    arrs =  [ 'conec', 'inno', 'outno', 'weights', 'eni', 'deo' ]
    for name in arrs:
        deftuple = fexport ( net.__dict__[ name ], name )
        declarations += deftuple[0]
        definitions += deftuple[1] + fcomment()
    # declared only arrays
    declarations += flines ( 'DOUBLE PRECISION units(%i)' %u )
    declarations += flines ( 'DOUBLE PRECISION input(%i)' %i )
    declarations += flines ( 'DOUBLE PRECISION output(%i)' %o )
    
    # call network propagation routine
    callnet = flines ( 'CALL normcall( weights, conec, %i, units, %i, ' %(n, u) )
    callnet += addfline ( 'inno, %i, outno, %i, eni, deo, input, output )'  %(i, o) )
    
    routine = fcomment( '-'*66 ) + \
              netroutine  + \
              fcomment( '-'*66 ) + \
              descr + \
              fcomment( '-'*66 ) + \
              declarations + fcomment() + \
              f2pyline( 'intent(in) input' ) + \
              f2pyline( 'intent(out) output' ) + fcomment() + \
              definitions + \
              callnet + fcomment() + \
              flines( 'END' )  
              #flines( 'EXTERNAL normcall' )

    return routine
    
def ffnetdiff(net, fname):
    """
    Takes ffnet network instance and returns string representing
    fortran source of the network derivative routine.
    """
    # Network routine definition
    netroutine = flines ( 'SUBROUTINE ' + fname + '(input, deriv)' )
    
    # Short description
    descr =  net.derivative.__doc__
    descr += "Arguments:\n"
    descr += "    input - 1-d array of length %i\n"%len(net.inno)
    descr += "    deriv - 2-d array of the shape (%i, %i)\n" \
                  %(len(net.outno), len(net.inno))
    descr = fcomment(descr, lstrip = True)
    
    # Network variables export
    declarations = ''
    definitions = ''
    # array lengths
    n = len(net.conec); u = len(net.units); i = len(net.inno); o = len(net.outno)
    dn = len(net.dconecno)
    # defined arrays
    arrs =  [ 'conec', 'dconecno', 'dconecmk', 'inno', 'outno', \
              'weights', 'eni', 'ded' ]
    for name in arrs:
        deftuple = fexport ( net.__dict__[ name ], name )
        declarations += deftuple[0]
        definitions += deftuple[1] + fcomment()
    # declared only arrays
    declarations += flines ( 'DOUBLE PRECISION units(%i)' %u )
    declarations += flines ( 'DOUBLE PRECISION input(%i)' %i )
    declarations += flines ( 'DOUBLE PRECISION deriv(%i, %i)' %(o, i) )
    
    # call network propagation routine
    callnet = flines ( 'CALL normdiff( weights, conec, %i, dconecno, %i, ' %(n, dn) )
    callnet += addfline ( 'dconecmk, units, %i, inno, %i, outno, %i, '  %(u, i, o) )
    callnet += addfline ( 'eni, ded, input, deriv)' )
    
    routine = fcomment( '-'*66 ) + \
              netroutine  + \
              fcomment( '-'*66 ) + \
              descr + \
              fcomment( '-'*66 ) + \
              declarations + fcomment() + \
              f2pyline( 'intent(in) input' ) + \
              f2pyline( 'intent(out) deriv' ) + fcomment() + \
              definitions + \
              callnet + fcomment() + \
              flines( 'END' )
              #flines( 'EXTERNAL normdiff' )

    return routine

def fheader(net, version = ''):
    # Preparing limits strings
    inlimits = ''; outlimits = ''
    for i, line in enumerate(net.inlimits):
        inlimits += flines ( '%i --> %s' %(i+1, str(line)) )
    for o, line in enumerate(net.outlimits):
        outlimits += flines ( '%i --> %s' %(o+1, str(line)) )

    header = \
"""##################################################################
THIS FILE IS AUTOMATICALLY GENERATED WITH:

ffnet-%s, feed-forward neural network for python
http://ffnet.sourceforge.net

Copyright (C) 2006 by Marek Wojciechowski
<mwojc@p.lodz.pl>
 
Distributed under the terms of the GNU General Public License:
http://www.gnu.org/copyleft/gpl.html
##################################################################
 
NETWORK SPECIFICATION
%s

INPUT LIMITS
%s
OUTPUT LIMITS
%s
NOTE: You need 'ffnet.f' file distributed with ffnet-%s
      sources to get the below routines to work.
""" %(version, net.__repr__(), inlimits, outlimits, version)
    
    header = fcomment ( header )
    
    return header

