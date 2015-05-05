#include <UIFunctions.h>
#include <ProUIMessage.h>
#include <ProArray.h>
#include <string>
using namespace std;

namespace isis
{

	void CreoMessageDialog(const wchar_t* msg, ProUIMessageType type)
	{
		ProUIMessageButton* buttons;
		ProUIMessageButton user_choice;
		ProArrayAlloc (1, sizeof (ProUIMessageButton), 1, (ProArray*)&buttons);
		buttons [0] = PRO_UI_MESSAGE_OK;
		ProUIMessageDialogDisplay(type, L"The following errors has been encountered during model generation:", (wchar_t*)msg, buttons, PRO_UI_MESSAGE_OK, &user_choice);
		//ProArrayFree((ProArray*)buttons);
	}

	void CreoMessageDialog(const string msg, ProUIMessageType type)
	{
		wstring wmsg(msg.begin(),msg.end());
		CreoMessageDialog(wmsg.c_str(), type);
	}

}