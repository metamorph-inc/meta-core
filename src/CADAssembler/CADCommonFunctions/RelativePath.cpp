#include "RelativePath.h"
#include <locale>
#include <iostream>


namespace isis_CADCommon
{

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string RelativePath_ConvertToUpperCase(const std::string &in_String)
	{
		std::string temp_string(in_String);
		std::locale loc;
		for (std::string::iterator p = temp_string.begin(); temp_string.end() != p; ++p)    *p = toupper(*p, loc);
		return temp_string;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	bool FindRelativePath_From_B_to_A ( const std::string	&in_Path_A, 
									    const std::string	&in_Path_B,
											  std::string	&out_RelativePath,
										bool				in_DefineRelativePathWithBackSlashes )
	{
		out_RelativePath = "";

		// Check for null strings
		if ( in_Path_A.size() == 0 || in_Path_B.size() == 0 ) return false;		

		std::string path_A = RelativePath_ConvertToUpperCase(in_Path_A);
		std::string path_B = RelativePath_ConvertToUpperCase(in_Path_B);

		// Replace any forward slashes ('/') with a back slashes ('\')
		for ( std::size_t i = 0; i < path_A.size(); ++i ) if ( path_A[i] == '/') path_A[i] = '\\';
		for ( std::size_t i = 0; i < path_B.size(); ++i ) if ( path_B[i] == '/') path_B[i] = '\\';

		// Replace  with a back slashes ('\')
		for ( std::size_t i = 0; i < path_A.size(); ++i ) if ( path_A[i] == '/') path_A[i] = '\\';
		for ( std::size_t i = 0; i < path_B.size(); ++i ) if ( path_B[i] == '/') path_B[i] = '\\';

		// Remove parentheses
		std::string tempString;
		for ( std::size_t i = 0; i < path_A.size(); ++i ) if ( path_A[i] != '\"') tempString += path_A[i];
		path_A = tempString;
		tempString = "";
		for ( std::size_t i = 0; i < path_B.size(); ++i ) if ( path_B[i] != '\"') tempString += path_B[i];
		path_B = tempString;

		// For consistency, add a closing back slash ('\') if not already there
		if ( path_A[path_A.size() - 1] != '\\' ) path_A += "\\";
		if ( path_B[path_B.size() - 1] != '\\' ) path_B += "\\";

		//std::cout << std::endl << "FindRelativePath_From_B_to_A, path_A " << path_A;
		//std::cout << std::endl << "FindRelativePath_From_B_to_A, path_B " << path_B;

		// See if A is subset of B where A starts at position 0
		if ( path_B.find( path_A) !=  0  ) return false;

		// At this point we know that B defines a path that is under/subordinate to A
		
		// Get the portion of B that is surbordinate to A
		std::string subordinate = path_B.substr(path_A.size() );
		//std::cout << std::endl << "FindRelativePath_From_B_to_A, subordinate: " << subordinate;

		// Count the number of back slashes '\'
		int numBackSlashes = 0;
		for each ( char i in subordinate ) if ( i == '\\' ) ++ numBackSlashes;

		// Delimiter string
		std::string slashDelimiterString;
		if ( in_DefineRelativePathWithBackSlashes ) 
			slashDelimiterString = "\\";
		else
			slashDelimiterString = "/";

		// Build relative path
		if ( numBackSlashes == 0 ) 
			out_RelativePath = "." + slashDelimiterString;
		else
			for ( int i = 0; i < numBackSlashes; ++i )  out_RelativePath += ".." + slashDelimiterString;
		
		return true;
	}

}