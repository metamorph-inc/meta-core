#include <stdlib.h>
#include <stdio.h>

extern int FAME_get_fault_operating_mode(const char *, int);
extern float FAME_get_fault_amount(const char *, float);

int main (int argn, const char **argv) {
  int i;
  for (i = 1;  i < argn;  i++) {
    printf("%s %d %f\n", argv[i],
           FAME_get_fault_operating_mode(argv[i], 0),
           FAME_get_fault_amount(argv[i], -2.0));
  }
  return 0;
}
