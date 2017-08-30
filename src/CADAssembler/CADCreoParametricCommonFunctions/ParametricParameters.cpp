#include "ParametricParameters.h"
#include <cc_CommonUtilities.h>
#include <CADCommonConstants.h>
#include <iostream>
#include <string>
#include <sstream>
#include "isis_ptc_toolkit_ostream.h"
#include "LoggerBoost.h"
#include "CommonDefinitions.h"


namespace isis
{

const int MAX_STRING_PARAMETER_LENGTH = 79;

////////////////////////////////////////////////////////////////////////

void SetParametricParameter(  
				const std::string	&in_model_name,
				ProMdl				*in_p_model, 
				const std::string   in_ParameterName,
				//const std::string	in_ParameterType,
				e_CADParameterType	in_ParameterType,
				const std::string   in_ParameterValue)
									throw (isis::application_exception)
{

	
	
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "   CADParameter";
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Model Name     "  <<  in_model_name;	
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Type           "  <<  in_ParameterType;
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Name           "	<<  in_ParameterName;
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Value          "	<<  in_ParameterValue;

	

	//typedef wchar_t	ProName[PRO_NAME_SIZE];
	if ( in_ParameterName.size() >= PRO_NAME_SIZE )
	{
		char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
		std::string err_str = "exception : Exceeded maximum number of characters. Parameter Name: "  + std::string(in_ParameterName) + ", Maximum allowed characters: " + itoa(PRO_NAME_SIZE - 1, temp_char_array, 10);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str;			
		throw isis::application_exception("C01001", err_str.c_str());
	}

	try
	{
		ProName ParameterName;
		ProStringToWstring(ParameterName, (char *)in_ParameterName.c_str() );

		//std::cout << std::endl << "ParameterName: " << ProWstringToString(temp_string, ParameterName);	
		//std::cout << std::endl << "in_p_model:    " << in_p_model;
		//std::cout << std::endl << "*in_p_model:    " << *in_p_model;

		ProModelitem  ParameterModelItem_struct;
	
		isis::isis_ProMdlToModelitem ( *in_p_model, &ParameterModelItem_struct );
	
		ProParameter  ProParameter_struct;

		isis::isis_ProParameterInit ( &ParameterModelItem_struct, ParameterName, &ProParameter_struct);
		ProParamvalue  ProParamvalue_struct;

		//switch ( CADParameterType_enum(in_ParameterType) )
		switch ( in_ParameterType )
		{
			case CAD_FLOAT:
				ProParamvalue_struct.type = PRO_PARAM_DOUBLE;
				ProParamvalue_struct.value.d_val = atof(in_ParameterValue.c_str());

				break;

			case CAD_INTEGER:
				ProParamvalue_struct.type = PRO_PARAM_INTEGER;
				ProParamvalue_struct.value.i_val = atoi(in_ParameterValue.c_str());

				break;
			
			case CAD_BOOLEAN:
				ProParamvalue_struct.type = PRO_PARAM_BOOLEAN;
				ProParamvalue_struct.value.l_val = isis::ProBoolean_enum(in_ParameterValue);
				break;

			case CAD_STRING:

				if ( in_ParameterValue.size() > MAX_STRING_PARAMETER_LENGTH )
				{
					std::stringstream errorString;
					errorString <<
					"Erroneous CADParameter Value, Parameter: " <<  CADParameterType_string(in_ParameterType) <<
					"  Value: " << in_ParameterValue << ", Value must be " << MAX_STRING_PARAMETER_LENGTH << 
					" characters or less.";
					throw isis::application_exception(errorString.str());
				}
				ProParamvalue_struct.type = PRO_PARAM_STRING;
				ProStringToWstring(ProParamvalue_struct.value.s_val,(char*)in_ParameterValue.c_str());
				break;

			default:
				//std::string err_str = "Erroneous CADParameter Type: " +  in_ParameterType + ", Should be FLOAT, INTEGER, or BOOLEAN.";
				std::string err_str = "exception : Erroneous CADParameter Type: " +  CADParameterType_string(in_ParameterType) + ", Should be FLOAT, INTEGER, or BOOLEAN.";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str;			
				throw isis::application_exception(err_str.c_str());
		}

		isis::isis_ProParameterValueSet( &ProParameter_struct, &ProParamvalue_struct );

		//std::cout << std::endl << "   Modified parameter: " <<  in_model_name << "::" <<  in_ParameterName << " --> " << in_ParameterValue;
		//std::clog << std::endl << "   Modified parameter: " <<  in_model_name << "::" <<  in_ParameterName << " --> " << in_ParameterValue;

		// Note - we are not using the units (i.e. k->CADValue().Units().present()).  If we were using units,
		//        then ProUnitConversionGet() would probably be needed to compute the converstion factor.  Also,
		//		  if the parameter units were set then they would be used for computing the scaling factor;
		//		  otherwise, the module units would be used.

	}
	catch ( isis::application_exception& ex )
	{
		ex.setComponentInfo(in_model_name);
		ex.setParameterInfo(in_ParameterName);
		std::string err_str =	"exception : Part/Assembly Name: " + in_model_name +
								", Parameter Name: " + in_ParameterName + ", " + ex.what();
		throw isis::application_exception("C01002", err_str.c_str()); 

	}

}  // end SetParametricParameter

////////////////////////////////////////////////////////////////////////

std::string GetParametricParameter(  
				const std::string	&in_model_name,
				ProMdl				*in_p_model,
				const std::string   &in_ParameterOwner,
				const std::string   &in_ParameterName )
									throw (isis::application_exception)
{

	
	

	//typedef wchar_t	ProName[PRO_NAME_SIZE];
	if ( in_ParameterOwner.size() >= PRO_NAME_SIZE )
	{
		char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
		std::string err_str = "exception : Exceeded maximum number of characters. Owner Name: "  + std::string(in_ParameterOwner) + ", Maximum allowed characters: " + _itoa(PRO_NAME_SIZE - 1, temp_char_array, 10);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str;			
		throw isis::application_exception("C01003", err_str);
	}

	//typedef wchar_t	ProName[PRO_NAME_SIZE];
	if ( in_ParameterName.size() >= PRO_NAME_SIZE )
	{
		char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
		std::string err_str = "exception : Exceeded maximum number of characters. Parameter Name: "  + std::string(in_ParameterName) + ", Maximum allowed characters: " + _itoa(PRO_NAME_SIZE - 1, temp_char_array, 10);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str;			
		throw isis::application_exception("C01004", err_str);
	}


	char  ReadValue[ISIS_CHAR_BUFFER_LENGTH];
	
	try
	{
		std::string ParameterOwner_string_upper = ConvertToUpperCase(in_ParameterOwner);

		ProName ParameterOwner_wchar;
		ProStringToWstring(ParameterOwner_wchar, (char *)in_ParameterOwner.c_str() );

		ProName ParameterName_wchar;
		ProStringToWstring(ParameterName_wchar, (char *)in_ParameterName.c_str() );

		//std::cout << std::endl << "ParameterName: " << ProWstringToString(temp_string, ParameterName_wchar);	
		//std::cout << std::endl << "in_p_model:    " << in_p_model;
		//std::cout << std::endl << "*in_p_model:    " << *in_p_model;

		ProModelitem  ParameterModelItem_struct;

		if ( ParameterOwner_string_upper == "MODEL" )
			isis::isis_ProMdlToModelitem ( *in_p_model, &ParameterModelItem_struct );
		else
			isis::isis_ProModelitemByNameInit (*in_p_model, PRO_FEATURE, ParameterOwner_wchar, &ParameterModelItem_struct);
	
		ProParameter  ProParameter_struct;

		isis::isis_ProParameterInit ( &ParameterModelItem_struct, ParameterName_wchar, &ProParameter_struct);
		
		ProParamvalue  ProParamvalue_struct;

		isis::isis_ProParameterValueGet( &ProParameter_struct, &ProParamvalue_struct );
		

		switch ( ProParamvalue_struct.type ) 
		{
			case PRO_PARAM_DOUBLE:
				sprintf(ReadValue,"%lf", ProParamvalue_struct.value.d_val);
				break;

			case PRO_PARAM_STRING:
				char narrow_string[ISIS_CHAR_BUFFER_LENGTH];
				ProWstringToString(narrow_string,ProParamvalue_struct.value.s_val); 
				sprintf(ReadValue,"%s", narrow_string );
				break;

			case PRO_PARAM_INTEGER:
				sprintf(ReadValue,"%d", ProParamvalue_struct.value.i_val);
				break;
			
			case PRO_PARAM_BOOLEAN:
				if ( ProParamvalue_struct.value.l_val == PRO_B_TRUE )
					sprintf(ReadValue,"%s", "TRUE");
				else
					sprintf(ReadValue,"%s", "FALSE");
				break;

			default:
				std::string err_str = "exception : Erroneous CADReadParameter Type, Supported types are DOUBLE, INTEGER, STRING, and BOOLEAN.";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str;			
				throw isis::application_exception(err_str);
		}


	}
	catch ( isis::application_exception& ex )
	{
		std::string err_str =	"exception : Part/Assembly Name: " + in_model_name +
								", Parameter Name: " + in_ParameterName + ", " + ex.what();
		throw isis::application_exception("C01005", err_str); 

	}

	return std::string(ReadValue);

}  // end GetParametricParameter

////////////////////////////////////////////////////////////////////////

ProError SetParametricParameter(  
				e_force             force,
				const std::string	&in_model_name,
				ProMdl				*in_p_model, 
				const std::string   in_ParameterName,
				e_CADParameterType	in_ParameterType,
				const std::string   in_ParameterValue)
									throw (isis::application_exception)
{
	ProError result = PRO_TK_NO_ERROR; 
	std::stringstream msg("SetParametricParameter: (force) ");

	
	
	
	isis_LOG(lg, isis_FILE, isis_INFO) <<  "  SetParametricParameter CADParameter" 
		<< isis_EOL <<  "      Force          "  <<  force
		<< isis_EOL <<  "      Model Name     "  <<  in_model_name 
		<< isis_EOL <<  "      Model Handle   "  <<  (const void*)in_p_model
		<< isis_EOL <<  "      Type           "  <<  in_ParameterType 
		<< isis_EOL <<  "      Name           "  <<  in_ParameterName
		<< isis_EOL <<  "      Value          "  <<  in_ParameterValue;

	if (in_p_model == NULL) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "null model handle";
		return PRO_TK_BAD_INPUTS;
	}

	isis_LOG(lg, isis_FILE, isis_INFO) << "SetParametricParameter : Set up the parameter"; 
	if ( in_ParameterName.size() >= PRO_NAME_SIZE ) {
		msg << "Exceeded maximum number of characters : "
		    << "Parameter Name: " << in_ParameterName << ", Maximum allowed characters: " << (PRO_NAME_SIZE - 1);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
		throw isis::application_exception("C01001", msg);
	}

	ProName parameter_key;
	ProStringToWstring(parameter_key, const_cast<char *>(in_ParameterName.c_str()) );
	ProParamvalue parameter_value;

	switch ( in_ParameterType ) {
		case CAD_FLOAT:
			parameter_value.type = PRO_PARAM_DOUBLE;
			parameter_value.value.d_val = atof(in_ParameterValue.c_str());
			break;
		case CAD_INTEGER:
			parameter_value.type = PRO_PARAM_INTEGER;
			parameter_value.value.i_val = atoi(in_ParameterValue.c_str());
			break;
		case CAD_BOOLEAN:
			parameter_value.type = PRO_PARAM_BOOLEAN;
			parameter_value.value.l_val = isis::ProBoolean_enum(in_ParameterValue);
			break;
		case CAD_STRING:
			if ( in_ParameterValue.size() > MAX_STRING_PARAMETER_LENGTH )
			{
				msg << "Erroneous CADParameter string value: "
					    << "Parameter: " <<  CADParameterType_string(in_ParameterType) 
					    << "  Value: " << in_ParameterValue 
						<< ", Value must be " << MAX_STRING_PARAMETER_LENGTH << " characters or less.";
				throw isis::application_exception(msg);
			}
			parameter_value.type = PRO_PARAM_STRING;
			ProStringToWstring(parameter_value.value.s_val, const_cast<char *>(in_ParameterValue.c_str()));
			break;

		default:
			msg << "Erroneous CADParameter Type: "
				<<  CADParameterType_string(in_ParameterType) << ", Should be FLOAT, INTEGER, or BOOLEAN.";
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
			throw isis::application_exception(msg);
	}

	ProError status;

	isis_LOG(lg, isis_FILE, isis_INFO) << "SetParametricParameter : get the model-item from the model";
	ProModelitem model_item;
	
	switch ( status = ProMdlToModelitem( *in_p_model, &model_item ) ) {
	case PRO_TK_NO_ERROR: 
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "The ProMdlToModelitem() was successful.";
			break;
		}
	case PRO_TK_BAD_INPUTS: 
		{
			msg	<< "ProMdlToModelitem: An argument is NULL, "
				<< " model: " << in_p_model 
				<< " model-item: " << model_item;
			isis::application_exception ex(msg);
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
			throw ex;
		}
	case PRO_TK_INVALID_PTR: 
		{
			msg << "ProMdlToModelitem: The handle is invalid. ";
			isis::application_exception ex(msg);
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
			throw ex;
		}
	}

	isis_LOG(lg, isis_FILE, isis_INFO) << "SetParametricParameter : set the property";
	ProParameter parameter_handle;
    switch ( status = ProParameterInit(&model_item, parameter_key, &parameter_handle) ) {
		case PRO_TK_E_NOT_FOUND:
		{
			switch (force) {
			case FORCE_KEY:
				break;
			case FORCE_VALUE:
				{
					msg << "key is not bound and should not be forced";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			case FORCE_NEITHER:
				{
					msg << "force neither : not yet implemented";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "The parameter was not found within the owner, so creating new.";
			switch ( status = ProParameterCreate (&model_item, parameter_key, &parameter_value, &parameter_handle ) ) {
			case PRO_TK_NO_ERROR:
				{
						isis_LOG(lg, isis_FILE, isis_INFO) << "ProParameterCreate successfully added the parameter.";
						result = PRO_TK_E_NOT_FOUND;
						break;
				}
			case PRO_TK_BAD_INPUTS:
				{
					msg	<< "One or more of the input arguments to ProParameterCreate() are invalid,"
						<< " key: " << parameter_key 
						<< " param: " << parameter_handle.owner.type;
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			case PRO_TK_BAD_CONTEXT:
				{
					msg << "The owner is nonexistent.";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			case PRO_TK_E_FOUND:
				{
					msg << "The specified parameter already exists.";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			case PRO_TK_GENERAL_ERROR: 
				{
					msg << "The function could not add the parameter to the database.";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			}
		}
	case PRO_TK_NO_ERROR: 
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "ProParameterInit() successfully initialized the handle.";
			switch (force) {
			case FORCE_KEY:
			case FORCE_VALUE:
				break;
			case FORCE_NEITHER:
				{
					msg << "force neither : not yet implemented";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			}	
			switch ( status = ProParameterValueSet(&parameter_handle, &parameter_value) ) {
			case PRO_TK_NO_ERROR:
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "ProParameterValueSet successfully updated the information.";
					result = PRO_TK_NO_ERROR;
					break;
				}
			case PRO_TK_E_NOT_FOUND: 
				{
					msg << "The owner was not found.";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			case PRO_TK_GENERAL_ERROR: 
				{
					msg << "The parameter was not found, or the function could not perform the action.";
					isis::application_exception ex(msg);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
					throw ex;
				}
			}
			break;
		}
	case PRO_TK_BAD_INPUTS: 
		{
			msg	<< "One or more of the input arguments are invalid,"
				<< " key: " << parameter_key 
				<< " param: " << parameter_handle.owner.type;
			isis::application_exception ex(msg);
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
			throw ex;
		}
	case PRO_TK_BAD_CONTEXT: 
		{
			msg << "The owner is nonexistent.";
			isis::application_exception ex(msg);
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << msg.str();
			throw ex;
		}
		
	}
	isis_LOG(lg, isis_FILE, isis_INFO) << "SetParametricParameter : COMPLETE";
	return result;
		
}  // end ForceParametricParameter

} // end namespace isis