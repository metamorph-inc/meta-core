

#ifndef UTILFILES_H
#define UTILFILES_H

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProFeature.h>
#include <ProMdl.h>
#include <TestError.h>

/*====================================================================*\
   FUNCTION :	ProUtilGenFilePtr
   PURPOSE  :	Generate a name for an output file
   INPUT    :   p_obj - the model
		filext - file extension
		permission - file permission used to open the file
   OUTPUT   :	filename - file name used to open the file
\*====================================================================*/
FILE *ProUtilGenFilePtr(ProMdl p_obj, char *filext, char *filename, 
							char *permission);

/*====================================================================*\
    FUNCTION :	TestQcrName()
    PURPOSE  :	Generate a name for an output QCR file.
\*====================================================================*/
char *ProTestQcrName(
    ProMdl *model,	/* Input - model */
    char filext[],	/* Input - file extension */
    char filename[]);	/* Output - file name */


/*====================================================================*\
  Function : ProUtilGetProType
  Purpose  : looks up the type for a specified string
\*====================================================================*/
ProType ProUtilGetProType( char *type_str );

/*====================================================================*\
  Function : ProUtilConfirmNameType
  Purpose  : Confirm a valid name and type
\*====================================================================*/
int ProUtilConfirmNameType(char *input_name, char *name, ProType *type);

/*====================================================================*\
  Function : ProUtilGetMdlTypeStr
  Purpose  : looks up the string for a specified type
\*====================================================================*/
char *ProUtilGetMdlTypeStr( ProMdlType mdltype );


/*============================================================================*\
 Function:  ProUtilFileOpen
 Purpose:   Select file from disk
\*============================================================================*/
ProError ProUtilFileOpen(char *extension, char *file_name);

#endif /* UTILFILES_H */
