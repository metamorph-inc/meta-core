#ifndef RELATIVE_PATH_H
#define RELATIVE_PATH_H

#include <string>

namespace isis_CADCommon
{

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


} // end namespace isis

#endif
