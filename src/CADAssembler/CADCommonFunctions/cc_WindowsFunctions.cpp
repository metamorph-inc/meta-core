#include "cc_WindowsFunctions.h"
#include "Windows.h"
#include <sstream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <Shlobj.h>



namespace isis_CADCommon
{
	// the ability to get the current working directory is provided by 
	//  ::boost::filesystem::current_path();

	/*
	std::string isis_GetCurrentDirectoryA()
	{
		char buffer[MAX_PATH+1];

		DWORD errorCode;

		errorCode = GetCurrentDirectoryA(MAX_PATH, buffer);

		if ( errorCode == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - GetCurrentDirectoryA, returned a zero error code, which indicates that the function failed.";
				throw isis::application_exception(errorString.str());			
		}
		
		return std::string(buffer);
	}
	*/


	void GetPublicDocuments(std::wstring& out_PublicDocumentsPath)
			throw (isis::application_exception)
	{
		//need a wchar_t* because that's what SHGetKnownFolderPath takes
		wchar_t* path = 0;
		SHGetKnownFolderPath(FOLDERID_PublicDocuments, 0, NULL, &path);
		//need wstringstream to convert to wstring
		std::wstringstream stream;
		stream << path;
		//need this to clean up memory for wchar_t*
		::CoTaskMemFree(static_cast<void*>(path));
		//populate output parameter
		out_PublicDocumentsPath = stream.str();

		if ( out_PublicDocumentsPath.size() == 0 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - GetPublicDocuments, did not find the public document folder.";
			throw isis::application_exception(errorString.str());		
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////	
	bool DirectoryExists(const std::string& in_DirectoryName)
	{
		// http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi

		DWORD ftyp = GetFileAttributesA(in_DirectoryName.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES) return false;  //something is wrong with your path!
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;   // this is a directory!

	  return false;    // this is not a directory!
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	void isis_CreateDirectory(const std::string dirname)
	{
		CreateDirectoryA(dirname.c_str(), 0);
	}

} // END namespace isis_CADCommon

