#ifndef CC_MISCELLANEOUS_FUNCTIONS_H
#define CC_MISCELLANEOUS_FUNCTIONS_H

#include <string>
#include <stdio.h>
#include <vector>
#include <locale>
#include <iostream>

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


	//	Description: 
	//		This function computes the relative path from in_Path_B to in_Path_A if the path exists.
	//
	//		if in_Path_B defines a path subordinate to in_Path_A
	//			then
	//				out_RelativePath = “..\..\”    \\ as appropriate
	//				return true
	//			else
	//				out_RelativePath = “”    \\ null string
	//				return false
	//
	//		Notes: 
	//			1. The delimiter separating directories can be a back slash or forward slash.
	//			2. This input strings are treated in a case insensitive manner (e.g. "aaa" == "AaA".
	//  
	//		Examples:
	//			in_Path_A			in_Path_B							out_RelativePath	in_DefineRelativePath	return
	//																						WithBackSlashes
	//			------------------	-----------------------------		----------------	---------------------	-----------
	//			C:\META/Proj_A		C:\META\Proj_A\results\abcd			..\..\				true					true
	//			C:\META/Proj_A		C:\META\Proj_A\results\abcd			../../				false					true
	//
	//			A trailing forward/backward slash is optional
	//			C:\META/Proj_A/		C:\META\Proj_A\results\abcd"		..\..\				true					true
	//
	//			C:\META/Proj_A		C:\META\Proj_A/						.\					true					true
	//
	//			Space are allowed
	//			C:\META/P r o j A\	 C:\META\P r o j A\results\abcd\	..\..\				true					true
	//
	//			Parentheses are ignored (i.e. removed before processing)
	//			WARNING -	This means you could have an invalid input path because the parentheses do not match;
	//						however this function would still return a relative path.
	//			"C:\META/Proj_A\"	"C:\META\Proj_A\results\abcd\"		..\..\				true					true
	//
	//			If you do the following, it is your responsibility to assure that the drives are the same.
	//			\META\Proj_A\		\META\Proj_A\results\abcd\x			..\..\..\			true					true
	//
	//			NULL String			NULL String							NULL String			true					false
	//
	//	Pre-Conditions:
	//		None
	//
	//	Post-Conditions:
	//		The following variables set as described above in the "Description" section above.
	//			out_RelativePath
	//			out_RelativePathFound
	//
	//		This function does not throw exceptions.
	//		This funcdtion is thread safe.
	bool FindRelativePath_From_B_to_A ( const std::string	&in_Path_A, 
									    const std::string	&in_Path_B,
											  std::string	&out_RelativePath,
										bool				in_DefineRelativePathWithBackSlashes = true );

}  // end namespace isis_CADCommon
#endif