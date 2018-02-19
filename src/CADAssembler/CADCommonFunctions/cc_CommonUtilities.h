/*! \file CommonUtilities.h
    \brief  Common utilities used byCAD applications.

	Common utilities that support operations such as copying, deleting... files/models.
*/

#ifndef CC_COMMON_UTILITIES_H
#define CC_COMMON_UTILITIES_H

#include "isis_application_exception.h"
#include <boost/filesystem.hpp>
#include <string>


namespace isis
{
		// This function hase a side effect, it changed the current working directory, and the input parameter.
		::boost::filesystem::path SetupWorkingDirectory( std::string & inout_workingDirectory );

		std::string ConvertToUpperCase(const std::string &in_String);
		std::string ConvertToLowerCase(const std::string &in_String);

		// Number with at least one digit and possibly "+", "-", and/or "."
		bool IsANumber(const std::string &in_String);

		// throw exception if in_String cannot be coverted to a double.
		double ConvertToDouble(const std::string &in_String) throw (isis::application_exception);


		// in_PathAndFileName_or_FileName must not contain double quotes (i.e. “);
		// if in_PathAndFileName_or_FileName will be enclosed in double quotes. This is 
		//	necessary because an error would occur if the string contained spaces, dashes, commas...
		void isis_DeleteFile(const std::string &in_FileName);


		// in_PathAndFileName_or_FileName must not contain double quotes (i.e. “);
		// if in_PathAndFileName_or_FileName will be enclosed in double quotes.  This is 
		//	necessary because an error would occur if the string contained spaces, dashes, commas...
		void IfFileExists_DeleteFile(const std::string &in_PathAndFileName_or_FileName);

		void ExecuteSystemCommand(const std::string &in_Instruction);


		// Description:
		//		Copy in_From_PathAndFileName to in_To_Path_or_PathAndFileName
		//
		// Pre-Conditoins
		//		The input strings must not contain double quotes (i.e. “);
		//		in_From_PathAndFileName must exist.
		//		in_To_Path_or_PathAndFileName must contain a valid path and optionally a file name.
		//
		// Post-Conditions
		//		The string(s) will be enclosed in double quotes.  This is necessary because an error
		//		would occur if the string contained spaces, dashes, commas...
		//		If the files cannot be copied a message will be printed to sysout.  
		//		No exceptions will be returned regardless of success/failure.
		void CopyFileIsis(	const std::string &in_From_PathAndFileName, 
					const std::string &in_To_PathAndFileName );



		//	This program returns true if the file identified by in_PathAndFilename can be 
		//	opened in read mode.  Typically, the file would not exist if it could not be 
		//	opened in read mode; however, it is possible that it could not be opened in 
		//	read mode because the process does not have privileges to read it.  In either 
		//	case, it would be an error condition for assemble_ptc.exe. 
		bool FileExists(const char * in_PathAndFilename) ;


		std::string META_PATH();

		// e.g "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\material_library.json"
		std::string MaterialsLibrary_PathAndFileName();

} // end namespace isis

#endif