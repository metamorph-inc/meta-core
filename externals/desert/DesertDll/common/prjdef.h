// -*-C++-*-

#ifndef PRJDEF_H
#define PRJDEF_H

#include <afxwin.h>
#include "error.h"

class CPrjDef
{
public:
  bool Load(const char *fname);
  void ParseLine(char *line);
  
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
