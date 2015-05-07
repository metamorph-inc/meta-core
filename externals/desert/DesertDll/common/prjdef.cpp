
#include "prjdef.h"

#define INTERFACE_DIR 	"\\etc"
#define SW_COMP_DIR		"\\sw_component"
#define HW_COMP_DIR		"\\hw_component"
#define SYSTEM_DIR		"\\system"
#define RUN_DIR			"\\run"

#define BUFLEN	1024
static TCHAR lineBuffer[BUFLEN];

void
SetAcsInstallPath(CPrjDef *prj, const TCHAR *value)
{
  prj->acsInstallPath = value;
  prj->interfacePath = prj->acsInstallPath + INTERFACE_DIR;
}

void
SetProjectName(CPrjDef *prj, const TCHAR *value)
{
  prj->projectName = value;
}

void
SetProjectPath(CPrjDef *prj, const TCHAR *value)
{
  prj->projectPath = value;
  prj->swComponentPath = prj->projectPath + SW_COMP_DIR;
  prj->hwComponentPath = prj->projectPath + HW_COMP_DIR;
  prj->systemPath = prj->projectPath + SYSTEM_DIR;
  prj->runPath = prj->projectPath + RUN_DIR;
}

struct _prjDeftag
{
  TCHAR *param;
  void (*SetParam)(CPrjDef *prj, const TCHAR *value);
} funcTable[] =
{
  { _T("ACS_Install_Path"), SetAcsInstallPath },
  { _T("Project_Name"), SetProjectName },
  { _T("Project_Path"), SetProjectPath },
  { NULL, NULL }
};


bool
CPrjDef::
Load(const TCHAR *name)
{
  TCHAR *line = lineBuffer;
  FILE *prjFile = _tfopen(name, _T("rt"));
  if (!prjFile)
  {
    Warning(_T("CPrjDef::Load"), _T("Unable to open file <%s>. Using the default directory structure"), name);
    // set the defaults - assuming that we are running from models directory
    acsInstallPath = _T("..");
    interfacePath = _T(".");
    projectName = _T("");
    SetProjectPath(this, _T("..\\runtime"));
    return false;
  }

  while(_fgetts(line, BUFLEN, prjFile))
  {
    if (line[0] == '#') continue;
    if (_tcslen(line) == 0) continue;
    ParseLine(line);
  }
  fclose(prjFile);
  return true;
}


void
CPrjDef::
ParseLine(TCHAR *line)
{
  CString param, value;

  while(line && 0 != *line)
  {
    if ( '=' == *line) { line++; break; }
    if (isspace(*line)) { line++; continue; }
    param += *line++;
  }
  while(line && 0 != *line)
  {
    if ( ';' == *line) { line++; break; }
    if (isspace(*line)) { line++; continue; }
    value += *line++;
  }

  for (int i=0; funcTable[i].param; i++)
  {
    if (0 == _tcscmp(funcTable[i].param, param))
    {
      (*funcTable[i].SetParam)(this, value);
      break;
    }
  }
}

