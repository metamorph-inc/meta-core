#!/bin/bash

# cheap makefile

clang++ gecode_scalability.cpp -o gecode_scalability -I/home/jporter/HCDDEStrunk/sandbox/jporter/OrderingSpike/include -L/home/jporter/HCDDEStrunk/sandbox/jporter/OrderingSpike/lib -lgecodekernel -lgecodeint -lgecodesupport -lgecodesearch -lboost_graph -lboost_regex


