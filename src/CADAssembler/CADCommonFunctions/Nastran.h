#ifndef NASTRAN_H
#define NASTRAN_H

#include <list>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "isis_application_exception.h"
#include "GraphicsFunctions.h"
#include <string>

namespace isis_CADCommon
{

	struct CoordSystem
	{		
	
		//enum CoordType {
		//	CORD2R,
		//	CORD2C,
		//	CORD2S
		//}; 
		
		std::string system;
		int CID;
		int RID;
		Point_3D A;
		Point_3D B;
		Point_3D C;
		std::string coordComment;
		friend std::ostream& operator<<(std::ostream& stream, const CoordSystem &myCoord);
	};

	struct Material
	{
		int MID;
		std::string name;
		std::string Young;
		std::string Shear;
		std::string Poisson;
		std::string Density;
		std::string Thermal;
		std::string RefTemp;
		std::string Damping;
	};

	enum NastranCardType
	{
		DISPLACEMENT,
		SPCFORCES,
		FORCE,
		OLOAD,
		STRESS,
		STRAIN,
		ESE,
	};
	
	typedef std::map<std::string, std::string> SubcaseStatements;			// keyword, value
	struct CaseControl
	{
		// key, pair<option, value>
		std::map<std::string, std::pair<std::string, std::string>> commands;

		// subcaseID, map<keyword, value>
		//std::map<std::string, SubcaseStatements> subcases;
		std::map<std::string, std::vector<std::pair<std::string, std::string>>> subcases;
	};
	
	struct PSolid
	{
		int PID;
		int MID;
		int Cordm;
		std::string In;
		std::string Stress;
		std::string Isop;
		std::string Fctn;
		friend std::ostream& operator<<(std::ostream& stream, const PSolid &myPSolid);
	};

	enum ElementType 
	{
		CHEXA,
		CPENTA,
		CTETRA
	};

	struct SolidElement
	{
		ElementType Type;
		int EID;
		int PID;
		std::vector<int> GID;

		friend std::ostream& operator<<(std::ostream& stream, SolidElement &myElement);
	};

	struct SPC
	{
		int SID;
		std::string G1;
		std::string C1;
		std::string D1;
		friend std::ostream& operator<<(std::ostream& stream, SPC &myConstraint);
	};

	struct Force
	{
		int SID;
		int GID;
		int CID;
		std::string F;
		std::string N1;
		std::string N2;
		std::string N3;
		friend std::ostream& operator<<(std::ostream& stream, Force &myForce);
	};

	struct Load
	{
		int SID;
		std::string ScaleFactor;
		std::vector<std::map<std::string, std::string>> ScaleLoad_Pairs;		// map<Scale, LoadID>
	};

	//From Ziyi
	struct ElementDistortionMetrics
	{
		double minimumAspectRation; // Longest_Line/Shortest_Line
		double minimumAngle; // Minimum angle of a tetrahedron
		double maximumAngle; // Maximum angle of a tetrahedron
		double tetrahedralQualityMeasure;
	};

	struct Triangle
	{
		double maxAngle; // Maximum angle of a triangle
		double minAngle; // Minimum angle of a triangle
	};

	//DEPRECATED
	//class CommonNastranDS
	//{
	//	//This class was originally a struct. We've turned it into a class with the
	//	// addition of the FindGridPointsFromPSolid function. In order to preserve
	//	// all the functionality of the struct, we've made all the member variables
	//	// public
	//public:
	//	CommonNastranDS(std::string fileName):
	//		Name(fileName),
	//		TetraCount(0),
	//		PentaCount(0),
	//		HexaCount(0),
	//		ElementTypeCount(0){}
	//				
	//	std::string Name;
	//	//***** Executive Control *****
	//	std::string solverType;
	//	int time;
	//	//*****************************
	//	
	//	//******** Case Control *******
	//	CaseControl caseControlData;
	//	//*****************************
	//	//********* Bulk Data *********
	//	std::map<int, Material>						materialData;				// MAT1 materials, others not supported right now
	//	std::map<int, GridPoint>					gridPointData;				// grid points
	//	std::map<int, PSolid>						psolidData;					// PSolids: <PID, psolid>
	//	std::multimap<int, SolidElement>			elementData;				// elements: <PID, element>
	//	std::map<int, CoordSystem>					coordSystems;
	//	std::multimap<int, SPC>						spcData;					// SPC
	//	std::multimap<int, Force>					forceLoadData;				// Force
	//	// Book-keeping Stuff
	//	int globalCord_ID;
	//	int gridCord_ID;
	//	int gridDisplacementCord_ID;
	//	std::multimap<int, int>						displaceCord_Elements;		// <dispCord_ID, Grid_ID>
	//	std::set<int>								displaceCord_Elements_KEYS;
	//	int TetraCount;
	//	int PentaCount;
	//	int HexaCount;
	//	int ElementTypeCount;
	//	//*******************

	//	// Description: This class takes a vector of PSolid ID integers and outputs a 
	//	//  corresponding vector of all SolidElement objects which are contained within
	//	//  the PSolids. This function is invoked in the FindGridPointsFromPSolids function
	//	//
	//	// Pre-conditions:  in_PSolidIDs is a valid std::vector<int>
	//	//                  out_elements is empty (although we ensure this later)
	//	// Post-conditions: An exception is thrown if any PSolid ID is invalid
	//	//                  Otherwise, the corresponding vector of SolidElement objects 
	//	//                   is returned by parameter
	//	void FindElementsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<SolidElement> &out_elements);

	//	// Description: Given a vector of SolidElements, we return a vector of gridPoints that belong to those
	//	//  SolidElement objects.
	//	//
	//	// Pre-conditions:  in_Elements is a valid vector of SolidElement objects 
	//	// Post-conditions: An exception is raised if any of the elements have an invalid ID
	//	//                  If no exception is raised, we return a vector of gridPoints that compose the 
	//	//                   SolidElement objects
	//	//
	//	void FindGridPointsFromElements(std::vector<SolidElement> &in_ElementIDs, std::vector<int> &out_gridPoints);

	//	// Description: Given a vector of PSolid IDs, we return a vector of grid point IDs that
	//	//  correspond to the PSolid parts
	//	//
	//	// Pre-conditions: std::vector<int> is a valid vector of ints	
	//	// Post-conditions: An exception is raised if any PSolidID is invalid
	//	//                  If no exception is rased, we return a vector of grid points that 
	//	//                   corresponds to the PSolids		
	//	void FindGridPointsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<int> &out_gridPoints);
	//	

	//	void SeparateGridPoints(std::vector<int> &in_gridPoints);
	//	void SeparateGridPoint(int &in_gridPoint);

	//};
	//////////////////////////////////////////
	// class NastranDeck
	//////////////////////////////////////////

	class NastranDeck
	{
		private:
			std::string name; // for the file name

			std::list<std::string> headerComments;	
			
			// For executiveControl
			std::string solverType;
			int time;

			std::list<std::string> executiveControl;
			std::string	endExecutiveControl;

			CaseControl caseControlData;
			std::list<std::string> caseControl;

			std::string	beginBulkData;
			std::list<std::string> bulkString;
			std::string endBulkData;

			//Data copied from the CommonNastranDS object
			std::map<int, Material>	materialData; // MAT1 materials, others not supported right now
			std::map<int, GridPoint> gridPointData;	// grid points
			std::map<int, PSolid> psolidData; // PSolids: <PID, psolid>
			std::map<int, SolidElement> elementData; // elements: <PID, element> - used to be multimap, but we
			// need to index into it. Hopefully this doesn't hurt us later

			std::map<int, CoordSystem> coordSystems;

			std::multimap<int, SPC>	spcData; // SPC
			std::multimap<int, Force> forceLoadData; // Force

			// Book-keeping Stuff
			int	globalCord_ID;
			int	gridCord_ID;
			int gridDisplacementCord_ID;

			std::multimap<int, int> displaceCord_Elements; // <dispCord_ID, Grid_ID>
			std::set<int> displaceCord_Elements_KEYS;


			int tetraCount;
			int pentaCount;
			int hexaCount;
			int elementTypeCount;

			int	maxCoordinateID;


		public:
			NastranDeck();
			
			//////////////////////////////
			// Accessors
			//////////////////////////////
			std::string& getName();
			std::list<std::string>& getHeaderComments();
			int getTime();
			std::string& getSolverType();
			std::list<std::string>& getExecutiveControl();
			std::string& getEndExecutiveControl();
			CaseControl& getCaseControlData();
			std::list<std::string>& getCaseControl();
			std::string& getBeginBulkData();
			std::list<std::string>& getBulkString();
			std::string& getEndBulkData();
			std::map<int, Material>& getMaterialData(); 
			std::map<int, GridPoint>& getGridPointData();	
			std::map<int, PSolid>& getPsolidData(); 
			std::map<int, SolidElement>& getElementData(); 
			std::map<int, CoordSystem>& getCoordSystems();
			std::multimap<int, SPC>& getSpcData();
			std::multimap<int, Force>& getForceLoadData(); 
			int getGlobalCord_ID();
			int getGridCord_ID();
			int getGridDisplacementCord_ID();
			std::multimap<int, int>& getDisplaceCord_Elements();
			std::set<int>& getDisplaceCord_Elements_KEYS();
			int getTetraCount();
			int getPentaCount();
			int getHexaCount();
			int getElementTypeCount();
			int getMaxCoordinateID();

			//////////////////////////////
			// Initialize/Load Deck
			//////////////////////////////
			void ReadNastranDeck( const std::string &in_InputFileName ) throw (isis::application_exception);

			
			
			// void GetMetaHeaderComments( 
			// void GetSolutionType   // Problem should use Nastran types
			// void GetSubCase ....
			//void GetMaterialProperties ( std::map<int, Material> &out_Material_map ) throw (isis::application_exception);
			// void GetCoordinateSystems
			// void GetSolidToMaterialMap
			// void GetTetras10Pt (std::map<int, ????>  &out_Tetras10Pt_map);
			void ParseExecutiveControl() throw (isis::application_exception);
			void ParseCaseControl(CaseControl &out_classCtrl) throw (isis::application_exception);

			

			//////////////////////////////
			// Write/Stream Functions
			//////////////////////////////
			void WriteNastranDeck( const std::string &in_OutputFileName ) throw (isis::application_exception);
			friend std::ostream& operator<<(std::ostream &output, const NastranDeck &in_NastranDeck );

			//////////////////////////////
			// Edit Functions
			//////////////////////////////
			void AppendHeaderComment( const std::string &in_Comment );

			// Note this adds a Subcase and Load entires
			// SUBCASE = 0
			//		SPC = 259
			//		LOAD = 559
			// LOAD,559,1.,1.,59
			// LOAD SID S S1 L1 S2 L2 S3 L3
			//		SID Load set identification number. (Integer > 0)
			//		S Overall scale factor. (Real)
			//		Si Scale factor on Li. (Real)
			//		Li Load set identification numbers defined on entry types listed above. (Integer > 0)
			void AddSubCaseAndLoadStatement( int in_SubCaseID, int in_ConstraintSetID, int in_LoadStatementID, int in_LoadSetID );

			// Constraint		SPC,259,6825,1,0.
			// SPC SID G1 C1 D1 G2 C2 D2
			//		SID Identification number of the single-point constraint set.
			//		Gi Grid or scalar point identification number.
			//		Ci Component number
			//		Di Value of enforced displacement for all degrees of freedom designated by Gi and Ci.
			void AddConstraintToDeck( int in_ConstraintID, int in_GridID, 
									  bool in_xDisp_Defined,
									  bool in_yDisp_Defined,
									  bool in_zDisp_Defined,
									  double in_xDisp, 
									  double in_yDisp, 
									  double in_zDisp ); 

			// For Cylindrical Coordinate System
			// Degree of Freedon	Direction
			// -----------------	----------
			//		1				r - radial
			//	`	2				theta - rotational
			//		3				z - axis
			//
			// For Sperical Coordinate System
			// Degree of Freedon	Direction
			// -----------------	----------
			//		1				r - radial
			//	`	2				theta - rotational
			//		3				z - axis
			void AddConstraintToDeck( int in_ConstraintID, int in_GridID, int in_DegreeOfFreedomID, double in_ConstraintValue ); 

			// FORCE SID G CID F N1 N2 N3
			// FORCE,59,1706,1,1.0,-0.0583861,0.0,0.0
			//		SID Load set identification number.
			//		G Grid point identification number
			//		CID Coordinate system identification number.
			//		F Scale factor.	
			//		Ni Components of a vector measured in coordinate system defined by CID.
			// Note in_LoadID must be the same in_LoadID in the function call AddSubCaseAndLoadStatement(...)
			void AddForceToDeck( int in_LoadID, int in_GridID, int in_CoordinateSystemID, double in_ForceScaleFactor, 
								 double in_Vector_x, double in_in_Vector_y, double in_in_Vector_z ); 

			// Acceleration		GRAV,459,1,333.,0.,-1.,0.
			// GRAV SID CID G N1 N2 N3
			//		SlD Set identification number.
			//		CID Coordinate system identification number.
			//		G Acceleration vector scale factor.
			//		Ni Acceleration vector components measured in coordinate system CID.
			// GRAV,459,1,333.,0.,-1.,0.
			void AddAccelerationToDeck( int in_LoadID, int in_CoordinateSystemID, double in_AccelerationScaleFactor, 
										double in_Vector_x, double in_Vector_y, double in_Vector_z ); 

			// Pressure			PLOAD4,59,1875,14904.,14904.,14904.,,355,1221
			// PLOAD4 SID EID P1 P2 P3 P4 G1 G3 or G4 
			//		  CID N1 N2 N3 SORL LDIR
			//		SID Load set identification number
			//		P Pressure
			//		Gi Grid point identification numbers
			// PLOAD4,59,5877,14904.,14904.,14904.,,418,1416
			// ??? Presure loads require retriving elements,  should do this later 
			///void AddPressureToDeck( ...); 


			// MAT1 MID E G NU RHO A TREF GE ST SC SS MCSID
			// MID Material identification number
			// E Young’s modulus
			// G Shear modulus
			// NU Poisson’s ratio
			// RHO Mass density
			// A Thermal expansion coefficient
			// TREF Reference temperature
			// ST, SC, SS Stress limits for tension, compression, and shear
			// MCSID Material coordinate system identification number
			void ReplaceMaterialTokens_ReturnMaterialToComponentID( 
					const std::string								&in_AssemblyComponentID,
					std::map<std::string, std::string>              &in_MaterialKey_ComponentID_map,
					std::map<std::string, double>					&in_ComponentID_PoissonsRatio_map,
					std::map<std::string, std::string>              &out_MaterialID_to_CompnentID_map );

			enum e_CoordinateSystemType
			{
				// RECTANGULAR_COORDINATE_SYSTEM,  Don't allow this 
				CYLINDRICAL_COORDINATE_SYSTEM,
				SPHERICAL_COORDINATE_SYSTEM
			};
			// CORD2C,4,2,2239.38,1094.41,870.,2239.38,1093.41,870.,
			// ,2240.38,1094.41,870.
			// CORD2C CID RID A1 A2 A3 B1 B2 B3
			// C1 C2 C3
			// CID Coordinate system identification number.
			// RID Identification number of a coordinate system that is defined independently from
			// this coordinate system.
			// Ai, Bi, Ci Coordinates of three points in coordinate system defined in field 3.
			//
			// CORD2S CID RID A1 A2 A3 B1 B2 B3
			// C1 C2 C3
			// CID Coordinate system identification number.
			// RID Identification number of a coordinate system that is defined independently from this
			// coordinate system
			// Ai, Bi, Ci Coordinates of three points in coordinate system defined in field 3.
			void AddCylindricalOrSphericalCoordinateSystem( 
									const Point_3D &in_PointOrgin,
									const Point_3D &in_PointZAxis,
									const Point_3D &in_PointClockingPlane,
									int			in_ReferenceCoordinateSystemID,
									e_CoordinateSystemType	in_CoordinateSystemType,
									int			&out_AddedCoordinateID );

			// GRID ID CP X1 X2 X3 CD PS SEID
			// lD Grid point identification number.
			// CP Identification number of coordinate system in which the location of the grid point is
			// defined.
			// X1, X2, X3 Location of the grid point in coordinate system CP.
			// CD Identification number of coordinate system in which the displacements, degrees of
			// freedom, constraints, and solution vectors are defined at the grid point.
			// PS Permanent single-point constraints associated with the grid point.
			// SEID Superelement identification number.
			void ModifyGridPointsDisplacementCoordinateSystemID( const std::set<int> &in_GridPointIDs,
																 int	in_CoordinateSystemID );

			// Description: Given a list of strings, parses the list and populates
			//  the data members of the class. 
			//
			// Pre-conditions: None.
			// Post-conditions: The bulk data has been parsed, data elements populated. 
			//  Note that the parsing of the bulk data is not guaranteed to maintain 
			//  the order in which the data is structured in the input file. This 
			//  is important when you want to write the NastranDeck to a file.
			//  This function was adapted from the former ReadBulkData(CommonNastranDS&)
			//  function.
			void ParseBulkData(const std::list<std::string> &in_bulkData);

			//void CreateCommonNastranDS(CommonNastranDS&);		// main function to populate CommonNastranDS

			// Description: This class takes a vector of PSolid ID integers and outputs a 
			//  corresponding vector of all SolidElement objects which are contained within
			//  the PSolids. This function is invoked in the FindGridPointsFromPSolids function
			//
			// Pre-conditions:  None.
			// Post-conditions: An exception is thrown if any PSolid ID is invalid
			//  Otherwise, the corresponding vector of SolidElement objects is
			//   returned by parameter
			void FindElementsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<int> &out_elementIDs);

			// Description: Given a vector of SolidElements, we return a vector of gridPoints
			//  that belong to those SolidElement objects.
			//
			// Pre-conditions:  None.
			// Post-conditions: An exception is raised if any of the elements have an invalid ID
			//  If no exception is raised, we return a vector of gridPoints that compose the 
			//   SolidElement objects
			void FindGridPointsFromElements(std::vector<int> &in_ElementIDs, std::vector<int> &out_gridPoints);

			// Description: Given a vector of PSolid IDs, we return a vector of grid point IDs that
			//  correspond to the PSolid parts which contain them.
			//
			// Pre-conditions: None.
			// Post-conditions: An exception is raised if any PSolidID is invalid
			//  If no exception is rased, we return a vector of grid points that 
			//   corresponds to the PSolids		
			void FindGridPointsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<int> &out_gridPoints);
			
			// Description: Given a grid point ID, we return a vector of PSolids
			//  that contain this gridpoint.
			//
			// Pre-conditions: None.
			// Post-conditions: If the grid point ID is invalid, an exception
			//  is raised. Otherwise, out_PSolidIDs is populated with the IDs
			//  of all the PSolids that contain this gridpoint. 
			void FindPSolidsFromGridPoint(int in_GridPointID, std::vector<int> &out_PSolidIDs);

			// Description: Given a grid point ID, we return a vector of element IDs 
			//  corresponding to elements containing this grid point.
			//
			// Pre-conditions: None.
			// Post-conditions: If the grid point ID is invalid, an exception
			//  is raised. Otherwise, out_ElementIDs is populated with the IDs
			//  of all the elements that contain this gridpoint. 
			void FindElementsFromGridPoint(int in_GridPointID, std::vector<int> &out_ElementIDs);
			
			// Description: Given a vector of element IDs, returns a vector of PSolid IDs
			//  corresponding to the parts that contain these elements
			//
			// Pre-conditions: None.
			// Post-conditions: If any of the element IDs are invalid, an exception
			//  is raised. Otherwise, out_PSolidIDs is populated with the IDs of 
			void FindPSolidsFromElements(std::vector<int> &in_ElementIDs, std::vector<int> &out_PSolidIDs);

			// Description: Given a vector of GridPoint IDs, we call SeparateGridPoint()
			//  on it.
			//
			// Pre-conditions: None. 
			// Post-conditions: We have called the SeparateGridPoint function on each
			//  ID in the vector. If any grid point ID is invalid, an exception is 
			//  thrown. 
			void SeparateGridPoints(std::vector<int> &in_gridPoints);

			// Description: This function accepts a gridPoint ID as a parameter.
			//  It finds every SolidElement with that GridPoint, and
			//  marks it. Then, we find out if any of the marked SolidElement 
			//  objects belong to different parts. If not, we do nothing. 
			//  If so, then we keep all the entries relating to one of the 
			//  PSolids the same. For each of the others PSolids, we create a new
			//  grid point with the same values as in_gridPoint (but with 
			//  a different GID, of course). Then for each SolidElement of
			//  that PSolid, we create 
			//
			//	Pre-conditions: in_gridPoint is a valid grid point ID.
			//  Post-conditions: If in_gridPoint is not a valid ID, an exception
			//   is thrown. Otherwise, for each part (PSolid) in_gridPoint 
			//   belongs to, a new grid point is created, and each SolidElement
			//   in the part which
			void SeparateGridPoint(int in_gridPoint);

			// Description: Given a vector of grid point IDs, we call JoinGridPoint()
			//  on it. 
			//
			// Pre-conditions: None. 
			// Post-conditions: The SeparateGridPoint function has been called on each
			//  ID in the vector. If any grid point ID is invalid, 
			//                   
			void JoinGridPoints(std::vector<int> &in_gridPoints);

			// Description: 
			//
			// Pre-conditions:
			// Post-conditions:
			void JoinGridPoint(int in_gridPoint);

			

			/******************************************************************
					Helper sub-parsing functions
			/******************************************************************/
			void ParseSubcase(
					std::vector<std::string>				&subcaseStr, 
					SubcaseStatements						&subcaseOptions)
					throw (isis::application_exception);


			//From Ziyi's functions

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Description:
			//			Distance between two points (points have x, y, and z coordinates)
			//
			// Pre-Conditions:
			//			in_Point_1.x must contain x coordinate
			//			in_Point_1.y must contain y coordinate
			//			in_Point_1.z must contain z coordinate
			//			in_Point_2.x must contain x coordinate
			//			in_Point_2.y must contain y coordinate
			//			in_Point_2.z must contain z coordinate
			//
			// Post-Conditions:
			//		if the two points are too close (determined by the value of in_Precision)
			//			then
			//				throw isis::application_exception
			//			else
			//				return their distance
			double DistanceBetweenPoints_3D( const Point_3D &in_Point_1,
											 const Point_3D &in_Point_2,
											 double in_Precision = DEFAULT_TOLERANCE )
											 throw (isis::application_exception);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Description:
			//			Get normal vector of a plane
			//
			// Pre-Conditions:
			//			v1, v2 ,v3 are three points of a triangle.
			//          This three points must be able to form a triangle.
			//
			// Post-Conditions:
			//		    return normal vector of this plane
			Point_3D GetVector_3D( const Point_3D &v1,
								   const Point_3D &v2,
								   const Point_3D &v3 );

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Description:
			//			Find circumsphere radius
			//
			// Pre-Conditions:
			//			in_Points must contain 4 points and must be ordered
			//          Reference: Nastran 2010 Getting Started with MD Nastran Guide.pdf (page 93)
			//                     points G1, G2, G3, G4 in Figure 6-19
			//
			// Post-Conditions:
			//		if the any two points are too close
			//			then
			//				throw isis::application_exception
			//			else
			//				return circumsphere radius
			double FindCircumsphereRadius( const std::vector<Point_3D> &p,
										   const double *edge );

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Description:
			//			Find the minimum angle's cosine value of a triangle
			//
			// Pre-Conditions:
			//			in_side_1, in_side_2, in_side_3 are the lengths of three sides(which should be greater than 0.0 ),
			//          and any side should less than the sum of the other two.
			//
			// Post-Conditions:
			//
			//			result[0] will be the maximumAngle (0 - p)
			//          result[1] will be the minimumAngle (0 - p)
			void GetMaxAndMinAngle( const double &in_side_1,
									const double &in_side_2,
									const double &in_side_3,
									Triangle &result );

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Description:
			//			Find the ratio    Longest_Line/Shortest_Line
			//          Find minimum angle
			//
			// Pre-Conditions:
			//			in_Points must contain 4 points and must be ordered
			//          Reference: Nastran 2010 Getting Started with MD Nastran Guide.pdf (page 93)
			//                     points G1, G2, G3, G4 in Figure 6-19
			//
			// Post-Conditions:
			//		if the any two points are too close
			//			then
			//				throw isis::application_exception
			//			else
			//				return out_ElementDistortionMetrics with the minimum angle and ratio of Longest_Line/Shortest_Line
			void ComputeTetraElementDistortionMetrics(const std::vector<Point_3D> &in_Points,
													  ElementDistortionMetrics &out_ElementDistortionMetrics);


	};

	std::ostream& operator<<(std::ostream &output, const std::map<int, GridPoint> &in_gridPoints_map );
	std::string DoubleToString(double in_data, std::streamsize in_precision = 8);
	std::string GridPointToString(const GridPoint &myGrid);
} // End namespace isis

#endif 
