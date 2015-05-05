

/*--------------------------------------------------------------------*\
Pro/TOOKIT includes
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"
#include "ProUtil.h"
/*--------------------------------------------------------------------*\
C System includes
\*--------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>
/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "UtilString.h"
#include "TestError.h"
#include "PTApplsUnicodeUtils.h"
/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/

/*====================================================================*\
    FUNCTION :	ProUtilStrcmp()
    PURPOSE  :	Like strcmp but case-independent
\*====================================================================*/
int ProUtilStrcmp(
    char *s,
    char *t)
{
    int i = 0;

    while( toupper(s[i]) == toupper(t[i]))
    {
	if( s[i++] == '\0' )
	    return(0);
    }
    return(s[i] - t[i]);
}

/*====================================================================*\
    FUNCTION :	ProUtilStrncmp()
    PURPOSE  :	Like strncmp but case-independent
\*====================================================================*/
int ProUtilStrncmp(
    char *s,
    char *t,
    int n)
{
    int i = 0;
    int counter = 0;

    while( toupper(s[i]) == toupper(t[i]))
    {
	if(( s[i++] == '\0') || (counter++ >= n) )
	    return(0);
    }
    return(s[i] - t[i]);
}

/*====================================================================*\
    FUNCTION :	ProUtilStringUpper()
    PURPOSE  :	Convert a string to upper case
\*====================================================================*/
void ProUtilStringUpper(
    char *input_string,
    char *output_string)
{
    int i = 0;

    while ( input_string[i] != '\0')
    {
	output_string[i] = (char)toupper(input_string[i]);
	i++;
    }

    output_string[i] = '\0';
}

/*====================================================================*\
    FUNCTION :	ProUtilStringLower()
    PURPOSE  :	Convert a string to lower case
\*====================================================================*/
void ProUtilStringLower(
    char *input_string,
    char *output_string)
{
    int i = 0;

    while ( input_string[i] != '\0')
    {
	output_string[i] = (char)tolower(input_string[i]);
	i++;
    }

    output_string[i] = '\0';
}

/*====================================================================*\
    FUNCTION :  ProUtilWstrcmp()
    PURPOSE  :  strcmp() but for wide strings, and case-sensitive.
\*====================================================================*/
int ProUtilWstrcmp(
    wchar_t *ws1,
    wchar_t *ws2)
{
  int result, status;

  status = ProWstringCompare (ws1, ws2, PRO_VALUE_UNUSED, &result);
  
  return (result);
}

/*====================================================================*\
    FUNCTION :	ProUtilWstrCmp()
    PURPOSE  :	strcmp() but for wide strings, and case-insensitive.
\*====================================================================*/
int ProUtilWstrCmp(
    wchar_t *ws1,
    wchar_t *ws2)
{
  int len1, len2;
  char* us1;
  char* us2;
  int result;
  
  ProWstringLengthGet (ws1, &len1);
  ProWstringLengthGet (ws2, &len2);
  
  /* Allow extra length for wchar_t -> char conversion */
  us1 = (char*) calloc ((4*len1+1), sizeof (char)); 
  us2 = (char*) calloc ((4*len2+1), sizeof (char));
  
  ProWstringToString (us1, ws1);
  ProWstringToString (us2, ws2);

  result = ProUtilStrcmp (us1, us2);

  free (us1);
  free (us2);

  return (result);
}

/*====================================================================*\
    FUNCTION :	ProUtilWstrncmp()
    PURPOSE  :	strncmp() but for wide strings, and case-insensitive.
\*====================================================================*/
int ProUtilWstrncmp(
    wchar_t *ws1,
    wchar_t *ws2,
    int n)
{
  int len1, len2;
  char* us1;
  char* us2;
  int result;
  
  ProWstringLengthGet (ws1, &len1);
  ProWstringLengthGet (ws2, &len2);
  
  /* Allow extra length for wchar_t -> char conversion */
  us1 = (char*) calloc ((4*len1+1), sizeof (char)); 
  us2 = (char*) calloc ((4*len2+1), sizeof (char));
  
  ProWstringToString (us1, ws1);
  ProWstringToString (us2, ws2);

  result = ProUtilStrncmp (us1, us2, n);

  free (us1);
  free (us2);

  return (result);
}

/*====================================================================*\
    FUNCTION :	ProUtilWstrcpy()
    PURPOSE  :	strcpy() but for wide strings
\*====================================================================*/
wchar_t *ProUtilWstrcpy(
    wchar_t *ws1,
    wchar_t *ws2)
{
    ProWstringCopy (ws2, ws1, PRO_VALUE_UNUSED);

    return (ws2);
}


/*====================================================================*\
    FUNCTION :	ProUtilStrwscmp()
    PURPOSE  :	Compare a string to a wide string
\*====================================================================*/
int ProUtilStrwscmp(
    char *s1,
    wchar_t *ws2)
{
    wchar_t* ws1;
    int result;
    int len = static_cast<int>(strlen (s1));

    ws1 = static_cast<wchar_t*>(calloc (len + 1, sizeof (wchar_t)));
    ProStringToWstring (ws1, s1);

    result = ProUtilWstrcmp(ws1, ws2);

    free (ws1);

    return (result);
}

/*====================================================================*\
    FUNCTION :	ProUtilStrnwscmp()
    PURPOSE  :	strncmp() for a string and a wide string
\*====================================================================*/
int ProUtilStrnwscmp(
    char *s1,
    wchar_t *ws2,
    int n)
{
     wchar_t* ws1;
    int result;

    ws1 = (wchar_t*) calloc (n+1, sizeof (wchar_t));
    ProStringToWstring (ws1, s1);

    result = ProUtilWstrncmp(ws1, ws2, n);

    free (ws1);

    return (result);
}


/*====================================================================*\
    FUNCTION :	ProUtilWstrPrint()
    PURPOSE  :	printf a wide string - for diagnostics
\*====================================================================*/
int ProUtilWstrPrint(
    wchar_t *ws1,
    int width)
{
    char s1[PRO_PATH_SIZE], format[10];

    ProTKSprintf(format,"%%-%ds", width);
    ProWstringToString(s1, ws1);
    ProTKPrintf(format, s1);

    return (0);
}
/*=====================================================================*\
Function:     ProUtilWstrLen()
Purpose:      Get the length of a wide string
\*=====================================================================*/
int ProUtilWstrLen(
    wchar_t *wstr)
{
    int n;
    ProWstringLengthGet (wstr, &n);

    return(n);
}

/*=====================================================================*\
Function:     ProUtilWstrcat()
Purpose:      Concatenates two wide strings
\*=====================================================================*/
wchar_t *ProUtilWstrcat(
        wchar_t *ws1,
        wchar_t *ws2)
{
  ProWstringConcatenate (ws2, ws1, PRO_VALUE_UNUSED);

  return (ws1);
}
 
/*=====================================================================*\
Function:     ProUtilWstrStrcatToWstr()
Purpose:      Concatenates a wstring and a string and returns a wstring
\*=====================================================================*/
wchar_t *ProUtilWstrStrcatToWstr(
                wchar_t *ws1,
                char *s2)
{
    wchar_t* ws2;
    int len = static_cast<int>(strlen (s2));

    ws2 = (wchar_t*) calloc (len + 1, sizeof (wchar_t));
    ProStringToWstring (ws2, s2);

    ProWstringConcatenate (ws2, ws1, PRO_VALUE_UNUSED);
 
    free (ws2);
    
    return(ws1);
}
 
/*=====================================================================*\
Function:     ProUtilStrWstrcatToStr
Purpose:      Concatenates a string and a wstring and returns a string
\*=====================================================================*/
char *ProUtilStrWstrcatToStr(char *s1, wchar_t *ws2)
{
   char s2[PRO_PATH_SIZE];
   ProError status;

    ProWstringToString(s2,ws2);
    status = ProUtilstrcat(s1, s2);

    return(s1);
}
 
/*=====================================================================*\
Function:     ProUtilWstringArrayToFile
Purpose:      prints an array of Wstrings into a file
\*=====================================================================*/
ProError ProUtilWstringArrayToFile (ProWstring* str_xar, ProPath wpath)
{
  ProError      status;
  int           str_num, str_count;
  ProCharLine   path;
  FILE *        fp;
  ProCharLine   a_string;
 
  ProWstringToString (path, wpath);
  fp = PTApplsUnicodeFopen(path, "w");
 
  status = ProArraySizeGet(str_xar, &str_num);
  TEST_CALL_REPORT("ProArraySizeGet", "ProUtilWstringArrayToFile",
                        status, (status != PRO_TK_NO_ERROR));
  if (status != PRO_TK_NO_ERROR) return (status);
 
  for (str_count = 0 ; str_count < str_num; str_count++)
  {
    ProWstringToString(a_string, (str_xar)[str_count]);
    ProTKFprintf(fp, "%s\n",a_string);
  }
  fclose(fp);
  return(PRO_TK_NO_ERROR);
}
 
/*=====================================================================*\
Function:     ProUtilFileToWstringArray
Purpose:      parses the lines of a file into a wide string array
\*=====================================================================*/
ProError ProUtilFileToWstringArray (ProPath wpath, ProWstring** str_xar)
{
  ProError      status;
  FILE *        fp;
  ProCharLine   a_line;
  ProLine       a_wline;
  ProWstring    a_wstring;
  ProCharLine   path;

  /*------------------------------------------------------------------*\
    Get the file pointer
  \*------------------------------------------------------------------*/
  ProWstringToString (path, wpath);
  fp = PTApplsUnicodeFopen(path, "r");
 
  /*------------------------------------------------------------------*\
    Add each line of the file into a location of the array
  \*------------------------------------------------------------------*/
  while(!feof(fp))
  {
    if (fgets (a_line, PRO_LINE_SIZE, fp) == NULL)
  	return (PRO_TK_NO_ERROR);
    
    ProStringToWstring(a_wline, a_line);
    a_wstring = a_wline;
 
    status = ProWstringArrayObjectAdd(str_xar, PRO_VALUE_UNUSED, 1, &a_wstring);
    TEST_CALL_REPORT("ProWstringArrayObjectAdd", "ProUtilFileToWstringArray",
                        status, (status != PRO_TK_NO_ERROR));
    if (status != PRO_TK_NO_ERROR) return (status);
  }
 
  fclose(fp);
  return (PRO_TK_NO_ERROR);
}

/*=====================================================================*\
Function:     ProUtilstrcpy
Purpose:      Copy source_string into dest_string
\*=====================================================================*/
ProError ProUtilstrcpy(
        char *dest_string,
        const char *source_string )
{
    if (dest_string == NULL)
      return PRO_TK_BAD_INPUTS;
       
   while  ( (*dest_string++ = *source_string++) != '\0') ;
 
   return PRO_TK_NO_ERROR;;
}

/*=====================================================================*\
Function:     ProUtilstrcat
Purpose:      Concatnet source_string to dest_string
\*=====================================================================*/
ProError ProUtilstrcat(char * dest_string, const char * source_string)
{
    char * cp = dest_string;
    ProError status;

    if (dest_string == NULL)
      return PRO_TK_BAD_INPUTS;
       
    while( *cp )
      ++cp;
 
    status = ProUtilstrcpy(cp,source_string);

    return PRO_TK_NO_ERROR;;

}

/*=====================================================================*\
Function:     ProUtilstrncpy
Purpose:      Copies copy_length char's of source_string into dest_string
\*=====================================================================*/
ProError ProUtilstrncpy (char  *dest_string, const char  *source_string,
                        int copy_length )   
{
   int  i;
   
   if (dest_string == NULL || source_string == NULL)
      return PRO_TK_BAD_INPUTS;

   if (copy_length < 0)
      return PRO_TK_BAD_INPUTS;
       
   if (copy_length == 0)
     return PRO_TK_BAD_INPUTS;

   for (i = 1 ; 
     ((*dest_string++ = *source_string++) != '\0') && (i < copy_length)  ;  i++ );
    
   for ( ; i < copy_length; i++ )
     *dest_string++ = '\0';

   return PRO_TK_NO_ERROR;
}

