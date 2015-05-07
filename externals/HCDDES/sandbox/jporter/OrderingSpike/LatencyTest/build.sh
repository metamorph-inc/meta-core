#!/bin/bash

# cheap makefile

g++ ExploreLatency.cpp TestOrdering.cpp -o ExploreLatency -I/home/jporter/HCDDEStrunk/sandbox/jporter/OrderingSpike/include -L/home/jporter/HCDDEStrunk/sandbox/jporter/OrderingSpike/lib -lgecodekernel -lgecodeint -lgecodesupport -lgecodesearch -lboost_graph -lboost_regex


