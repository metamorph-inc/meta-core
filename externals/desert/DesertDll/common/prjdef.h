// -*-C++-*-

#ifndef PRJDEF_H
#define PRJDEF_H

#include <afxwin.h>
#include "error.h"

class CPrjDef
{
public:
  bool Load(const TCHAR *fname);
  void ParseLine(TCHAR *line);
  
  CString acsInstallPath;
  CString projectName;
  CString projectPath;
  CString swComponentPath;
  CString hwComponentPath;
  CString systemPath;
  CString runPath;
  CString interfacePath;
};

#endif // #ifndef PRJDEF_H
