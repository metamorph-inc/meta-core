#ifndef CC_WINDOWS_FUNCTIONS_H
#define	CC_WINDOWS_FUNCTIONS_H

#pragma warning( disable : 4290 )  // a future feature : exception specification, i.e. throw

#include<string>
#include "isis_application_exception.h"

namespace isis_CADCommon
{
	// std::string isis_GetCurrentDirectoryA() throw (isis::application_exception); 


	// Get the location of the public folder.
	// Example, C:\Users\Public\Documents
	// If directory not found, then isis::application_exception would be thrown.
	void GetPublicDocuments(std::wstring& out_PublicDocumentsPath) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// If in_DirectoryName is an actual directory on the system
	//		then
	//			return true
	// If in_DirectoryName points to a file or points to nothing that exists in file system
	//		then
	//			return false
	//
	// If in_DirectoryName contains spaces, do NOT enclose the string in  parentheses.
	// Passing the following string will work:
	//		C:\Users\rowens\Documents\Microsoft Research
	//		e.g string myString =  "C:\\Users\\rowens\\Documents\\Microsoft Research"
	//
	//	Passing the following string will NOT work:
	//		"C:\\Users\\rowens\\Documents\\Microsoft Research\"
	//		e.g string myString = "\"C:\\Users\\rowens\\Documents\\Microsoft Research\""
	bool DirectoryExists(const std::string& in_DirectoryName);

	// Create a directory
	void isis_CreateDirectory(const std::string dirname);

} // END namespace isis_CADCommon

#endif