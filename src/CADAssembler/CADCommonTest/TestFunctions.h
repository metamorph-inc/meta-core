#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include "isis_application_exception.h"
#include "cc_GraphicsFunctions.h"
#include "cppunit\Test.h"
#include "cppunit\TestSuite.h"
#include "cppunit\extensions\HelperMacros.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace isis_CADCommon;

class Tests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Tests);
	CPPUNIT_TEST(VolumeOfTetrahedron_test);
	CPPUNIT_TEST(DotProduct_2D_test);
	CPPUNIT_TEST(DotProduct_3D_test);
	CPPUNIT_TEST(CrossProduct_3D_test);
	CPPUNIT_TEST(Magnitude_3D_test);
	CPPUNIT_TEST(UnitVector_3D_test);
	CPPUNIT_TEST(PointOnLine_2D_test);
	//CPPUNIT_TEST(Point2D_Within_Polygon_2D_test);
	CPPUNIT_TEST(AngleBetweenVectors_test);
	CPPUNIT_TEST(TransformationMatrix_test);
	CPPUNIT_TEST(ComputeRotationAngles_test);
	CPPUNIT_TEST(DuplicatePointInList_test);
	CPPUNIT_TEST(PointsWithinPolygon_test);
	CPPUNIT_TEST(GetGridPointsWithinCircle_test);
	CPPUNIT_TEST(GetGridPointsBetweenOrOnConcentricCircles_test);
	CPPUNIT_TEST(ShortestDistanceBetweenPointAndLine_test);
	CPPUNIT_TEST(OnOrWithinCylinder_test);
	CPPUNIT_TEST(OnCylinderSurface_test);
	CPPUNIT_TEST(OnSphereSurface_test);
	CPPUNIT_TEST(WithinSphere_test);
	CPPUNIT_TEST(WithinExtrusion_test);
	//CPPUNIT_TEST(ComputeStressValues_test);
	CPPUNIT_TEST(ComputeTransformationToAlignVectors_test);
	CPPUNIT_TEST(Determinant_4_x_4_test);
	//CPPUNIT_TEST(NastranDeck_test);
	//CPPUNIT_TEST(PointWithinOrOnTetrahedron_test);
	//CPPUNIT_TEST(CentroidOfTetrahedron_test);
	CPPUNIT_TEST_SUITE_END();
private:
	void VolumeOfTetrahedron_test();
	void DotProduct_2D_test();
	void DotProduct_3D_test();
	void CrossProduct_3D_test();
	void Magnitude_3D_test();
	void UnitVector_3D_test();
	void PointOnLine_2D_test();
	//void Point2D_Within_Polygon_2D_test();
	void AngleBetweenVectors_test();
	
	void TransformationMatrix_test();
	void ComputeRotationAngles_test();

	void DuplicatePointInList_test();
	void PointsWithinPolygon_test();

	void GetGridPointsWithinCircle_test();
	void GetGridPointsBetweenOrOnConcentricCircles_test();
	void ShortestDistanceBetweenPointAndLine_test();
	void OnOrWithinCylinder_test();
	void OnCylinderSurface_test();
	void WithinSphere_test();
	void OnSphereSurface_test();
	void WithinExtrusion_test();

	void NastranDeck_test();
	//void ComputeStressValues_test();
	void ComputeTransformationToAlignVectors_test();

	void Determinant_4_x_4_test ();

	//void PointWithinOrOnTetrahedron_test();

	//void CentroidOfTetrahedron_test();
};

#endif