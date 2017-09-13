#include "TestFunctions.h"

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "cc_GraphicsFunctions.h"
#include "cc_Nastran.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


/////////////////////////////////////////////////////////////////////////////////
void Tests::DotProduct_2D_test()
{

	// Dot Product  2D
	std::vector<double>  vector_1(2);
	std::vector<double>  vector_2(2);

	vector_1[0] =	2.0; 
	vector_1[1] =	-3.0;
	vector_2[0] =	-.5;
	vector_2[1] =	 4.0;

	double dp = DotProduct_2D(vector_1, vector_2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-13, dp, 0.0001);

}
/////////////////////////////////////////////////////////////////////////////////
void Tests::DotProduct_3D_test()
{
	/// Dot Product  3D
	std::vector<double>  vector_1(3);
	std::vector<double>  vector_2(3);

	vector_1[0] =	 4.0; 
	vector_1[1] =	 2.0;
	vector_1[2] =	-6.0;
	vector_2[0] =	-5.0;
	vector_2[1] =	 3.0;
	vector_2[2] =	-2.0;

	double dp = DotProduct_3D(vector_1, vector_2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, dp, 0.0001);

}
/////////////////////////////////////////////////////////////////////////////////
void Tests::CrossProduct_3D_test()
{

	std::vector<double>  vector_1(3);
	std::vector<double>  vector_2(3);

	vector_1[0] =	 2.0; 
	vector_1[1] =	 1.0;
	vector_1[2] =	-3.0;
	vector_2[0] =	 3.0;
	vector_2[1] =	-1.0;
	vector_2[2] =	 4.0;

	std::vector<double>  vector_out = CrossProduct_3D(vector_1, vector_2);

	//std::cout << std::endl << "Cross Product: " <<  vector_out[0] << "  " <<  vector_out[1] << "  " <<  vector_out[2];
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vector_out[0], 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-17, vector_out[1], 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-5, vector_out[2], 0.0001);
}

void Tests::Magnitude_3D_test()
{
	std::vector<double>  vector_1(3);

	vector_1[0] =	 3.0; 
	vector_1[1] =	 2.0;
	vector_1[2] =	-6.0;

	// Answer should be 7.0
	double m = Magnitude_3D(vector_1);
	//std::cout << std::endl << "Magnitude 3D: " <<   m;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7, m, 0.0001);
}

/////////////////////////////////////////////////////////////////////////////////
void Tests::UnitVector_3D_test()
{

	///** Magnitude 3D
	std::vector<double>  vector_1(3);

	vector_1[0] =	 1.0; 
	vector_1[1] =	 5.0;
	vector_1[2] =	 3.0;

	std::vector<double>  vector_out = UnitVector_3D(vector_1);

	// Answer should be 0.169030851	0.845154255	0.507092553
	//std::cout << std::endl << "Unit Vector: " <<  vector_out[0] << "  " <<  vector_out[1] << "  " <<  vector_out[2];
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.16903, vector_out[0], 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.84515, vector_out[1], 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.50709, vector_out[2], 0.0001);

}
/////////////////////////////////////////////////////////////////////////////////

void Tests::PointOnLine_2D_test()
{

	bool b = PointOnLine_2D( Point_2D(-.0001, 10),Line_2D( Point_2D(0,10),   Point_2D(10,10)), .001 );

	CPPUNIT_ASSERT(b);

}
/////////////////////////////////////////////////////////////////////////////////
#if 0
	////////////////////////////////
	//	struct BoostPolygon_struct
	///////////////////////////////
	struct BoostPolygon_struct
	{
		typedef boost::geometry::model::d2::point_xy<double> boostpoint_type;
		typedef boost::geometry::model::polygon<boostpoint_type> boostpolygon_type;
			
		boostpolygon_type boostPolygon;

	};
static void Point2D_Within_Polygon_2D_test(bool in_IncludePointsOnBoundary)
{
	// WARNING - Before running this function, make sure that BoostPolygon_struct has not changed
	// the above version should agree verbatim with the GraphicsFunctions.cpp version

	std::vector<Point_3D> points;
	points.push_back( Point_3D(0,0,0) );
	points.push_back( Point_3D(10,0,0) );
	points.push_back( Point_3D(10,10,0) );
	points.push_back( Point_3D(0,10,0) );
	
	std::vector<Line_2D> PolygonLines;
	for ( std::vector<Point_3D>::const_iterator i(points.begin()); i != points.end(); ++i )
	{
		std::vector<Point_3D>::const_iterator  j = i;
		++j;
		if ( j != points.end() )
			PolygonLines.push_back( Line_2D( Point_2D(i->x,i->y),   Point_2D(j->x,j->y) ));
		else
			PolygonLines.push_back( Line_2D( Point_2D(i->x,i->y), Point_2D( points.begin()->x,points.begin()->y)) );
	}

	::BoostPolygon_struct   boostPolygon_struct;
	// Populate boost polygon
	for ( std::vector<Point_3D>::const_iterator i(points.begin()); i != points.end(); ++i )
	{
		boost::geometry::exterior_ring(boostPolygon_struct.boostPolygon).push_back(boost::geometry::model::d2::point_xy<double>(i->x,i->y));
	} 
	// Correct the plolygon - clockwise orientiatin, closed loop (i.e. last point repeated to close the loop
	boost::geometry::correct(boostPolygon_struct.boostPolygon);

	if (   boost::geometry::intersects(boostPolygon_struct.boostPolygon) )
		std::cout << std::endl << "ERROR -->Intersects";
	else
		std::cout << std::endl << "Does Not Intersect";


	std::vector<Line_2D> PolygonLinesSorted = SortPolygonLines (PolygonLines, .001 );

	if ( Point2D_Within_Polygon_2D( Point_2D(-.001, 10.001), PolygonLinesSorted, &boostPolygon_struct, true, .001) )
		std::cout << std::endl << "Inside/On Polygon = TRUE";
	else
		std::cout << std::endl << "Inside/On Polygon = FALSE";

	if ( Point2D_Within_Polygon_2D( Point_2D(-.001, 10.001), PolygonLinesSorted,&boostPolygon_struct, false , .001) )
		std::cout << std::endl << "Inside (but not on) Polygon = TRUE";
	else
		std::cout << std::endl << "Inside (but not on) = FALSE";

	if ( Point2D_Within_Polygon_2D( Point_2D(1.0, 6), PolygonLinesSorted, &boostPolygon_struct,true, .001) )
		std::cout << std::endl << "Inside/On Polygon = TRUE";
	else
		std::cout << std::endl << "Inside/On Polygon = FALSE";

	if ( Point2D_Within_Polygon_2D( Point_2D(1.0, 6), PolygonLinesSorted, &boostPolygon_struct,false, .001) )
		std::cout << std::endl << "Inside (but not on) = TRUE";
	else
		std::cout << std::endl << "Inside (but not on) = FALSE";

	std::vector<double> vector_x_axis;
	std::vector<double> vector_y_axis;

	VectorsFormingIntersection_of_FirstAndLastLine_2D(PolygonLinesSorted, vector_x_axis, vector_y_axis);

	std::cout << std::endl << "vector_x_axis: " <<  std::setw(10) <<  vector_x_axis[0] << 
													std::setw(10) <<  vector_x_axis[1];

	std::cout << std::endl << "vector_y_axis: " <<  std::setw(10) <<  vector_y_axis[0] << 
													std::setw(10) <<  vector_y_axis[1]; 
}

void Test::Point2D_Within_Polygon_2D_test()
{
	Point2D_Within_Polygon_2D_test(true);
}
#endif
/////////////////////////////////////////////////////////////////////////////////
void Tests::AngleBetweenVectors_test()
{
	
	std::vector<double>  vector_1(3);
	vector_1[0] =	 0.0; 
	vector_1[1] =	 1.0;
	vector_1[2] =	 0.0;

	std::vector<double>  vector_2(3);
	vector_2[0] =	 0; 
	vector_2[1] =	 -5.0;    //-1.41421;
	vector_2[2] =	 5.0;    //1.41421;


	/* 
	std::vector<double>  vector_1(3);
	vector_1[0] =	 2.0; 
	vector_1[1] =	 2.0;
	vector_1[2] =	 12.0;

	std::vector<double>  vector_2(3);
	vector_2[0] =	 1.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 6.0;
	*/

	double a = AngleBetweenVectors_3D(vector_1, vector_2);
	//std::cout << std::endl << "AngleBetweenVectors: " << a;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.35619, a, 0.0001);
	

}
/////////////////////////////////////////////////////////////////////////////////
void Tests::TransformationMatrix_test()
{
	std::vector<double>  vector_1(3);
	// xy plane
	//vector_1[0] =	 1.0; 
	//vector_1[1] =	 1.0;
	//vector_1[2] =	 0.0;
	// yz plane
	//vector_1[0] =	 0.0; 
	//vector_1[1] =	 1.0;
	//vector_1[2] =	 1.0;
	// xz plane
	vector_1[0] =	 1.0; 
	vector_1[1] =	 0.0;
	vector_1[2] =	 1.0;


	std::vector<double>  vector_2(3);
	// xy plane
	//vector_2[0] =	 -1; 
	//vector_2[1] =	 1;
	//vector_2[2] =	 0;
	// yz plane
	//vector_2[0] =	 0.0; 
	//vector_2[1] =	 -1.0;
	//vector_2[2] =	 1.0;
	// xz plane
	vector_2[0] =	 -1.0; 
	vector_2[1] =	 0.0;
	vector_2[2] =	 1.0;

	std::vector<double>  offset(3);
	offset[0] =	 0; 
	offset[1] =	 0;
	offset[2] =	 0;

	//std::cout << std::endl << "Vector 1: " <<  std::setw(10) << vector_1[0] <<  std::setw(10) << vector_1[1] << std::setw(10) << vector_1[2];
	//std::cout << std::endl << "Vector 2: " <<  std::setw(10) << vector_2[0] << std::setw(10)  << vector_2[1] << std::setw(10) << vector_2[2];
	//std::cout << std::endl << "offset  : " <<  std::setw(10) << offset[0]   << std::setw(10)  << offset[1]   << std::setw(10) << offset[2];
	
	TransformationMatrix  transformationMatrix_1;

	transformationMatrix_1.setTransformationMatrix( vector_1, vector_2, offset );

	//std::cout << std::endl << transformationMatrix_1;
	//std::cout << std::endl;

	Point_3D  TestCoords;
	// xy plane
	//TestCoords.x = 03;
	//TestCoords.y = 3;
	//TestCoords.z = 0;
	// yz plane
	//TestCoords.x = 0;
	//TestCoords.y = 3;
	//TestCoords.z = 3;
	// xz plane
	TestCoords.x = 0;
	TestCoords.y = 0;
	TestCoords.z = 4;

	Point_3D  TransformedCoords;

	TransformedCoords = transformationMatrix_1.getTransformedCoordinates(  TestCoords  );

	//std::cout << std::endl << "TestCoords x, y, z: " <<  TestCoords ;
	//std::cout << std::endl << "Transposed x, y, z: " <<  TransformedCoords;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.82843, TransformedCoords.x, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.82843, TransformedCoords.y, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, TransformedCoords.z, 0.0001);
}
/////////////////////////////////////////////////////////////////////////////////
void Tests::ComputeRotationAngles_test()
{
	std::vector<double>  vector_1(3);
	std::vector<double>  vector_2(3);

	//////////////////
	// No Rotation
	///////////////////
	/*
	// r[2,0] = 0   correct
	vector_1[0] =	 1.0; 
	vector_1[1] =	 0.0;
	vector_1[2] =	 0.0;

	vector_2[0] =	 0.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 0.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  45 degrees counter clockwise about z
	/////////////////////////////////////////////////
	// r[2,0] = 0      correct
	/*
	vector_1[0] =	 1.0; 
	vector_1[1] =	 1.0;
	vector_1[2] =	 0.0;

	vector_2[0] =	 -1.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 0.0;
	*/
	//////////////////////////////////////////////////
	// Rotation  45 degrees clockwise about z
	/////////////////////////////////////////////////
	/*
	// r[2,0] = 0      correct
	vector_1[0] =	 1.0; 
	vector_1[1] =	 -1.0;
	vector_1[2] =	 0.0;

	vector_2[0] =	 1.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 0.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  45 about Y
	/////////////////////////////////////////////////
	/*
	// r[2,0] = 0     correct
	vector_1[0] =	 1.0; 
	vector_1[1] =	 0.0;
	vector_1[2] =	 1.0;

	vector_2[0] =	 -1.0; 
	vector_2[1] =	 0.0;
	vector_2[2] =	 1.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  -45 about Y
	/////////////////////////////////////////////////
	/*
	// r[2,0] = 0     correct
	vector_1[0] =	 1.0; 
	vector_1[1] =	 0.0;
	vector_1[2] =	 -1.0;

	vector_2[0] =	 1.0; 
	vector_2[1] =	 0.0;
	vector_2[2] =	 1.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  ??
	/////////////////////////////////////////////////
	// r[2,0] = 1   Correct
	/*
	vector_1[0] =	 0.0; 
	vector_1[1] =	 1.0;
	vector_1[2] =	 1.0;

	vector_2[0] =	 0.0; 
	vector_2[1] =	 -1.0;
	vector_2[2] =	 1.0;	
	*/
	//////////////////////////////////////////////////
	// Rotation  -????
	/////////////////////////////////////////////////
	// r[2,0] = 1   correct
	/*
	vector_1[0] =	 0.0; 
	vector_1[1] =	 -1.0;
	vector_1[2] =	 -1.0;

	vector_2[0] =	 0.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 -1.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  -????
	/////////////////////////////////////////////////
	// r[2,0] = -1   correct
	/*
	vector_1[0] =	 0.0; 
	vector_1[1] =	 -1.0;
	vector_1[2] =	 1.0;

	vector_2[0] =	 0.0; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 1.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  small angle counter clockwise about z
	/////////////////////////////////////////////////
	// r[2,0] = 0  correct
	/*
	vector_1[0] =	 1.0; 
	vector_1[1] =	 .00001;
	vector_1[2] =	 0;

	vector_2[0] =	 -.00001;; 
	vector_2[1] =	 1.0;
	vector_2[2] =	 0.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  small angle counter clockwise about z
	/////////////////////////////////////////////////
	// r[2,0] = 0
	/*
	double  value_1 = .00000001;
	double  value_2 = .99999999;

	vector_1[0] =	 value_1;
	vector_1[1] =	 value_2; 
	vector_1[2] =	 0;

	vector_2[0] =	 -value_2; 
	vector_2[1] =	 value_1;
	vector_2[2] =	 0.0;
	*/

	//////////////////////////////////////////////////
	// Rotation  small angle counter clockwise about x
	/////////////////////////////////////////////////
	// r[2,0] = 0
	///*
	double  value_1 = .00000001;
	double  value_2 = .99999999;

	vector_1[0] =	 value_1;
	vector_1[1] =	 value_2; 
	vector_1[2] =	 value_1;

	vector_2[0] =	 value_1; 
	vector_2[1] =	 -value_1;
	vector_2[2] =	 value_2;
	//*/


	std::vector<double>  offset(3);
	offset[0] =	 0; 
	offset[1] =	 0;
	offset[2] =	 0;

	//std::cout << std::endl << "Vector 1: " <<  std::setw(10) << vector_1[0] <<  std::setw(10) << vector_1[1] << std::setw(10) << vector_1[2];
	//std::cout << std::endl << "Vector 2: " <<  std::setw(10) << vector_2[0] << std::setw(10)  << vector_2[1] << std::setw(10) << vector_2[2];
	//std::cout << std::endl << "offset  : " <<  std::setw(10) << offset[0]   << std::setw(10)  << offset[1]   << std::setw(10) << offset[2];
	
	TransformationMatrix  transformationMatrix_1;

	transformationMatrix_1.setTransformationMatrix( vector_1, vector_2, offset );

	//std::cout << std::endl << transformationMatrix_1;
	//std::cout << std::endl;

	double rot_x, rot_y, rot_z;

	transformationMatrix_1.getRotationAngles( rot_x, rot_y, rot_z );

	//std::cout << std::endl << "Rotation Angles x, y. z: " <<  rot_x << "  " << rot_y << "  " << rot_z;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.5708, rot_x, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.5708, rot_y, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, rot_z, 0.0001);
}


/////////////////////////////////////////////////////////////////////////////////
void Tests::DuplicatePointInList_test()
{
	////////////////////////////////
	// DuplicatePointInList
	////////////////////////////////
	double precision = .001;
	std::list< Point_3D> Points_set;
	Points_set.push_back(Point_3D(0, 0, 0));
	Points_set.push_back(Point_3D(-5, 1, 4));
	Points_set.push_back(Point_3D(.01, 0, 0));
	Points_set.push_back(Point_3D(6, 7, 1));

	CPPUNIT_ASSERT(!DuplicatePointInList( Points_set, precision ));
}
/////////////////////////////////////////////////////////////////////////////////
void Tests::PointsWithinPolygon_test()
{

	////////////////////////////////
	// Points Within Polygon
	////////////////////////////////

		
	std::map<int,GridPoint> gridPoints_map;
	std::vector<Point_3D>   polygon;
	double precision = .0001;
	std::vector<int>       gridPointIds_WithinPolygon;
		
	/*
	gridPoints_map[1] = GridPoint(1, 2, Point_3D(0, 0, 0), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(5, 0, 0), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(5, 5, 0), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(0, 5, 0), 2);	

	polygon.push_back( Point_3D( -1,-1, 0));   // should yield 1 2 3 4
	polygon.push_back( Point_3D( 6,-1, 0));
	polygon.push_back( Point_3D( 6, 6, 0));
	polygon.push_back( Point_3D(-1, 6, 0));
	*/

	/*
	gridPoints_map[1] = GridPoint(1, 2, Point_3D(2.1, 3.5, 0), 2); // should yield 1
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(0, 6, 0), 2); 
	//gridPoints_map[3] = GridPoint(3, 2, Point_3D(5, 5, 0), 2);
	//gridPoints_map[4] = GridPoint(4, 2, Point_3D(0, 5, 0), 2);		

	polygon.push_back( Point_3D( 0, 5, 0));
	polygon.push_back( Point_3D( 0, 0, 0));
	polygon.push_back( Point_3D( 5, 0, 0));
	polygon.push_back( Point_3D( 5, 5, 0));
	*/

	/*
	polygon.push_back( Point_3D( 1, 0, 0));
	polygon.push_back( Point_3D( 5, 0, 0));
	polygon.push_back( Point_3D( 5, 5, 0));
	polygon.push_back( Point_3D( 1, 5, 0));
	*/
		
	/*
	gridPoints_map[1] = GridPoint(1, 2, Point_3D(6, 5, 1), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(6, 5.5, 8), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(10, 5, 20), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(5, 5, 20), 2);	

		
	polygon.push_back( Point_3D( 6, 5, 0));  // should be 1 3 4
	polygon.push_back( Point_3D( 10, 5, 0));
	polygon.push_back( Point_3D( 10, 5, 20));
	polygon.push_back( Point_3D( 5, 5, 20));
	*/
	
		
	// Slanted Plane
		
	gridPoints_map[1] = GridPoint(1, 2, Point_3D(3, 3, 18), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(6, 5.1, 8), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(10, 5, 20), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(5, 5, 20), 2);	


	polygon.push_back( Point_3D( 1, 1, 0));  // should be 1 4
	polygon.push_back( Point_3D( 6, 6, 0));
	polygon.push_back( Point_3D( 6, 6, 20));
	polygon.push_back( Point_3D( 1, 1, 20));	
		
		
	// Slanted negative Plane

/*
	gridPoints_map[1] = GridPoint(1, 2, Point_3D(-3, 3, 18), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(6, 5.1, 8), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(10, 5, 20), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(5, 5, 20), 2);	

	polygon.push_back( Point_3D( -1, 1, 0));  // should be 1
	polygon.push_back( Point_3D( -6, 6, 0));
	polygon.push_back( Point_3D( -6, 6, 20));
	polygon.push_back( Point_3D( -1, 1, 20));
		
	//GetGridPointsWithinPolygon( gridPoints_map,polygon, precision, gridPointIds_WithinPolygon);

*/

	Polygon_3D polygon_obj(polygon, precision ); 

	//polygon_obj.pointWithinGeometry( gridPoints_map[2].point, true, precision ); 

		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										polygon_obj,
										GEOMETRY_INTERIOR_AND_BOUNDARY,
										precision,
										gridPointIds_WithinPolygon );

	CPPUNIT_ASSERT_EQUAL(2, (int)gridPointIds_WithinPolygon.size());
	CPPUNIT_ASSERT_EQUAL(1, gridPointIds_WithinPolygon[0]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_WithinPolygon[1]);

	/*std::cout << std::endl << "IDs Within Polygon";
	for ( std::vector<int>::const_iterator i( gridPointIds_WithinPolygon.begin()); i != gridPointIds_WithinPolygon.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/

}

/////////////////////////////////////////////////////////////////////////////////
static void GetGridPointsWithinCircle_test(bool in_IncludePointsOnBoundary)
{

	/////////////////////////////
	// GetGridPointsWithinCircle
	/////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;
	double precision = .0001;
	std::vector<int>       gridPointIds_WithinCircle;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(0, 0, 10), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(25, 21 - .0001, 10), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(25.5, 20.5, 10.0001), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(25.5, 20, 10), 2);	
	gridPoints_map[5] = GridPoint(5, 2, Point_3D(25.00, 19, 10), 2);	

	Circle_3D   circle(  Point_3D( 25,20, 10),    // should get back 2 3 4 5
							Point_3D( 26,20, 10),
							Point_3D( 25,22, 10));

	//Circle_3D   circle(  Point_3D( 25,20, 10),  // second point identical to first, should throw exception
	//					 Point_3D( 25,20, 10),
	//					 Point_3D( 25,22, 10));

	//Circle_3D   circle(  Point_3D( 25,20, 10),  // third point identical to first, should throw exception
	//					 Point_3D( 26,20, 10),
	//					 Point_3D( 25,20, 10));

	//Circle_3D   circle(  Point_3D( 25,20, 10),  // 3rd point colinear with line from 1st and 2nd points, should throw exception
	//					 Point_3D( 26,20, 10),
	//					 Point_3D( 27,20, 10));
	
		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										circle,
									 	GEOMETRY_INTERIOR_AND_BOUNDARY,	
										precision,
										gridPointIds_WithinCircle );

	CPPUNIT_ASSERT_EQUAL(4, (int)gridPointIds_WithinCircle.size());
	CPPUNIT_ASSERT_EQUAL(2, gridPointIds_WithinCircle[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_WithinCircle[1]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_WithinCircle[2]);
	CPPUNIT_ASSERT_EQUAL(5, gridPointIds_WithinCircle[3]);
	/*std::cout << std::endl << "IDs Within Circle";
	for ( std::vector<int>::const_iterator i( gridPointIds_WithinCircle.begin()); i != gridPointIds_WithinCircle.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/
		

}

void Tests::GetGridPointsWithinCircle_test()
{
	::GetGridPointsWithinCircle_test(true);
}
/////////////////////////////////////////////////////////////////////////////////
static void GetGridPointsBetweenOrOnConcentricCircles_test(bool in_IncludePointsOnBoundary)
{

	////////////////////////////////////////////
	// GetGridPointsBetweenOrOnConcentricCircles
	////////////////////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;
	double precision = .0001;
	std::vector<int>       gridPointIds_BetweenOrOnConcentricCircles;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(0, 0, 10), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(25, 21 - .0001, 10), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(25.5, 20.5, 10.0001), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(25.5, 20, 10), 2);	
	gridPoints_map[5] = GridPoint(5, 2, Point_3D(25.00, 19, 10), 2);	

	ConcentricCircles_3D concentricCircles(
							Point_3D( 25,20, 10),    // should get back 2 3 4 5
							Point_3D( 26,20, 10),
							Point_3D( 25,20.5, 10));
	
		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										concentricCircles,
										GEOMETRY_INTERIOR_AND_BOUNDARY,
										precision,
										gridPointIds_BetweenOrOnConcentricCircles );

	CPPUNIT_ASSERT_EQUAL(4, (int)gridPointIds_BetweenOrOnConcentricCircles.size());
	CPPUNIT_ASSERT_EQUAL(2, gridPointIds_BetweenOrOnConcentricCircles[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_BetweenOrOnConcentricCircles[1]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_BetweenOrOnConcentricCircles[2]);
	CPPUNIT_ASSERT_EQUAL(5, gridPointIds_BetweenOrOnConcentricCircles[3]);
/*	std::cout << std::endl << "IDs Between/Within Circle";
	for ( std::vector<int>::const_iterator i( gridPointIds_BetweenOrOnConcentricCircles.begin()); i != gridPointIds_BetweenOrOnConcentricCircles.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/

}

void Tests::GetGridPointsBetweenOrOnConcentricCircles_test()
{
	::GetGridPointsBetweenOrOnConcentricCircles_test(true);
}
/////////////////////////////////////////////////////////////////////////////////
void Tests::ShortestDistanceBetweenPointAndLine_test()
{

	////////////////////////////////////////////
	// ShortestDistanceBetweenPointAndLine
	////////////////////////////////////////////	

	std::vector<Point_3D> line;
	double precision = .001;
	
	line.push_back( Point_3D(-5,5,0));
	line.push_back( Point_3D(-10,10,0));

	Point_3D  point(-10.001,10,0);  // on second point
	//Point_3D  point(-10,5,0);
	//Point_3D  point(-6,6,0);  // on the line

	double distance = ShortestDistanceBetweenPointAndLine(line, point, precision );
	/*std::cout << std::endl << "Line, start point, end point: " << line[0] << "  " << line[1];
	std::cout << std::endl << "Point: " << point;
	std::cout << std::endl << "Distance: " << distance;*/

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, distance, 0.001);
		

}
/////////////////////////////////////////////////////////////////////////////////
static void OnOrWithinCylinder_test(e_GeneralGeometryInclusionSpecifier	in_GeneralGeometryInclusionSpecifier)
{

	////////////////////////////////////////////
	// On or Within Cylinder
	////////////////////////////////////////////		

	std::map<int,GridPoint> gridPoints_map;

	double precision = .001;
	std::vector<int>       gridPointIds_OnCylinderSurface;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(0, 0, 10), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(358.9941, 104.2714, 165.0611),  2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(307.16, 75 + 30, 267.98), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(338.0405, 95.0519, 169.8683), 2);	
	gridPoints_map[5] = GridPoint(5, 2, Point_3D(350, 80, 170), 2);	// interior of cylinder

	Cylinder_3D  cyclinder(
							Point_3D( 307.16, 75, 267.98),    // should get back 2 3 4 5
							Point_3D( 385.66, 75, 132.02),
							Point_3D( 358.9941, 104.2714, 165.0611),
							GEOMETRY_INCLUDE_END_CAP);

	//cylinder.push_back( Point_3D( 307.16, 75, 267.98));  // 1st point, first point at center of cylinder
	//cylinder.push_back( Point_3D( 385.66, 75, 132.02));	// 2nd point, second point at center of cylinder
	////cylinder.push_back( Point_3D( 25,20, 10));  // 2nd pointCheck identical points
	//cylinder.push_back( Point_3D( 358.9941, 104.2714, 165.0611));  // 3rd point, on cylinder surface
 	////cylinder.push_back( Point_3D( 25,20, 10));  // 3rd Check identical points
	////cylinder.push_back( Point_3D( 27,20, 10));   // 3rd Check colinear with line from 1st and 2nd points
		
	GetGridPointsWithinGeometry (	gridPoints_map, 
									cyclinder,
									in_GeneralGeometryInclusionSpecifier,
									precision,
									gridPointIds_OnCylinderSurface );

	CPPUNIT_ASSERT_EQUAL(3, (int)gridPointIds_OnCylinderSurface.size());
	CPPUNIT_ASSERT_EQUAL(2, gridPointIds_OnCylinderSurface[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_OnCylinderSurface[1]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_OnCylinderSurface[2]);
	/*std::cout << std::endl << "IDs on/within cylinder";
	for ( std::vector<int>::const_iterator i( gridPointIds_OnCylinderSurface.begin()); i != gridPointIds_OnCylinderSurface.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/			
}

void Tests::OnOrWithinCylinder_test()
{
	::OnOrWithinCylinder_test(isis_CADCommon::GEOMETRY_BOUNDARY_ONLY);
}
/////////////////////////////////////////////////////////////////////////////////
static void OnCylinderSurface_test(e_GeneralGeometryInclusionSpecifier	in_GeneralGeometryInclusionSpecifier)
{

	////////////////////////////////////////////
	// On Cylinder Surface
	////////////////////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;

	double precision = .001;
	std::vector<int>       gridPointIds_OnCylinderSurface;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(0, 0, 10), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(358.9941, 104.2714, 165.0611),  2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(307.16, 75 + 30, 267.98), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(338.0405, 95.0519, 169.8683), 2);	

	Cylinder_3D  cyclinderSurface(
							Point_3D( 307.16, 75, 267.98),		// should get back 2 3 4 for GEOMETRY_INCLUDE_END_CAP
							Point_3D( 385.66, 75, 132.02),		// should get back 2 4 for GEOMETRY_EXCLUDE_END_CAP
							Point_3D( 358.9941, 104.2714, 165.0611),
							GEOMETRY_INCLUDE_END_CAP);

	//cylinder.push_back( Point_3D( 307.16, 75, 267.98));  // 1st point, first point at center of cylinder
	//cylinder.push_back( Point_3D( 385.66, 75, 132.02));	// 2nd point, second point at center of cylinder
	////cylinder.push_back( Point_3D( 25,20, 10));  // 2nd pointCheck identical points
	//cylinder.push_back( Point_3D( 358.9941, 104.2714, 165.0611));  // 3rd point, on cylinder surface
 	////cylinder.push_back( Point_3D( 25,20, 10));  // 3rd Check identical points
	////cylinder.push_back( Point_3D( 27,20, 10));   // 3rd Check colinear with line from 1st and 2nd points
		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										cyclinderSurface,
										GEOMETRY_BOUNDARY_ONLY,
										precision,
										gridPointIds_OnCylinderSurface );

	CPPUNIT_ASSERT_EQUAL(3, (int)gridPointIds_OnCylinderSurface.size());
	CPPUNIT_ASSERT_EQUAL(2, gridPointIds_OnCylinderSurface[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_OnCylinderSurface[1]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_OnCylinderSurface[2]);
/*	std::cout << std::endl << "IDs On cylinder";
	for ( std::vector<int>::const_iterator i( gridPointIds_OnCylinderSurface.begin()); i != gridPointIds_OnCylinderSurface.end(); ++i)
	{
		std::cout << std::endl << *i;
	}	*/
}

void Tests::OnCylinderSurface_test()
{
	::OnCylinderSurface_test(GEOMETRY_BOUNDARY_ONLY);
}
/////////////////////////////////////////////////////////////////////////////////
static void WithinSphere_test(bool in_IncludePointsOnBoundary)
{

	////////////////////////////////////////////
	// Within Sphere 
	////////////////////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;

	double precision = .001;
	std::vector<int>       gridPointIds_WithinSphere;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(5, 6, 1), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(5.1, 6.1, .1 ),  2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(6, 6, 0 ), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(5, 7.00099999, 0), 2);	

	Sphere_3D  sphere(
							Point_3D(  5, 6, 0 ),    // should get back ???
							Point_3D(  5, 7, 0 ));
	
	//sphere.pointWithinGeometry( Point_3D(5, 7.001, 0),true, precision );
		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										sphere,
										GEOMETRY_INTERIOR_AND_BOUNDARY,
										precision,
										gridPointIds_WithinSphere );

	CPPUNIT_ASSERT_EQUAL(4, (int)gridPointIds_WithinSphere.size());
	CPPUNIT_ASSERT_EQUAL(1, gridPointIds_WithinSphere[0]);
	CPPUNIT_ASSERT_EQUAL(2, gridPointIds_WithinSphere[1]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_WithinSphere[2]);
	CPPUNIT_ASSERT_EQUAL(4, gridPointIds_WithinSphere[3]);
/*	std::cout << std::endl << "IDs within Sphere";
	for ( std::vector<int>::const_iterator i( gridPointIds_WithinSphere.begin()); i != gridPointIds_WithinSphere.end(); ++i)
	{
		std::cout << std::endl << *i;
	}	*/


}

void Tests::WithinSphere_test()
{
	::WithinSphere_test(isis_CADCommon::GEOMETRY_BOUNDARY_ONLY);
}
/////////////////////////////////////////////////////////////////////////////////

void Tests::OnSphereSurface_test()
{

	////////////////////////////////////////////
	// On Sphere Surface 
	////////////////////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;

	double precision = .001;
	std::vector<int>       gridPointIds_WithinsphereSurface;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(5, 6.001, 1.00), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(5.1, 6.1, .1 ),  2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(6, 6, 0 ), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(338.0405, 95.0519, 169.8683), 2);	

	SphereSurface_3D  sphereSurface(
							Point_3D( 5, 6, 0 ),    // should get back ???
							Point_3D(  5, 7, 0 ));
	
	sphereSurface.pointWithinGeometry(Point_3D(5, 6.01, 1.00), true, precision);

		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										sphereSurface,
										GEOMETRY_INTERIOR_AND_BOUNDARY,
										precision,
										gridPointIds_WithinsphereSurface );

	CPPUNIT_ASSERT_EQUAL(2, (int)gridPointIds_WithinsphereSurface.size());
	CPPUNIT_ASSERT_EQUAL(1, gridPointIds_WithinsphereSurface[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_WithinsphereSurface[1]);
	/*std::cout << std::endl << "IDs within sphereSurface";
	for ( std::vector<int>::const_iterator i( gridPointIds_WithinsphereSurface.begin()); i != gridPointIds_WithinsphereSurface.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/	
	

}
/////////////////////////////////////////////////////////////////////////////////

static void WithinExtrusion_test(bool in_IncludePointsOnBoundary)
{
	////////////////////////////////////////////
	// Within Extrusion  
	////////////////////////////////////////////	

	std::map<int,GridPoint> gridPoints_map;

	double precision = .001;
	std::vector<int>       gridPointIds_WithinExtrusion;

	gridPoints_map[1] = GridPoint(1, 2, Point_3D(5, 0, 0 ), 2); 
	gridPoints_map[2] = GridPoint(2, 2, Point_3D(1, -1, 1), 2); 
	gridPoints_map[3] = GridPoint(3, 2, Point_3D(3, 3, 4.99), 2);
	gridPoints_map[4] = GridPoint(4, 2, Point_3D(5, 5, 20), 2);	

	std::vector<Point_3D>   polygon;

	polygon.push_back( Point_3D( 5, 0, 0));  
	polygon.push_back( Point_3D( 5, 5, 0));
	polygon.push_back( Point_3D( 0, 5, 0));
	polygon.push_back( Point_3D( 0, 0, 0));

	Point_3D  offsetPoint (0, 0, 5 );

	Extrusion_3D  extrusion( polygon,     // should get back ???
								offsetPoint,    
								precision);
	
	//extrusion.pointWithinGeometry(Point_3D(4, 3, 1.00), true, precision);

		
	GetGridPointsWithinGeometry (	gridPoints_map, 
										extrusion,
										GEOMETRY_INTERIOR_AND_BOUNDARY,
										precision,
										gridPointIds_WithinExtrusion );

	CPPUNIT_ASSERT_EQUAL(2, (int)gridPointIds_WithinExtrusion.size());
	CPPUNIT_ASSERT_EQUAL(1, gridPointIds_WithinExtrusion[0]);
	CPPUNIT_ASSERT_EQUAL(3, gridPointIds_WithinExtrusion[1]);

	/*std::cout << std::endl << "IDs within extrusionSurface";
	for ( std::vector<int>::const_iterator i( gridPointIds_WithinExtrusion.begin()); i != gridPointIds_WithinExtrusion.end(); ++i)
	{
		std::cout << std::endl << *i;
	}*/	
}

void Tests::WithinExtrusion_test()
{
	::WithinExtrusion_test(true);
}
#if 0
/////////////////////////////////////////////////////////////////////////////////
//	void ComputeStressValues( double  in_StressTensor[3][3], 
//							  double &out_VonMises,		// always postive
//							  double &out_MaxShear,		// either 0 ( no shear stress ) or positive 
//							  double &out_MaxBearing	// either 0 ( no bearing stress ) or positive
//							 ) throw (isis::application_exception);
void ComputeStressValues_test()
{
	//		S00	S01	S02
	//		S10	S11	S12
	//		S20	S21	S22

	double stressTensor[3][3];

	// From Abaqus
	stressTensor[0][0] = -0.2267;
	stressTensor[1][1] = .0133441;
	stressTensor[2][2] = .0311546;

	stressTensor[0][1] = -.0137862;
	stressTensor[0][2] = -.107133;
	stressTensor[1][2] = -.0170857;
	
	// Result From Abaqus
	// Principal  .0721027   .0123583  -.266663
	// Von Mises  .313196
	// Tresca = .338765
	// Pressure = .0607339

	// From Abaqus
	stressTensor[0][0] = 100;
	stressTensor[1][1] = 100;
	stressTensor[2][2] = 100;

	stressTensor[0][1] = 0;
	stressTensor[0][2] = 0;
	stressTensor[1][2] = 0;

	//stressTensor[0][0] = 100;
	//stressTensor[1][1] = 200;
	//stressTensor[2][2] = 0;

	//stressTensor[0][1] = 88;
	//stressTensor[0][2] = 0;
	//stressTensor[1][2] = 0;


	double VonMises;
	double MaxShear;
	double MaxBearing;

	isis_CADCommon::ComputeStressValues( stressTensor, VonMises, MaxShear, MaxBearing );


	std::cout << std::endl << "Stress Tensor";
	std::cout << std::endl << stressTensor[0][0] << "  " << stressTensor[0][1] << "  " << stressTensor[0][2];
	std::cout << std::endl << stressTensor[1][0] << "  " << stressTensor[1][1] << "  " << stressTensor[1][2];
	std::cout << std::endl << stressTensor[2][0] << "  " << stressTensor[2][1] << "  " << stressTensor[2][2];
	std::cout << std::endl;
	std::cout << std::endl << "VonMises:   " << VonMises;
	std::cout << std::endl << "MaxShear:   " << MaxShear;
	std::cout << std::endl << "MaxBearing: " << MaxBearing;

	std::cout << std::endl << "******************************************";
	std::cout << std::endl << "************** CalculiX ******************";
	std::cout << std::endl << "******************************************";


	double stressTensor_CalculiX[6];
	stressTensor_CalculiX[0] = stressTensor[0][0];
	stressTensor_CalculiX[1] = stressTensor[1][1];
	stressTensor_CalculiX[2] = stressTensor[2][2];
	stressTensor_CalculiX[3] = stressTensor[0][1];
	stressTensor_CalculiX[5] = stressTensor[0][2];
	stressTensor_CalculiX[4] = stressTensor[1][2];

	double stressPrincipal_CalculiX[12];
	double alpha_0[3];
	double alpha_1[3];
	double alpha_2[3];
#define  TEST1
	//calcPrinc( double *s, double *p, double *a0, double *a1, double *a2, int flag )
	isis_CADCommon::calcPrinc(  stressTensor_CalculiX,  stressPrincipal_CalculiX, alpha_0, alpha_1, alpha_2, 0 );
	std::cout << std::endl << "CalculiX Principal 0: " << stressPrincipal_CalculiX[0];
	std::cout << std::endl << "CalculiX Principal 1: " << stressPrincipal_CalculiX[1];
	std::cout << std::endl << "CalculiX Principal 2: " << stressPrincipal_CalculiX[2];
/*
	double stressTensor[3][3];

	stressTensor[0][0] = -2;
	stressTensor[1][1] = 10;
	stressTensor[2][2] = -5;

	stressTensor[0][1] = 7;
	stressTensor[0][2] = -3;
	stressTensor[1][2] = 5;

	double VonMises;
	double MaxShear;
	double MaxBearing;

	isis_CADCommon::ComputeStressValues( stressTensor, VonMises, MaxShear, MaxBearing );


	std::cout << std::endl << "Stress Tensor";
	std::cout << std::endl << stressTensor[0][0] << "  " << stressTensor[0][1] << "  " << stressTensor[0][2];
	std::cout << std::endl << stressTensor[1][0] << "  " << stressTensor[1][1] << "  " << stressTensor[1][2];
	std::cout << std::endl << stressTensor[2][0] << "  " << stressTensor[2][1] << "  " << stressTensor[2][2];
	std::cout << std::endl;
	std::cout << std::endl << "VonMises:   " << VonMises;
	std::cout << std::endl << "MaxShear:   " << MaxShear;
	std::cout << std::endl << "MaxBearing: " << MaxBearing;
*/
}
#endif

template<typename T> std::vector<T> make_vec(T *values, int N) {
    std::vector<T> v;
	for (int i = 0; i < N; i++)
		v.push_back(values[i]);
	return v;
}

void Tests::NastranDeck_test()
{
	try
		{
			NastranDeck deck;
			std::cout << std::endl << "Reading Deck: " << "..\\..\\Edited_Thermal_Mesh.nas";
			deck.ReadNastranDeck("src\\CADAssembler\\CADCommonTest\\Edited_Thermal_Mesh.nas");
			//deck.ReadNastranDeck("..\\Edited_Thermal_Mesh.nas");


			//std::cout << std::endl << "Display Deck: ";
			//std::cout << deck;


			NastranDeckHelper nastranDeckHelper(deck);

			bool	DefaultGridPointTemperature_set;
			double  DefaultGridPointTemperature;

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  nastranDeckHelper ***********************";

			///////////////////////////////
			// DefaultGridPointTemperature
			///////////////////////////////
			nastranDeckHelper.getDefaultGridPointTemperature( DefaultGridPointTemperature_set, DefaultGridPointTemperature );

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  DefaultGridPointTemperature ***********************";

			CPPUNIT_ASSERT(DefaultGridPointTemperature_set);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(293, DefaultGridPointTemperature, 0.0001);

			///////////////////////////////
			// GridPointTemperatures
			///////////////////////////////
			 std::map<int,double> gridPointToTemperature_map;

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  GridPointTemperatures ***********************";

			nastranDeckHelper.getSpecifiedGridPointTemperatures( gridPointToTemperature_map );


			CPPUNIT_ASSERT_EQUAL(9, (int)gridPointToTemperature_map.size());
			for each ( const std::pair<int,double> &i in gridPointToTemperature_map)
			{
				//std::cout << std::endl << "Grid Point: " << i.first << "  Temperature: " << i.second;
				CPPUNIT_ASSERT_DOUBLES_EQUAL(401.123, i.second, 0.0001);
			}

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  findElementsContainingSurface ***********************";

			std::vector<int> surfaceCornerGridIDs;

			surfaceCornerGridIDs.push_back(20);
			surfaceCornerGridIDs.push_back(14);
			surfaceCornerGridIDs.push_back(32);

			bool elementFound;
			std::set<int> elementIDs;

			nastranDeckHelper.findElementsContainingSurface( surfaceCornerGridIDs,elementFound, elementIDs );

			//std::cout <<std::endl << "Element Key: "; 
			//for each ( const int &j in surfaceCornerGridIDs) std::cout << " " << j;



			CPPUNIT_ASSERT(elementFound);

			// In the deck Edited_Thermal_Mesh.nas, there are two tetra with the element combination
			// CTETRA,20,1,20,14,32,13,93,94,
			//	,117,87,84,92
			// CTETRA,10,1,14,20,32,5,93,117,       // We want this one
			//	,94,50,51,52

			int elementID = 0;
			for each ( const int &i_target in elementIDs )
			{
				if ( i_target == 10 ) elementID = 10;
			}
	
			CPPUNIT_ASSERT_EQUAL(10, elementID);
			CPPUNIT_ASSERT_EQUAL(10, deck.getElementData().find(elementID)->second.EID);
			CPPUNIT_ASSERT_EQUAL(CTETRA, deck.getElementData().find(elementID)->second.Type);

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  getHeatFluxLoadsForBoundarySurfaces ***********************";

			std::vector<HeatFluxLoad> heatFluxLoads;
			nastranDeckHelper.getHeatFluxLoadsForBoundarySurfaces( heatFluxLoads ) ;
			CPPUNIT_ASSERT_EQUAL(2, (int)heatFluxLoads.size());
			//const std::map<int, FEAElement> &elementData_map = deck.getElementData();

			CPPUNIT_ASSERT_EQUAL(std::string("QBDY3"), heatFluxLoads[0].name);
			CPPUNIT_ASSERT_EQUAL(39, heatFluxLoads[0].elementIDThatContainsSurface);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(310, heatFluxLoads[0].Q0, 0.0001);
			CPPUNIT_ASSERT_EQUAL(59, heatFluxLoads[0].SID);
			int vals[] = {12,30,29,83,141,82};
			CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals, 6), heatFluxLoads[0].surfaceGridPointIDs);

			CPPUNIT_ASSERT_EQUAL(std::string("QBDY3"), heatFluxLoads[1].name);
			CPPUNIT_ASSERT_EQUAL(49, heatFluxLoads[1].elementIDThatContainsSurface);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(310, heatFluxLoads[1].Q0, 0.0001);
			CPPUNIT_ASSERT_EQUAL(59, heatFluxLoads[1].SID);
			int vals2[] = {29,12,11,82,73,77};
			CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals2, 6), heatFluxLoads[1].surfaceGridPointIDs);

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  getSurfaceConvectionConstraints ***********************";
			
			std::vector<SurfaceConvection> surfaceConvections;
			nastranDeckHelper.getSurfaceConvectionConstraints ( surfaceConvections);
			CPPUNIT_ASSERT_EQUAL(2, (int)surfaceConvections.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(222.34, surfaceConvections[0].convectionCoefficient, 0.01);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(243.99, surfaceConvections[0].ambientTemperature, 0.01);
			CPPUNIT_ASSERT_EQUAL(24, surfaceConvections[0].elementIDThatContainsSurface);
			CPPUNIT_ASSERT_EQUAL(100005, surfaceConvections[0].convID);
			CPPUNIT_ASSERT_EQUAL(100004, surfaceConvections[0].pconvID);
			CPPUNIT_ASSERT_EQUAL(100003, surfaceConvections[0].mat4ID);
			CPPUNIT_ASSERT_EQUAL(145, surfaceConvections[0].spointID);
			int vals3[] = {3,1,4,35,36,44};
			CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals3, 6), surfaceConvections[0].surfaceGridPointIDs);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(222.34, surfaceConvections[1].convectionCoefficient, 0.01);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(243.99, surfaceConvections[1].ambientTemperature, 0.01);
			CPPUNIT_ASSERT_EQUAL(43, surfaceConvections[1].elementIDThatContainsSurface);
			CPPUNIT_ASSERT_EQUAL(100006, surfaceConvections[1].convID);
			CPPUNIT_ASSERT_EQUAL(100004, surfaceConvections[1].pconvID);
			CPPUNIT_ASSERT_EQUAL(100003, surfaceConvections[1].mat4ID);
			CPPUNIT_ASSERT_EQUAL(145, surfaceConvections[1].spointID);
			int vals4[] = {2,1,3,34,35,41};
			CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals4, 6), surfaceConvections[1].surfaceGridPointIDs);

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  getSurfaceElementsContainingGridPoints ***********************";

			std::set<int>  targetGridPoints;
			// Element 1
			// CTETRA,1,1,17,22,33,23,102,124,
			// ,105,103,121,128
			targetGridPoints.insert(17);
			targetGridPoints.insert(22);
			targetGridPoints.insert(33);
			//targetGridPoints.insert(23);

			// Element 9 
			// CTETRA,9,1,9,15,21,10,63,96,
			// ,64,62,67,69

			targetGridPoints.insert(9);
			targetGridPoints.insert(15);
			targetGridPoints.insert(21);

			std::multimap< int, std::vector<int>> foundElements_map;

			nastranDeckHelper.getSurfaceElementsContainingGridPoints (	targetGridPoints, foundElements_map);
		
			CPPUNIT_ASSERT_EQUAL(3, (int)foundElements_map.size());
		
			for each ( const std::pair< int, std::vector<int>> &i in foundElements_map )
			{
				//std::cout << std::endl <<  "Element ID: " << i.first  << " Grid Points:" ;
				//for each ( const int &j in i.second ) std::cout << " " << j;
				if (i.first == 1)
				{
					int vals4[] = {17, 22, 33, 102, 124, 105};
					CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals4, 6), i.second);
				} else if (i.first == 7)
				{
					int vals4[] = {21, 22, 33, 118, 124, 120};
					CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals4, 6), i.second);
				} else if (i.first == 9)
				{
					int vals4[] = {9, 15, 21, 63, 96, 64};
					CPPUNIT_ASSERT_EQUAL(make_vec<int>(vals4, 6), i.second);
				} else {
					CPPUNIT_ASSERT(false);
				}

			}

			//std::cout << std::endl;
			//std::cout << std::endl << "*****************  getVolumetricHeatGenerations ***********************";

			std::vector<VolumetricHeatGeneration> volumetricHeatGenerations;

			nastranDeckHelper.getVolumetricHeatGenerations( volumetricHeatGenerations );

			CPPUNIT_ASSERT_EQUAL(1, (int)volumetricHeatGenerations.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(.003114, volumetricHeatGenerations[0].powerInputPerVolume, 0.0001);
			int i=1;
			for each ( const int j in volumetricHeatGenerations[0].elementIDs )
			{
				CPPUNIT_ASSERT_EQUAL(j, i);
				i++;
			}

		}

		//catch ( isis::application_exception &ex )
		catch ( isis::application_exception &)
		{
			CPPUNIT_ASSERT(false);

		}

}

/////////////////////////////////////////////////////////////////////////////////

void Tests::ComputeTransformationToAlignVectors_test()
{
	// Warning, This function needs work
	// See GraphicsFunctions ComputeRotationMatrixToAlignVectorsy.cpp

	std::vector<double>	in_Vector_1;
	std::vector<double>	in_Vector_2;

	in_Vector_1.push_back(1);
	in_Vector_1.push_back(1);
	in_Vector_1.push_back(3);

	in_Vector_2.push_back(-1);
	in_Vector_2.push_back(10);
	in_Vector_2.push_back(5);

	double	rotationMatrix[3][3] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

	//isis_CADCommon::ComputeRotationMatrixToAlignVectors( in_Vector_1, in_Vector_2,rotationMatrix);

	TransformationMatrix transformationMatrix;

	std::vector<double>	offset(3, 0.0);

	transformationMatrix.setTransformationMatrix(rotationMatrix, offset);

	isis_CADCommon::Point_3D  point_start(1,1,3);
	
	isis_CADCommon::Point_3D  point_transformed = transformationMatrix.getTransformedCoordinates( point_start);

	/*std::cout << std::endl << "Vector 1";
	for each ( double i in in_Vector_1 ) std::cout << std::endl << i;

	std::cout << std::endl << "Vector 2";
	for each ( double i in in_Vector_2 ) std::cout << std::endl << i;

	std::cout << std::endl;
	std::cout << std::endl << "Point Start:       " << point_start;
	std::cout << std::endl << "Point Transformed: " << point_transformed;*/

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, point_transformed.x, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, point_transformed.y, 0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0, point_transformed.z, 0.0001);
}

/////////////////////////////////////////////////////////////////////////////////
	void Tests::Determinant_4_x_4_test ( )
	{
		// Example from http://nebula.deanza.edu/~bloom/math43/Determinant4x4Matrix.pdf
		double matrix[4][4];

		matrix[0][0] = 3.0;
		matrix[0][1] = 0.0;
		matrix[0][2] = 2.0;
		matrix[0][3] = -1.0;

		matrix[1][0] = 1.0;
		matrix[1][1] = 2.0;
		matrix[1][2] = 0.0;
		matrix[1][3] = -2.0;

		matrix[2][0] = 4.0;
		matrix[2][1] = 0.0;
		matrix[2][2] = 6.0;
		matrix[2][3] = -3.0;

		matrix[3][0] = 5.0;
		matrix[3][1] = 0.0;
		matrix[3][2] = 2.0;
		matrix[3][3] = 0.0;

		double determinant = Determinant_4_x_4(matrix);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(20, determinant, 0.001);
		//std::cout << std::endl << "Determinant, Answer should be 20.";
		//std::cout << std::endl << "Determinant Answer: " << determinant;

		matrix[0][0] = 1.0;
		matrix[0][1] = 2.0;
		matrix[0][2] = 3.0;
		matrix[0][3] = 4.0;

		matrix[1][0] = -1.0;
		matrix[1][1] = -2.0;
		matrix[1][2] = -3.0;
		matrix[1][3] = -4.0;

		matrix[2][0] = 5.0;
		matrix[2][1] = 6.0;
		matrix[2][2] = 7.0;
		matrix[2][3] = 8.0;

		matrix[3][0] = -8.0;
		matrix[3][1] = -7.0;
		matrix[3][2] = -6.0;
		matrix[3][3] = -5.0;

		determinant = Determinant_4_x_4(matrix);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(0, determinant, 0.001);
		//std::cout << std::endl << "Determinant, Answer should be 0.";
		//std::cout << std::endl << "Determinant Answer: " << determinant;

		matrix[0][0] = 5.0;
		matrix[0][1] = 23.0;
		matrix[0][2] = 14.0;
		matrix[0][3] = 17.0;

		matrix[1][0] = 4.0;
		matrix[1][1] = 5.0;
		matrix[1][2] = 6.0;
		matrix[1][3] = 7.0;

		matrix[2][0] = 8.0;
		matrix[2][1] = 9.0;
		matrix[2][2] = 10.0;
		matrix[2][3] = 11.0;

		matrix[3][0] = 24.0;
		matrix[3][1] = 18.0;
		matrix[3][2] = -2.0;
		matrix[3][3] = 4.0;

		determinant = Determinant_4_x_4(matrix);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(2136, determinant, 0.001);
		//std::cout << std::endl << "Determinant, Answer should be 2136.";
		//std::cout << std::endl << "Determinant Answer: " << determinant;


	}
#if 0
	void PointWithinOrOnTetrahedron_test()
	{

		/* All positive coordinates
		isis_CADCommon::Point_3D  point_1(  0,  0, 0 );
		isis_CADCommon::Point_3D  point_2(  0,  1, 0 );
		isis_CADCommon::Point_3D  point_3(  1,  1, 0 );
		isis_CADCommon::Point_3D  point_4( .5, .5, 1.0 );

		std::vector<isis_CADCommon::Point_3D> Tetrahedron_points;

		Tetrahedron_points.push_back(point_1);
		Tetrahedron_points.push_back(point_2);
		Tetrahedron_points.push_back(point_3);
		Tetrahedron_points.push_back(point_4);

		//Point_3D point(.5,.5, 1.0005);  // Not Within/On
		//Point_3D point(.5,.5, 1.000);  // Within/On 

		//Point_3D point(.25,.25, .500001);  // Not Within/On 
		//Point_3D point(.25,.25, .5);  // Within/On 

		//Point_3D point(.75,.75, .500001);  // Not Within/On 
		//Point_3D point(.75,.75, .5);  // Within/On 

		//Point_3D point(0.0, 1.0, 0);  // Within/On 
		//Point_3D point(0.0, 1.000006, 0);  // Not Within/On 

		//Point_3D point(1.0, 1.0, 0);  // Within/On 
		//Point_3D point(1.0, 1.000006, 0);  // Not Within/On 

		Point_3D point(.2, .2, .2);  //  Within/On 
		bool pointWithin/On =  PointWithin/OnTetrahedron ( Tetrahedron_points, point );

		*/

		/* All negative coordinates
		isis_CADCommon::Point_3D  point_1(  0,  0, 0 );
		isis_CADCommon::Point_3D  point_2(  0,  -1, 0 );
		isis_CADCommon::Point_3D  point_3(  -1,  -1, 0 );
		isis_CADCommon::Point_3D  point_4( -.5, -.5, -1.0 );

		std::vector<isis_CADCommon::Point_3D> Tetrahedron_points;

		Tetrahedron_points.push_back(point_1);
		Tetrahedron_points.push_back(point_2);
		Tetrahedron_points.push_back(point_3);
		Tetrahedron_points.push_back(point_4);

		//Point_3D point(-.5,-.5, -1.0005);  // Not Within/On
		//Point_3D point(-.5,-.5, -1.000);  // Within/On 

		//Point_3D point(-.25,-.25, -.500001);  // Not Within/On 
		//Point_3D point(-.25,-.25, -.5);  // Within/On 

		//Point_3D point(-.75,-.75, -.500001);  // Not Within/On 
		//Point_3D point(-.75,-.75, -.5);  // Within/On 

		//Point_3D point(0.0, -1.0, 0);  // Within/On 
		//Point_3D point(0.0, -1.000006, 0);  // Not Within/On 

		//Point_3D point(-1.0, -1.0, 0);  // Within/On 
		//Point_3D point(-1.0, -1.000006, 0);  // Not Within/On 

		Point_3D point(-.2, -.2, -.2);  //  Within/On 

		bool pointWithin/On =  PointWithin/OnTetrahedron ( Tetrahedron_points, point );
		*/


		//* Mix negative and positive
		isis_CADCommon::Point_3D  point_1( -.5,   0, 0 );
		isis_CADCommon::Point_3D  point_2(   .5,  .5, 0 );
		isis_CADCommon::Point_3D  point_3(  .5,  0, 0 );

		isis_CADCommon::Point_3D  point_4(   0,   .5, 1.0 );

		std::vector<isis_CADCommon::Point_3D> Tetrahedron_points;

		Tetrahedron_points.push_back(point_1);
		Tetrahedron_points.push_back(point_2);
		Tetrahedron_points.push_back(point_3);
		Tetrahedron_points.push_back(point_4);

		//Point_3D point( 0, 0.5, -1.0005);  // Not Within/On
		//Point_3D point( 0, 0.5, 1.000);  // Within/On 

		//Point_3D point(.25, .25, .500001);  // Not Within/On 
		//Point_3D point(.25, .25, .5);  // Within/On 

		//Point_3D point( .25, .25, .500001);  // Not Within/On 
		//Point_3D point( -.25, .25, .5);  // Within/On 

		//Point_3D point( .5, .5, 0);  // Within/On 
		//Point_3D point(.5, .5000001, 0);  // Not Within/On 

		//Point_3D point(-.5, 0 , 0);  // Within/On 
		//Point_3D point( -.50000001, 0 , 0);  // Not Within/On 

		Point_3D point(.2, .2, .2);  //  Within/On 

		bool pointWithin =  PointWithinOrOnTetrahedron ( Tetrahedron_points, point );
		//*/

		std::cout << std::endl << "pointWithin: " << pointWithin;
	}

	void CentroidOfTetrahedron_test()
	{
		//isis_CADCommon::Point_3D  point_1(  0,  0, 0 );
		//isis_CADCommon::Point_3D  point_2(  1,  0, 0 );
		//isis_CADCommon::Point_3D  point_3(  0,  1, 0 );
		//isis_CADCommon::Point_3D  point_4( .5, .5, 1.0 );

		isis_CADCommon::Point_3D  point_1(  1,  0, 0 );
		isis_CADCommon::Point_3D  point_2(  0,  2, 0 );
		isis_CADCommon::Point_3D  point_3(  0,  4, 1 );
		isis_CADCommon::Point_3D  point_4( 0, 0, 3.0 );
		// Answer Centroid: 0.25  1.5  1


		std::vector<isis_CADCommon::Point_3D> Tetrahedron_points;

		Tetrahedron_points.push_back(point_1);
		Tetrahedron_points.push_back(point_2);
		Tetrahedron_points.push_back(point_3);
		Tetrahedron_points.push_back(point_4);		

		Point_3D centroid;

		CentroidOfTetrahedron( Tetrahedron_points, centroid);

		std::cout << std::endl << "Centroid: " << centroid.x << "  " << centroid.y << "  " << centroid.z; 
	}
#endif

	void Tests::VolumeOfTetrahedron_test()
	{
		// https://www.easycalculation.com/analytical/parellelepiped-tetrahedron-volume.php

		std::vector<isis_CADCommon::Point_3D> Tetrahedron_points;

		isis_CADCommon::Point_3D  point_1(  0,  0, 0 );
		isis_CADCommon::Point_3D  point_2(  2,  0, 0 );
		isis_CADCommon::Point_3D  point_3(  1,  2, 0 );
		isis_CADCommon::Point_3D  point_4( 1,  1.0, 2.0 );


		Tetrahedron_points.clear();
		Tetrahedron_points.push_back(point_1);
		Tetrahedron_points.push_back(point_2);
		Tetrahedron_points.push_back(point_3);
		Tetrahedron_points.push_back(point_4);	

		double volume =  VolumeOfTetrahedron( Tetrahedron_points);
		//std::cout << std::endl << "Answer should be: 1.33"; 
		//std::cout << std::endl << "Volume: " << volume; 
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.33333, volume, 0.001);

		isis_CADCommon::Point_3D  point_1_2(  0,  0, 0 );
		isis_CADCommon::Point_3D  point_2_2(  1,  0, 0 );
		isis_CADCommon::Point_3D  point_3_2(  0,  1, 0 );
		isis_CADCommon::Point_3D  point_4_2( .5, .5, 1.0 );

		Tetrahedron_points.clear();
		Tetrahedron_points.push_back(point_1_2);
		Tetrahedron_points.push_back(point_2_2);
		Tetrahedron_points.push_back(point_3_2);
		Tetrahedron_points.push_back(point_4_2);	

		volume =  VolumeOfTetrahedron( Tetrahedron_points);
		//std::cout << std::endl << "Answer should be: 1.1666"; 
		//std::cout << std::endl << "Volume: " << volume;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.16666666, volume, 0.001);


		isis_CADCommon::Point_3D  point_1_3(  -5,  0, 0 );
		isis_CADCommon::Point_3D  point_2_3(  1,  0, 0 );
		isis_CADCommon::Point_3D  point_3_3(  2,  -1, 0 );
		isis_CADCommon::Point_3D  point_4_3( .5, .5, 3.0 );

		Tetrahedron_points.clear();
		Tetrahedron_points.push_back(point_1_3);
		Tetrahedron_points.push_back(point_2_3);
		Tetrahedron_points.push_back(point_3_3);
		Tetrahedron_points.push_back(point_4_3);	

		volume =  VolumeOfTetrahedron( Tetrahedron_points);
		//std::cout << std::endl << "Answer should be: 3.0"; 
		//std::cout << std::endl << "Volume: " << volume; 
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, volume, 0.001);
	}

