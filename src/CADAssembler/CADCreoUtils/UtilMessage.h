
#ifndef UTILMESSAGE_H
#define UTILMESSAGE_H

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application external data
\*--------------------------------------------------------------------*/

extern void ProUtilMsgPrint(const char *ftype, const char *format, ...);

extern int ProUtilIntGet(int range[2], int *def, int *input);

extern int ProUtilDoubleGet(double range[2], double *def, double *input);

int ProUtilStringGet(wchar_t* input, wchar_t* def, int max_length);

int ProUtilYesnoGet( char *def );

#endif /* ifdef UTILMESSAGE_H */
