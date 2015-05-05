
#ifndef TESTERROR_H
#define TESTERROR_H

#include <ProSizeConst.h>
#include <ProWindows.h>
#include <ProCore.h>
 

/*--------------------------------------------------------------------*\
    Run mode definitions
\*--------------------------------------------------------------------*/
#define TEST_RUN_MODE_SILENT    1    /* - Run silently (do nothing)     */
#define TEST_RUN_MODE_REPORT    2    /* - Report an error, don't crash  */
#define TEST_RUN_MODE_CRASH     3    /* - Report an error, then crash   */
#define TEST_RUN_MODE_STAT	4    /* - Statistic */
#define TEST_RUN_MODE_CRASH_REP 5    /* - Report only if 4-th param is TRUE */

PRO_BEGIN_C_DECLS
extern ProError ProEngineerAbort();
PRO_END_C_DECLS

#ifndef PROTK_ABORT
#define PROTK_ABORT() { ProEngineerAbort (); }
#endif

/*--------------------------------------------------------------------*\
    Macro to force a crash of Pro/ENGINEER.
\*--------------------------------------------------------------------*/
#define TEST_CRASH()                                            \
{                                                               \
    ProTestErrlogWrite((char *)"** CRASHED **\n");                      \
    ProTestErrlogClose();                                       \
    PROTK_ABORT();						        \
}

/*--------------------------------------------------------------------*\
    Macro to report a call to a toolkit function.
\*--------------------------------------------------------------------*/
#define TEST_LOG_CALL(call,from,status)                 \
{                                                       \
    char str[PRO_LINE_SIZE];                            \
    sprintf(str,"%-37s %-37.37s% -d\n",                 \
                        call, from, status);            \
    ProTestErrlogWrite(str);                            \
}


/*--------------------------------------------------------------------*\
    Macro for user guide examples, ignores the fourth argument.
\*--------------------------------------------------------------------*/
#ifdef DEBUG
#define ERROR_CHECK( func, call, err ) \
  if ( err != PRO_TK_NO_ERROR ) \
    fprintf( stderr, "%s in %s returned %s\n", call, func, protk_perror(err) );
#else
#if 1
        /* for PTC internal use */
#define ERROR_CHECK( func, call, err ) TEST_CALL_REPORT(call, func, err, 0)
#else
        /* for customers (omit all macro code to reduce application size) */
#define ERROR_CHECK( func, call, err ) /* */
#endif  /* if 1 */
#endif  /* ifdef DEBUG */

/*--------------------------------------------------------------------*\
    Define TEST_CALL_REPORT in your makefile if you dislike TEST_CALL_REPORT
\*--------------------------------------------------------------------*/
#ifndef NO_TEST_CALL_REPORT
#define TEST_CALL_REPORT( call, from, status, error)	\
{							\
    ProTestCallReport(call, from,status, error);	\
}
#else
#define TEST_CALL_REPORT(a,b,c,d)
#endif

PRO_BEGIN_C_DECLS
    
void ProTestCallReport(const char *call, const char *from, ProError status,int error);
int ProTestErrlogOpen(char *regtest_name,  char *proe_vsn,   char *build);
int ProTestErrlogReopen();
int ProTestErrlogClose();
int ProTestErrlogWrite(char *);
int ProTestRunmodeGet();
ProError ProUtilErrPrint(ProError, char *);
int ProUtilCommandLineParse( int argc, char *argv[] );

PRO_END_C_DECLS

#endif /* ifdef TESTERROR_H */


