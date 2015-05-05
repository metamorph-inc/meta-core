


/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProObjects.h>
#include <ProMdl.h>
#include <ProFeature.h>
#include <ProTKRunTime.h>
#include "PTApplsUnicodeUtils.h"
#include <ProUtil.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilString.h"
#include "UtilMessage.h"

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/

static struct str_to_type
{
    char *str;
    ProType type;
}

str_type_table[] =
    {
        {(char *)"sec", PRO_2DSECTION},
        {(char *)"prt", PRO_PART},
        {(char *)"asm", PRO_ASSEMBLY},
        {(char *)"drw", PRO_DRAWING},
        {(char *)"mfg", PRO_MFG},
        {(char *)"lay", PRO_LAYOUT},
        {(char *)"frm", PRO_DWGFORM},
        {(char *)"rep", PRO_REPORT},
        {(char *)"mrk", PRO_MARKUP},
        {(char *)"dgm", PRO_DIAGRAM},
        {(char *)"???", PRO_TYPE_INVALID}
    };

/*====================================================================*\
  Function : ProUtilGetProType
  Purpose  : looks up the type for a specified string
\*====================================================================*/
ProType ProUtilGetProType( char *type_str )
{
    int table_size;
    int i;

    table_size = sizeof(str_type_table)/sizeof(struct str_to_type);
    for( i = 0; i < table_size - 1; i++ )
    {
	if( !ProUtilStrcmp(type_str, str_type_table[i].str) )
	    return( str_type_table[i].type );
    }

    return( str_type_table[table_size-1].type );
}

/*====================================================================*\
  Function : ProUtilGetMdlTypeStr
  Purpose  : looks up the string for a specified type
\*====================================================================*/
char *ProUtilGetMdlTypeStr( ProMdlType mdltype )
{
    int table_size;
    int i;

    table_size = sizeof(str_type_table)/sizeof(struct str_to_type);
    for( i = 0; i < table_size - 1; i++ )
    {
	if( str_type_table[i].type == (ProType)mdltype )
	    return( str_type_table[i].str );
    }

    return( str_type_table[table_size-1].str );
}

   
char *ProUtilModelnameGet(ProMdl *model, char *name, char *type);

/*====================================================================*\
   FUNCTION :	UserGenFilename
   PURPOSE  :	Generate a name for an output file
   INPUT    :   p_obj - the model
		filext - file extension
		permission - file permission used to open the file
   OUTPUT   :	filename - file name used to open the file
\*====================================================================*/
FILE *ProUtilGenFilePtr(ProMdl p_obj, char *filext, char *filename, 
							char *permission)
{
	char model_name[PRO_NAME_SIZE];
	char type[PRO_EXTENSION_SIZE];
	char *error;
	FILE *fp;
	char str[PRO_LINE_SIZE];

	error = ProUtilModelnameGet(&p_obj, model_name, type);

	if( error == NULL )
	    return( NULL );
	else
	    ProUtilstrcpy(filename, (const char *)model_name);

	ProUtilstrcat(filename,(const char *)filext);

	if( (fp = PTApplsUnicodeFopen(filename, permission)) == NULL )
	{
	    ProTKSprintf(str, "Could not open the file '%s'", filename);
	    ProUtilMsgPrint("gen", "USER %0s", str);
	}

	return(fp);
}

/*====================================================================*\
  Function : ProUtilConfirmNameType
  Purpose  : Confirm a valid name and type
\*====================================================================*/
int ProUtilConfirmNameType(char *input_name, char *name, ProType *type)
{
    char *dot_occur,		/* occurrence of a dot in input_name */
	 *slash_occur,		/* occurence of a slash */
	  type_str[PRO_EXTENSION_SIZE];
    int name_length,		/* length of a part name */
	type_length;		/* lenght of a part type */

    /*** if there is a full path name, extract the filename first ***/
    if((slash_occur = strrchr(input_name, '/')) != NULL)
	ProUtilstrcpy(input_name, (const char *)&slash_occur[1]);

    /*** get the position of the dot in a file name ***/
    if((dot_occur = strchr(input_name, '.')) != NULL)
    {
	/*** extract name ***/
	if(( name_length = static_cast<int>(strlen(input_name) - strlen(dot_occur)))>0)
	{
	    ProUtilstrncpy(name,(const char *)input_name,name_length);
	    name[name_length] = '\0';
	}
	else
	    return( PRO_TK_BAD_INPUTS );

	/*** extract type ***/
	if ((type_length = static_cast<int>(strlen( &dot_occur[1] )))==3)
	{
	    ProUtilstrncpy(type_str, (const char *)&dot_occur[1], type_length);
	    type_str[3]='\0';

	    /* get the mode */
	    return( (*type = ProUtilGetProType(type_str)) );
	}
	else
	    return( PRO_TK_BAD_INPUTS );
    }
    else
        return( PRO_TK_BAD_INPUTS );
}

/*============================================================================*\
 Function:  ProUtilFileOpen
 Purpose:   Select file from disk
\*============================================================================*/
ProError ProUtilFileOpen(
    char *extension,
    char *file_name)
{ 
    ProLine	w_ext;
    ProPath     *w_path_arr, w_def_path, w_sel_path;
    ProName     *w_path_lab_arr;
    ProError     err;
                             
    ProStringToWstring(w_def_path, (char *)".");
    ProStringToWstring(w_ext, extension);

    /* No default dirs */
    err = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray*)&w_path_arr);
    err = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray*)&w_path_lab_arr);
    /* Open file */
    err = ProFileOpen(NULL, w_ext, w_path_arr, w_path_lab_arr, w_def_path, NULL,
		      w_sel_path);
    TEST_CALL_REPORT("ProFileOpen()", "ProUtilFileOpen()", 
					    err, err != PRO_TK_NO_ERROR);
    ProArrayFree((ProArray*)&w_path_arr);
    ProArrayFree((ProArray*)&w_path_lab_arr);
    ProWstringToString(file_name, w_sel_path);
    return(err);
}
