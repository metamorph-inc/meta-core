

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include        "ProToolkit.h"	 /* Needed for standard C includes */
#include        "ProTKRunTime.h"
/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilString.h"

#include <stdlib.h>

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/
static int user_run_mode = TEST_RUN_MODE_SILENT;/*  Initialize to be silent  */
static int user_is_run_mode_set = 0;		/*  Is the run mode set yet? */

/*====================================================================*\
    FUNCTION :	ProTestRunmodeGet()
    PURPOSE  :	To check status of user_run_mode
	Determined by the value of the environment variable
	"PROTOOL_DEBUG" :
	    "silent" or not set		- run silently (take no action
					    over error statuses)
	    "crash"			- report error in log file,
					    then crash
	    "stat"                      - statistic
	    "report" or any other value	- report error only
\*====================================================================*/
int ProTestRunmodeGet()
{
    char *p_env;

/*--------------------------------------------------------------------*\
    If the run mode is already set,  just return it
\*--------------------------------------------------------------------*/
    if(user_is_run_mode_set)
	return(user_run_mode);

/*=====================================================================*\
|   Check for "R_PRODEV_REGRESS", to run from auto
\*=====================================================================*/
    if( (p_env = getenv("R_PRODEV_REGRESS")) != NULL)
    {	/*  Check for regression mode  */
        if(!strcmp(p_env,"no_crash"))
            /*  Report errors, no abort  */
	    user_run_mode = TEST_RUN_MODE_REPORT;	
	else
	    user_run_mode = TEST_RUN_MODE_CRASH;
    } 
    else 
    {
/*--------------------------------------------------------------------*\
    Read the value of PROTOOL_DEBUG
\*--------------------------------------------------------------------*/
        p_env = getenv("PROTOOL_DEBUG");

/*--------------------------------------------------------------------*\
    Is it SILENT?
\*--------------------------------------------------------------------*/
        if(p_env == NULL ||
        !ProUtilStrcmp(p_env, (char *)"silent"))
        {
	    user_run_mode = TEST_RUN_MODE_SILENT;
	    ProTKPrintf("\n   Pro/TOOLKIT Run Mode SILENT\n\n");
        }
/*--------------------------------------------------------------------*\
    Are we to CRASH?
\*--------------------------------------------------------------------*/
        else if(!ProUtilStrcmp(p_env, (char *)"crash"))
        {
	    user_run_mode = TEST_RUN_MODE_CRASH;
	    ProTKPrintf("\n   Pro/TOOLKIT Run Mode CRASH\n\n");
        }
/*--------------------------------------------------------------------*\
    Are we to statistic ?
\*--------------------------------------------------------------------*/
        else if (!ProUtilStrcmp(p_env, (char *)"stat"))
        {
            user_run_mode = TEST_RUN_MODE_STAT;
            ProTKPrintf ("\n   Pro/TOOLKIT Run Mode STATISTIC\n\n");
        }
/*--------------------------------------------------------------------*\
    Are we to potential crash report only ?
\*--------------------------------------------------------------------*/
        else if (!ProUtilStrcmp(p_env, (char *)"crash_rep"))
        {
            user_run_mode = TEST_RUN_MODE_CRASH_REP;
            ProTKPrintf ("\n   Pro/TOOLKIT Run Mode CRASH_REPORT\n\n");
        }
/*--------------------------------------------------------------------*\
    Then just REPORT.
\*--------------------------------------------------------------------*/
        else
        {
	    user_run_mode = TEST_RUN_MODE_REPORT;
	    ProTKPrintf("\n   Pro/TOOLKIT Run Mode REPORT\n\n");
        }
    }
/*--------------------------------------------------------------------*\
    Run mode is now set
\*--------------------------------------------------------------------*/
    user_is_run_mode_set = 1;

/*--------------------------------------------------------------------*\
    Return run mode
\*--------------------------------------------------------------------*/
    return(user_run_mode);
}
