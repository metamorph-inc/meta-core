#ifndef MISCELLANEOUS_FUNCTIONS_H
#define MISCELLANEOUS_FUNCTIONS_H

#include <string>
#include <stdio.h>
#include <vector>

namespace isis_CADCommon
{

	std::string IntegerToString( int in_Integer);
	
	inline void tokenize_strtok(std::vector<std::string> &tokens, std::string input, std::string delimiters)
	{
		int wordCount = 0;
		char *nextWordPtr = NULL;
		char *context = NULL;

		nextWordPtr = strtok_s(&input[0], delimiters.c_str(), &context);		// split using space as divider
		while (nextWordPtr != NULL) 
		{
			tokens.push_back(nextWordPtr);
			nextWordPtr = strtok_s(NULL, delimiters.c_str(), &context);
		}
	}

	inline void stringTokenize(const std::string& str,
		std::vector<std::string>& tokens,
		std::string delimiters)
	{
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (	std::string::npos != pos || 	std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			std::string substring=str.substr(lastPos, pos - lastPos);
			//LOGGER("SUBSTRING is %s",substring.c_str());
			//std::cout<<substring;
			tokens.push_back(substring);
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

	std::string GetDayMonthTimeYear();

}  // end namespace isis
#endif