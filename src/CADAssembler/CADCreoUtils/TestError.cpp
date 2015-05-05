

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"
#include "ProArray.h"

/*--------------------------------------------------------------------*\
C System includes
\*--------------------------------------------------------------------*/
#include <time.h>
#include <string.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "PTApplsUnicodeUtils.h"
#include "UtilString.h"

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/
#define MAX_STAT_SIZE 38
/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/
typedef struct
{
    int num;
    ProError status;
    char call[MAX_STAT_SIZE];
}TestStatistic;

typedef struct 
{
    char     function_name[40];
    ProError permited_err_code;
} PermitedErrors;
    

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/
static FILE *errlog_fp=NULL;
static char errlog_name[PRO_PATH_SIZE]={'\0'};
TestStatistic *statistic = NULL;
char	trail_name[PRO_PATH_SIZE] = ""; /* The name of currently running
					   trail file */

static PermitedErrors permited_errors[] = {
    {"ProMenuProcess", PRO_TK_E_FOUND},
    {"ProModelitemNameGet", PRO_TK_E_NOT_FOUND},
    {"ProSelect", PRO_TK_PICK_ABOVE},
    {"ProSelect", PRO_TK_USER_ABORT},
    {"ProSolidDisplay", PRO_TK_E_FOUND},
    {"ProGraphicsColorSet", PRO_TK_NO_CHANGE},
    {"ProLinestyleSet", PRO_TK_NO_CHANGE}
};



/*====================================================================*\
  Function : ProTestStatisticCmp()
  Purpose  : compare two files by version
\*====================================================================*/
int ProTestStatisticCmp(TestStatistic *st1, TestStatistic *st2)
{
    int call_result;
    call_result = strncmp (st1->call, st2->call, MAX_STAT_SIZE);
    if (call_result == 0)
    {
        call_result = st1->status - st2->status; 
    }
    return (call_result);
}

/*====================================================================*\
    FUNCTION :	ProTestStatisticWrite()
    PURPOSE  :	Write the specified params to the statistic file.
\*====================================================================*/
int ProTestStatisticWrite (char *call, char *from, ProError status)
{
    int i, line_num, line_found = 0;
    TestStatistic line;

    ProArraySizeGet ((ProArray)statistic, &line_num);
    if (errlog_fp == NULL)
	return (0);

    for (i = 0; i < line_num; i++)
    {
        if (strncmp (statistic[i].call, call, MAX_STAT_SIZE)==0)
        {
            if (statistic[i].status == status)
            {
                statistic[i].num++;
                line_found = 1;
                break;
            }
        }
    }
    if (!line_found)
    {
        ProUtilstrncpy (line.call, (const char *)call, MAX_STAT_SIZE);
        if (strlen (call) >= MAX_STAT_SIZE)
        {
            line.call[MAX_STAT_SIZE-4] = '\0';
            ProUtilstrcat (line.call, "..");
        }
        line.status = status;
        line.num = 1;
        ProArrayObjectAdd ((ProArray*)&statistic, PRO_VALUE_UNUSED,
            1, &line);
        if (line_num > 1)
        {
            qsort(statistic, line_num+1, sizeof(TestStatistic), 
	        (int (*)(const void *, const void *))
  	        ProTestStatisticCmp);
  	}
    }
    return (1);
}

/*====================================================================*\
    FUNCTION : TEST_CALL_REPORT
    PURPOSE  : report a Pro/TOOLKIT function call, and status,
                        if status expression false

    call   = name of Pro/TOOLKIT function
    from   = name of function calling the Pro/Toolkit function
    status = status returned by Pro/Toolkit function
    error  =  BOOLEAN expression whether status is reportable
\*====================================================================*/
void ProTestCallReport(const char *call, const char *from, ProError status,int error)    
{                                                       
    int i, mode;                                           
    char str[100], *p_ch, line[100];

    if (error)
    {
	ProUtilstrcpy(line, call);
	p_ch =  strchr(line, '(');
	if (p_ch != NULL)
	   *p_ch = '\0';
        for (i=0; i<sizeof(permited_errors)/sizeof(permited_errors[0]); i++)
        {
            if ((status == permited_errors[i].permited_err_code) && 
                (strcmp(permited_errors[i].function_name, line) == 0))
                error = 0;
        }
    }
    
    mode = ProTestRunmodeGet();
    switch (mode)
    {
    case  TEST_RUN_MODE_STAT:       
        ProTestStatisticWrite ((char *)call, (char *)from, status);
        break;
    case TEST_RUN_MODE_CRASH:
    case TEST_RUN_MODE_REPORT:
	ProTKSprintf(str,"%-37s %-37.37s% -d\n", call, from, status);
	ProTestErrlogWrite(str);
        if(mode == TEST_RUN_MODE_CRASH && error)            
        {                                                   
            ProTKPrintf("\n*** CRASHING Pro/TOOLKIT TEST ***\n");
            ProTKPrintf("    Function         : %s\n", call);    
            ProTKPrintf("    Calling function : %s\n", from);    
            ProTKPrintf("    Status           : %d\n", status);  
            TEST_CRASH()                                    
        }
        break;
    case TEST_RUN_MODE_CRASH_REP:
        if (error)
        {
            /* Report only potential crashes in crash mode*/
	    ProTKSprintf(str,"%-37s %-37.37s% -d\n", call, from, status);
	    ProTestErrlogWrite(str);
        } 
    }                                                  
}


/*====================================================================*\
    FUNCTION :	ProTestErrlogOpen()
    PURPOSE  :	Open the Error Log file
\*====================================================================*/
int ProTestErrlogOpen(
    char *regtest_name,	/* Reg test name */
    char *proe_vsn,	/* Pro/E version */
    char *build)	/* Pro/E build   */
{
  int runmode, path_size;
  char *dot, *p_env, log_dir[PRO_PATH_SIZE];
  time_t t;
  char *default_regtest_name = (char *)"regtest";
  ProError status;

  /*--------------------------------------------------------------------*\
    Don't open it if the run mode is "silent"
    \*--------------------------------------------------------------------*/
  runmode = ProTestRunmodeGet();
  if(runmode == TEST_RUN_MODE_SILENT)
    return(0);

  /*--------------------------------------------------------------------*\
    Find the directory in which to put the logfile
    \*--------------------------------------------------------------------*/
  size_t buf_size;
  // p_env = getenv("PROTOOL_LOG_DIR");
  _dupenv_s(&p_env, &buf_size, "PROTOOL_LOG_DIR");

  if ( p_env == NULL )
    log_dir[0] = '\0';
  else
    {
      ProUtilstrcpy(log_dir, (const char *)p_env);
      path_size = static_cast<int>(strlen(log_dir));
      if ( log_dir[path_size - 1] != '/')
	{
	  log_dir[path_size] = '/';
	  log_dir[path_size + 1] = '\0';
	}
    }

  free(p_env);
  ProTKPrintf("   Logfile dir : %s\n", log_dir);
    
  /*--------------------------------------------------------------------*\
    Make the name "<regtest>.log" and open the file for write.
    \*--------------------------------------------------------------------*/
  // p_env = getenv( "PROTOOL_LOG_FILE" );
  _dupenv_s(&p_env, &buf_size, "PROTOOL_LOG_FILE");

  if ( p_env != NULL )
    regtest_name = p_env;

  if( regtest_name == NULL )
    regtest_name = default_regtest_name;

  if (  ! strcmp( regtest_name, "stderr" )  )
    {
      ProUtilstrcpy( errlog_name, (const char *)regtest_name );
      errlog_fp = stderr;
    }
  else if (  ! strcmp( regtest_name, "stdout" )  )
    {
      ProUtilstrcpy( errlog_name, (const char *)regtest_name );
      errlog_fp = stdout;
    }
  else
    {
      dot = strchr(regtest_name, '.');
      if(dot != NULL)
	*dot = '\0';
      ProUtilstrcpy(errlog_name, (const char *)regtest_name);
      ProUtilstrcat(errlog_name, ".log");
      ProUtilstrcat(log_dir, (const char *)errlog_name);
      ProUtilstrcpy(errlog_name, (const char *)log_dir);
      errlog_fp = PTApplsUnicodeFopen(errlog_name,"a");
    }
  if (p_env != NULL) free(p_env);

  /*--------------------------------------------------------------------*\
    Write a header
    \*--------------------------------------------------------------------*/
  t = time(NULL);
  ProTKFprintf(errlog_fp, "# Pro/TOOLKIT Regression Test Log\n");
  fflush(errlog_fp);
  ProTKFprintf(errlog_fp, "#--------------------------------\n");
  fflush(errlog_fp);
  ProTKFprintf(errlog_fp, "# Pro/ENGINEER version : %s\n", proe_vsn);
  fflush(errlog_fp);
  ProTKFprintf(errlog_fp, "# Pro/ENGINEER build   : %s\n", build);
  fflush(errlog_fp);
  const size_t buff_size = 36;
  char buff[buff_size];
  ctime_s( buff, buff_size, &t ); 
  ProTKFprintf(errlog_fp, "# Started at           : %s", buff);
  fflush(errlog_fp);
  if( strlen( trail_name ) )
    {
      ProTKFprintf(errlog_fp, "# Trail file name      : %s\n", trail_name );
      fflush(errlog_fp);
    }
  ProTKFprintf(errlog_fp, "#\n" );
  fflush(errlog_fp);
  ProTKFprintf(errlog_fp, 
	  "# Pro/TOOLKIT Function         Called From                   Status\n#\n");
  fflush(errlog_fp);

  if (runmode == TEST_RUN_MODE_STAT)
    {
      if (statistic == NULL)
        {
	  status = ProArrayAlloc (0, sizeof (TestStatistic), 
				  10, (ProArray*)&statistic);
        }
    }

  return(0);
}

/*====================================================================*\
    FUNCTION :	ProTestErrlogReopen()
    PURPOSE  :	To reopen an ongoing error log file
\*====================================================================*/
int ProTestErrlogReopen()
{
  if (  strlen( errlog_name ) > 0 )
    {
      if (  ! strcmp( errlog_name, "stderr" )  )
	errlog_fp = stderr;
      else if (  ! strcmp( errlog_name, "stdout" )  )
	errlog_fp = stdout;
      else
	errlog_fp = PTApplsUnicodeFopen(errlog_name,"a");
    }

  return(0);
}

/*====================================================================*\
    FUNCTION :	ProTestErrlogClose()
    PURPOSE  :	Flush and close the error log file.
\*====================================================================*/
int ProTestErrlogClose()
{
  int num, i;
    
  if(errlog_fp != NULL)
    {
      if (statistic != NULL)
        {
	  ProArraySizeGet ((ProArray)statistic, &num);
	  for (i = 0; i < num; i++)
            {
	      ProTKFprintf (errlog_fp, "%-37s% -20s% -6d% -6d\n",
		       statistic[i].call, 
		       "internal()",
		       statistic[i].status,
		       statistic[i].num);
            }
	  ProArrayFree ((ProArray*)&statistic);
        }  
      fflush(errlog_fp);
      if (
	  strcmp( errlog_name, "stderr" )	&&
	  strcmp( errlog_name, "stdout" )
	  )
	{
	  fclose(errlog_fp);
	  errlog_fp = NULL;
	}
    }

  return(0);
}

/*====================================================================*\
    FUNCTION :	ProTestErrlogWrite()
    PURPOSE  :	Write the specified string to the error log file.
\*====================================================================*/
int ProTestErrlogWrite(
    char *str)
{
    if(errlog_fp == NULL)
	return(0);

    ProTKFprintf(errlog_fp, "%s", str);
    fflush(errlog_fp);
    ProTestErrlogClose();
    ProTestErrlogReopen();

    return(0);
}



/*====================================================================*\
    FUNCTION :  ProUtilCommandLineParse()
    PURPOSE  :  Parse the specified command line setting global params
\*====================================================================*/
int ProUtilCommandLineParse( int argc, char *argv[] )
{
    char		*p;
    int			i;


    for( i=1; i<argc; i++ )
    {
	if( strstr( argv[i], "+trail:" ) )
	{
	    p = strchr( argv[i], ':' );
	    ProUtilstrcpy( trail_name, (const char *)(p+1) );
	}
    }


    return PRO_TK_NO_ERROR;
}

