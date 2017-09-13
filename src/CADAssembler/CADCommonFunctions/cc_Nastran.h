#ifndef NASTRAN_H
#define NASTRAN_H

#include <list>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "isis_application_exception.h"
#include "cc_GraphicsFunctions.h"
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

	struct MAT1
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

		MAT1() : name("MAT1"), MID(0){};
	};

	struct MAT4
	{
		// Defines the constant or temperature-dependent thermal material properties for conductivity,
		// heat capacity, density, dynamic viscosity, heat generation, reference enthalpy, and latent heat
		// associated with a single-phase change.
		std::string name;
		int MID;			// Material identification number. (Integer > 0)
		std::string K;      // Thermal conductivity. (Blank or Real > 0.0)
		std::string CP;     // Heat capacity per unit mass at constant pressure (specific heat). (Blank or
							// Real > 0.0)
		std::string p;		// Density. (Real > 0.0; Default = 1.0)
		std::string H;		// Free convection heat transfer coefficient. (Real or blank)
		std::string m;		// Dynamic viscosity. See Remark 2. (Real > 0.0 or blank)
		std::string HGEN;   // Heat generation capability used with QVOL entries. (Real > 0.0; Default = 1.0)
		std::string REFENTH;// Reference enthalpy. (Real or blank)
		std::string TCH;    // Lower temperature limit at which phase change region is to occur. (Real or
							// blank)
		std::string TDELTA; // Total temperature change range within which a phase change is to occur.
							// (Real > 0.0 or blank)
		std::string QLAT;   // Latent heat of fusion per unit mass associated with the phase change. (Real > 0.0
							// or blank)

		MAT4(): name("MAT4"),MID(0) {};
	};


	struct TEMP
	{
		// Defines temperature at grid points for determination of thermal loading,
		// temperature-dependent material properties, or stress recovery.

		int SID;  // Temperature set identification number. (Integer > 0)
		std::string name;
		int			G1;  // Grid point identification number. (Integer > 0)
		std::string T1;  // Temperature. (Real)
		int			G2;  // Grid point identification number. (Integer > 0)
		std::string T2;  // Temperature. (Real)
		int			G3;  // Grid point identification number. (Integer > 0)
		std::string T3;  // Temperature. (Real)

		TEMP(): name("TEMP"), SID(0), G1(0), G2(0), G3(0) {};

	};

	struct TEMPD
	{

		// Defines a temperature value for all grid points of the structural model that have not been given
		// a temperature on a TEMP entry.

		// NOTES
		//	1) If a card has more than one entry (i.e. SID/T pairs) the card will be broken up into multiple cards.
		//  2) if there are more than one SIDs, the SIDs must not be the same.  For a SID, there can be one and
		//	   only one default temperature.
		//	3) If TEMPD card contains more than one SID, then that card will be broken up into separate TEMPD structures.

		std::string name;
		int			SID;	// Temperature set identification number. (Integer > 0)
		std::string T;	// Default temperature value. (Real)
		//int			SID2;	// Temperature set identification number. (Integer > 0)
		//std::string T2;		// Default temperature value. (Real)
		//int			SID3;	// Temperature set identification number. (Integer > 0)
		//std::string T3;		// Default temperature value. (Real)
		//int			SID4;	// Temperature set identification number. (Integer > 0)
		//std::string T4;		// Default temperature value. (Real)

		TEMPD() : name("TEMPD"), SID(0)  {};
	};

	struct SPOINT
	{
		// Note - If more than one ID exists on a card, this program will output those as separate
		//		  SPOINT cards.
		// Defines scalar points
		std::string name;
		int			ID;  // Scalar point identification number.

		//std::string ID2;  
		//std::string ID3;  
		//std::string ID4;  
		//std::string ID5;  
		//std::string ID6;  
		//std::string ID7;
		//std::string ID8;

		SPOINT() : name("SPOINT"), ID (0) {};
	};

	struct PCONV
	{
		// Specifies the free convection boundary condition properties of a boundary condition surface
		// element used for heat transfer analysis.
		std::string name;
		int			PCONID; // Convection property identification number. (Integer > 0)
		int			MID;	// Material property identification number. (Integer > 0)
		std::string FORM;	// Type of formula used for free convection. (Integer 0, 1, 10, 11, 20, or 21; Default = 0)
		std::string EXPF;   // Free convection exponent as implemented within the context of the particular form that is chosen.

		PCONV() : name("PCONV"), PCONID(0), MID(0) {};
	};

	struct CONV
	{
		// Specifies the free convection boundary condition properties of a boundary condition surface
		// element used for heat transfer analysis.
		std::string name;
		int			EID;     // CHBDYG, CHBDYE, or CHBDYP surface element identification number. (Integer > 0)
		int			PCONID;  // Convection property identification number of a PCONV entry. (Integer > 0)
		std::string FLMND;	 // Point for film convection fluid property temperature. (Integer > 0; Default = 0)
		std::string CNTRLND; // Control point for free convection boundary condition. (Integer > 0; Default = 0)
		int TA1;	 // Ambient points used for convection. (Integer > 0 for TA1 and Integer > 0 for TA2
							 // through TA8; Default for TA2 through TA8 is TA1.)
		int TA2;
		int TA3;
		int TA4;
		int TA5;
		int TA6;
		int TA7;
		int TA8;

		CONV(): name("CONV"), EID(0), PCONID(0), TA1(0), TA2(0), TA3(0), TA4(0), TA5(0), TA6(0), TA7(0), TA8(0){};
	};

	struct CHBDYG
	{
		// Defines a boundary condition surface element without reference to a property entry.
		std::string name;
		int			 EID;	// Surface element identification number. (Unique (0 < Integer < 100,000,000) among
							// all elemental entries.)
		std::string TYPE;	// Surface type. TYPE specifies the kind of element surface; allowed types are: REV, AREA3, AREA4,
							// AREA6, and AREA8.
		std::string IVIEWF;	// A VIEW entry identification number for the front face. (Integer > 0). 
							// See MSC Thermal Analysis User's Guide for details
		std::string IVIEWB; // A VIEW entry identification number for the back face. (Integer > 0;).
							// See MSC Thermal Analysis User's Guide for details
		std::string RADMIDF;// RADM identification number for front face of surface element. (Integer > 0)
							// See MSC Thermal Analysis User's Guide for details
		std::string RADMIDB;// RADM identification number for back face of surface element. (Integer > 0) 
							// See MSC Thermal Analysis User's Guide for details

		int G1;		//  Grid point IDs of grids bounding the surface. (Integer > 0)
		int G2;
		int G3;
		int G4;
		int G5;
		int G6;
		int G7;
		int G8;

		CHBDYG(): name("CHBDYG"), EID(0), G1(0), G2(0), G3(0), G4(0), G5(0), G6(0), G7(0), G8(0) {};
	};

	struct QBDY3
	{
		// Defines a uniform heat flux into a set of grid points.

		// Warning - This program does not suppor the THRU and BY constructs for EID.
		std::string name;
		int SID;			 // Load set identification number. (Integer > 0)
		std::string Q0;		 // Thermal heat flux load, or load multiplier. Q0 is positive for heat flow into a surface. (Real)
		std::string CNTRLND; // Control point for thermal flux load. (Integer > 0; Default = 0)	
		int		EID1;		 // EIDrid point identification of connected EIDrid points. (InteEIDer > 0 or blank)
		int		EID2;	
		int		EID3;	
		int		EID4;	
		int		EID5;	
		int		EID6;	
		int		EID7;	
		int		EID8;	

		QBDY3() : name("QBDY3"), SID(0), EID1(0), EID2(0), EID3(0), EID4(0), EID5(0), EID6(0), EID7(0), EID8(0) {};
	};

	struct QVOL  // Volume Heat Addition
	{
		// Defines a rate of volumetric heat addition in a conduction element.
		std::string name;
		int				SID;		//	Load set identification. (Integer > 0)
		std::string		QVOL_Value;	//	Power input per unit volume produced by a heat conduction element. (Real)
		std::string		CNTRLND;	//	Control point used for controlling heat generation. (Integer > 0; Default = 0)
		int				EID1;		//  Heat conduction elements. (Integer > 0
		int				EID2;				
		int				EID3;	
		int				EID4;	
		int				EID5;		

		QVOL() : name("QVOL"), EID1(0), EID2(0), EID3(0), EID4(0), EID5(0) {};
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
		ELEMENT_TYPE_UNDEFINED,
		CHEXA,
		CPENTA,
		CTETRA,
		CQUAD,
		CTRIA3
	};

	std::string ElementType_ToString( ElementType in_ElementType);

	struct FEAElement
	{
		ElementType Type;
		int EID;
		int PID;
		std::vector<int> GIDs;  // These GIDs must be in the order read from the card.
		double thickness;   // Applies to shell elements only

		//	Description:
		//		This function validates that the element type is a valid type.
		//	Pre-Conditions:
		//		None
		//	Post-Conditions
		//		if ( Type != CHEXA ||  Type != CPENTA || Type != CTETRA || Type != CQUAD || Type != CTRIA3 )
		//			throw exception
		//		else
		//			Do nothing
		void validateElementType_and_NumberGIDs( const std::string &in_CallingFunctionName) const throw (isis::application_exception);

		//	Description:
		//		This function computes the minimum number of GIDs for an element.  For example, a Tetra element must have a minimum of 4 elements.  
		//		It could have ten elements.
		//
		//		Supported elements: CHEXAC, CPENTA, CTETRA, CQUAD and CTRIA3 
		//
		//	Pre-Conditions:
		//		None
		//		if ( Type != CHEXA &&  Type != CPENTA &&Type != CTETRA && Type != CQUAD && Type != CTRIA3 )
		//			throw exception
		//		else
		//			Return minimum number of GIDs
		int getElementMinimumNumberOfGIDS() const;

		//	Description:
		//		This function computes the minimum number of GIDs for a surface.  For example, a Tetra element must have a minimum of 3 GIDs per surface.  
		//		It could have 6 elements per surface, but normally we are only interested in the corner elements, which would be the first 3.
		//
		//		Supported elements: CHEXAC, CTETRA, CQUAD and CTRIA3 
		//		CPENTA not supported because some surfaces have 3 nodes and other sufaces have 4 nodes.
		//
		//	Pre-Conditions:
		//		None
		//		if ( Type != CHEXA && Type != CTETRA && Type != CQUAD && Type != CTRIA3 )
		//			throw exception
		//		else
		//			Return minimum number of GIDs
		int FEAElement::getSurfaceMinimumNumberOfGIDS() const;

		//	Description:
		//		This function returns the corner GIDs of an element.  For example, a Tetra elements has four corner points 
		//		and thus four GIDs would be returned for a Tetra element.
		//
		//		validateElementType_and_NumberGIDs is called at the beginning of this function.
		//
		//		Notes - 1) out_ElementCornerPoints is cleared/emptied before adding the corner point GIDs
		//				2) Supported elements: CHEXAC, CPENTA, CTETRA, CQUAD and CTRIA3 
		//			
		//	Pre-Conditions:
		//		None
		//	Post-Conditions:
		//		If validateElementType_and_NumberGIDs finds an error
		//			throw (isis::application_exception)
		//		else
		//			out_ElementCornerPoints would be populated with the corner GIDs
		void getElementCornerPoints( std::vector<int> &out_ElementCornerPoints ) const throw (isis::application_exception);

		//	Description:
		//		Compute the volume of CTetra (4 or 10 node) elements.  Other elements 
		//		( e.g. CHexa, CPenta) are not supported at this time.  When needed, 
		//		this function should be modified to support those elements.
		//		
		//		Supported elements: CTETRA 
		//
		//	Pre-Conditions:
		//		The grid points in FEAElement must not be congruent.  If one or more grid points are
		//		congruent, then the behaviour of this function is undefined.
		//	Post-Conditions
		//		If FEAElement.Type != CTETRA
		//			throw sis::application_exception
		//		If FEAElement.GID.size() < 4
		//			throw sis::application_exception
		//		If a GID is not in in_GridPointData
		//			throw sis::application_exception
		//		If no exceptions
		//			return volume
		double getVolume( const std::map<int, GridPoint> &in_GridPointData ) const throw (isis::application_exception);
		
		// SurfaceID is just a unique ID for each surface within an element.  For example, a Tetra element has four surfaces, which
		// would be numbered 1, 2, 3, and 4.
		// The CornerPoints are in an order that would seem logical for the particular surface.  This order does not serve as a key.
		// To use the CornerPoints as a key, you must first sort them.
		//
		// Supported elements: CHEXAC, PENTA, CTETRA, CQUAD and CTRIA3 
		//
		void getCornerPointsForEachSurface( std::map<int, std::vector<int>> &out_SurfaceID_to_CornerPoints_map ) const;

		// This function finds the diagonal point for an element.  This is a point used by certain cards (e.g. PLOAD4) to 
		// identify a particular surface on an element.
		// Note -	in_SurfacePoints does not need to be sorted.  It will be copied to another vector then sorted.  The
		//			sorted vector is used to find the particular surface.
		// -1 will be returned for elements that have only one surface (e.g. CQUAD, CTRIA3) and thus do not need diagonal points.
		//
		//	in_SurfacePoints can include the surface corner points followed by any mid points, or it could just be
		//	the surface corner points.  If the mid points are present, they will be ignored.
		//
		//
		void FEAElement::getDiagonalPointForSurface( const std::vector<int> &in_SurfacePoints,
													int    &out_AnchorPoint,
													int    &out_DiagonalPoint) const;

		FEAElement();

		friend std::ostream& operator<<(std::ostream& stream, FEAElement &myElement);
	};


	struct SPC
	{
		int SID;
		std::string G1;
		std::string C1;
		std::string D1;
		friend std::ostream& operator<<(std::ostream& stream, const SPC &myConstraint);
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
		friend std::ostream& operator<<(std::ostream& stream, const Force &myForce);
	};

	struct Acceleration   // GRAV card
	{
		int SID;
		int CID;
		std::string A;
		std::string N1;  // Acceleration vector
		std::string N2;
		std::string N3;

		friend std::ostream& operator<<(std::ostream& stream, const Acceleration &myAcceleration);
	};


	struct Pressure   // PLOAD4
	{
		int SID;
		int EID;
		std::string P1;
		std::string P2;
		std::string P3;
		std::string P4;
		std::string G1;
		std::string G3_or_G4;

		friend std::ostream& operator<<(std::ostream& stream, const Pressure &myPressure);
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
	//	std::multimap<int, FEAElement>			elementData;				// elements: <PID, element>
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
	//	//  corresponding vector of all FEAElement objects which are contained within
	//	//  the PSolids. This function is invoked in the FindGridPointsFromPSolids function
	//	//
	//	// Pre-conditions:  in_PSolidIDs is a valid std::vector<int>
	//	//                  out_elements is empty (although we ensure this later)
	//	// Post-conditions: An exception is thrown if any PSolid ID is invalid
	//	//                  Otherwise, the corresponding vector of FEAElement objects 
	//	//                   is returned by parameter
	//	void FindElementsFromPSolids(std::vector<int> &in_PSolidIDs, std::vector<FEAElement> &out_elements);

	//	// Description: Given a vector of SolidElements, we return a vector of gridPoints that belong to those
	//	//  FEAElement objects.
	//	//
	//	// Pre-conditions:  in_Elements is a valid vector of FEAElement objects 
	//	// Post-conditions: An exception is raised if any of the elements have an invalid ID
	//	//                  If no exception is raised, we return a vector of gridPoints that compose the 
	//	//                   FEAElement objects
	//	//
	//	void FindGridPointsFromElements(std::vector<FEAElement> &in_ElementIDs, std::vector<int> &out_gridPoints);

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
			std::map<int, MAT1>			materialData_MAT1; // MAT1 and MAT4 materials, others not supported right now
			std::map<int, MAT4>			materialData_MAT4; 
			std::multimap<int, TEMP>	temperature_TEMP; 
			std::multimap<int, TEMPD>	temperature_TEMPD; 
			std::map<int, SPOINT>		point_SPOINT; 
			std::map<int, PCONV>		convection_PCONV; 
			std::map<int, CONV>			convection_CONV; 
			std::map<int, CHBDYG>		surfaceElement_CHBDYG; 
			std::multimap<int, QBDY3>	heatFlux_QBDY3; 
			std::multimap<int, QVOL>	heatVolume_QVOL; 

			std::map<int, GridPoint> gridPointData;	// grid points
			int	 gridPointIDCurrentMax;
			std::map<int, PSolid> psolidData; // PSolids: <PID, psolid>
			std::map<int, FEAElement> elementData; // elements: <EID, element> - used to be multimap, but we
			// need to index into it. Hopefully this doesn't hurt us later

			std::map<int, CoordSystem> coordSystems;

			std::multimap<int, SPC>	spcData_SIDKey; // SPC with SID ( constraint set) Key
			std::multimap<int, SPC>	spcData_GridIDKey; // SPC with grid ID key
			std::multimap<int, Force> forceLoadData; // Force
			std::multimap<int, Acceleration> accelerationLoadData; // Acceleration
			std::multimap<int, Pressure> pressureLoadData; // Pressure


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

			bool MATERIALS_PLACEMENT_MAT4_set;
			bool TEMPERATURE_TEMP_set;
			bool TEMPERATURE_TEMPD_set;
			bool POINT_SPOINT_set;
			bool CONVECTION_PCONV_set;
			bool CONVECTION_CONV_set;
			bool SURFACE_ELEMENT_CHBDYG_set;
			bool SURFACE_ELEMENT_QBDY3_set;
			bool VOLUME_ELEMENT_QVOL_set;

		public:
			NastranDeck();
			int createdElementCounter;
			
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
			const std::map<int, MAT1>& getMaterialData_MAT1() const; 
			const std::map<int, MAT4>& getMaterialData_MAT4() const; 
			void deleteAllMaterialData_MAT4();
			void AddMaterialData_MAT4( const MAT4 &in_MAT4);

			const std::multimap<int, TEMP>& getTemperature_TEMP() const; 
			const std::multimap<int, TEMPD>& getTemperature_TEMPD() const;

			void  AddTemperature_TEMPD(const TEMPD &in_TEMPD);
			void  AddTemperature_TEMP(const TEMP &in_TEMP);

			const std::map<int, SPOINT>& getPoint_SPOINT() const;
			void	AddPoint_SPOINT(const SPOINT &in_SPOINT);

			const std::map<int, PCONV>& getConvection_PCONV() const ; 
			void AddConvection_PCONV( const PCONV &in_PCONV );

			const std::map<int, CONV>& getConvection_CONV() const;
			void AddConvection_CONV( const CONV &in_CONV );


			const std::map<int, CHBDYG>& getSurfaceElement_CHBDYG() const ; 
			void  AddSurfaceElement_CHBDYG(const CHBDYG &in_CHBDYG);

			const std::multimap<int, QBDY3>& getHeatFlux_QBDY3() const; 
			void  AddHeatFlux_QBDY3(const QBDY3 &in_QBDY3);

			const std::multimap<int, QVOL>& getHeatVolume_QVOL() const; 
			void  AddHeatVolume_QVOL(const QVOL &in_QVOL);

			std::map<int, GridPoint>& getGridPointData();
			int	getNextGridPointID();

			std::map<int, PSolid>& getPsolidData(); 
			const std::map<int, FEAElement>& getElementData() const; 
			std::map<int, CoordSystem>& getCoordSystems();
			std::multimap<int, SPC>& getSpcData_SIDKey();
			const std::multimap<int, SPC>& getSpcData_GridIDKey() const;
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
			void AddSubCaseAndLoadStatement( int in_SubCaseID, int in_ConstraintSetID, int in_LoadStatementID, int in_LoadSetID, 
											 bool in_IncludeSubcaseStatement = true);

			void AddCardToCaseControl( const std::string &in_Card);
			void NastranDeck::ReplaceExecutiveControlWithThermalStatements();
			void ReplaceCaseControlWithThermalStatements(	const	std::string &in_Title,	
															//int		in_TemperatureInitialID, 
															int		in_NLParmID );



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
			void AddPressureToDeck( int in_LoadID, int in_ElemID, double in_Pressure, int in_Grid_1, int in_Grid_3_or_4 );


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


			void NastranDeck::CommentOutMat1Cards();

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
			//  corresponding vector of all FEAElement objects which are contained within
			//  the PSolids. This function is invoked in the FindGridPointsFromPSolids function
			//
			// Pre-conditions:  None.
			// Post-conditions: An exception is thrown if any PSolid ID is invalid
			//  Otherwise, the corresponding vector of FEAElement objects is
			//   returned by parameter
			void FindElementsFromPSolids(const std::vector<int> &in_PSolidIDs, std::vector<int> &out_elementIDs);

			//	Description:
			//		Compute the total volume of all the in_PSolidIDs elements. 
			//		This function calls FEAElement:getVolume for each element identified by in_PSolidIDs.
			//	Pre-Conditions:
			//		The pre-conditions for FEAElement:getVolume apply to this function
			//	Post-Conditions
			//		The exceptions for FEAElement:getVolume apply to this function.
			//		if (in_PSolidIDs.empty())
			//			throw isis::application_exception
			//		if ( in_PSolidIDs contains an invalide PSolidID )
			//			throw isis::application_exception
			//		if no elements found for one or more in_PSolidIDs
			//			throw isis::application_exception
			//		if no exceptions
			//			return the total volume of all the in_PSolidIDs
			double VolumeOfPSolids(const std::vector<int> &in_PSolidIDs);

			// Description: Given a vector of SolidElements, we return a vector of gridPoints
			//  that belong to those FEAElement objects.
			//
			// Pre-conditions:  None.
			// Post-conditions: An exception is raised if any of the elements have an invalid ID
			//  If no exception is raised, we return a vector of gridPoints that compose the 
			//   FEAElement objects
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
			//  It finds every FEAElement with that GridPoint, and
			//  marks it. Then, we find out if any of the marked FEAElement 
			//  objects belong to different parts. If not, we do nothing. 
			//  If so, then we keep all the entries relating to one of the 
			//  PSolids the same. For each of the others PSolids, we create a new
			//  grid point with the same values as in_gridPoint (but with 
			//  a different GID, of course). Then for each FEAElement of
			//  that PSolid, we create 
			//
			//	Pre-conditions: in_gridPoint is a valid grid point ID.
			//  Post-conditions: If in_gridPoint is not a valid ID, an exception
			//   is thrown. Otherwise, for each part (PSolid) in_gridPoint 
			//   belongs to, a new grid point is created, and each FEAElement
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


	struct HeatFluxLoad
	{
		int	elementIDThatContainsSurface;  // Tetra element ID
		std::vector<int> surfaceGridPointIDs; 
		double Q0; // Heat flux load
		int SID; // Load set identification number. (Integer > 0)
		std::string name;

		HeatFluxLoad() : Q0(0.0), elementIDThatContainsSurface(0) {};
	};

	struct SurfaceConvection
	{
		int	convID;  // Convection ID
		int	pconvID;  // Convection property identification number of a PCONV entry. (Integer > 0) 
		int mat4ID;
		int spointID;
		int	elementIDThatContainsSurface;     // Tetra element ID
		std::vector<int> surfaceGridPointIDs; 
		double				convectionCoefficient;
		double				ambientTemperature;
		//int CID; // Constraint set identification number. (Integer > 0)

		SurfaceConvection() : convID(0), pconvID(0), mat4ID(0), spointID(0), elementIDThatContainsSurface(0),  convectionCoefficient(0.0), ambientTemperature(0.0) {};
	};

	struct VolumetricHeatGeneration
	{
		// J/(sec * mm^3)
		double powerInputPerVolume;
		std::vector<int> elementIDs;
	};

	class NastranDeckHelper
	{
		// This class assumes that all the loads/constraints defined in nastranDeck apply.  In other words, there is
		// not support for a nastranDeck defining multiple runs based on load/constraint sets.  nastranDeck would have
		// be created in such a manner as to support only one run and thus all loads/constraints would apply to the run.

		public:
			NastranDeckHelper( const NastranDeck &in_NastranDeck );

			// Description:
			//		The DefaultGridPointTemperature is the temperature of all grid points that do not have an explicit temperature
			//		assignment.
			//		This may/may-not be set.
			//
			// Pre-Conditions:
			//		None
			// Post-Conditions:
			//		An exception would occur if nastranDeck had more than one DefaultGridPointTemperature setting.  This would
			//		indicate erroneous data in the deck and would be due to a bug in the code that created nastranDeck.
			//		An exception would also be thrown if the temperature-data string in nastranDeck did not represent a floating point number.
			//		This would have been due to a coding error in the NastranDeck class.
			//		If no exceptions:
			//			return out_DefaultGridPointTemperature_set and out_DefaultGridPointTemperature wherein
			//				out_DefaultGridPointTemperature_set would be false if nastranDeck did not have a DefaultGridPointTemperature
			//				setting. 
			//				if out_DefaultGridPointTemperature_set == true
			//					then
			//						out_DefaultGridPointTemperature would be set with the DefaultGridPointTemperature
			void getDefaultGridPointTemperature( bool &out_DefaultGridPointTemperature_set, double &out_DefaultGridPointTemperature ) const
																								throw (isis::application_exception);
			// Description:
			//		Defines temperature at grid points for determination of thermal loading.
			// Pre-Conditions:
			//		None
			// Post-Conditions:
			//		out_GridPointToTemperature_map will be cleared (erased) before reading the data from nastranDeck.
			//		If no specified grid point temperatures were present in nastranDeck then out_GridPointToTemperature_map.size() == 0
			//		If nastranDeck contained erroneous data (e.g. temperature not a float) then isis::application_exception would be thrown.
			//		Otherwise, out_GridPointToTemperature_map would be returned appropriately populated.
			void getSpecifiedGridPointTemperatures( std::map<int,double> &out_GridPointToTemperature_map ) const
																								throw (isis::application_exception);

			// Description:
			//		This function searches for an element based on a key, which consists of three grid point IDs.
			//		Only tetra elements are currently supported.
			//		The key would be three grid point IDs representing a single triangular face of a tetra element 
			//		The first element found that has the key is returned.  It is common that a triangular face is shared
			//		between tetra elements and thus would be associated with possibly  two elements.
			// Pre-Conditions:
			//		This function only works with Tetra elements.
			// Post-Conditions:
			//		if "NastranDeck::elementData" contains element types other than Tetra, then isis::application_exception would be thrown
			//		if in_SurfaceCornerGridIDs found
			//			then
			//				out_ElementFound set to true
			//				out_ElementID set to the element ID containing the face
			void findElementsContainingSurface(		const std::vector<int> &in_SurfaceCornerGridIDs, 
													bool &out_ElementFound, 
													std::set<int> &out_ElementIDs )throw (isis::application_exception);

			// Description:
			//		This function return the heat flux load for boundary surfaces.  It currently does not test whether the surface
			//      is actually a boundary surface.  That check will be added later.
			//		See the QBDY3 card description in the Nastran documentation.
			// Pre-Conditions:
			//		This function only works with Tetra elements.
			// Post-Conditions:
			//		if the deck contains erroneous data pertinent to heat flux or locating the surfaces associated with a heat flux then
			//			isis::application_exception would be thrown.
			//		if there are no heat fluxes defined the deck, then out_HeatFluxLoads.size() == 0
			//		Otherwise, return a populated out_HeatFluxLoads
			void getHeatFluxLoadsForBoundarySurfaces( std::vector<HeatFluxLoad> &out_HeatFluxLoads ) throw (isis::application_exception);

			// Description:
			//		This function returns the free convection boundary condition for heat transfer analysis through connection to
			//		a surface element.  See CONV card description in the Nastran documentation.
			// Pre-Conditions:
			//		This function only works with Tetra elements.
			// Post-Conditions:
			//		if the deck contains erroneous data pertinent to convection or locating the surfaces associated with the convection boundary then
			//			isis::application_exception would be thrown.
			//		if there are no convection data defined the deck, then out_SurfaceConvections.size() == 0
			//		Otherwise, return a populated out_SurfaceConvections
			void getSurfaceConvectionConstraints ( std::vector<SurfaceConvection> &out_SurfaceConvections ) throw (isis::application_exception);

			// Description:
			//		This function finds the surfaces that have corner grid points in in_GridPointIDs.
			//		out_ElementID_to_SurfacePoints_map returns the element id along with the complete set of points defining the surface.
			//		The complete set of points includes the corner points and the mid points.  The points are ordered per Nastran's
			//		grid point element numbering.
			// Pre-Conditions:
			//		This function only works with Tetra elements.
			// Post-Conditions:
			//		if the deck contains erroneous data pertinent to locating the surfaces then
			//			isis::application_exception would be thrown.
			//		if there are no surfaces found,  then out_ElementID_to_SurfacePoints_map.size() == 0
			//		Otherwise, return a populated out_ElementID_to_SurfacePoints_map
			//
			//     	if ( elem_itr->second.Type != CTETRA &&  elem_itr->second.Type != CQUAD )
			//			throw (isis::application_exception)
			//
			//	Supported Element Types: CTETRA and CQUAD
			//
			void getSurfaceElementsContainingGridPoints ( const std::set<int> &in_GridPointIDs, 
														  std::multimap< int, std::vector<int>> &out_ElementID_to_SurfacePoints_map )
														  throw (isis::application_exception);

			// Description:
			//		This function returns the volumetric-heat-generation value information in the structure  VolumetricHeatGeneration.
			//		This structure contains the value for powerInputPerVolume and elementIDs.
			//		See QVOL card description in the Nastran documentation.
			// Pre-Conditions:
			//		This function only works with Tetra elements.
			// Post-Conditions:
			//		if there are no volumetric-heat-generation data defined in the deck, then out_VolumetricHeatGenerations.size() == 0
			//		Otherwise, return a populated out_VolumetricHeatGenerations
			void getVolumetricHeatGenerations( std::vector<VolumetricHeatGeneration> &out_VolumetricHeatGenerations );

		private:
			const NastranDeck &nastranDeck;
			// Don't allow this class to be instantiated without specifying  the NastranDeck as an input.
			NastranDeckHelper();

			bool sufaceCornerGridIDs_to_FEAElementIDs_map_populated;  // Initially set to false by the constructor

			// For sufaceIDs_to_SolidElement_map:
			//	1) Only tetra elements are currently supported
			//	2) The key (i.e. std::vector<int>) is the four corner points representing the four faces of a tetra element.
			//	3) The keys for a tetra with the four corner points numbered 1, 2, 3, 4 are
			//	   Face 1, Keys: 1 2 3
			//	   Face 2, Keys: 1 2 4
			//	   Face 3, Keys: 1 3 4
			//	   Face 4, Keys: 2 3 4
			std::map<std::vector<int>, std::set<int>> sufaceCornerGridIDs_to_FEAElementIDs_map;  // surfaceIDs represent a key to a solid element


			// Supports Elements: CHEXAC, PENTA, CTETRA, CQUAD and CTRIA3 
			void populate_sufaceCornerGridIDs_to_FEAElementIDs_map();
	};

	
} // End namespace isis

#endif 
