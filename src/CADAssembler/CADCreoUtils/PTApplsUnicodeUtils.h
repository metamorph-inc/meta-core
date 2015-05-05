


/*--------------------------------------------------------------------*\
  C System includes
\*--------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include <ProTKRunTime.h>


#ifndef _PTApplsUnicodeUtils_h_
#define _PTApplsUnicodeUtils_h_



FILE * PTApplsUnicodeFopen(const char *filename, const char *mode);
/*
   Purpose: Used as a unicode equivalent for fopen.

  Input Arguments:
      filename      - The name of the file to be opened.

      mode          - The mode in which the file will be operated.

   Output Arguments:
      none

   Return Values:
      handle       -  The file handle for the open file.
*/ 


void PTApplsWriteBOM(FILE *fp);
/*
   Purpose: Utility to write BOM character to the file.

  Input Arguments:
      fp         - The file handle of the file.

   Output Arguments:
      none

   Return Values:
      none
*/



#endif
