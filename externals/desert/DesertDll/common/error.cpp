#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "common/error.h"
#include "time.h"

static const TCHAR *logName=_T("error.log");

static FILE *logFile=0;
static bool touchLog=false;
static TCHAR buf[1024];

static void Log(const TCHAR *msg, const TCHAR *loc, int level);
static void Display(const TCHAR *msg, const TCHAR *loc, int level);
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

void StartLogging(const TCHAR *n, bool append)
{
  if (n) logName = n;
  if(!append)
	logFile = _tfopen(logName, _T("w"));
  else
  {
	logFile = _tfopen(logName, _T("a"));

	TCHAR currtime[128];
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	_sntprintf(currtime, _countof(currtime), _T("%02d-%02d-%04d  %02d:%02d:%02d"), (tm->tm_mon + 1), tm->tm_mday, (tm->tm_year+1900), tm->tm_hour, tm->tm_min, tm->tm_sec);

	_ftprintf(logFile, _T("\n=====Run Desert at %s======\n"), currtime);
  }
}

void StopLogging(void)
{
  if (logFile)
  {
    fflush(logFile);
    fclose(logFile);
    logFile = 0;
  }
  if (!touchLog) _tunlink(logName);
}

void Todo(const TCHAR *loc, const TCHAR *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  _vsntprintf(buf, _countof(buf), fmt, ap);
  va_end(ap);

  Log(buf, loc, cTodo);
}

void Info(const TCHAR *loc, const TCHAR *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  _vsntprintf(buf, _countof(buf), fmt, ap);
  va_end(ap);

  Log(buf, loc, cInfo);
}

void Warning(const TCHAR *loc, const TCHAR *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  _vsntprintf(buf, _countof(buf), fmt, ap);
  va_end(ap);

  Log(buf, loc, cWarning);
  Display(buf, loc, cWarning);
}

void Error(const TCHAR *loc, const TCHAR *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  _vsntprintf(buf, _countof(buf), fmt, ap);
  va_end(ap);

  Log(buf, loc, cError);
  Display(buf, loc, cError);
}

void Fatal(const TCHAR *loc, const TCHAR *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  _vsntprintf(buf, _countof(buf), fmt, ap);
  va_end(ap);

  Log(buf, loc, cFatal);
  Display(buf, loc, cFatal);
  AbortHandler(cFatal);
}

static void Log(const TCHAR *msg, const TCHAR *loc, int level)
{
  if (!logFile) return;
  touchLog = true;

  switch(level)
  {
  case cTodo:
    _ftprintf(logFile, _T("Todo:<%s>: %s\n"), loc, msg);
    break;
  case cInfo:
    _ftprintf(logFile, _T("Info:<%s>: %s\n"), loc, msg);
    break;
  case cWarning:
    _ftprintf(logFile, _T("Warning:<%s>: %s\n"), loc, msg);
    break;
  case cError:
    _ftprintf(logFile, _T("Error:<%s>: %s\n"), loc, msg);
    break;
  case cFatal:
    _ftprintf(logFile, _T("Fatal:<%s>: %s\n"), loc, msg);
    break;
  default:
    _ftprintf(logFile, _T("Unknown:<%s>: %s\n"), loc, msg);
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

static void Display(const TCHAR *msg, const TCHAR *loc, int level)
{
  if (level < ignoreLevel) return;

  CWinApp *app = AfxGetApp();
  if (app)
  {
    CString str;
    str.Format(_T("%s\n%s"), msg, loc);
    AfxMessageBox(str, (level > cWarning) ? MB_OK | MB_ICONSTOP : MB_OK);
  }
  else
  {
    switch(level)
    {
    case cWarning:
      _ftprintf(stderr, _T("Warning:<%s>: %s\n"), loc, msg);
      break;
    case cError:
      _ftprintf(stderr, _T("Error:<%s>: %s\n"), loc, msg);
      break;
    case cFatal:
      _ftprintf(stderr, _T("Fatal:<%s>: %s\n"), loc, msg);
      break;
    default:
      _ftprintf(stderr, _T("Unknown:<%s>: %s\n"), loc, msg);
    }
  }
}

