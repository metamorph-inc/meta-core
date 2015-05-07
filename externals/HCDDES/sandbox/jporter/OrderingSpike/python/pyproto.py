#!/usr/bin/env python
#
# Python prototype for extracting orderings from problems
#

import pygraphviz as pgv
import networkx as nx
import constraint as cstr

#p = cstr.Problem(cstr.MinConflictsSolver())
#p = cstr.Problem()
p = cstr.Problem(cstr.RecursiveBacktrackingSolver())

#g1 = pgv.AGraph( 'test20.dot' )
g1 = pgv.AGraph( 'ifaces_only.dot' )
dg = nx.DiGraph( g1 )

# Create variables for the permutation space
N = len( dg.nodes() )
for n in dg.nodes():
    nname = 'n' + str(n)
    p.addVariable( nname, range( 1, N+1 ) )

# Break symmetry
sorted_list = nx.topological_sort( dg )
firstname = 'n' + sorted_list[0]
lastname = 'n' + sorted_list[-1]
#print [firstname, lastname]
p.addConstraint( cstr.InSetConstraint([1]), [firstname] )
p.addConstraint( cstr.InSetConstraint([N]), [lastname] )

# Enforce the distinctness (it is a permutation)
p.addConstraint( cstr.AllDifferentConstraint() ) # Takes a second argument which is a sublist of the variables to use

# Ordering function
def func(a, b):
    return b > a

# Add the edge orders
for n, nbr in dg.edges_iter():
    nname = 'n' + n
    nbrname = 'n' + nbr
    p.addConstraint( func, [nname, nbrname] )

#print( p._variables['n41'] )
print(  p.getSolution() )
#print( sorted( p.getSolution() ) )


# Get solutions
#iter = p.getSolutionIter()
#print( iter.next() )
