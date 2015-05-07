
#ifndef _MPRINT_H_
#define _MPRINT_H_
#include <stdio.h>

static void mprintf(const char *format, double d) {
  if (1 / d == 0) {
    if (d > 0) {
      printf("Inf, ");
    } else {
  	  printf("-Inf, ");
    }
  } else {
    if (d >= 0 || d <= 0) {
      printf(format, d);
    } else {
      printf("NaN, ");
    }
  }
}

#endif

