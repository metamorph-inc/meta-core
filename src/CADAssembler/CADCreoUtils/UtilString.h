
#ifndef UTILSTRING_H
#define UTILSTRING_H
#include <ProToolkit.h>
#include <ProWstring.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char ProUtilCname[PRO_NAME_SIZE];

/*--------------------------------------------------------------------*\
    Straight ASCII chars
\*--------------------------------------------------------------------*/
extern int ProUtilStrcmp(char *s, char *t);
extern int ProUtilStrncmp(char *s, char *t, int n);
extern void ProUtilStringUpper(char *input_string, char *output_string);
extern void ProUtilStringLower(char *input_string, char *output_string);
extern ProError ProUtilstrcpy(char *dest_string, const char *source_string );
extern ProError ProUtilstrcat(char * dest_string, const char * source_string);
extern ProError ProUtilstrncpy(char  *dest_string, const char  *source_string,
                              int copy_length );

/*--------------------------------------------------------------------*\
    Straight wchar_t chars 
\*--------------------------------------------------------------------*/
extern int ProUtilWstrcmp(wchar_t *ws1, wchar_t *ws2);
extern int ProUtilWstrCmp(wchar_t *ws1, wchar_t *ws2);
extern int ProUtilWstrncmp(wchar_t *ws1, wchar_t *ws2, int n);
extern wchar_t *ProUtilWstrcpy(wchar_t *ws1, wchar_t *ws2);
extern int ProUtilWstrLen(wchar_t *ws1);
extern wchar_t *ProUtilWstrcat(wchar_t *ws1, wchar_t *ws2);

/*--------------------------------------------------------------------*\
    Mixed ASCII &  wchar_t chars 
\*--------------------------------------------------------------------*/
extern int ProUtilStrwscmp(char *s1, wchar_t *ws2);
extern int ProUtilStrnwscmp(char *s1, wchar_t *ws2, int n);
extern int ProUtilWstrPrint(wchar_t *ws1, int width);
extern int ProUtilStrWstrcat(char *s1, wchar_t * ws2);
extern int ProUtilWstrStrcat(wchar_t *ws1, char * s2);

extern wchar_t *ProUtilWstrStrcatToWstr(wchar_t *ws1, char *s2);
extern char *ProUtilStrWstrcatToStr(char *s1, wchar_t *ws2);

/*--------------------------------------------------------------------*\
    String Array <------> File
\*--------------------------------------------------------------------*/
extern ProError ProUtilWstringArrayToFile(ProWstring* str_xar, ProPath wpath);
extern ProError ProUtilFileToWstringArray(ProPath wpath, ProWstring** str_xar);

/*--------------------------------------------------------------------*\
    Pro/DEVELOP string utilities
\*--------------------------------------------------------------------*/
extern char* pro_wstr_to_str(char *str, wchar_t *wstr);
extern wchar_t* pro_str_to_wstr(wchar_t *wstr, char *str);

/*--------------------------------------------------------------------*\
    Definitions
\*--------------------------------------------------------------------*/
#define INVALID_INPUT "invalid input"
#ifndef NULL_WCHAR
#define	NULL_WCHAR	0
#endif

#ifndef NULL_CHAR
#define	NULL_CHAR	'\0'
#endif

 #ifdef __cplusplus
 }
 #endif

#endif /* ifdef UTILSTRING_H */
