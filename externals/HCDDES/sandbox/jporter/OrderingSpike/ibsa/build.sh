#!/bin/bash

#g++ -g -o testTrace -I. -I../include -L../lib traceViz.cpp testTrace.cpp -lctemplate

#g++ -g -o gengraphs -I../include -L../lib gengraphs.cpp -lboost_graph -lboost_random 

#g++ -g -o schedtest -I../include -L../lib SchedTest.cpp Procs.cpp -lboost_graph

#g++ -g -o procsorttest -I../include -L../lib ProcSortTest.cpp Procs.cpp -lboost_graph

#g++ -g -o cptest -I../include -L../lib cpTest.cpp DepGraph.cpp -lboost_graph

#g++ -g -o serinjecttest -I../include -L../lib serinjectTest.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph

#g++ -g -o serinjecttest -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib serinjectTest.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph

g++ -g -o searchtest1 -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib SearchTest1.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph

g++ -g -o searchtest2 -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib SearchTest2.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph

g++ -g -o searchtest3 -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib SearchTest3.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph

g++ -g -o searchtest4 -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib SearchTest4.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph


#g++ -g -o schedtest -I/Users/jporter/Desktop/Tools/boost/include -L/Users/jporter/Desktop/Tools/boost/lib SchedTest.cpp IBSA.cpp Procs.cpp DepGraph.cpp -lboost_graph


