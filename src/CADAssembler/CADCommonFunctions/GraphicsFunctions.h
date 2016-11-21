#ifndef GRAPHICS_FUNCTIONS_H
#define GRAPHICS_FUNCTIONS_H

#pragma warning( disable : 4290 )  // Turn off the warning "C++ exception specification ignored except to indicate a function is not __declspec(nothrow)"

#include "isis_application_exception.h"
#include "CADCommonDefinitions.h"
#include <vector>
#include <iostream>
#include <map>
#include <list>

namespace isis_CADCommon
{
	const double DEFAULT_TOLERANCE = .001;

	std::ostream& operator<<(std::ostream &output,  const std::vector<double> &in_Vector );
	std::ostream& operator<<(std::ostream &output,  const std::vector<int> &in_Vector );


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct Point_2D
	{
		double x, y;

		Point_2D() : x(0), y(0) {};
		Point_2D(double in_x, double in_y) : x(in_x), y(in_y) {};

	};

	std::ostream& operator<<(std::ostream &output,  const Point_2D &in_Point );

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct Point_3D
	{
		double x, y, z;

		Point_3D() : x(0), y(0), z(0) {};
		Point_3D(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {};
		Point_3D( const Point_3D  &in_Point_3D ): x(in_Point_3D.x), y(in_Point_3D.y), z(in_Point_3D.z) {}; 
		bool operator==( const Point_3D &in_Point_3D )const;
		bool operator<(const Point_3D &in_Point_3D )const;
		std::vector<double> vect() const;
	};

	std::ostream& operator<<(std::ostream &output,  const Point_3D &in_Point );

	std::ostream& operator<<(std::ostream &output, const std::map<int, Point_3D> &in_Point_3D_map );


	bool DuplicatePointInList( const std::list< Point_3D> &in_Points,  double in_Tolerance );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct Line_2D
	{
		Point_2D startPoint;
		Point_2D endPoint;

		Line_2D( const Point_2D &in_StartPoint, const Point_2D &in_EndPoint) : startPoint(in_StartPoint), endPoint(in_EndPoint) {}
	};

	std::ostream& operator<<(std::ostream &output,  const Line_2D &in_Line );
	std::ostream& operator<<(std::ostream &output,  const std::vector<Line_2D> &in_Lines );



	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct GridPoint
	{
		int ID;
		int locationCoordinateSystem_ID;
		Point_3D point; 
		int displacementCoordinateSystem_ID;

		GridPoint():  ID(0),
						locationCoordinateSystem_ID(0),
						point(0,0,0),
						displacementCoordinateSystem_ID(0) {};

		GridPoint(	int in_ID, 
					int in_LocationCoordinateSystem_ID, 
					Point_3D in_Point, 
					int in_DisplacementCoordinateSystem_ID ) :
						ID(in_ID),
						locationCoordinateSystem_ID(in_LocationCoordinateSystem_ID),
						point(in_Point),
						displacementCoordinateSystem_ID(in_DisplacementCoordinateSystem_ID) {};

	};

	std::ostream& operator<<(std::ostream &output, const GridPoint &in_Grid );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool  ValueAreEqual(	double in_Value_1, double in_Value_2, double in_Precision );
	bool  PointsAreEqual_2D(  const Point_2D &in_Point_1, const Point_2D &in_Point_2, double in_Tolerance = DEFAULT_TOLERANCE );
	bool  PointsAreEqual_3D(  const Point_3D &in_Point_1, const Point_3D &in_Point_2, double in_Tolerance = DEFAULT_TOLERANCE );
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		Dot product of two vectors (vectors have x and y coordinates)
	//
	//	Pre-Conditions:
	//		in_Vector_1[0] must contain x coordinate
	//		in_Vector_1[1] must contain y coordinate
	//		in_Vector_2[0] must contain x coordinate
	//		in_Vector_2[1] must contain y coordinate
	//
	//	Post-Conditions:
	//		if either in_Vector_1 or in_Vector_2 does not have a size of 2 
	//			then 
	//				throw isis::application_exception
	//			else
	//				return in_Vector_1[0] * in_Vector_2[0] + in_Vector_1[1] * in_Vector_2[1]
	double DotProduct_2D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//			Dot product of two vectors (vectors have x, y, and z coordinates)
	//
	// Pre-Conditions:
	//			in_Vector_1[0] must contain x coordinate
	//			in_Vector_1[1] must contain y coordinate
	//			in_Vector_1[2] must contain z coordinate
	//			in_Vector_2[0] must contain x coordinate
	//			in_Vector_2[1] must contain y coordinate
	//			in_Vector_2[2] must contain z coordinate
	//
	// Post-Conditions:
	//		if either in_Vector_1 or in_Vector_2 does not have a size of 3 
	//			then 
	//				throw isis::application_exception
	//			else
	//				return in_Vector_1[0] * in_Vector_2[0] + in_Vector_1[1] * in_Vector_2[1] + in_Vector_1[2] * in_Vector_2[2]
	double DotProduct_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Cross product of two vectors (vectors have x, y, and z coordinates)
	//
	// Pre-Conditions:
	//			in_Vector_1[0] must contain x coordinate
	//			in_Vector_1[1] must contain y coordinate
	//			in_Vector_1[2] must contain z coordinate
	//			in_Vector_2[0] must contain x coordinate
	//			in_Vector_2[1] must contain y coordinate
	//			in_Vector_2[2] must contain z coordinate
	//
	// Post-Conditions
	//		if either in_Vector_1 or in_Vector_2 does not have a size of 3 
	//			then 
	//				throw isis::application_exception
	//			else
	//				return in_Vector_1 X in_Vector_2
	//				Note - If the vectors are inline, then the cross product will a vector of all zeros.
	std::vector<double> CrossProduct_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2  ) throw (isis::application_exception);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Magnitude of a 3D Vector (vector has x, y, and z coordinates)
	//
	// Pre-Conditions:
	//
	//		in_Vector_1[0] must contain x coordinate
	//		in_Vector_1[1] must contain y coordinate
	//		in_Vector_1[2] must contain z coordinate
	//
	// Post-Conditions:
	//		if in_Vector does not have a size of 3 
	//			then 
	//				throw isis::application_exception
	//			else
	//				return sqrt( pow(in_Vector[0], 2) + pow(in_Vector[1], 2) + pow(in_Vector[2], 2) )
	double Magnitude_3D( const std::vector<double> &in_Vector ) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Magnitude of a 2D Vector (vector has x, y, and z coordinates)
	//
	// Pre-Conditions:
	//
	//		in_Vector_1[0] must contain x coordinate
	//		in_Vector_1[1] must contain y coordinate
	//		in_Vector_1[2] additional elements are ignored
	//
	// Post-Conditions:
	//		if in_Vector does not have a size of 2
	//			then 
	//				throw isis::application_exception
	//			else
	//				return sqrt( pow(in_Vector[0], 2) + pow(in_Vector[1], 2)  )
	double Magnitude_2D( const std::vector<double> &in_Vector ) throw (isis::application_exception);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Unit Vector (vector has x, y, and z coordinates)
	//
	// Pre-Conditions:
	//
	//		in_Vector[0] must contain x coordinate
	//		in_Vector[1] must contain y coordinate
	//		in_Vector[2] must contain z coordinate
	//
	// Post-Conditions:
	//		if in_Vector.size() != 3  || if the magnitude of in_Vector == 0 
	//			then 
	//				throw isis::application_exception
	//			else
	//				return unit vector
	std::vector<double> UnitVector_3D( const std::vector<double> &in_Vector ) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Angle (in radians) between two vectors (vectors have x, y, and z coordinates)
	//
	// Pre-Conditions:
	//			in_Vector_1[0] must contain x coordinate
	//			in_Vector_1[1] must contain y coordinate
	//			in_Vector_1[2] must contain z coordinate
	//			in_Vector_2[0] must contain x coordinate
	//			in_Vector_2[1] must contain y coordinate
	//			in_Vector_2[2] must contain z coordinate
	//
	// Post-Conditions
	//		if in_A.size() != 3  || in_B.size() != 3 || magnitude  in_A or in_B == 0
	//			then 
	//				throw isis::application_exception
	//			else
	//				return angle in radians, Note: angle < PI
	double AngleBetweenVectors_3D(  const std::vector<double> &in_A, const std::vector<double> &in_B) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Vectors are equal (vectors have x, y, and z coordinates)
	//
	// Pre-Conditions:
	//			in_Vector_1[0] must contain x coordinate
	//			in_Vector_1[1] must contain y coordinate
	//			in_Vector_1[2] must contain z coordinate
	//			in_Vector_2[0] must contain x coordinate
	//			in_Vector_2[1] must contain y coordinate
	//			in_Vector_2[2] must contain z coordinate
	//
	// Post-Conditions
	//		if in_Vector_1.size() != 3  || in_Vector_2.size() != 3 || magnitude of in_Vector_1 or in_Vector_2 == 0
	//			then 
	//				throw isis::application_exception
	//			else
	//				if vectors equal within the range of in_Tolerance
	//					return true;
	//				else
	//					return false;
	bool VectorsEqual_3D( const std::vector<double> &in_Vector_1, const std::vector<double> &in_Vector_2, double in_Tolerance = DEFAULT_TOLERANCE );


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Point is on line?
	//
	// Pre-Conditions:
	//		in_Point must be popululated with x and y coordinate
	//		in_Line  must be populated with two x, y, points
	//		in_Line  can be a zero length line (i.e. startPoint == endPoint, within in_Tolerance) 
	//		in_Line  can be a vertical line (i.e. infinite slope line).
	// Post-Conditions
	//		if ( in_Point on in_Line within in_Tolerance )
	//			then 
	//				return true
	//			else
	//				return false;
	bool PointOnLine_2D( const Point_2D &in_Point, const Line_2D &in_Line, double in_Tolerance = DEFAULT_TOLERANCE );

	void  MultiplyMatrix_3x3_x_3x3( double in_Matrix_1[3][3], double in_Matrix_2[3][3], double out_Matrix[3][3] );

	void  MultiplyMatrix_3x3_x_1x3( const double in_Matrix_1[3][3], const double in_Matrix_2[3], double out_Matrix[3] );

	void  MultiplyMatrix_2x2_x_1x2( const double in_Matrix_1[2][2], const double in_Matrix_2[2], double out_Matrix[2] );

	bool AllMatrixValuesEqualTarget_3X3( double in_Matrix[3][3], double in_Target );
	
	void SetFromToMatrix_3X3( double in_From[3][3], double out_To[3][3] );

	// Forward declare the structure for holding a boost polygon
	struct BoostPolygon_struct;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Point inside polygon or optionally on a line forming polygon?
	//
	// Pre-Conditions:
	//		in_Point must be popululated with x and y coordinate
	//		in_PolygonLines must be populated with at least three lines.
	//
	// Post-Conditions
	//		if ( in_PolygonLines contains a zero length line)
	//			throw isis::application_exception
	//		else 
	//			if ( ( in_Point within in_PolygonLines ) or (if in_IncludePointsOnBoundary == true and
	//				 in_Point on a line forming in_PolygonLines) )
	//				return true
	//			else
	//				return false;
	bool   Point2D_Within_Polygon_2D(	const Point_2D &in_Point, 
										const std::vector<Line_2D> &in_PolygonLines,
										BoostPolygon_struct   *in_BoostPolygon_struct_ptr,  
										bool in_IncludePointsOnBoundary,
										double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Sort the lines representing a polygon such that lines are in the order forming a continuous loop.
	//
	// Pre-Conditions:
	//		in_PolygonLines must be populated with at least three lines.
	//
	//		The order of the points in the lines in in_PolygonLines does not 
	//		matter.  In other words, the start point and end point can be 
	//		switched for any line and the results will not be altered.
	//
	// Post-Conditions
	//		if in_PolygonLines does not have a size of 3 
	//			then 
	//				throw isis::application_exception
	//
	//		if the lines do not form a contigious close polygon
	//			then
	//				throw isis::application_exception
	//
	//		if no exceptions
	//			then
	//				return sorted list of polygons
	std::vector<Line_2D> SortPolygonLines (const std::vector<Line_2D> &in_PolygonLines, double in_Tolerance = DEFAULT_TOLERANCE  );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		Find two vectors forming the intersection between the first and last line in in_SortedPolygonLines.
	//		The two vectors have x and y coordinates.  This is because in_SortedPolygonLines is assumed to be 
	//		planar with a Z axis = 0.  Only the x an y coordinates are used.
	//
	// Pre-Conditions:
	//		The order of the points in the lines in in_PolygonLines does not 
	//		matter.  In other words, the start point and end point can be 
	//		switched for any line and the results will not be altered.
	//
	// Post-Conditions
	//		if in_SortedPolygonLines does not have a size of at least 2 
	//			then 
	//				throw isis::application_exception
	//
	//		if the first line startPoint is not common with either the start or end point of the last line
	//			then
	//				throw isis::application_exception
	//
	//		if no exceptions
	//			then
	//				populate x and y coordinates of out_Vector_1 and out_Vector_2 with vectors, where 
	//					out_Vector_1 =  (first line endPoint) - (first line startPoint) 
	//					out_Vector_1 =  (second line point that does not match first line startPoint) - 
	//										(second line point that does match first line startPoint)
	//					
	void VectorsFormingIntersection_of_FirstAndLastLine_2D( const std::vector<Line_2D> &in_SortedPolygonLines, 	
															  std::vector<double>  &out_Vector_1,
															  std::vector<double>  &out_Vector_2, 
															  double in_Tolerance = DEFAULT_TOLERANCE );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	//								Class TransformationMatrix
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Represent a transformation matrix based on a coordinate system created from the intersection of two 3d vectors.  
	// The coordinate system x axis is in the direction of the first vector.  The second vector defines the plane of 
	// the y axis.  The two vectors must have a common vertex and must not be collinear.
	//
	//   Rotation Matrix
	//		For the case where the new coordinate system is defined relative to the global coordinate system 
	//		(i.e. 1.0i 1.0j 1.0k), the rotation matrix is simply the unit vectors representing the new coordinate system.
	//		The equations follow:
	//
	//		Normally the rotation matrix is defined as		
	//
	//			New_Matrix = Rotation_Matrix * GlobalCordinateSystem_Matrix
	//			rearranging												  
	//                                                                      -1
	//			Rotation_Matrix = New_Matrix * GlobalCordinateSystem_Matrix
	//
	//			but for our case, GlobalCordinateSystem_Matrix is the idenity matrix, which has an inverse of identiy.
	//			therefore
	//				Rotation_Matrix = New_Matrix   // Note - New_Matrix must be in unit vector form
	//
	//	 Offset
	//		x	y	z    Offset from the reference coordinate system
	//
	class TransformationMatrix
	{
		private:
	//		rotationMatrix
	//			Rotation Matrix
	//				[0][0]	 [0][1]  [0][2]    // rotation about x axis
	//				[1][0]	 [1][1]  [1][2]    // rotation about y axis
	//				[2][0]	 [2][1]  [2][2]    // rotation about z axis
	//			Offset
	//				[3][0]  // x offset
	//				[3][1]  // y offset
	//				[3][2]  // z offset
			double rotationMatrix[3][3];  // Rotation Matrix   
			double offset[3];  // x, y, z offset from the gloabla coordiante system to the new coordinate system

			bool m_TransformationPopulated;									

		public:


		// sets m_TransformationPopulated = false;
		TransformationMatrix();

		// in_Vector_XDirection, in_Vector_XYPlane, and in_offset are 3D vectors
		// in_Vector_XDirection and in_Vector_XYPlane must not be equal also,
		// UnitVector(in_Vector_XDirection) and UnitVector(in_Vector_XYPlane) must not be equal.
		void setTransformationMatrix(	const std::vector<double> &in_Vector_XDirection, 
										const std::vector<double> &in_Vector_XYPlane,
										const std::vector<double> &in_offset,
										double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);


		void setTransformationMatrix(	const double in_RotationMatrix[3][3],
										const std::vector<double> &in_Offset,
										double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);


		//	out_Offset is cleared (i.e. remove all elements and set the size to 0) before adding 
		//	the offset values.  This means that out_Offset as inputed to this function
		//	can be empty or contain values.  In either case, this function will return a
		//  out_Offset with three values, which represents the offset portion of the transformation. 
		void getTransformationMatrix(	double out_RotationMatrix[3][3],
										std::vector<double> &out_Offset ) throw (isis::application_exception);


		Point_3D  getTransformedCoordinates( const Point_3D &in_Point_3D ) const;

		void getRotationAngles( double &rotationAboutX,
			  				    double &rotationAboutY,
								double &rotationAboutZ ) const;
		
		void getOffset( double &offset_X,
			  			double &offset_Y,
						double &offset_Z ) const;


		//void getTransformedCoordinates( double in_GlobalCoordinates[3], double out_TransformedCoordinates[3] ) const;

		friend std::ostream& operator<<(std::ostream &output, const TransformationMatrix &in_TransformationMatrix );
	};




	// in_Line must contain two unique points.  The two points define the line.
	// The distance between in_Point and in_Line is returned.  It is acceptable for in_Point to be on
	// in_Line.  For that case, zero would be returned or a very small number (e.g. 2.91923e-008).
	double ShortestDistanceBetweenPointAndLine( const std::vector<Point_3D> &in_Line,
												const Point_3D				&in_Point,
												double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);

	bool LinesCollinear(	const std::vector<Point_3D> &in_Line_1,
							const std::vector<Point_3D> &in_Line_2,
							double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);


	bool VectorsPointInTheSameDirection_3D(		const std::vector<double> &in_Vector_1,
												const std::vector<double> &in_Vector_2,
												double in_Tolerance = DEFAULT_TOLERANCE ) throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Geometry_3D
	/////////////////////////////////////
	class Geometry_3D
	{
		public:
			// in_Point: in global coordinante system
			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception) = 0 ;
			virtual ~Geometry_3D();		

		protected:
			Geometry_3D();	

	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Polygon_3D
	/////////////////////////////////////
	struct BoostPolygon_struct;

	class Polygon_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			std::vector<Point_3D>  polygon_Points;
			TransformationMatrix transformationMatrix;

			// These values are in the local coordinate system
			std::vector<Line_2D> polygon_Lines_Transformed;
			BoostPolygon_struct   *boostPolygon_ptr;

		public:
			//	in_Polygon must contain at least three points
			//  in_Polygon must be ordered such that connecting lines in the given order does not
			//	resutl in crossed lines.
			Polygon_3D(  const std::vector<Point_3D>  in_Polygon_Points,
						 double in_Tolerance, const std::string &in_ErrorSourceString = "" ) throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);

			bool Polygon_3D::pointProjectedOntoPolygonPlaneWithinGeometry(	const Point_3D &in_Point, 
											bool in_IncludePointsOnBoundary,
											double in_Tolerance ) const throw (isis::application_exception);

			TransformationMatrix  getTransformationMatrix();

			virtual ~Polygon_3D();

		friend std::ostream& operator<<(std::ostream &output, const Polygon_3D &in_Polygon_3D );
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Circle_3D
	/////////////////////////////////////
	class Circle_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			Point_3D  center_Point;
			Point_3D  circumference_Point;
			Point_3D  xyPlane_Point;
			double	  radius;
			TransformationMatrix transformationMatrix;
		public:
			//		1) in_Center_Point -		Center of circle
			//		2) in_Circumference_Point - Point on the circle
			//		3) in_xyPlane_Point-		Point defining xy plane of circle.  Does not have to be on the circle.
			//									Must be a different point than 2nd point and must not be collinear with 
			//									a line going from the 1st point and 2nd point. 
			Circle_3D(  const Point_3D &in_Center_Point,
						const Point_3D &in_Circumference_Point,
						const Point_3D &in_xyPlane_Point,
						double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);
			double		getRadius() const;

			virtual ~Circle_3D();

		friend std::ostream& operator<<(std::ostream &output, const Circle_3D &in_Circle_3D );
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class ConcentricCircles_3D
	/////////////////////////////////////
	class ConcentricCircles_3D: public Geometry_3D
	{
		private:
			Circle_3D outerCircle;
			Circle_3D innerCircle;

		public:
			//	in_Center_Point -								Center of circle
			//	in_circumferenceOutterCircle_Point -			Point on the outer circle
			//	in_xyPlane_circumferenceInnterCircle_Point -	Point on the inner circle.  Must not be collinear  with the line 
			//													the first two points.
			ConcentricCircles_3D(	const Point_3D &in_Center_Point,
									const Point_3D &in_circumferenceOutterCircle_Point,
									const Point_3D &in_xyPlane_circumferenceInnterCircle_Point,
									double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);

			virtual ~ConcentricCircles_3D();

		friend std::ostream& operator<<(std::ostream &output, const ConcentricCircles_3D &in_ConcentricCircles_3D );
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// class Cylinder_3D
	/////////////////////////////////////
	class Cylinder_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			Point_3D				startCenterline_Point;
			Point_3D				endCenterline_Point;
			Point_3D				defineRadius_Point;
			e_CylinderGeometryInclusionSpecifier	cylinderGeometryInclusionSpecifier;

		protected:
			double					radius;
			TransformationMatrix	transformationMatrix;

			// These values are in the local coordinate system
			Point_3D				startCenterline_Point_Transformed;
			Point_3D				endCenterline_Point_Transformed; 
		public:
			//	in_Cylinder must contain three points as follows:
			//	1) in_startCenterline_Point(A)- Centerline start point
			//	2) in_endCenterline_Point  (B)- Centerline end point
			//	3) in_defineRadius_Point   (C)- Point defining radius of cylinder.  This point must be on the cylinder
			//									surface, but it does not have to be between points A and B.
			//									It could be anywhere on an infinite cylinder. This point must
			//									not be collinear with the line formed by A-B									
			//	
			//			|---------|	
			//			|		  |	
			//			A---------B	
			//			|		  |	 
			//			|----C----|          
			//
			//	Note - C does not have to be between A and B.  It could be as follows:
			//
			//			|---------|
			//			|		  |
			//			A---------B
			//			|		  |
			//			|---------|  C
			//
			//	C merely defines the distance to the A-B axis, which is the radius of the cylinder.
			//
			//  Note -	when checking for a point being within or on the cylinder, the point must between or on the 
			//			cylinder end surfaces defined by point A and B.
			Cylinder_3D(	const Point_3D &in_startCenterline_Point,
							const Point_3D &in_endCenterline_Point,
							const Point_3D &in_defineRadius_Point,
							e_CylinderGeometryInclusionSpecifier	in_CylinderGeometryInclusionSpecifier,
							double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);
			double		getRadius() const;

			

			virtual ~Cylinder_3D();



		friend std::ostream& operator<<(std::ostream &output, const Cylinder_3D &in_Cylinder_3D );
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// class CylinderSurface_3D
	/////////////////////////////////////
	/*  Deprecated, no longer needed since we now have and us the qualifiers
			GEOMETRY_INTERIOR_AND_BOUNDARY,   
		GEOMETRY_BOUNDARY_ONLY, 
		GEOMETRY_INTERIOR_ONLY,
	class CylinderSurface_3D: public Cylinder_3D
	{
		public:
			//	See the instructions for Cylinder_3D. CylinderSurface_3D works the same as Cylinder_3D except it 
			//	returns the points on the suface of the cylinder instead of within the cylinder.
			CylinderSurface_3D(	const Point_3D &in_startCenterline_Point,
							const Point_3D &in_endCenterline_Point,
							const Point_3D &in_defineRadius_Point,
							double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);


			//  Note - in_IncludePointsOnBoundary does not apply to surfaces.  Points on the surface are always included
			//         for CylinderSurface_3D.
			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);

			virtual ~CylinderSurface_3D();

		    friend std::ostream& operator<<(std::ostream &output, const CylinderSurface_3D &in_CylinderSurface_3D );
	};
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Sphere_3D
	/////////////////////////////////////
	class Sphere_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			Point_3D				center_Point;
			Point_3D				surface_Point;

		protected:
			double					radius;
			TransformationMatrix	transformationMatrix;

			// These values are in the local coordinate system
			Point_3D				center_Point_Transformed;
			Point_3D				surface_Point_Transformed; 
		public:
			//	in_Sphere must contain two points as follows:
			//	1) in_center_Point(A)- Centerline start point
			//	2) in_surface_Point  (B)- Centerline end point
			//	3) in_defineRadius_Point   (C)- Point defining radius of Sphere.  This point must be on the Sphere
			//									surface, but it does not have to be between points A and B.
			//									It could be anywhere on an infinite Sphere. This point must
			//									not be collinear with the line formed by A-B									
			//
			//			   radus	
			//		    A--------->B          
			//
			Sphere_3D(	    const Point_3D &in_center_Point,
							const Point_3D &in_surface_Point,
							double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);
			double		getRadius() const;
		
			virtual ~Sphere_3D();

		friend std::ostream& operator<<(std::ostream &output, const Sphere_3D &in_Sphere_3D );
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class SphereSurface_3D
	/////////////////////////////////////
	class SphereSurface_3D: public Sphere_3D
	{
		public:
			//	See the instructions for Sphere_3D. SphereSurface_3D works the same as Sphere_3D except it 
			//	returns the points on the suface of the Sphere instead of within the Sphere.
			SphereSurface_3D(	const Point_3D &in_center_Point,
								const Point_3D &in_surface_Point,
							double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			//  Note - in_IncludePointsOnBoundary does not apply to surfaces.  Points on the surface are always included
			//         for SphereSurface_3D.
			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);

			virtual ~SphereSurface_3D();

		    friend std::ostream& operator<<(std::ostream &output, const SphereSurface_3D &in_SphereSurface_3D );
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Box_3D
	/////////////////////////////////////
	/* Box not used and not tested
	class Box_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			Point_3D				planarRectangleCorner_Pt_1;
			Point_3D				planarRectangleCorner_Pt_2;
			Point_3D				planarRectangleCorner_Pt_3;
			Point_3D				planarRectangleCorner_Pt_4;
			Point_3D				offsetFromPlanarRectangle_Pt;

		protected:
			TransformationMatrix	transformationMatrix;

			// These values are in the local coordinate system
			Point_3D				planarRectangleCorner_Pt_1_transformed;
			Point_3D				planarRectangleCorner_Pt_2_transformed;
			Point_3D				planarRectangleCorner_Pt_3_transformed;
			Point_3D				planarRectangleCorner_Pt_4_transformed;
			Point_3D				offsetFromPlanarRectangle_Pt_transformed;
		public:
			Box_3D(	const Point_3D	&in_PlanarRectangleCorner_Pt_1,  //Points 1 thru 4 must be ordered (clockwise or counterclockwise)
					const Point_3D	&in_PlanarRectangleCorner_Pt_2,
					const Point_3D	&in_PlanarRectangleCorner_Pt_3,
					const Point_3D	&in_PlanarRectangleCorner_Pt_4,
					const Point_3D	&in_OffsetFromPlanarRectangle_Pt_transformed,  
					double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);
		
			virtual ~Box_3D();

		friend std::ostream& operator<<(std::ostream &output, const Box_3D &in_Box_3D );
	};
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////
	// class Extrusion_3D
	/////////////////////////////////////
	class Extrusion_3D: public Geometry_3D
	{
		private:
			// These values are in the global coordinate system
			Polygon_3D				extrusionPlanarGeometry_polygon;
			Point_3D				offsetFromPolygon_pt;

		protected:
			TransformationMatrix	transformationMatrix;

			// These values are in the transformed coordinate system
			Point_3D				offsetFromPolygon_pt_transformed;

		public:
			Extrusion_3D(	const std::vector<Point_3D>  in_Polygon_Points,
							const Point_3D	&in_offsetFromPolygon,  
							double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);

			virtual bool pointWithinGeometry(  const Point_3D &in_Point, 
											   bool in_IncludePointsOnBoundary,
											   double in_Tolerance = DEFAULT_TOLERANCE ) const throw (isis::application_exception);
		
			virtual ~Extrusion_3D();

		friend std::ostream& operator<<(std::ostream &output, const Extrusion_3D &in_Extrusion_3D );
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This function must allways add to out_GridPointIds_WithinGeometry.  It should never null out
	// out_GridPointIds_WithinGeometry,
	void GetGridPointsWithinGeometry( const std::map<int,GridPoint>		&in_GridPoints_map, 
									  const Geometry_3D					&in_Geometry,
									  e_GeneralGeometryInclusionSpecifier		in_GeneralGeometryInclusionSpecifier,
									  double							in_Tolerance,
									  std::vector<int>					&out_GridPointIds_WithinGeometry) 
																			throw (isis::application_exception);

	double Determinant_2_x_2( double in_Matrix[2][2] );
	double Determinant_3_x_3( double in_Matrix[3][3] );
	bool Positive_Definite_3_x_3( double in_Matrix[3][3] );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	double Determinant_4_x_4 ( double in_Matrix[4][4] );

	//	Description: 
	//		Compute the volume of a tetrahedron based on the four corner points.
	//	Pre-Conditions:
	//		The four corner points must be four unique points.  In other words if one or more points are
	//		coincident, then the behavior of this function is undefined. 
	//	Post-Conditions
	//		If (in_TetrahedronFourPoints.size() != 4 ) 
	//			throw isis::application_exception
	//		If no exceptions
	//			return the volume.
	//		Note - the volume will always be a positive number.
	double VolumeOfTetrahedron( const std::vector<Point_3D> &in_TetrahedronFourPoints ) throw (isis::application_exception);

}  // namespace isis_CADCommon


#endif