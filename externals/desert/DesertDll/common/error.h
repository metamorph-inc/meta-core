// -*-C++-*-
#ifndef ERROR_H
#define ERROR_H

#include <tchar.h>

typedef enum { cTodo, cInfo,  cWarning, cError, cFatal } CErrorLevel;
typedef void (* CExitHook)(int exitCode);

void SetExitHandler(CExitHook hook);
void SetIgnoreLevel(CErrorLevel level);
void SetAbortLevel(CErrorLevel level);
void StartLogging(const TCHAR *fileName=0, bool append=false);
void StopLogging();
void Todo(const TCHAR *loc, const TCHAR *fmt, ...);
void Info(const TCHAR *loc, const TCHAR *fmt, ...);
void Warning(const TCHAR *loc, const TCHAR *fmt, ...);
void Error(const TCHAR *loc, const TCHAR *fmt, ...);
void Fatal(const TCHAR *loc, const TCHAR *fmt, ...);

#ifdef _DEBUG
#define ASSERT_EX( cond, loc, msg ) \
{ \
  if ( !(cond) ) Fatal(loc, _T("Assertion %s failed : %s"), #cond , msg); \
}
#else
#define ASSERT_EX( cond, loc, msg )
#endif



#endif //#ifndef ERROR_H
