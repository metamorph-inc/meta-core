#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonConstants.h>
#include <CreoStringToEnumConversions.h>
#include <cc_MultiFormatString.h>
#include <cc_CommonUtilities.h>
#include <cc_CommonStructures.h>
#include <sstream>
#include "cc_LoggerBoost.h"

namespace isis
{

	void isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
										   // Added Arguments
										  const std::string &in_ComponentID,
										  const std::string &in_Model_Name,
										  const isis::MultiFormatString &in_Representation,
										  // Original arguments
										  const ProFamilyName name, 
										  ProMdlType    type,
										  ProMdl       *p_handle)
										  throw (isis::application_exception)
	{

		
		
		try
		{

			if ( in_Representation.size() > 0 )
			{

				// Retreive the whole assembly.  This is a workaround because bounding box does not 
				// compute properly if a simplified rep leaves out .prts/.asms from memory.
				// Pull everything into memory, and then load the simplified rep models.
				// If this is done then the STEP file would contain all the geometry isis::isis_ProMdlRetrieve(name,type, p_handle);	

				// try to retrieve the simplified rep
				if ( type == PRO_MDL_ASSEMBLY )
				{
					ProError err;
						
					if ( isis::ConvertToUpperCase(in_Representation) ==  "DEFAULT ENVELOPE REP")
					{

						err = ProDefaultEnvelopeSimprepRetrieve( (wchar_t*)name, (ProAssembly *)p_handle );
					
						//std::cout << std::endl << std::endl <<	">>>> Assembly: " << std::endl  << 
						//											"   Model  Name:                       " << in_Model_Name << std::endl  << 
						//											"   Representation:                    " << in_Representation << std::endl  << 
						//											"   ProAssemblySimprepRetrieve, Error: " << err;

						if (  err == PRO_TK_BAD_INPUTS )
						{
							std::stringstream errorString;
							errorString 
							<<	"Function - isis_ProMdlRetrieve_WithDescriptiveErrorMsg, ProAssemblySimprepRetrieve received erroneous inputs: " << std::endl <<
								"   ComponentInstanceID: " <<  in_ComponentID << std::endl <<
								"   Model Name:          " <<  in_Model_Name  << std::endl <<
								"   Model Handle:         " << p_handle << std::endl <<
								"   Representation:      " <<  in_Representation<< std::endl;  
							throw isis::application_exception(errorString);

						}

					}
					else
					{

						err = ProAssemblySimprepRetrieve( (wchar_t*)name, (wchar_t*)(const wchar_t*) in_Representation, NULL, (ProAssembly *)p_handle );
					
						isis::MultiFormatString temp_string(name);
						//std::cout << std::endl << std::endl <<	">>>>>>>>>>>> Assembly: " << std::endl  << 
						//											"   Model  Name:                       " << in_Model_Name << std::endl  << 
						//											"   Representation:                    " << in_Representation << std::endl  << 
						//											"   Representation Name:               " << (char *)(const char *)temp_string << std::endl  << 
						//											"   ProAssemblySimprepRetrieve, Error: " << err << std::endl <<
						//											"<<<<<<<<<<<<<<<<<<";


						if (  err == PRO_TK_BAD_INPUTS )
						{
							std::stringstream errorString;
							errorString 
							<<	"Function - isis_ProMdlRetrieve_WithDescriptiveErrorMsg, ProAssemblySimprepRetrieve received erroneous inputs: " << std::endl <<
								"   ComponentInstanceID: " <<  in_ComponentID << std::endl <<
								"   Model Name:          " <<  in_Model_Name  << std::endl <<
								"   Model Handle:         " << p_handle << std::endl <<
								"   Representation:      " <<  in_Representation<< std::endl;  
							throw isis::application_exception(errorString);

						}

						
						//ProError proError_temp;
						//ProSimprep proSimprep_temp;


						//ProSimprepActiveGet ((ProSolid)*p_handle, 
                        //            &proSimprep_temp);

						//std::cout << std::endl << "##########################";
						//std::cout << std::endl << "proSimprep_temp.id: "  << proSimprep_temp.id;
						//std::cout << std::endl << "proSimprep_temp.owner: "  << proSimprep_temp.owner;
						//std::cout << std::endl << "roSimprep_temp.type: "  << proSimprep_temp.type;
						//std::cout << std::endl << "##########################";

						
						//proError_temp  = ProSimprepInit ((wchar_t*)(const wchar_t*) in_Representation,
						//								  -1,
						//								  (ProSolid)*p_handle,
						//								  &proSimprep_temp );
						//
						//std::cout << std::endl << std::endl << "ProSimprepInit, Error Code: " << proError_temp;
						//
						//ProSimprepActivate ((ProSolid)*p_handle, &proSimprep_temp);
						//std::cout << std::endl << std::endl << "ProSimprepActivate, Error Code: " << proError_temp << std::endl;
						//

					}



					if ( err == PRO_TK_NO_ERROR )   
					{
						// assembly was retrieved.
						isis_LOG(lg, isis_FILE, isis_INFO) << "Representation: " << in_Representation << ", was found for ComponentInstanceID: " << in_ComponentID << ", Model Name: " << in_Model_Name << ".";
						return; // model was retrieved. 
					}
					else
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Representation: " << in_Representation << ", was NOT found for ComponentInstanceID: " << in_ComponentID << ", Model Name: " << in_Model_Name << ".  Using active representation.";
					}

				}
				else
				{  // PRO_MDL_PART

					ProError err; 
					
					if ( isis::ConvertToUpperCase(in_Representation) ==  "MASTER REP")
					{
						// This code seems to never set the "Master Rep".  Will leave it in at least
						// until the problem is understood.
						err = ProPartSimprepRetrieve ((wchar_t*)name, 
												PRO_SIMPREP_MASTER_REP,
												(wchar_t*)(const wchar_t*) in_Representation,
												(ProPart*)p_handle);


						//std::cout << std::endl << std::endl <<	">>>> Part: " << std::endl  << 
						//										"   Model  Name:                   " << in_Model_Name << std::endl  << 
						//										"   Representation:                " << in_Representation << std::endl  << 
						//										"   ProPartSimprepRetrieve, Error: " << err;

						if (  err == PRO_TK_BAD_INPUTS )
						{
							std::stringstream errorString;
							errorString 
							<<	"Function - isis_ProMdlRetrieve_WithDescriptiveErrorMsg, ProPartSimprepRetrieve received erroneous inputs: " << std::endl <<
								"   ComponentInstanceID: " <<  in_ComponentID << std::endl <<
								"   Model Name:          " <<  in_Model_Name  << std::endl <<
								"   Model Handle:         " << p_handle << std::endl <<
								"   Representation:      " <<  in_Representation<< std::endl;  
							throw isis::application_exception(errorString);

						}
					}
					else
					{
						err = ProPartSimprepRetrieve ((wchar_t*)name, 
												PRO_SIMPREP_USER_DEFINED,
												(wchar_t*)(const wchar_t*) in_Representation,
												(ProPart*)p_handle);

						//std::cout << std::endl << std::endl <<	">>>> Part: " << std::endl  << 
						//										"   Model  Name:                   " << in_Model_Name << std::endl  << 
						//										"   Representation:                " << in_Representation << std::endl  << 
						//										"   ProPartSimprepRetrieve, Error: " << err;

						if (  err == PRO_TK_BAD_INPUTS )
						{
							std::stringstream errorString;
							errorString 
							<<	"Function - isis_ProMdlRetrieve_WithDescriptiveErrorMsg, ProPartSimprepRetrieve received erroneous inputs: " << std::endl <<
								"   ComponentInstanceID: " <<  in_ComponentID << std::endl <<
								"   Model Name:          " <<  in_Model_Name  << std::endl <<
								"   Model Handle:         " << p_handle << std::endl <<
								"   Representation:      " <<  in_Representation<< std::endl;  
							throw isis::application_exception(errorString);

						}

					}
					

					if ( err == PRO_TK_NO_ERROR )
					{
						// Part retrieved
						isis_LOG(lg, isis_FILE, isis_INFO) << "Representation: " << in_Representation << ", was found for ComponentInstanceID: " << in_ComponentID << ", Model Name: " << in_Model_Name << ".";
						return; // model was retrieved. 
					}
					else
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Representation: " << in_Representation << ", was NOT found for ComponentInstanceID: " << in_ComponentID << ", Model Name: " << in_Model_Name << ".  Using active representation.";
					}
				} // END Else if ( type == PRO_MDL_ASSEMBLY )

				

			}
			else
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "A Representation was not specified for ComponentInstanceID: " << in_ComponentID << ", Model Name: " << in_Model_Name << ".";
			} // END if ( in_Representation.size() > 0 )

			// If made it to here, then the simplified rep was not found.

			// Commented lines are for testing via setting the case
			//isis::MultiFormatString  name_multi(name);
			//std::string temp_Upper = ConvertToLowerCase(name_multi);	
			//isis::MultiFormatString  name_Upper_multi(temp_Upper);
			//std::cout << std::endl << "name size: " << name_multi.size();
			//isis::isis_ProMdlRetrieve(name_Upper_multi,type, p_handle);	

			isis::isis_ProMdlRetrieve(name,type, p_handle);	
		}
		catch ( isis::application_exception& ex )
		{
			try 
			{
				ProPath path; 
				isis_ProDirectoryCurrentGet( path );
				isis::MultiFormatString  path_MultiFormatString(path);
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "ProDirectoryCurrent: " << path_MultiFormatString;
			}
			catch ( isis::application_exception ex)
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Failed to retrieve ProDirectoryCurrent. Exception: " << ex.what(); 
			}

			//char modelName[ISIS_CHAR_BUFFER_LENGTH];
			//ProWstringToString(modelName, (char *)in_Model_Name.c_str());	

			std::stringstream errorString;
				errorString <<
						"exception : Failed to open a Creo Model:"  << std::endl <<
						"   Model Name:            " << in_Model_Name << std::endl <<
						"   Model Type:            " << isis::ProMdlType_string(type) <<  std::endl <<
						"   Component Instance ID: " << in_ComponentID << std::endl <<
						"   Exception Message:     " << ex.what();
			throw isis::application_exception("C05001",errorString.str().c_str());
		}

	}

	void isis_ProModelitemByNameInit_WithDescriptiveErrorMsg( 
										 // Added Arguments
										 const std::string &in_ComponentID,
										 const std::string &in_Model_Name,
										 ProMdlType     mdltype,
										 // Original arguments
										 ProMdl         mdl, 
                                         ProType        type, 
                                         const ProName        name, 
                                         ProModelitem*  p_item ) throw(isis::application_exception)
	{

		try
		{
			isis::isis_ProModelitemByNameInit (mdl, type, name, p_item);
		}
		catch ( isis::application_exception& ex )
		{
			char featureName[ISIS_CHAR_BUFFER_LENGTH];
			ProWstringToString(featureName, (wchar_t *)name );

			std::stringstream errorString;
				errorString <<
						"Could not find feature in Creo model:"  << std::endl <<
						"   Feature Name:      " << featureName <<  std::endl <<
						"   Feature Type:      " << type << ", Point-66, Axis-21, Coordinate System-25, Plane/Surface-5" << std::endl <<
						"   Model Name:        " << in_Model_Name << std::endl <<
						"   Model Type:        " << isis::ProMdlType_string(mdltype)<<  std::endl <<
						"   Component Instance ID:      " << in_ComponentID << std::endl <<
						"   Exception Message: " << ex.what();
			throw isis::application_exception("C05002",errorString.str().c_str());
		}

	}


	void isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( 
						const std::string										&in_ComponentID,
						const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
						ProMassProperty*											mass_prop )
																throw(isis::application_exception) 									
	{

		std::map<std::string, isis::CADComponentData>::const_iterator itr;
		itr = in_CADComponentData_map.find(in_ComponentID);
		
		if ( itr == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_ComponentID that is not in in_CADComponentData_map. in_ComponentID:  " << in_ComponentID;
			throw isis::application_exception(errorString);	
		}


		try
		{
			//isis::isis_ProSolidMassPropertyGet( static_cast<ProSolid>(in_CADComponentData_map[in_ComponentID].cADModel_hdl), NULL, mass_prop );
			isis::isis_ProSolidMassPropertyGet( static_cast<ProSolid>(itr->second.cADModel_hdl), NULL, mass_prop );
		}
		catch ( isis::application_exception& ex )
		{
			std::stringstream errorString;
				errorString <<
						"Failed to retrieve mass properties:"  << std::endl <<
						"   Model Name:            " <<	 itr->second.name << std::endl <<
						"   Model Type:            " << isis::ProMdlType_string(itr->second.modelType)<<  std::endl <<
						"   Component Instance ID: " <<  in_ComponentID <<  std::endl <<
						"   Exception Message:     " << ex.what();
				throw isis::application_exception("C05002",errorString.str().c_str());
		}
	}

} // END namespace isis