// -*-C++-*-
// utils.h
// Declaration of utility functions

#ifndef UTILS_H
#define UTILS_H

#include "core/coredefs.h"

int log2(int num);
int compareInt( const void *arg1, const void *arg2);
int *toVector(int val, int len, int *vec);
int toInteger(int *vector, int len);
void RemoveCRLF(CString& text);

#endif // #ifndef UTILS_H
