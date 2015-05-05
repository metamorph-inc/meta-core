#ifndef UIFUNCTIONS_H
#define UIFUNCTIONS_H
#include <string>
#include <ProUIMessage.h>

namespace isis
{
	void CreoMessageDialog(const std::string msg, ProUIMessageType type = PROUIMESSAGE_ERROR);
	void CreoMessageDialog(const wchar_t* msg, ProUIMessageType type = PROUIMESSAGE_ERROR);
}


#endif