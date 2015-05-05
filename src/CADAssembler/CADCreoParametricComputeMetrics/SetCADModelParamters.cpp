
#include <SetCADModelParamters.h>

#include <isis_ptc_toolkit_functions.h>
#include <isis_include_ptc_headers.h>
#include <StringToEnumConversions.h>
#include <ParametricParameters.h>
#include <iostream>
#include <fstream>
#include <list>
#include <locale>


namespace isis
{

struct  CADIncrementParameter_struct
{
	std::string Name;
	std::string Type;
	double StartValue;
	double EndValue;
	double Increment;
	CADIncrementParameter_struct  *Next_ptr;
};


struct  CADReadParameter_struct
{
	std::string Owner;
	std::string Name;
};

////////////////////////////////////////////////////////////////////////////////////

void stream_CADIncrementParameter_structs( CADIncrementParameter_struct * const in_CADIncrementParameter_struct_ptr, 
								  const char *in_indent, 
								  std::ostream &out_Stream)
{
	CADIncrementParameter_struct *CADIncrementParameter_struct_ptr = in_CADIncrementParameter_struct_ptr;

	out_Stream << std::endl << in_indent << "BEGIN CADIncrementParameter structures linked list";
	while ( CADIncrementParameter_struct_ptr != NULL )
	{
		out_Stream << std::endl << in_indent << "   Name       "        << CADIncrementParameter_struct_ptr->Name; 
		out_Stream << std::endl << in_indent << "   StartValue "  << CADIncrementParameter_struct_ptr->StartValue; 
		out_Stream << std::endl << in_indent << "   EndValue   "    << CADIncrementParameter_struct_ptr->EndValue; 
		out_Stream << std::endl << in_indent << "   Increment  "   << CADIncrementParameter_struct_ptr->Increment; 
		CADIncrementParameter_struct_ptr = CADIncrementParameter_struct_ptr->Next_ptr;
		if ( CADIncrementParameter_struct_ptr != NULL ) out_Stream << std::endl;
	}
	out_Stream << std::endl << in_indent << "END CADIncrementParameter structures linked list";

}

void delete_CADIncrementParameter_structs( CADIncrementParameter_struct *in_CADIncrementParameter_struct_ptr )
{
	CADIncrementParameter_struct *CADIncrementParameter_struct_ptr = in_CADIncrementParameter_struct_ptr;

	CADIncrementParameter_struct *CADIncrementParameter_temp_ptr; 

	while ( CADIncrementParameter_struct_ptr != NULL )
	{
		CADIncrementParameter_temp_ptr = CADIncrementParameter_struct_ptr;
		CADIncrementParameter_struct_ptr = CADIncrementParameter_struct_ptr->Next_ptr;
		delete CADIncrementParameter_temp_ptr;
	}

}

////////////////////////////////////////////////////////////////////////////////////
//  zzzz should make this a common function.
void RetrieveProEModel( const std::string &in_ModelName_Withoutsuffix, 
					    const std::string &in_ModelType_PartOrAssembly, 
						ProMdl *out_p_model  )
								throw (isis::application_exception)
{

	wchar_t  PartName[ISIS_CHAR_BUFFER_LENGTH];

	std::string ModelName_temp_string;
	
	ProMdlType pro_model_type = isis::ProMdlType_enum( in_ModelType_PartOrAssembly);

	ProStringToWstring(PartName, (char *) in_ModelName_Withoutsuffix.c_str() );

	isis::isis_ProMdlRetrieve(PartName,pro_model_type, out_p_model);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void WriteMassPropertiesToCSVFile( ProMdl		  *in_p_model, 
								   bool			  in_FirstEntry,
								   std::ofstream  &in_MetricsOutputFile)
										throw (isis::application_exception)

{
	ProMassProperty  mass_prop;
	isis::isis_ProSolidMassPropertyGet( (ProSolid)*in_p_model, NULL, &mass_prop ); 
	
	char  computed_value[ISIS_CHAR_BUFFER_LENGTH];
	sprintf(computed_value,"%lf", mass_prop.volume);
	//std::cout << std::endl << "PRECISION: volume with cout: " << mass_prop.volume;
	//std::cout << std::endl <<  "PRECISION: volume with sprintf(param_string,%lf, mass_prop.volume): " << computed_value;
	if ( in_FirstEntry)
		in_MetricsOutputFile << std::endl;
	else
		in_MetricsOutputFile << ",";

	in_MetricsOutputFile << computed_value;
	
	sprintf(computed_value,"%lf", mass_prop.surface_area);
	//in_MetricsOutputFile << "," << mass_prop.surface_area;
	in_MetricsOutputFile << "," << computed_value;

	sprintf(computed_value,"%lf", mass_prop.density);
	//in_MetricsOutputFile << "," << mass_prop.density;
	in_MetricsOutputFile << "," << computed_value;

	sprintf(computed_value,"%lf", mass_prop.mass);
	//in_MetricsOutputFile << "," << mass_prop.mass;
	in_MetricsOutputFile << "," << computed_value;

	sprintf(computed_value,"%lf", mass_prop.center_of_gravity[0]);
	//in_MetricsOutputFile << "," << mass_prop.center_of_gravity[0];
	in_MetricsOutputFile << "," << computed_value;

	sprintf(computed_value,"%lf", mass_prop.center_of_gravity[1]);
	//in_MetricsOutputFile << "," << mass_prop.center_of_gravity[1];
	in_MetricsOutputFile << "," << computed_value;

	sprintf(computed_value,"%lf", mass_prop.center_of_gravity[2]);
	//in_MetricsOutputFile << "," << mass_prop.center_of_gravity[2];
	in_MetricsOutputFile << "," << computed_value;

}

////////////////////////////////////////////////////////////////////////////////////////

void WriteReadParametersToCSVFile( const std::string					&in_model_name,
								   ProMdl								*in_p_model, 
								   bool									in_FirstEntry,
								   std::list<CADReadParameter_struct>	&in_ReadParameter_list, 
								   std::ofstream						&in_MetricsOutputFile)  
																		       throw (isis::application_exception)
{


	bool FirstEntry = in_FirstEntry;

	for ( std::list<CADReadParameter_struct>::const_iterator ReadParm_itr = in_ReadParameter_list.begin(); 
		  ReadParm_itr != in_ReadParameter_list.end();
		  ++ReadParm_itr )
	{
		if ( FirstEntry )
		{
			in_MetricsOutputFile << std::endl;
			FirstEntry = false;
		}
		else
			in_MetricsOutputFile << ",";

		in_MetricsOutputFile << isis::GetParametricParameter( in_model_name, in_p_model, ReadParm_itr->Owner, ReadParm_itr->Name  );
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void set_CADIncrementParameters_read_ReadParameters(	const std::string	&in_model_name,
						ProMdl				*in_p_model,  
					    CADIncrementParameter_struct *in_CADIncrementParameter_struct_ptr, 
					    std::list<double> &in_IncrementParameter_list,
						std::list<CADReadParameter_struct> &in_ReadParameter_list,
						std::ofstream &in_MetricsOutputFile ) 	throw (isis::application_exception)
{
	if ( in_CADIncrementParameter_struct_ptr != NULL )
	{
		for ( double param =  in_CADIncrementParameter_struct_ptr->StartValue; 
					 param <=  in_CADIncrementParameter_struct_ptr->EndValue;
					 param+=  in_CADIncrementParameter_struct_ptr->Increment )
		{

			char  param_string[ISIS_CHAR_BUFFER_LENGTH];
			sprintf(param_string,"%lf", param);

			//printf("\n\n****************** ==> param_string: %s", param_string ); 
			//std::cout << "\nparam: " <<  param;
			//std::cout << "\n****************** ==> Start, end, increment: " <<   
			//	in_CADIncrementParameter_struct_ptr->StartValue << "  " <<  in_CADIncrementParameter_struct_ptr->EndValue <<
			//	"  " << in_CADIncrementParameter_struct_ptr->Increment;	

			isis::SetParametricParameter(	in_model_name, 
											 in_p_model, 
											 in_CADIncrementParameter_struct_ptr->Name,
											 CADParameterType_enum(in_CADIncrementParameter_struct_ptr->Type), 
											 param_string );

			in_IncrementParameter_list.push_back( param );
			set_CADIncrementParameters_read_ReadParameters( in_model_name, in_p_model, in_CADIncrementParameter_struct_ptr->Next_ptr, in_IncrementParameter_list, in_ReadParameter_list, in_MetricsOutputFile );

			if ( in_CADIncrementParameter_struct_ptr->Next_ptr == NULL )
			{
				// This is at the last item in the stack, gather metrics, write metrics to output file

				bool first_record = true;
				for ( std::list<double>::const_iterator param_itr = in_IncrementParameter_list.begin();
					  param_itr !=  in_IncrementParameter_list.end();
					  ++param_itr )
				{
					if ( first_record )
					{
						in_MetricsOutputFile << std::endl << *param_itr;
						first_record = false;
					}
					else
						in_MetricsOutputFile << "," << *param_itr;
				}


				//////////////////////////////////
				// Regnerate Solid
				//////////////////////////////////
				isis::isis_ProSolidRegenerate ((ProSolid) *in_p_model, PRO_REGEN_NO_RESOLVE_MODE);

				ProSolidRegenerationStatus RegenStatus;

				isis::isis_ProSolidRegenerationstatusGet( (ProSolid) *in_p_model, &RegenStatus);

				if ( RegenStatus == PRO_SOLID_FAILED_REGENERATION )
				{
					// Treat this as a warning.  Continue building the rest of the assemblies.  The assembly with
					// the regeneration error should open; however, the components that failed to regenerate will show up
					// as red items in the assembly tree.  They will not be positioned properly.

					string err_str = "WARNING - Model failed to regenerate. Model Name: " + 
						in_model_name + " Parameter List: ";
					std::cout << std::endl << err_str; 
					std::clog << std::endl << err_str; 
					for ( std::list<double>::const_iterator param_itr = in_IncrementParameter_list.begin();
					  param_itr !=  in_IncrementParameter_list.end();
					  ++param_itr )
					{
						std::cout << " " << *param_itr;
						std::clog << " " << *param_itr;

					}

					in_MetricsOutputFile << ",Fail";
				}
				else
				{
					std::cout << std::endl;
					std::clog << std::endl;
					for ( std::list<double>::const_iterator param_itr = in_IncrementParameter_list.begin();
					  param_itr !=  in_IncrementParameter_list.end();
					  ++param_itr )
					{
						std::cout << " " << *param_itr;
						std::clog << " " << *param_itr;
					}

					in_MetricsOutputFile << ",Pass";
				
					WriteMassPropertiesToCSVFile( in_p_model, false, in_MetricsOutputFile);							
					WriteReadParametersToCSVFile( in_model_name, in_p_model, false, in_ReadParameter_list, in_MetricsOutputFile);
				}  

			}  // end if

			in_IncrementParameter_list.pop_back();
		} // end for ( double param =  in_CAD...
	} // end if ( in_CADIncrementParameter_struct_ptr != NULL )
}  // end set_CADIncrementParameters_read_ReadParameters

////////////////////////////////////////////////////////////////////////////////////

void WriteCSVHeaderLine(  bool									in_IncludeRegenPassFail,
						  CADIncrementParameter_struct			*in_CADIncrementParameter_struct_ptr, 
						  std::list<CADReadParameter_struct>	&in_ReadParameter_list, 
						  std::ofstream							&in_MetricsOutputFile  )
{


	CADIncrementParameter_struct	*CADIncrementParameter_struct_ptr = in_CADIncrementParameter_struct_ptr;

	bool FirstEntryWritten = false;

	while ( CADIncrementParameter_struct_ptr != NULL )
	{
		if ( FirstEntryWritten)
		{
			in_MetricsOutputFile << "," << CADIncrementParameter_struct_ptr->Name; 
		}
		else
		{
			in_MetricsOutputFile << CADIncrementParameter_struct_ptr->Name; 
			FirstEntryWritten = true;
		}
		CADIncrementParameter_struct_ptr = CADIncrementParameter_struct_ptr->Next_ptr;
	}

	if ( FirstEntryWritten) in_MetricsOutputFile << ",";

	if ( in_IncludeRegenPassFail )
	{ 
		in_MetricsOutputFile << "Regen Pass/Fail";
		FirstEntryWritten = true;
	}
	
	if ( FirstEntryWritten) in_MetricsOutputFile << ",";

	in_MetricsOutputFile << "Volume,Surface Area,Density,Mass,CG x,CG y,CG z";

	// Add the read titles
	for ( std::list<CADReadParameter_struct>::const_iterator ReadParm_itr = in_ReadParameter_list.begin(); 
		  ReadParm_itr != in_ReadParameter_list.end();
		  ++ReadParm_itr )
	{
		in_MetricsOutputFile << "," << ReadParm_itr->Name;
	}

}

////////////////////////////////////////////////////////////////////////////////////

void SetCADModelParamters(std::auto_ptr<CADComponentsType>  &in_CADComponents_ptr)
													throw (isis::application_exception)
{

	for ( CADComponentsType::CADComponent_const_iterator i(in_CADComponents_ptr->CADComponent().begin()); 
		  i != in_CADComponents_ptr->CADComponent().end(); ++i )
	{
		std::clog << std::endl << "Setting parameters in CADComponent:";   
		std::clog << std::endl << "   Name              " << i->Name();
		std::clog << std::endl << "   Type              " << i->Type();
		std::clog << std::endl << "   MetricsOutputFile " << i->MetricsOutputFile();

		std::cout << std::endl << "Setting parameters in CADComponent:";   
		std::cout << std::endl << "   Name              " << i->Name();
		std::cout << std::endl << "   Type              " << i->Type();
		std::cout << std::endl << "   MetricsOutputFile " << i->MetricsOutputFile();

		// Check if <ParametricParameters> are present, if not return 
		if ( !i->ParametricParameters().present())
		{
			std::clog << std::endl << "Part/Assembly: " << i->Name() << ", <ParametricParameters> not present in xml file.";
			std::cout << std::endl << "Part/Assembly: " << i->Name() << ", <ParametricParameters> not present in xml file..";
			return;
		}

		std::ofstream MetricsOutputFile;
		MetricsOutputFile.open (i->MetricsOutputFile().c_str(), std::ios::out | std::ios::trunc );
		if (!MetricsOutputFile.is_open())
		{
			std::string TempError = "Could not open metrics file, file name: " + i->MetricsOutputFile();
			throw isis::application_exception(TempError.c_str());
		}

		////////////////////////////////////////////////////////
		// XML Read parameters ==> CADReadParameter_struct list
		////////////////////////////////////////////////////////
		std::list<CADReadParameter_struct> ReadParameter_list;
		if (i->ParametricParameters().get().Read().present())
		{
			for ( CADComponentsType::CADComponent_type::ParametricParameters_type::Read_type::CADReadParameter_const_iterator j( i->ParametricParameters().get().Read().get().CADReadParameter().begin());
					j != i->ParametricParameters().get().Read().get().CADReadParameter().end();
				  ++j )	
			{
				CADReadParameter_struct CADReadParameter_temp;
				CADReadParameter_temp.Owner = j->Owner();
				CADReadParameter_temp.Name  = j->Name();
				ReadParameter_list.push_back(CADReadParameter_temp);
			}
		}


		ProMdl p_model;

		RetrieveProEModel(i->Name(), i->Type(), &p_model  );									

		if ( i->ParametricParameters().present() )
		{	
			CADIncrementParameter_struct  *CADIncrementParameter_head_ptr;
			CADIncrementParameter_struct  *CADIncrementParameter_previous_ptr;
			bool          CADIncrementParameter_head_defined = false;

			std::list<double> IncrementParameter_list;
	
			
			if (i->ParametricParameters().get().Increment().present())
			{
				for ( CADComponentsType::CADComponent_type::ParametricParameters_type::Increment_type::CADIncrementParameter_const_iterator j( i->ParametricParameters().get().Increment().get().CADIncrementParameter().begin());
						j != i->ParametricParameters().get().Increment().get().CADIncrementParameter().end();
					  ++j )	

				{
					std::clog << std::endl <<   "      CADIncrementParameter";
					std::clog << std::endl <<   "         Name        "  << j->Name();
					//std::clog << std::endl << "         Type        "  << j->Type();
					//std::clog << std::endl << "         StartValue  "  << j->StartValue();
					//std::clog << std::endl << "         EndValue    "  << j->EndValue();
					//std::clog << std::endl << "         Increment   "  << j->Increment();


					CADIncrementParameter_struct *CADIncrementParameter_struct_ptr = new CADIncrementParameter_struct;
					
					CADIncrementParameter_struct_ptr->Name = j->Name();
					CADIncrementParameter_struct_ptr->Type = j->Type();
					CADIncrementParameter_struct_ptr->StartValue = atof(j->StartValue().c_str());
					CADIncrementParameter_struct_ptr->EndValue =   atof(j->EndValue().c_str());
					CADIncrementParameter_struct_ptr->Increment =  atof(j->Increment().c_str());
					CADIncrementParameter_struct_ptr->Next_ptr = NULL;

					if ( !CADIncrementParameter_head_defined )
					{
						CADIncrementParameter_head_defined = true;
						CADIncrementParameter_head_ptr	  = CADIncrementParameter_struct_ptr;
						CADIncrementParameter_previous_ptr = CADIncrementParameter_struct_ptr;
					}
					else
					{
						CADIncrementParameter_previous_ptr->Next_ptr = CADIncrementParameter_struct_ptr;
						CADIncrementParameter_previous_ptr = CADIncrementParameter_struct_ptr;
					}
				} // end    for( CADComponentsType::CADCom...
				
				stream_CADIncrementParameter_structs(CADIncrementParameter_head_ptr, "    ", std::clog );
				WriteCSVHeaderLine( true, CADIncrementParameter_head_ptr, ReadParameter_list, MetricsOutputFile );
				set_CADIncrementParameters_read_ReadParameters( i->Name(), &p_model, CADIncrementParameter_head_ptr, IncrementParameter_list,ReadParameter_list, MetricsOutputFile );
				delete_CADIncrementParameter_structs(CADIncrementParameter_head_ptr);
			}   
			else
			{
				if ( ReadParameter_list.size() > 0 )
				{
					WriteCSVHeaderLine(false, NULL, ReadParameter_list, MetricsOutputFile );
					WriteMassPropertiesToCSVFile( &p_model, true, MetricsOutputFile);	
					WriteReadParametersToCSVFile( i->Name(), &p_model, false, ReadParameter_list, MetricsOutputFile);
				}
				else
				{
					std::clog << std::endl << "Part/Assembly: " << i->Name() << ", no CADIncrementParameter/CADReadParameter entries present in xml file.";
					std::cout << std::endl << "Part/Assembly: " << i->Name() << ", no CADIncrementParameter/CADReadParameter entries present in xml file.";
				}
			}  // if (i->ParametricParameters().get().Increment().present())
		}  // end    if ( i->ParametricParameters().present() )
		else
		{
			std::clog << std::endl << "Part/Assembly: " << i->Name() << ", no CADIncrementParameter entries present in xml file.";
			std::cout << std::endl << "Part/Assembly: " << i->Name() << ", no CADIncrementParameter entries present in xml file.";
		}

		MetricsOutputFile.close();
		isis::isis_ProMdlErase(p_model);

	}


} // end SetCADModelParamters

} // end namespace isis