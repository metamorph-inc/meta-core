#include "CADSoftwareEnvirUtils.h"
#include <cc_CommonUtilities.h>
#include <windows.h>
#include "cc_WindowsFunctions.h"
//#include <malloc.h>
#include <iostream>
//#include <vector>
//#include <tchar.h>
#include <algorithm>
#include <sstream>
#include <boost/filesystem.hpp>

// #define BUFFER 8192

namespace isis
{		
	const int MAX_KEY_LENGTH =	256;
	//const int MAX_VALUE_NAME  = 16383;  // See http://msdn.microsoft.com/en-us/library/windows/desktop/ms724872(v=vs.85).aspx
	const int MAX_VALUE_NAME  = 2048;   // Note - Set to 2048 because the values we are searching for in
										// HKEY_LOCAL_MACHINE\SOFTWARE\PTC\Creo Parametric\1.0\2011190
										// are typically just MAX_PATH.  Allocated the larger size for safety.  

	// Return empty string if registry value was not found.
	// WARNING - Do not use this function for values larger that 2048.
	//////////////////////////////////////////////////////////////////////////////////////////////// 
	std::string RetrieveRegistryStringValue( const HKEY in_hkey, const std::string &in_Key )
	{
		std::string tempString = "";

		DWORD	pdwType;
		char	pvData[MAX_VALUE_NAME];
		DWORD	pcbData = MAX_VALUE_NAME;

		LSTATUS errorStatus = 
			RegGetValueA(	in_hkey,		// A handle to an open registry key
							NULL,			// subkey of the key specified by the hkey parameter.
							in_Key.c_str(), // The name of the registry value. 
							RRF_RT_REG_SZ,  // Flags that restrict the data type of value to be queried.  RRF_RT_REG_SZ only strings.
							&pdwType,		// A pointer to a variable that receives a code indicating the type of data stored
							pvData,			// A pointer to a buffer that receives the value's data. 
							&pcbData  );		// A pointer to a variable that specifies the size of the buffer pointed to by the pvData 
		
		if ( errorStatus == ERROR_SUCCESS ) tempString = (char *) pvData;

		return tempString;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////// 
	// out_SubKeys will be empty of no sub-keys were found
	void RetrieveRegistryListOfSubkeys( const HKEY in_hkey, std::vector<std::string> &out_SubKeys )
	{
		// The following code is from http://msdn.microsoft.com/en-us/library/ms724256(VS.85).aspx

		CHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
		DWORD    cbName;                   // size of name string 
		CHAR     achClass[MAX_PATH] = "";  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys=0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;                  // number of values for key 
		DWORD    cchMaxValue;              // longest value name 
		DWORD    cbMaxValueData;           // longest value data 
		DWORD    cbSecurityDescriptor;     // size of security descriptor 
		FILETIME ftLastWriteTime;          // last write time 
 
		DWORD i, retCode; 
 
		//WCHAR  achValue[MAX_VALUE_NAME]; 
		//DWORD cchValue = MAX_VALUE_NAME; 
 
		// Get the class name and the value count. 
		retCode = RegQueryInfoKeyA(
			in_hkey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 
 
		// Enumerate the subkeys, until RegEnumKeyEx fails.
    
		if (cSubKeys)
		{
			//printf( "\nNumber of subkeys: %d\n", cSubKeys);

			for (i=0; i<cSubKeys; i++) 
			{ 
				cbName = MAX_KEY_LENGTH;
				retCode = RegEnumKeyExA(in_hkey, i,
						 achKey, 
						 &cbName, 
						 NULL, 
						 NULL, 
						 NULL, 
						 &ftLastWriteTime); 
				if (retCode == ERROR_SUCCESS) 
				{
					//wprintf(TEXT("(%d) %s\n"), i+1, achKey);
					out_SubKeys.push_back(achKey);
				}
			}
		} 

	} 



	//////////////////////////////////////////////////////////////////////////////////////////////// 
	//  Registry Hierarchy
	//	HKEY_LOCAL_MACHINE
	//		SOFTWARE
	//			PTC
	//				Creo Parametric
	//					1.0    // Note - this could be 1.1, 1.2 should pick the highest 1.x number, Note 2.x not supported yet
	//						2011190  // Year build number,  this could be many numbers, should pick the highest number
	//							InstallDir // e.g. C:\Program Files\PTC\Creo 1.0\Parametric
	//							CommonFilesLocation // e.g. C:\Program Files\PTC\Creo 1.0\Common Files\F000							
	//
	//	in_StartingKeyPath - Creo 1.0 = "SOFTWARE\\PTC\\Creo Parametric";
	//  in_StartingKeyPath - Creo 2.0 = "SOFTWARE\\Wow6432Node\\PTC\\Creo Parametric";
	//	in_SupportedVersionPrefixes -			1.  2.  // This means 1.x, 2.x, but only enter 1., 2. 
	//  in_SupportedVersionString_ForErrorMsg - 1.x, 2.x, or 3.x  // currently only support 1.x
	//	out_CreoParametricInstallPath -			This should be short name.  e.g. C:\Progra~1\PTC\CREO1~1.0\PARAME~1\
	//	out_CreoParametricCommMsgExe -			This should be long name.   e.g. C:\Program Files\PTC\Creo 1.0\Common Files\F000\x86e_win64\obj\pro_comm_msg 

	void RetrieveFromRegistryCreoInstallLocations(
								const std::string &in_StartingKeyPath,
								const std::vector<std::string> &in_SupportedVersionPrefixes,
								const std::string			   &in_SupportedVersionString_ForErrorMsg,
								std::string &out_CreoParametricInstallPath,
								std::string &out_CreoParametricCommMsgExe ) throw (isis::application_exception)
	{
		std::string key_Creo_Parametric = in_StartingKeyPath;

		std::string errorMsg_1 =  "Creo Parametric is not installed.  Please install Creo " + in_SupportedVersionString_ForErrorMsg + ". Note - Higher versions are currently not supported.  ";
		std::string errorMsg_2 =  "Creo Parametric is not installed or the registry information for the install is incorrect or the format of the registry information has changed.  If not installed, please install Creo " + in_SupportedVersionString_ForErrorMsg + ". Note - Higher versions are currently not supported.  ";

		std::string key_Temp;
		//////////////////////////////////////////////////////////////////////
		// Retrieve Key to in_StartingKeyPath (e.g. HKEY_LOCAL_MACHINE\SOFTWARE\PTC\Creo Parametric)
		/////////////////////////////////////////////////////////////////////
		HKEY hKey;
		if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			 key_Creo_Parametric.c_str(),
			 0, KEY_READ|KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		{
			std::string TempError =   errorMsg_1 +
				std::string("Could not find registry key HKEY_LOCAL_MACHINE\\") + key_Creo_Parametric; 
			throw isis::application_exception(TempError.c_str());
		}
		///////////////////////////////////////////////////////////////////
		// Get Subkeys of in_StartingKeyPath
		//////////////////////////////////////////////////////////////////
		std::vector<std::string> subKeys_vec;
		RetrieveRegistryListOfSubkeys( hKey, subKeys_vec );
		RegCloseKey(hKey);

		std::vector<std::string> versionNumber_vec;
		for ( std::vector<std::string>::const_iterator i(subKeys_vec.begin()); i != subKeys_vec.end(); ++i )
		{
			//std::cout << std::endl << *i;
			for ( std::vector<std::string>::const_iterator j(in_SupportedVersionPrefixes.begin()); j != in_SupportedVersionPrefixes.end(); ++ j )
			{
				if ( i->find(*j) != i->front())
				{
					versionNumber_vec.push_back(*i);
					break;
				} 
			}
		}

		if ( versionNumber_vec.size() == 0 )
		{
			std::string TempError =   errorMsg_2 + 
				std::string("Could not find registry keys subordinate to HKEY_LOCAL_MACHINE\\") + key_Creo_Parametric; 
			throw isis::application_exception(TempError.c_str());
		}

		// Sort the keys, so that the highest key (highest Creo Version) could be selected.
		std::sort( versionNumber_vec.begin(), versionNumber_vec.end() );

		key_Temp = key_Creo_Parametric + "\\" + versionNumber_vec[versionNumber_vec.size() - 1 ];

		//std::cout << std::endl << key_Temp;

		////////////////////////////////////////////////////////////////////////////////
		// Retrieve next Key (e.g.in_StartingKeyPath\1.0 )
		////////////////////////////////////////////////////////////////////////////////
		if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			 key_Temp.c_str(),
			 0, KEY_READ|KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		{
			std::string TempError =   errorMsg_2 +
				std::string("Could not find key HKEY_LOCAL_MACHINE\\") + key_Temp; 
			throw isis::application_exception(TempError.c_str());
		}
		subKeys_vec.empty();

		/////////////////////////////////////////////////////////////////////////////////////////////
		// Get Subkeys of in_StartingKeyPath\1.0   could be 1.1, 2.0 ...
		/////////////////////////////////////////////////////////////////////////////////////////////
		subKeys_vec.clear();
		RetrieveRegistryListOfSubkeys( hKey, subKeys_vec );
		RegCloseKey(hKey);

		if ( subKeys_vec.size() == 0 )
		{
			std::string TempError =  errorMsg_2 + 
				std::string("Could not find registry keys subordinate to HKEY_LOCAL_MACHINE\\") + key_Temp; 
			throw isis::application_exception(TempError.c_str());
		}


		//for ( std::vector<std::string>::const_iterator i(subKeys_vec.begin()); i != subKeys_vec.end(); ++i )
		//{
		//	std::cout << std::endl << *i;
		//}

		// Sort the keys, so that the highest key (highest Creo Version) could be selected.
		std::sort( subKeys_vec.begin(), subKeys_vec.end() );

		key_Temp = key_Temp + "\\" + subKeys_vec[subKeys_vec.size() - 1 ];

		//std::cout << std::endl << key_Temp;

		////////////////////////////////////////////////////////////////////////////////
		// Retrieve next Key (e.g. in_StartingKeyPath\2.0\2011109 )
		////////////////////////////////////////////////////////////////////////////////
		if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			 key_Temp.c_str(),
			 0, KEY_READ|KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		{
			std::string TempError =   errorMsg_2 +
				std::string("Could not find key HKEY_LOCAL_MACHINE\\") + key_Temp; 
			throw isis::application_exception(TempError.c_str());
		}


		////////////////////////////////
		// Get CommonFilesLocation
		////////////////////////////////
		std::string commonFilesLocation = RetrieveRegistryStringValue( hKey, "CommonFilesLocation" );

		if ( commonFilesLocation.size() == 0 )
		{
			std::string TempError =   errorMsg_2 + 
				std::string("Could not find registry value \"CommonFilesLocation\" subordinate to HKEY_LOCAL_MACHINE\\") + key_Temp; 
			throw isis::application_exception(TempError.c_str());
		}

		//std::cout << std::endl << "CommonFilesLocation: " <<  commonFilesLocation;

		////////////////////////////////
		// Get InstallDir
		////////////////////////////////
		std::string installDir = RetrieveRegistryStringValue( hKey, "InstallDir" );
		RegCloseKey(hKey);

		if ( commonFilesLocation.size() == 0 )
		{
			std::string TempError =   errorMsg_2 +
				std::string("Could not find registry value \"InstallDir\" subordinate to HKEY_LOCAL_MACHINE\\") + key_Temp; 
			throw isis::application_exception(TempError.c_str());
		}

		//std::cout << std::endl << "InstallDir: " <<  installDir;
		
		out_CreoParametricInstallPath = installDir + "\\";

		out_CreoParametricCommMsgExe =  commonFilesLocation + "\\x86e_win64\\obj\\pro_comm_msg";
	
		//std::cout << std::endl << "out_CreoParametricInstallPath: " <<  out_CreoParametricInstallPath;
		//std::cout << std::endl << "out_CreoParametricCommMsgExe: " <<  out_CreoParametricCommMsgExe;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Example return values
	//	if ( C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf exists)
	//		C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf %*
	//	else
	//		C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input		
	std::string BuildCreoStartCommand( bool					in_GraphicsModeOn,
									   bool					in_CreoExceptInputFromThisProgramAndCreoUI, 
									   const std::string	&in_CreoParametricInstallPath )
	{
		std::string creoStartCommand				 =  "bin\\parametric.exe";
		std::string creoStartCommandSuffix			 = "";
		std::string creoStartCommand_psFile_FileName =  "bin\\parametric.psf";
		std::string creoStartCommandSuffix_psfFile	 =  " %*";

		if ( !in_GraphicsModeOn ) creoStartCommandSuffix += " -g:no_graphics";
		if ( !in_CreoExceptInputFromThisProgramAndCreoUI ) creoStartCommandSuffix += " -i:rpc_input";

		std::string creoStartCommand_psfFile_PathAndFileName = in_CreoParametricInstallPath + creoStartCommand_psFile_FileName ;

		// Check if \PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf exists
		if (::boost::filesystem::exists(creoStartCommand_psfFile_PathAndFileName) )
			//  C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf %*
			return  "\"" + in_CreoParametricInstallPath +
			creoStartCommand + "\" " + creoStartCommandSuffix +
					" \"" + creoStartCommand_psfFile_PathAndFileName + "\" " +
					creoStartCommandSuffix_psfFile;
		else
			//  C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input 
			return in_CreoParametricInstallPath + creoStartCommand + " " + creoStartCommandSuffix;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetupCreoEnvironmentVariables(
								bool				in_GraphicsModeOn,
								bool				in_CreoExceptInputFromThisProgramAndCreoUI,
								std::string &out_CreoStartCommand ) throw (isis::application_exception)
	{
		//std::cout << std::endl << "************* IN SetupCreoEnvironmentVariables";
	
		bool readEnvVariablesFromRegistry = true;

		char *EnvVariable_USE_ENVIR_VARS;
		EnvVariable_USE_ENVIR_VARS = getenv ("CREO_PARAMETRIC_USE_ENVIR_VARS");

		if ( EnvVariable_USE_ENVIR_VARS != NULL )
		{
			std::string tempString_1 = EnvVariable_USE_ENVIR_VARS;
			std::string tempString_2 = isis::ConvertToUpperCase(tempString_1);
			std::remove( tempString_2.begin(), tempString_2.end(), ' '); 
			if ( tempString_2 == "TRUE" ) readEnvVariablesFromRegistry = false;
		}

		if ( readEnvVariablesFromRegistry )
		{
			std::string creoParametricInstallPath;
			std::string creoParametricCommMsgExe;
			
			try // Creo 3.0
			{   
				std::string in_StartingKeyPath = "SOFTWARE\\Wow6432Node\\PTC\\PTC Creo Parametric";
				std::vector<std::string>	supportedVersionPrefixes;
				std::string					supportedVersionString_ForErrorMsg = "3.x";  // e.g. 1.x, 2.x, or 3.x  
				supportedVersionPrefixes.push_back("3.");  // 
				isis::RetrieveFromRegistryCreoInstallLocations( in_StartingKeyPath,
																supportedVersionPrefixes,
																supportedVersionString_ForErrorMsg,
																creoParametricInstallPath, 
																creoParametricCommMsgExe );
			}

			catch ( isis::application_exception& ex_Creo_3 )
			{
				std::stringstream errorString;
				errorString <<
						"Could not find an installation of Creo 3.0.  Creo 2.0 and earlier are no longer supported, "
						<< std::endl << ex_Creo_3.what();
						throw isis::application_exception(errorString.str());
			}

			// 4/26/2016, We are now compiling with Creo 3, which means this program will only
			// work with Creo 3.0 or higher (e.g. Creo 4.0 when it is released)
			// Leave the following code commented out so that Creo 4.0 can easily be supported when available.

			/**
			catch ( isis::application_exception& ex_Creo_3 )
			{
				try // Creo 2.0
				{
				std::string in_StartingKeyPath = "SOFTWARE\\Wow6432Node\\PTC\\Creo Parametric";
				std::vector<std::string>	supportedVersionPrefixes;
				std::string					supportedVersionString_ForErrorMsg = "2.x";  // e.g. 1.x, 2.x, or 3.x  
				supportedVersionPrefixes.push_back("2.");  // 
				isis::RetrieveFromRegistryCreoInstallLocations( in_StartingKeyPath,
																supportedVersionPrefixes,
																supportedVersionString_ForErrorMsg,
																creoParametricInstallPath, 
																creoParametricCommMsgExe );
				}
				catch ( isis::application_exception& ex_Creo_2 )
				{
					std::stringstream errorString;
					errorString <<
							"Could not find an installation of Creo 3.0 or Creo 2.0, "
							<< std::endl << ex_Creo_3.what() 
							<< std::endl << ex_Creo_2.what(); 
							throw isis::application_exception(errorString.str());
				}
			}	
			****/
			_putenv_s( "PRO_COMM_MSG_EXE", creoParametricCommMsgExe.c_str() );
			char *EnvVariableCOMM_MSG_EXE  = getenv ("PRO_COMM_MSG_EXE");
			//std::cout << std::endl << "************* PRO_COMM_MSG_EXE: " << creoParametricCommMsgExe;
			//std::cout << std::endl << "************* getenv (\"PRO_COMM_MSG_EXE\"): " << EnvVariableCOMM_MSG_EXE;

			out_CreoStartCommand = 	BuildCreoStartCommand(	in_GraphicsModeOn,
															in_CreoExceptInputFromThisProgramAndCreoUI, 
															creoParametricInstallPath );
		}
		else  // For this case, the system environment variables must be set.
		{
			//std::cout << std::endl << "************* IN Make sure environment variables";

			// Make sure environment variables are set 
			// (i.e. CREO_PARAMETRIC_INSTALL_PATH, CREO_PARAMETRIC_COMM_MSG_EXE	)
			char *EnvVariable_INSTALL_PATH, *EnvVariableCOMM_MSG_EXE;
			EnvVariable_INSTALL_PATH = getenv ("CREO_PARAMETRIC_INSTALL_PATH");
			EnvVariableCOMM_MSG_EXE  = getenv ("CREO_PARAMETRIC_COMM_MSG_EXE");
			if (EnvVariable_INSTALL_PATH == NULL || EnvVariableCOMM_MSG_EXE == NULL )
			{
				std::string TempError = "Environment variables CREO_PARAMETRIC_INSTALL_PATH and/or CREO_PARAMETRIC_COMM_MSG_EXE are not set properly.  Please see \"...\\0Readme - CreateAssembly.txt\" for instructions on how to properly configure your system." +
					std::string("This file is typically located at \"C:\\Program Files (x86)\\META\\bin\\CAD\\Creo\\bin\".");
				throw isis::application_exception(TempError.c_str());
			}
			_putenv_s( "PRO_COMM_MSG_EXE",  EnvVariableCOMM_MSG_EXE );

			std::string CreoParametricInstallPath = EnvVariable_INSTALL_PATH;
			if (CreoParametricInstallPath.back() != '\\') {
				CreoParametricInstallPath += "\\";
			}

			//out_CreoStartCommand = EnvVariable_INSTALL_PATH + creoStartCommandSuffix;
			out_CreoStartCommand = 	BuildCreoStartCommand(	in_GraphicsModeOn,
															in_CreoExceptInputFromThisProgramAndCreoUI, 
															CreoParametricInstallPath);

		}


		//std::cout << std::endl << "SetupCreoEnvironmentVariables: out_CreoStartCommand:    " << out_CreoStartCommand);
		//std::cout << std::endl << "SetupCreoEnvironmentVariables: PRO_COMM_MSG_EXE:     "    << getenv ("PRO_COMM_MSG_EXE");
		//std::cout << std::endl << "SetupCreoEnvironmentVariables: out_CreoStartCommand: "    << out_CreoStartCommand;

	}  // End SetupCreoEnvironmentVariables()


} // end namespace isis