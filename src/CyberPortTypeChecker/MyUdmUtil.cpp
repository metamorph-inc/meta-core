#include "UdmConfig.h"

namespace MyUdmUtil
{
	CString getHyperLinkPath_CString(Udm::Object focusObject)
	{
		CString hyperMsg = "";
		hyperMsg.Format("<a href=\"mga:%s\">%s</a>", UdmGme::UdmId2GmeId(focusObject.uniqueId()).c_str(), focusObject.getPath2().c_str()); 
		return hyperMsg;
	}
	
	std::string getHyperLinkPath_StdString(Udm::Object focusObject)
	{
		return std::string(getHyperLinkPath_CString(focusObject));
	}
}