#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "common/error.h"
#include "time.h"

static const char *logName="error.log";

static FILE *log=0;
static bool touchLog=false;
static char buf[1024];

static void Log(const char *msg, const char *loc, int level);
static void Display(const char *msg, const char *loc, int level);
static void AbortHandler(int level);

static CErrorLevel ignoreLevel = cError;
static CErrorLevel abortLevel = cFatal;
static CExitHook exitHook = 0;

void SetExitHandler(CExitHook h)
{
  exitHook = h;
}

void SetIgnoreLevel(CErrorLevel level)
{
  ignoreLevel = level;
}

void SetAbortLevel(CErrorLevel level)
{
  abortLevel = level;
}

void StartLogging(const char *n, bool append)
{
  if (n) logName = n;
  if(!append)
	log = fopen(logName, "w");
  else
  {
	log = fopen(logName, "a");

	char currtime[128];
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	sprintf(currtime,"%02d-%02d-%04d  %02d:%02d:%02d", (tm->tm_mon + 1), tm->tm_mday, (tm->tm_year+1900), tm->tm_hour, tm->tm_min, tm->tm_sec);

	fprintf(log, "\n=====Run Desert at %s======\n", currtime);
  }
}

void StopLogging(void)
{
  if (log)
  {
    fflush(log);
    fclose(log);
    log = 0;
  }
  if (!touchLog) _unlink(logName);
}

void Todo(const char *loc, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  Log(buf, loc, cTodo);
}

void Info(const char *loc, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  Log(buf, loc, cInfo);
}

void Warning(const char *loc, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  Log(buf, loc, cWarning);
  Display(buf, loc, cWarning);
}

void Error(const char *loc, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  Log(buf, loc, cError);
  Display(buf, loc, cError);
}

void Fatal(const char *loc, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  Log(buf, loc, cFatal);
  Display(buf, loc, cFatal);
  AbortHandler(cFatal);
}

static void Log(const char *msg, const char *loc, int level)
{
  if (!log) return;
  touchLog = true;

  switch(level)
  {
  case cTodo:
    fprintf(log, "Todo:<%s>: %s\n", loc, msg);
    break;
  case cInfo:
    fprintf(log, "Info:<%s>: %s\n", loc, msg);
    break;
  case cWarning:
    fprintf(log, "Warning:<%s>: %s\n", loc, msg);
    break;
  case cError:
    fprintf(log, "Error:<%s>: %s\n", loc, msg);
    break;
  case cFatal:
    fprintf(log, "Fatal:<%s>: %s\n", loc, msg);
    break;
  default:
    fprintf(log, "Unknown:<%s>: %s\n", loc, msg);
  }
}

#ifndef _CONSOLE
#define VC_EXTRALEAN
#include <afxwin.h>
#endif

void AbortHandler(int level)
{
  if (level < abortLevel) return;

  StopLogging();
  
  if (exitHook) exitHook(1);
  abort();
}

static void Display(const char *msg, const char *loc, int level)
{
  if (level < ignoreLevel) return;

  CWinApp *app = AfxGetApp();
  if (app)
  {
    CString str;
    str.Format("%s\n%s", msg, loc);
    AfxMessageBox(str, (level > cWarning) ? MB_OK | MB_ICONSTOP : MB_OK);
  }
  else
  {
    switch(level)
    {
    case cWarning:
      fprintf(stderr, "Warning:<%s>: %s\n", loc, msg);
      break;
    case cError:
      fprintf(stderr, "Error:<%s>: %s\n", loc, msg);
      break;
    case cFatal:
      fprintf(stderr, "Fatal:<%s>: %s\n", loc, msg);
      break;
    default:
      fprintf(stderr, "Unknown:<%s>: %s\n", loc, msg);
    }
  }
}

