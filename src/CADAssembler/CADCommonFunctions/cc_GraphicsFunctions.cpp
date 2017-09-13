#include "cc_GraphicsFunctions.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <limits>
#include <iomanip>
#include <algorithm> 
#include <sstream>
#include <map>
#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace isis_CADCommon
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const std::vector<double> &in_Vector )
	{
		for ( std::vector<double>::const_iterator i(in_Vector.begin()); i != in_Vector.end(); ++i )
		{
			output << "  " << *i;
		}
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const std::vector<int> &in_Vector )
	{
		for ( std::vector<int>::const_iterator i(in_Vector.begin()); i != in_Vector.end(); ++i )
		{
			output << "  " << *i;
		}
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const Point_2D &in_Point )
	{
		output << "Point (x,y): " << in_Point.x << "  " <<  in_Point.y;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const Point_3D &in_Point )
	{
		output << "Point (x,y,z): " << in_Point.x << "  " <<  in_Point.y << "  " << in_Point.z;
		return output;
	}

	std::ostream& operator<<(std::ostream &output,  const Line_2D &in_Line )
	{
		output << "Line: Start " <<  in_Line.startPoint << "  End " << in_Line.endPoint;
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const std::vector<Line_2D> &in_Lines )
	{

		for ( std::vector<Line_2D>::const_iterator i(in_Lines.begin()); i != in_Lines.end(); ++i )
		{
			output << std::endl << *i;

		}
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output,  const std::vector<Point_3D> &in_Points )
	{

		for ( std::vector<Point_3D>::const_iterator i(in_Points.begin()); i != in_Points.end(); ++i )
		{
			output << std::endl << *i;

		}
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const std::map<int, Point_3D> &in_Point_3D_map )
	{
		for ( std::map<int, Point_3D>::const_iterator i(in_Point_3D_map.begin()); i != in_Point_3D_map.end(); ++i )
		{
			output << std::endl << "ID: " << i->first << "  " <<  i->second;

		}
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool ValueAreEqual(	double in_Value_1, 
						double in_Value_2, 
						double in_Precision )
	{
		if ( std::abs( in_Value_1 - in_Value_2) <= in_Precision )
			return true;
		else
			return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////		
	bool Point_3D::operator==( const Point_3D &in_Point_3D ) const
	{
		if ( in_Point_3D.x == x && 
			 in_Point_3D.y == y &&
			 in_Point_3D.z == z )
			return true;
		else
			return false;
	}

	bool Point_3D::operator<( const Point_3D &in_Point_3D ) const
	{
		if ( x != in_Point_3D.x ) return ( x < in_Point_3D.x );
		if ( y != in_Point_3D.y ) return ( y < in_Point_3D.y );
		if ( z != in_Point_3D.z ) 
			return ( z < in_Point_3D.z );
		else
			return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const GridPoint &in_Grid )
	{
		output << std::endl << "Grid, ID: " << in_Grid.ID << "  Location CS ID: " << in_Grid.locationCoordinateSystem_ID <<
							    "  x coord: " << in_Grid.point.x <<
								"  y coord: " << in_Grid.point.y <<
								"  z coord: " << in_Grid.point.z <<
								"  Displacement CS ID: " << in_Grid.displacementCoordinateSystem_ID;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool  PointsAreEqual_2D(  const Point_2D &in_Point_1, const Point_2D &in_Point_2, double in_Tolerance )
	{

		if (  ValueAreEqual(in_Point_1.x, in_Point_2.x, in_Tolerance) &&
			  ValueAreEqual(in_Point_1.y, in_Point_2.y, in_Tolerance) )
			return true;
		else 
			return false;

//		if ( ( std::abs( in_Point_1.x - in_Point_2.x) <= in_Tolerance ) &&
//			 ( std::abs( in_Point_1.y - in_Point_2.y) <= in_Tolerance ) )
//			return true;
//		else 
//			return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool  PointsAreEqual_3D(  const Point_3D &in_Point_1, 
							  const Point_3D &in_Point_2, 
							  double in_Tolerance )
	{
		if (  ValueAreEqual(in_Point_1.x, in_Point_2.x, in_Tolerance) &&
			  ValueAreEqual(in_Point_1.y, in_Point_2.y, in_Tolerance) &&
			  ValueAreEqual(in_Point_1.z, in_Point_2.z, in_Tolerance) )
			return true;
		else 
			return false;

//		if ( ( std::abs( in_Point_1.x - in_Point_2.x) <= in_Tolerance ) &&
//			 ( std::abs( in_Point_1.y - in_Point_2.y) <= in_Tolerance ) && 
//			 ( std::abs( in_Point_1.z - in_Point_2.z) <= in_Tolerance ))
//			return true;
//		else 
//			return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	double DotProduct_2D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception)
	{
		if ( in_Vector_1.size() != 2 || in_Vector_2.size() != 2 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - DotProduct_2D, received a vector that does not have two and only two elements." <<
				"  in_Vector_1: " << in_Vector_1 << 
				"  in_Vector_2: " << in_Vector_2;
			throw isis::application_exception(errorString.str());
		}
		std::vector<double> outVector(2);

		return in_Vector_1[0] * in_Vector_2[0] + in_Vector_1[1] * in_Vector_2[1];
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	double DotProduct_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception)
	{
		if ( in_Vector_1.size() != 3 || in_Vector_2.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - DotProduct_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector_1: " << in_Vector_1 << 
				"  in_Vector_2: " << in_Vector_2;
			throw isis::application_exception(errorString.str());
		}
		std::vector<double> outVector(3);


		return in_Vector_1[0] * in_Vector_2[0]   +   in_Vector_1[1] * in_Vector_2[1]   +   in_Vector_1[2] * in_Vector_2[2];
	}


	std::vector<double> CrossProduct_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception)
	{ 
		if ( in_Vector_1.size() != 3 || in_Vector_2.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - CrossProduct_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector_1: " << in_Vector_1 << 
				"  in_Vector_2: " << in_Vector_2;
			throw isis::application_exception(errorString.str());
		}

		std::vector<double> outVector(3);
		outVector[0] =   in_Vector_1[1] * in_Vector_2[2] - in_Vector_1[2] * in_Vector_2[1];
		outVector[1] = -(in_Vector_1[0] * in_Vector_2[2] - in_Vector_1[2] * in_Vector_2[0]);
		outVector[2] =   in_Vector_1[0] * in_Vector_2[1] - in_Vector_1[1] * in_Vector_2[0]; 

		return outVector;
	}


	double Magnitude_3D( const std::vector<double> &in_Vector ) throw (isis::application_exception)
	{
		if ( in_Vector.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - Magnitude_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector: " << in_Vector; 
			throw isis::application_exception(errorString.str());
		}

		std::vector<double> outVector(3);

		return sqrt( pow(in_Vector[0], 2) + pow(in_Vector[1], 2) + pow(in_Vector[2], 2) ) ;
	}

	double Magnitude_2D( const std::vector<double> &in_Vector ) throw (isis::application_exception)
	{
		if ( in_Vector.size() != 2 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - Magnitude_2D, received a vector that does not have two and only two elements." <<
				"  in_Vector: " << in_Vector; 
			throw isis::application_exception(errorString.str());
		}

		std::vector<double> outVector(3);

		return sqrt( pow(in_Vector[0], 2) + pow(in_Vector[1], 2) ) ;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<double> UnitVector_3D( const std::vector<double> &in_Vector ) throw (isis::application_exception)
	{
		if ( in_Vector.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - UnitVector_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector: " << in_Vector; 
			throw isis::application_exception(errorString.str());
		}

		std::vector<double> outVector(3);

		double magnitude = abs(Magnitude_3D(in_Vector));

		if ( magnitude == 0 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - UnitVector_3D, received a vector that has a magnitude of zero." <<
				"  in_Vector: " << in_Vector; 
			throw isis::application_exception(errorString.str());
		}
		else
		{
			outVector[0] = in_Vector[0] / magnitude;
			outVector[1] = in_Vector[1] / magnitude;
			outVector[2] = in_Vector[2] / magnitude;
		}

		return outVector;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	double AngleBetweenVectors_3D(  const std::vector<double> &in_A, const std::vector<double> &in_B) throw (isis::application_exception)
	{
		if ( in_A.size() != 3  || in_B.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - AngleBetweenVectors_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector_A: " << in_A << 
				"  in_Vector_B: " << in_B;
			throw isis::application_exception(errorString.str());
		}

		//angle = cos-1( ( A . B )/ ( |A| |B| )

		double  magnitude_a = Magnitude_3D(in_A);
		double  magnitude_b = Magnitude_3D(in_B);

		if ( magnitude_a == 0 || magnitude_b == 0 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - AngleBetweenVectors_3D, received a vector that has a magnitude of zero." <<
				"  in_Vector_A: " << in_A << 
				"  in_Vector_B: " << in_B;
			throw isis::application_exception(errorString.str());
		}
		else
			return std::acos( DotProduct_3D( in_A, in_B ) / ( magnitude_a * magnitude_b ) );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool VectorsEqual_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2, double in_Tolerance )
	{

		if ( in_Vector_1.size() != 3  || in_Vector_2.size() != 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - VectorsEqual_3D, received a vector that does not have three and only three elements." <<
				"  in_Vector_1: " << in_Vector_1 << 
				"  in_Vector_2: " << in_Vector_2;
			throw isis::application_exception(errorString.str());
		}

		//if ( std::abs( in_Vector_1[0] - in_Vector_2[0] ) < in_Tolerance   &&
		//	 std::abs( in_Vector_1[1] - in_Vector_2[1] ) < in_Tolerance   &&
		//	 std::abs( in_Vector_1[2] - in_Vector_2[2] ) < in_Tolerance  )
		if ( ValueAreEqual(in_Vector_1[0], in_Vector_2[0],in_Tolerance) &&
			 ValueAreEqual(in_Vector_1[1], in_Vector_2[1],in_Tolerance) &&
			 ValueAreEqual(in_Vector_1[2], in_Vector_2[2],in_Tolerance)  )
			return true;
		else
			return false;

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool CoordinateWithinRange( double in_Range_1, 
								double in_Range_2, 
								double in_Cord, 
								bool   in_IncludeValuesOnBoundary,
								double in_Tolerance )
	{

		if ( in_IncludeValuesOnBoundary )
		{
			if ( in_Range_1 < in_Range_2 )
				return (( in_Cord >= ( in_Range_1 - in_Tolerance ) ) &&  ( in_Cord <= ( in_Range_2 + in_Tolerance)));
			else
				return (( in_Cord >= ( in_Range_2 - in_Tolerance ) ) &&  ( in_Cord <= ( in_Range_1 + in_Tolerance)));
		}
		else
		{
			if ( in_Range_1 < in_Range_2 )
				return (( in_Cord > ( in_Range_1 + in_Tolerance ) ) &&  ( in_Cord < ( in_Range_2 - in_Tolerance)));
			else
				return (( in_Cord > ( in_Range_2 + in_Tolerance ) ) &&  ( in_Cord < ( in_Range_1 - in_Tolerance)));
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool DuplicatePointInList( const std::list< Point_3D> &in_Points,  double in_Tolerance )
	{
		std::list< Point_3D> points(in_Points);
		points.sort();
		std::list< Point_3D>::const_iterator j; 
		for ( std::list< Point_3D>::const_iterator i(points.begin()); i != points.end(); ++ i )
		{
			j = i;
			++j;
			if ( j != points.end() )
			{
				//if ( ( std::abs( i->x - j->x) <= in_Tolerance ) &&
				//	 ( std::abs( i->y - j->y) <= in_Tolerance ) &&
				//	 ( std::abs( i->z - j->z) <= in_Tolerance ) ) return true;
				if ( ValueAreEqual( i->x, j->x, in_Tolerance ) &&
					 ValueAreEqual( i->y, j->y, in_Tolerance ) &&
					 ValueAreEqual( i->z, j->z, in_Tolerance ) ) return true;
			}
		}
			
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool PointOnLine_2D( const Point_2D &in_Point, const Line_2D &in_Line, double in_Tolerance )
	{

		//std::cout << std::endl << "PointOnLine_2D: in_Tolerance: " << in_Tolerance;
		//std::cout << std::endl << "PointOnLine_2D: in_Point" << in_Point;
		//std::cout << std::endl << "PointOnLine_2D: in_Line" << in_Line;

		// Determine if in_Point is on in_Line's start or endpoint.
		if ( PointsAreEqual_2D( in_Point, in_Line.startPoint, in_Tolerance) ||
			 PointsAreEqual_2D( in_Point, in_Line.endPoint, in_Tolerance) )
		{
			return true;
		}

		// Check for zero length in_Line
		if ( PointsAreEqual_2D( in_Line.startPoint, in_Line.endPoint, in_Tolerance) )
		{
			// We know in_Point is not on in_Line's start or endpoint; therefore, if in_line
			// is zero length, then in_Point is not on the line.
			return false;
		}

		// Verify the slopes are identical, relative to the first point in the line.
		double slopeLine;
		double slopeToPoint;

		bool slopeLine_Infinite;
		bool slopeToPoint_Infinite;

		// Must handle the case of a vertical line (i.e. infinite slope )
		//if ( ( std::abs(in_Line.endPoint.x -  in_Line.startPoint.x )) <=  in_Tolerance )
		if ( ValueAreEqual(in_Line.endPoint.x, in_Line.startPoint.x, in_Tolerance ) )
		{
			slopeLine = std::numeric_limits<double>::max();
			slopeLine_Infinite = true;
		}
		else
		{
			slopeLine =    ( in_Line.endPoint.y -  in_Line.startPoint.y ) / ( in_Line.endPoint.x -  in_Line.startPoint.x );
			slopeLine_Infinite = false;
		}
	
		//if ( std::abs((in_Point.x -  in_Line.startPoint.x )) <=  in_Tolerance )
		if ( ValueAreEqual(in_Point.x, in_Line.startPoint.x, in_Tolerance ))
		{
			slopeToPoint = std::numeric_limits<double>::max();
			slopeToPoint_Infinite = true;
		}
		else
		{
			slopeToPoint = ( in_Point.y         -  in_Line.startPoint.y ) / ( in_Point.x         -  in_Line.startPoint.x );
			slopeToPoint_Infinite = false;
		}

		//std::cout << std::endl << "Slopes Line Point: " << slopeLine << "  " << slopeToPoint;

		//if ( std::abs(slopeLine - slopeToPoint) >= in_Tolerance ) return false;  // slopes don't match
		if ( !ValueAreEqual(slopeLine, slopeToPoint, in_Tolerance )) return false;  // slopes don't match

		// Check if offsets (i.e. b) match    y = mx + b;   b = y - mx;
		if ( ! (slopeLine_Infinite && slopeToPoint_Infinite) ) // if they both have infinite slope, offsets match
		{
			double line_b        = in_Line.startPoint.y - slopeLine    * in_Line.startPoint.x;
			double lineToPoint_b = in_Point.y           - slopeToPoint * in_Point.x;
			//if ( std::abs(line_b - lineToPoint_b) >= in_Tolerance ) return false;  // offsets don't match
			if ( !ValueAreEqual(line_b, lineToPoint_b, in_Tolerance )) return false;  // offsets don't match
		}


		// Now we know that the point is on the line, must check if the point is within line segment.
		if ( CoordinateWithinRange(in_Line.startPoint.x, in_Line.endPoint.x, in_Point.x, true, in_Tolerance ) &&
			 CoordinateWithinRange(in_Line.startPoint.y, in_Line.endPoint.y, in_Point.y, true, in_Tolerance )	)
			 return true;

		return false;
	}

	////////////////////////////////
	//	struct BoostPolygon_struct
	///////////////////////////////
	struct BoostPolygon_struct
	{
		typedef boost::geometry::model::d2::point_xy<double> boostpoint_type;
		typedef boost::geometry::model::polygon<boostpoint_type> boostpolygon_type;
			
		boostpolygon_type boostPolygon;

	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool   Point2D_Within_Polygon_2D(	const Point_2D &in_Point, 
										const std::vector<Line_2D> &in_PolygonLines, 
										BoostPolygon_struct   *in_BoostPolygon_struct_ptr,  
										bool in_IncludePointsOnBoundary,
										double in_Tolerance ) throw (isis::application_exception)
	{

		//std::cout << std::endl << "Point: " <<  in_Point;
		if ( in_PolygonLines.size() < 3 ) return false;  // at least three lines are needed to form polygon

		// First check if the point is on any of the lines.  If so, return true/false depending on in_IncludePointsOnBoundary
		for ( std::vector<Line_2D>::const_iterator i(in_PolygonLines.begin()); i !=  in_PolygonLines.end(); ++i )
		{
			if ( PointOnLine_2D( in_Point, *i, in_Tolerance )) 
			{
				if ( in_IncludePointsOnBoundary ) 
					return true;
				else 
					return false;
			}
		}

		if ( boost::geometry::within( BoostPolygon_struct::boostpoint_type(in_Point.x,in_Point.y), 
									  in_BoostPolygon_struct_ptr->boostPolygon) )
			return true;
		else
			return false;

		/*  Old approach, before boost
		std::vector<double> vCrossProductUnit_BaseLine(3);

		bool baseLineEstablished = false;

		for ( std::vector<Line_2D>::const_iterator i(in_PolygonLines.begin()); i !=  in_PolygonLines.end(); ++i )
		{

			if ( PointsAreEqual_2D( i->startPoint, i->endPoint, in_Tolerance ))
			{
				std::string TempError = "Error - Passed a zero length line to Point2D_Within_Polygon_2D(...)";
				throw isis::application_exception(TempError.c_str());
			}

			std::vector<double> vA(3);
			std::vector<double> vB(3);
			std::vector<double> vCrossProductUnit(3);
		
			//vA[0] = in_Point.x - i->startPoint.x;
			//vA[1] = in_Point.y - i->startPoint.y;
			//vA[2] = 0;

			//vB[0] =  i->startPoint.x -  i->endPoint.x;
			//vB[1] =  i->startPoint.y -  i->endPoint.y;
			//vB[2] = 0;

			vA[0] = i->startPoint.x - in_Point.x;
			vA[1] = i->startPoint.y - in_Point.y;
			vA[2] = 0;

			vB[0] = i->endPoint.x - in_Point.x;
			vB[1] = i->endPoint.y - in_Point.y;
			vB[2] = 0;

			//std::cout << std::endl << "Unit Vector vA: " <<  vA[0] << "  " <<  vA[1] << "  " <<  vA[2];
			//std::cout << std::endl << "Unit Vector vB: " <<  vB[0] << "  " <<  vB[1] << "  " <<  vB[2];

			// We know the point is not on one of the lines; however, the point could be outside the ploygon
			// but in line with one of  the lines.  If so the cross product will be zero.
			std::vector<double> crossProductVector = CrossProduct_3D( vA, vB);
			if ( crossProductVector[0] == 0 && crossProductVector[1] == 0 &&  crossProductVector[2] == 0 ) return false; 

			if (baseLineEstablished )
			{
				vCrossProductUnit = UnitVector_3D( CrossProduct_3D( vA, vB));
				//std::cout << std::endl << "vCrossProductUnitr: " <<  vCrossProductUnit[0] << "  " <<  vCrossProductUnit[1] << "  " <<  vCrossProductUnit[2];
				if ( !VectorsEqual_3D( vCrossProductUnit_BaseLine, vCrossProductUnit, in_Tolerance) ) return false; 
			}
			else
			{
				vCrossProductUnit_BaseLine = UnitVector_3D( CrossProduct_3D( vA, vB));
				//std::cout << std::endl << "vCrossProductUnit_BaseLine: " <<  vCrossProductUnit_BaseLine[0] << "  " <<  vCrossProductUnit_BaseLine[1] << "  " <<  vCrossProductUnit_BaseLine[2];
				baseLineEstablished = true;
			}
		}

		return true;
		*/
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  MultiplyMatrix_3x3_x_3x3( double in_Matrix_1[3][3], double in_Matrix_2[3][3], double out_Matrix[3][3] )
	{

		for ( int row = 0; row < 3; ++row )
		{
			for ( int col = 0; col < 3; ++col )
			{
				out_Matrix[row][col] = in_Matrix_1[row][0] * in_Matrix_2[0][col] +
									   in_Matrix_1[row][1] * in_Matrix_2[1][col] +
									   in_Matrix_1[row][2] * in_Matrix_2[2][col];

			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  MultiplyMatrix_3x3_x_1x3( const double in_Matrix_1[3][3], const double in_Matrix_2[3], double out_Matrix[3] )
	{
		for ( int row = 0; row < 3; ++row )
		{
			out_Matrix[row] =	in_Matrix_1[row][0] * in_Matrix_2[0] +
								in_Matrix_1[row][1] * in_Matrix_2[1] +
								in_Matrix_1[row][2] * in_Matrix_2[2];
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  MultiplyMatrix_2x2_x_1x2( const double in_Matrix_1[2][2], const double in_Matrix_2[2], double out_Matrix[2] )
	{
		for ( int row = 0; row < 2; ++row )
		{
			out_Matrix[row] =	in_Matrix_1[row][0] * in_Matrix_2[0] +
								in_Matrix_1[row][1] * in_Matrix_2[1];
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AllMatrixValuesEqualTarget_3X3( double in_Matrix[3][3], double in_Target )
	{
		for ( int i = 0; i < 3; ++i )
			for ( int j = 0; j < 3; ++j )
				if (in_Matrix[i][j] != in_Target) return false; 

		return true;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	void SetFromToMatrix_3X3( double in_From[3][3], double out_To[3][3] )
	{
		for ( int i = 0; i < 3; ++i )
			for ( int j = 0; j < 3; ++j ) out_To[i][j] =  in_From[i][j];
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FindIndexOfNextLineInPolygon(	const std::vector<Line_2D>	&in_PolygonLines,
										const std::vector<int>		&in_AlreadySorted, 
										Point_2D					&in_out_CurrentTargetPoint, 
										int							&out_FoundLineIndex,
										double						in_Tolerance  ) throw (isis::application_exception)
	{

		int NumLines = (int)in_PolygonLines.size();

		std::vector<int>::const_iterator find_itr;

		for ( int i = 0; i < NumLines; ++i ) 
		{
			find_itr = std::find( in_AlreadySorted.begin(), in_AlreadySorted.end(), i );

			if ( find_itr == in_AlreadySorted.end() )  
			{
				if ( PointsAreEqual_2D( in_out_CurrentTargetPoint, in_PolygonLines[i].startPoint, in_Tolerance ))
				{
					in_out_CurrentTargetPoint = in_PolygonLines[i].endPoint;
					out_FoundLineIndex = i;
					return;
				}
				else
				{
					if ( PointsAreEqual_2D( in_out_CurrentTargetPoint, in_PolygonLines[i].endPoint, in_Tolerance ))
					{
						in_out_CurrentTargetPoint = in_PolygonLines[i].startPoint;
						out_FoundLineIndex = i;
						return;
					}		
				}
			}
		}
		std::stringstream errorString;
		errorString <<
				"Function - FindIndexOfNextLineInPolygon, Failed to find the next line in polygon. This means that the polygon is not contiguous.  " << 
				"Was serching for: " << in_out_CurrentTargetPoint << " as and endpoint in the following lines: " <<
				in_PolygonLines << std::endl << "The indexes of the following lines were already selected: " << std::endl <<
				in_AlreadySorted;
		throw isis::application_exception(errorString.str());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Line_2D> SortPolygonLines (const std::vector<Line_2D> &in_PolygonLines, double in_Tolerance  ) throw (isis::application_exception)
	{
		// Must have at least three lines to make a polygon
		if ( in_PolygonLines.size() < 3 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - SortPolygonLines, received a vector that does not have at least three elements." <<
				"  in_PolygonLines: " << in_PolygonLines;
			throw isis::application_exception(errorString.str());
		}

		std::vector<int> sortedOrder;

		// Assume first line is the start of the polygon.

		int currentLine_index = 0;
		sortedOrder.push_back(currentLine_index);

		Point_2D searchPoint = in_PolygonLines[0].endPoint;

		for ( std::vector<Line_2D>::const_iterator i = in_PolygonLines.begin() + 1; i < in_PolygonLines.end(); ++i )
		{

			FindIndexOfNextLineInPolygon(	in_PolygonLines,	// input
											sortedOrder,		// input
											searchPoint,		// both input and output 
											currentLine_index,	// output
											in_Tolerance );		// input

			sortedOrder.push_back( currentLine_index);
		}

		std::vector<Line_2D> outPolygonLines;

		for (std::vector<int>::const_iterator j(sortedOrder.begin()); j < sortedOrder.end(); ++j )	
		{
			outPolygonLines.push_back( in_PolygonLines[*j] );
		}

		if ( !PointsAreEqual_2D( outPolygonLines.front().startPoint , outPolygonLines.back().startPoint, in_Tolerance) && 
			 !PointsAreEqual_2D( outPolygonLines.front().startPoint , outPolygonLines.back().endPoint, in_Tolerance ))
		{
			std::stringstream errorString;
			errorString <<
				"Function - SortPolygonLines, Polygon lines do not form a closed loop. The lines follow: " << in_PolygonLines;
			throw isis::application_exception(errorString.str());
		}

		return outPolygonLines;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void VectorsFormingIntersection_of_FirstAndLastLine_2D( const std::vector<Line_2D> &in_SortedPolygonLines, 	
															  std::vector<double>  &out_Vector_1,
															  std::vector<double>  &out_Vector_2, 
															  double in_Tolerance ) throw (isis::application_exception)
	{
		// Must have at least two lines, necessary to create the two vectors.
		if ( in_SortedPolygonLines.size() < 2 )
		{
			std::stringstream errorString;
			errorString <<
				"Function - VectorsFormingIntersection_of_FirstAndLastLine_2D, received a vector that does not have at least two elements." <<
				"  in_SortedPolygonLines: " << in_SortedPolygonLines;
			throw isis::application_exception(errorString.str());
		}

		Line_2D  firstLine = in_SortedPolygonLines.front();
		Line_2D  lastLine =  in_SortedPolygonLines.back();

		// First Line Start Point is always the vertex
		Point_2D vertex = firstLine.startPoint;

		// Set out_Vector_1
		out_Vector_1.push_back( firstLine.endPoint.x - firstLine.startPoint.x );
		out_Vector_1.push_back( firstLine.endPoint.y - firstLine.startPoint.y );

		if ( PointsAreEqual_2D( firstLine.startPoint, lastLine.startPoint, in_Tolerance ))
		{
			out_Vector_2.push_back( lastLine.endPoint.x - lastLine.startPoint.x );
			out_Vector_2.push_back( lastLine.endPoint.y - lastLine.startPoint.y );
		}
		else
		{
			if ( PointsAreEqual_2D( firstLine.startPoint, lastLine.endPoint, in_Tolerance ))
			{
				out_Vector_2.push_back( lastLine.startPoint.x - lastLine.endPoint.x );
				out_Vector_2.push_back( lastLine.startPoint.y - lastLine.endPoint.y );
			}
			else
			{
				// Should throw an exception here.  This should not happen
				std::string TempError = "ERROR - VectorsFormingIntersetion_of_FirstAndLastLine, The first and last lines in a polygon do not interset.";
				throw isis::application_exception(TempError.c_str());
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	//								Class TransformationMatrix
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	TransformationMatrix::TransformationMatrix( )
	{
		m_TransformationPopulated = false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void TransformationMatrix::setTransformationMatrix(	
										const std::vector<double> &in_Vector_XDirection, 
										const std::vector<double> &in_Vector_XYPlane,
										const std::vector<double> &in_offset,
										double in_Tolerance  ) throw (isis::application_exception)
		{	
			if ( VectorsEqual_3D( UnitVector_3D(in_Vector_XDirection), UnitVector_3D(in_Vector_XYPlane), in_Tolerance ))
			//if ( VectorsEqual_3D(in_Vector_XDirection, in_Vector_XYPlane, in_Tolerance ))
			{
				std::stringstream errorString;
				errorString <<
					"Function - setTransformationMatrix, recieved input vectors that have identical unit vectors." << std::endl <<
					"  in_Vector_XDirection: " << in_Vector_XDirection << 
					"  in_Vector_XYPlane: " << in_Vector_XYPlane << std::endl <<
					"  UnitVector_3D(in_Vector_XDirection): " << UnitVector_3D(in_Vector_XDirection) << 
					"  UnitVector_3D(in_Vector_XYPlane): " << UnitVector_3D(in_Vector_XYPlane);
				throw isis::application_exception(errorString.str());
			}

			// Set the new coordinate system offset from reference coodinate system
			offset[0] = in_offset[0];
			offset[1] = in_offset[1];
			offset[2] = in_offset[2];

			// Set the rotation matrix about the x axis
			std::vector<double> unitVector_x_Axis = UnitVector_3D(in_Vector_XDirection);
			rotationMatrix[0][0] = unitVector_x_Axis[0];
			rotationMatrix[0][1] = unitVector_x_Axis[1];
			rotationMatrix[0][2] = unitVector_x_Axis[2];
			//std::cout << std::endl << "in_Vector_XDirection: " << in_Vector_XDirection;
			//std::cout << std::endl << "unitVector_x_Axis: " << unitVector_x_Axis;


			// Set the rotation matrix about the z axis
			// z axis is in the direction of the cross product of in_Vector_XDirection and in_Vector_XYPlane
			std::vector<double> unitVector_z_Axis = UnitVector_3D(CrossProduct_3D(in_Vector_XDirection, in_Vector_XYPlane));
			rotationMatrix[2][0] = unitVector_z_Axis[0];
			rotationMatrix[2][1] = unitVector_z_Axis[1];
			rotationMatrix[2][2] = unitVector_z_Axis[2];

			// Set the new coordinate system y axis
			// y axis is in the direction of the cross product of unitVector_z_Axis and unitVector_x_Axis
			std::vector<double> unitVector_y_Axis = UnitVector_3D( CrossProduct_3D(unitVector_z_Axis, unitVector_x_Axis));
			rotationMatrix[1][0] = unitVector_y_Axis[0];
			rotationMatrix[1][1] = unitVector_y_Axis[1];
			rotationMatrix[1][2] = unitVector_y_Axis[2];
	
		}  // End setTransformationMatrix

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		void TransformationMatrix::setTransformationMatrix(	
										const double in_RotationMatrix[3][3],
										const std::vector<double> &in_Offset,
										double in_Tolerance ) throw (isis::application_exception)
		{
			offset[0] = in_Offset[0];
			offset[1] = in_Offset[1];
			offset[2] = in_Offset[2];

			for ( int i = 0; i < 3; ++i )
				for ( int j = 0; j < 3; ++j ) rotationMatrix[i][j] = in_RotationMatrix[i][j];

		}

		void TransformationMatrix::getTransformationMatrix(	double out_RotationMatrix[3][3],
										std::vector<double> &out_Offset ) throw (isis::application_exception)
		{
			out_Offset.clear();
			out_Offset.push_back( offset[0]);
			out_Offset.push_back( offset[1]);
			out_Offset.push_back( offset[2]);

			for ( int i = 0; i < 3; ++i )
				for ( int j = 0; j < 3; ++j ) out_RotationMatrix[i][j] = rotationMatrix[i][j];

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		Point_3D TransformationMatrix::getTransformedCoordinates( const Point_3D &in_Point_3D ) const
		{

			double tempMatrix[3];
			tempMatrix[0] = in_Point_3D.x - offset[0];
			tempMatrix[1] = in_Point_3D.y - offset[1];
			tempMatrix[2] = in_Point_3D.z - offset[2];

			double TransformedCoordinates[3];

			MultiplyMatrix_3x3_x_1x3(rotationMatrix, tempMatrix, TransformedCoordinates );

			return Point_3D(TransformedCoordinates[0], TransformedCoordinates[1], TransformedCoordinates[2] );

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		void TransformationMatrix::getRotationAngles( double &rotationAboutX,
													  double &rotationAboutY,
													  double &rotationAboutZ) const
		{
		   // https://truesculpt.googlecode.com/hg-history/38000e9dfece971460473d5788c235fbbe82f31b/Doc/rotation_matrix_to_euler.pdf
		   // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToEuler/index.htm

			const double ROTATION_ZERO_TOLERANCE = .00001;
			
			// Check for rotationMatrix[2][0] == 1 or -1 
			if (  ValueAreEqual( std::abs( rotationMatrix[2][0]),  1.0, ROTATION_ZERO_TOLERANCE ))
			{
				rotationAboutZ = 0.0;
				// Check for rotationMatrix[2][0] == 1 
				if ( ValueAreEqual( rotationMatrix[2][0], 1.0, ROTATION_ZERO_TOLERANCE ) )
				{
					//rotationMatrix[2][0] == 1 
					rotationAboutY = -M_PI / 2.0;
					rotationAboutX = -rotationAboutZ + atan2( -rotationMatrix[0][1], -rotationMatrix[0][2] ); 

				}
				else
				{   //rotationMatrix[2][0] == -1 
					rotationAboutY = M_PI / 2.0;
					rotationAboutX = rotationAboutZ + atan2( rotationMatrix[0][1], rotationMatrix[0][2] ); 
				}
			}
			else
			{   
				rotationAboutY = -std::asin(  rotationMatrix[2][0] );
				double cos_rY = std::cos(rotationAboutY);
				rotationAboutX = std::atan2( rotationMatrix[2][1] / cos_rY, rotationMatrix[2][2] / cos_rY ); 
				rotationAboutZ = std::atan2( rotationMatrix[1][0] / cos_rY, rotationMatrix[0][0] / cos_rY ); 			
			}	   

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		void TransformationMatrix::getOffset(	double &offset_X,
			  									double &offset_Y,
												double &offset_Z ) const
		{
			offset_X = offset[0];
			offset_Y = offset[1];
			offset_Z = offset[2];
		}

		/**
		void TransformationMatrix::getTransformedCoordinates( double in_GlobalCoordinates[3], double out_TransformedCoordinates[3] ) const
		{

			double tempMatrix[3];
			tempMatrix[0] = in_GlobalCoordinates[0] - offset[0];
			tempMatrix[1] = in_GlobalCoordinates[1] - offset[1];
			tempMatrix[2] = in_GlobalCoordinates[2] - offset[2];

			MultiplyMatrix_3x3_x_1x3(rotationMatrix, tempMatrix, out_TransformedCoordinates );

		}
		**/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		std::ostream& operator<<(std::ostream &output, const TransformationMatrix &in_TransformationMatrix )
		{
			output <<  std::endl << "***** BEGIN - Transformation Matrix *****";

			output <<  std::endl << "     Roatation Matrix:";

			output <<  std::endl << "     " << std::setw(12) << in_TransformationMatrix.rotationMatrix[0][0] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[0][1] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[0][2];

			output <<  std::endl << "     " << std::setw(12) << in_TransformationMatrix.rotationMatrix[1][0] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[1][1] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[1][2];

			output <<  std::endl << "     " << std::setw(12) << in_TransformationMatrix.rotationMatrix[2][0] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[2][1] << "  " << 
											   std::setw(12) <<	in_TransformationMatrix.rotationMatrix[2][2];

			output <<  std::endl;
			output <<  std::endl << "     Offset:" ; 
			output <<  std::endl <<	"     "	<< std::setw(12) << in_TransformationMatrix.offset[0] << 
											   std::setw(12) << in_TransformationMatrix.offset[1] << 
											   std::setw(12) << in_TransformationMatrix.offset[2];

			output <<  std::endl << "***** END - Transformation Matrix *****";

			return output;
		}



	////////////////////////////////////////////////////////////////////////////////////////////////////

	double ShortestDistanceBetweenPointAndLine( const std::vector<Point_3D> &in_Line,
												const Point_3D				&in_Point,
												double in_Precision ) throw (isis::application_exception)
	{
		//	A . B = |A| |B| * cos(theta)
		//  A - Vector from in_Line[0] to in_Line[1]
		//  B - Vector from in_Line[0] to in_Point
		//	C - Distance perpenducular to A to B
		//  
		//					>
		//			B	.	|
		//			.		| C
		//		.			|
		//		----------------->
		//				A
		//
		//  theta = cos-1( (A.B)/(|A| |B|) 
		//  C = sin(theta) * |B|

		if ( in_Line.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - ShortestDistanceBetweenPointAndLine, received in_Line vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Line.size() != 2 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - ShortestDistanceBetweenPointAndLine, received in_Line vector, which does contain two points (i.e. start and end points of the line).";
				throw isis::application_exception(errorString.str());
		}


		if ( PointsAreEqual_3D( in_Line[0],in_Line[1], in_Precision ))
		{
				std::stringstream errorString;
				errorString <<
					"Function - ShortestDistanceBetweenPointAndLine, received in_Line vector, which contains two identical points.  The start and end points must be different.";
				throw isis::application_exception(errorString.str());
		}

		if ( PointsAreEqual_3D( in_Line[0],in_Point, in_Precision ) ||
			 PointsAreEqual_3D( in_Line[1],in_Point, in_Precision )) return 0;

		std::vector<double> vector_A(3);
		vector_A[0] = in_Line[1].x - in_Line[0].x;
		vector_A[1] = in_Line[1].y - in_Line[0].y;
		vector_A[2] = in_Line[1].z - in_Line[0].z;

		std::vector<double> vector_B(3);
		vector_B[0] = in_Point.x - in_Line[0].x;
		vector_B[1] = in_Point.y - in_Line[0].y;
		vector_B[2] = in_Point.z - in_Line[0].z;

		//  theta = cos-1( (A.B)/|A||B|) 
		double theta = std::acos( DotProduct_3D(vector_A, vector_B) / ( Magnitude_3D(vector_A) * Magnitude_3D(vector_B)) );

		// C = sin(theta) * |B|
		return std::abs(std::sin(theta) * Magnitude_3D(vector_B));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool LinesCollinear(	const std::vector<Point_3D> &in_Line_1,
							const std::vector<Point_3D> &in_Line_2,
												double in_Precision ) throw (isis::application_exception)
	{

		if ( in_Line_1.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - LinesCollinear, received in_Line vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Line_1.size() != 2 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - LinesCollinear, received in_Line vector, which does contain two points (i.e. start and end points of the line).";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Line_2.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - LinesCollinear, received in_Line vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Line_2.size() != 2 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - LinesCollinear, received in_Line vector, which does contain two points (i.e. start and end points of the line).";
				throw isis::application_exception(errorString.str());
		}

		// Detemine if the points defining line 2 are on line 1

		double distance = isis_CADCommon::ShortestDistanceBetweenPointAndLine( in_Line_1, in_Line_2[0]  );

		if  ( distance > in_Precision ) return false;

		distance = isis_CADCommon::ShortestDistanceBetweenPointAndLine( in_Line_1, in_Line_2[1]  );

		if  ( distance > in_Precision ) return false;
				
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool VectorsPointInTheSameDirection_3D(		const std::vector<double> &in_Vector_1,
												const std::vector<double> &in_Vector_2,
												double in_Precision ) throw (isis::application_exception)
	{
		if ( in_Vector_1.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - VectorsPointInTheSameDirection_3D, received in_Vector_1 vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Vector_1.size() != 3 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - VectorsPointInTheSameDirection_3D, received in_Vector_1 vector, which does contain three values.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Vector_2.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - VectorsPointInTheSameDirection_3D, received in_Vector_2 vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		if ( in_Vector_2.size() != 3 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - VectorsPointInTheSameDirection_3D, received in_Vector_2 vector, which does contain three values.";
				throw isis::application_exception(errorString.str());
		}


		std::vector<double> unit_vector_1 = UnitVector_3D( in_Vector_1 );
		std::vector<double> unit_vector_2 = UnitVector_3D( in_Vector_2 );

		if ( PointsAreEqual_3D( Point_3D( unit_vector_1[0], unit_vector_1[1], unit_vector_1[2] ), 
								Point_3D( unit_vector_2[0], unit_vector_2[1], unit_vector_2[2] )))
			return true;
		else
			return false;

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ValueOnOrWithinRange(double in_Value, double in_StartRange, double in_EndRange, double in_Precision )
	{
		double startRange;
		double endRange;

		if ( in_StartRange < in_EndRange )
		{
			startRange = in_StartRange - in_Precision;
			endRange   = in_EndRange + in_Precision;
		}
		else
		{
			startRange = in_EndRange - in_Precision;
			endRange   = in_StartRange + in_Precision ;
		}

		if ( ( in_Value >= startRange )  && ( in_Value <= endRange ) )
			return true;
		else 
			return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////
	// Class Geometry_3D
	///////////////////////////
	Geometry_3D::Geometry_3D() {}
	Geometry_3D::~Geometry_3D(){};
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////
	//	class Polygon_3D
	//////////////////////////
	Polygon_3D::Polygon_3D(  const std::vector<Point_3D>  in_Polygon_Points,
						     double in_Tolerance, 
							 const std::string &in_ErrorSourceString ): polygon_Points(in_Polygon_Points) 
	{
		if ( in_Polygon_Points.size() < 3 )
		{
				std::stringstream errorString;
				std::string errorSourceString;
				if ( in_ErrorSourceString.size() > 0 ) errorSourceString = std::string("Error Source: ") + in_ErrorSourceString + std::string("  "); 
				errorString << errorSourceString << 
					"Function - Polygon_3D::Polygon_3D, received in_Polygon_Points vector, which does not have at least three elements." <<
					"  in_Polygon: " << in_Polygon_Points;
				throw isis::application_exception(errorString.str());
		}

		std::list< Point_3D> points_temp(in_Polygon_Points.begin(), in_Polygon_Points.end());

		if ( DuplicatePointInList( points_temp, in_Tolerance ) )
		{
				std::stringstream errorString;
				std::string errorSourceString;
				if ( in_ErrorSourceString.size() > 0 ) errorSourceString = std::string("Error Source: ") + in_ErrorSourceString + std::string("  "); 
				errorString << errorSourceString << 
					"Function - Polygon_3D::Polygon_3D, received two identical points defining the polygon." << std::endl <<
					"in_Polygon_Points: " << in_Polygon_Points;
				throw isis::application_exception(errorString.str());
		}

		// Need two vectors forming the X -axis and plane of y - axis
		std::vector<double> vector_x;

		vector_x.push_back( in_Polygon_Points[1].x -  in_Polygon_Points[0].x );
		vector_x.push_back( in_Polygon_Points[1].y -  in_Polygon_Points[0].y );
		vector_x.push_back( in_Polygon_Points[1].z -  in_Polygon_Points[0].z );

		std::vector<double> vector_xy_plane;
		vector_xy_plane.push_back( in_Polygon_Points[3].x -  in_Polygon_Points[0].x );
		vector_xy_plane.push_back( in_Polygon_Points[3].y -  in_Polygon_Points[0].y );
		vector_xy_plane.push_back( in_Polygon_Points[3].z -  in_Polygon_Points[0].z );

		// Create offset vector
		std::vector<double> offset;
		offset.push_back( in_Polygon_Points[0].x );
		offset.push_back( in_Polygon_Points[0].y );
		offset.push_back( in_Polygon_Points[0].z );

		// Set transformation Matrix
		transformationMatrix.setTransformationMatrix(vector_x, vector_xy_plane, offset );

		// Tranform the points forming the polygon to the new coordinate system
		std::vector<Point_3D>  polygonCoordinates_transformed;
		for ( std::vector<Point_3D>::const_iterator i(in_Polygon_Points.begin()); i != in_Polygon_Points.end(); ++i )
		{
			polygonCoordinates_transformed.push_back(transformationMatrix.getTransformedCoordinates(*i));				
		} 

		// For polygonCoordinates_transformed, all z axis values must be zero; otherwise the polygon points are not coplannar.
		for ( std::vector<Point_3D>::const_iterator i(polygonCoordinates_transformed.begin()); i != polygonCoordinates_transformed.end(); ++i )
		{
			if ( std::abs(i->z) > in_Tolerance ) 
			{
				std::stringstream errorString;
				std::string errorSourceString;
				if ( in_ErrorSourceString.size() > 0 ) errorSourceString = std::string("Error Source: ") + in_ErrorSourceString + std::string("  "); 
				errorString << errorSourceString << 
					"Function - Polygon_3D::Polygon_3D, received points defining the polygon where the points were not coplanar." <<
					"  in_Polygon_Points: " << in_Polygon_Points;
				throw isis::application_exception(errorString.str());
			}
		} 

		// Create polygin lines (i.e. polygon_Lines_Transformed )
		for ( std::vector<Point_3D>::const_iterator i(polygonCoordinates_transformed.begin()); i != polygonCoordinates_transformed.end(); ++i )
		{
			if ( (i + 1) != polygonCoordinates_transformed.end() )
			{
				polygon_Lines_Transformed.push_back( Line_2D( Point_2D(i->x, i->y), Point_2D((i + 1)->x,(i + 1)->y)) );	
			}
			else
			{
				std::vector<Point_3D>::const_iterator j = polygonCoordinates_transformed.begin();
				polygon_Lines_Transformed.push_back( Line_2D( Point_2D(i->x, i->y), Point_2D(j->x,j->y)) );	
			}
		}
		

		///////////////////////////
		// Boost Polygon
		//////////////////////////
		boostPolygon_ptr = new BoostPolygon_struct();

		// Populate boost polygon
		for ( std::vector<Point_3D>::const_iterator i(polygonCoordinates_transformed.begin()); i != polygonCoordinates_transformed.end(); ++i )
		{
			boost::geometry::exterior_ring(boostPolygon_ptr->boostPolygon).push_back(boost::geometry::model::d2::point_xy<double>(i->x,i->y));
		} 
		// Correct the plolygon - clockwise orientiatin, closed loop (i.e. last point repeated to close the loop
		boost::geometry::correct(boostPolygon_ptr->boostPolygon);

		// Make sure the polygon lines do not cross
		if (   boost::geometry::intersects(boostPolygon_ptr->boostPolygon) )
		{
			std::stringstream errorString;
			std::string errorSourceString;
			if ( in_ErrorSourceString.size() > 0 ) errorSourceString = std::string("Error Source: ") + in_ErrorSourceString + std::string("  "); 
			errorString << errorSourceString << 
				"Function - Polygon_3D::Polygon_3D, received points defining the polygon where the points form lines and at least one of the lines crosses another line." <<
				"  in_Polygon_Points: " << in_Polygon_Points;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Polygon_3D::~Polygon_3D(){delete boostPolygon_ptr;};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool Polygon_3D::pointWithinGeometry(	const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{

		Point_3D temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		if ( std::abs(temp_Point_3D.z) > in_Tolerance ) return false;


		if ( Point2D_Within_Polygon_2D( Point_2D( temp_Point_3D.x, temp_Point_3D.y), polygon_Lines_Transformed,boostPolygon_ptr, in_IncludePointsOnBoundary, in_Tolerance) )
			return true;
		else
			return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Polygon_3D::pointProjectedOntoPolygonPlaneWithinGeometry(	const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{

		Point_3D temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		if ( Point2D_Within_Polygon_2D( Point_2D( temp_Point_3D.x, temp_Point_3D.y), polygon_Lines_Transformed,boostPolygon_ptr,in_IncludePointsOnBoundary, in_Tolerance) )
			return true;
		else
			return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	TransformationMatrix  Polygon_3D::getTransformationMatrix(){ return transformationMatrix;};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Polygon_3D &in_Polygon_3D )
	{
		output <<  std::endl << "Polygon:";
		output <<  std::endl << in_Polygon_3D.polygon_Points;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////
	//	class Circle_3D
	//////////////////////////


	Circle_3D::Circle_3D(  const Point_3D &in_Center_Point,
						   const Point_3D &in_Circumference_Point,
						   const Point_3D &in_xyPlane_Point,
						   double in_Tolerance): 
									center_Point(in_Center_Point),
									circumference_Point(in_Circumference_Point),
									xyPlane_Point( in_xyPlane_Point), radius(0)
	{
		std::list< Point_3D> points_temp;
		points_temp.push_back(in_Center_Point);
		points_temp.push_back(in_Circumference_Point);
		points_temp.push_back(in_xyPlane_Point);

		if ( DuplicatePointInList( points_temp, in_Tolerance ) )
		{
				std::stringstream errorString;
				errorString <<
					"Function - Circle_3D::Circle_3D, received two identical points defining the circle." << std::endl <<
					"in_Center_Point: " << in_Center_Point << std::endl <<
					"in_Circumference_Point: " << in_Circumference_Point << std::endl <<
					"xyPlane_Point: " << xyPlane_Point;
				throw isis::application_exception(errorString.str());
		}

		// Need two vectors forming the X -axis and plane of y - axis
		std::vector<double> vector_x;

		vector_x.push_back( in_Circumference_Point.x -  in_Center_Point.x );
		vector_x.push_back( in_Circumference_Point.y -  in_Center_Point.y );
		vector_x.push_back( in_Circumference_Point.z -  in_Center_Point.z );

		std::vector<double> vector_xy_plane;
		vector_xy_plane.push_back( xyPlane_Point.x -  in_Center_Point.x );
		vector_xy_plane.push_back( xyPlane_Point.y -  in_Center_Point.y );
		vector_xy_plane.push_back( xyPlane_Point.z -  in_Center_Point.z );

		// Create offset vector
		std::vector<double> offset;
		offset.push_back( in_Center_Point.x );
		offset.push_back( in_Center_Point.y );
		offset.push_back( in_Center_Point.z );

		// Initialize transformation Matrix
		try
		{
			transformationMatrix.setTransformationMatrix(vector_x, vector_xy_plane, offset );
		}
		catch (isis::application_exception ex )
		{
			std::stringstream errorString;
			errorString <<
				"Function - Circle_3D::Circle_3D, failed to compute the transformation matrix.  " << std::endl <<
				"in_Center_Point: " << in_Center_Point << std::endl <<
				"in_Circumference_Point: " << in_Circumference_Point << std::endl <<
				"xyPlane_Point: " << xyPlane_Point << std::endl <<
				 "setTransformationMatrix: " << ex.what();
				throw isis::application_exception(errorString.str());
		}

		// Set Radius
		radius = Magnitude_3D(vector_x);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Circle_3D::~Circle_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//  If in_IncludePointsOnBoundary == true
	//		efective_radius = radius + in_Tolerance
	//		points within or on the efective_radius are included
	//	else
	//		efective_radius = radius - in_Tolerance
	//		points within but NOT on the efective_radius are included
	bool Circle_3D::pointWithinGeometry(  const Point_3D &in_Point, 
										  bool in_IncludePointsOnBoundary,
										  double in_Tolerance ) const throw (isis::application_exception)
	{
		Point_3D temp_Point_3D;
		temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		bool found = false;

		if ( std::abs(temp_Point_3D.z) < in_Tolerance )
		{
			std::vector<double> pointXY(2);
			pointXY[0] = temp_Point_3D.x;
			pointXY[1] = temp_Point_3D.y;
			double magnitude = Magnitude_2D(pointXY);

			if ( in_IncludePointsOnBoundary ) 
			{
				if ( magnitude <= ( radius + in_Tolerance)) found = true;
			}
			else
			{
				if ( magnitude < ( radius - in_Tolerance ) ) found = true;
			}
		}
		return found;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	double	Circle_3D::getRadius() const { return radius;}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Circle_3D &in_Circle_3D )
	{
		output <<  std::endl << "Circle:";
		output <<  std::endl << "Center Point: " << in_Circle_3D.center_Point;
		output <<  std::endl << "Circumference Point: " << in_Circle_3D.circumference_Point;
		output <<  std::endl << "xyPlane Point: " << in_Circle_3D.xyPlane_Point;
		output <<  std::endl << "Radius: " << in_Circle_3D.radius;
		output <<  std::endl << "Transformation Matrix: " << in_Circle_3D.transformationMatrix;
		return output;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	//					Class ConcentricCircles_3D
	/////////////////////////////////////////////////////////
	ConcentricCircles_3D::ConcentricCircles_3D(  
							const Point_3D &in_Center_Point,
							const Point_3D &in_circumferenceOutterCircle_Point,
							const Point_3D &in_xyPlane_circumferenceInnterCircle_Point,
						   double in_Tolerance) : 
									outerCircle( in_Center_Point, 
												  in_circumferenceOutterCircle_Point, 
												  in_xyPlane_circumferenceInnterCircle_Point),
									innerCircle( in_Center_Point, 
												  in_xyPlane_circumferenceInnterCircle_Point, 
												  in_circumferenceOutterCircle_Point)

	{

		if ( innerCircle.getRadius() >=  outerCircle.getRadius() )
		{
			std::stringstream errorString;
			errorString <<
				"Function - ConcentricCircles_3D::ConcentricCircles_3D, The inner circle has a radius larger than or equal to the outer radius. " << std::endl <<
				"Inner Circle: " << innerCircle << std::endl <<
				"Outer Circle: " << outerCircle << std::endl;
				throw isis::application_exception(errorString.str());
		}

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	ConcentricCircles_3D::~ConcentricCircles_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool ConcentricCircles_3D::pointWithinGeometry(  const Point_3D &in_Point, 
										  bool in_IncludePointsOnBoundary,
										  double in_Tolerance ) const throw (isis::application_exception)
	{

		if (  outerCircle.pointWithinGeometry( in_Point,   in_IncludePointsOnBoundary, in_Tolerance)  &&
		     !innerCircle.pointWithinGeometry( in_Point,  !in_IncludePointsOnBoundary, in_Tolerance) )
			return true;
		else
			return false;

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const ConcentricCircles_3D &in_ConcentricCircles_3D )
	{
		output <<  std::endl << "Concentric Circles:";
		output <<  std::endl << "Outer Circle:";
		output <<  std::endl << in_ConcentricCircles_3D.outerCircle;
		output <<  std::endl << "Inner Circle:";
		output <<  std::endl << in_ConcentricCircles_3D.innerCircle;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////
	//	class Cylinder_3D
	//////////////////////////

	Cylinder_3D::Cylinder_3D(	const Point_3D &in_startCenterline_Point,
								const Point_3D	&in_endCenterline_Point,
								const Point_3D	&in_defineRadius_Point,
								e_CylinderGeometryInclusionSpecifier	in_CylinderGeometryInclusionSpecifier,
								double in_Tolerance): 
									startCenterline_Point(in_startCenterline_Point),
									endCenterline_Point(in_endCenterline_Point),
									defineRadius_Point( in_defineRadius_Point),
									cylinderGeometryInclusionSpecifier(in_CylinderGeometryInclusionSpecifier), radius(0)
	{

		std::list< Point_3D> points_temp;
		points_temp.push_back(in_startCenterline_Point);
		points_temp.push_back(in_endCenterline_Point);
		points_temp.push_back(in_defineRadius_Point);

		if ( DuplicatePointInList( points_temp, in_Tolerance ) )
		{
				std::stringstream errorString;
				errorString <<
					"Function - Cylinder_3D::Cylinder_3D, received two identical points defining the cylinder.  " <<
					"The first point defines the start of the cylinder axis, the second point defines the end of the cylinder axis, and the third point defines the cylinder radius." << std::endl <<
					"  in_startCenterline_Point: " << in_startCenterline_Point << std::endl <<
					"  in_endCenterline_Point: "   << in_endCenterline_Point << std::endl <<
					"  in_defineRadius_Point: "    << in_defineRadius_Point;
				throw isis::application_exception(errorString.str());
		}

		// Need two vectors forming the X -axis and plane of y - axis
		std::vector<double> vector_x;

		vector_x.push_back( in_endCenterline_Point.x -  in_startCenterline_Point.x );
		vector_x.push_back( in_endCenterline_Point.y -  in_startCenterline_Point.y );
		vector_x.push_back( in_endCenterline_Point.z -  in_startCenterline_Point.z );

		std::vector<double> vector_temp;

		vector_temp.push_back( in_defineRadius_Point.x -  in_startCenterline_Point.x );
		vector_temp.push_back( in_defineRadius_Point.y -  in_startCenterline_Point.y );
		vector_temp.push_back( in_defineRadius_Point.z -  in_startCenterline_Point.z );

		std::vector<double> vector_xy_plane = CrossProduct_3D( vector_x, vector_temp  );

		// Create offset vector
		std::vector<double> offset;
		offset.push_back( in_startCenterline_Point.x );
		offset.push_back( in_startCenterline_Point.y );
		offset.push_back( in_startCenterline_Point.z );

		// Populate transformation Matrix
		try
		{
			transformationMatrix.setTransformationMatrix(vector_x, vector_xy_plane, offset );
		}
		catch (isis::application_exception ex )
		{
			std::stringstream errorString;
			errorString <<
				"Function - Cylinder_3D::Cylinder_3D, failed to compute the transformation matrix.  " <<
				"The first point defines the start of the cylinder axis, the second point defines the end of the cylinder axis, and the third point defines the cylinder radius." <<  std::endl << 
				"  in_startCenterline_Point: " << in_startCenterline_Point << std::endl <<
				"  in_endCenterline_Point: "   << in_endCenterline_Point << std::endl <<
				"  in_defineRadius_Point: "    << in_defineRadius_Point <<				
				ex.what();
				throw isis::application_exception(errorString.str());
		}

		std::vector<Point_3D> line;

		// Compute Cylinder radius
		line.push_back(in_startCenterline_Point);
		line.push_back(in_endCenterline_Point);
		radius = ShortestDistanceBetweenPointAndLine( line, in_defineRadius_Point, in_Tolerance );

		startCenterline_Point_Transformed =  transformationMatrix.getTransformedCoordinates(in_startCenterline_Point);
		endCenterline_Point_Transformed   =  transformationMatrix.getTransformedCoordinates(in_endCenterline_Point);

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Cylinder_3D::~Cylinder_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//  If in_IncludePointsOnBoundary == true
	//		efective_radius = radius + in_Tolerance
	//		points within or on the efective_radius are included
	//	else
	//		efective_radius = radius - in_Tolerance
	//		points within but NOT on the efective_radius are included
	bool Cylinder_3D::pointWithinGeometry(  const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{
		bool found = false;

		Point_3D temp_Point_3D;
		temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		if ( ValueOnOrWithinRange(temp_Point_3D.x, startCenterline_Point_Transformed.x, endCenterline_Point_Transformed.x, in_Tolerance ))
		{
			std::vector<double> normalTo_XAxis_2DVector;
			normalTo_XAxis_2DVector.push_back(temp_Point_3D.y);
			normalTo_XAxis_2DVector.push_back(temp_Point_3D.z);

			double magnitude_yz_vector = Magnitude_2D(normalTo_XAxis_2DVector);

			if ( in_IncludePointsOnBoundary )
			{
				if (   magnitude_yz_vector <= ( radius + in_Tolerance ) ) found = true;
			}
			else
			{
				if (   magnitude_yz_vector < ( radius - in_Tolerance ) ) found = true;
			}
			
			if ( found && cylinderGeometryInclusionSpecifier == GEOMETRY_EXCLUDE_END_CAP )
			{
				// Should exclude the points where x cord is either at the beginning or 
				// end of the cylinder and where ( magnitude_yz_vector < ( radius - in_Tolerance ).
				// This is the points not on the cylinder surface but are on the interior area
				// of the end caps

				bool pointAtStartOfCylinder =  ValueAreEqual( startCenterline_Point_Transformed.x, 															 
															  temp_Point_3D.x,
															  in_Tolerance);

				bool pointAtEndOfCylinder =   ValueAreEqual(  endCenterline_Point_Transformed.x, 
															  temp_Point_3D.x,
															  in_Tolerance);

				if ( (pointAtStartOfCylinder || pointAtEndOfCylinder) && 
					 !ValueAreEqual( radius, magnitude_yz_vector, in_Tolerance ))
				{
					found = false;	
				}

			}

			//if ( ValueOnOrWithinRange( magnitude_yz_vector,  radius, radius, in_Tolerance))
			//{
			//	found = true;
			//}
		} 			

		return found;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	double	Cylinder_3D::getRadius() const { return radius;}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Cylinder_3D &in_Cylinder_3D )
	{
		output <<  std::endl << "Cylinder:";
		output <<  std::endl << "Centerline Start Point: " << in_Cylinder_3D.startCenterline_Point;
		output <<  std::endl << "Centerline End Point:   " << in_Cylinder_3D.endCenterline_Point;
		output <<  std::endl << "Point Defining Radius:  " << in_Cylinder_3D.defineRadius_Point;
		output <<  std::endl << "Radius: " << in_Cylinder_3D.radius;
		output <<  std::endl << "Transformation Matrix: " << in_Cylinder_3D.transformationMatrix;
		return output;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////
	//	class CylinderSurface_3D
	/////////////////////////////////
	/*  Deprecated
	CylinderSurface_3D::CylinderSurface_3D(	const Point_3D &in_startCenterline_Point,
								const Point_3D	&in_endCenterline_Point,
								const Point_3D	&in_defineRadius_Point,
								double in_Tolerance): 
									Cylinder_3D(in_startCenterline_Point,
								                in_endCenterline_Point,
								                in_defineRadius_Point,
												in_Tolerance )
	{

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	CylinderSurface_3D::~CylinderSurface_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	

	bool CylinderSurface_3D::pointWithinGeometry(  const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{
		bool found = false;

		Point_3D temp_Point_3D;
		temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		if ( ValueOnOrWithinRange(temp_Point_3D.x, startCenterline_Point_Transformed.x, endCenterline_Point_Transformed.x, in_Tolerance ))
		{
			std::vector<double> normalTo_XAxis_2DVector;
			normalTo_XAxis_2DVector.push_back(temp_Point_3D.y);
			normalTo_XAxis_2DVector.push_back(temp_Point_3D.z);

			double magnitude_yz_vector = Magnitude_2D(normalTo_XAxis_2DVector);

			if ( ValueOnOrWithinRange( magnitude_yz_vector,  radius, radius, in_Tolerance))
			{
				found = true;
			}
		} 			
		return found;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const CylinderSurface_3D &in_CylinderSurface_3D )
	{
		output <<  std::endl << "Cylinder Surface:";
		output <<  std::endl << static_cast<Cylinder_3D>(in_CylinderSurface_3D);
		return output;
	}

	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////
	//	class Sphere_3D
	//////////////////////////

	Sphere_3D::Sphere_3D(	const Point_3D &in_center_Point,
							const Point_3D	&in_surface_Point,
						    double in_Tolerance): 
									center_Point(in_center_Point),
									surface_Point(in_surface_Point),
									radius(0)
	{

		if ( PointsAreEqual_3D( in_center_Point, in_surface_Point, in_Tolerance ) )
		{
				std::stringstream errorString;
				errorString <<
					"Function - Sphere_3D::Sphere_3D, received two identical points defining the Sphere.  " <<
					"The first point defines the center of the Sphere and the second point defines a point on the surface of the sphere." << std::endl <<
					"  in_center_Point: " << in_center_Point << std::endl <<
					"  in_surface_Point: "   << in_surface_Point;
				throw isis::application_exception(errorString.str());
		}


		// Create offset vector
		std::vector<double> offset;
		offset.push_back( in_center_Point.x );
		offset.push_back( in_center_Point.y );
		offset.push_back( in_center_Point.z );
		
		double rotationMatrix[3][3] = {1,0,0, 0,1,0, 0,0, 1};  // Identiy matrix. For a sphere, we only care about the radius.
																  // In other words, a sphere has no orientation.

		// Populate transformation Matrix
		try
		{
			transformationMatrix.setTransformationMatrix(rotationMatrix, offset );

		}
		catch (isis::application_exception ex )
		{
			std::stringstream errorString;
			errorString <<
				"Function - Sphere_3D::Sphere_3D, failed to compute the transformation matrix.  " <<
					"The first point defines the center of the Sphere and the second point defines a point on the surface of the sphere." << std::endl <<
					"  in_center_Point: " << in_center_Point << std::endl <<
					"  in_surface_Point: "   << in_surface_Point;			
				ex.what();
				throw isis::application_exception(errorString.str());
		}

		std::vector<double> radius_vector;
		radius_vector.push_back( in_surface_Point.x -  in_center_Point.x );
		radius_vector.push_back( in_surface_Point.y -  in_center_Point.y );
		radius_vector.push_back( in_surface_Point.z -  in_center_Point.z );

		radius = Magnitude_3D(radius_vector);

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Sphere_3D::~Sphere_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//  If in_IncludePointsOnBoundary == true
	//		efective_radius = radius + in_Tolerance
	//		points within or on the efective_radius are included
	//	else
	//		efective_radius = radius - in_Tolerance
	//		points within but NOT on the efective_radius are included
	bool Sphere_3D::pointWithinGeometry(    const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{
		bool found = false;

		Point_3D temp_Point_3D;
		temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		std::vector<double> temp_Point_Vector;
		temp_Point_Vector.push_back( temp_Point_3D.x );
		temp_Point_Vector.push_back( temp_Point_3D.y );
		temp_Point_Vector.push_back( temp_Point_3D.z );

		double temp_Point_Radius = Magnitude_3D(temp_Point_Vector);
		

		if ( in_IncludePointsOnBoundary )
		{
			if (  temp_Point_Radius <= ( radius + in_Tolerance ) )  found = true;
		}
		else
		{
			if (   temp_Point_Radius < ( radius - in_Tolerance ) ) found = true;
		}

		return found;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	double	Sphere_3D::getRadius() const { return radius;}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Sphere_3D &in_Sphere_3D )
	{
		output <<  std::endl << "Sphere:";
		output <<  std::endl << "Center Point: " << in_Sphere_3D.center_Point;
		output <<  std::endl << "Point on Sphere Surface:   " << in_Sphere_3D.surface_Point;
		output <<  std::endl << "Radius: " << in_Sphere_3D.radius;
		output <<  std::endl << "Transformation Matrix: " << in_Sphere_3D.transformationMatrix;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////
	//	class SphereSurface_3D
	/////////////////////////////////

	SphereSurface_3D::SphereSurface_3D(	const Point_3D &in_center_Point,
								const Point_3D	&in_surface_Point,
								double in_Tolerance): 
									Sphere_3D(in_center_Point,
								                in_surface_Point,
												in_Tolerance )
	{

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	SphereSurface_3D::~SphereSurface_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool SphereSurface_3D::pointWithinGeometry(  const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception)
	{

		Point_3D temp_Point_3D;
		temp_Point_3D = transformationMatrix.getTransformedCoordinates( in_Point);

		std::vector<double> temp_Point_Vector;
		temp_Point_Vector.push_back( temp_Point_3D.x );
		temp_Point_Vector.push_back( temp_Point_3D.y );
		temp_Point_Vector.push_back( temp_Point_3D.z );

		double temp_Point_Radius = Magnitude_3D(temp_Point_Vector);

		if ( ValueOnOrWithinRange(temp_Point_Radius, radius, radius, in_Tolerance ))
			return true;
		else
			return false;
		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const SphereSurface_3D &in_SphereSurface_3D )
	{
		output <<  std::endl << "Sphere Surface:";
		output <<  std::endl << static_cast<Sphere_3D>(in_SphereSurface_3D);
		return output;
	}
	/*** Box not used,  This was not tested
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Box_3D::Box_3D(	const Point_3D	&in_PlanarRectangleCorner_Pt_1,
				const Point_3D	&in_PlanarRectangleCorner_Pt_2,
				const Point_3D	&in_PlanarRectangleCorner_Pt_3,
				const Point_3D	&in_PlanarRectangleCorner_Pt_4,
				const Point_3D	&in_OffsetFromPlanarRectangle_Pt,  
				double in_Tolerance )
					: planarRectangleCorner_Pt_1( in_PlanarRectangleCorner_Pt_1 ), 
					  planarRectangleCorner_Pt_2( in_PlanarRectangleCorner_Pt_2 ), 
					  planarRectangleCorner_Pt_3( in_PlanarRectangleCorner_Pt_3 ), 
					  planarRectangleCorner_Pt_4( in_PlanarRectangleCorner_Pt_4 ),
					  offsetFromPlanarRectangle_Pt (in_OffsetFromPlanarRectangle_Pt )
	{
		std::list< Point_3D> points_temp;
		points_temp.push_back(in_PlanarRectangleCorner_Pt_1);
		points_temp.push_back(in_PlanarRectangleCorner_Pt_2);
		points_temp.push_back(in_PlanarRectangleCorner_Pt_3);
		points_temp.push_back(in_PlanarRectangleCorner_Pt_4);
		points_temp.push_back(in_OffsetFromPlanarRectangle_Pt);

		if (  DuplicatePointInList( points_temp, in_Tolerance ))
		{
				std::stringstream errorString;
				errorString <<
					"Error: (Function - Box_3D::Box_3D), received two identical points defining a box." << std::endl <<
					"in_PlanarRectangleCorner_Pt_1: "   << in_PlanarRectangleCorner_Pt_1 << std::endl <<
					"in_PlanarRectangleCorner_Pt_2: "   << in_PlanarRectangleCorner_Pt_2 << std::endl <<
					"in_PlanarRectangleCorner_Pt_3: "   << in_PlanarRectangleCorner_Pt_3 << std::endl <<
					"in_PlanarRectangleCorner_Pt_4: "   << in_PlanarRectangleCorner_Pt_4 << std::endl <<
					"in_OffsetFromPlanarRectangle_Pt: " << in_OffsetFromPlanarRectangle_Pt;
				throw isis::application_exception(errorString.str());
		}

		// Need two vectors forming the X -axis and plane of y - axis
		std::vector<double> vector_x;

		vector_x.push_back( in_PlanarRectangleCorner_Pt_2.x -  in_PlanarRectangleCorner_Pt_1.x );
		vector_x.push_back( in_PlanarRectangleCorner_Pt_2.y -  in_PlanarRectangleCorner_Pt_1.y );
		vector_x.push_back( in_PlanarRectangleCorner_Pt_2.z -  in_PlanarRectangleCorner_Pt_1.z );

		std::vector<double> vector_xy_plane;
		vector_xy_plane.push_back( in_PlanarRectangleCorner_Pt_3.x -  in_PlanarRectangleCorner_Pt_1.x );
		vector_xy_plane.push_back( in_PlanarRectangleCorner_Pt_3.y -  in_PlanarRectangleCorner_Pt_1.y );
		vector_xy_plane.push_back( in_PlanarRectangleCorner_Pt_3.z -  in_PlanarRectangleCorner_Pt_1.z );

		// Create offset vector
		std::vector<double> offset;
		offset.push_back( in_PlanarRectangleCorner_Pt_1.x );
		offset.push_back( in_PlanarRectangleCorner_Pt_1.y );
		offset.push_back( in_PlanarRectangleCorner_Pt_1.z );

		// Set transformation Matrix
		transformationMatrix.setTransformationMatrix(vector_x, vector_xy_plane, offset );

		// Tranform the points forming the box to the new coordinate system 
		planarRectangleCorner_Pt_1_transformed   = transformationMatrix.getTransformedCoordinates(planarRectangleCorner_Pt_1);
		planarRectangleCorner_Pt_2_transformed   = transformationMatrix.getTransformedCoordinates(planarRectangleCorner_Pt_2);
		planarRectangleCorner_Pt_3_transformed   = transformationMatrix.getTransformedCoordinates(planarRectangleCorner_Pt_3);
		planarRectangleCorner_Pt_4_transformed   = transformationMatrix.getTransformedCoordinates(planarRectangleCorner_Pt_4);
		offsetFromPlanarRectangle_Pt_transformed = transformationMatrix.getTransformedCoordinates(offsetFromPlanarRectangle_Pt);

		if ( ( planarRectangleCorner_Pt_1_transformed.z > in_Tolerance ) ||
			 ( planarRectangleCorner_Pt_2_transformed.z > in_Tolerance ) ||
			 ( planarRectangleCorner_Pt_3_transformed.z > in_Tolerance ) ||
			 ( planarRectangleCorner_Pt_4_transformed.z > in_Tolerance ) )
		{
				std::stringstream errorString;
				errorString <<
					"Error: (Function - Box_3D::Box_3D), received four corner points which are not plannar." << std::endl <<
					"in_PlanarRectangleCorner_Pt_1: "   << in_PlanarRectangleCorner_Pt_1 << std::endl <<
					"in_PlanarRectangleCorner_Pt_2: "   << in_PlanarRectangleCorner_Pt_2 << std::endl <<
					"in_PlanarRectangleCorner_Pt_3: "   << in_PlanarRectangleCorner_Pt_3 << std::endl <<
					"in_PlanarRectangleCorner_Pt_4: "   << in_PlanarRectangleCorner_Pt_4 << std::endl <<
					"in_OffsetFromPlanarRectangle_Pt: " << in_OffsetFromPlanarRectangle_Pt;
				throw isis::application_exception(errorString.str());
		}

		// Would also need to verify that the box had 90 degree angles

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Box_3D::~Box_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Box_3D::pointWithinGeometry(   const Point_3D &in_Point, 
										bool in_IncludePointsOnBoundary,
										double in_Tolerance ) const throw (isis::application_exception)
	{
		// First tranlate the point to the box's coodinate system
		// The tranformed point would be within the box if:
		//	a) set transformed z axis to zero, then the transformed point within the bounded box defined by points 1 thru 4
		//	b) z axis of tranformed point within the range of 0 to offsetFromPlanarRectangle_Pt_transformed.z

		Point_3D point_transformed = transformationMatrix.getTransformedCoordinates(in_Point);

		// the x axis of the box coordinate system points from planarRectangleCorner_Pt_1_transformed to planarRectangleCorner_Pt_2_transformed
		// the y axis of the box coordinate system points from planarRectangleCorner_Pt_1_transformed to planarRectangleCorner_Pt_4_transformed

		bool found = false;

		if (( CoordinateWithinRange( 0, point_transformed.x, planarRectangleCorner_Pt_1_transformed.x, in_IncludePointsOnBoundary, in_Tolerance )) &&
		    ( CoordinateWithinRange( 0, point_transformed.y, planarRectangleCorner_Pt_1_transformed.y, in_IncludePointsOnBoundary, in_Tolerance )) &&
		    ( CoordinateWithinRange( 0, point_transformed.z, planarRectangleCorner_Pt_1_transformed.z, in_IncludePointsOnBoundary, in_Tolerance )) ) found = true;

		return found;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Box_3D &in_Box_3D )
	{
		output <<  std::endl << "Box:";
		output <<  std::endl <<	"planarRectangleCorner_Pt_1:   "  << in_Box_3D.planarRectangleCorner_Pt_1;
		output <<  std::endl <<	"planarRectangleCorner_Pt_2:   "  << in_Box_3D.planarRectangleCorner_Pt_2;
		output <<  std::endl <<	"planarRectangleCorner_Pt_3:   "  << in_Box_3D.planarRectangleCorner_Pt_3;
		output <<  std::endl <<	"planarRectangleCorner_Pt_4:   "  << in_Box_3D.planarRectangleCorner_Pt_4;
		output <<  std::endl <<	"offsetFromPlanarRectangle_Pt: " << in_Box_3D.offsetFromPlanarRectangle_Pt;
		return output;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Extrusion_3D::Extrusion_3D(	const std::vector<Point_3D>  in_Polygon_Points,
								const Point_3D	&in_offsetFromPolygon_pt,    
								double in_Tolerance )
								: extrusionPlanarGeometry_polygon( in_Polygon_Points, in_Tolerance, "Extrusion_3D::Extrusion_3D" ), 
								offsetFromPolygon_pt (in_offsetFromPolygon_pt )
	{
		transformationMatrix = extrusionPlanarGeometry_polygon.getTransformationMatrix();
		offsetFromPolygon_pt_transformed = transformationMatrix.getTransformedCoordinates(offsetFromPolygon_pt);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Extrusion_3D::~Extrusion_3D(){};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Extrusion_3D::pointWithinGeometry(   const Point_3D &in_Point, 
										bool in_IncludePointsOnBoundary,
										double in_Tolerance ) const throw (isis::application_exception)
	{
		if ( !extrusionPlanarGeometry_polygon.pointProjectedOntoPolygonPlaneWithinGeometry(in_Point, in_IncludePointsOnBoundary, in_Tolerance )) 
			return false;

		Point_3D  tempPoint = transformationMatrix.getTransformedCoordinates(in_Point);

		if ( CoordinateWithinRange( 0, offsetFromPolygon_pt_transformed.z, tempPoint.z, in_IncludePointsOnBoundary, in_Tolerance ))
			return true;
		else
			return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream &output, const Extrusion_3D &in_Extrusion_3D )
	{
		output <<  std::endl << "Extrusion:";
		output <<  std::endl <<	"extrusionPlanarGeometry_polygon: "  << in_Extrusion_3D.extrusionPlanarGeometry_polygon;
		output <<  std::endl <<	"offsetFromPolygon_pt: " << in_Extrusion_3D.offsetFromPolygon_pt;
		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// This function must allways add to out_GridPointIds_WithinGeometry.  It should never null out
	// out_GridPointIds_WithinGeometry,
	void FindGridPointsWithinGeometry( const std::map<int,GridPoint>	&in_GridPoints_map, 
									   const Geometry_3D				&in_Geometry,
									   bool								in_IncludePointsOnBoundary,
									   double							in_Tolerance,
									   std::vector<int>					&out_GridPointIds_WithinGeometry)
																					throw (isis::application_exception)
	{								  
		for ( std::map<int, GridPoint>::const_iterator i(in_GridPoints_map.begin()); 
				i != in_GridPoints_map.end(); 
				++i )
		{
			if ( in_Geometry.pointWithinGeometry( i->second.point, in_IncludePointsOnBoundary, in_Tolerance))	
				out_GridPointIds_WithinGeometry.push_back(i->first);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This function must allways add to out_GridPointIds_WithinGeometry.  It should never null out
	// out_GridPointIds_WithinGeometry,
	void GetGridPointsWithinGeometry( const std::map<int,GridPoint>		&in_GridPoints_map, 
									  const Geometry_3D					&in_Geometry,
									  e_GeneralGeometryInclusionSpecifier		in_GeneralGeometryInclusionSpecifier,
									  double							in_Tolerance,
									  std::vector<int>					&out_GridPointIds_WithinGeometry) 
																			throw (isis::application_exception)
	{

		//	InteriorAndBoundary -   Invoke pointWithinGeometry with in_IncludePointsOnBoundary = true
		//	BoundaryOnly -			A - B; where 
		//								A = Invoke pointWithinGeometry with in_IncludePointsOnBoundary = true
		//								B = Invoke pointWithinGeometry with in_IncludePointsOnBoundary = false
		//	InteriorOnly -			Invoke pointWithinGeometry with in_IncludePointsOnBoundary = false

		if ( in_GridPoints_map.size() == 0 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - GetGridPointsWithinGeometry, received in_GridPoints vector, which does not have any elements.";
				throw isis::application_exception(errorString.str());
		}

		bool IncludePointsOnBoundary;
		switch ( in_GeneralGeometryInclusionSpecifier)
		{
			case GEOMETRY_INTERIOR_AND_BOUNDARY:
				IncludePointsOnBoundary = true;
				FindGridPointsWithinGeometry( in_GridPoints_map, 
											  in_Geometry,
											  IncludePointsOnBoundary,
											  in_Tolerance,
											  out_GridPointIds_WithinGeometry);
				break;

			case GEOMETRY_INTERIOR_ONLY:
				IncludePointsOnBoundary = false;
				FindGridPointsWithinGeometry( in_GridPoints_map, 
											  in_Geometry,
											  IncludePointsOnBoundary,
											  in_Tolerance,
											  out_GridPointIds_WithinGeometry);
				break;

			case GEOMETRY_BOUNDARY_ONLY:
				std::vector<int>			  A_vector, B_vector;
				IncludePointsOnBoundary = true;
				FindGridPointsWithinGeometry( in_GridPoints_map, 
											  in_Geometry,
											  IncludePointsOnBoundary,
											  in_Tolerance,
											  A_vector);
				IncludePointsOnBoundary = false;
				FindGridPointsWithinGeometry( in_GridPoints_map, 
											  in_Geometry,
											  IncludePointsOnBoundary,
											  in_Tolerance,
											  B_vector);


				std::set<int>				  A_set( A_vector.begin(), A_vector.end());
				std::set<int>				  B_set( B_vector.begin(), B_vector.end());

				std::set<int>				  Boundary_set;

				set_difference( A_set.begin(), A_set.end(), B_set.begin(), B_set.end(), 
								std::inserter(Boundary_set, Boundary_set.end())); 

				for ( std::set<int>::const_iterator i(Boundary_set.begin());
					  i != Boundary_set.end(); ++i )
				{
					out_GridPointIds_WithinGeometry.push_back(*i);

				}
				break;
		}
	}
	
	double Determinant_2_x_2( double in_Matrix[2][2] )
	{
		//  a    b
		//  c    d
		//
		// Determinant = ad - bc

		return in_Matrix[0][0] * in_Matrix[1][1]  -  in_Matrix[0][1] * in_Matrix[1][0];
	}


	double Determinant_3_x_3( double in_Matrix[3][3] )
	{
		//  a    b    c
		//  d    e    f
		//  g    h    i
		// Determinant =  a ( ei - fh) - b(di-fg) + c(dh-eg)

		return		//		a				e					i					f				h
					+in_Matrix[0][0] * ( in_Matrix[1][1] * in_Matrix[2][2] - in_Matrix[1][2] * in_Matrix[2][1] )
					//		b				d					i					f				g
					-in_Matrix[0][1] * ( in_Matrix[1][0] * in_Matrix[2][2] - in_Matrix[1][2] * in_Matrix[2][0] )
					//		c				d					h					e				g
					+in_Matrix[0][2] * ( in_Matrix[1][0] * in_Matrix[2][1] - in_Matrix[1][1] * in_Matrix[2][0] );
			
	}

	bool Positive_Definite_3_x_3( double in_Matrix[3][3] )
	{
		if ( in_Matrix[0][0] < 0.0 ) return false;

		double Temp_2_2[2][2];

		Temp_2_2[0][0] = in_Matrix[0][0];
		Temp_2_2[0][1] = in_Matrix[0][1];
		Temp_2_2[1][0] = in_Matrix[1][0];
		Temp_2_2[1][1] = in_Matrix[1][1];

		if ( Determinant_2_x_2(Temp_2_2)  < 0.0 ) return false;

		if ( Determinant_3_x_3(in_Matrix) < 0.0 ) return false;

		return true;
	}

		// matrix numbering starts a 0 (e.g. first row is 0, second row is 1)
	void Extract_3_3_Matrix(  double in_Matrix[4][4], int in_SkipRow, int in_SkipColumn, double out_Matrix[3][3] )
	{
		int currentRow =  0;

		for ( int i = 0; i < 4; ++i ) 
		{
			if ( i != in_SkipRow )
			{
				int currentColumn =  0;
				for ( int j = 0; j < 4; ++j )  
				{
					if ( j != in_SkipColumn )
					{
						out_Matrix[currentRow][currentColumn]  = in_Matrix[i][j];
						++currentColumn;
					}
				}
				++currentRow;
			}
		}
		//std::cout << std::endl << std::endl;
		//for ( int i = 0; i < 3; ++i )
		//{
		//	std::cout << std::endl;
		//	for ( int j = 0; j < 3; ++j ) std::cout << out_Matrix[i][j] << "  ";
		//}
	}

	double Determinant_4_x_4 ( double in_Matrix[4][4] )
	{
		//  a    b    c   d
		//  e    f    g   h
		//  i    j    k	  l
		//  m    n    o   p

		double determinant_0;
		double determinant_1;
		double determinant_2;
		double determinant_3;

		double temp_3_x_3[3][3];

		// Get determinant of 
		//  f    g   h
		//  j    k	 l
		//  n    o   p

		Extract_3_3_Matrix( in_Matrix, 0, 0, temp_3_x_3);
		determinant_0 = Determinant_3_x_3(temp_3_x_3);

		// Get determinant of 
		//  e    g   h
		//  i    k	 l
		//  m    o   p
		Extract_3_3_Matrix( in_Matrix, 0, 1, temp_3_x_3);
		determinant_1 = Determinant_3_x_3(temp_3_x_3);

		// Get determinant of 
		//  e    f    h
		//  i    j    l
		//  m    n    p

		Extract_3_3_Matrix( in_Matrix, 0, 2, temp_3_x_3);
		determinant_2 = Determinant_3_x_3(temp_3_x_3);

		// Get determinant of 
		//  e    f    g 
		//  i    j    k	
		//  m    n    o 
		Extract_3_3_Matrix( in_Matrix, 0, 3, temp_3_x_3);
		determinant_3 = Determinant_3_x_3(temp_3_x_3);

		return		in_Matrix[0][0] * determinant_0  
				-   in_Matrix[0][1] * determinant_1 
				+   in_Matrix[0][2] * determinant_2
				-   in_Matrix[0][3] * determinant_3; 

	}



	double VolumeOfTetrahedron( const std::vector<Point_3D> &in_TetrahedronFourPoints ) throw (isis::application_exception)
	{
		if ( in_TetrahedronFourPoints.size() != 4 )
		{
				std::stringstream errorString;
				errorString <<
					"Function - " + std::string(__FUNCTION__) +
					" received in_TetrahedronFourPoints vector, which does not have four elements.";
				throw isis::application_exception(errorString.str().c_str());
		}

		//std::cout << std::endl << "Points:" << std::endl;
		//for each ( const Point_3D &i in in_TetrahedronFourPoints ) std::cout << i;
	
		//	X0 Y0 Z0 1
		//	X1 Y1 Z1 1
		//	X2 Y2 Z2 1
		//	X3 Y3 Z3 1

		double pointMatrix[4][4];
		pointMatrix[0][3] = 1.0;
		pointMatrix[1][3] = 1.0;
		pointMatrix[2][3] = 1.0;
		pointMatrix[3][3] = 1.0;

		for ( int i = 0; i < 4; ++i )
		{
			pointMatrix[i][0] = in_TetrahedronFourPoints[i].x;
			pointMatrix[i][1] = in_TetrahedronFourPoints[i].y;
			pointMatrix[i][2] = in_TetrahedronFourPoints[i].z;
		}

		//for ( int i = 0; i < 4; ++i )
		//{
		//	std::cout << std::endl;
		//	for ( int j = 0; j < 4; ++j ) std::cout << pointMatrix[i][j] << "  ";
		//}

		return 1.0/6.0 * abs( Determinant_4_x_4( pointMatrix ));
	}


}  // namespace isis