#include "Nastran.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "MiscellaneousFunctions.h"


#pragma warning( disable : 4996 ) // Disable warining about not using the safe char* functions (e.g. sscanf_s, sprintf_s...
namespace isis_CADCommon
{
	std::string DoubleToString(double in_data, std::streamsize in_precision)
	{
		if(abs(in_data) < 1e-19)
			in_data = 0;		
		std::stringstream stream; 
		stream.precision(in_precision);
		stream << in_data;
		if(in_data == (int)in_data)
			stream << ".";
		return stream.str();
	}

	std::ostream& operator<<(std::ostream& stream, const PSolid &myPSolid)
	{
		stream << "PSOLID," << myPSolid.PID << "," << myPSolid.MID;
		return stream;
	}

	std::string GridPointToString(const GridPoint &myGrid)
	{
		std::stringstream stream;
		
		stream << "GRID," << myGrid.ID << "," << myGrid.locationCoordinateSystem_ID
			<< "," << DoubleToString(myGrid.point.x) << "," 
			<< DoubleToString(myGrid.point.y) << "," << DoubleToString(myGrid.point.z)
			<< "," << myGrid.displacementCoordinateSystem_ID;
		return stream.str();
	}

	std::ostream& operator<<(std::ostream& stream, const SolidElement &myElement)
	{
		std::string elementType = "";
		if(myElement.Type == isis_CADCommon::CTETRA)
			elementType = "CTETRA";
		else if(myElement.Type == isis_CADCommon::CPENTA)
			elementType = "CPENTA";
		else //CHEXA
			elementType = "CHEXA";
		stream << elementType;
		stream << "," << myElement.EID << "," << myElement.PID;
		for(size_t i = 0; i < myElement.GID.size(); ++i)
		{
			stream << "," << myElement.GID[i];
			if(i%5 == 0 && i != 0)
				stream << ",\n";
		}
		return stream;
	}
	
	std::ostream& operator<<(std::ostream& stream, const SPC &myConstraint)
	{
		stream << "SPC," << myConstraint.SID << "," << myConstraint.G1
			<< "," << myConstraint.C1 << "," << myConstraint.D1;
		return stream;
	}
	
	std::ostream& operator<<(std::ostream& stream, const Force &myForce)
	{
		stream << "FORCE," << myForce.SID << "," << myForce.GID << ","
			<<  myForce.CID << "," << myForce.F << "," << myForce.N1
			<< "," << myForce.N2 << "," << myForce.N3;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const CoordSystem &myCoord)
	{
		//how to preserve decimal point at end of doubles/floats?
		if(myCoord.coordComment != "")
			stream << myCoord.coordComment << std::endl;

		stream << myCoord.system << "," << myCoord.CID << "," << myCoord.RID
			<< "," << DoubleToString(myCoord.A.x) << "," << DoubleToString(myCoord.A.y)
			<< "," << DoubleToString(myCoord.A.z)	<< "," << DoubleToString(myCoord.B.x)
			<< "," << DoubleToString(myCoord.B.y) << "," << DoubleToString(myCoord.B.z)
			<< ",\n," << DoubleToString(myCoord.C.x) << "," << DoubleToString(myCoord.C.y)
			<< "," << DoubleToString(myCoord.C.z);
		return stream;
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReplaceCommasWithSpace( std::string &in_out_String )
	{
		size_t commaPosition;
		commaPosition = in_out_String.find(",");
		while ( commaPosition != std::string::npos )
		{
			in_out_String.replace(commaPosition,1, " " );
			commaPosition = in_out_String.find(",", commaPosition);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReplaceSpacesWithNull( std::string &in_out_String )
	{
		size_t spacePosition;
		spacePosition = in_out_String.find(" ");
		while ( spacePosition != std::string::npos )
		{
			in_out_String.replace(spacePosition,1, "" );
			spacePosition = in_out_String.find(" ", spacePosition);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReplaceDoubleCommasWithCommaSpaceComma( std::string &in_out_String )
	{
		size_t doubleComma;
		doubleComma = in_out_String.find(",,");
		while ( doubleComma != std::string::npos )
		{
			in_out_String.replace(doubleComma,2, ", ," );
			doubleComma = in_out_String.find(",,", doubleComma);
		}
	}

	void ReplaceCommaSpaceCommaWithDoubleCommas( std::string &in_out_String )
	{
		size_t commaSpaceComma;
		commaSpaceComma = in_out_String.find(", ,");
		while ( commaSpaceComma != std::string::npos )
		{
			in_out_String.replace(commaSpaceComma,3, ",," );
			commaSpaceComma = in_out_String.find(", ,", commaSpaceComma);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//	class NastranDeck
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	NastranDeck::NastranDeck():maxCoordinateID(0) {}


	/////////////
	// Accessors
	/////////////
	std::string& NastranDeck::getName()
	{
		return name;
	}
	std::list<std::string>& NastranDeck::getHeaderComments()
	{
		return headerComments;
	}
	int NastranDeck::getTime()
	{
		return time;
	}
	std::string& NastranDeck::getSolverType()
	{
		return solverType;
	}
	std::list<std::string>& NastranDeck::getExecutiveControl() 
	{
		return executiveControl;
	}
	std::string& NastranDeck::getEndExecutiveControl()
	{
		return endExecutiveControl;
	}
	CaseControl& NastranDeck::getCaseControlData() 
	{
		return caseControlData;
	}
	std::list<std::string>& NastranDeck::getCaseControl()
	{
		return caseControl;
	}
	std::string& NastranDeck::getBeginBulkData()
	{
		return beginBulkData;
	}
	std::list<std::string>& NastranDeck::getBulkString() 
	{
		return bulkString;
	}
	std::string& NastranDeck::getEndBulkData() 
	{
		return endBulkData;
	}
	std::map<int, Material>& NastranDeck::getMaterialData() 
	{
		return materialData;
	} 
	std::map<int, GridPoint>& NastranDeck::getGridPointData() 
	{
		return gridPointData;
	}	
	std::map<int, PSolid>& NastranDeck::getPsolidData() 
	{
		return psolidData;
	}
	std::map<int, SolidElement>& NastranDeck::getElementData() 
	{
		return elementData;
	}
	std::map<int, CoordSystem>& NastranDeck::getCoordSystems() 
	{
		return coordSystems;
	}
	std::multimap<int, SPC>& NastranDeck::getSpcData() 
	{
		return spcData;
	}
	std::multimap<int, Force>& NastranDeck::getForceLoadData()
	{
		return forceLoadData;
	}
	int NastranDeck::getGlobalCord_ID()
	{
		return globalCord_ID;
	}
	int NastranDeck::getGridCord_ID() 
	{
		return gridCord_ID;
	}
	int NastranDeck::getGridDisplacementCord_ID() 
	{
		return gridDisplacementCord_ID;
	}
	std::multimap<int, int>& NastranDeck::getDisplaceCord_Elements() 
	{
		return displaceCord_Elements;
	} 
	std::set<int>& NastranDeck::getDisplaceCord_Elements_KEYS() 
	{
		return displaceCord_Elements_KEYS;
	}
	int NastranDeck::getTetraCount() 
	{
		return tetraCount;
	}
	int NastranDeck::getPentaCount() 
	{
		return pentaCount;
	}
	int NastranDeck::getHexaCount() 
	{
		return hexaCount;
	}
	int NastranDeck::getElementTypeCount() 
	{
		return elementTypeCount;
	}
	int NastranDeck::getMaxCoordinateID() 
	{
		return maxCoordinateID;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::ReadNastranDeck( const std::string &in_InputFileName ) throw (isis::application_exception)
	{
		std::string::size_type pos = in_InputFileName.find(".nas");
		name = in_InputFileName.substr(0, pos);

		enum DeckSection { e_EXECUTIVE , e_CASE, e_BULK };

		DeckSection deckSection = e_EXECUTIVE;

		std::ifstream  deckFile;
		deckFile.open (in_InputFileName.c_str(), std::ios::in );

		if (!deckFile.is_open())
		{
			std::string errorString;
			errorString = "Function - NastranDeck::ReadNastranDeck, could not open file: " + in_InputFileName;
			throw isis::application_exception(errorString.c_str());	
		}

		std::string deckLine;
		std::list<std::string> tempBulk;
		while ( deckFile.good() )
		{

			getline(deckFile, deckLine );
			//std::cout<<deckLine << std::endl;
			switch (  deckSection )
			{
				case e_EXECUTIVE:
					if ( deckLine.find("CEND") != std::string::npos ) 
					{
						ParseExecutiveControl();
						deckSection = e_CASE;
						endExecutiveControl = deckLine;
					}
					else
					{
						executiveControl.push_back(deckLine);
					}

					break;
				case e_CASE:
					if ( deckLine.find("BEGIN BULK") != std::string::npos )
					{
						ParseCaseControl(caseControlData);
						deckSection = e_BULK;
						beginBulkData = deckLine;
					}
					else
					{
						caseControl.push_back(deckLine);
					}
					break;
				case e_BULK:
					if ( deckLine.find("ENDDATA") != std::string::npos )
					{

						ParseBulkData(tempBulk);
						endBulkData = deckLine;
					}
					else
					{
						
						tempBulk.push_back(deckLine);

						if ( (deckLine.find("CORD1") != std::string::npos ) ||
							 (deckLine.find("CORD2") != std::string::npos ))
						{
							ReplaceCommasWithSpace(deckLine);
							char  name[32];
							int	  coordinateID;
							//maxCoordinateID
							sscanf(deckLine.c_str(), "%s %d", name, &coordinateID );
							if ( coordinateID > maxCoordinateID ) maxCoordinateID = coordinateID;
						}
						//populate other data
					}
					/*
					if ( deckLine.find("GRID") != std::string::npos )
					{
						GridPoint gridPoint;
						char  name[32];
						//std::cout << std::endl << deckLine;
						ReplaceCommasWithSpace(deckLine);
						//std::cout << std::endl << deckLine;
						sscanf(deckLine.c_str(), "%s %d %d %lf %lf %lf %d", 
														name, 
														&gridPoint.ID, 
														&gridPoint.locationCoordinateSystem_ID, 
														&gridPoint.point.x,
														&gridPoint.point.y,
														&gridPoint.point.z,
														&gridPoint.displacementCoordinateSystem_ID );
						//std::cout << std::endl << gridPoint;
						gridPoints_map[gridPoint.ID] = gridPoint;					
					}
					*/
					break;
			}
		} // End 	while ( deckFile.good() )
		deckFile.close();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::WriteNastranDeck(const std::string &in_OutputFileName ) throw (isis::application_exception)
	{
		std::ofstream  deckFile;
		deckFile.open (in_OutputFileName.c_str(),std::ios::out | std::ios::trunc  );

		deckFile << *this;

		deckFile.close(); 
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//void NastranDeck::SetConstraintID( int in_ConstraintID) { constraintID = in_ConstraintID; }
	//void NastranDeck::SetLoadID( int in_LoadID) { loadID = in_LoadID; }
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AppendHeaderComment( const std::string &in_Comment )
	{
		headerComments.push_back(in_Comment);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddSubCaseAndLoadStatement(	int in_SubCaseID, 
													int in_ConstraintSetID, 
													int in_LoadStatementID, 
													int in_LoadSetID )
	{
		std::string tempString;

		tempString = "SUBCASE = " + IntegerToString(in_SubCaseID);
		caseControl.push_back( tempString );

		tempString = "   SPC = " + IntegerToString(in_ConstraintSetID);
		caseControl.push_back( tempString );

		tempString = "   LOAD = " + IntegerToString(in_LoadStatementID);
		caseControl.push_back( tempString );

		tempString = "LOAD," + IntegerToString(in_LoadStatementID) + ",1.,1.," + 
							   IntegerToString(in_LoadSetID);
				
		bulkString.insert( bulkString.begin(), tempString );		//bulkData.push_front( tempString );

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddConstraintToDeck(	int in_ConstraintID, int in_GridID, 
											bool in_xDisp_Defined,
											bool in_yDisp_Defined,
											bool in_zDisp_Defined,
											double in_xDisp, 
											double in_yDisp, 
											double in_zDisp )

	{
		//let's keep this as is for now
		std::string prefixString;
		std::string tempString;
		char buffer_1[64];

		prefixString = "SPC," +  IntegerToString(in_ConstraintID) + 
						"," +    IntegerToString(in_GridID);

		if ( in_xDisp_Defined )
		{
			sprintf(buffer_1, "%lf", in_xDisp ); 
			tempString =   prefixString + ",1," +  std::string(buffer_1);
			bulkString.push_back( tempString );
		}

		if ( in_yDisp_Defined )
		{
			sprintf(buffer_1, "%lf", in_yDisp ); 
			tempString =   prefixString + ",2," +  std::string(buffer_1);
			bulkString.push_back( tempString );
		}

		if ( in_zDisp_Defined )
		{
			sprintf(buffer_1, "%lf", in_zDisp ); 
			tempString =   prefixString + ",3," +  std::string(buffer_1);
			bulkString.push_back( tempString );
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddConstraintToDeck(	int in_ConstraintID, 
											int in_GridID, 
											int in_DegreeOfFreedomID, 
											double in_ConstraintValue )
	{
		std::string prefixString;
		std::string tempString;
		char buffer_1[64];
		char buffer_2[64];
		char buffer_3[64];

		prefixString = "SPC," +  std::string(itoa(in_ConstraintID,		 buffer_1,10)) + 
						"," +    std::string(itoa(in_GridID,			 buffer_2,10)) +
						"," +    std::string(itoa(in_DegreeOfFreedomID,  buffer_3,10));

		sprintf(buffer_1, "%lf", in_ConstraintValue ); 
		tempString =   prefixString + "," +  std::string(buffer_1);
		bulkString.push_back( tempString );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string VectorString( double in_Vector_x, double in_Vector_y, double in_Vector_z)
	{
		std::string tempString;
		char buffer_1[64];

		sprintf(buffer_1, "%lf", in_Vector_x ); 
		tempString =  "," + std::string(buffer_1);

		sprintf(buffer_1, "%lf", in_Vector_y ); 
		tempString +=  "," + std::string(buffer_1);

		sprintf(buffer_1, "%lf", in_Vector_z ); 
		tempString +=  "," + std::string(buffer_1);

		return tempString;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ScaleFactorVectorString( double in_ScaleFactor, double in_Vector_x, double in_Vector_y, double in_Vector_z)
	{
		std::string tempString;
		char buffer_1[64];

		sprintf(buffer_1, "%lf", in_ScaleFactor ); 
		tempString =  "," + std::string(buffer_1);

		return tempString + VectorString(in_Vector_x, in_Vector_y, in_Vector_z );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddForceToDeck( int in_LoadID, int in_GridID, int in_CoordinateSystemID, double in_ForceScaleFactor, 
									  double in_Vector_x, double in_Vector_y, double in_Vector_z )
	{
		if(forceLoadData.empty())
			bulkString.push_back("FORCES_PLACEMENT");

		Force newForce = Force();
		newForce.SID = in_LoadID;
		newForce.GID = in_GridID;
		newForce.CID = in_CoordinateSystemID;
		//we have to convert the doubles to strings first
		newForce.F = DoubleToString(in_ForceScaleFactor);
		newForce.N1 = DoubleToString(in_Vector_x);
		newForce.N2 = DoubleToString(in_Vector_y);
		newForce.N3 = DoubleToString(in_Vector_z);

		//Finally, insert this into our forceLoadData object
		forceLoadData.insert(std::make_pair(newForce.SID, newForce));


		/*std::string tempString;


		tempString = "FORCE,"	+ IntegerToString(in_LoadID) +
					 ","		+ IntegerToString(in_GridID) +
					 ","		+ IntegerToString(in_CoordinateSystemID);
		
		tempString +=  ScaleFactorVectorString( in_ForceScaleFactor, in_Vector_x, in_Vector_y, in_Vector_z );
		bulkString.push_back( tempString );*/

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
			// Acceleration		GRAV,459,1,333.,0.,-1.,0.
			// GRAV SID CID G N1 N2 N3
			//		SlD Set identification number.
			//		CID Coordinate system identification number.
			//		G Acceleration vector scale factor.
			//		Ni Acceleration vector components measured in coordinate system CID.
	void NastranDeck::AddAccelerationToDeck( int in_LoadID,  int in_CoordinateSystemID, double in_AccelerationScaleFactor, 
											 double in_Vector_x, double in_Vector_y, double in_Vector_z ) 
	{
		if(forceLoadData.empty())
			bulkString.push_back("FORCES_PLACEMENT");
		std::string tempString;


		tempString = "GRAV,"	+ IntegerToString(in_LoadID) +
					 ","		+ IntegerToString(in_CoordinateSystemID);
		
		tempString +=  ScaleFactorVectorString( in_AccelerationScaleFactor, in_Vector_x, in_Vector_y, in_Vector_z );
		bulkString.push_back( tempString );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// in_TargetKey could be -.1112  
	// corresponding MaterialKey in in_MaterialKey_ComponentID_map could be -.11120
	// Must turn both keys to doubles to do the comparision.
	void SearchForKeyMatch( std::map<std::string, std::string>   &in_MaterialKey_ComponentID_map,
										const std::string					&in_TargetKey,
										bool								&out_KeyFound,
										std::string							&out_KeyMatched )
	{
		out_KeyFound = false;
		out_KeyMatched = "";

		double tolerance = .0000001;

		double targetKey_float = atof(in_TargetKey.c_str());

		for each ( std::pair<std::string, std::string> i in in_MaterialKey_ComponentID_map )
		{
			double mapKey_float = atof(i.first.c_str());			

			if ( abs( mapKey_float - targetKey_float ) < tolerance )
			{
				out_KeyFound = true;
				out_KeyMatched = i.first;
				//std::cout << std::endl << "Match Found, in_TargetKey: " << in_TargetKey << " mapKey: " <<  i.first;
				break;
			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::ReplaceMaterialTokens_ReturnMaterialToComponentID( 
					const std::string								&in_AssemblyComponentID,
					std::map<std::string, std::string>              &in_MaterialKey_ComponentID_map,
					std::map<std::string, double>					&in_ComponentID_PoissonsRatio_map,
					std::map<std::string, std::string>              &out_MaterialID_to_CompnentID_map )
	{
		
		bool aMaterialFound = false;
		for ( std::list<std::string>::iterator i(bulkString.begin()); i != bulkString.end(); ++i )
		{	
			if ( i->find("MAT1") != std::string::npos ) 
			{
				aMaterialFound = true;
				// See "Nastran Linear Static Analysis Guide.pdf"
				char	Mat1[32];	//Mat1[0] = '\0';
				char	MID[32];	//MID[32];
				char	E[32];		//E[32];
				char	G[32];		//G[32];
				char	NU_key[32];  // poisson's ratio key
				char    RHO[32];
				char	A[32];
				char	TREF[32];	TREF[0]		= '\0';  
				char	ST[32];		ST[0]		= '\0';  
				char	MCSID[32];  MCSID[0]	= '\0';  // This is needed becasue sscanf will not populate the last field if it is empty.

				// Read current MAT1 values 
				char buffer[256];
				std::string tempString = *i;
				// Need to replace double commans(i.e. ,,) with space comma space (i.e. , ,).  This is needed becasue scanf will not 
				// scan double commas.
				ReplaceDoubleCommasWithCommaSpaceComma(tempString);

				sscanf(tempString.c_str(), "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^',']", Mat1, MID, E, G, NU_key, RHO, A, TREF, ST, MCSID);

				std::string NU_key_matched;
				bool NU_key_matched_found = false;

				// The NU_key could have been written to the decks as -.1112  where the key in the map would be -.11120
				SearchForKeyMatch( in_MaterialKey_ComponentID_map,
									NU_key,
									NU_key_matched_found,
									NU_key_matched );

				//if ( in_MaterialKey_ComponentID_map.find(NU_key) == in_MaterialKey_ComponentID_map.end() )
				if ( !NU_key_matched_found )
				{
					// Token (e.g. -1111 not found
					std::stringstream errorString;
					errorString <<
					"Function: ReplaceMaterialTokens_ReturnMaterialToComponentID, A material key was not found" << std::endl << 
					"in the <working_dir>/Analysis/…orig.nas deck. This could be caused by a material being" << std::endl <<
					"assigned in Creo Simulate (Creo UI, Applications Simulate).  Normally Creo Simulate would" << std::endl <<
					"not be used at all when performing analysis per this program.  If there is a material" << std::endl <<
					"assigned via Creo Simulate, please un-assign it." << std::endl <<
					"   Nastran deck entry:               " << *i <<  std::endl <<
					"   Nastran deck Poisson's Ratio Key: " << NU_key;
					for each ( std::pair<std::string, std::string> ik in in_MaterialKey_ComponentID_map )
						errorString << std::endl << "   Component Instance ID: " << ik.second << "  Key: " << ik.first;
					throw isis::application_exception(errorString.str().c_str());	
				}

				std::string ComponentID = in_MaterialKey_ComponentID_map[NU_key_matched];
				double NU_actual = in_ComponentID_PoissonsRatio_map[ComponentID];
				out_MaterialID_to_CompnentID_map[std::string(MID)] = ComponentID;

				//std::cout << std::endl << "-----------------> ComponentID: " << ComponentID;
				//std::cout << std::endl << "-----------------> NU_key_matched, Nu_Actual: " <<  std::setprecision (10)<< NU_key_matched << "  "  <<  std::setprecision (10) << NU_actual;

				sprintf(buffer, "%s,%s,%s,%s,%lf,%s,%s,%s,%s,%s", Mat1, MID, E, G, NU_actual, RHO, A, TREF, ST, MCSID);
				//sprintf(buffer, "%s,%s,%s,%s,%f,%s,%s,%s,%s", Mat1, MID, E, G, componentVistorMaterialTokens.materialKey_MaterialDefintion_map[NU].poissonsRatio, RHO, A, TREF, ST, MCSID);
				tempString = std::string(buffer);
				ReplaceSpacesWithNull(tempString);
				//i->empty();
				//i->assign("Material");
				i->assign(tempString);
			}
			else
			{
				// We are assuming that the materials are group togather in the deck.  Once we have read a material
				// and reach a record that does not begin with "MAT1", then there are no other materials.
				if ( aMaterialFound ) break;
			}

		} // End for ( std::list<std::string>::iterator i(bulkData.begin()); i != bulkData.end(); ++i )
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddCylindricalOrSphericalCoordinateSystem( 
							const Point_3D			&in_PointOrgin,
							const Point_3D			&in_PointZAxis,
							const Point_3D			&in_PointClockingPlane,
							int						in_ReferenceCoordinateSystemID,
							e_CoordinateSystemType	in_CoordinateSystemType,
							int						&out_AddedCoordinateID )
	{
		CoordSystem coord;
					++maxCoordinateID;
					if (in_CoordinateSystemType == CYLINDRICAL_COORDINATE_SYSTEM)
			coord.system = "CORD2C";
		else if (in_CoordinateSystemType == SPHERICAL_COORDINATE_SYSTEM)
			coord.system = "CORD2S";
		else // should never happen
			coord.system = "ERROR";

		coord.CID = maxCoordinateID;
		coord.RID = in_ReferenceCoordinateSystemID;
		coord.A.x = in_PointOrgin.x;
		coord.A.y = in_PointOrgin.y;
		coord.A.z = in_PointOrgin.z;
		coord.B.x = in_PointZAxis.x;
		coord.B.y = in_PointZAxis.y;
		coord.B.z = in_PointZAxis.z;
		coord.C.x = in_PointClockingPlane.x;
		coord.C.y = in_PointClockingPlane.y;
		coord.C.z = in_PointClockingPlane.z;
		coord.coordComment = "";
		coordSystems.insert(std::pair<int, CoordSystem>(maxCoordinateID, coord));
		out_AddedCoordinateID = maxCoordinateID;

		//// Insert coordinate system after the last existing coordinate system
		//for (std::list<std::string>::iterator i(bulkString.begin()); i != bulkString.end(); ++i )
		//{
		//	std::string deckLine = *i;
		//	if ( (deckLine.find("CORD1") != std::string::npos ) ||
		//	      (deckLine.find("CORD2") != std::string::npos ))
		//	{
		//			ReplaceCommasWithSpace(deckLine);
		//		char  name[32];
		//		int	  coordinateID;
		//		//maxCoordinateID
		//		sscanf(deckLine.c_str(), "%s %d", name, &coordinateID );
		//		if ( coordinateID >= maxCoordinateID )
		//		{
		//			// Need to move past the two line CORD2C.. line 
		//			std::list<std::string>::iterator j = i;
		//			++j;
		//			if ( j !=  bulkString.end()) ++j;  //This would be a malformed deck if at the end here 
		//			++maxCoordinateID;
		//			char line_1[82];
		//			char line_2[82];

		//			std::string coordinateSystemName = "ERROR";
		//			if (in_CoordinateSystemType == CYLINDRICAL_COORDINATE_SYSTEM)
		//				coordinateSystemName = "CORD2C";
		//			else
		//				if (in_CoordinateSystemType == SPHERICAL_COORDINATE_SYSTEM)
		//					coordinateSystemName = "CORD2S";

		//			sprintf(line_1,	"%s,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf", 
		//				coordinateSystemName.c_str(),
		//				maxCoordinateID,
		//				in_ReferenceCoordinateSystemID,
		//				in_PointOrgin.x, 
		//				in_PointOrgin.y, 
		//				in_PointOrgin.z, 
		//				in_PointZAxis.x, 
		//				in_PointZAxis.y, 
		//				in_PointZAxis.z);

		//			sprintf(line_2,	",%lf,%lf,%lf", 
		//				in_PointClockingPlane.x, 
		//				in_PointClockingPlane.y, 
		//				in_PointClockingPlane.z);

		//			bulkString.insert(j,line_1);
		//			bulkString.insert(j,line_2);

		//			out_AddedCoordinateID = maxCoordinateID;
		//			break;

		//		}
		//	}	
		//}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// GRID ID CP X1 X2 X3 CD PS SEID
	// lD Grid point identification number.
	// CP Identification number of coordinate system in which the location of the grid point is
	// defined.
	// X1, X2, X3 Location of the grid point in coordinate system CP.
	// CD Identification number of coordinate system in which the displacements, degrees of
	// freedom, constraints, and solution vectors are defined at the grid point.
	// PS Permanent single-point constraints associated with the grid point.
	// SEID Superelement identification number.
	void NastranDeck::ModifyGridPointsDisplacementCoordinateSystemID( const std::set<int> &in_GridPointIDs,
															int	in_CoordinateSystemID )
	{

		//iterate through our input set
		for(std::set<int>::iterator iter = in_GridPointIDs.begin();
			iter != in_GridPointIDs.end();
			++iter)
				{
			//for each element, find in grid point map
			//should we check if it exists first?
			gridPointData[*iter].displacementCoordinateSystem_ID = in_CoordinateSystemID;
		}
		
		//for (std::list<std::string>::iterator i(bulkString.begin()); i != bulkString.end(); ++i )
		//{
		//	//if I find a gridPoint
		//	if ( i->find("GRID") != std::string::npos )
		//	{
		//		std::string buldDataLine = *i;
		//		//create a new gridPoint
		//		GridPoint gridPoint;
		//		char  name[32];
		//		//std::cout << std::endl << deckLine;
		//		ReplaceCommasWithSpace(buldDataLine);
		//		//std::cout << std::endl << deckLine;
		//		sscanf(buldDataLine.c_str(), "%s %d %d %lf %lf %lf %d", 
		//										name, 
		//										&gridPoint.ID, 
		//										&gridPoint.locationCoordinateSystem_ID, 
		//										&gridPoint.point.x,
		//										&gridPoint.point.y,
		//										&gridPoint.point.z,
		//										&gridPoint.displacementCoordinateSystem_ID );
		//		
		//		if ( in_GridPointIDs.find(gridPoint.ID) != in_GridPointIDs.end() )
		//		{
		//			
		//			char newBuldDataLine[82];
		//			
		//			sprintf(newBuldDataLine, "%s,%d,%d,%lf,%lf,%lf,%d", 
		//										name, 
		//										gridPoint.ID, 
		//										gridPoint.locationCoordinateSystem_ID, 
		//										gridPoint.point.x,
		//										gridPoint.point.y,
		//										gridPoint.point.z,
		//										in_CoordinateSystemID );
		//			

		//			*i = std::string(newBuldDataLine);
		//			
		//		}
		//	}
		//}

					

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	std::ostream& operator<<(std::ostream &output, const NastranDeck &in_NastranDeck )
	{
		//output << std::endl << "Nastran Deck";

		
		// Header Comments
		for ( std::list<std::string>::const_iterator i(in_NastranDeck.headerComments.begin()); 
			  i != in_NastranDeck.headerComments.end();
			  ++i )
		{	
			output << *i << std::endl;
				
		}

		// Executive Control
		for ( std::list<std::string>::const_iterator i(in_NastranDeck.executiveControl.begin()); 
			  i != in_NastranDeck.executiveControl.end();
			  ++i )
		{	

			output << *i << std::endl;

		}
		output << in_NastranDeck.endExecutiveControl << std::endl;

		// Case Control
		for ( std::list<std::string>::const_iterator i(in_NastranDeck.caseControl.begin()); 
			  i != in_NastranDeck.caseControl.end();
			  ++i )
		{	
			output << *i << std::endl;
		}

		// Bulk Data
		output << in_NastranDeck.beginBulkData << std::endl;
		for ( std::list<std::string>::const_iterator i(in_NastranDeck.bulkString.begin()); 
			  i != in_NastranDeck.bulkString.end();
			  ++i )
		{	
			std::string line = *i;
			
			if(line == "MATERIALS_PLACEMENT")
			{
				//to be replaced later by iterating through
				//materials?
				//DO NOTHING for now
			}
			else if(line == "PSOLIDS_PLACEMENT")
			{
				std::map<int, PSolid>::const_iterator it = in_NastranDeck.psolidData.begin();
				std::map<int, PSolid>::const_iterator endMap = in_NastranDeck.psolidData.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl;
				}
			}
			else if(line == "GRID_POINTS_PLACEMENT")
			{
				std::map<int, GridPoint>::const_iterator it = in_NastranDeck.gridPointData.begin();
				std::map<int, GridPoint>::const_iterator endMap = in_NastranDeck.gridPointData.end();
				for(; it != endMap; ++it)
				{
					output <<  GridPointToString(it->second) << std::endl;
				}
			}
			else if(line == "ELEMENTS_PLACEMENT")
			{
				std::map<int, SolidElement>::const_iterator it = in_NastranDeck.elementData.begin();
				std::map<int, SolidElement>::const_iterator endMap = in_NastranDeck.elementData.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl ;
				}
			}
			else if(line == "CONSTRAINTS_PLACEMENT")
			{
				std::multimap<int, SPC>::const_iterator it = in_NastranDeck.spcData.begin();
				std::multimap<int, SPC>::const_iterator endMap = in_NastranDeck.spcData.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl;
		}
			}
			else if(line == "FORCES_PLACEMENT")
			{
				std::multimap<int, Force>::const_iterator it = in_NastranDeck.forceLoadData.begin();
				std::multimap<int, Force>::const_iterator endMap = in_NastranDeck.forceLoadData.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl;
				}
			}
			else if(line == "COORDS_PLACEMENT")
			{
				std::map<int, CoordSystem>::const_iterator it = in_NastranDeck.coordSystems.begin();
				std::map<int, CoordSystem>::const_iterator endMap = in_NastranDeck.coordSystems.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl;
				}
			}
			else //it doesn't match any of the above
			{
				ReplaceCommaSpaceCommaWithDoubleCommas(line);
				output << line << std::endl;
			}

			
		}
		output << in_NastranDeck.endBulkData;
		return output;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	std::ostream& operator<<(std::ostream &output, const std::map<int, GridPoint> &in_gridPoints_map )
	{
		for ( std::map<int, GridPoint>::const_iterator i(in_gridPoints_map.begin()); i !=in_gridPoints_map.end(); ++i )
		{
			output << i->second;
		}
		return output;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	void NastranDeck::ParseExecutiveControl() throw (isis::application_exception)
	{
		for ( std::list<std::string>::iterator i(executiveControl.begin()); i != executiveControl.end(); ++i )
		{	
			std::string data = *i;
			if (data != "" && data[0] != '$')
				{
					std::vector<std::string> tokens;
					isis_CADCommon::tokenize_strtok(tokens, data, " ");

					if (tokens.empty())
						throw isis::application_exception(std::string("Problem parsing ExecutiveControl statements - Line:" + data).c_str());	

					if (tokens.size() >= 2)
					{
						if (tokens[0] == "SOL")
						solverType = (tokens[1] == "SESTATIC") ? "101" : tokens[1];
						else if (tokens[0] == "TIME")
						time = atoi(tokens[1].c_str());
					}
					else
						std::cout << "WARNING: ExecutiveControl Parsing [" << data << "]" << std::endl;
				}
				
			
		}
	}
	
	void NastranDeck::ParseCaseControl(CaseControl &out_caseCtrl) throw (isis::application_exception)
	{
		bool subcase = 0;	
		std::vector<std::string> subcaseStr;
		//int size = caseControl.size();
		//for ( int i = 0; i < size;)
		std::list<std::string>::const_iterator zi = caseControl.end();
		for (std::list<std::string>::const_iterator yi = caseControl.begin(); yi != zi;)
		{
			std::string data = *yi, card, outputConfig;
			std::vector<std::string> tokens;
			isis_CADCommon::tokenize_strtok(tokens, data, " =");
			if (tokens.size() != 2)
				throw isis::application_exception(std::string("Problem parsing CaseControl statements - Line:" + data).c_str());
			card = tokens[0];
			outputConfig = tokens[1];

			if (card == "SUBCASE")		// start of subcase
			{		
				std::vector<std::pair<std::string, std::string>> sss;
				yi++;
				while (yi != zi)				// not end of caseControl vector
				{
					std::string line = *yi;
					if (line.find("SUBCASE") == std::string::npos)
					{
						std::vector<std::string> tokens_subcase;
						tokenize_strtok(tokens_subcase, line, " =");
						if (tokens_subcase.size() != 2)
							throw isis::application_exception(std::string("Subcase statements - Keyword has no value! -  Line:" + line).c_str());

						sss.push_back(make_pair(tokens_subcase[0], tokens_subcase[1]));
						tokens_subcase.clear();
						yi++;
					}
					else
						break;
				}
				out_caseCtrl.subcases[outputConfig] = sss;
			}
			else
			{	
				tokens.clear();
				tokenize_strtok(tokens, card, "()");
				out_caseCtrl.commands[tokens[0]] = make_pair((tokens.size() > 1 ? tokens[1] : ""), outputConfig);				
				yi++;
			}

		}

	}

	void NastranDeck::ParseBulkData(const std::list<std::string> &in_bulkData)
	{
		//int size = bulkData.size();
		//for (int i = 0; i < size; i++)

		//Create a bunch of boolean values to see if we've visited 
		// certain sections

		std::list<std::string>::const_iterator endBulk = in_bulkData.end(), bulkIter = in_bulkData.begin();
		for (; bulkIter != endBulk ; bulkIter++)
		{
			std::string data = *bulkIter;
			if (data == "")
				bulkString.push_back(data);
			if (data[0] != '$' && data[0] != ',')	// not a comment or extention line
			{
				std::vector<std::string> tokens;
				std::string card = data.substr(0, data.find_first_of(','));
				ReplaceDoubleCommasWithCommaSpaceComma(data);
				if (card == "MAT1")
				{
					if(materialData.empty())
					{
						bulkString.push_back("MATERIALS_PLACEMENT");
					}

					Material material;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() != 9)
					{
						std::cout << "MAT1: Not enough entries, need to be 9 [" << data << "]" << std::endl;
						continue;
					}
					material.name = tokens[0];
					material.MID = atoi(tokens[1].c_str());
					material.Young = tokens[2];
					material.Shear = tokens[3];
					material.Poisson = tokens[4];
					material.Density = tokens[5];
					material.Thermal = tokens[6];
					material.RefTemp = (tokens[7] == " " ? "0": tokens[7]);
					material.Damping = tokens[8];
					materialData[material.MID] = material;
					bulkString.push_back(data); //need this because of the double commas
					//we don't know how original string delimits tokens - or maybe not
					//replace comma space comma with double comma?
				}
				else if (card == "LOAD")
				{
					//Load load;
					//stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					bulkString.push_back(data);

				}
				else if (card == "CORD2R" || card == "CORD2C" || card == "CORD2S")							// 2 line
				{

					bool global = 0, grid = 0, disp = 0;
					std::string coordComment = "";
					if (bulkIter != in_bulkData.begin())
					{
						//Kept from Di's code - looks at the coord comment
						//above and notes its type from the comment above it

						std::list<std::string>::const_iterator xi = bulkIter;
						xi--;
						coordComment = *xi;

						if (coordComment.find("Global Coordinate") != std::string::npos)
							global = 1;
						else if (coordComment.find("grid coordinates") != std::string::npos)
							grid = 1;
						else if (coordComment.find("grid displacement") != std::string::npos)
							disp = 1;
					}

					bulkIter++;
					data += *bulkIter;
					stringTokenize(data.c_str(), tokens, ",");
					if (tokens.size() < 12)
					{
						std::cout<< "CORD2R: <12 entries [" << data << "]" << std::endl;
						continue;
					}

					CoordSystem coord;
					coord.CID = atoi(tokens[1].c_str());
					coord.RID = atoi(tokens[2].c_str());
					coord.A.x = atof(tokens[3].c_str());
					coord.A.y = atof(tokens[4].c_str());
					coord.A.z = atof(tokens[5].c_str());
					coord.B.x = atof(tokens[6].c_str());
					coord.B.y = atof(tokens[7].c_str());
					coord.B.z = atof(tokens[8].c_str());
					coord.C.x = atof(tokens[9].c_str());
					coord.C.y = atof(tokens[10].c_str());
					coord.C.z = atof(tokens[11].c_str());
					coord.system = card;
					coord.coordComment = "";



					if (global)
					{
						globalCord_ID = coord.CID;
						coord.coordComment = coordComment;
						bulkString.pop_back();
					}
					else if (grid)
					{
						gridCord_ID = coord.CID;
						coord.coordComment = coordComment;
						bulkString.pop_back();
					}
					else if (disp)
					{
						gridDisplacementCord_ID = coord.CID;
						coord.coordComment = coordComment;
						bulkString.pop_back();
					}
					//else do nothing
					if(coordSystems.empty())
					{
						bulkString.push_back("COORDS_PLACEMENT");
						
					}
					coordSystems[coord.CID] = coord;
				}
				else if (card == "PSOLID")		
				{
					if(psolidData.empty())
					{
						bulkString.push_back("PSOLIDS_PLACEMENT");
						
					}
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)

					if (tokens.size() < 3)
					{
						std::cout<< "PSOLID: <3 entries [" << data << "]" << std::endl;
						continue;
					}
					PSolid psolid;
					psolid.PID = atoi(tokens[1].c_str());
					psolid.MID = atoi(tokens[2].c_str());
					psolidData[psolid.PID] = psolid;
				}
				else if (card == "GRID")
				{
					if(gridPointData.empty())
					{
						bulkString.push_back("GRID_POINTS_PLACEMENT");
						
					}
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 7)
					{
						std::cout<< "GRID: <7 entries [" << data << "]" << std::endl;
						continue;
					}
					GridPoint grid;
					grid.ID = atoi(tokens[1].c_str());
					grid.locationCoordinateSystem_ID = atoi(tokens[2].c_str());
					grid.point.x = atof(tokens[3].c_str());
					grid.point.y = atof(tokens[4].c_str());
					grid.point.z = atof(tokens[5].c_str());
					grid.displacementCoordinateSystem_ID = atoi(tokens[6].c_str());

					gridPointData[grid.ID] = grid;
					displaceCord_Elements_KEYS.insert(grid.displacementCoordinateSystem_ID);
					displaceCord_Elements.insert(std::make_pair(grid.displacementCoordinateSystem_ID, grid.ID));
				}
				else if (card == "CTETRA")							// 2 line
				{
					if(elementData.empty())
					{
						bulkString.push_back("ELEMENTS_PLACEMENT");
					}
					bulkIter++;
					if (bulkIter != endBulk)
					{
						data += *bulkIter;
						stringTokenize(data.c_str(), tokens, ",");			// tokenize(data)
					}

					if (tokens.size() < 13)
					{
						std::cout << "CTETRA: [ " << data << "]" << std::endl;
						continue;
					}

					SolidElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CTETRA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GID.push_back(atoi(tempGID[i].c_str()));
					}
					elementData.insert(std::make_pair(element.EID, element));
					tetraCount++;
				}
				else if (card == "CPENTA")							// 3 lines
				{
					if(elementData.empty())
					{
						bulkString.push_back("ELEMENTS_PLACEMENT");
					}
					bulkIter++;
					for (int j = 1; j < 3 && bulkIter != endBulk; j++, bulkIter++)
						data += *bulkIter;
			
					stringTokenize(data.c_str(), tokens, ",");			// tokenize(data)

					if (tokens.size() < 18)
					{											
						std::cout << "CPENTA: [ " << data << "]" << std::endl;
						continue;
					}
					SolidElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CPENTA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GID.push_back(atoi(tempGID[i].c_str()));
					}
					elementData.insert(std::make_pair(element.EID, element));
					pentaCount++;
				}
				else if (card == "CHEXA")							// 3 lines
				{
					if(elementData.empty())
					{
						bulkString.push_back("ELEMENTS_PLACEMENT");
					}
					bulkIter++;
					for (int j = 1; j < 3 && bulkIter != endBulk; j++, bulkIter++)
						data += *bulkIter;
		
					stringTokenize(data.c_str(), tokens, ",");			// tokenize(data)

					if (tokens.size() < 23)
					{						
						std::cout << "CHEXA: [ " << data << "]" << std::endl;
						continue;
					}
					
					SolidElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CHEXA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GID.push_back(atoi(tempGID[i].c_str()));
					}
					elementData.insert(std::make_pair(element.EID, element));
					hexaCount++;
				}
				else if (card == "SPC")		
				{
					if(spcData.empty())
					{
						bulkString.push_back("CONSTRAINTS_PLACEMENT");
					}
					stringTokenize(data.c_str(), tokens, ",");		// tokenize(data);
					if (tokens.size() < 5)
					{
						std::cout<< "SPC: Wrong number of entries [" << data << "]" << std::endl;
						continue;
					}

					SPC spc;
					spc.SID = atoi(tokens[1].c_str());
					spc.G1 = tokens[2];
					spc.C1 = tokens[3];
					spc.D1 = tokens[4];
					spcData.insert(std::make_pair(spc.SID, spc));
				}
				else if (card == "FORCE")	
				{
					if(forceLoadData.empty())
					{
						bulkString.push_back("");
					}
					stringTokenize(data.c_str(), tokens, ",");		// tokenize(data);
					if (tokens.size() < 8)
					{
						std::cout<< "FORCE: Wrong number of entries [" << data << "]" << std::endl;
						continue;
					}

					Force force;
					force.SID = atoi(tokens[1].c_str());
					force.GID = atoi(tokens[2].c_str());
					force.CID = atoi(tokens[3].c_str());
					force.F = tokens[4];
					force.N1 = tokens[5];
					force.N2 = tokens[6];
					force.N3 = tokens[7];
					forceLoadData.insert(std::make_pair(force.SID, force));
				}
				else
					bulkString.push_back(data);
					//else just push it back
					//std::cout<< "GetBulkData(): Unsupported card found [" << data << "]" << std::endl;

			} // if
			else if(data[0] == '$')
			{
				//push back comments
				bulkString.push_back(data);
				}
				else
			{
				//do nothing
			}
		} // for

		if (hexaCount > 0)
			elementTypeCount++;
		if (pentaCount > 0)
			elementTypeCount++;
		if (tetraCount > 0)
			elementTypeCount++;
	}


	void NastranDeck::FindElementsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<int> &out_elementIDs)
	{
		if(in_PSolidIDs.empty())
			throw isis::application_exception("NastranDeck::FindELementsFromPSolids,"
			"input vector of PSolid IDs is empty");
		
		//first ensure that our PSolids are in range
		//throw exception if any ID not in range
		for(size_t i = 0; i < in_PSolidIDs.size(); ++i)
		{
			if(in_PSolidIDs[i] > psolidData.size() || in_PSolidIDs[i] < 1)
			{
				throw isis::application_exception("NastranDeck::FindElementsFromPsolids,"   
					"invalid PSolid ID in input vector (at least one ID is out of range)");
			}
		}		
		
		//ensure that our elements container is empty
		out_elementIDs.clear();

		//iterate through the NastranDeck's elementData map
		for(std::map<int, SolidElement>::iterator iter = elementData.begin(); iter!=elementData.end(); ++iter)
		{
			//if an element's PID belongs to one of the psolids in 
			//our input vector, add it to our output vector
			if(std::find(in_PSolidIDs.begin(), in_PSolidIDs.end(), iter->second.PID) != in_PSolidIDs.end())
			{
				out_elementIDs.push_back(iter->first);
			}
			//else, do nothing

		} //end for
		//the output vector has been appropriately populated
	}

	void NastranDeck::FindGridPointsFromElements(std::vector<int> &in_ElementIDs, std::vector<int> &out_gridPoints)
	{
		//throw exception for empty input vector
		if(in_ElementIDs.empty())
			throw isis::application_exception("NastranDeck::FindGridPointsFromElements, input vector of SolidElement IDs is empty");
		//ensure that our Elements are in range
		//throw exception if any ID not in range
		for(size_t i = 0; i < in_ElementIDs.size(); ++i)
		{
			 if(in_ElementIDs[i] > elementData.size() || in_ElementIDs[i] < 1)
			 {
				 throw isis::application_exception("NastranDeck::FindGridPointsFromElements, invalid SolidElement ID in input vector (at least one ID is out of range)");
			 }
		}
		
		//Next, we find all the grid points belonging to each element. We record these in a set
		// to preserve uniqueness, then copy the elements to a vector
		std::set<int> gridPointSet = std::set<int>();

		for(size_t i = 0; i < in_ElementIDs.size(); ++i)
		{
			size_t gridMax = 0;
			//we find the max number of grid points (size of the element's GID vector)
			gridMax = elementData[in_ElementIDs[i]].GID.size();

			for(size_t j = 0; j < gridMax; ++j)
			{
				//we insert the element into our set
				gridPointSet.insert(elementData[in_ElementIDs[i]].GID[j]);
			}
		}
		//Our set is populated (ensuring uniqueness), but we need to populate our vector
		//Use std::copy to copy the values
		std::copy(gridPointSet.begin(), gridPointSet.end(), std::back_inserter(out_gridPoints));

	}

	void NastranDeck::FindGridPointsFromPSolids(std::vector<int> &in_PSolidIDs, 
		std::vector<int> &out_gridPoints)
	{
		//no need to check for exceptions, since the our two method calls do that for us
		std::vector<int> allElements; // records all elements in each PSolid
		FindElementsFromPSolids(in_PSolidIDs, allElements);
		FindGridPointsFromElements(allElements, out_gridPoints);
	}

	void NastranDeck::FindPSolidsFromGridPoint(int in_GridPointID, std::vector<int> &out_PSolidIDs)
	{
		//create a temp vector of ints
		std::vector<int> elementVector = std::vector<int>();
		//populate it
		FindElementsFromGridPoint(in_GridPointID, elementVector);
		//pass it as a parameter
		FindPSolidsFromElements(elementVector, out_PSolidIDs);
	}

	void NastranDeck::FindElementsFromGridPoint(int in_GridPointID, std::vector<int> &out_ElementIDs)
	{
		//first ensure that our element ID vector is empty.
		out_ElementIDs.clear();
		if(gridPointData.find(in_GridPointID) == gridPointData.end())
			throw isis::application_exception("NastranDeck::FindElementsFromGridPoint"
			" - input Grid Point ID does not exist");
		//iterate through elementData, adding to our set
		
		for(std::map<int, SolidElement>::iterator iter = elementData.begin();
			iter != elementData.end(); ++iter)
		{
			//std::cout << "Element ID: " << iter->second.EID << std::endl;
			size_t gridMax = iter->second.GID.size();
			//find its number of grid points
			for(size_t i = 0; i < gridMax; ++i)
			{
				//for each grid point
				//if our grid point is equal to in_GridPointID
				// we put it in the vector
				//std::cout << iter->second.GID[i] << std::endl;
				if(in_GridPointID == iter->second.GID[i])
				{
					
					out_ElementIDs.push_back(iter->first);
				}
				//otherwise, we do nothing
			}
			//advance to next element
		}
	}

	void NastranDeck::FindPSolidsFromElements(std::vector<int> &in_ElementIDs, std::vector<int> &out_PSolidIDs)
	{
		//first ensure that our PSolidID vector is empty
		if(in_ElementIDs.empty())
			throw isis::application_exception("NastranDeck::FindPSolidsFromElements - input SolidElement vector is empty");

		out_PSolidIDs.clear();
		std::set<int> uniquePSolids = std::set<int>();
		for(std::vector<int>::iterator iter = in_ElementIDs.begin();
			iter != in_ElementIDs.end(); ++iter)
		{
			if(elementData.find(*iter) == elementData.end())
				throw isis::application_exception("NastranDeck::FindPSolidsFromElements - invalid SolidElement ID in input vector (at least one ID is out of range)");
			//for each element ID, we look it up and find out which part it belonged to
			uniquePSolids.insert(elementData[*iter].PID);
			//to guarantee uniqueness, we make a set

		}	
		//then copy the values over
		std::copy(uniquePSolids.begin(), uniquePSolids.end(), std::back_inserter(out_PSolidIDs));
	}

	void NastranDeck::SeparateGridPoints(std::vector<int> &in_gridPoints)
	{
		//call SeparateGridPoint on each gridPoint

		for(size_t i = 0; i != in_gridPoints.size();	++i)
		{
			SeparateGridPoint(in_gridPoints[i]);
		}
	}

	void NastranDeck::SeparateGridPoint(int in_gridPoint)
	{
		//if the grid point isn't in range, throw exception
		if(gridPointData.find(in_gridPoint) == gridPointData.end())
			throw isis::application_exception("NastranDeck::SeparateGridPoint - input Grid Point ID does not exist");

		//find the PSolid parts from the grid point
		std::vector<int> PSolidVec = std::vector<int>();
		FindPSolidsFromGridPoint(in_gridPoint, PSolidVec);
		
		int firstPSolid = -1;
		if(!PSolidVec.empty())
			firstPSolid = *(PSolidVec.begin()); //we need to make sure that this won't be null
		//but this allows us to get the ID of first PSolid

		//Iterate through the PSolids
		for(std::vector<int>::iterator PSolidIter = PSolidVec.begin();
			PSolidIter != PSolidVec.end(); 
			++PSolidIter)
		{
			
			if(*PSolidIter != firstPSolid) //we keep elements belonging to the first PSolid the same
			{
				//Invoke the compiler-provided copy constructor
				GridPoint newGridPoint(gridPointData.find(in_gridPoint)->second); 
				//We've copied the data from the previous grid point, but we
				// need to change its ID
				//We look up the last used ID in the gridPointData map

				//Note that maps are ordered by key, and gridPointData is ordered
				// by integers - the IDs of the gridpoints. 

				//This is a bit of hack. Since gridPointData.end() does not correspond
				// to a valid map entry (and points to the past-the-end element), we 
				// need to backtrack to the last element of the container using the --
				// operator. We then find its "first" entry, which is its ID
				// Our newGridPoint now has the highest gridPoint ID
			
				newGridPoint.ID = ((--gridPointData.end())->first)+1;
				//We just need to insert it into the map

				gridPointData[newGridPoint.ID] = newGridPoint;

				//Now that we've added the new grid point, we need to update
				// the references in each of this part's polygons to use this
				// new point instead of the previous one
			
				for(std::map<int, SolidElement>::iterator iter = elementData.begin(); 
					iter != elementData.end(); 
					++iter)
				{
					//if we find a SolidElement with the PID of our PSolid
					if(iter->second.PID == *PSolidIter)
					{
						//iterate through its gridpoints
						for(size_t j = 0; j < iter->second.GID.size(); ++j)
						{
							//If the GID of the current polygon 
							//is equal to our input gridpoint
							if(iter->second.GID[j] == in_gridPoint)
							{
								//replace the gridpoint with our new one
								iter->second.GID[j] = newGridPoint.ID;
							
							} //endif
						} //endfor (GID)
					} //endif
				} //endfor (Polygons)
			} // endif
		} //endfor (PSolidSet)
	}


	void NastranDeck::JoinGridPoints(std::vector<int> &in_gridPoints)
	{
		if(in_gridPoints.empty())
			throw isis::application_exception("NastranDeck::JoinGridPoints =- input GridPoint vector is empty");
		for(size_t i = 0; i < in_gridPoints.size(); ++i)
		{
			//only call the function is in_gridPoints corresponds
			// to a valid existing grid point in the map
			//This is important because JoinGridPoint has the potential
			// to delete some of the grid points that we would 
			// want to join (and if we do, then that point 
			// has been joined)
			if(gridPointData.find(in_gridPoints[i]) != gridPointData.end())
				JoinGridPoint(in_gridPoints[i]);
		}
	}

	void NastranDeck::JoinGridPoint(int in_gridPoint)
	{
		//We're given a grid point ID as input
		// find all grid points with coincicdent coordinates
		
		//make a data structure to contain them
		std::vector<int> equalGrids = std::vector<int>();
		for(std::map<int, GridPoint>::iterator iter = gridPointData.begin();
			iter != gridPointData.end(); ++iter)
		{
			if(PointsAreEqual_3D(iter->second.point, gridPointData[in_gridPoint].point) && iter->first != in_gridPoint)
			{
				//add it to equalGrids
				equalGrids.push_back(iter->first);
			}
		}
		if(equalGrids.empty())
			std::clog << "The gridpoint " << in_gridPoint << " does not have any coincident points. Cannot join." << std::endl;
		// Now we have a list of all points equal to our input point
		
		//We want to join these points - which means we select one point to stay
		//and delete the rest - change their references

		//For each in equalGrids, delete it from gridPointData (unless it's our input param)
		std::map<int, GridPoint>::iterator iter = gridPointData.begin();
		while(iter != gridPointData.end())
		{
			//erase if in equalGrids and not the input
			if(std::find(equalGrids.begin(), equalGrids.end(), iter->first) != equalGrids.end())
			{
				//first increment the iterator, then
				// erase the previous copy
				gridPointData.erase(iter++);
			}
			else
			{
				++iter;
			}
		}

		//we've now deleted all grid points but the one for the input parameter
		//we now need to reassign the deleted gridpoints in the elements to point
		// to that parameter. O(n^3) efficiency
		for(std::map<int, SolidElement>::iterator iter = elementData.begin();
			iter != elementData.end(); ++iter)
		{
			//find out if the element at iter has a grid point in equalGrids
			size_t maxSize = iter->second.GID.size();

			//iterate through its grid IDs
			for(size_t i = 0; i < maxSize; ++i)
			{
				//iterate through equalGrids to see if any of them 
				// are equal to our grid ID
				for(size_t j = 0; j < equalGrids.size(); ++j)
				{
					if(equalGrids[j] == iter->second.GID[i])
					{
						/*std::cout << "CHANGED: "  << iter->second.EID <<  " - " 
							<< iter->second.GID[i] << " to " <<  in_gridPoint  << std::endl;*/
						//replace the former element with the input param
						iter->second.GID[i] = in_gridPoint;
						
					}
				}
			}
		}
	}

	/******************************************************************
					Helper sub-parsing functions
	/******************************************************************/
	void NastranDeck::ParseSubcase(std::vector<std::string> &subcaseStr, 
											SubcaseStatements &subcaseOptions) 
											throw (isis::application_exception)
	{
		size_t size = subcaseStr.size();
		for (int i = 0; i < size; i++)
		{
			std::vector<std::string> tokens;
			tokenize_strtok(tokens, subcaseStr[i], " =");
			if (tokens.size() != 2)
				throw isis::application_exception(std::string("Subcase statements - Keyword has no value! -  Line:" + subcaseStr[i]).c_str());
			subcaseOptions[tokens[0]] = tokens[1];
			tokens.clear();
		}
	}

	//From Ziyi's functions
	double NastranDeck::DistanceBetweenPoints_3D( const Point_3D &in_Point_1,
											 const Point_3D &in_Point_2,
											 double in_Precision)
											 throw (isis::application_exception)
	{
		if( PointsAreEqual_3D(in_Point_1, in_Point_2, in_Precision))
		{
			std::stringstream errorString;
			errorString << "NastranDeck::DistanceBetweenPoints_3D, two input points are too close";
			throw isis::application_exception(errorString.str().c_str());
		}

		return std::sqrt((in_Point_1.x - in_Point_2.x) * (in_Point_1.x - in_Point_2.x)
				  + (in_Point_1.y - in_Point_2.y) * (in_Point_1.y - in_Point_2.y)
				  + (in_Point_1.z - in_Point_2.z) * (in_Point_1.z - in_Point_2.z));
	}


	Point_3D NastranDeck::GetVector_3D( const Point_3D &v1,
							const Point_3D &v2,
							const Point_3D &v3 )
	{
		Point_3D vec1;
		vec1.x = v2.x - v1.x;
		vec1.y = v2.y - v1.y;
		vec1.z = v2.z - v1.z;

		Point_3D vec2;
		vec2.x = v3.x - v1.x;
		vec2.y = v3.y - v1.y;
		vec2.z = v3.z - v1.z;

		Point_3D vec;
		vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
		vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
		vec.z = vec1.x * vec2.y - vec1.y * vec2.x;

		double d = sqrt (vec.x * vec.x + vec.y  * vec.y  + vec.z * vec.z);
		vec.x = vec.x / d; 
		vec.y = vec.y / d; 
		vec.z = vec.z / d;

		return vec;
	}

	double NastranDeck::FindCircumsphereRadius( const std::vector<Point_3D> &p,
                               const double *edge )
	{
		double a = DistanceBetweenPoints_3D(p[0], p[1]) / 2.0;

		Point_3D v1 = GetVector_3D(p[3], p[1], p[0]);

		Point_3D v2 = GetVector_3D(p[0], p[1], p[2]);

		double Angle_a = 3.1415926535 - acos(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);

		double Angle1 = acos((edge[2] * edge[2] + edge[4] * edge[4] - edge[0] * edge[0])
							  / (2.0 * edge[2] * edge[4]));
		double Angle2 = acos((edge[1] * edge[1] + edge[3] * edge[3] - edge[0] * edge[0])
							  / (2.0 * edge[1] * edge[3]));

		return a / sin(Angle_a) * sqrt(pow(1.0 / (sin(Angle1) * sin(Angle2)), 2) -
									   pow(1.0 / (tan(Angle1) * tan(Angle2)) + cos(Angle_a), 2));

	}


	void NastranDeck::GetMaxAndMinAngle( const double &in_side_1,
							const double &in_side_2,
							const double &in_side_3,
							Triangle &result )
	{
		double x(in_side_1), y(in_side_2), z(in_side_3), t;

		if(x > y) t = x, x = y, y = t;
		if(y > z) t = y, y = z, z = t;
		if(x > y) t = x, x = y, y = t;

		result.maxAngle = acos((x * x + y * y - z * z) / (2.0 * x * y));
		result.minAngle = acos((y * y + z * z - x * x) / (2.0 * y * z));
	}

	void NastranDeck::ComputeTetraElementDistortionMetrics(const std::vector<Point_3D> &in_Points,
											  ElementDistortionMetrics &out_ElementDistortionMetrics)
	{
		int n(0);
		double Longest_Line(-1.0), Shortest_Line(10e6), edge[6];

		// Find the ratio: Longest_Line/Shortest_Line
		for(int i = 0; i < 3; ++i)
		{
			for(int j = i + 1; j < 4; ++j, ++n)
			{
				edge[n] = DistanceBetweenPoints_3D(in_Points[i], in_Points[j]);
				if(edge[n] > Longest_Line) Longest_Line = edge[n];
				if(edge[n] < Shortest_Line) Shortest_Line = edge[n];
			}
		}
		out_ElementDistortionMetrics.minimumAspectRation = Longest_Line/Shortest_Line;

		// Find max and min angles, value's range is (0, 180)
		Triangle angles, temp;

		GetMaxAndMinAngle(edge[0], edge[2], edge[4], angles);
		GetMaxAndMinAngle(edge[3], edge[4], edge[5], temp);
		if(temp.maxAngle > angles.maxAngle) angles.maxAngle = temp.maxAngle;
		if(temp.minAngle < angles.minAngle) angles.minAngle = temp.minAngle;

		GetMaxAndMinAngle(edge[1], edge[5], edge[2], temp);
		if(temp.maxAngle > angles.maxAngle) angles.maxAngle = temp.maxAngle;
		if(temp.minAngle < angles.minAngle) angles.minAngle = temp.minAngle;

		GetMaxAndMinAngle(edge[1], edge[0], edge[3], temp);
		if(temp.maxAngle > angles.maxAngle) angles.maxAngle = temp.maxAngle;
		if(temp.minAngle < angles.minAngle) angles.minAngle = temp.minAngle;

		out_ElementDistortionMetrics.maximumAngle = angles.maxAngle * 57.29578;
		out_ElementDistortionMetrics.minimumAngle = angles.minAngle * 57.29578;

		// Find the ratio: VOLUME OF TETRAHEDRON DIVIDED BY THE VOLUME OF EQUILATERAL
		//                 TETRAHEDRON WITH SAME CIRCUMSPHERE RADIUS
		double a = in_Points[0].x - in_Points[3].x;
		double b = in_Points[1].x - in_Points[3].x;
		double c = in_Points[2].x - in_Points[3].x;
		double d = in_Points[0].y - in_Points[3].y;
		double e = in_Points[1].y - in_Points[3].y;
		double f = in_Points[2].y - in_Points[3].y;
		double g = in_Points[0].z - in_Points[3].z;
		double h = in_Points[1].z - in_Points[3].z;
		double i = in_Points[2].z - in_Points[3].z;
		double Volume_TETRAHEDRON = 1.0 / 6.0 * std::abs(a * e * i +
														 b * f * g +
														 c * d * h -
														 c * e * g -
														 b * d * i -
														 a * f * h);

		double CIRCUMSPHERE_RADIUS = FindCircumsphereRadius(in_Points, edge);
		double Volume_EQUILATERAL_TETRAHEDRON = (8.0 * sqrt(3.0) * pow(CIRCUMSPHERE_RADIUS, 3)) / 27.0;
		out_ElementDistortionMetrics.tetrahedralQualityMeasure = Volume_TETRAHEDRON / Volume_EQUILATERAL_TETRAHEDRON;
	}

} // End namespace isis