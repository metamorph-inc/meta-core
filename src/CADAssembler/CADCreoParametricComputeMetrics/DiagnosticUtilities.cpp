#include <DiagnosticUtilities.h>

//#include <StringToEnumConversions.h>

namespace isis
{

void stream_CADComponentsTree( std::auto_ptr<CADComponentsType>  &in_CADComponents_ptr,
							   std::ostream &out_Stream )
{
	/////////////////////////////
	// Display contents of tree
	////////////////////////////
	
	for ( CADComponentsType::CADComponent_const_iterator i(in_CADComponents_ptr->CADComponent().begin()); 
		  i != in_CADComponents_ptr->CADComponent().end(); ++i )
	{
		out_Stream << std::endl << "CADComponent";   
		out_Stream << std::endl << "   Name              " << i->Name();
		out_Stream << std::endl << "   Type              " << i->Type();
		out_Stream << std::endl << "   MetricsOutputFile " << i->MetricsOutputFile();

		if ( i->ParametricParameters().present() )
		{	
			
			if (i->ParametricParameters().get().Increment().present())
			{
				for ( CADComponentsType::CADComponent_type::ParametricParameters_type::Increment_type::CADIncrementParameter_const_iterator j( i->ParametricParameters().get().Increment().get().CADIncrementParameter().begin());
						j != i->ParametricParameters().get().Increment().get().CADIncrementParameter().end();
					  ++j )	
				{
					out_Stream << std::endl << "      CADIncrementParameter";
					out_Stream << std::endl << "         Name        "  << j->Name();
					out_Stream << std::endl << "         Type        "  << j->Type();
					out_Stream << std::endl << "         StartValue  "  << j->StartValue();
					out_Stream << std::endl << "         EndValue    "  << j->EndValue();
					out_Stream << std::endl << "         Increment   "  << j->Increment();
				}
			}

			if (i->ParametricParameters().get().Read().present())
			{
				for ( CADComponentsType::CADComponent_type::ParametricParameters_type::Read_type::CADReadParameter_const_iterator j( i->ParametricParameters().get().Read().get().CADReadParameter().begin());
						j != i->ParametricParameters().get().Read().get().CADReadParameter().end();
					  ++j )	
				{
					out_Stream << std::endl << "      CADReadParameter";
					out_Stream << std::endl << "         Name        "  << j->Name();
				}
			}

		}  // if ( i->ParametricParameters().present() )

	}

} // end stream_CADComponentsTree



} // END namespace isis