#include "Helper.hpp"
#include <iostream>

std::string convertToIdentifierName( const std::string &input ) {

	std::string identifierName;

	std::string::const_iterator strItr = input.begin();

	char character = *strItr;
	if (  !isalpha( character ) && character != '_'  ) identifierName += "_";

	while( strItr != input.end() ) {
		character = *strItr;
		if (  !isalnum( character ) && character != '_'  ) identifierName += "_";
		else                                               identifierName += character;

		++strItr;
	}

	return identifierName;
}
