#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jporter/OrderingSpike/lib

./gecode_scalability $1
