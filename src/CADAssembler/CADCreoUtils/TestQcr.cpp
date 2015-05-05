

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include        <ProToolkit.h>
#include        <ProObjects.h>
#include        <ProMdl.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include        "TestError.h"
#include	"UtilString.h"

/*====================================================================*\
    FUNCTION :	TestQcrName()
    PURPOSE  :	Generate a name for an output QCR file.
\*====================================================================*/
char *ProTestQcrName(
    ProMdl *model,	/* Input - model */
    char filext[],	/* Input - file extension */
    char filename[])	/* Output - file name */
{
    ProError status;
    char model_name[30], model_type[10];
    char *ProUtilModelnameGet(ProMdl*,char*,char*);

/*--------------------------------------------------------------------*\
    Get the current model
\*--------------------------------------------------------------------*/
    if(model == NULL)
    {
	status = ProMdlCurrentGet(model);

	/* No error check so this code can be used out of mode */
	TEST_CALL_REPORT("ProMdlCurrentGet()", "ProTestQcrName()",
			    status, status != PRO_TK_NO_ERROR);
    }

/*--------------------------------------------------------------------*\
    If there is still no model (so no current mode),
	use the name "nomodel".
\*--------------------------------------------------------------------*/
    if(model == NULL)
	ProUtilstrcpy(filename,"nomodel");
    else
    {
/*--------------------------------------------------------------------*\
	Use the name of the current model as the file name.
\*--------------------------------------------------------------------*/
	ProUtilModelnameGet(model, model_name,  model_type);
	ProUtilstrcpy(filename,(const char *)model_name);
    }

/*--------------------------------------------------------------------*\
    Add the file extension.
\*--------------------------------------------------------------------*/
    ProUtilstrcat(filename,(const char *)filext);

    return(filename);
}
