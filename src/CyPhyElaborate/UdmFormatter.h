#pragma once

#include <string>
#include <afxwin.h>
#include "UdmBase.h"

namespace GMEConsole
{
	const COLORREF CLOUDBLUE = RGB(128, 184, 223);
	const COLORREF WHITE = RGB(255, 255, 255);
	const COLORREF BLACK = RGB(1, 1, 1);
	const COLORREF DARKGRAY = RGB(128, 128, 128);
	const COLORREF LIGHTGRAY = RGB(210, 210, 210);
	const COLORREF GREEN = RGB(185, 245, 148);
	const COLORREF RED = RGB(186, 12, 72);


	class Formatter
	{
	public:
		static std::string MakeBold(const std::string & text){return std::string("<b>") + text +"</b>";}
		static std::string MakeItalic(const std::string & text){return std::string("<i>") + text +"</i>";}
		static std::string MakeUnderline(const std::string & text){return std::string("<u>") + text +"</u>";}
		static std::string MakeHyperlink(const std::string & text, const std::string& url){return std::string("<a href=\"") + url + "\">" + text +"</a>";}
		static std::string MakeObjectHyperlink(const std::string & text, const Udm::Object& object);
		static std::string MakeColored(const std::string & text, COLORREF color);

	};
}
/*
"<A HREF=\"mga:" + id +"\">" + nm + "</A>";
*/
