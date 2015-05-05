#include "stdafx.h"
#include "UdmFormatter.h"
#include <sstream>
#include <iomanip>
#include "UdmGme.h"

using namespace std;

namespace GMEConsole
{
	std::string Formatter::MakeObjectHyperlink(const std::string & text, const Udm::Object& object)
	{
		ostringstream ostr;
		ostr <<"<a href=\"mga:" << UdmGme::UdmId2GmeId(object.uniqueId()) << "\">" << text << "</a>";
		return ostr.str();
	}

	std::string Formatter::MakeColored(const std::string & text, COLORREF color)
	{
		ostringstream ostr; ostr << "<font color=\"#" << hex << setfill('0') << setw(2) << (int)GetRValue(color) 
			<<setw(2) << (int)GetGValue(color) <<setw(2) << (int)GetBValue(color) << "\">" << text <<"</font>";
		
		return ostr.str();
	}
}

/*
<font color="#ffffff">
My very first html page RGB
</font>
*/