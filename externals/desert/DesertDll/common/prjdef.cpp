
#include "prjdef.h"

#define INTERFACE_DIR 	"\\etc"
#define SW_COMP_DIR		"\\sw_component"
#define HW_COMP_DIR		"\\hw_component"
#define SYSTEM_DIR		"\\system"
#define RUN_DIR			"\\run"

#define BUFLEN	1024
static char lineBuffer[BUFLEN];

void
SetAcsInstallPath(CPrjDef *prj, const char *value)
{
  prj->acsInstallPath = value;
  prj->interfacePath = prj->acsInstallPath + INTERFACE_DIR;
}

void
SetProjectName(CPrjDef *prj, const char *value)
{
  prj->projectName = value;
}

void
SetProjectPath(CPrjDef *prj, const char *value)
{
  prj->projectPath = value;
  prj->swComponentPath = prj->projectPath + SW_COMP_DIR;
  prj->hwComponentPath = prj->projectPath + HW_COMP_DIR;
  prj->systemPath = prj->projectPath + SYSTEM_DIR;
  prj->runPath = prj->projectPath + RUN_DIR;
}

struct _prjDeftag
{
  char *param;
  void (*SetParam)(CPrjDef *prj, const char *value);
} funcTable[] =
{
  { "ACS_Install_Path", SetAcsInstallPath },
  { "Project_Name", SetProjectName },
  { "Project_Path", SetProjectPath },
  { NULL, NULL }
};


bool
CPrjDef::
Load(const char *name)
{
  char *line = lineBuffer;
  FILE *prjFile = fopen(name, "rt");
  if (!prjFile)
  {
    Warning("CPrjDef::Load", "Unable to open file <%s>. Using the default directory structure", name);
    // set the defaults - assuming that we are running from models directory
    acsInstallPath = "..";
    interfacePath = ".";
    projectName = "";
    SetProjectPath(this, "..\\runtime");
    return false;
  }

  while(fgets(line, BUFLEN, prjFile))
  {
    if (line[0] == '#') continue;
    if (strlen(line) == 0) continue;
    ParseLine(line);
  }
  fclose(prjFile);
  return true;
}


void
CPrjDef::
ParseLine(char *line)
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
    if (0 == strcmp(funcTable[i].param, param))
    {
      (*funcTable[i].SetParam)(this, value);
      break;
    }
  }
}

