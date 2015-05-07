#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cctype>	// for isalpha, isalnum

class SFUtils
{
public:
	static std::string convertToCPlusPlusName( const std::string& name)
	{
/*		unsigned char unaut= 0xfc;
		unsigned char unaut= 'ü';
		std::cout << unaut;
		if ( isalpha( unaut))
			std::cout << " is alpha."; 
		else 
			std::cout << " is not alpha.";
		std::cout << std::endl;
*/		// empty name
		if ( name.empty())
			return "";
		// name must start with letter
		std::string converted;
		if ( false== isalpha( (unsigned char)name[ 0]))
			converted= "pre";
		// name can contain alphanum characthers and '_'
		for( std::string::const_iterator i= name.begin(); i!= name.end(); ++i)
		{
			if ( isalnum( (unsigned char)*i) || (*i== '_'))
				converted+= *i;
			else
				converted+= '_';
		}
		return converted;
	}
};

#endif //UTILS_H
