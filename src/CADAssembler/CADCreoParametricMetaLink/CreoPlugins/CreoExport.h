#ifndef _CREOEXPORT_H_
#define _CREOEXPORT_H_

#include <ProSizeConst.h>
#include <ProObjects.h>

#ifdef __cplusplus
extern "C" {
#endif

// Callbacks for the different plugin buttons
ProError DoCreateComponent();
ProError DoResync();
ProError DoConnectComponents();
ProError DoRemoveComponent();
ProError DoAddComponent();
ProError DoCreateConnector();
ProError DoCreateAnalysisPoints();
ProError DoDumpPosition();
ProError DoDatumResolve();

int UpdateComponent(ProMdl mdl);
int Resync(ProMdl mdl);
int RequestComponentList(char* dialog);
int CreateNewComponent(ProMdl mdl);

ProError CompAddDialogCreate();
void CompTreeSelectAction (char* dialog, char* component, ProAppData data);
void ErrorDialogStr(const std::string msg);
void ErrorDialog(const wchar_t* msg);

#ifdef __cplusplus
}
#endif

std::string getStrFromWstr(const wchar_t* ws);

#endif