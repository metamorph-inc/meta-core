#include "cc_CommonUtilities.h"
#include <fstream>
#include <locale>
#include "cc_WindowsFunctions.h"
#include	  "cc_MultiFormatString.h"
#include <Windows.h>

//#include <iostream>

namespace isis
{


	::boost::filesystem::path SetupWorkingDirectory( std::string & inout_workingDirectory ) 
	{
	   ::boost::filesystem::current_path(inout_workingDirectory);
	   ::boost::filesystem::path workingDir = ::boost::filesystem::current_path();
	   inout_workingDirectory = workingDir.generic_string();
	   return workingDir;
	}

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
		// TODO test == -1 and modify clients to accept the error
		_unlink(in_PathAndFileName_or_FileName.c_str());
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	void IfFileExists_DeleteFile(const std::string &in_PathAndFileName_or_FileName)
	{
		// TODO test == -1 and modify clients to accept the error
		_unlink(in_PathAndFileName_or_FileName.c_str());
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
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	std::string META_PATH()
	{
		std::string metapath;
		HKEY software_meta;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\META", 0, KEY_READ, &software_meta) == ERROR_SUCCESS)
		{
			BYTE data[MAX_PATH];
			DWORD type, size = sizeof(data) / sizeof(data[0]);
			if (RegQueryValueExA(software_meta, "META_PATH", 0, &type, data, &size) == ERROR_SUCCESS)
			{
				metapath = std::string(data, data + strnlen((const char*)data, size));
			}
			RegCloseKey(software_meta);
		}
		return metapath;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////

	// e.g "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\material_library.json"
	std::string MaterialsLibrary_PathAndFileName()
	{
		auto dev_path = boost::filesystem::path(META_PATH()) / "models" / "MaterialLibrary" / "material_library.json";
		OutputDebugStringA(dev_path.string().c_str());
		OutputDebugStringA("\n");
		if (boost::filesystem::is_regular_file(dev_path))
		{
			return dev_path.string();
		}

		std::wstring publicDocuments_wide;

		// Public Documents e.g. "C:\\Users\\Public\\Documents\\META Documents"
		isis_CADCommon::GetPublicDocuments(publicDocuments_wide);
		isis::MultiFormatString publicDocuments_multiformat(publicDocuments_wide.c_str() );

		// Materials Library
		// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\material_library.json"

		return (std::string)publicDocuments_multiformat + std::string("\\META Documents\\MaterialLibrary\\material_library.json");	;
	}

} // end namespace isis