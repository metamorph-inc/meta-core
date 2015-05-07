#include "stdafx.h"
#include "UdmFormatter.h"
#include <sstream>
#include <iomanip>

using namespace std;

namespace GMEConsole
{
	std::string Formatter::MakeObjectHyperlink(const std::string & text, const Udm::Object& object)
	{
		// Number magic, see uniqueId_type GmeObject::uniqueId() const 
		unsigned long id = object.uniqueId(), c, p;
		p=id%100000000;
		c=id/100000000;
		c+=100;
		ostringstream ostr; ostr <<"<a href=\"mga:id-" << hex << setfill('0')<<setw(4) << c <<'-'<< setw(8)<< p << "\">" << text << "</a>";
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