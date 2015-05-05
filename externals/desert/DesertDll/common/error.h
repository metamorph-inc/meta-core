// -*-C++-*-
#ifndef ERROR_H
#define ERROR_H

typedef enum { cTodo, cInfo,  cWarning, cError, cFatal } CErrorLevel;
typedef void (* CExitHook)(int exitCode);

void SetExitHandler(CExitHook hook);
void SetIgnoreLevel(CErrorLevel level);
void SetAbortLevel(CErrorLevel level);
void StartLogging(const char *fileName=0, bool append=false);
void StopLogging();
void Todo(const char *loc, const char *fmt, ...);
void Info(const char *loc, const char *fmt, ...);
void Warning(const char *loc, const char *fmt, ...);
void Error(const char *loc, const char *fmt, ...);
void Fatal(const char *loc, const char *fmt, ...);

#ifdef _DEBUG
#define ASSERT_EX( cond, loc, msg ) \
{ \
  if ( !(cond) ) Fatal(loc, "Assertion %s failed : %s", #cond , msg); \
}
#else
#define ASSERT_EX( cond, loc, msg )
#endif



#endif //#ifndef ERROR_H
