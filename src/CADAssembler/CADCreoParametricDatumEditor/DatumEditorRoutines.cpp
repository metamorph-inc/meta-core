#include "DatumEditorRoutines.h"
#include <isis_ptc_toolkit_functions.h>
#include <isis_include_ptc_headers.h>
#include <StringToEnumConversions.h>
#include <GraphicsFunctions.h>
#include <CADCommonConstants.h>
#include <CoordinateSystemRoutines.h>
#include <CommonUtilities.h>
#include <sstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>


namespace isis
{

	const double CAD_PRECISION_TOLERANCE = .00001;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ErrorStatusMessage( const ErrorStatus &in_ErrorStatus)
	{
		std::string errorMessage;
		if ( in_ErrorStatus.warningsOccurred && in_ErrorStatus.errorsOccurred ) 
			errorMessage = "Errors and Warnings occurred.";
		else 
			if ( in_ErrorStatus.errorsOccurred ) 
				errorMessage = "Errors occurred.";
			else 
				if ( in_ErrorStatus.warningsOccurred )  
					errorMessage = "Warnings occurred.";
				else 
					errorMessage = "Completed successfully with no errors or warnings.";
		return errorMessage;
	}


	//int ErrorStatusCode( const ErrorStatus &in_ErrorStatus)
	//{
	//	if ( in_ErrorStatus.errorsOccurred ) return -1;
	//	else if ( in_ErrorStatus.warningsOccurred ) -2;
	//	else return 0;
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	std::string CADComponent_string( const CADDatumEditor::CADComponent &cADComponent)
	{
		std::stringstream output;
		output <<
		"   CADComponent Name:                      \"" << cADComponent.Name() <<		"\"" << std::endl <<
		"   CADComponent Type:                      \"" << cADComponent.Type() <<		"\"" << std::endl <<
		"   CADComponent LibraryID:                 \"" << cADComponent.LibraryID() << "\"";

		return output.str();
	}

	std::string AddCoordinateSystem_string(const CADDatumEditor::AddCoordinateSystem &in_AddCoordinateSystem)
	{
		std::stringstream output;
		output << 
		"   CADComponent CoordinateSystemName: \"" <<	in_AddCoordinateSystem.CoordinateSystemName() << "\"  ReplaceIfExists:  \"" <<  in_AddCoordinateSystem.ReplaceIfExists() << "\""  << std::endl;
	
		CADDatumEditor::Origin origin = in_AddCoordinateSystem.Origin_child();
		output << 
			"   Origin X: " << origin.X() << "  Y: " << origin.Y() << "  Z: " << origin.Z() << "\"" << std::endl;

		CADDatumEditor::XVector xVector = in_AddCoordinateSystem.XVector_child();
		output << 
			"   XVector X: " << xVector.X() << "  Y: " << xVector.Y() << "  Z: " << xVector.Z() << "\"" << std::endl;

		CADDatumEditor::YVector yVector = in_AddCoordinateSystem.YVector_child();
		output << 
			"   YVector X: " << yVector.X() << "  Y: " << yVector.Y() << "  Z: " << yVector.Z() << "\"";

		return output.str();
	}

	std::string AddDatum_string(const CADDatumEditor::AddDatum &in_AddDatum)
	{
		std::stringstream output;
		output << 
		"   CADComponent DatumName:                 \"" <<	in_AddDatum.DatumName()				<<	"\"" << std::endl <<
		"   CADComponent DatumType:                 \"" <<	in_AddDatum.DatumType()				<<	"\"" << std::endl <<
		"   CADComponent CoordinateName:            \"" <<  in_AddDatum.CoordinateSystemName()			<<	"\"" << std::endl <<
		"   CADComponent CoordinateAlignment:       \"" <<  in_AddDatum.CoordinateAlignment()		<<	"\"" << std::endl <<
		"   CADComponent FlipDatumPlaneDirection:   \"" <<  in_AddDatum.FlipDatumPlaneDirection()	<<	"\"" << std::endl <<
		"   CADComponent ReplaceIfExists:           \"" <<  in_AddDatum.ReplaceIfExists() << "\"";
		return output.str();
	}

	std::string DeleteDatum_string(const CADDatumEditor::DeleteDatum &in_DeleteDatum)
	{
		std::stringstream output;
		output << 
		"   CADComponent DatumName:                 \"" << in_DeleteDatum.DatumName()		<<	"\"" << std::endl <<
		"   CADComponent DatumType:                 \"" << in_DeleteDatum.DatumType()		<<	"\"";
			return output.str();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	enum e_MessageSeverity
	{
		CAD_INFORMATION,
		CAD_WARNING,
		CAD_ERROR
	};

	std::string MessageSeverity_string( e_MessageSeverity in_MessageSeverity )
	{
		switch ( in_MessageSeverity )
		{
		case CAD_INFORMATION:
			return "INFORMATION";
			break;
		case CAD_WARNING:
			return "WARNING";
			break;
		case CAD_ERROR:
			return "ERROR";
			break;

		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	std::string isis_FormatMessage ( e_MessageSeverity in_MessageSeverity,
									const std::string &in_Message)							
	{
		std::stringstream tempString;
		
		tempString << setiosflags(ios::left) << std::setw(12) << MessageSeverity_string(in_MessageSeverity) << " " << 
					  setiosflags(ios::left) << std::setw(45) << in_Message << "  ";
			
		//std::cout << std::endl << "isis_FormatMessage 1: " << tempString.str();
		return tempString.str();
	}

	/////////////////////////
	std::string isis_FormatMessage ( e_MessageSeverity in_MessageSeverity,
								const std::string &in_Message,
								const std::string &in_ModelName,
								const std::string &in_ModelType )								
	{
		std::stringstream tempString;
		std::string modelSuffix;
		//std::cout << std::endl << "isis_FormatMessage 2 A: " << tempString.str();
		if ( isis::ProMdlType_enum(in_ModelType) == PRO_PART )
			modelSuffix = "prt";
		else
			modelSuffix = "asm";
		
		tempString <<	isis_FormatMessage( in_MessageSeverity, in_Message) <<
						in_ModelName << "." << modelSuffix << "  ";			

		//std::cout << std::endl << "isis_FormatMessage 2 B: " << tempString.str();
		return tempString.str();
	}

	/////////////////////////
	std::string isis_FormatMessage ( e_MessageSeverity in_MessageSeverity,
								const std::string &in_Message,
								const std::string &in_ModelName,
								const std::string &in_ModelType,
								const std::string &in_DatumName,
								const std::string &in_DatumType )								
	{
		std::stringstream tempString;
		std::string modelSuffix;
		
		tempString <<	isis_FormatMessage(in_MessageSeverity, in_Message, in_ModelName, in_ModelType)  <<
						in_DatumName << "(" <<
						in_DatumType << ")"; 				

		//std::cout << std::endl << "isis_FormatMessage 3: " << tempString.str();
		return tempString.str();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool FeatureExistsInCreoModel(	ProMdl	in_p_model, 
									const std::string		&in_FeatureName, 
									ProType					in_FeatureType ) throw(isis::application_exception)
	{

		wchar_t  datumName_wide[ISIS_CHAR_BUFFER_LENGTH];

		ProStringToWstring(datumName_wide, (char *) in_FeatureName.c_str() );
		ProModelitem  modelDatumItem;

		bool datumExists = true;
		try
		{
			isis::isis_ProModelitemByNameInit (
						 in_p_model,
				         in_FeatureType, 
						 datumName_wide, 
						 &modelDatumItem);
		}
		catch (...)
		{
			datumExists = false;
		}

		return datumExists;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TraverseXMLDocument(Udm::SmartDataNetwork &in_parsedXMLDiagram,
							 DatumEditorDiagram_functor &in_DatumEditorDiagram_functor ) throw (isis::application_exception )
	{
		CADDatumEditor::CADDatumEditor DatumEditor_ptr = CADDatumEditor::CADDatumEditor::Cast(in_parsedXMLDiagram.GetRootObject());

		/////////////////////////////
		// Read Libaries Content
		/////////////////////////////
		vector<CADDatumEditor::Libraries> librariesVector = DatumEditor_ptr.Libraries_kind_children();
		
		for each( CADDatumEditor::Libraries i in librariesVector )
		{
			vector<CADDatumEditor::Library> libraryVector = i.Library_kind_children();
			for each ( CADDatumEditor::Library j in libraryVector)
			{
				in_DatumEditorDiagram_functor( CAD_PARTS_LIBRARY, j);
			}
		}

		vector<CADDatumEditor::CADComponents> cADComponentsVector = DatumEditor_ptr.CADComponents_kind_children();

		for each( CADDatumEditor::CADComponents i in  cADComponentsVector )
		{
			vector<CADDatumEditor::CADComponent> cADComponentsVector = i.CADComponent_kind_children();

			for each( CADDatumEditor::CADComponent j in cADComponentsVector )
			{
				vector<CADDatumEditor::Add> addVector = j.Add_kind_children();
				for each(CADDatumEditor::Add k in addVector )
				{
					/////////////////////////
					// Add Coordinate System
					/////////////////////////

					vector<CADDatumEditor::AddCoordinateSystems> addCoordinateSystemsVector = k.AddCoordinateSystems_kind_children();

					for each(CADDatumEditor::AddCoordinateSystems l in addCoordinateSystemsVector )
					{

						vector<CADDatumEditor::AddCoordinateSystem> addCoordinateSystemVector = l.AddCoordinateSystem_kind_children();

						for each(CADDatumEditor::AddCoordinateSystem m in addCoordinateSystemVector )
						{
							in_DatumEditorDiagram_functor( CAD_ADD_COORDINATE_SYSTEM, m, &j);
						}
					}
					////////////////
					// Add Datum
					////////////////

					vector<CADDatumEditor::AddDatums> addDatumsVector = k.AddDatums_kind_children();

					for each(CADDatumEditor::AddDatums l in addDatumsVector )
					{

						vector<CADDatumEditor::AddDatum> addDatumVector = l.AddDatum_kind_children();

						for each(CADDatumEditor::AddDatum m in addDatumVector )
						{
							in_DatumEditorDiagram_functor( CAD_ADD_DATUM, m, &j);
						}
					}
				 } // END for each(CADDatumEditor::Add k in addVector )

				////////////////
				// Delete Datum
				////////////////

				vector<CADDatumEditor::Delete> deleteVector = j.Delete_kind_children();

				for each(CADDatumEditor::Delete k in deleteVector )
				{
					vector<CADDatumEditor::DeleteDatums> deleteDatumsVector = k.DeleteDatums_kind_children();

					for each(CADDatumEditor::DeleteDatums l in deleteDatumsVector )
					{

						vector<CADDatumEditor::DeleteDatum> deleteDatumVector = l.DeleteDatum_kind_children();

						for each(CADDatumEditor::DeleteDatum m in deleteDatumVector )
						{
							in_DatumEditorDiagram_functor( CAD_DELETE_DATUM, m, &j);
						}
					}
				} 

			} // END for each( CADDatumEditor::CADComponent j in cADComponentsVector )

		} // END for each( CADDatumEditor::CADComponents i in  cADComponentsVector )
		
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	bool CADComponentFunction( e_DatumEditorFunction in_DatumEditorFunction )
	{	 
		return	( in_DatumEditorFunction == CAD_ADD_DATUM  || 
				in_DatumEditorFunction == CAD_DELETE_DATUM ||
				in_DatumEditorFunction == CAD_ADD_COORDINATE_SYSTEM );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void Log_DatumEditorDiagram_functor::operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
													 Udm::Object &in_UdmObject_function,
													 Udm::Object *in_UdmObject_component ) 
															throw (isis::application_exception )
	{

		if ( CADComponentFunction( in_DatumEditorFunction))
		{
			CADDatumEditor::CADComponent cADComponent = CADDatumEditor::CADComponent::Cast(*in_UdmObject_component);

			std::clog << std::endl << std::endl << "CADComponent Name: \"" << cADComponent.Name() << "\"  Type: \"" << cADComponent.Type() << "\"  LibraryID: \"" << cADComponent.LibraryID() << "\"";
			//std::cout << std::endl << "CADComponent Name: \"" << cADComponent.Name() << "\"  Type: \"" << cADComponent.Type() << "\"  LibraryID: \"" << cADComponent.LibraryID() << "\"";
		}

		if ( in_DatumEditorFunction == CAD_ADD_COORDINATE_SYSTEM  )
		{
			CADDatumEditor::AddCoordinateSystem addCoordinateSystem = CADDatumEditor::AddCoordinateSystem::Cast(in_UdmObject_function);
			std::clog << std::endl << "   *** ADD Coordinate System **** " << std::endl << AddCoordinateSystem_string(addCoordinateSystem);
		}


		if ( in_DatumEditorFunction == CAD_ADD_DATUM  )
		{
			CADDatumEditor::AddDatum addDatum = CADDatumEditor::AddDatum::Cast(in_UdmObject_function);

			std::clog << std::endl << "   *** ADD DATUM **** " << std::endl << AddDatum_string(addDatum);
		}

		else if ( in_DatumEditorFunction == CAD_DELETE_DATUM )
		{
			CADDatumEditor::DeleteDatum deleteDatum = CADDatumEditor::DeleteDatum::Cast(in_UdmObject_function);
			std::clog << std::endl <<  "   *** DELETE DATUM **** " << std::endl << DeleteDatum_string(deleteDatum);

		}

		else if ( in_DatumEditorFunction == CAD_PARTS_LIBRARY )
		{
			CADDatumEditor::Library library = CADDatumEditor::Library::Cast(in_UdmObject_function);

			clog << std::endl << std::endl << "Library ID: \"" << library.ID() << "\"  DirectoryPath: \"" << library.DirectoryPath() << "\""; 
			//cout << std::endl << "Library ID: \"" << library.ID() << "\"  DirectoryPath: \"" << library.DirectoryPath() << "\""; 
			
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDataValidity_DatumEditorDiagram_functor::operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
													 Udm::Object &in_UdmObject_function,
													 Udm::Object *in_UdmObject_component )
																throw (isis::application_exception )
	{
		CADDatumEditor::CADComponent cADComponent;

		if ( CADComponentFunction( in_DatumEditorFunction))
		{
			cADComponent = CADDatumEditor::CADComponent::Cast(*in_UdmObject_component);

			if ( std::string(cADComponent.Name()).size() == 0 ||
				 std::string(cADComponent.Type()).size() == 0 || 
				 std::string(cADComponent.LibraryID()).size() == 0 )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an empty string for one of the following entries.  Please search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent);
				throw isis::application_exception(messageString.str().c_str());	
			}
		}

		////////////////////////////////////
		// AddCoordinateSystem
		////////////////////////////////////
		if ( in_DatumEditorFunction == CAD_ADD_COORDINATE_SYSTEM  )
		{
			CADDatumEditor::AddCoordinateSystem addCoordinateSystem = CADDatumEditor::AddCoordinateSystem::Cast(in_UdmObject_function);
			//CADDatumEditor::Origin  origin  = addCoordinateSystem.Origin_child();
			CADDatumEditor::XVector xVector = addCoordinateSystem.XVector_child();
			CADDatumEditor::YVector yVector = addCoordinateSystem.YVector_child();

			//isis_CADCommon::Point_3D  orgin_temp( origin.X(), origin.Y(), origin.Z());
			isis_CADCommon::Point_3D  xVector_temp( xVector.X(), xVector.Y(), xVector.Z());
			isis_CADCommon::Point_3D  yVector_temp( yVector.X(), yVector.Y(), yVector.Z());

			// Check if vectors x and y are equal
			if ( isis_CADCommon::PointsAreEqual_3D( xVector_temp, yVector_temp, CAD_PRECISION_TOLERANCE ))
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains a coordinate system defintion that has identical x and y vectors." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddCoordinateSystem_string(addCoordinateSystem); 
				throw isis::application_exception(messageString.str().c_str());	
			}
			std::vector<double> xVector_tempVector;
			std::vector<double> yVector_tempVector;
			xVector_tempVector.push_back(xVector.X());
			xVector_tempVector.push_back(xVector.Y());
			xVector_tempVector.push_back(xVector.Z());
			yVector_tempVector.push_back(yVector.X());
			yVector_tempVector.push_back(yVector.Y());
			yVector_tempVector.push_back(yVector.Z());

			// Check if vectors x and y are orthogonal
			//std::cout << std::endl << " PI/2 " << M_PI / 2.0;
			//std::cout << std::endl << " Angle " << std::abs(isis_CADCommon::AngleBetweenVectors_3D(xVector_tempVector, yVector_tempVector));

			if ( !isis_CADCommon::ValueAreEqual( M_PI / 2.0, std::abs(isis_CADCommon::AngleBetweenVectors_3D(xVector_tempVector, yVector_tempVector)), CAD_PRECISION_TOLERANCE ))
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains a coordinate system defintion that has x and y vectors that are not orthogonal." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddCoordinateSystem_string(addCoordinateSystem); 
				throw isis::application_exception(messageString.str().c_str());	
			}
		}
		////////////////////////////////////
		// AddDatum
		////////////////////////////////////
		if ( in_DatumEditorFunction == CAD_ADD_DATUM  )
		{
			CADDatumEditor::AddDatum addDatum = CADDatumEditor::AddDatum::Cast(in_UdmObject_function);

			// Check for empty string for DatumName, DatumType, CoordinateName, ReplaceIfExists
			if ( std::string(addDatum.DatumName()).size() == 0 || 
				 std::string(addDatum.DatumType()).size() == 0  || 
				 std::string(addDatum.CoordinateSystemName()).size() == 0 ||
				 std::string(addDatum.ReplaceIfExists()).size() == 0 )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an empty string for one of the following entries.  Please search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddDatum_string(addDatum);
				throw isis::application_exception(messageString.str().c_str());	
			}
		
			bool errorOccured = false;
			ProType proDatumType;
			// Check if DatumType is of type Plane, Axis, or Point

			try
			{
				proDatumType = FeatureGeometryType_enum(addDatum.DatumType());
			}
			catch (...)
			{
				errorOccured = true;
			}

			if ( errorOccured || 
				( proDatumType!=PRO_SURFACE && // Surface is a Datum Plane
				  proDatumType!=PRO_AXIS && 
				  proDatumType!=PRO_POINT && 
				  proDatumType !=PRO_CSYS) )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an erroneous DatumType.  The allowed types are Datum, Axis, Point or CSyS. Search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddDatum_string(addDatum);
				throw isis::application_exception(messageString.str().c_str());	

			}


			ProBoolean replaceIfExists;
			try
			{
				replaceIfExists = String_To_ProBoolean(addDatum.ReplaceIfExists());
			}
			catch(...)
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains erroneous data.  ReplaceIfExists must be set to False (not supporting True yet).  Search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddDatum_string(addDatum);
				throw isis::application_exception(messageString.str().c_str());	

			}

			if ( replaceIfExists == PRO_B_TRUE && proDatumType ==PRO_AXIS )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains erroneous data. ReplaceIfExists must be set to False for Axes (not supporting True for Axes, are supporting true for Plane, Point, and CSys types).  Search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddDatum_string(addDatum);
				throw isis::application_exception(messageString.str().c_str());	
			}


			// Check if FlipDatumPlaneDirection is true, false, or empty  
			ProBoolean flipDatumPlaneDirection;
			try
			{
				flipDatumPlaneDirection = String_To_ProBoolean(addDatum.FlipDatumPlaneDirection());
			}
			catch(...)
			{
				//std::cout << std::endl << "proDatumType: " << proDatumType;
				if ( proDatumType == PRO_SURFACE )  // Datum Plane is of type PRO_SURFACE 
				{
					std::stringstream messageString;
					messageString << std::endl << std::endl <<
						"Error: The input xml contains erroneous data.  If DatumType = Plane, then FlipDatumPlaneDirection must be set to False (not supporting true yet).  Search the log file to locate the anomaly." << std::endl <<
						CADComponent_string( cADComponent) << std::endl <<
						AddDatum_string(addDatum);
					throw isis::application_exception(messageString.str().c_str());	
				}
				else 
				{
					if ( std::string(addDatum.FlipDatumPlaneDirection()).size() != 0 )
					{
						std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Axis or DatumType = Point, then FlipDatumPlaneDirection must be set to an empty string.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	
					}
				}
			}  // END Catch

			// Check if Axis or Point, then FlipDatumPlaneDirection must be empty.
			if (  ( proDatumType == PRO_AXIS ||
					proDatumType == PRO_POINT ) &&
					std::string(addDatum.FlipDatumPlaneDirection()).size() != 0 ) 
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains erroneous data.  If  DatumType = Axis or DatumType = Point, then FlipDatumPlaneDirection must be set to an empty string.  Search the log file to locate the anomaly." << std::endl <<
					CADComponent_string( cADComponent) << std::endl <<
					AddDatum_string(addDatum);
				throw isis::application_exception(messageString.str().c_str());	
			}

			// Check Point, then CoordinateAlignment must be empty.
			switch( proDatumType )
			{
				case PRO_POINT:
					if(	std::string(addDatum.CoordinateAlignment()).size() != 0 ) 
					{
						std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Point, then CoordinateAlignment must be set to an empty string.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	
					}

					break;
				case PRO_AXIS:
					ProType coordinateAxisAlignment;
					try
					{
						coordinateAxisAlignment = CoordinateAxisAlignment_enum( addDatum.CoordinateAlignment());
					}
					catch (...)
					{
					std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Axis or DatumType = Plane, then CoordinateAlignment must be set to x, y, z, xy, yz, or zx.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	

					}

					if (    coordinateAxisAlignment != PRO_CSYS_AXIS_X && 
							coordinateAxisAlignment != PRO_CSYS_AXIS_Y &&
							coordinateAxisAlignment != PRO_CSYS_AXIS_Z )
					{
						std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Axis, then CoordinateAlignment must be set to x, y, or z.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	
					}

					break;
				case PRO_SURFACE:

					//if ( flipDatumPlaneDirection == PRO_B_TRUE)
					//{
					//		std::stringstream messageString;
					//		messageString << std::endl << std::endl <<
					//			"Error: The input xml contains erroneous data.  If DatumType = Plane, then FlipDatumPlaneDirection must be set to False (not supporting true yet).  Search the log file to locate the anomaly." << std::endl <<
					//			CADComponent_string( cADComponent) << std::endl <<
					//			AddDatum_string(addDatum);
					//		throw isis::application_exception(messageString.str().c_str());	
				 	//}

					ProDtmplnConstrType coordinatePlaneAlignment;
					try
					{
						coordinatePlaneAlignment = CoordinatePlaneAlignment_enum( addDatum.CoordinateAlignment());
					}
					catch (...)
					{
					std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Plane, then CoordinateAlignment must be set xy, yz, or zx.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	

					}
					if (  coordinatePlaneAlignment != PRO_DTMPLN_THRU_CSYS_XY && 
						  coordinatePlaneAlignment != PRO_DTMPLN_THRU_CSYS_YZ &&
						  coordinatePlaneAlignment != PRO_DTMPLN_THRU_CSYS_ZX )
					{
						std::stringstream messageString;
						messageString << std::endl << std::endl <<
							"Error: The input xml contains erroneous data.  If  DatumType = Axis or DatumType = Plane, then CoordinateAlignment must be set to xy, yz, or zx.  Search the log file to locate the anomaly." << std::endl <<
							CADComponent_string( cADComponent) << std::endl <<
							AddDatum_string(addDatum);
						throw isis::application_exception(messageString.str().c_str());	
					}
					break;

			}
	
		}
		////////////////////////////////////
		// DeleteDatum
		////////////////////////////////////
		else if ( in_DatumEditorFunction == CAD_DELETE_DATUM )
		{
			CADDatumEditor::DeleteDatum deleteDatum = CADDatumEditor::DeleteDatum::Cast(in_UdmObject_function);

			// Check for empty string for DatumName, DatumType
			if ( std::string(deleteDatum.DatumName()).size() == 0 || 
				 std::string(deleteDatum.DatumType()).size() == 0  )

			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an empty string for one of the following entries.  Please search the log file to locate the anomaly." << std::endl <<
					CADComponent_string(cADComponent) <<
					DeleteDatum_string (deleteDatum);
				throw isis::application_exception(messageString.str().c_str());	
			}

			// Check if DatumType is of type PRO_SURFACE(Datum Plane type), PRO_POINT, PRO_AXIS, or PRO_CSYS
			ProType proDatumType;
			try
			{
				proDatumType = FeatureGeometryType_enum(deleteDatum.DatumType());
			}
			catch (...)
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an erroneous DatumType.  The allowed types are Datum, Axis, or Point. Search the log file to locate the anomaly." << std::endl <<
					"   CADComponent Name:          \"" << cADComponent.Name() <<		"\"" << std::endl <<
					"   CADComponent Type:          \"" << cADComponent.Type() <<		"\"" << std::endl <<
					"   CADComponent DatumName:     \"" << deleteDatum.DatumName()		<<	"\"" << std::endl <<
					"   CADComponent DatumType:     \"" << deleteDatum.DatumType()		<<	"\"";
				throw isis::application_exception(messageString.str().c_str());	
			}

			// Special Case PRO_AXIS not currently supported, see DeleteFeature(...) function for details
			if ( proDatumType == PRO_AXIS )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an erroneous DatumType.  Deleting an Axis is currently not supported. Search the log file to locate the anomaly." << std::endl <<
					"   CADComponent Name:          \"" << cADComponent.Name() <<		"\"" << std::endl <<
					"   CADComponent Type:          \"" << cADComponent.Type() <<		"\"" << std::endl <<
					"   CADComponent DatumName:     \"" << deleteDatum.DatumName()		<<	"\"" << std::endl <<
					"   CADComponent DatumType:     \"" << deleteDatum.DatumType()		<<	"\"";
				throw isis::application_exception(messageString.str().c_str());	
			}
		}

		////////////////////////////////////
		// LIBRARY
		////////////////////////////////////
		else if ( in_DatumEditorFunction == CAD_PARTS_LIBRARY )
		{
			CADDatumEditor::Library library = CADDatumEditor::Library::Cast(in_UdmObject_function);

			if ( std::string(library.ID()).size() == 0 || 
				 std::string(library.DirectoryPath()).size() == 0 )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The input xml contains an empty string for one of the following entries.  Please search the log file to locate the anomaly." << std::endl <<
					"   Library ID:              \"" << library.ID() <<		"\"" << std::endl <<
					"   Library DirectoryPath:   \"" << library.DirectoryPath() <<		"\"";
					throw isis::application_exception(messageString.str().c_str());	
			}

			//clog << std::endl << "Library ID: \"" << library.ID() << "\"  DirectoryPath: \"" << library.DirectoryPath() << "\""; 
			//cout << std::endl << "Library ID: \"" << library.ID() << "\"  DirectoryPath: \"" << library.DirectoryPath() << "\""; 
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateLibraryMap_DatumEditorDiagram_functor::operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
													 Udm::Object &in_UdmObject_function,
													 Udm::Object *in_UdmObject_component )
																throw (isis::application_exception )
	{

		if ( in_DatumEditorFunction == CAD_PARTS_LIBRARY )
		{
			CADDatumEditor::Library library = CADDatumEditor::Library::Cast(in_UdmObject_function);

			if (  libID_to_DirectoryPath_map.find(library.ID()) != libID_to_DirectoryPath_map.end() )
			{
				std::stringstream messageString;
				messageString << std::endl << std::endl <<
					"Error: The following library ID already exists. A library ID can appear only once in the input xml.  Please search the log file to locate the anomaly." << std::endl <<
					"   Library ID:              \"" << library.ID() <<		"\"" << std::endl <<
					"   Library DirectoryPath:   \"" << library.DirectoryPath() <<		"\"";
					throw isis::application_exception(messageString.str().c_str());	
			}

			libID_to_DirectoryPath_map[library.ID()] = library.DirectoryPath();

		}

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description: 
	//	This function deletes the following Creo feature types: 
	//		PRO_SURFACE
	//		PRO_DATUM_PLANE
	//		PRO_POINT
	//		PRO_CSYS
	//
	//	This function currently does not support deleting PRO_AXIS features. This is due to a Creo 
	//	SDK bug as reported in "PTC Case C11125674".
	//
	//	Pre-Conditions:
	//		p_model must point to an in-memory Creo model.
	//
	//	Post-Condtions:
	//	If in_FeatureName does not exist in p_model
	//		then
	//			throw isis::application_exception  // it is expected that a check is first made to assure the feature exists
	//	
	//	If in_FeatureType is not one of the types listed in the description as a supported type
	//		then
	//			throw isis::application_exception
	//////////////////////////////////////////////////////////////////////////////////////////////////
    void	DeleteFeature(	ProMdl				p_model, 
							const std::string	&in_FeatureName, 
							ProType				in_FeatureType )
															throw (isis::application_exception )
	{
		int *feature_list;
		ProName featureName;
		ProName modelName;		
		ProFeatureDeleteOptions *opts;
		ProModelitem  modelFeatureItem;
		ProType		  tempFeatureType;

		
		if ( in_FeatureType != PRO_SURFACE &&
			 in_FeatureType != PRO_DATUM_PLANE &&
			 in_FeatureType != PRO_POINT &&
			// in_FeatureType != PRO_AXIS &&
			 in_FeatureType != PRO_CSYS )
		{
			std::stringstream messageString;
			messageString << std::endl << std::endl <<
				"Error: (Function - DeleteFeature), received in_FeatureType that is not currently supported.  " << 
				"Supported feature types are PRO_SURFACE, PRO_DATUM_PLANE, PRO_POINT, and PRO_CSYS";
			throw isis::application_exception(messageString.str().c_str());
		}
		

		//isis::isis_ProMdlCurrentGet(&p_model);	

		//isis::isis_ProMdlRetrieve(L"plate_01", PRO_MDL_PART, &p_model);

		//isis::isis_ProMdlNameGet(p_model, lName);	
		//ProWstringToString(strmdlname, lName);
	
		// Datum Planes  - Work
		// Datum Points - Work
		// Datum Axis - Does not work
		// Coordinate Systems - Work with PRO_FEAT_DELETE_NO_OPTS


		//in_FeatureType = PRO_FEATURE;

		ProStringToWstring(featureName, (char *) in_FeatureName.c_str() );

		//std::cout << std::endl << "in_FeatureName: " << in_FeatureName;
		//char  modelName_narrow[500];
		//isis::isis_ProMdlNameGet(p_model, modelName );
		//ProWstringToString(modelName_narrow, modelName );

		// The following was an attempt to make delete to work with axes.  The problem is that
		// isis_ProModelitemByNameInit does not return the correct modelFeatureItem_temp.id for
		// the axis.  The id is three values higher than the actual feature id. 
		// if the following calls are made for axis features:
		//		sis::isis_ProModelitemByNameInit (p_model, PRO_FEATURE, featureName, &modelFeatureItem);
		//		sis::isis_ProFeatureWithoptionsDelete( (ProSolid)modelFeatureItem.owner, feature_list, opts, PRO_REGEN_NO_FLAGS );	
		// then isis_ProFeatureWithoptionsDelete will return a -1. 
		// The only approach that would appear to work would be to search through the model to 
		// find the correct axis id and then use isis::isis_ProFeatureInit.
		// PTC Case C11125674 was opened and PTC indicated that they will fix the problem with
		// ProModelitemByNameInit/ProFeatureWithoptionsDelete for axes.
		//
		//if ( in_FeatureType = PRO_AXIS )
		//{   // Hopefully would work for PRO_AXIS
		//	ProModelitem  modelFeatureItem_temp;
		//	isis::isis_ProModelitemByNameInit (p_model, PRO_AXIS, featureName, &modelFeatureItem_temp);	

		//	std::cout << std::endl << "modelFeatureItem_temp.id: " << modelFeatureItem_temp.id;
		//	std::cout << std::endl << "modelFeatureItem_temp.type: " << modelFeatureItem_temp.type;
		//	std::cout << std::endl << "modelFeatureItem_temp.owner: " << modelFeatureItem_temp.owner;

		//	isis::isis_ProFeatureInit( (ProSolid)p_model, modelFeatureItem_temp.id, &modelFeatureItem );		
		//}
		//else
		//{	// This works for Datum Planes, Points, and Coordinate Systems
		//	isis::isis_ProModelitemByNameInit (p_model, PRO_FEATURE, featureName, &modelFeatureItem);
		//}
	
		/*
		if ( in_FeatureType = PRO_AXIS )
		{   
			ProAxis		p_axis;
		
			if ( ! FoundProAxisBasedOnAxisName(p_model, in_FeatureName, p_axis) )
			{ 
				std::stringstream messageString;
				messageString << std::endl << "Attempted to delete an axis that does not exist in the Creo model.  Axis name: "
					<< featureName;
				throw isis::application_exception(messageString.str().c_str());
			}

			int axisID;
			isis::isis_ProAxisIdGet (p_axis, &axisID);
			isis::isis_ProFeatureInit( (ProSolid)p_model, axisID, &modelFeatureItem );	
			std::cout << std::endl << "modelFeatureItem_temp.id: " << modelFeatureItem.id;
			std::cout << std::endl << "modelFeatureItem_temp.type: " << modelFeatureItem.type;
			std::cout << std::endl << "modelFeatureItem_temp.owner: " << modelFeatureItem.owner;
		}
		else
		{	// This works for Datum Planes, Points, and Coordinate Systems
			isis::isis_ProModelitemByNameInit (p_model, PRO_FEATURE, featureName, &modelFeatureItem);
		}
		*/

		isis::isis_ProModelitemByNameInit (p_model, PRO_FEATURE, featureName, &modelFeatureItem);

		isis::isis_ProArrayAlloc( 0, sizeof(int), 1, (ProArray*)&feature_list);
		isis::isis_ProArrayObjectAdd((ProArray*)&feature_list, -1, 1, &modelFeatureItem.id);
				
		isis::isis_ProArrayAlloc( 1, sizeof(ProFeatureDeleteOptions), 1, (ProArray*)&opts);
		if ( in_FeatureType != PRO_CSYS )
			opts[0] = PRO_FEAT_DELETE_NO_OPTS;	// This is required for deleting coordinate systems.
		else
			opts[0] = PRO_FEAT_DELETE_CLIP;

		//opts[0] = PRO_FEAT_DELETE_CLIP;	
		//opts[0] = PRO_FEAT_DELETE_NO_OPTS;	
		//opts[0] = PRO_FEAT_DELETE_FIX;
		//opts[0] = PRO_FEAT_DELETE_KEEP_EMBED_DATUMS;
		//opts[0] = PRO_FEAT_DELETE_CLIP_INDIV_GP_MEMBERS;
		//opts[0] = PRO_FEAT_DELETE_CLIP_ALL;
		
		isis::isis_ProFeatureWithoptionsDelete( (ProSolid)modelFeatureItem.owner, feature_list, opts, PRO_REGEN_NO_FLAGS );	
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////

	void DeleteFeature_IfFailsLogErrorThrowException(
											const std::string	&in_modelName,
											const std::string	&in_modelType,
											ProMdl				in_p_model,
											const std::string	&in_FeatureName, 
											const std::string	&in_FeatureType,
											const std::string   &in_ErrorMessage,
											ErrorStatus			&out_ErrorStatus) throw (isis::application_exception )
	{
		try
		{
			DeleteFeature( in_p_model, in_FeatureName,  isis::FeatureGeometryType_enum(in_FeatureType));
		}

		catch ( isis::application_exception &ex )
		{
			std::stringstream messageString;
			out_ErrorStatus.errorsOccurred = true;
			messageString << std::endl << isis_FormatMessage (CAD_ERROR, in_ErrorMessage,
							in_modelName, in_modelType, in_FeatureName, in_FeatureType) << "  " << ex.what();
			throw isis::application_exception(messageString.str().c_str());	

		}
		catch ( ... )
		{
			std::stringstream messageString;
			out_ErrorStatus.errorsOccurred = true;
			messageString << std::endl << isis_FormatMessage (CAD_ERROR, in_ErrorMessage,
							in_modelName, in_FeatureType, in_FeatureName, in_FeatureType);
			throw isis::application_exception(messageString.str().c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// If in_DatumName_wide datum plane already exists, then this function would create a datum plane
	// named with the default (e.g. DTM1, DTM2...) naming conventon; otherwise, it could create a 
	// datum plane named in_DatumName_wide.
	void AddDatumBasedOnCoordinateSystem_DatumPlane( 
							ProMdl					in_p_model,
							ProSelection			in_ModelCoordinateSystemSelection,
							ProDtmplnConstrType		in_CoordinateSystemAlignment,
							const std::string		&in_DatumName,
							const std::string		&in_DatumType,
							bool					in_FlipDatumPlaneDirection )
	{
		wchar_t  datumName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(datumName_wide, (char *) isis::ConvertToUpperCase(in_DatumName).c_str() );

		//======================================================
		//  PRO_E_FEATURE_TREE
		//======================================================
		ProElement	elem_tree, elem_ftype, elem_consts, elem_datum_plane_const;
		isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree);

		//======================================================
		// PRO_E_FEATURE_TYPE
		//======================================================
		isis_ProElementAlloc(PRO_E_FEATURE_TYPE, &elem_ftype);
		ProValue	 value;
		ProValueData value_data;

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_FEAT_DATUM;
		
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		//std::cout << std::endl << "Before 1 isis_ProElementValueSet(elem_const_type, value);";
		isis_ProElementValueSet(elem_ftype, value);
		//std::cout << std::endl << "After 1 isis_ProElementValueSet(elem_const_type, value);";
		
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);

		//===================================================================
		// PRO_E_STD_FEATURE_NAME
		//===================================================================
		ProElement	elem_feature_name_type;
		isis_ProElementAlloc(PRO_E_STD_FEATURE_NAME, &elem_feature_name_type);

		value_data.type = PRO_VALUE_TYPE_WSTRING;
		value_data.v.w = datumName_wide; 
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_feature_name_type, value);

		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_feature_name_type);

		//======================================================
		// PRO_E_DTMPLN_CONSTRAINTS
		//======================================================
		isis_ProElementAlloc(PRO_E_DTMPLN_CONSTRAINTS, &elem_consts);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_consts);

		//================================================================
		// PRO_E_DTMPLN_CONSTRAINT
		//================================================================
		isis_ProElementAlloc(PRO_E_DTMPLN_CONSTRAINT, &elem_datum_plane_const);
		isis_ProElemtreeElementAdd(elem_consts, NULL, elem_datum_plane_const);

		//===================================================================
		// PRO_E_DTMPLN_CONSTR_TYPE
		//===================================================================
		ProElement	elem_const_type;
		isis_ProElementAlloc(PRO_E_DTMPLN_CONSTR_TYPE, &elem_const_type);
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = in_CoordinateSystemAlignment;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_const_type, value);

		isis_ProElemtreeElementAdd(elem_datum_plane_const, NULL, elem_const_type);

		//=============================================================== 
		// PRO_E_DTMPLN_CONSTR_REF
		//=============================================================== 
		ProElement	elem_datum_plane_const_ref;
		isis_ProElementAlloc(PRO_E_DTMPLN_CONSTR_REF, &elem_datum_plane_const_ref);
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r = in_ModelCoordinateSystemSelection;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		isis_ProElementValueSet(elem_datum_plane_const_ref, value);

		isis_ProElemtreeElementAdd(elem_datum_plane_const, NULL, elem_datum_plane_const_ref);

		//======================================================
		// PRO_E_DTMPLN_FLIP_DIR
		//======================================================

		if ( in_FlipDatumPlaneDirection )
		{
			isis_ProElementAlloc(PRO_E_DTMPLN_FLIP_DIR, &elem_ftype);
			ProValue	 value;
			ProValueData value_data;

			value_data.type = PRO_VALUE_TYPE_INT;
			value_data.v.i =  PRO_DTMPLN_FLIP_DIR_YES;
		
			isis_ProValueAlloc(&value);
			isis_ProValueDataSet(value, &value_data);
			isis_ProElementValueSet(elem_ftype, value);
			isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);
		}

		//=============================================================== 
		// Intialize Pro Selection
		//=============================================================== 
		ProAsmcomppath model_path;
		ProIdTable c_id_table;
		c_id_table [0] = -1;
		isis::isis_ProAsmcomppathInit((ProSolid)in_p_model, c_id_table, 0, &model_path);

		ProModelitem	model_modelitem;
		isis_ProMdlToModelitem(model_path.owner, &model_modelitem);	
	

		ProSelection	mdl_sel;
		isis_ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);

		//=============================================================== 
		// Create the Datum Plane
		//=============================================================== 

		ProFeature	feature;
		ProErrorlist	errors;

		isis_ProFeatureCreate(mdl_sel, elem_tree, NULL, 0, &feature, &errors);

		isis_ProElementFree(&elem_tree);

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// If in_DatumName_wide datum point already exists, then this function would create a second datum
	// point with the same name. 
	void AddDatumBasedOnCoordinateSystem_DatumPoint( 
							ProMdl					in_p_model,
							ProSelection			in_ModelCoordinateSystemSelection,
							const std::string		&in_DatumName,
							const std::string		&in_DatumType )
	{
		
		wchar_t  datumName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(datumName_wide, (char *) isis::ConvertToUpperCase(in_DatumName).c_str() );
		//======================================================
		//  PRO_E_FEATURE_TREE
		//======================================================
		ProElement	elem_tree, elem_ftype, elem_consts, elem_datum_point_const;
		isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree);

		//======================================================
		// PRO_E_FEATURE_TYPE
		//======================================================
		isis_ProElementAlloc(PRO_E_FEATURE_TYPE, &elem_ftype);
		ProValue	 value;
		ProValueData value_data;

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_FEAT_DATUM_POINT;
		
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		//std::cout << std::endl << "Before 1 isis_ProElementValueSet(elem_const_type, value);";
		isis_ProElementValueSet(elem_ftype, value);
		//std::cout << std::endl << "After 1 isis_ProElementValueSet(elem_const_type, value);";
		
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);

		//======================================================
		// PRO_E_DPOINT_TYPE
		//======================================================
		isis_ProElementAlloc(PRO_E_DPOINT_TYPE, &elem_ftype);

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_DPOINT_TYPE_OFFSET_CSYS;
		
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		//std::cout << std::endl << "Before 1 isis_ProElementValueSet(elem_const_type, value);";
		isis_ProElementValueSet(elem_ftype, value);
		//std::cout << std::endl << "After 1 isis_ProElementValueSet(elem_const_type, value);";
		
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);

		//===================================================================
		// PRO_E_STD_FEATURE_NAME
		//===================================================================
		/*
		ProElement	elem_feature_name_type;
		isis_ProElementAlloc(PRO_E_STD_FEATURE_NAME, &elem_feature_name_type);

		value_data.type = PRO_VALUE_TYPE_WSTRING;
		value_data.v.w = in_DatumName_wide; 
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_feature_name_type, value);

		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_feature_name_type);
		*/
		//===================================================================
		// PRO_E_DPOINT_OFST_CSYS_TYPE
		//===================================================================
		ProElement	elem_ofst_csys_type;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_TYPE, &elem_ofst_csys_type);

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_DTMPNT_CONSTR_TYPE_CARTESIAN;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_ofst_csys_type, value);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ofst_csys_type);

		//===================================================================
		// PRO_E_DPOINT_OFST_CSYS_REF
		//===================================================================
		ProElement	elem_ofst_csys_ref;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_REF, &elem_ofst_csys_ref);

		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r = in_ModelCoordinateSystemSelection;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_ofst_csys_ref, value);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ofst_csys_ref);

		//===================================================================
		// PRO_E_DPOINT_OFST_CSYS_WITH_DIMS
		//===================================================================
		ProElement	elem_ofst_csys_with_dims;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_WITH_DIMS, &elem_ofst_csys_with_dims);

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_B_FALSE;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_ofst_csys_with_dims, value);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ofst_csys_with_dims);

		//======================================================
		// PRO_E_DPOINT_OFST_CSYS_PNTS_ARRAY
		//======================================================
		ProElement	elem_ofst_csys_pnts_array;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_PNTS_ARRAY, &elem_ofst_csys_pnts_array);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ofst_csys_pnts_array);

		//================================================================
		// PRO_E_DPOINT_OFST_CSYS_PNT
		//================================================================
		ProElement	elem_ofst_csys_pnt;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_PNT, &elem_ofst_csys_pnt);
		isis_ProElemtreeElementAdd(elem_ofst_csys_pnts_array, NULL, elem_ofst_csys_pnt);

		//===================================================================
		// PRO_E_DPOINT_OFST_CSYS_PNT
		//===================================================================
		ProElement	elem_ofst_csys_pnt_name;
		isis_ProElementAlloc(PRO_E_DPOINT_OFST_CSYS_PNT_NAME, &elem_ofst_csys_pnt_name);

		value_data.type = PRO_VALUE_TYPE_WSTRING;
		value_data.v.w = datumName_wide; 

		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_ofst_csys_pnt_name, value);

		isis_ProElemtreeElementAdd(elem_ofst_csys_pnt, NULL, elem_ofst_csys_pnt_name);
		


		//=============================================================== 
		// Intialize Pro Selection
		//=============================================================== 
		ProAsmcomppath model_path;
		ProIdTable c_id_table;
		c_id_table [0] = -1;
		isis::isis_ProAsmcomppathInit((ProSolid)in_p_model, c_id_table, 0, &model_path);

		ProModelitem	model_modelitem;
		isis_ProMdlToModelitem(model_path.owner, &model_modelitem);	
		
		ProSelection	mdl_sel;
		isis_ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);

		//=============================================================== 
		// Create the Datum Point
		//=============================================================== 

		ProFeature	feature;
		ProErrorlist	errors;

		isis_ProFeatureCreate(mdl_sel, elem_tree, NULL, 0, &feature, &errors);

		isis_ProElementFree(&elem_tree);

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// If in_DatumName_wide datum axis already exists, then this function would create a datum axis
	// named with the default (e.g. DTM1, DTM2...) naming conventon; otherwise, it could create a 
	// datum axis named in_DatumName_wide.

	// ERROR This function DOES NOT WORK.  This is because the API does not support
	// adding an axis based on a coordinate system
	void AddDatumBasedOnCoordinateSystem_DatumAxis( 
							ProMdl					in_p_model,
							ProModelitem			&in_ModelCoordinateSystemItem,
							ProType					in_CoordinateSystemAlignment,
							const std::string		&in_DatumName,
							const std::string		&in_DatumType )
	{
		wchar_t  datumName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(datumName_wide, (char *) isis::ConvertToUpperCase(in_DatumName).c_str() );

		//======================================================
		//  PRO_E_FEATURE_TREE
		//======================================================
		ProElement	elem_tree, elem_ftype, elem_consts, elem_datum_axis_const;
		isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree);

		//======================================================
		// PRO_E_FEATURE_TYPE
		//======================================================
		isis_ProElementAlloc(PRO_E_FEATURE_TYPE, &elem_ftype);
		ProValue	 value;
		ProValueData value_data;

		value_data.type = PRO_VALUE_TYPE_INT;
	    value_data.v.i =  PRO_FEAT_DATUM_AXIS;
		
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		//std::cout << std::endl << "Before 1 isis_ProElementValueSet(elem_const_type, value);";
		isis_ProElementValueSet(elem_ftype, value);
		//std::cout << std::endl << "After 1 isis_ProElementValueSet(elem_const_type, value);";
		
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);

		//===================================================================
		// PRO_E_STD_FEATURE_NAME
		//===================================================================
		ProElement	elem_feature_name_type;
		isis_ProElementAlloc(PRO_E_STD_FEATURE_NAME, &elem_feature_name_type);

		value_data.type = PRO_VALUE_TYPE_WSTRING;
		value_data.v.w = datumName_wide; 
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_feature_name_type, value);

		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_feature_name_type);


		//======================================================
		// PRO_E_DTMAXIS_CONSTRAINTS
		//======================================================
		isis_ProElementAlloc(PRO_E_DTMAXIS_CONSTRAINTS, &elem_consts);
		isis_ProElemtreeElementAdd(elem_tree, NULL, elem_consts);

		//================================================================
		// PRO_E_DTMAXIS_CONSTRAINT
		//================================================================
		isis_ProElementAlloc(PRO_E_DTMAXIS_CONSTRAINT, &elem_datum_axis_const);
		isis_ProElemtreeElementAdd(elem_consts, NULL, elem_datum_axis_const);

		//===================================================================
		// PRO_E_DTMAXIS_CONSTR_TYPE
		//===================================================================
		ProElement	elem_const_type;
		isis_ProElementAlloc(PRO_E_DTMAXIS_CONSTR_TYPE, &elem_const_type);
		value_data.type = PRO_VALUE_TYPE_INT;
		//value_data.v.i = in_CoordinateSystemAlignment;
		value_data.v.i = PRO_DTMAXIS_CONSTR_TYPE_THRU;
		//value_data.v.i = PRO_DTMAXIS_CSYS_AXIS_X;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);

		isis_ProElementValueSet(elem_const_type, value);

		isis_ProElemtreeElementAdd(elem_datum_axis_const, NULL, elem_const_type);

		//======================================================
		// Get the PRO_CSYS_AXIS_X, Y, or Z
		//======================================================
		 ProModelitem csys_axis;
		 //isis_ProModelitemInit((ProMdl)in_ModelCoordinateSystemItem.owner, in_ModelCoordinateSystemItem.id, PRO_CSYS_AXIS_X, &csys_axis);
		 isis_ProModelitemInit((ProMdl)in_ModelCoordinateSystemItem.owner, in_ModelCoordinateSystemItem.id, in_CoordinateSystemAlignment, &csys_axis);
		 ProSelection     rev_srf_sel;
		 isis_ProSelectionAlloc(NULL, &csys_axis, &rev_srf_sel);

		//=============================================================== 
		// PRO_E_DTMAXIS_CONSTR_REF
		//=============================================================== 
		ProElement	elem_datum_axis_const_ref;
		isis_ProElementAlloc(PRO_E_DTMAXIS_CONSTR_REF, &elem_datum_axis_const_ref);
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r = rev_srf_sel;
		isis_ProValueAlloc(&value);
		isis_ProValueDataSet(value, &value_data);
		isis_ProElementValueSet(elem_datum_axis_const_ref, value);

		isis_ProElemtreeElementAdd(elem_datum_axis_const, NULL, elem_datum_axis_const_ref);
		
		// Constraint
		
		//=============================================================== 
		// Intialize Pro Selection
		//=============================================================== 
		ProAsmcomppath model_path;
		ProIdTable c_id_table;
		c_id_table [0] = -1;
		isis::isis_ProAsmcomppathInit((ProSolid)in_p_model, c_id_table, 0, &model_path);

		ProModelitem	model_modelitem;
		isis_ProMdlToModelitem(model_path.owner, &model_modelitem);	
		
		ProSelection	mdl_sel;
		isis_ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);

		//=============================================================== 
		// Create the Datum Axis
		//=============================================================== 

		ProFeature	feature;
		ProErrorlist	errors;

		isis_ProFeatureCreate(mdl_sel, elem_tree, NULL, 0, &feature, &errors);

		isis_ProElementFree(&elem_tree);

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	void AddDatumBasedOnCoordinateSystem(	const std::string		&in_modelName,
											const std::string		&in_modelType,
											ProMdl					in_p_model,
											const std::string		&in_CoordinateSystemName,
											const std::string		&in_CoordinateSystemAlignment,
											const std::string		&in_DatumName,
											const std::string		&in_DatumType,
											bool					in_FlipDatumPlaneDirection,
											//bool					in_ReplaceIfExists,
											ErrorStatus				&out_ErrorStatus)
	{

		//===========================================
		// Retrieve the coordinate system element ID
		//===========================================
		ProModelitem  modelCoordinateSystemItem;
		wchar_t  CoordinateSystemName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(CoordinateSystemName_wide, (char *) isis::ConvertToUpperCase(in_CoordinateSystemName).c_str() );

		try
		{
			isis::isis_ProModelitemByNameInit (in_p_model, PRO_CSYS, CoordinateSystemName_wide, &modelCoordinateSystemItem);
		}
		catch(isis::application_exception &ex)
		{
			out_ErrorStatus.errorsOccurred = true;
			std::stringstream messageString;
			messageString << std::endl << 
			isis_FormatMessage ( CAD_ERROR, "Coordinate system not found",
						  in_modelName,in_modelType, in_DatumName, in_DatumType) << 
						  " Coordinate System: " << in_CoordinateSystemName << "  " << ex.what();

			throw isis::application_exception(messageString.str().c_str());
		}
		catch(...)
		{
			out_ErrorStatus.errorsOccurred = true;
			std::stringstream messageString;
			messageString << std::endl << 
			isis_FormatMessage ( CAD_ERROR, "Coordinate system not found",
						  in_modelName,in_modelType, in_DatumName, in_DatumType) << 
						  " Coordinate System: " << in_CoordinateSystemName;

			throw isis::application_exception(messageString.str().c_str());
		}

		ProSelection modelCoordinateSystemSelection;
		isis::isis_ProSelectionAlloc(NULL, &modelCoordinateSystemItem, &modelCoordinateSystemSelection );

		try
		{
			switch ( FeatureGeometryType_enum(in_DatumType) )
			{
				case PRO_SURFACE:
					AddDatumBasedOnCoordinateSystem_DatumPlane( 
										in_p_model,
										modelCoordinateSystemSelection,
										CoordinatePlaneAlignment_enum(in_CoordinateSystemAlignment),
										in_DatumName,
										in_DatumType,
										in_FlipDatumPlaneDirection );

					break;
				case PRO_AXIS:
					AddDatumBasedOnCoordinateSystem_DatumAxis( 
										in_p_model,
										modelCoordinateSystemItem,
										CoordinateAxisAlignment_enum(in_CoordinateSystemAlignment),
										in_DatumName,
										in_DatumType );

					break;
				case PRO_POINT:
					AddDatumBasedOnCoordinateSystem_DatumPoint( 
										in_p_model,
										modelCoordinateSystemSelection,
										in_DatumName,
										in_DatumType );

				break;

			}  // END Switch
		}	
		catch ( isis::application_exception& ex )
		{
			out_ErrorStatus.errorsOccurred = true;
			std::stringstream messageString;
			messageString << std::endl << 
			isis_FormatMessage ( CAD_ERROR, "Datum creation failed",
							in_modelName,in_modelType, in_DatumName, in_DatumType) << 
							" isis:application_exception: " << ex.what();
			throw isis::application_exception(messageString.str().c_str());
		}
		catch ( ... )
		{
			out_ErrorStatus.errorsOccurred = true;
			std::stringstream messageString;
			messageString << std::endl << 
			isis_FormatMessage ( CAD_ERROR, "Datum creation failed",
							in_modelName,in_modelType, in_DatumName, in_DatumType) << 
							" Unexpected Exception";
			throw isis::application_exception(messageString.str().c_str());
		}
		isis_ProSelectionFree(&modelCoordinateSystemSelection);
	}									  

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void Add_FrontTopRight_Datums_IfTheyDoNotExist(	ProMdl &in_p_model, 
											const std::string in_ModelName,
											const std::string in_ModelType,
											ProMdlType proMdlType )
	{
		ProCsys p_csys;
		FindDefaultCoordinateSystem( in_p_model,
									 in_ModelName,
									 p_csys );

		int id;
		isis::isis_ProCsysIdGet (p_csys, &id);
		ProModelitem	mdlitem;
		isis::isis_ProModelitemInit( in_p_model, id, PRO_CSYS, &mdlitem);

		ProName csysName_wide;
		isis::isis_ProModelitemNameGet(&mdlitem, csysName_wide);

		char temp_string[PRO_NAME_SIZE];
		ProWstringToString(temp_string, csysName_wide);

		std::string csysName_string(temp_string);

		// Must create the intial three Datums
		std::string datumFront, datumRight, datumTop;
		if ( proMdlType == PRO_ASSEMBLY )
		{
			datumFront = "ASM_FRONT";
			datumRight = "ASM_RIGHT";
			datumTop   = "ASM_TOP";
		}
		else
		{
			datumFront = "FRONT";
			datumRight = "RIGHT";
			datumTop   = "TOP";
		}

		ErrorStatus errorStatus;

		if ( !FeatureExistsInCreoModel( in_p_model, datumFront, PRO_SURFACE ))
		{
				AddDatumBasedOnCoordinateSystem( 
					in_ModelName, 
					in_ModelType, 
					in_p_model,
					csysName_string,
					"XY",
					datumFront,
					"SURFACE",
					false,
					errorStatus);
		}

		if ( !FeatureExistsInCreoModel( in_p_model, datumRight, PRO_SURFACE ))
		{
				AddDatumBasedOnCoordinateSystem( 
					in_ModelName, 
					in_ModelType, 
					in_p_model,
					csysName_string,
					"YZ",
					datumRight,
					"SURFACE",
					false,
					errorStatus);
		}

		if ( !FeatureExistsInCreoModel( in_p_model, datumTop, PRO_SURFACE ))
		{
				AddDatumBasedOnCoordinateSystem( 
					in_ModelName, 
					in_ModelType, 
					in_p_model,
					csysName_string,
					"ZX",
					datumTop,
					"SURFACE",
					false,
					errorStatus);
		}
	}				
	//////////////////////////////////////////////////////////////////////////////////////////////////

    void DeleteAndCreateCSysAndDatums(	Udm::SmartDataNetwork	&in_parsedXMLDiagram,
								const std::map<std::string, std::string> &in_LibID_to_DirectoryPath_map,
								const std::string		&in_StartingDirectory,
								ErrorStatus				&out_ErrorStatus)
								throw (isis::application_exception )
	{

		ProPath  startingDirectory_wchar;
		ProStringToWstring(startingDirectory_wchar, (char *)in_StartingDirectory.c_str() );

		std::clog << std::endl;
		CADDatumEditor::CADDatumEditor DatumEditor_ptr = CADDatumEditor::CADDatumEditor::Cast(in_parsedXMLDiagram.GetRootObject());

		vector<CADDatumEditor::CADComponents> cADComponentsVector = DatumEditor_ptr.CADComponents_kind_children();

							
		for each( CADDatumEditor::CADComponents i in  cADComponentsVector )
		{
			vector<CADDatumEditor::CADComponent> cADComponentsVector = i.CADComponent_kind_children();

			for each( CADDatumEditor::CADComponent j in cADComponentsVector )
			{

				/////////////////////////
				// Set Working Directory 
				////////////////////////

				std::map<std::string, std::string>::const_iterator lib_itr = in_LibID_to_DirectoryPath_map.find(j.LibraryID());
				if (  lib_itr == in_LibID_to_DirectoryPath_map.end() )
				{
					std::stringstream messageString;

					out_ErrorStatus.errorsOccurred = true;
					std::clog << std::endl << 
					isis_FormatMessage(CAD_ERROR, "Invalid library ID", j.Name(), j.Type()) << " Library ID: " << j.LibraryID();
					throw isis::application_exception(messageString.str().c_str());	
				}

				ProPath  WorkingDirPath_wchar;
				ProStringToWstring(WorkingDirPath_wchar, (char *)lib_itr->second.c_str() );
				try
				{
					// Set to the starting directory first, in case a relative path was used
					// e.g.  .   or ./Assembly
					isis::isis_ProDirectoryChange( startingDirectory_wchar );
					isis::isis_ProDirectoryChange( WorkingDirPath_wchar );
				}
				catch ( isis::application_exception &ex)
				{
					std::stringstream messageString;
					out_ErrorStatus.errorsOccurred = true;
					messageString << std::endl << 
						isis_FormatMessage(CAD_ERROR, "Could not set to library dir", j.Name(), j.Type()) << 
						" Library ID: " << j.LibraryID() << " Directory: "<< lib_itr->second << "  " << ex.what();
					throw isis::application_exception(messageString.str().c_str());
				}
				catch ( ... )
				{
					std::stringstream messageString;
					out_ErrorStatus.errorsOccurred = true;
					messageString << std::endl << 
						isis_FormatMessage(CAD_ERROR, "Could not set to library dir", j.Name(), j.Type()) << 
						" Library ID: " << j.LibraryID() << " Directory: "<< lib_itr->second;
					throw isis::application_exception(messageString.str().c_str());
				}

				/////////////////////////
				// Open the Creo Model 
				////////////////////////
				wchar_t  modelName[ISIS_CHAR_BUFFER_LENGTH];
				ProStringToWstring(modelName, (char *) std::string(j.Name()).c_str() );
				ProMdl     p_model;

				if ( CadFileType_enum(j.Format()) == CAD_STEP )
				{
					// Read STEP File
					wchar_t  stepFileName[ISIS_CHAR_BUFFER_LENGTH];
					ProStringToWstring(stepFileName, (char *) std::string(j.File()).c_str() );

					ProMdlType proMdlType = isis::ProMdlType_enum(j.Type()); //PRO_ASSEMBLY/PRO_PART
					isis::isis_ProIntfimportModelWithOptionsCreate( 
										stepFileName,						//ProPath  import_file, 
										NULL,	  							//ProP4ath  profile, 
										PRO_INTF_IMPORT_STEP,				//ProIntfImportType type, 
										proMdlType,							//ProMdlType create_type, PRO_ASSEMBLY/PRO_PART
										PRO_IMPORTREP_MASTER,				//ProImportRepType rep_type, 
										modelName,							//ProName  new_model_name, 
										NULL,								//ProIntfimportLayerFilter filter_func, 
										NULL,								//ProAppData application_data, 
										&p_model);							//ProMdl* created_model 

					// Add for assemblies, ASM_FRONT, ASM_RIGHT, ASM_TOP, 
					// Add for parts FRONT, RIGHT, TOP
					Add_FrontTopRight_Datums_IfTheyDoNotExist( p_model, j.Name(), j.Type(), proMdlType );
				}
				else
				{
					// Read Creo Model
					try 
					{
						isis::isis_ProMdlRetrieve(modelName, (ProMdlType)isis::ProMdlType_enum(j.Type()), &p_model);
					}
					catch ( isis::application_exception &ex)
					{
						std::stringstream messageString;
						out_ErrorStatus.errorsOccurred = true;
						messageString << std::endl << 
							isis_FormatMessage(CAD_ERROR, "Could not open Creo Model", j.Name(), j.Type()) << 
							" Library ID: " << j.LibraryID() << " Directory: "<< lib_itr->second << "  " << ex.what();
						throw isis::application_exception(messageString.str().c_str());
					}
					catch ( ... )
					{
						std::stringstream messageString;
						out_ErrorStatus.errorsOccurred = true;
						messageString << std::endl << 
							isis_FormatMessage(CAD_ERROR, "Could not open Creo Model", j.Name(), j.Type()) << 
							" Library ID: " << j.LibraryID() << " Directory: "<< lib_itr->second;
						throw isis::application_exception(messageString.str().c_str());
					}
				}
			
				////////////////
				// Delete
				////////////////
	
				vector<CADDatumEditor::Delete> deleteVector = j.Delete_kind_children();

				for each(CADDatumEditor::Delete k in deleteVector )
				{
					vector<CADDatumEditor::DeleteDatums> deleteDatumsVector = k.DeleteDatums_kind_children();

					for each(CADDatumEditor::DeleteDatums l in deleteDatumsVector )
					{

						vector<CADDatumEditor::DeleteDatum> deleteDatumVector = l.DeleteDatum_kind_children();

						for each(CADDatumEditor::DeleteDatum m in deleteDatumVector )
						{

							bool featureExists = FeatureExistsInCreoModel( p_model, m.DatumName(), FeatureGeometryType_enum(m.DatumType()) );

							if ( featureExists )
							{
																						
								DeleteFeature_IfFailsLogErrorThrowException(
											j.Name(),
											j.Type(),
											p_model,
											m.DatumName(), 
											 m.DatumType(),
											"Delete failed even though the datum feature exists in the Creo Model.",
											out_ErrorStatus);

								std::stringstream messageString;
								messageString << std::endl << 
									isis_FormatMessage (CAD_INFORMATION, "Deleted feature.",
										j.Name(), j.Type(), m.DatumName(), m.DatumType());
								std::clog << messageString.str();
								std::cout << messageString.str();

							}
							else
							{			
								std::stringstream messageString;
								messageString << std::endl << isis_FormatMessage (CAD_WARNING, "Delete ignored because the datum feature does not exist in the Creo Model.",
														j.Name(), j.Type(), m.DatumName(), m.DatumType());
								std::cout << messageString.str();
								std::clog << messageString.str();
							}// END if ( featureExists )
						} // END for each(CADDatumEditor::DeleteDatum m in deleteDatumVector )
					}
				} 

				////////////////
				// Add
				////////////////

				vector<CADDatumEditor::Add> addVector = j.Add_kind_children();

				for each(CADDatumEditor::Add k in addVector )
				{
					////////////////////////
					// AddCoordinateSystems
					////////////////////////
					vector<CADDatumEditor::AddCoordinateSystems> addCoordinateSystemsVector = k.AddCoordinateSystems_kind_children();

					ProCsys referenceCoordinateSystem;

					if ( addCoordinateSystemsVector.size() > 0 )
					{
						FindDefaultCoordinateSystem ( p_model, j.Name(), referenceCoordinateSystem );
					}

					for each(CADDatumEditor::AddCoordinateSystems l in addCoordinateSystemsVector )
					{

						vector<CADDatumEditor::AddCoordinateSystem> addCoordinateSystemVector = l.AddCoordinateSystem_kind_children();

						for each(CADDatumEditor::AddCoordinateSystem m in addCoordinateSystemVector )
						{			
							bool featureExists = FeatureExistsInCreoModel( p_model, m.CoordinateSystemName(), PRO_CSYS );
							bool replaceIfExists = String_To_ProBoolean(m.ReplaceIfExists());
							bool deleteSucceeded = false;

							if ( featureExists && replaceIfExists )
							{
								DeleteFeature_IfFailsLogErrorThrowException(
											j.Name(),
											j.Type(),
											p_model,
											m.CoordinateSystemName(), 
											FeatureGeometryType_string(PRO_CSYS),
											"Replace-if-exists delete failed.  A check was made to determine that the datum feature does exist; however, deleting the feature failed.",
											out_ErrorStatus);

								std::stringstream messageString;
								messageString << std::endl << isis_FormatMessage (CAD_INFORMATION, "Replace-if-exists delete succeeded.",
														j.Name(), j.Type(), m.CoordinateSystemName(), "CSYS");
								std::cout << messageString.str();
								std::clog << messageString.str();

								deleteSucceeded = true;

							} // END if ( featureExists && replaceIfExists )
							if ( featureExists && !replaceIfExists )
							{
								std::stringstream messageString;
								messageString << std::endl << isis_FormatMessage (CAD_INFORMATION, "Feature exists and replaceIfExists=False.",
														j.Name(), j.Type(), m.CoordinateSystemName(), "CSYS");
								std::cout << messageString.str();
								std::clog << messageString.str();

							} 

							if ( !featureExists || ( featureExists && replaceIfExists && deleteSucceeded ))
							{
								CADDatumEditor::Origin  origin  = m.Origin_child();
								CADDatumEditor::XVector xVector = m.XVector_child();
								CADDatumEditor::YVector yVector = m.YVector_child();

								std::vector<double> origin_tempVector;
								std::vector<double> xVector_tempVector;
								std::vector<double> yVector_tempVector;

								origin_tempVector.push_back(origin.X());
								origin_tempVector.push_back(origin.Y());
								origin_tempVector.push_back(origin.Z());

								xVector_tempVector.push_back(xVector.X());
								xVector_tempVector.push_back(xVector.Y());
								xVector_tempVector.push_back(xVector.Z());

								yVector_tempVector.push_back(yVector.X());
								yVector_tempVector.push_back(yVector.Y());
								yVector_tempVector.push_back(yVector.Z());

						
								CreateCoordinateSystem( p_model, 
														//isis::ProMdlType_enum(j.Type()),
														referenceCoordinateSystem,
														m.CoordinateSystemName(),
														origin_tempVector,
														xVector_tempVector,
														yVector_tempVector ); 

								std::stringstream messageString;
								messageString << std::endl << 
									isis_FormatMessage (CAD_INFORMATION, "Coordinate system created.",
										j.Name(), j.Type(), m.CoordinateSystemName(), "CSYS");
								std::clog << messageString.str();
								std::cout << messageString.str();
							}

						
						}
					} // END for each(CADDatumEditor::AddDatums l in addDatumsVector )

					////////////////
					// AddDatums
					////////////////

					vector<CADDatumEditor::AddDatums> addDatumsVector = k.AddDatums_kind_children();

					for each(CADDatumEditor::AddDatums l in addDatumsVector )
					{

						vector<CADDatumEditor::AddDatum> addDatumVector = l.AddDatum_kind_children();

						for each(CADDatumEditor::AddDatum m in addDatumVector )
						{
							bool featureExists = FeatureExistsInCreoModel( p_model, m.DatumName(), FeatureGeometryType_enum(m.DatumType()) );
							bool replaceIfExists = String_To_ProBoolean(m.ReplaceIfExists());
							bool deleteSucceeded = false;

							if ( featureExists && replaceIfExists )
							{

								DeleteFeature_IfFailsLogErrorThrowException(
										j.Name(),
										j.Type(),
										p_model,
										m.DatumName(), 
										m.DatumType(),
										"Replace-if-exists delete failed.  A check was made to determine that the datum feature does exist; however, deleting the feature failed.",
										out_ErrorStatus);
		
								std::stringstream messageString;
								messageString << std::endl << isis_FormatMessage (CAD_INFORMATION, "Replace-if-exists delete succeeded.",
														j.Name(), j.Type(),  m.DatumName(), m.DatumType());
								std::cout << messageString.str();
								std::clog << messageString.str();

								deleteSucceeded = true;

							} // END if ( featureExists && replaceIfExists )

							if ( featureExists && !replaceIfExists )
							{
								std::stringstream messageString;
								messageString << std::endl << isis_FormatMessage (CAD_INFORMATION, "Feature exists and replaceIfExists=False.",
														j.Name(), j.Type(),  m.DatumName(), m.DatumType());
								std::cout << messageString.str();
								std::clog << messageString.str();

							} 

							if ( !featureExists || ( featureExists && replaceIfExists && deleteSucceeded ))
							{
								// flipDatumPlaneDirection_temp not supported yet.
								ProBoolean flipDatumPlaneDirection_temp = PRO_B_FALSE;
								if ( std::string( m.FlipDatumPlaneDirection()).size() > 0 )
									flipDatumPlaneDirection_temp = String_To_ProBoolean(m.FlipDatumPlaneDirection());

								AddDatumBasedOnCoordinateSystem( 
											j.Name(), 
											j.Type(), 
											p_model,
											m.CoordinateSystemName(),
											m.CoordinateAlignment(),
											m.DatumName(),
											m.DatumType(),
											flipDatumPlaneDirection_temp,
											//String_To_ProBoolean(m.ReplaceIfExists()),
											out_ErrorStatus);

								std::stringstream messageString;
								messageString << std::endl << 
									isis_FormatMessage (CAD_INFORMATION, "Datum created.",
										j.Name(), j.Type(), m.DatumName(), m.DatumType());
								std::clog << messageString.str();
								std::cout << messageString.str();
							} // END if ( !featureExists || ( featureExists && replaceIfExists && deleteSucceeded ))
						}
					} // END for each(CADDatumEditor::AddDatums l in addDatumsVector )
				 }

				/////////////////////////
				// Save the Creo Model
				/////////////////////////
				isis::isis_ProMdlSave(p_model);
			}  // END for each( CADDatumEditor::CADComponent j in cADComponentsVector )

		}
		
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////


} // End namespace isis