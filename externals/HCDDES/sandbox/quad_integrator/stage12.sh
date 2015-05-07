#!/bin/bash

Stage1 -f $1.mga -o $1.xml 
Stage2 -f $1.xml -o $1.scs -t Sched -r 1ms
SchedTool -f $1.scs -o $1.rslt
SchedResults -r $1.rslt -a $1.xml
Stage2 -f $1.xml -o $1 -t FRODO

