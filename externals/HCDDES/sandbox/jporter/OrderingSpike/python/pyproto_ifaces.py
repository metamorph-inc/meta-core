#!/usr/bin/env python
#
# Create the interfaces for a task graph
#

####
# NEXT: Use topological sort for the ordering!  How big of a problem can we
#       handle using topological sort?  How hard is it to modify the problem?
####

import pygraphviz as pgv
import networkx as nx
import random as rnd
import itertools
import time

g1 = pgv.AGraph( 'test50000.dot' )
dg1 = nx.DiGraph( g1 )

#print dg1.nodes()

# Algorithm:
# Create a new graph for the fully expanded model

dg2 = nx.DiGraph()
dg2.add_nodes_from( dg1 )

# Create a new graph just for the interfaces

dg3 = nx.DiGraph()


msg_nodes = []
iface_nodes = []
count = 0

# Create nodes for messages
for n in dg1.nodes_iter():                  # For each task
    es = dg1.successors( n )                # Get all receivers
    if ( len( es ) < 1 ):
        continue
    parts = rnd.randrange( 1, len( es )+1 ) # partition them among
    msgs = [[] for idx in range(parts)]     # a set of messages
    for e in es:
        idx = rnd.randrange( parts )        # deal the receivers randomly 
        msgs[idx].append(e)                 # into the message bins
    for m in msgs:
        if ( len( m ) > 0 ):                # for non-empty message lists
            msgname = 'm' + str(count)      # create a name
            count = count + 1  
            sname = msgname + '_s'             
            dg2.add_node( sname )
            dg3.add_node( sname, order=-1 )
            dg2.add_edge( n, sname )           # create a receiver interface
            iface_nodes.append(sname)
            dg2.add_node( msgname )    # create the message
            dg2.add_edge( sname, msgname )
            msg_nodes.append( msgname )
            scount = 1
            for e in m:                     # create the sender interfaces
                rname = msgname + '_r' + str( scount )
                scount = scount + 1
                dg2.add_node( rname )
                dg3.add_node( rname, order=-1 )
                dg2.add_edge( msgname, rname )
                dg3.add_edge( sname, rname, dummy=0 )
                dg2.add_edge( rname, e )
                iface_nodes.append( rname )

for n in dg1.nodes_iter():
    pred = dg2.predecessors(n)
    succ = dg2.successors(n)
    if ( ( len( pred ) > 0 ) and ( len( succ ) > 0 ) ):
        for p,s in itertools.product( pred, succ ):
            dg3.add_edge( p, s )
 
#print dg2.nodes()
#print dg2.edges()


#print msg_nodes
#print iface_nodes


#nx.write_dot( dg2, 'ifaces.dot' )

t0 = time.clock()
ts = nx.topological_sort( dg2 )
t1 = time.clock() - t0
print "Initial " + str(t1)


t0 = time.clock()
ts3 = nx.topological_sort( dg3 )
#print ts
idx = 1
for t in ts3:
    dg3[t]['order'] = idx
    idx = idx + 1
t1 = time.clock() - t0
print "Inerfaces " + str(t1)

#print dg3.nodes(data=True)

#nx.write_dot( dg3, 'ifaces_only.dot' )

for idx in range(len(ts3)-1):
    dg2.add_edge( ts3[idx], ts3[idx+1] )

t0 = time.clock()
ts2 = nx.topological_sort( dg2 )
t1 = time.clock() - t0
print "Everything " + str(t1)

