


/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Pro/DEVELOP includes
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
C System includes
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "PTApplsUnicodeUtils.h"


/*====================================================================*\
    FUNCTION :	PTApplsUnicodeFopen
    PURPOSE  :	Unicode equivalent of fopen. Writes the BOM character to
                the file.
\*====================================================================*/
FILE *PTApplsUnicodeFopen(const char *filename, const char *mode)
{

  FILE *fp;

  if((strcmp(mode, "r") == 0)) {
    // fp = fopen(filename, mode);
    fopen_s(&fp, filename, mode);
  }


  else if((strcmp(mode,"a") == 0) || (strcmp(mode,"a+") == 0))
    {
      int was_found;

      // fp = fopen(filename, "r");
	  fopen_s(&fp, filename, "r");

      was_found = (fp != NULL);
      
      if (was_found)
	{
	  fclose (fp);
	  // fp = fopen(filename, mode);
      fopen_s(&fp, filename, mode);
	}
     
      else
	{
          // fp = fopen(filename, mode);
          fopen_s(&fp, filename, mode);
          PTApplsWriteBOM(fp);
	}
    }

  else if((strcmp(mode,"w") == 0) || (strcmp(mode,"w+") == 0) || (strcmp(mode,"wt") == 0) || (strcmp(mode,"w+t") == 0))
    {
      // fp = fopen(filename, mode);
      fopen_s(&fp, filename, mode);
      PTApplsWriteBOM(fp);
    }

  return(fp);
}


/*====================================================================*\
    FUNCTION :	PTApplsWriteBOM
    PURPOSE  :	Writes the BOM character to the file for unicode purposes.
\*====================================================================*/
void PTApplsWriteBOM(FILE *fp)
{
  fputs("\357\273\277", fp);
  fflush(fp);
  return;
}




