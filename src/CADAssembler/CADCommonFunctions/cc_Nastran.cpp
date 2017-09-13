#include "cc_Nastran.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "cc_MiscellaneousFunctions.h"
#include <boost/algorithm/string.hpp>


#pragma warning( disable : 4996 ) // Disable warining about not using the safe char* functions (e.g. sscanf_s, sprintf_s...
namespace isis_CADCommon
{
	const std::string c_PRESSURES_PLACEMENT("PRESSURES_PLACEMENT");
	const std::string c_ACCELERATIONS_PLACEMENT("ACCELERATIONS_PLACEMENT");
		
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

	std::string IDToString_0Null( int in_Int)
	{
		if ( in_Int == 0 )
		{
			return "";
		}
		else
		{
			char tempBuffer[64];
			itoa(in_Int, tempBuffer, 10 );
			return std::string( tempBuffer);
		}
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

	std::ostream& operator<<(std::ostream& stream, const FEAElement &myElement)
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
		for(size_t i = 0; i < myElement.GIDs.size(); ++i)
		{
			stream << "," << myElement.GIDs[i];
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

	std::ostream& operator<<(std::ostream& stream, const MAT4 &in_Material_MAT4)
	{

		std::stringstream temp;

		temp << "MAT4," << IDToString_0Null(in_Material_MAT4.MID)		<< "," <<
		in_Material_MAT4.K			<< "," <<
		in_Material_MAT4.CP			<< "," <<
		in_Material_MAT4.p			<< "," <<
		in_Material_MAT4.H			<< "," <<
		in_Material_MAT4.m			<< "," <<
		in_Material_MAT4.HGEN		<< "," <<
		in_Material_MAT4.REFENTH	<< "," <<
		in_Material_MAT4.TCH		<< "," <<
		in_Material_MAT4.TDELTA		<< "," <<
		in_Material_MAT4.QLAT;

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}


	std::ostream& operator<<(std::ostream& stream, const TEMP &in_TEMP)
	{
		std::stringstream temp;

		temp << "TEMP," << IDToString_0Null(in_TEMP.SID)	<< "," <<
		IDToString_0Null(in_TEMP.G1)						<< "," <<
		in_TEMP.T1											<< "," <<
		IDToString_0Null(in_TEMP.G2)						<< "," <<
		in_TEMP.T2											<< "," <<
		IDToString_0Null(in_TEMP.G3)						<< "," <<
		in_TEMP.T3;

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const TEMPD &in_TEMPD)
	{
		std::stringstream temp;

		temp << "TEMPD," << 
		IDToString_0Null(in_TEMPD.SID)		<< "," <<
		in_TEMPD.T;	
		//in_TEMPD.SID2		<< "," <<
		//in_TEMPD.T2			<< "," <<
		//in_TEMPD.SID3		<< "," <<
		//in_TEMPD.T3			<< "," <<
		//in_TEMPD.SID4		<< "," <<
		//in_TEMPD.T4;

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const SPOINT &in_SPOINT)
	{
		std::stringstream temp;

		temp << "SPOINT," << 
		IDToString_0Null(in_SPOINT.ID);	//<< "," <<
	//	in_SPOINT.ID2	<< "," <<
	//	in_SPOINT.ID3	<< "," <<
	//	in_SPOINT.ID4	<< "," <<
	//	in_SPOINT.ID5	<< "," <<
	//	in_SPOINT.ID6	<< "," <<
	//	in_SPOINT.ID7	<< "," <<
	//	in_SPOINT.ID8;

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const PCONV &in_PCONV)
	{
		std::stringstream temp;

		temp << "PCONV," << 
		IDToString_0Null(in_PCONV.PCONID)	<< "," <<
		IDToString_0Null(in_PCONV.MID)	<< "," <<
		in_PCONV.FORM	<< "," <<
		in_PCONV.EXPF;

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const CONV &in_CONV)
	{
		std::stringstream temp;

		temp << "CONV," << 
		IDToString_0Null(in_CONV.EID)		<< "," <<
		IDToString_0Null(in_CONV.PCONID)	<< "," <<
		in_CONV.FLMND						<< "," <<
		in_CONV.CNTRLND						<< "," <<
		IDToString_0Null(in_CONV.TA1)		<< "," <<
		IDToString_0Null(in_CONV.TA2)		<< "," <<
		IDToString_0Null(in_CONV.TA3)		<< "," <<
		IDToString_0Null(in_CONV.TA4)		<< "," <<
		IDToString_0Null(in_CONV.TA5)		<< "," <<
		IDToString_0Null(in_CONV.TA6)		<< "," <<
		IDToString_0Null(in_CONV.TA7)		<< "," <<
		IDToString_0Null(in_CONV.TA8);

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	
	std::ostream& operator<<(std::ostream& stream, const CHBDYG &in_CHBDYG)
	{
		std::stringstream temp1;

		temp1 << "CHBDYG," << 
		IDToString_0Null(in_CHBDYG.EID)		<< ",," <<  // before Type is an empty field.
		in_CHBDYG.TYPE						<< "," <<
		in_CHBDYG.IVIEWF					<< "," <<
		in_CHBDYG.IVIEWB					<< "," <<
		in_CHBDYG.RADMIDF					<< "," <<
		in_CHBDYG.RADMIDB;

		std::string s1 = temp1.str();
		s1.erase(s1.find_last_not_of(",") + 1);

		std::stringstream temp2;

		temp2 << "," <<
		IDToString_0Null(in_CHBDYG.G1)		<< "," <<
		IDToString_0Null(in_CHBDYG.G2)		<< "," <<
		IDToString_0Null(in_CHBDYG.G3)		<< "," <<
		IDToString_0Null(in_CHBDYG.G4)		<< "," <<
		IDToString_0Null(in_CHBDYG.G5)		<< "," <<
		IDToString_0Null(in_CHBDYG.G6)		<< "," <<
		IDToString_0Null(in_CHBDYG.G7)		<< "," <<
		IDToString_0Null(in_CHBDYG.G8);

		// Delete trailing commas
		std::string s2 = temp2.str();
		s2.erase(s2.find_last_not_of(",") + 1);

		if ( s2.size() > 0 )
			stream << s1 << std::endl << s2;
		else
			stream << s1;

		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const QBDY3 &in_QBDY3)
	{
		std::stringstream temp1;

		temp1 << "QBDY3," << 
		IDToString_0Null(in_QBDY3.SID)	<< "," <<
		in_QBDY3.Q0		<< "," <<
		in_QBDY3.CNTRLND		<< "," <<
		IDToString_0Null(in_QBDY3.EID1)		<< "," <<
		IDToString_0Null(in_QBDY3.EID2)		<< "," <<
		IDToString_0Null(in_QBDY3.EID3)		<< "," <<
		IDToString_0Null(in_QBDY3.EID4)		<< "," <<
		IDToString_0Null(in_QBDY3.EID5);


		std::stringstream temp2;
		temp2 << "," <<
		IDToString_0Null(in_QBDY3.EID6)		<< "," <<
		IDToString_0Null(in_QBDY3.EID7)		<< "," <<
		IDToString_0Null(in_QBDY3.EID8);

		// Delete trailing commas
		std::string s1 = temp1.str();
		s1.erase(s1.find_last_not_of(",") + 1);

		std::string s2 = temp2.str();
		s2.erase(s2.find_last_not_of(",") + 1);

		if ( s2.size() > 0 )
			stream << s1 << std::endl << s2;
		else
			stream << s1;
		return stream;
	}


	std::ostream& operator<<(std::ostream& stream, const QVOL &in_QVOL)
	{
		std::stringstream temp;

		temp << "QVOL," << 
		IDToString_0Null(in_QVOL.SID)	<< "," <<
		in_QVOL.QVOL_Value		<< "," <<
		in_QVOL.CNTRLND		<< "," <<
		IDToString_0Null(in_QVOL.EID1)		<< "," <<
		IDToString_0Null(in_QVOL.EID2)		<< "," <<
		IDToString_0Null(in_QVOL.EID3)		<< "," <<
		IDToString_0Null(in_QVOL.EID4)		<< "," <<
		IDToString_0Null(in_QVOL.EID5);

		// Delete trailing commas
		std::string s = temp.str();
		s.erase(s.find_last_not_of(",") + 1);

		stream << s;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const Force &myForce)
	{
		stream << "FORCE," << myForce.SID << "," << myForce.GID << ","
			<<  myForce.CID << "," << myForce.F << "," << myForce.N1
			<< "," << myForce.N2 << "," << myForce.N3;
		return stream;
	}

	
	std::ostream& operator<<(std::ostream& stream, const Acceleration &myAcceleration)
	{
		stream	<< "GRAV," << myAcceleration.SID << "," << myAcceleration.CID << "," << myAcceleration.A << ","
				<< myAcceleration.N1 << "," << myAcceleration.N2 << "," << myAcceleration.N3;
		return stream;
	}
	

	std::ostream& operator<<(std::ostream& stream, const Pressure &myPressure)
	{
		stream << "PLOAD4," << myPressure.SID << "," << myPressure.EID << ","
			<<  myPressure.P1 << "," << myPressure.P2 << "," << myPressure.P3
			<< "," << myPressure.P4 << "," << myPressure.G1 <<   "," << myPressure.G3_or_G4;
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
	void FEAElement::validateElementType_and_NumberGIDs( const std::string &in_CallingFunctionName) const throw (isis::application_exception)
	{
		if ( Type != CHEXA &&  Type != CPENTA && Type != CTETRA && Type != CQUAD && Type != CTRIA3 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:    " << in_CallingFunctionName << std::endl <<
						"ElementType: "	<<  ElementType_ToString(Type) <<  std::endl <<
						"Error:       " << "Only CHEXA, CPENTA, CTETRA, CQUAD, and CTRIA3 currently supported.";
			throw isis::application_exception(errorString.str()); 
		}
			
		switch ( Type )
		{
			case CHEXA:
				if ( GIDs.size() < 8 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  " << in_CallingFunctionName << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementNumberGridPoints: " << GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 8 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;
			case CPENTA:
				if ( GIDs.size() < 6 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  " << in_CallingFunctionName << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementNumberGridPoints: " << GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 6 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;

			case CTETRA:
				if (GIDs.size() < 4 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				 " << in_CallingFunctionName << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementNumberGridPoints: " << GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 4 grid points.";
					throw isis::application_exception(errorString.str()); 
				}

				break;
			case CQUAD:
				if (GIDs.size() < 4 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  " << in_CallingFunctionName << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementNumberGridPoints: " << GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 4 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;
			case CTRIA3:
				if (GIDs.size() < 3 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  " << in_CallingFunctionName << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementNumberGridPoints: " << GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 3 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
		} // END Switch

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int FEAElement::getElementMinimumNumberOfGIDS() const
	{
		if ( Type != CHEXA &&  Type != CPENTA && Type != CTETRA && Type != CQUAD && Type && CTRIA3 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:    " << __FUNCTION__ << std::endl <<
						"ElementType: "	<<  ElementType_ToString(Type) <<  std::endl <<
						"Error:       " << "Only CHEXA, CPENTA, CTETRA, CQUAD, and CTRIA3 currently supported.";
			throw isis::application_exception(errorString.str()); 
		}

		int minimumNumberOfGIDS = 0;
		switch ( Type )
		{
			case CHEXA:
				minimumNumberOfGIDS = 8;
				break;
			case CPENTA:
				minimumNumberOfGIDS = 6;
				break;
			case CTETRA:
				minimumNumberOfGIDS = 4;
				break;
			case CQUAD:
				minimumNumberOfGIDS = 4;
				break;
			case CTRIA3:
				minimumNumberOfGIDS = 3;
		} // END Switch

		return minimumNumberOfGIDS;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int FEAElement::getSurfaceMinimumNumberOfGIDS() const
	{
		//if ( Type != CHEXA &&  Type != CPENTA && Type != CTETRA && Type != CQUAD && Type && CTRIA3 )
		// CPENTA not supported because some surfaces have 3 nodes and other surfaces have 4 nodes.
		if ( Type != CHEXA &&  Type != CTETRA && Type != CQUAD && Type && CTRIA3 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:    " << __FUNCTION__ << std::endl <<
						"ElementType: "	<<  ElementType_ToString(Type) <<  std::endl <<
						//"Error:       " << "Only CHEXA, CPENTA, CTETRA, CQUAD, and CTRIA3 currently supported.";
						"Error:       " << "Only CHEXA, CTETRA, CQUAD, and CTRIA3 currently supported.";
			throw isis::application_exception(errorString.str()); 
		}

		int minimumNumberOfGIDS = 0;
		switch ( Type )
		{
			case CHEXA:
				minimumNumberOfGIDS = 4;
				break;
			case CPENTA:
				minimumNumberOfGIDS = 4;  // 3 surface have a minimum of 4 nodes, and 2 Surfaces have a minimum of 3 
				break;
			case CTETRA:
				minimumNumberOfGIDS = 3;
				break;
			case CQUAD:
				minimumNumberOfGIDS = 4;
				break;
			case CTRIA3:
				minimumNumberOfGIDS = 3;
		} // END Switch

		return minimumNumberOfGIDS;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FEAElement::getElementCornerPoints ( std::vector<int> &out_ElementCornerPoints ) const
	{
		validateElementType_and_NumberGIDs(__FUNCTION__);

		out_ElementCornerPoints.clear();

		for ( int i = 0; i < getElementMinimumNumberOfGIDS(); ++ i )
		{
			out_ElementCornerPoints.push_back(GIDs[i]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Only volume of CTETRA currently supported
	double FEAElement::getVolume( const std::map<int, GridPoint> &in_GridPointData ) const throw (isis::application_exception)
	{
		// Note - this function should be expanded to support the other (e.g. CHEXA, and CPENTA) element types.
		std::stringstream errorString;
		std::vector<Point_3D> cornerPoints;

		double volume = 0.0;

		switch ( Type )
		{
			case CHEXA:
				errorString <<  "Error, Function: " << __FUNCTION__ ", volume computation is currently not supported for CHEXA elements. ";
				throw isis::application_exception(errorString.str()); 		
				break;
			case CPENTA:
				errorString <<  "Error, Function: " << __FUNCTION__ ", volume computation is currently not supported for CPENTA elements. ";
				throw isis::application_exception(errorString.str()); 		
				break;
			case CTETRA:
				if ( GIDs.size() < 4 )
				{
					errorString <<  "Error, Function: " << __FUNCTION__ ", Insufficient points for computing the volume.  There must be at least four points.  " << std::endl <<
						"Number of points: " << GIDs.size();
					throw isis::application_exception(errorString.str()); 	
				}

				for ( int i = 0; i < 4; ++i )
				{
					std::map<int, GridPoint>::const_iterator grid_itr = in_GridPointData.find(GIDs[i] );

					if ( grid_itr == in_GridPointData.end() )
					{
						errorString <<  "Error, Function: " << __FUNCTION__ ", grid point for tetra element not found. Grid point ID: " << GIDs[0];
						throw isis::application_exception(errorString.str()); 	
					}
					cornerPoints.push_back(grid_itr->second.point);
				}
				volume =  VolumeOfTetrahedron( cornerPoints );
				//std::cout << std::endl << "volume =  VolumeOfTetrahedron( cornerPoints ): " << volume;
				break;
			default:
				errorString <<  "Error, Function: " << __FUNCTION__ ", received a type other than CTETRA. Volume computation is currently only supported for CTETRA elements. " << std::endl <<
						    "Volume computations for elements such as CHEXA, and CPENTA are not supported.";
				throw isis::application_exception(errorString.str()); 
		}
		return volume;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FEAElement::getCornerPointsForEachSurface( std::map<int, std::vector<int>> &out_SurfaceID_to_CornerPoints_map ) const
	{

		validateElementType_and_NumberGIDs(__FUNCTION__);

		int numberSurfaceCornerPoints = 4;  // This will be 3 or 4
		int totalCornerPoints = 1000;  // Make large, so it will have to be reset or otherwise caught by the error handler.
		std::stringstream errorString;

		std::vector<int> cornerPoints(32);
		switch ( Type )
		{
			case CHEXA:
				// See Nastran documentation for Tetra CHEXA point numbering scheme
				// Faces by Grid Point Numbering  
				// Face		Grid Points				GIDs Vector Numbering
				//			Nastran Numbering
				//	1		1 2 3 4					0 1 2 3					
				//	2		1 2 6 5				`	0 1 5 4
				//	3		1 5 8 4					0 4 7 3
				//	4		2 3 7 6					1 2 6 5
				//  5       3 4 8 7                 2 3 7 6
				//  6		5 6 7 8					4 5 6 7

				totalCornerPoints = 8;
				numberSurfaceCornerPoints = 4;
				if ( GIDs.size() < 	totalCornerPoints  )
				{				
					errorString <<
						"Function - " + std::string(__FUNCTION__) +
						" Insufficient grid points for a " << ElementType_ToString(Type) << " element.  Number of required grid points: " <<  totalCornerPoints << ".  Number of found grid points:  GIDs.size()";
					throw isis::application_exception(errorString.str().c_str());
				}

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[2];
				cornerPoints[3] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[1] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[5];
				cornerPoints[3] = GIDs[4];
				out_SurfaceID_to_CornerPoints_map[2] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[4];
				cornerPoints[2] = GIDs[7];
				cornerPoints[3] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[3] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[1];
				cornerPoints[1] = GIDs[2];
				cornerPoints[2] = GIDs[6];
				cornerPoints[3] = GIDs[5];
				out_SurfaceID_to_CornerPoints_map[4] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[2];
				cornerPoints[1] = GIDs[3];
				cornerPoints[2] = GIDs[7];
				cornerPoints[3] = GIDs[6];
				out_SurfaceID_to_CornerPoints_map[5] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[4];
				cornerPoints[1] = GIDs[5];
				cornerPoints[2] = GIDs[6];
				cornerPoints[3] = GIDs[7];
				out_SurfaceID_to_CornerPoints_map[6] =  cornerPoints;

				break;
			case CPENTA:
				// Surface could have three corner points or four points
				// See Nastran documentation for Tetra CPENTA point numbering scheme
				// Faces by Grid Point Numbering  
				// Face		Grid Points				GIDs Vector Numbering
				//			Nastran Numbering
				//	1		1 2 3 					0 1 2 					
				//	2		4 5 6				`	3 4 5
				//	3		1 2 5 4					0 1 4 3
				//	4		2 3 6 5				    1 2 5 4
				//  5       1 3 6 4                 0 2 5 3

				totalCornerPoints = 6;
				numberSurfaceCornerPoints = 4;  // two surfaces have only three corner points
				if ( GIDs.size() < 	totalCornerPoints  )
				{				
					errorString <<
						"Function - " + std::string(__FUNCTION__) +
						" Insufficient grid points for a " << ElementType_ToString(Type) << " element.  Number of required grid points: " <<  totalCornerPoints << ".  Number of found grid points:  GIDs.size()";
					throw isis::application_exception(errorString.str().c_str());
				}

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[2];
				//cornerPoints[3] = GIDs[3];
				//																							This surface has 3 points
				out_SurfaceID_to_CornerPoints_map[1] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints-1]);

				cornerPoints[0] = GIDs[3];
				cornerPoints[1] = GIDs[3];
				cornerPoints[2] = GIDs[5];
				//cornerPoints[3] = GIDs[4];
				//																							This surface has 3 points
				out_SurfaceID_to_CornerPoints_map[2] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints - 1]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[4];
				cornerPoints[3] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[3] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[1];
				cornerPoints[1] = GIDs[2];
				cornerPoints[2] = GIDs[5];
				cornerPoints[3] = GIDs[4];
				out_SurfaceID_to_CornerPoints_map[4] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[2];
				cornerPoints[2] = GIDs[5];
				cornerPoints[3] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[5] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				break;
			case CTETRA:
				// See Nastran documentation for Tetra Grid point numbering scheme
				//
				// Faces by Grid Point Numbering  
				// Face		Grid Points				GIDs Vector Numbering
				//			Nastran Numbering
				//	1		1 2 3					0 1 2
				//	2		1 2 4					0 1 3
				//	3		2 3 4					1 2 3
				//	4		1 3 4					0 2 3

				totalCornerPoints = 4;
				numberSurfaceCornerPoints = 3;

				if ( GIDs.size() < numberSurfaceCornerPoints )
				{
					errorString <<
						"Function - " + std::string(__FUNCTION__) +
						" Insufficient grid points for a " << ElementType_ToString(Type) << " element.  Number of required grid points: " <<  totalCornerPoints << ".  Number of found grid points:  GIDs.size()";
					throw isis::application_exception(errorString.str().c_str());
				}

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[2];
				out_SurfaceID_to_CornerPoints_map[1] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[2] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[1];
				cornerPoints[1] = GIDs[2];
				cornerPoints[2] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[3] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[2];
				cornerPoints[2] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[4] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				break;
			case CQUAD:
				// See Nastran documentation for CQUAD Grid point numbering scheme
				//
				// Faces by Grid Point Numbering  
				// Face		Grid Points				GIDs Vector Numbering
				//			Nastran Numbering
				//	1		1 2 3 4					0 1 2 3

				totalCornerPoints = 4;
				numberSurfaceCornerPoints = 4;

				if ( GIDs.size() < numberSurfaceCornerPoints )
				{
					errorString <<
						"Function - " + std::string(__FUNCTION__) +
						" Insufficient grid points for a " << ElementType_ToString(Type) << " element.  Number of required grid points: " <<  totalCornerPoints << ".  Number of found grid points:  GIDs.size()";
					throw isis::application_exception(errorString.str().c_str());
				}

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[2];
				cornerPoints[3] = GIDs[3];
				out_SurfaceID_to_CornerPoints_map[1] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);

				break;
			case CTRIA3:
				// See Nastran documentation for CTRIA3 Grid point numbering scheme
				//
				// Faces by Grid Point Numbering  
				// Face		Grid Points				GIDs Vector Numbering
				//			Nastran Numbering
				//	1		1 2 3 					0 1 2

				totalCornerPoints = 3;
				numberSurfaceCornerPoints = 3;

				if ( GIDs.size() < numberSurfaceCornerPoints )
				{
					errorString <<
						"Function - " + std::string(__FUNCTION__) +
						" Insufficient grid points for a " << ElementType_ToString(Type) << " element.  Number of required grid points: " <<  totalCornerPoints << ".  Number of found grid points:  GIDs.size()";
					throw isis::application_exception(errorString.str().c_str());
				}

				cornerPoints[0] = GIDs[0];
				cornerPoints[1] = GIDs[1];
				cornerPoints[2] = GIDs[2];
				out_SurfaceID_to_CornerPoints_map[1] =  std::vector<int>(&cornerPoints[0],&cornerPoints[numberSurfaceCornerPoints]);
				break;
			default:
				std::stringstream errorString;
				errorString <<
					"Function - " + std::string(__FUNCTION__) +
					" unsupported element type: " <<  ElementType_ToString(Type) << ". Supported element types:  CHEXA, CPENTA, CTETRA, CQUAD, CTRIA3";
				throw isis::application_exception(errorString.str().c_str());
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FEAElement::getDiagonalPointForSurface( const std::vector<int> &in_SurfacePoints,
												int    &out_AnchorPoint,
												int    &out_DiagonalPoint) const
	{

		out_AnchorPoint = -99;
		out_DiagonalPoint = -99;

		// G1	Identification number of a grid point connected to a corner of the face. Required
		//		data for solid elements only. (Integer > 0 or blank)
		// G3	Identification number of a grid point connected to a corner diagonally opposite to
		//		G1 on the same face of a CHEXA or CPENTA element. Required data for
		//		quadrilateral faces of CHEXA and CPENTA elements only. G3 must be omitted
		//		for a triangular surface on a CPENTA element.
		// G4	Identification number of the CTETRA grid point located at the corner; this grid
		//		point may not reside on the face being loaded. This is required data and is used for
		//		CTETRA elements only. (Integer > 0)

		std::vector<int> elementCornerPoints;
		getElementCornerPoints(elementCornerPoints);

		if ( elementCornerPoints.size() < 3)
		{
			std::stringstream errorString;
			errorString <<  "Erroneous number of corner grid points.  There must be at least three corner grid points."		<< std::endl <<
						"Function:                " << __FUNCTION__ << std::endl <<
						"ElementType:             "	<<  ElementType_ToString(Type) <<  std::endl <<
						"ElementCornerGridPoints: ";
						for each ( const int &i_grid in elementCornerPoints) errorString << " " <<  i_grid;
			throw isis::application_exception(errorString.str()); 

		}

		if ( in_SurfacePoints.size() < 3)
		{
			std::stringstream errorString;
			errorString <<  "Erroneous number of surface grid points.  There must be at least three corner grid points."		<< std::endl <<
						"Function:                " << __FUNCTION__ << std::endl <<
						"ElementType:             "	<<  ElementType_ToString(Type) <<  std::endl <<
						"ElementCornerGridPoints: ";
						for each ( const int &i_grid in in_SurfacePoints) errorString << " " <<  i_grid;
			throw isis::application_exception(errorString.str()); 

		}


		if (Type == CQUAD  )
		{
			out_AnchorPoint = elementCornerPoints[0];
			out_DiagonalPoint = -1; // Single surface elements do not need a diagnoal point
			return;
		}

		if ( Type != CTETRA )
		{
				std::stringstream errorString;
				errorString <<
					"Function - " + std::string(__FUNCTION__) +
					" unsupported element type: " <<  ElementType_ToString(Type) << ". Supported element types: CTETRA, CQUAD, CTRIA3";
				throw isis::application_exception(errorString.str()); 
		}


		std::set<int> A(elementCornerPoints.begin(), elementCornerPoints.end());
		std::set<int> B(in_SurfacePoints.begin(), in_SurfacePoints.begin() + getSurfaceMinimumNumberOfGIDS());
	
		std::set<int> A_minus_B;
		
		std::set_difference(A.begin(), A.end(), 
							B.begin(), B.end(),
									std::inserter(	A_minus_B, A_minus_B.end()));

		switch ( Type )
		{
			//case CHEXA:  Not supported yet
			//case CPENTA: Not supported yet
			case CTETRA:
				// The desired point is the point not on the surface defined by in_SurfacePoints
				// This would be the point in A_minus_B
				if ( A_minus_B.size() != 1 )
				{
					std::stringstream errorString;
					errorString <<  "Erroneous Input."		<< std::endl <<
								"Function:                " << __FUNCTION__ << std::endl <<
								"ElementType:             "	<<  ElementType_ToString(Type) <<  std::endl <<
								"ElementCornerGridPoints: ";
								for each ( const int &i_grid in elementCornerPoints) errorString << " " <<  i_grid;
								errorString << 	std::endl << "SurfaceGridPoints:       ";
								for each ( const int &i_grid in in_SurfacePoints) errorString << " " <<  i_grid;
								errorString << 	std::endl << "ElementCornerGridPoints (A): ";
								for each ( const int &i_grid in A) errorString << " " <<  i_grid;
								errorString << 	std::endl << "SurfaceCornerGridPoints (B): ";
								for each ( const int &i_grid in B) errorString << " " <<  i_grid;
								errorString << 	std::endl << "ElementCornerGridPoints - SurfaceCornerGridPoints (A_minus_B): ";
								for each ( const int &i_grid in A_minus_B) errorString << " " <<  i_grid;
								errorString <<  std::endl << 
								"Error:                    There should be 4 ElementCornerGridPoints, with 3 of the points being SurfaceCornerGridPoints.";
					throw isis::application_exception(errorString.str()); 
				}
				
				out_AnchorPoint = in_SurfacePoints[0];
				out_DiagonalPoint = *A_minus_B.begin();

			break;

			case CQUAD:  // Single surface, no need for a diagnoal point		
			case CTRIA3:
				break;
			default:
				std::stringstream errorString;
				errorString <<
					"Function - " + std::string(__FUNCTION__) +
					" unsupported element type: " <<  ElementType_ToString(Type) << ". Supported element types: CTETRA, CQUAD, CTRIA3";
				throw isis::application_exception(errorString.str().c_str());
		}

	}

		FEAElement::FEAElement(): Type(ELEMENT_TYPE_UNDEFINED), EID(0), PID(0), thickness(0.0) {};

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

	// Token must contain a postive (one or  greater) integer; otherwise, throw an exception
	// IDs in a Nastran deck are always one or greater
	int atoi_WithVerifyIDInt(const std::string &in_Card, int in_FieldNum, const std::string &in_Token)
	{
		if ( in_Token.size() == 0  || in_Token.find_first_not_of("0123456789") != std::string::npos )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						    "Deck Line:    " << in_Card					<< std::endl <<
							"Field Number: " << in_FieldNum + 1			<< std::endl <<
							"Error:		   " << "The field (designated by Field Number) must contain an integer.";
			throw isis::application_exception(errorString.str()); 
		}


		int temp =	atoi(in_Token.c_str());

		if ( temp < 1 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						    "Deck Line:     " << in_Card				<< std::endl <<
							"Field Number:  " << in_FieldNum + 1		<< std::endl <<
							"Integer Value: " << temp					<< std::endl <<
							"Error:		   " << "The field (designated by Field Number) must contain an integer with a value greater than zero.";
			throw isis::application_exception(errorString.str()); 

		}

		return temp;
	}




	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//	class NastranDeck
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	NastranDeck::NastranDeck():	maxCoordinateID(0),
								MATERIALS_PLACEMENT_MAT4_set(false),
								TEMPERATURE_TEMP_set(false),
								TEMPERATURE_TEMPD_set(false), 
								POINT_SPOINT_set(false),
								CONVECTION_PCONV_set(false),
								CONVECTION_CONV_set(false),
								SURFACE_ELEMENT_CHBDYG_set(false),
								SURFACE_ELEMENT_QBDY3_set(false),
								VOLUME_ELEMENT_QVOL_set(false),
								createdElementCounter(100000),
								gridPointIDCurrentMax(0){}


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
	const std::map<int, MAT1>& NastranDeck::getMaterialData_MAT1() const
	{
		return materialData_MAT1;
	} 

	const std::map<int, MAT4>& NastranDeck::getMaterialData_MAT4() const
	{
		return materialData_MAT4;
	} 

	void NastranDeck::deleteAllMaterialData_MAT4()
	{
			materialData_MAT4.clear();
	};


	void NastranDeck::AddMaterialData_MAT4( const MAT4 &in_MAT4)
	{
	
		materialData_MAT4.insert(std::pair<int, MAT4>(in_MAT4.MID, in_MAT4));
		if ( !MATERIALS_PLACEMENT_MAT4_set )
		{
			bulkString.push_back("MATERIALS_PLACEMENT_MAT4");
			MATERIALS_PLACEMENT_MAT4_set = true;
		}
	}

	const std::multimap<int, TEMP>& NastranDeck::getTemperature_TEMP() const 
	{
		return temperature_TEMP;
	} 

	const std::multimap<int, TEMPD>& NastranDeck::getTemperature_TEMPD() const
	{
		return temperature_TEMPD;
	} 

	void  NastranDeck::AddTemperature_TEMPD(const TEMPD &in_TEMPD)
	{
		temperature_TEMPD.insert(std::pair<int, TEMPD>(in_TEMPD.SID, in_TEMPD));
		if ( !TEMPERATURE_TEMPD_set )
		{
			TEMPERATURE_TEMPD_set = true;
			bulkString.push_back("TEMPERATURE_TEMPD");
		}

	}

	void  NastranDeck::AddTemperature_TEMP(const TEMP &in_TEMP)
	{
		temperature_TEMP.insert(std::pair<int, TEMP>(in_TEMP.SID, in_TEMP));
		if ( !TEMPERATURE_TEMP_set )
		{
			TEMPERATURE_TEMP_set = true;
			bulkString.push_back("TEMPERATURE_TEMP");
		}
	}

	const std::map<int, SPOINT>& NastranDeck::getPoint_SPOINT() const
	{
		return point_SPOINT;
	} 

	void	NastranDeck::AddPoint_SPOINT(const SPOINT &in_SPOINT)
	{
		point_SPOINT.insert(std::pair<int, SPOINT>(in_SPOINT.ID, in_SPOINT));
		if ( !POINT_SPOINT_set )
		{
			bulkString.push_back("POINT_SPOINT");
			POINT_SPOINT_set = true;
		}
	}

	const std::map<int, PCONV>& NastranDeck::getConvection_PCONV() const 
	{
		return convection_PCONV;
	}

	void NastranDeck::AddConvection_PCONV( const PCONV &in_PCONV )
	{
		convection_PCONV.insert(std::pair<int, PCONV>(in_PCONV.PCONID, in_PCONV));
		if ( !CONVECTION_PCONV_set )
		{
			bulkString.push_back("CONVECTION_PCONV");
			CONVECTION_PCONV_set = true;
		}
	}

	const std::map<int, CONV>& NastranDeck::getConvection_CONV() const
	{
		return convection_CONV;
	} 

	void NastranDeck::AddConvection_CONV( const CONV &in_CONV )
	{
		convection_CONV.insert(std::pair<int, CONV>(in_CONV.EID, in_CONV));
		if ( !CONVECTION_CONV_set )
		{
			bulkString.push_back("CONVECTION_CONV");
			CONVECTION_CONV_set = true;
		}
	}

	const std::map<int, CHBDYG>& NastranDeck::getSurfaceElement_CHBDYG() const
	{
		return surfaceElement_CHBDYG;
	} 

	void  NastranDeck::AddSurfaceElement_CHBDYG(const CHBDYG &in_CHBDYG)
	{
		surfaceElement_CHBDYG.insert(std::pair<int, CHBDYG>(in_CHBDYG.EID, in_CHBDYG));

		if ( !SURFACE_ELEMENT_CHBDYG_set )
		{
			bulkString.push_back("SURFACE_ELEMENT_CHBDYG");
			SURFACE_ELEMENT_CHBDYG_set = true;
		}
	}

	const std::multimap<int, QBDY3>& NastranDeck::getHeatFlux_QBDY3() const
	{
		return heatFlux_QBDY3;
	} 

	void  NastranDeck::AddHeatFlux_QBDY3(const QBDY3 &in_QBDY3)
	{
		heatFlux_QBDY3.insert(std::pair<int, QBDY3>(in_QBDY3.SID, in_QBDY3));
		if ( !SURFACE_ELEMENT_QBDY3_set )
		{
			bulkString.push_back("SURFACE_ELEMENT_QBDY3");
			SURFACE_ELEMENT_QBDY3_set = true;
		}
	}

	const std::multimap<int, QVOL>& NastranDeck::getHeatVolume_QVOL() const
	{
		return heatVolume_QVOL;
	} 

	void  NastranDeck::AddHeatVolume_QVOL(const QVOL &in_QVOL)
	{
		//std::cout << std::endl << "-------------NastranDeck::AddHeatVolume_QVOL";
		//std::cout << std::endl << in_QVOL;
		//std::cout << std::endl << "SID" << in_QVOL.SID;
		//std::cout << std::endl << "EID1" << in_QVOL.EID1;
		heatVolume_QVOL.insert(std::pair<int, QVOL>(in_QVOL.SID, in_QVOL));
		if ( !VOLUME_ELEMENT_QVOL_set )
		{
			bulkString.push_back("VOLUME_ELEMENT_QVOL");
			VOLUME_ELEMENT_QVOL_set = true;
		}
	}


	std::map<int, GridPoint>& NastranDeck::getGridPointData() 
	{
		return gridPointData;
	}	

	int	 NastranDeck::getNextGridPointID()
	{
		if ( gridPointIDCurrentMax == 0 )
		{
			if ( gridPointData.size() > 0 )
			{
				gridPointIDCurrentMax = (--gridPointData.end())->first;
			}
		}
		return ++gridPointIDCurrentMax;
	}

	std::map<int, PSolid>& NastranDeck::getPsolidData() 
	{
		return psolidData;
	}
	const std::map<int, FEAElement>& NastranDeck::getElementData() const
	{
		return elementData;
	}
	std::map<int, CoordSystem>& NastranDeck::getCoordSystems() 
	{
		return coordSystems;
	}
	std::multimap<int, SPC>& NastranDeck::getSpcData_SIDKey() 
	{
		return spcData_SIDKey;
	}
	const std::multimap<int, SPC>& NastranDeck::getSpcData_GridIDKey() const
	{
		return spcData_GridIDKey;
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
	void NastranDeck::ReadNastranDeck( const std::string &in_InputFileName )
												throw (isis::application_exception)
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
													int in_LoadSetID, 
													bool in_IncludeSubcaseStatement )
	{
		std::string tempString;

		std::string indentString;

		if ( in_IncludeSubcaseStatement )
		{
			tempString = "SUBCASE = " + IntegerToString(in_SubCaseID);
			caseControl.push_back( tempString );
			indentString = "   ";
		}

		tempString = indentString + "SPC = " + IntegerToString(in_ConstraintSetID);
		caseControl.push_back( tempString );

		tempString = indentString + "LOAD = " + IntegerToString(in_LoadStatementID);
		caseControl.push_back( tempString );

		tempString = "LOAD," + IntegerToString(in_LoadStatementID) + ",1.,1.," + 
							   IntegerToString(in_LoadSetID);
				
		if ( bulkString.size() > 2 )
		{
			//Insert after first to param statements
			std::list<std::string>::iterator itr_bulk = bulkString.begin();
			++itr_bulk;
			++itr_bulk;
			bulkString.insert( itr_bulk, tempString );

		}
		else
		{
			bulkString.insert( bulkString.begin(), tempString );
		}

	}

	void NastranDeck::ReplaceExecutiveControlWithThermalStatements()
	{
		executiveControl.clear();
		executiveControl.push_back("SOL 153"); 
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::AddCardToCaseControl( const std::string &in_Card)
	{
		caseControl.push_back(in_Card);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void NastranDeck::ReplaceCaseControlWithThermalStatements(	const std::string	&in_Title,	
																//int					in_TemperatureInitialID, 
																int					in_NLParmID )
	{
			caseControl.clear();
			caseControl.push_back("TITLE = " + std::string(in_Title));
			caseControl.push_back("ANALYSIS = HEAT");
			caseControl.push_back("THERMAL = ALL");
			caseControl.push_back("FLUX = ALL");
			caseControl.push_back("SPCF = ALL");
			caseControl.push_back("OLOAD = ALL");
			//caseControl.push_back("TEMP(INIT) = " + IntegerToString(in_TemperatureInitialID));

			// Initialize with a TEMPD card.  This will get replaced if TEMPD is 
			//temperature_TEMPD.clear();

			// If a TEMPD card was added by the Creo mesher, delete it so that a correct value can (optionally) be added later.
			temperature_TEMPD.clear();

			caseControl.push_back("NLPARM = " + IntegerToString(in_NLParmID));

			std::string tempString = "NLPARM," + IntegerToString(in_NLParmID) + ",10,,AUTO,5,25,,NO";
				

			if ( bulkString.size() > 2 )
			{
				//Insert after first to param statements
				std::list<std::string>::iterator itr_bulk = bulkString.begin();
				++itr_bulk;
				++itr_bulk;
				bulkString.insert( itr_bulk, tempString );

			}
			else
			{
				bulkString.insert( bulkString.begin(), tempString );
			}
				



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
		if (accelerationLoadData.empty()) bulkString.push_back(c_ACCELERATIONS_PLACEMENT);

		Acceleration newAcceleration = Acceleration();
		newAcceleration.SID = in_LoadID;
		newAcceleration.CID = in_CoordinateSystemID;
		//we have to convert the doubles to strings first
		newAcceleration.A = DoubleToString(in_AccelerationScaleFactor);
		newAcceleration.N1 = DoubleToString(in_Vector_x);
		newAcceleration.N2 = DoubleToString(in_Vector_y);
		newAcceleration.N3 = DoubleToString(in_Vector_z);

		//Finally, insert this into our forceLoadData object
		accelerationLoadData.insert(std::make_pair(newAcceleration.SID, newAcceleration));

		//std::string tempString;

		//tempString = "GRAV,"	+ IntegerToString(in_LoadID) +
					 ","		+ IntegerToString(in_CoordinateSystemID);
		
		//tempString +=  ScaleFactorVectorString( in_AccelerationScaleFactor, in_Vector_x, in_Vector_y, in_Vector_z );
		//bulkString.push_back( tempString );
	}
	
	// Pressure			PLOAD4,59,1875,14904.,14904.,14904.,,355,1221
	// PLOAD4 SID EID P1 P2 P3 P4 G1 G3 or G4 
	//		  CID N1 N2 N3 SORL LDIR
	//		SID Load set identification number
	//		P Pressure
	//		Gi Grid point identification numbers
	// PLOAD4,59,5877,14904.,14904.,14904.,,418,1416
	void NastranDeck::AddPressureToDeck( int in_LoadID, int in_ElemID, double in_Pressure, int in_Grid_1, int in_Grid_3_or_4 )
	{
		if( pressureLoadData.empty())
			bulkString.push_back(c_PRESSURES_PLACEMENT);

		Pressure newPressure = Pressure();
		newPressure.SID = in_LoadID;
		newPressure.EID = in_ElemID;

		//we have to convert the doubles to strings first
		newPressure.P1 = DoubleToString(in_Pressure);
		newPressure.G1 = IntegerToString(in_Grid_1);

		if ( in_Grid_3_or_4 != -1 )
			newPressure.G3_or_G4 = IntegerToString(in_Grid_3_or_4);

		//Finally, insert this into our pressureLoadData object
		pressureLoadData.insert(std::make_pair(newPressure.SID, newPressure));

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

	void NastranDeck::CommentOutMat1Cards()
	{
		for ( std::list<std::string>::iterator i(bulkString.begin()); i != bulkString.end(); ++i )
		{	
			if ( i->find("MAT1") == 0 ) *i = "$META " + *i;
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
			// The deck will contain a card as follows:
			// MATERIALS_PLACEMENT_MAT1
			// Must only search on cards that begin with MAT1
			if ( i->find("MAT1") == 0 )  // Card must start with MAT1 
			{
				//std::cout << std::endl << "MAT1 Card: " << *i;
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
					throw isis::application_exception(errorString.str());	
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
			
			if(line == "MATERIALS_PLACEMENT_MAT1")
			{
				//to be replaced later by iterating through
				//materials?
				//DO NOTHING for now
			}
			else if(line == "MATERIALS_PLACEMENT_MAT4")
			{
				for each ( const std::pair<int,MAT4> &i in in_NastranDeck.materialData_MAT4 )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "TEMPERATURE_TEMP")
			{
				for each ( const std::pair<int,TEMP> &i in in_NastranDeck.temperature_TEMP )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "TEMPERATURE_TEMPD")
			{
				for each ( const std::pair<int,TEMPD> &i in in_NastranDeck.temperature_TEMPD )
				{
					output << i.second << std::endl;
				}

			}


			else if(line == "POINT_SPOINT")
			{
				for each ( const std::pair<int,SPOINT> &i in in_NastranDeck.point_SPOINT )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "CONVECTION_PCONV")
			{
				for each ( const std::pair<int,PCONV> &i in in_NastranDeck.convection_PCONV )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "CONVECTION_CONV")
			{
				for each ( const std::pair<int,CONV> &i in in_NastranDeck.convection_CONV )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "SURFACE_ELEMENT_CHBDYG")
			{
				for each ( const std::pair<int,CHBDYG> &i in in_NastranDeck.surfaceElement_CHBDYG )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "SURFACE_ELEMENT_QBDY3")
			{
				for each ( const std::pair<int,QBDY3> &i in in_NastranDeck.heatFlux_QBDY3 )
				{
					output << i.second << std::endl;
				}

			}

			else if(line == "VOLUME_ELEMENT_QVOL")
			{
				for each ( const std::pair<int,QVOL> &i in in_NastranDeck.heatVolume_QVOL )
				{
					output << i.second << std::endl;
				}

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
				std::map<int, FEAElement>::const_iterator it = in_NastranDeck.elementData.begin();
				std::map<int, FEAElement>::const_iterator endMap = in_NastranDeck.elementData.end();
				for(; it != endMap; ++it)
				{
					output << it->second << std::endl ;
				}
			}
			else if(line == "CONSTRAINTS_PLACEMENT")
			{
				std::multimap<int, SPC>::const_iterator it = in_NastranDeck.spcData_SIDKey.begin();
				std::multimap<int, SPC>::const_iterator endMap = in_NastranDeck.spcData_SIDKey.end();
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
			else if(line == c_ACCELERATIONS_PLACEMENT)
			{
				for each ( const std::pair<int,Acceleration> &it in in_NastranDeck.accelerationLoadData ) output << it.second << std::endl;
			}

			else if(line == c_PRESSURES_PLACEMENT)
			{
				for each ( const std::pair<int,Pressure> &it in in_NastranDeck.pressureLoadData ) output << it.second << std::endl;
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

			// Skip blank lines
			// Skip comment (i.e. $ ...)
			if (data == "" || data[0] == '$' )
			{
				yi++;
				continue;
			}

			// Only handling SUBCASE
			if (data.find("SUBCASE") == std::string::npos)	
			{
				yi++;
				continue;
			}		

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
					//if (line.find("SUBCASE") == std::string::npos)
					if ( line != "" && line[0] != '$' &&  line.find("SUBCASE") == std::string::npos )
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
					if(materialData_MAT1.empty())
					{
						bulkString.push_back("MATERIALS_PLACEMENT_MAT1");
					}

					MAT1 material;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() != 9)
					{
						std::cout << "MAT1: Not enough entries, need to be 9 [" << data << "]" << std::endl;
						continue;
					}
					material.name = tokens[0];
					material.MID = atoi_WithVerifyIDInt(data, 1, tokens[1]);
					material.Young = tokens[2];
					material.Shear = tokens[3];
					material.Poisson = tokens[4];
					material.Density = tokens[5];
					material.Thermal = tokens[6];
					material.RefTemp = (tokens[7] == " " ? "0": tokens[7]);
					material.Damping = tokens[8];
					materialData_MAT1[material.MID] = material;
					bulkString.push_back(data); //need this because of the double commas
					//we don't know how original string delimits tokens - or maybe not
					//replace comma space comma with double comma?
				}
				else if (card == "MAT4")
				{
					if(materialData_MAT4.empty())
					{
						bulkString.push_back("MATERIALS_PLACEMENT_MAT4");
						MATERIALS_PLACEMENT_MAT4_set = true;
					}

					MAT4 material;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 3 )
					{
						std::cout << "MAT4: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 ) material.name =		tokens[0];
					if ( tokens.size() > 1 ) { material.MID =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 ) { material.K =			(tokens[2] == " " ? "": tokens[2]);
					if ( tokens.size() > 3 ) { material.CP =		(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 ) { material.p =			(tokens[4] == " " ? "": tokens[4]);
					if ( tokens.size() > 5 ) { material.H =			(tokens[5] == " " ? "": tokens[5]);
					if ( tokens.size() > 6 ) { material.m =			(tokens[6] == " " ? "": tokens[6]);
					if ( tokens.size() > 7 ) { material.HGEN =		(tokens[7] == " " ? "": tokens[7]);
					if ( tokens.size() > 8 ) { material.REFENTH =	(tokens[8] == " " ? "": tokens[8]);
					if ( tokens.size() > 9 ) { material.TCH =		(tokens[9] == " " ? "": tokens[9]);
					if ( tokens.size() > 10 ){ material.TDELTA =	(tokens[10] == " " ? "": tokens[10]);
					if ( tokens.size() > 11 ){ material.QLAT =		(tokens[11] == " " ? "": tokens[11]);
					}}}}}}}}}}}

					materialData_MAT4[material.MID] = material;
					//bulkString.push_back(data); //need this because of the double commas
					//we don't know how original string delimits tokens - or maybe not
					//replace comma space comma with double comma?
				}


				else if (card == "TEMP")
				{
					if(temperature_TEMP.empty())
					{
						bulkString.push_back("TEMPERATURE_TEMP");
						TEMPERATURE_TEMP_set = true;
					}

					TEMP elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 2)
					{
						std::cout << "TEMP: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 ) elem.name =	tokens[0];
					if ( tokens.size() > 1 ) { elem.SID =	atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 ) { elem.G1 =	atoi_WithVerifyIDInt(data, 2, tokens[2]);
					if ( tokens.size() > 3 ) { elem.T1 =	(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 ) { elem.G2 =	atoi_WithVerifyIDInt(data, 4, tokens[4]);
					if ( tokens.size() > 5 ) { elem.T2 =	(tokens[5] == " " ? "": tokens[5]);
					if ( tokens.size() > 6 ) { elem.G3 =	atoi_WithVerifyIDInt(data, 6, tokens[6]);
					if ( tokens.size() > 7 ) { elem.T3 =	(tokens[7] == " " ? "": tokens[7]);
					}}}}}}}

					temperature_TEMP.insert(std::pair<int,TEMP>(elem.SID, elem));
					//bulkString.push_back(data); //need this because of the double commas
					//we don't know how original string delimits tokens - or maybe not
					//replace comma space comma with double comma?
				}

				
				else if (card == "TEMPD")
				{
					if(temperature_TEMPD.empty())
					{
						bulkString.push_back("TEMPERATURE_TEMPD");
						TEMPERATURE_TEMPD_set = true;
					}

					TEMPD elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 2)
					{
						std::cout << "TEMPD: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					// Note - if there are more than one SIDs, the SIDs must not be the same.  For a SID, there can be one and
					//		  only one default temperature.

					if ( tokens.size() > 0 ) elem.name =	tokens[0];
					if ( tokens.size() > 1 ) { elem.SID =	atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 ) { elem.T =	(tokens[2] == " " ? "": tokens[2]); temperature_TEMPD.insert(std::pair<int,TEMPD>(elem.SID, elem));
					if ( tokens.size() > 3 ) { elem.SID =	atoi_WithVerifyIDInt(data, 3, tokens[3]);
					if ( tokens.size() > 4 ) { elem.T =	(tokens[4] == " " ? "": tokens[4]);  temperature_TEMPD.insert(std::pair<int,TEMPD>(elem.SID, elem));
					if ( tokens.size() > 5 ) { elem.SID =	atoi_WithVerifyIDInt(data, 5, tokens[5]);
					if ( tokens.size() > 6 ) { elem.T =	(tokens[6] == " " ? "": tokens[6]);  temperature_TEMPD.insert(std::pair<int,TEMPD>(elem.SID, elem));
					if ( tokens.size() > 7 ) { elem.SID =	atoi_WithVerifyIDInt(data, 7, tokens[7]);
					if ( tokens.size() > 8 ) { elem.T =	(tokens[8] == " " ? "": tokens[8]);  temperature_TEMPD.insert(std::pair<int,TEMPD>(elem.SID, elem));
					}}}}}}}}
				}				

				else if (card == "SPOINT")
				{
					if(point_SPOINT.empty())
					{
						bulkString.push_back("POINT_SPOINT");
						POINT_SPOINT_set = true;

					}

					SPOINT elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 2)
					{
						std::cout << "SPOINT: Not enough entries, need to be greater than one [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 ) elem.name =	tokens[0];
					if ( tokens.size() > 1 ) { elem.ID =	atoi_WithVerifyIDInt(data, 1, tokens[1]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 2 ) { elem.ID =	atoi_WithVerifyIDInt(data, 2, tokens[2]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 3 ) { elem.ID =	atoi_WithVerifyIDInt(data, 3, tokens[3]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 4 ) { elem.ID =	atoi_WithVerifyIDInt(data, 4, tokens[4]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 5 ) { elem.ID =	atoi_WithVerifyIDInt(data, 5, tokens[5]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 6 ) { elem.ID =	atoi_WithVerifyIDInt(data, 6, tokens[6]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 7 ) { elem.ID =	atoi_WithVerifyIDInt(data, 7, tokens[7]); point_SPOINT[elem.ID] = elem;
					if ( tokens.size() > 8 ) { elem.ID =	atoi_WithVerifyIDInt(data, 8, tokens[8]); point_SPOINT[elem.ID] = elem;
					}}}}}}}}

				}
				
				else if (card == "PCONV")
				{
					if(convection_PCONV.empty())
					{
						bulkString.push_back("CONVECTION_PCONV");
						CONVECTION_PCONV_set = true;
					}

					PCONV elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 3)
					{
						std::cout << "PCONV: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 ) elem.name =		tokens[0];
					if ( tokens.size() > 1 ) { elem.PCONID =	atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 ) { elem.MID =		atoi_WithVerifyIDInt(data, 2, tokens[2]);
					if ( tokens.size() > 3 ) { elem.FORM =		(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 ) { elem.EXPF =		(tokens[4] == " " ? "": tokens[4]); 
					}}}}

					convection_PCONV[elem.PCONID] = elem;
				}
				
				
				else if (card == "CONV")
				{
					if(convection_CONV.empty())
					{
						bulkString.push_back("CONVECTION_CONV");
						CONVECTION_CONV_set = true;
					}


					CONV elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 3)
					{
						std::cout << "CONV: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 )   elem.name =		tokens[0];
					if ( tokens.size() > 1 )  { elem.EID =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 )  { elem.PCONID =	atoi_WithVerifyIDInt(data, 2, tokens[2]);
					if ( tokens.size() > 3 )  { elem.FLMND =	(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 )  { elem.CNTRLND =	(tokens[4] == " " ? "": tokens[4]);
					if ( tokens.size() > 5 )  { elem.TA1 =		atoi_WithVerifyIDInt(data, 5, tokens[5]);
					if ( tokens.size() > 6 )  { elem.TA2 =		atoi_WithVerifyIDInt(data, 6, tokens[6]);
					if ( tokens.size() > 7 )  { elem.TA3 =		atoi_WithVerifyIDInt(data, 7, tokens[7]); 
					if ( tokens.size() > 8 )  { elem.TA4 =		atoi_WithVerifyIDInt(data, 8, tokens[8]);
					}}}}}}}}

					// Must check for continuation card
					std::list<std::string>::const_iterator bulkIter_temp = bulkIter;
					++bulkIter_temp;
					if (bulkIter_temp != endBulk )
					{
						std::string tempString =  *bulkIter_temp;
						if ( tempString[0] == ',' )
						{
							++bulkIter;
							data = *bulkIter;
							tokens.clear();
							stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
							if (tokens.size() < 1 )
							{
								std::cout << "CONV: Not enough entries, need to be one or more [" << data << "]" << std::endl;
								continue;
							}
							if ( tokens.size() > 0 )  { elem.TA5 =		atoi_WithVerifyIDInt(data, 0, tokens[0]);
							if ( tokens.size() > 1 )  { elem.TA6 =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
							if ( tokens.size() > 2 )  { elem.TA7 =		atoi_WithVerifyIDInt(data, 2, tokens[2]);
							if ( tokens.size() > 3 )  { elem.TA8 =		atoi_WithVerifyIDInt(data, 3, tokens[3]);
							}}}}
						}
					}
					convection_CONV[elem.EID] = elem;
				}	
				
				else if (card == "CHBDYG")
				{
					if(surfaceElement_CHBDYG.empty())
					{
						bulkString.push_back("SURFACE_ELEMENT_CHBDYG");
						SURFACE_ELEMENT_CHBDYG_set = true;
					}

					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 3)
					{
						std::cout << "CHBDYG: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					CHBDYG elem;
					if ( tokens.size() > 0 )   elem.name =		tokens[0];
					if ( tokens.size() > 1 )  { elem.EID =	atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 )  { elem.TYPE =	(tokens[2] == " " ? "": tokens[2]);
					if ( tokens.size() > 3 )  { elem.IVIEWF =	(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 )  { elem.IVIEWB =	(tokens[4] == " " ? "": tokens[4]);
					if ( tokens.size() > 5 )  { elem.RADMIDF =	(tokens[5] == " " ? "": tokens[5]);
					if ( tokens.size() > 6 )  { elem.RADMIDB =	(tokens[6] == " " ? "": tokens[6]);
					}}}}}}

					// Must check for continuation card
					std::list<std::string>::const_iterator bulkIter_temp = bulkIter;
					++bulkIter_temp;
					if (bulkIter_temp != endBulk )
					{
						std::string tempString =  *bulkIter_temp;
						if ( tempString[0] == ',' )
						{
							++bulkIter;
							data = *bulkIter;
							tokens.clear();
							stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
							if (tokens.size() < 1)
							{
								std::cout << "CHBDYG: Not enough entries, need one or more [" << data << "]" << std::endl;
								continue;
							}

							if ( tokens.size() > 0 )  { elem.G1 =		atoi_WithVerifyIDInt(data, 0, tokens[0]);
							if ( tokens.size() > 1 )  { elem.G2 =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
							if ( tokens.size() > 2 )  { elem.G3 =		atoi_WithVerifyIDInt(data, 2, tokens[2]); 
							if ( tokens.size() > 3 ) { elem.G4 =		atoi_WithVerifyIDInt(data, 3, tokens[3]);
							if ( tokens.size() > 4 ) { elem.G5 =		atoi_WithVerifyIDInt(data, 4, tokens[4]);
							if ( tokens.size() > 5 ) { elem.G6 =		atoi_WithVerifyIDInt(data, 5, tokens[5]);
							if ( tokens.size() > 6 ) { elem.G7 =		atoi_WithVerifyIDInt(data, 6, tokens[6]);
							if ( tokens.size() > 7 ) { elem.G8 =		atoi_WithVerifyIDInt(data, 7, tokens[7]);
							}}}}}}}}

						}
					}

					//std::cout << std::endl << "****************** G1 *************: " << 	 elem.G1;
					surfaceElement_CHBDYG[elem.EID] = elem;
				}	

				else if (card == "QBDY3")
				{
					if(heatFlux_QBDY3.empty())
					{
						bulkString.push_back("SURFACE_ELEMENT_QBDY3");
						SURFACE_ELEMENT_QBDY3_set = true;
					}

					QBDY3 elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 3)
					{
						std::cout << "QBDY3: Not enough entries, need to be greater than two [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 )   elem.name =		tokens[0];
					if ( tokens.size() > 1 )  { elem.SID =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 )  { elem.Q0 =		(tokens[2] == " " ? "": tokens[2]);
					if ( tokens.size() > 3 )  { elem.CNTRLND =		(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 )  { elem.EID1 =		atoi_WithVerifyIDInt(data, 4, tokens[4]);
					if ( tokens.size() > 5 )  { elem.EID2 =		atoi_WithVerifyIDInt(data, 5, tokens[5]);
					if ( tokens.size() > 6 )  { elem.EID3 =		atoi_WithVerifyIDInt(data, 6, tokens[6]); 
					if ( tokens.size() > 7 )  { elem.EID4 =		atoi_WithVerifyIDInt(data, 7, tokens[7]);
					if ( tokens.size() > 8 )  { elem.EID5 =		atoi_WithVerifyIDInt(data, 8, tokens[8]);
					}}}}}}}}
					

					// Must check for continuation card
					std::list<std::string>::const_iterator bulkIter_temp = bulkIter;
					++bulkIter_temp;
					if (bulkIter_temp != endBulk )
					{
						std::string tempString =  *bulkIter_temp;
						if ( tempString[0] == ',' )
						{
							++bulkIter;
							data = *bulkIter;
							tokens.clear();
							stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
							if (tokens.size() < 1)
							{
								std::cout << "QBDY3: Not enough entries, need one or more [" << data << "]" << std::endl;
								continue;
							}
							if ( tokens.size() > 0 ) { elem.EID6 =		atoi_WithVerifyIDInt(data, 0, tokens[0]);
							if ( tokens.size() > 1 ) { elem.EID7 =		atoi_WithVerifyIDInt(data, 1, tokens[1]);
							if ( tokens.size() > 2 ) { elem.EID8 =		atoi_WithVerifyIDInt(data, 2, tokens[2]);
							}}}

						}
					}

					heatFlux_QBDY3.insert(std::pair<int,QBDY3>(elem.SID, elem));

				}

				else if (card == "QVOL")
				{
					if(heatVolume_QVOL.empty())
					{
						bulkString.push_back("VOLUME_ELEMENT_QVOL");
						VOLUME_ELEMENT_QVOL_set  = true;
					}

					QVOL elem;
					stringTokenize(data.c_str(), tokens, ",");		//tokenize(data)
					if (tokens.size() < 5)
					{
						std::cout << "QVOL: Not enough entries, need to be greater than four [" << data << "]" << std::endl;
						continue;
					}

					if ( tokens.size() > 0 )   elem.name =			tokens[0];
					if ( tokens.size() > 1 )  { elem.SID =			atoi_WithVerifyIDInt(data, 1, tokens[1]);
					if ( tokens.size() > 2 )  { elem.QVOL_Value =	(tokens[2] == " " ? "": tokens[2]);
					if ( tokens.size() > 3 )  { elem.CNTRLND =		(tokens[3] == " " ? "": tokens[3]);
					if ( tokens.size() > 4 )  { elem.EID1 =			atoi_WithVerifyIDInt(data, 4, tokens[4]);
					if ( tokens.size() > 5 )  { elem.EID2 =			atoi_WithVerifyIDInt(data, 5, tokens[5]);
					if ( tokens.size() > 6 )  { elem.EID3 =			atoi_WithVerifyIDInt(data, 6, tokens[6]); 
					if ( tokens.size() > 7 )  { elem.EID4 =			atoi_WithVerifyIDInt(data, 7, tokens[7]);
					if ( tokens.size() > 8 )  { elem.EID5 =			atoi_WithVerifyIDInt(data, 8, tokens[8]);
					}}}}}}}}
					
					heatVolume_QVOL.insert(std::pair<int,QVOL>(elem.SID, elem));

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
					
					FEAElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CTETRA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GIDs.push_back(atoi(tempGID[i].c_str()));
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
					FEAElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CPENTA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GIDs.push_back(atoi(tempGID[i].c_str()));
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
					
					FEAElement element;
					element.EID = atoi(tokens[1].c_str());
					element.PID = atoi(tokens[2].c_str());
					element.Type = CHEXA;
					std::vector<std::string> tempGID;
					tempGID.assign(tokens.begin() + 3, tokens.end());
					for(size_t i = 0; i < tempGID.size(); ++i)
					{
						element.GIDs.push_back(atoi(tempGID[i].c_str()));
					}
					elementData.insert(std::make_pair(element.EID, element));
					hexaCount++;
				}
				else if (card == "SPC")		
				{
					if(spcData_SIDKey.empty())
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
					spcData_SIDKey.insert(std::make_pair(spc.SID, spc));
					int gridPointID = atoi(spc.G1.c_str());
					spcData_GridIDKey.insert(std::make_pair(gridPointID, spc));
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


	void NastranDeck::FindElementsFromPSolids(const std::vector<int> &in_PSolidIDs, std::vector<int> &out_elementIDs)
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
		for(std::map<int, FEAElement>::iterator iter = elementData.begin(); iter!=elementData.end(); ++iter)
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

	double NastranDeck::VolumeOfPSolids(const std::vector<int> &in_PSolidIDs)
	{
		if(in_PSolidIDs.empty())
			throw isis::application_exception( std::string(__FUNCTION__) + ", input vector of PSolid IDs is empty");
		
		//first ensure that our PSolids are in range
		//throw exception if any ID not in range
		for(size_t i = 0; i < in_PSolidIDs.size(); ++i)
		{
			if(in_PSolidIDs[i] > psolidData.size() || in_PSolidIDs[i] < 1)
			{
				throw isis::application_exception(std::string(__FUNCTION__) + ", invalid PSolid ID in input vector (at least one ID is out of range)");
			}
		}	

		std::vector<int> elementIDs;

		FindElementsFromPSolids(in_PSolidIDs, elementIDs);

		if ( elementIDs.size() == 0 )
		{
			throw isis::application_exception(std::string(__FUNCTION__) + ", no elements found.  Volume could not be computed.");
		}

		double volume = 0.0;

		for each ( const int &i in elementIDs )
		{
			std::map<int, FEAElement>::const_iterator elem_itr = elementData.find(i);

			if ( elem_itr == elementData.end() )
			{	
				std::stringstream errorString;
				errorString <<
					"Function - " + std::string(__FUNCTION__) +
					" element not found in \"std::map<int, FEAElement> elementData\".  Elemenet ID: " << i;
				throw isis::application_exception(errorString.str().c_str());
			}

			volume += elem_itr->second.getVolume(gridPointData);	
			//std::cout << std::endl << "Element: " << elem_itr->first << "  " << elem_itr->second;
			//std::cout << std::endl << "Volume: " << volume;
		}
		return volume;
	}

	void NastranDeck::FindGridPointsFromElements(std::vector<int> &in_ElementIDs, std::vector<int> &out_gridPoints)
	{
		//throw exception for empty input vector
		if(in_ElementIDs.empty())
			throw isis::application_exception("NastranDeck::FindGridPointsFromElements, input vector of FEAElement IDs is empty");
		//ensure that our Elements are in range
		//throw exception if any ID not in range
		for(size_t i = 0; i < in_ElementIDs.size(); ++i)
		{
			 if(in_ElementIDs[i] > elementData.size() || in_ElementIDs[i] < 1)
			 {
				 throw isis::application_exception("NastranDeck::FindGridPointsFromElements, invalid FEAElement ID in input vector (at least one ID is out of range)");
			 }
		}
		
		//Next, we find all the grid points belonging to each element. We record these in a set
		// to preserve uniqueness, then copy the elements to a vector
		std::set<int> gridPointSet = std::set<int>();

		for(size_t i = 0; i < in_ElementIDs.size(); ++i)
		{
			size_t gridMax = 0;
			//we find the max number of grid points (size of the element's GID vector)
			gridMax = elementData[in_ElementIDs[i]].GIDs.size();

			for(size_t j = 0; j < gridMax; ++j)
			{
				//we insert the element into our set
				gridPointSet.insert(elementData[in_ElementIDs[i]].GIDs[j]);
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
		
		for(std::map<int, FEAElement>::iterator iter = elementData.begin();
			iter != elementData.end(); ++iter)
		{
			//std::cout << "Element ID: " << iter->second.EID << std::endl;
			size_t gridMax = iter->second.GIDs.size();
			//find its number of grid points
			for(size_t i = 0; i < gridMax; ++i)
			{
				//for each grid point
				//if our grid point is equal to in_GridPointID
				// we put it in the vector
				//std::cout << iter->second.GID[i] << std::endl;
				if(in_GridPointID == iter->second.GIDs[i])
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
			throw isis::application_exception("NastranDeck::FindPSolidsFromElements - input FEAElement vector is empty");

		out_PSolidIDs.clear();
		std::set<int> uniquePSolids = std::set<int>();
		for(std::vector<int>::iterator iter = in_ElementIDs.begin();
			iter != in_ElementIDs.end(); ++iter)
		{
			if(elementData.find(*iter) == elementData.end())
				throw isis::application_exception("NastranDeck::FindPSolidsFromElements - invalid FEAElement ID in input vector (at least one ID is out of range)");
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
			
				for(std::map<int, FEAElement>::iterator iter = elementData.begin(); 
					iter != elementData.end(); 
					++iter)
				{
					//if we find a FEAElement with the PID of our PSolid
					if(iter->second.PID == *PSolidIter)
					{
						//iterate through its gridpoints
						for(size_t j = 0; j < iter->second.GIDs.size(); ++j)
						{
							//If the GID of the current polygon 
							//is equal to our input gridpoint
							if(iter->second.GIDs[j] == in_gridPoint)
							{
								//replace the gridpoint with our new one
								iter->second.GIDs[j] = newGridPoint.ID;
							
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
		for(std::map<int, FEAElement>::iterator iter = elementData.begin();
			iter != elementData.end(); ++iter)
		{
			//find out if the element at iter has a grid point in equalGrids
			size_t maxSize = iter->second.GIDs.size();

			//iterate through its grid IDs
			for(size_t i = 0; i < maxSize; ++i)
			{
				//iterate through equalGrids to see if any of them 
				// are equal to our grid ID
				for(size_t j = 0; j < equalGrids.size(); ++j)
				{
					if(equalGrids[j] == iter->second.GIDs[i])
					{
						/*std::cout << "CHANGED: "  << iter->second.EID <<  " - " 
							<< iter->second.GID[i] << " to " <<  in_gridPoint  << std::endl;*/
						//replace the former element with the input param
						iter->second.GIDs[i] = in_gridPoint;
						
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
			throw isis::application_exception(errorString.str());
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	NastranDeckHelper::NastranDeckHelper( const NastranDeck &in_NastranDeck ) : nastranDeck( in_NastranDeck ), sufaceCornerGridIDs_to_FEAElementIDs_map_populated(false) {}

	void NastranDeckHelper::getDefaultGridPointTemperature( bool &out_DefaultGridPointTemperature_set, double &out_DefaultGridPointTemperature ) const
																								throw (isis::application_exception)
	{
		out_DefaultGridPointTemperature_set = false;
		out_DefaultGridPointTemperature = 0.0;

		if ( nastranDeck.getTemperature_TEMPD().size() == 0 ) return;

		if ( nastranDeck.getTemperature_TEMPD().size() > 1 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:       getDefaultGridPointTemperature" << std::endl <<
							"Number Default Grid Point Temperatures:  " <<  nastranDeck.getTemperature_TEMPD().size()		<< std::endl <<
							"Error:		   " << "A nastranDeck can contain at the most one Default-Node temperature";
			throw isis::application_exception(errorString.str()); 
		}

		std::pair<int, TEMPD> temp_pair = *nastranDeck.getTemperature_TEMPD().begin();

		std::string temp_string = temp_pair.second.T;

		// This check should be moved to when the deck was read from the BDF.  
		// Leave the check here for now to assure the integrity of the data.
		if ( temp_string.find_first_not_of("0123456789.-+eE") != std::string::npos )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:						NastranDeckHelper::getDefaultGridPointTemperature" << std::endl <<
							"DefaultGridPointTemperature string: " << temp_string		<< std::endl <<
							"Error:		                    " << "DefaultGridPointTemperature string must represent a valid floating point number.";
			throw isis::application_exception(errorString.str()); 

		}

		out_DefaultGridPointTemperature = atof(temp_pair.second.T.c_str());
		out_DefaultGridPointTemperature_set = true;
	}



	void NastranDeckHelper::getSpecifiedGridPointTemperatures( std::map<int,double> &out_GridPointToTemperature_map ) const
																								throw (isis::application_exception)
	{
		out_GridPointToTemperature_map.clear();

		const std::multimap<int, TEMP> &temperature_TEMP_map = nastranDeck.getTemperature_TEMP();

		for each ( const std::pair<int, TEMP> &i in temperature_TEMP_map )
		{
			if ( i.second.G1 != 0 ) // 0 would indicate that the temperature is not set
			{		
				if ( i.second.T1.find_first_not_of("0123456789.-+eE") != std::string::npos )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:						NastranDeckHelper::getSpecifiedGridPointTemperatures" << std::endl <<
							"GridPointTemperature string:   " <<  i.second.T1		<< std::endl <<
							"Error:		                    " << "GridPointTemperature string must represent a valid floating point number.";
					throw isis::application_exception(errorString.str()); 
				}
				out_GridPointToTemperature_map[i.second.G1] = atof(i.second.T1.c_str());
			}		

			if ( i.second.G2 != 0 ) // 0 would indicate that the temperature is not set
			{		
				if ( i.second.T2.find_first_not_of("0123456789.-+eE") != std::string::npos )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:						NastranDeckHelper::getSpecifiedGridPointTemperatures" << std::endl <<
							"GridPointTemperature string:   " <<  i.second.T2		<< std::endl <<
							"Error:		                    " << "GridPointTemperature string must represent a valid floating point number.";
					throw isis::application_exception(errorString.str()); 
				}
				out_GridPointToTemperature_map[i.second.G2] = atof(i.second.T2.c_str());
			}		

			if ( i.second.G3 != 0 ) // 0 would indicate that the temperature is not set
			{		
				if ( i.second.T3.find_first_not_of("0123456789.-+eE") != std::string::npos )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:						NastranDeckHelper::getSpecifiedGridPointTemperatures" << std::endl <<
							"GridPointTemperature string:   " <<  i.second.T3		<< std::endl <<
							"Error:		                    " << "GridPointTemperature string must represent a valid floating point number.";
					throw isis::application_exception(errorString.str()); 
				}
				out_GridPointToTemperature_map[i.second.G3] = atof(i.second.T3.c_str());
			}		

		}
	}

	// Supports Elements: CHEXAC, PENTA, CTETRA, CQUAD and CTRIA3 
	void NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map() throw (isis::application_exception)
	{
		const std::map<int, FEAElement> &elementData_map = nastranDeck.getElementData();

		for each ( const std::pair<int, FEAElement> &i in elementData_map)
		{
			if (i.second.Type != CHEXA &&  i.second.Type != CPENTA && i.second.Type != CTETRA && i.second.Type != CQUAD && i.second.Type != CTRIA3 )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
							"Function:				" << __FUNCTION__ << std::endl <<
							"ElementType:			"	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
							"Error:		            " << "Only CHEXA, CPENTA, CTETRA, CQUAD, and CTRIA3 currently supported.";
				throw isis::application_exception(errorString.str()); 
			}
			
			switch ( i.second.Type )
			{
			case CHEXA:
				if (i.second.GIDs.size() < 8 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map" << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
								"ElementNumberGridPoints: " << i.second.GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 8 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;
			case CPENTA:
				if (i.second.GIDs.size() < 6 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map" << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
								"ElementNumberGridPoints: " << i.second.GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 6 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;

			case CTETRA:
				if (i.second.GIDs.size() < 4 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map" << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
								"ElementNumberGridPoints: " << i.second.GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 4 grid points.";
					throw isis::application_exception(errorString.str()); 
				}

				break;
			case CQUAD:
				if (i.second.GIDs.size() < 4 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map" << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
								"ElementNumberGridPoints: " << i.second.GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 4 grid points.";
					throw isis::application_exception(errorString.str()); 
				}
				break;
			case CTRIA3:
				if (i.second.GIDs.size() < 3 )  
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
								"Function:				  NastranDeckHelper::populate_sufaceCornerGridIDs_to_FEAElementIDs_map" << std::endl <<
								"ElementType:			  "	<<  ElementType_ToString(i.second.Type) <<  std::endl <<
								"ElementNumberGridPoints: " << i.second.GIDs.size() <<  std::endl <<
								"Error:		            " << "Must have at least 3 grid points.";
					throw isis::application_exception(errorString.str()); 
				}

				break;
			} // End switch


			std::map<int, std::vector<int>> surfaceID_to_CornerPoints_map;

			i.second.getCornerPointsForEachSurface(surfaceID_to_CornerPoints_map);

			for each ( std::pair<int, std::vector<int>> j in surfaceID_to_CornerPoints_map )
			{
				std::vector<int> key = j.second;
			    std::sort(key.begin(), key.end());
				//if ( sufaceCornerGridIDs_to_FEAElementIDs_map.find(key) == sufaceCornerGridIDs_to_FEAElementIDs_map.end())
				// The corner grid points can be shared between elements.  The only exception would be if the corner grid points
				// are on the outside (i.e. extrior surface) of the part.  
				sufaceCornerGridIDs_to_FEAElementIDs_map[key].insert(i.second.EID);		
			}

		} // for each (

		sufaceCornerGridIDs_to_FEAElementIDs_map_populated = true;
	} //void NastranDeckHelper::populate_S...

	void NastranDeckHelper::findElementsContainingSurface(	const std::vector<int> &in_SurfaceCornerGridIDs, 
															bool &out_ElementFound, 
															std::set<int> &out_ElementIDs )
																					throw (isis::application_exception)
	{
		out_ElementFound = false;
		out_ElementIDs.clear();

		std::vector<int> surfaceCornerGridIDs = in_SurfaceCornerGridIDs;

		std::sort(surfaceCornerGridIDs.begin(), surfaceCornerGridIDs.end());

		if (!sufaceCornerGridIDs_to_FEAElementIDs_map_populated) populate_sufaceCornerGridIDs_to_FEAElementIDs_map();

		const std::map<std::vector<int>, std::set<int>>::const_iterator i = sufaceCornerGridIDs_to_FEAElementIDs_map.find(surfaceCornerGridIDs);

		if ( i != sufaceCornerGridIDs_to_FEAElementIDs_map.end() )
		{
			
			out_ElementFound = true;
			for each ( const int &j in i->second ) out_ElementIDs.insert(j);
		}
	}

	void NastranDeckHelper::getHeatFluxLoadsForBoundarySurfaces( std::vector<HeatFluxLoad> &out_HeatFluxLoads ) 	throw (isis::application_exception)
	{

		out_HeatFluxLoads.clear();

		if (!sufaceCornerGridIDs_to_FEAElementIDs_map_populated) populate_sufaceCornerGridIDs_to_FEAElementIDs_map();
		                                                                

		const std::multimap<int, QBDY3>	&heatFlux_QBDY3_map = nastranDeck.getHeatFlux_QBDY3();
		const std::map<int, CHBDYG> &surfaceElement_CHBDYG_map = nastranDeck.getSurfaceElement_CHBDYG(); 

		for each ( const std::pair<int, QBDY3> &i in  heatFlux_QBDY3_map )
		{
			HeatFluxLoad heatFluxLoad;

			if ( i.second.Q0.find_first_not_of("0123456789.-+eE") != std::string::npos )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"HeatFlux string:               " <<  i.second.Q0		<< std::endl <<
						"Error:                         " << "HeatFlux string must represent a valid floating point number.";
				throw isis::application_exception(errorString.str()); 
			}

			heatFluxLoad.Q0	  = atof(i.second.Q0.c_str());
			heatFluxLoad.SID  = i.second.SID;
			heatFluxLoad.name = i.second.name;

			std::vector<int> CHBDYG_EIDs;
			
			if ( i.second.EID1 != 0 ) CHBDYG_EIDs.push_back(i.second.EID1);
			if ( i.second.EID2 != 0 ) CHBDYG_EIDs.push_back(i.second.EID2);
			if ( i.second.EID3 != 0 ) CHBDYG_EIDs.push_back(i.second.EID3);
			if ( i.second.EID4 != 0 ) CHBDYG_EIDs.push_back(i.second.EID4);
			if ( i.second.EID5 != 0 ) CHBDYG_EIDs.push_back(i.second.EID5);
			if ( i.second.EID6 != 0 ) CHBDYG_EIDs.push_back(i.second.EID6);
			if ( i.second.EID7 != 0 ) CHBDYG_EIDs.push_back(i.second.EID7);
			if ( i.second.EID8 != 0 ) CHBDYG_EIDs.push_back(i.second.EID8);

			for each ( const int &j in CHBDYG_EIDs )
			{
				std::map<int, CHBDYG>::const_iterator k_itr = surfaceElement_CHBDYG_map.find(j);

				if ( k_itr == surfaceElement_CHBDYG_map.end() )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"QBDY3:                         " <<   i.second		<< std::endl <<
						"CHBDYG ID                      " <<  j		<< std::endl <<
						"Error:                         " << "QBDY3 referenced a CHBDYG card, but the CHBDYG was not found in the deck.";
					throw isis::application_exception(errorString.str()); 
				}

				if ( k_itr->second.G1 == 0 || k_itr->second.G2 == 0 || k_itr->second.G2 == 0 )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"QBDY3:                         " <<   i.second		<< std::endl <<
						"CHBDYG ID                      " <<  j		<< std::endl <<
						"Error:		                    " << "CHBDYG does not have at least three grid point IDs defined.";
					throw isis::application_exception(errorString.str()); 
				}

				std::vector<int> key;

				key.push_back(k_itr->second.G1);
				key.push_back(k_itr->second.G2);
				key.push_back(k_itr->second.G3);

				std::sort(key.begin(), key.end());

				const std::map<std::vector<int>, std::set<int>>::const_iterator key_itr = sufaceCornerGridIDs_to_FEAElementIDs_map.find(key);

				if ( key_itr == sufaceCornerGridIDs_to_FEAElementIDs_map.end() )
				{
					std::stringstream errorString;
					errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"QBDY3:                         " <<   i.second		<< std::endl <<
						"CHBDYG ID                      " <<  j		<< std::endl <<
						"Error:                         " << "Could not locate the tetra element containing the face defined by the CHBDYG card.";	
					throw isis::application_exception(errorString.str()); 
				}
				else
				{
					for each ( const int &l in key_itr->second)
					{
						heatFluxLoad.elementIDThatContainsSurface = l;
						heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G1);
						heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G2);
						heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G3);
						if ( k_itr->second.G4 != 0 ) heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G4);
						if ( k_itr->second.G5 != 0 ) heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G5);
						if ( k_itr->second.G6 != 0 ) heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G6);
						if ( k_itr->second.G7 != 0 ) heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G7);
						if ( k_itr->second.G8 != 0 ) heatFluxLoad.surfaceGridPointIDs.push_back(k_itr->second.G8);
				
						out_HeatFluxLoads.push_back(heatFluxLoad);
					}
				}
			} // for each ( const int &j in CHBDYG_EIDs ) 
		}  // for each ( const std::pair<int, QBDY3> &i in  heatFlux_QBDY3_map )
	}

	void NastranDeckHelper::getSurfaceConvectionConstraints ( std::vector<SurfaceConvection> &out_SurfaceConvections ) throw (isis::application_exception)
	{
		if (!sufaceCornerGridIDs_to_FEAElementIDs_map_populated) populate_sufaceCornerGridIDs_to_FEAElementIDs_map();

		out_SurfaceConvections.clear();

		//const std::map<int, SPOINT> &SPOINT_map = nastranDeck.getPoint_SPOINT();
		const std::map<int, PCONV>		&PCONV_map				= nastranDeck.getConvection_PCONV(); 
		const std::map<int, CONV>		&CONV_map				= nastranDeck.getConvection_CONV(); 
		const std::map<int, CHBDYG>		&CHBDYG_map				= nastranDeck.getSurfaceElement_CHBDYG(); 
		const std::map<int, MAT4>		&MAT4_map				= nastranDeck.getMaterialData_MAT4();
        const std::multimap<int, SPC>	&SPC_GridIDKey_map		= nastranDeck.getSpcData_GridIDKey();

		for each ( const std::pair<int, CONV> &conv_ref in CONV_map )
		{
			SurfaceConvection surfaceConvection;
			surfaceConvection.convID = conv_ref.second.EID;

			surfaceConvection.pconvID = conv_ref.second.PCONID;

			if ( conv_ref.second.TA1 == 0 )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					//"Function:                      NastranDeckHelper::getSurfaceConvectionConstraints" << std::endl <<
					"Function:                      " __FUNCTION__ <<  std::endl <<
					"CONV:                          " <<   conv_ref.second		<< std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID	<< std::endl <<
					"SPOINTID:                      " <<  conv_ref.second.TA1	<< std::endl <<
					"Error:                         " << "SPOINTID not set. This is the node in space that is used to define the ambient temperature.";	
				throw isis::application_exception(errorString.str()); 
			}

			surfaceConvection.spointID = conv_ref.second.TA1;

			// Find Temperature associated with surfaceConvection.spointID

			std::pair <std::multimap<int,SPC>::const_iterator, std::multimap<int,SPC>::const_iterator> sPC_pairs;

			sPC_pairs = SPC_GridIDKey_map.equal_range(surfaceConvection.spointID);
			int count = 0;
			for ( std::multimap<int,SPC>::const_iterator spc_itr = sPC_pairs.first; spc_itr != sPC_pairs.second; ++spc_itr )
			{
				++count;
				surfaceConvection.ambientTemperature = atof(spc_itr->second.D1.c_str());
			}

			if ( count == 0 )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		<< std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID	<< std::endl <<
					"SPOINTID:                      " <<  conv_ref.second.TA1	<< std::endl <<
					"Error:                         " << "SPC card error.  SPC card with point == SPOINTID not found.";	
				throw isis::application_exception(errorString.str()); 
			}


			if ( count > 1 )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		<< std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID	<< std::endl <<
					"SPOINTID:                      " <<  conv_ref.second.TA1	<< std::endl <<
					"Error:                         " << "SPC card error.  More that one SPC cards with point == SPOINTID found.";	
				throw isis::application_exception(errorString.str()); 
			}


			const std::map<int, PCONV>::const_iterator pconv_itr = PCONV_map.find( conv_ref.second.PCONID );

			if ( pconv_itr == PCONV_map.end() )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		<< std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID	<< std::endl <<
					"Error:                         " << "Could not locate PCONID card.";	
				throw isis::application_exception(errorString.str()); 
			}
		
			surfaceConvection.mat4ID = pconv_itr->second.MID;

			const std::map<int, MAT4>::const_iterator mat4_itr = MAT4_map.find(pconv_itr->second.MID);

			if ( mat4_itr == MAT4_map.end() )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		  << std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID  << std::endl <<
					"MAT4ID                         " <<  pconv_itr->second.MID   << std::endl <<
					"Error:                         " << "Could not locate MAT4 card.";	
				throw isis::application_exception(errorString.str()); 
			}

			if (mat4_itr->second.H.find_first_not_of("0123456789.-+eE") != std::string::npos )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		  << std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID  << std::endl <<
					"MAT4ID                         " <<  pconv_itr->second.MID   << std::endl <<
					"MAT4 H ()                      " << mat4_itr->second.H  << std::endl <<
					"Error:                         " << "MAT4 H (convectionCoefficient) could not be converted to a float point number.";	
				throw isis::application_exception(errorString.str()); 
			}

			surfaceConvection.convectionCoefficient = atof(mat4_itr->second.H.c_str());
			
			std::map<int, CHBDYG>::const_iterator chbdyg_itr = CHBDYG_map.find( conv_ref.second.EID);  // EID is Surface element identification number.

			if ( chbdyg_itr == CHBDYG_map.end() )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
					"Function:                      " __FUNCTION__ << std::endl <<
					"CONV:                          " <<   conv_ref.second		  << std::endl <<
					"PCONID                         " <<  conv_ref.second.PCONID  << std::endl <<
					"MAT4ID                         " <<  pconv_itr->second.MID   << std::endl <<
					"MAT4 H ()                      " << mat4_itr->second.H  << std::endl <<
					"CHBDYG ID                      " << conv_ref.second.EID  << std::endl <<
					"Error:                         " << "CHBDYG ID card could not be found.";	
				throw isis::application_exception(errorString.str()); 
			}

			std::vector<int> key;

			key.push_back(chbdyg_itr->second.G1);
			key.push_back(chbdyg_itr->second.G2);
			key.push_back(chbdyg_itr->second.G3);

			std::sort(key.begin(), key.end());

			const std::map<std::vector<int>, std::set<int>>::const_iterator key_itr = sufaceCornerGridIDs_to_FEAElementIDs_map.find(key);

			if ( key_itr == sufaceCornerGridIDs_to_FEAElementIDs_map.end() )
			{
				std::stringstream errorString;
				errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
				"Function:                      " __FUNCTION__ << std::endl <<
				"CONV:                          " <<   conv_ref.second		  << std::endl <<
				"PCONID                         " <<  conv_ref.second.PCONID  << std::endl <<
				"MAT4ID                         " <<  pconv_itr->second.MID   << std::endl <<
				"MAT4 H ()                      " << mat4_itr->second.H  << std::endl <<
				"CHBDYG ID                      " << conv_ref.second.EID  << std::endl <<
				"Error:                         " << "Could not locate the tetra element containing the face defined by the CHBDYG card.";	
				throw isis::application_exception(errorString.str()); 
			}
			else
			{
				for each ( const int &l in key_itr->second )
				{
					surfaceConvection.elementIDThatContainsSurface = l;
					surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G1);
					surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G2);
					surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G3);
					if ( chbdyg_itr->second.G4 != 0 ) surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G4);
					if ( chbdyg_itr->second.G5 != 0 ) surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G5);
					if ( chbdyg_itr->second.G6 != 0 ) surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G6);
					if ( chbdyg_itr->second.G7 != 0 ) surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G7);
					if ( chbdyg_itr->second.G8 != 0 ) surfaceConvection.surfaceGridPointIDs.push_back(chbdyg_itr->second.G8);
				
					out_SurfaceConvections.push_back(surfaceConvection);
				}
			}

		} // for each ( const std::pair<int, CONV> &conv_ref in CONV_map )

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		This function finds the face that contains in_SearchForGridPoints and returns the complete set of grid points for 
	//		that face.  For example, for a Tetra elements with 4 nodes, then the face points would always consist of 3 nodes.
	//		However, for a 10 Tetra element, the face nodes would consist of 6 nodes.
	//
	//		Supported elements: CTETRA elments only 
	//
	//		WARNING -	in_SearchForCornerGridPoints must be a valid face in in_FEAElement.  Don't call this function
	//					unless you know that is true.  An exeption will be thrown
	//
	//	Pre-Conditions:
	//		None
	//		
	//	Post-Conditons
	//
	//	if (in_FEAElement.Type != CTETRA )
	//		throw isis::application_exception
	//  if ( in_SearchForCornerGridPoints.size() != 4 )
	//		throw isis::application_exception			
	//	if ( in_FEAElement.GIDs.size() < 4 )
	//		throw isis::application_exception	
	//	if face not found
	//		throw isis::application_exception	
	//  if no exceptions
	//		return out_FaceGridPoints_IncludingMidpoints where the order of the points are the order as they would appear in the
	//		Nastran CTETRA card.
	void FindFaceContainingCornerGridPoints_ReturnAllGridPointsForFace_CTETRA(	
																const std::set<int>		&in_SearchForCornerGridPoints,
																const FEAElement		&in_FEAElement,
																std::vector<int>		&out_FaceGridPoints_IncludingMidpoints ) 
																			throw (isis::application_exception)
	{

		if ( in_FEAElement.Type != CTETRA )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:    "	<< __FUNCTION__ << std::endl <<
						"ElementType: "	<<  ElementType_ToString(in_FEAElement.Type) <<  std::endl <<
						"Error:       " << "Only CTETRA element supported by this function.";
			throw isis::application_exception(errorString.str()); 
		}


		 std::vector<int> elementCornerPoints;
		 in_FEAElement.getElementCornerPoints(elementCornerPoints);

		if (elementCornerPoints.size() != 4 )
		{
			std::stringstream errorString;
			errorString <<  "Erroneous input" << std::endl <<
						"Function:     " <<   __FUNCTION__ << std::endl <<
						"ElementType:  " <<  ElementType_ToString(in_FEAElement.Type) <<  std::endl <<
						"ElementID:    " <<   in_FEAElement.EID	  << std::endl <<
						"in_SearchForCornerGridPoints size: " << in_SearchForCornerGridPoints.size() << std::endl <<
						"Error:        " << "in_SearchForCornerGridPoints size must be 4.";
			throw isis::application_exception(errorString.str()); 
		}


		if ( in_FEAElement.GIDs.size() < 4 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
			"Function:            " __FUNCTION__ << std::endl <<
			"ElementID:           " <<   in_FEAElement.EID	  << std::endl <<
			"Element Grid Points: ";
			for each ( const int &i_grid in in_FEAElement.GIDs) errorString << " " <<  i_grid;
			errorString <<
			"Search for Grid Points:  ";
			for each ( const int &i_search in in_SearchForCornerGridPoints )errorString << " " <<  i_search;
			errorString <<
			"Error:                         " << "\"Element Grid Points\" must contain at least four grid points.";	
			throw isis::application_exception(errorString.str()); 

		}

		out_FaceGridPoints_IncludingMidpoints.clear();

		// Could be one of four faces
		// Face Keys Numbering from 1 ( Grid points as numberd by Nastran
		// 1 2 3
		// 1 2 4
		// 2 3 4
		// 1 3 4

		// Face Keys Numbering from 0 
		// 0 1 2
		// 0 1 3
		// 1 2 3
		// 0 2 3

		std::set<int> intersect_set;
		for each ( int i in in_SearchForCornerGridPoints) intersect_set.insert(i);

		std::set<int> faceKey_1_set;
		faceKey_1_set.insert( in_FEAElement.GIDs[0] );
		faceKey_1_set.insert( in_FEAElement.GIDs[1] );
		faceKey_1_set.insert( in_FEAElement.GIDs[2] );

		std::set<int> faceKey_2_set;
		faceKey_2_set.insert( in_FEAElement.GIDs[0] );
		faceKey_2_set.insert( in_FEAElement.GIDs[1] );
		faceKey_2_set.insert( in_FEAElement.GIDs[3] );

		std::set<int> faceKey_3_set;
		faceKey_3_set.insert( in_FEAElement.GIDs[1] );
		faceKey_3_set.insert( in_FEAElement.GIDs[2] );
		faceKey_3_set.insert( in_FEAElement.GIDs[3] );

		std::set<int> faceKey_4_set;
		faceKey_4_set.insert( in_FEAElement.GIDs[0] );
		faceKey_4_set.insert( in_FEAElement.GIDs[2] );
		faceKey_4_set.insert( in_FEAElement.GIDs[3] );


		if ( faceKey_1_set == intersect_set)
		{
			// First Face Match
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[0]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[1]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[2]);
			if (  in_FEAElement.GIDs.size() == 10 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[4]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[5]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[6]);
			}

		}
		else if ( faceKey_2_set == intersect_set)
		{
			// Second Face Match
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[0]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[1]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[3]);
			if (  in_FEAElement.GIDs.size() == 10 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[4]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[8]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[7]);
			}
		}
		else if ( faceKey_3_set == intersect_set)
		{
			// Third Face Match
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[1]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[2]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[3]);
			if (  in_FEAElement.GIDs.size() == 10 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[5]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[9]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[8]);
			}

		}
		else if ( faceKey_4_set == intersect_set)
		{
			// Fourth Face Match
			// Third Face Match
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[0]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[2]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[3]);
			if (  in_FEAElement.GIDs.size() == 10 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[6]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[9]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[7]);
			}
		}	

		if ( out_FaceGridPoints_IncludingMidpoints.size() == 0 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
			"Function:            " __FUNCTION__ << std::endl <<
			"ElementID:           " <<   in_FEAElement.EID	 << std::endl <<
			"Element Grid Points: ";
			for each ( const int &i_grid in in_FEAElement.GIDs) errorString << " " <<  i_grid;
			errorString <<
			"Search for Grid Points:          ";
			for each ( const int &i_search in in_SearchForCornerGridPoints )errorString << " " <<  i_search;
			errorString <<  std::endl <<
			"Error:               " << "Could not locate \"Search for Grid Points\" in \"Element Grid Points\".";	
			throw isis::application_exception(errorString.str()); 
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		This function finds the face that contains in_SearchForGridPoints and returns the complete set of grid points for 
	//		that face.  For example, for a CQUAD elements with 4 nodes, then the face points would always consist of 4 nodes.
	//		However, for a 8/9 CQUAD elements, the face nodes would consist of 8/9 nodes.  Note - the 9th node is an optional 
	//		center point node.
	//
	//		Supported elements: CQUAD elments only 
	//
	//		WARNING -	in_SearchForCornerGridPoints must be a valid face in in_FEAElement.  Don't call this function
	//					unless you know that is true.  An exeption will be thrown
	//
	//	Pre-Conditions:
	//		None
	//		
	//	Post-Conditons
	//
	//	if (in_FEAElement.Type != CQUAD )
	//		throw isis::application_exception
	//  if ( in_SearchForCornerGridPoints.size() != 4 )
	//		throw isis::application_exception			
	//	if ( in_FEAElement.GIDs.size() < 4 )
	//		throw isis::application_exception	
	//	if face not found
	//		throw isis::application_exception	
	//  if no exceptions
	//		return out_FaceGridPoints_IncludingMidpoints where the order of the points are the order as they would appear in the
	//		Nastran CTETRA card.
	void FindFaceContainingCornerGridPoints_ReturnAllGridPointsForFace_CQUAD(	
																const std::set<int>		&in_SearchForCornerGridPoints,
																const FEAElement		&in_FEAElement,
																std::vector<int>		&out_FaceGridPoints_IncludingMidpoints ) 
																			throw (isis::application_exception)
	{

		if ( in_FEAElement.Type != CQUAD )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:				"	<< __FUNCTION__ << std::endl <<
						"ElementType:			"	<<  ElementType_ToString(in_FEAElement.Type) <<  std::endl <<
						"Error:		            " << "Only CQUAD element supported by this function.";
			throw isis::application_exception(errorString.str()); 
		}

		if (in_SearchForCornerGridPoints.size() != 4 )
		{
			std::stringstream errorString;
			errorString <<  "Erroneous input"		<< std::endl <<
						"Function:				      "	<< __FUNCTION__ << std::endl <<
						"ElementType:			      "	<<  ElementType_ToString(in_FEAElement.Type) <<  std::endl <<
						"ElementID:                   " <<   in_FEAElement.EID	  << std::endl <<
						"in_SearchForCornerGridPoints size: " << in_SearchForCornerGridPoints.size() << std::endl <<
						"Error:		                  " << "in_SearchForCornerGridPoints size must be 4.";
			throw isis::application_exception(errorString.str()); 
		}


		if ( in_FEAElement.GIDs.size() < 4 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
			"Function:                      " __FUNCTION__ << std::endl <<
			"ElementID:                     " <<   in_FEAElement.EID	  << std::endl <<
			"Element Grid Points:          ";
			for each ( const int &i_grid in in_FEAElement.GIDs) errorString << " " <<  i_grid;
			errorString <<
			"Search for Grid Points:          ";
			for each ( const int &i_search in in_SearchForCornerGridPoints )errorString << " " <<  i_search;
			errorString <<
			"Error:                         " << "\"Element Grid Points\" must contain at least four grid points.";	
			throw isis::application_exception(errorString.str()); 

		}


		out_FaceGridPoints_IncludingMidpoints.clear();

		// Only one face on a CQuad
		// Face Keys Numbering from 1 ( Grid points as numberd by Nastran
		// 1 2 3 4


		// Face Keys Numbering from 0 
		// 0 1 2 3

		std::set<int> intersect_set;
		for each ( int i in in_SearchForCornerGridPoints) intersect_set.insert(i);

		std::set<int> faceKey_1_set;
		faceKey_1_set.insert( in_FEAElement.GIDs[0] );
		faceKey_1_set.insert( in_FEAElement.GIDs[1] );
		faceKey_1_set.insert( in_FEAElement.GIDs[2] );
		faceKey_1_set.insert( in_FEAElement.GIDs[3] );


		if ( faceKey_1_set == intersect_set)
		{
			// Only match possible
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[0]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[1]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[2]);
			out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[3]);

			if (  in_FEAElement.GIDs.size() == 8 || in_FEAElement.GIDs.size() == 9 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[4]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[5]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[6]);
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[7]);
			}
			if (  in_FEAElement.GIDs.size() == 9 )
			{
				out_FaceGridPoints_IncludingMidpoints.push_back( in_FEAElement.GIDs[8]);
			}

		}


		if ( out_FaceGridPoints_IncludingMidpoints.size() == 0 )
		{
			std::stringstream errorString;
			errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
			"Function:                      " __FUNCTION__ << std::endl <<
			"ElementID:                     " <<   in_FEAElement.EID	 << std::endl <<
			"Element Grid Points:          ";
			for each ( const int &i_grid in in_FEAElement.GIDs) errorString << " " <<  i_grid;
			errorString <<
			"Search for Grid Points:          ";
			for each ( const int &i_search in in_SearchForCornerGridPoints )errorString << " " <<  i_search;
			errorString <<
			"Error:                         " << "Could not locate \"Search for Grid Points\" in \"Element Grid Points\".";	
			throw isis::application_exception(errorString.str()); 
		}

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void NastranDeckHelper::getSurfaceElementsContainingGridPoints (	
														const std::set<int>						&in_GridPointIDs, 
														std::multimap< int, std::vector<int>>	&out_ElementID_to_SurfacePoints_map )
																							throw (isis::application_exception)
	{
	
		const std::map<int, FEAElement> &elementData_map = nastranDeck.getElementData();

		if (!sufaceCornerGridIDs_to_FEAElementIDs_map_populated) populate_sufaceCornerGridIDs_to_FEAElementIDs_map();	

		//std::map<std::vector<int>, int> sufaceCornerGridIDs_to_FEAElementIDs_map
		for each ( const std::pair<std::vector<int>, std::set<int>> &i in sufaceCornerGridIDs_to_FEAElementIDs_map )
		{
			std::set<int> elementCornerPoints_set;
			for each ( const int &j in i.first ) elementCornerPoints_set.insert(j);
			std::set<int> intersect_set;
			std::set_intersection(	in_GridPointIDs.begin(),			in_GridPointIDs.end(),
									elementCornerPoints_set.begin(),	elementCornerPoints_set.end(),
									std::inserter(intersect_set,intersect_set.begin()));


			if ( intersect_set.size() == elementCornerPoints_set.size() )
			{
				// Found an element with all the corner points in in_GridPointIDs
				// Need to find the complete set of grid points for the applicable face

				for each ( const int &j in i.second )
				{

					const std::map<int, FEAElement>::const_iterator elem_itr = elementData_map.find(j);

					if ( elem_itr == elementData_map.end())
					{
						std::stringstream errorString;
						errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"ElementID:                     " <<   j		  << std::endl <<
						"Error:                         " << "Could not locate an element identified with ElementID.";	
						throw isis::application_exception(errorString.str()); 
					}
				
					std::vector<int> faceGridPoints;

					if ( elem_itr->second.Type != CTETRA &&  elem_itr->second.Type != CQUAD )
					{
						std::stringstream errorString;
						errorString <<  "Nastran deck contains erroneous data."		<< std::endl <<
						"Function:                      " __FUNCTION__ << std::endl <<
						"ElementID:                     " <<   j		  << std::endl <<
						"ElementType					" << ElementType_ToString(elem_itr->second.Type)  << std::endl <<
						"Error:                         " << "Only CTETRA and CQUAD element types are supported";	
						throw isis::application_exception(errorString.str()); 
					}


					if ( elem_itr->second.Type == CTETRA )
					{
						// Returns conrner points and middle points for the face in the order of the CTETRA card
						FindFaceContainingCornerGridPoints_ReturnAllGridPointsForFace_CTETRA(	elementCornerPoints_set,
																								elem_itr->second,
																								faceGridPoints );
						//std::cout << std::endl;
						//std::cout << std::endl << "After FindFaceContainingCornerGridPoints_ReturnAllGridPointsForFace_CTETRA";
						//std::cout << std::endl << "   Element Corner Points:";
						//for each ( const int &pt in elementCornerPoints_set ) std::cout << std::endl << "      " << pt; 
						//std::cout << std::endl << "   Element Data:";
						//std::cout << std::endl << elem_itr->second;
						//std::cout << std::endl << "   Element Face Points:";
						//for each ( const int &pt in faceGridPoints ) std::cout << std::endl << "      " << pt; 
						//std::cout << std::endl;
					}

					if ( elem_itr->second.Type == CQUAD )
					{
						// Returns conrner points and middle points for the face in the order of the CQUAD card
						FindFaceContainingCornerGridPoints_ReturnAllGridPointsForFace_CQUAD(	elementCornerPoints_set,
																								elem_itr->second,
																								faceGridPoints );
			

					}
				
					if ( faceGridPoints.size() > 0 )
					{
						out_ElementID_to_SurfacePoints_map.insert(std::pair<int, std::vector<int>>(elem_itr->first, faceGridPoints));
					}
				}


			} // END if ( intersect_set.size() == elementCornerPoints_set.size() )

		} // for each ( const std::pair<std::vector<int>, int> &i
	} // NastranDeckHelper::getSurfaceElementsContainingGridPoints

	void NastranDeckHelper::getVolumetricHeatGenerations( std::vector<VolumetricHeatGeneration> &out_VolumetricHeatGenerations )
	{

		const std::multimap<int, QVOL>  &heatVolume_QVOL_map = nastranDeck.getHeatVolume_QVOL();

		std::map<double, VolumetricHeatGeneration> heatGenerationValue_to_volumetricHeatGeneration_map;	

		for each ( const std::pair<int, QVOL> &i in heatVolume_QVOL_map)
		{
			double value = atof(i.second.QVOL_Value.c_str());
			if ( i.second.EID1 != 0 ) heatGenerationValue_to_volumetricHeatGeneration_map[value].elementIDs.push_back(i.second.EID1);
			if ( i.second.EID2 != 0 ) heatGenerationValue_to_volumetricHeatGeneration_map[value].elementIDs.push_back(i.second.EID2);
			if ( i.second.EID3 != 0 ) heatGenerationValue_to_volumetricHeatGeneration_map[value].elementIDs.push_back(i.second.EID3);
			if ( i.second.EID4 != 0 ) heatGenerationValue_to_volumetricHeatGeneration_map[value].elementIDs.push_back(i.second.EID4);
			if ( i.second.EID5 != 0 ) heatGenerationValue_to_volumetricHeatGeneration_map[value].elementIDs.push_back(i.second.EID5);			
		}

		for each ( const std::pair<double, VolumetricHeatGeneration> &i in heatGenerationValue_to_volumetricHeatGeneration_map)
		{
			VolumetricHeatGeneration volumetricHeatGeneration_temp;
			volumetricHeatGeneration_temp.powerInputPerVolume = i.first;
			volumetricHeatGeneration_temp.elementIDs = i.second.elementIDs;
			out_VolumetricHeatGenerations.push_back(volumetricHeatGeneration_temp);
		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ElementType_ToString( ElementType in_ElementType)
	{
		switch ( in_ElementType ) 
		{
			case CHEXA:
				return "CHEXA";
				break;
			case CPENTA:
				return "CPENTA";
				break;
			case CTETRA:
				return "CTETRA";
				break;
			case CQUAD:
				return "CQUAD";
				break;
			case CTRIA3:
				return "CTRIA3";
				break;
			default:
				return "";
		}
	}



} // End namespace isis