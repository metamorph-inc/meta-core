#include "cc_CommonUtilities.h"
#include <fstream>
#include <locale>

//#include <iostream>

namespace isis
{

	std::string	EncloseStringInDoubleQuotes( const std::string &in_String )
	{
		std::string OutString;	

			OutString = "\"" + in_String + "\"";

		return OutString;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ConvertToUpperCase(const std::string &in_String)
	{
		std::string temp_string(in_String);
		std::locale loc;
		for (std::string::iterator p = temp_string.begin(); temp_string.end() != p; ++p)    *p = toupper(*p, loc);
		return temp_string;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ConvertToLowerCase(const std::string &in_String)
	{
		std::string temp_string(in_String);
		std::locale loc;
		for (std::string::iterator p = temp_string.begin(); temp_string.end() != p; ++p)    *p = tolower(*p, loc);
		return temp_string;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsANumber(const std::string &in_String)
	{
		if ( in_String.size() == 0 ) return false;

		for ( std::string::const_iterator i( in_String.begin()); i != in_String.end(); ++i)
		{
			if ( isdigit(*i) == 0 ) // not a digit
			{
				if ( !( *i == '.' ||  *i == '+' || *i == '-' || *i == ' ' ))  return false;
			}
		}

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	double ConvertToDouble(const std::string &in_String) throw (isis::application_exception)
	{

		if ( !IsANumber(in_String) )
		{
			std::string TempError = "Function ConvertToDouble, Could not convert string \"" + in_String + "\" to a double.";
			throw isis::application_exception(TempError.c_str());
		}

		return atof(in_String.c_str() );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////

	void isis_DeleteFile(const std::string &in_PathAndFileName_or_FileName)
	{

		std::string DeleteFileName = EncloseStringInDoubleQuotes( in_PathAndFileName_or_FileName );

		// std::cout << std::endl << "DeleteFile ----> File: " << DeleteFileName;
		char  DeleteInstruction[1024];
		strcpy( DeleteInstruction, "del ");
		strcat( DeleteInstruction, (char *)DeleteFileName.c_str());
		system(DeleteInstruction);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	void IfFileExists_DeleteFile(const std::string &in_PathAndFileName_or_FileName)
	{

		std::string DeleteFileName = EncloseStringInDoubleQuotes( in_PathAndFileName_or_FileName );

		//std::cout << std::endl << "DeleteFile ----> File: " << DeleteFileName;
		char  DeleteInstruction[1024];
		strcpy( DeleteInstruction, "IF EXIST ");
		strcat( DeleteInstruction, (char *)DeleteFileName.c_str());
		strcat( DeleteInstruction, " DEL ");
		strcat( DeleteInstruction, (char *)DeleteFileName.c_str());
		//std::cout << std::endl << "IfFileExists_DeleteFile: " << DeleteInstruction;
		system(DeleteInstruction);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	void ExecuteSystemCommand(const std::string &in_Instruction)
	{
		char  Instruction[1024];
		strcpy( Instruction, in_Instruction.c_str());
		system(Instruction);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	void CopyFileIsis(	const std::string &in_From_PathAndFileName, 
					const std::string &in_To_Path_or_PathAndFileName )
	{
		char  CopyInstruction[1024];

		std::string From;
		std::string To;

		From = EncloseStringInDoubleQuotes( in_From_PathAndFileName );

		To = EncloseStringInDoubleQuotes(in_To_Path_or_PathAndFileName);

		//std::cout << std::endl << "CopyFile ----> From: " << From;
		//std::cout << std::endl << "CopyFile ----> To:   " << To;

		strcpy( CopyInstruction, "copy /y ");
		//strcpy( CopyInstruction, "xcopy ");	xcopy with /q did not suppress the file copy message.
		strcat( CopyInstruction, (char *)From.c_str());
		strcat( CopyInstruction, " ");
		strcat( CopyInstruction, (char *)To.c_str());
		// strcat( CopyInstruction, " /y /q"); 
		system(CopyInstruction);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	bool FileExists(const char * in_PathAndFilename) 
	{ 
		std::ofstream TestFile;
		
		// Try to open the file in read mode.
		TestFile.open(in_PathAndFilename, std::ios::in );
		
		if (TestFile.is_open())
		{
			TestFile.close();
			return true;
		}
		else
		{
			return false;
		}
	} 
	

} // end namespace isis