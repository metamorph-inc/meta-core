// -*- C++ -*-
// defs.h
// Copyright (c) Vanderbilt University, 1996
// ALL RIGHTS RESERVED

#ifndef defs_h
#define defs_h 

#ifdef _NO_BOOL
typedef unsigned short bool;
const unsigned short true = 1;
const unsigned short false = 0;
#endif

#define nil 0

#define todo(msg) printf("> ToDo: %s\n",msg)

typedef unsigned long ID;
const ID NullID = (ID)0;

ID MakeID();

#include <assert.h>
#include <stdio.h>

#endif
