#ifndef  CC_COMMON_STRUCTURES_H
#define  CC_COMMON_STRUCTURES_H
#include "isis_application_exception.h"
#include "CADStringToEnumConversions.h"
#include "MultiFormatString.h"
#include "GraphicsFunctions.h"
#include "Joint.h"   
#include <iostream>
#include <ostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <limits>
#include <boost/functional/hash.hpp>

typedef               double  MatrixD4x4[4][4];

namespace isis
{
	//std::ostream& operator <<(std::ostream & out, const bool & value);
	std::string  Bool_string( bool in_Bool );

	struct CADAssembledFeature
	{
		e_CADFeatureGeometryType type;		// ProType  type;
		int      id;
		void*    owner;   					// ProMdl owner; where typedef void* ProMdl;
	};

	const int CAD_NAME_SIZE = 32;	// This is the same size as PRO_NAME_SIZE, which means the max model name is 31 characters.  The 32nd character is a null terminator.
									// In general, other CAD systems allow longer names, but we probably should stick to the shorter Creo name limitation so that we can
									// build the same CyPhy model using multiple CAD systems.

	const int CAD_LINE_SIZE = 81;   // Same comment as for CAD_NAME_SIZE but applies to PRO_LINE_SIZE.

	typedef std::list<int> IntList;

	template <typename Container> struct ContainerHash 
	{
	std::size_t operator()(Container const& c) const 
		{
			return boost::hash_range(c.begin(), c.end());
		}
	};

	const int c_InifiteLifeNumberOfCycles = 500000000;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*! \struct ConstraintOffset
    \brief Structure that defines a constraint offset.

	A constraint offset is the distance from a datum to the desired placement.
	*/
	struct ConstraintOffset
	{
		e_CADAssemblyConstraintType offsetAlignmentType;
		// ProAsmcompConstrType	offsetAlignmentType; /**< PRO_ASM_ALIGN_OFF, PRO_ASM_MATE_OFF */
		double					value;

		bool					unitsPresent;				
		e_CADUnitsDistance		units;				/**<CAD_UNITS_MM, CAD_UNITS_CM, CAD_UNITS_INCH */

		ConstraintOffset()
			//: offsetAlignmentType(PRO_ASM_MATE_OFF), 
			: offsetAlignmentType( CAD_ASM_MATE_OFF),
			value(0.0),
			unitsPresent(false), 
			units(CAD_UNITS_MM) 
		{}

	};

	/*! \struct ConstraintFeature
    \brief Structure that defines a constraint feature.

	A constraint feature defines the geometry that is used to define the constraint.
	*/
	struct ConstraintFeature
	{
		std::string			componentInstanceID;
		//std::string		featureName;
		MultiFormatString	featureName;
		// ProDatumside		featureOrientationType;   /**< PRO_DATUM_SIDE_NONE, PRO_DATUM_SIDE_YELLOW, PRO_DATUM_SIDE_RED */
		e_CADDatumside		featureOrientationType;   /**< PRO_DATUM_SIDE_NONE, PRO_DATUM_SIDE_YELLOW, PRO_DATUM_SIDE_RED */

		ConstraintFeature()
			//:featureOrientationType(PRO_DATUM_SIDE_NONE),
			:featureOrientationType(CAD_DATUM_SIDE_NONE),
			 featureName(CAD_NAME_SIZE - 1)
		{}
	};


	/*! \struct ConstraintPair
    \brief Structure that defines a constraint pair.

	A constraint pair defines the meta data about constraint.
	*/
	struct ConstraintPair
	{
		e_CADAssemblyConstraintType featureAlignmentType;
		//ProAsmcompConstrType			featureAlignmentType;     // PRO_ASM_ALIGN, PRO_ASM_MATE, PRO_ASM_MATE_OFF, always PRO_ASM_ALIGN for now 

		//ProType							featureGeometryType;      // PRO_SURFACE, PRO_AXIS
		e_CADFeatureGeometryType		featureGeometryType;
		e_FeatureInterfaceType			featureInterfaceType;     // CAD_DATUM, CAD_MODEL_INTERFACE, CAD_MODEL_USER_DATA 
		e_CADTreatConstraintAsAGuide	treatConstraintAsAGuide;  // Means that it would be applied and then removed.  This would establish an initial position for kinematic joints.
		
		std::vector<ConstraintFeature>	constraintFeatures;

		bool						constraintOffsetPresent;
		ConstraintOffset			constraintOffset;

		ConstraintPair() 
			: constraintOffsetPresent(false), 
			featureInterfaceType(CAD_DATUM), 
			treatConstraintAsAGuide(CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE) 
		{}
	};


	struct InputJoint
	{
		std::string JointID;
		enum LimitRefRole
		{
			DEFAULT,
			MIN,
			MAX,
		};
		struct LimitRef
		{
			std::string ComponentID;
			std::string FeatureName;
			//ProType FeatureType;
			e_CADFeatureGeometryType FeatureType;
			bool Provided;
		};
		struct Limit
		{
			double Value;
			bool Provided;
		};
		InputJoint()
		{
			RotationLimits[DEFAULT].Provided = RotationLimits[MIN].Provided = RotationLimits[MAX].Provided = false;
			RotationLimitRefs_This[DEFAULT].Provided = RotationLimitRefs_This[MIN].Provided = RotationLimitRefs_This[MAX].Provided = false;
			RotationLimitRefs_Other[DEFAULT].Provided = RotationLimitRefs_Other[MIN].Provided = RotationLimitRefs_Other[MAX].Provided = false;
			TranslationLimits[DEFAULT].Provided = TranslationLimits[MIN].Provided = TranslationLimits[MAX].Provided = false;
			TranslationLimitRefs_This[DEFAULT].Provided = TranslationLimitRefs_This[MIN].Provided = TranslationLimitRefs_This[MAX].Provided = false;
			TranslationLimitRefs_Other[DEFAULT].Provided = TranslationLimitRefs_Other[MIN].Provided = TranslationLimitRefs_Other[MAX].Provided = false;
			Type = UNKNOWN_JOINT_TYPE;
		};
		e_CADJointType Type;

		LimitRef RotationLimitRefs_This[MAX+1]; // Limit refs in base model
		LimitRef RotationLimitRefs_Other[MAX+1]; // Limit refs in added model
		Limit RotationLimits[MAX+1];

		LimitRef TranslationLimitRefs_This[MAX+1]; // Limit refs in base model
		LimitRef TranslationLimitRefs_Other[MAX+1]; // Limit refs in added model
		Limit TranslationLimits[MAX+1];

	};


	struct JointPosition
	{
		isis_CADCommon::Point_3D location;			//	Offset definining the location of the joint relative.
		isis_CADCommon::Point_3D orientation;		//	Vector defining the orientation of the joint (e.g. revolute joint: this would be the axis 
													//	direction, planar joint: this would be the normal to the plane...).  
		double					 rotation;			//	Angle of rotation about the orientation axis.  Postive direction is per the
													//  the right-hand rule.  This is probably not needed.  Will leave for now.
		JointPosition() : location(0.0,0.0,0.0), orientation(0.0,0.0,0.0), rotation(0.0) {};
	};

	struct ComputedJointData
	{
		std::string coordinatesystem; // This has been computed in the coordinate system of this component

		bool junctiondDefined_withoutGuide;
		isis::cad::Junction junction_withoutguide;
		e_CADJointType  jointType_withoutguide;

		bool junctiondDefined_withGuide;
		isis::cad::Junction junction_withguide;
		e_CADJointType  jointType_withguide;

		bool globalJointPositionDefined;
		JointPosition globalJointPosition;  // Relative to the top-assembly coordinate system

		// The local joint position will not be used at this time. The local position could be set with the current code for the 
		// added .prt/.asm but not for the constrained-to .prts/.asms when there was more than one constrained-to .prts/.asms.  
		// This is because the Joint (see Joint.h, Joint::pair_t infer_joint_pair(std::vector<Joint::pair_t> in_joints) 
		// class computes the location, orientation, and rotation in the .prt/.asm  coordinate system.  If a .prt/.asm was 
		// constrained to more than one .prt/.asm the Joint class could not compute the correct joint type because the datums 
		// would be miss-oriented because they would be in the coordinate system of the particular .prt/.asm and not 
		// in the global coordinate system.   Some modifications to the Joint class would be necessary to support the 
		// case of multiple constrained-to .prts/.asms as well as modifications in how this program works.  
		// The .prts/.asms would have to be assembled first before determining the joint type of the constrained-to components, 
		// and once that was known, it could be validated that the assembled .prt/.asm has the same joint type as the constrained-to 
		// .prt(s)/.asm(s).  
		//std::map<std::string, JointPosition> localJointPosition_map;		// Joint position of the constrained-to .prt(s)/.asm(s) int the .prt/.asm
																			// coordinate system.

		ComputedJointData():	junctiondDefined_withoutGuide(false), 
								junctiondDefined_withGuide(false), 
								globalJointPositionDefined(false), 
								jointType_withoutguide(UNKNOWN_JOINT_TYPE),
								jointType_withguide(UNKNOWN_JOINT_TYPE){};
	};

	/*! \struct ConstraintData
    \brief Structure that contains a constraint pair.

	A constraint pair defines the alignment of a datum on one part/assembly to a datum on another part/assembly.
	*/
	struct ConstraintData
	{
		std::vector<ConstraintPair>	constraintPairs;

		std::set<std::string> constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates;
		std::set<std::string> constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs;   // This can be .asms and .prts 
																	// in the metrics file only interested in .prts and thus .asms should be filtered
																	// by the metrics file code.
																	// These constrainedTo component IDs are based on:
																	//    a) constraintPairs, follow .asms to .prts
																	//	  b) inferred from feature tree ProSelections for assembled leaf-assembly subordinates

		//std::set<std::string> constrainedTo_ComponentInstanceIDs_DerivedFromFollowingAssemblyReferences;


		// Only used if this constraint is towards an assembly. It stores the information which part it is exactly constrained to
		// within the other assembly. Having multiple elements in this set is an indication of a modeling mistake
		// This path is relative to the other assembly. This information is currently gathered during ApplyConstrainst
		//std::unordered_set<IntList, ContainerHash<IntList>> constrainedTo_Paths;

		// The instance id of the part inferred from constrainedTo_Paths.
		// This ID is gonna end up as the instance id in the metrics file
		//std::string constrainedTo_InstanceID;

		// Only used if this is an assembly. It stores the information which part within this assembly is constrained to something
		// Having multiple elements in this set is an indication of a modeling mistake
		// This path is relative to this assembly. This information is currently gathered during ApplyConstrainst
		//std::unordered_set<IntList, ContainerHash<IntList>> constrainedFrom_Paths;

		// The instance id of the part inferred from constrainedFrom_Paths
		// This ID is gonna end up as the instance id in the metrics file
		//std::string constrainedFrom_InstanceID;

		bool hasAGuideConstraint() const;
		std::vector<ConstraintPair> getConstraintPairsWithoutGuide() const;

		InputJoint inputJoint; // The joint information read from the input xml file.
		ComputedJointData	computedJointData;
	};

	/*! \struct ConstraintDefinition
    \brief Structure defining constraints.

	Constraints are used to position parts/assemblies.  Constraints are defined via sets of 
	part/assembly datums, which are used to position (mate/align) parts/assemblies.
	*/
	struct ConstraintDefinition
	{
		bool applyAllConstraints;		/**<  Currently not used. Will be used in the future. */
		int  applySingleConstaintID;	/**<  Currently not used. Future use: 0 - first constraint, 1 - second constraint, 2 - third constraint */

		std::vector<ConstraintData> constraints;    /**< List of constraints */

	
		// In the following line doesn't inlcude constraints(3) becauses this create 3 empty elements,
		// which will not work with the MetaLink Code because that code checks for the size and
		// only constrains if the size is 0.
		//ConstraintDefinition() : constraints(3),  applyAllConstraints(true), applySingleConstaintID(0) {}
		ConstraintDefinition() :  applyAllConstraints(true), applySingleConstaintID(0) {}

		bool hasAGuideConstraint() const;
	};

	/*! \struct CADParameter
    \brief Structure defining parameters.

	Parameters are used to change features about a part/assembly.  For example, a parameter named LENGTH might be used to vary the length  of a part. 
	A boolean parameter might be used to keep or remove a part/assembly feature.
	*/
	struct CADParameter
	{
		MultiFormatString		name;   /**<  Parameter name. */
		e_CADParameterType		type;   /**<  CAD_FLOAT, CAD_INTEGER, CAD_BOOLEAN */ 
		MultiFormatString		value;  /**<   Parameter value (e.g 1.4, 34...) */

		CADParameter() : name(CAD_NAME_SIZE - 1), value(CAD_LINE_SIZE-1){}

		// Only compares if the names are equal
		bool operator==(const CADParameter &in_Parameter ) const;
	};

	std::ostream& operator<<(std::ostream& output, const CADParameter &in_CADParameter); 

	struct AnalysisTempMaterial
	{
		MultiFormatString		name;
		double					poissonsRatio;
		std::string				tokenPoissonsRatio;
		AnalysisTempMaterial() : name(CAD_NAME_SIZE - 1), poissonsRatio(0.0), tokenPoissonsRatio("") {}
	};


	struct ScalarsDistanceXyx
	{
		e_CADUnitsDistance units;
		double	x;
		double	y;
		double	z;
		
		e_AnalysisConstraintFixedFree x_FixedFree;
		e_AnalysisConstraintFixedFree y_FixedFree;
		e_AnalysisConstraintFixedFree z_FixedFree;

		ScalarsDistanceXyx(): units(CAD_UNITS_MM), x(0.0),y(0.0), z(0.0), 
													x_FixedFree(ANALYSIS_CONSTRAINT_SCALAR), y_FixedFree(ANALYSIS_CONSTRAINT_SCALAR), z_FixedFree(ANALYSIS_CONSTRAINT_SCALAR){};
	};

	struct ScalarsDirectionXyx
	{
		double	x;
		double	y;
		double	z;

		ScalarsDirectionXyx():  x(0),y(0), z(0) {};
	};


	struct ScalarsRotationXyx
	{
		e_CADUnitsAngle units;

		// http://mathworld.wolfram.com/EulerAngles.html
		double	alpha;
		double	beta;
		double	gamma;
		
		e_AnalysisConstraintFixedFree alpha_FixedFree;
		e_AnalysisConstraintFixedFree beta_FixedFree;
		e_AnalysisConstraintFixedFree gamma_FixedFree;

		ScalarsRotationXyx(): units(CAD_UNITS_RADIAN), alpha(0.0),beta(0.0), gamma(0.0), 
													alpha_FixedFree(ANALYSIS_CONSTRAINT_SCALAR), beta_FixedFree(ANALYSIS_CONSTRAINT_SCALAR), gamma_FixedFree(ANALYSIS_CONSTRAINT_SCALAR){};
	};


	struct ScalarsForceXyx
	{
		e_CADUnitsForce units;
		double	x;
		double	y;
		double	z;

		ScalarsForceXyx(): units(CAD_UNITS_N), x(0.0),y(0.0), z(0.0) {};
	};

	struct ScalarsMomentXyx
	{
		e_CADUnitsMoment units;
		double	x;
		double	y;
		double	z;

		ScalarsMomentXyx(): units(CAD_UNITS_N_MM), x(0.0),y(0.0), z(0.0) {};
	};

	struct ScalarPressureValue
	{
		e_CADUnitsPressure	units;
		double		value;

		ScalarPressureValue(): units(CAD_UNITS_PA), value(0) {};
	};

	struct ScalarAccelerationAndXyzDirection
	{
		e_CADUnitsAcceleration	units;
		//double		magnitude;
		ScalarsDirectionXyx	direction;

		ScalarAccelerationAndXyzDirection(): units(CAD_UNITS_MM_PER_SEC_SQ) {};
	};

	/*
	struct ScalarAngleValue
	{
		e_CADUnitsAngle	units;
		double		value;

		ScalarAngleValue(): units(CAD_UNITS_RADIAN), value(0) {};
	};
   */

/*
	struct BoolsXyx
	{
		bool	x;
		bool	y;
		bool	z;

		BoolsXyx(): x(false),y(false), z(false) {};
		BoolsXyx( bool in_x, bool in_y, bool in_z): x(in_x),y(in_y), z(in_z) {};
	};
*/

	struct AnalysisDisplacement
	{
		//BoolsXyx	translation_Fixed;
		ScalarsDistanceXyx	translation;

		
		//BoolsXyx	rotation_Fixed;
		ScalarsRotationXyx	rotation;

		AnalysisDisplacement()//: translation_Fixed(true, true,true),
								/*rotation_Fixed(true, true, true) */  {};
	};

	struct AnalysisPin
	{
		e_AnalysisConstraintFixedFree axialRotationConstraint;  // true if fixed, false if free
		e_AnalysisConstraintFixedFree axialDisplacementConstraint;

		AnalysisPin(): axialRotationConstraint(ANALYSIS_CONSTRAINT_FIXED), axialDisplacementConstraint(ANALYSIS_CONSTRAINT_FIXED) {};
	};

	struct CADFeature
	{
		std::string			componentID;
		MultiFormatString	datumName;    
		CADFeature() : datumName( CAD_NAME_SIZE - 1 ){}
	};


	struct AnalysisGeometryFeature
	{	
		std::string					featureID;
		e_CADGeometryType			geometryType;			//  CAD_GEOMETRY_POLYGON, CAD_GEOMETRY_CIRCLE, CAD_GEOMETRY_CONCENTRIC_CIRCLES,
															//	CAD_GEOMETRY_CYLINDER_SURFACE, CAD_GEOMETRY_CYLINDER_SURFACE, CAD_GEOMETRY_SPHERE_SURFACE
															//  CAD_GEOMETRY_SPHERE_SURFACE, CAD_GEOMETRY_EXTRUSION, CAD_GEOMETRY_EXTRUSION_SURFACE,
															//	CAD_GEOMETRY_NONE
		std::list<CADFeature>		features;            
		e_FeatureInterfaceType		featureInterfaceType;	// CAD_DATUM 
		e_CADAnalysisFeatureGeometryType	analysisFeatureGeometryType;	// POINT, maybe other type later. for now points define polygons, lines, and spheres

		bool							primaryGeometryQualifierDefined;
		bool							secondaryGeometryQualifierDefined;
		
		e_CADPrimaryGeometryQualifier   primaryGeometryQualifier;
		e_CADSecondaryGeometryQualifier secondaryGeometryQualifier;
		
		//bool							geometrySetOperationDefined;
		//e_GeometrySetOperation			geometrySetOperation;

		AnalysisGeometryFeature(): primaryGeometryQualifierDefined(false), secondaryGeometryQualifierDefined(false), geometryType(CAD_GEOMETRY_NONE) {};
	};


	struct AnalysisGeometrySetOperation
	{
		e_GeometrySetOperation		setOperation;
		std::vector<std::string>	featureID;	
		//std::string				feature_1_ID;
		//std::string				feature_2_ID;
	
		//e_CADPrimaryGeometryQualifier	 feature_1_PrimaryGeometryQualifier;
		//e_CADPrimaryGeometryQualifier	 feature_2_PrimaryGeometryQualifier;
		
		//e_CADSecondaryGeometryQualifier  feature_1_SecondaryGeometryQualifier;
		//e_CADSecondaryGeometryQualifier  feature_2_SecondaryGeometryQualifier;

	};

	struct AnalysisGeometry
	{	
		std::list<AnalysisGeometryFeature>		features;	// actual datum names in the Creo model

		// GeometryPerEntireComponent_componentInstanceIDs applies to thermal HeatGeneration 
		// only. HeatGeneration QVOL type is applied to all elements (e.g. Tetra elements)
		// within a componentInstanceID where the componentInstanceID is for a 
		// part (not an assembly).  There can be more than one part per HeatGeneration value.  
		std::vector<std::string> GeometryPerEntireComponent_ComponentInstanceIDs;

		//e_CADPrimaryGeometryQualifier   primaryGeometryQualifier;
		//e_CADSecondaryGeometryQualifier secondaryGeometryQualifier;

		bool setOperationDefined;
		std::list<AnalysisGeometrySetOperation> setOperations;

		AnalysisGeometry(): setOperationDefined(false){};
	};

	 struct ConvectionBoundary
	 {
		double convectionCoefficient;
		std::string unit;
		//bool	ambientTemperatureDefined;
		//double ambientTemperature;

		//ConvectionBoundary() : convectionCoefficient(0.0), ambientTemperature(0.0), ambientTemperatureDefined(false) {};
		ConvectionBoundary() : convectionCoefficient(0.0) {};
	 };


//	struct AnalysisGeometry
//	{	
//		e_CADGeometryType			geometryType;			// CAD_GEOMETRY_POLYGON, CAD_GEOMETRY_CYLINDER, CAD_GEOMETRY_SPHERE
//		std::string					componentID;
//		e_FeatureInterfaceType		featureInterfaceType;	// CAD_DATUM 
//		e_CADAnalysisFeatureGeometryType	featureGeometryType;	// POINT, maybe other type later. for now points define polygons, lines, and spheres
//		std::list<std::string>		features;				// actual datum names in the Creo model
//	};

	struct AnalysisConstraint
	{	
		AnalysisGeometry		geometry;

		bool					analysisDisplacementDefined;
		AnalysisDisplacement    displacement;

		bool					analysisPinDefined;
		AnalysisPin				pin;

		bool					analysisBallDefined;
		// for a ball constraint, rotation is always free (never fixed)

		bool					convectionBoundaryDefined;
		ConvectionBoundary		convectionBoundary;           // CONV, Specifies a free convection boundary condition for heat transfer analysis.


		AnalysisConstraint() :	analysisDisplacementDefined(false), 
								analysisPinDefined(false), 
								analysisBallDefined(false), 
								convectionBoundaryDefined(false) {};

				
	};


	struct Force
	{
		ScalarsForceXyx	force;
		ScalarsMomentXyx	moment;
	};

	struct HeatFlux
	{
		double		value;
		std::string unit;
		HeatFlux() : value(0.0){};
	};

	struct HeatGeneration
	{
		double		value;
		std::string unit;
		HeatGeneration() : value(0.0){};
	};

	struct Temperature
	{
		double value;
		std::string unit;
		Temperature() : value(0.0) {};
	};


	struct AnalysisLoad
	{	
		//e_AnalysisLoadType					type; // ANALYSIS_LOAD_FORCE, ANALYSIS_LOAD_PRESSURE, ANALYSIS_LOAD_BEARING...
		AnalysisGeometry					geometry;

		bool								forceDefined;
		Force								force;	

		bool								pressureDefined;
		ScalarPressureValue					pressure;

		bool								accelerationDefined;
		ScalarAccelerationAndXyzDirection	acceleration;

		bool								heatFluxDefined; // QBDY3, Defines a uniform heat flux load for a boundary surface.
		HeatFlux							heatFlux;

		bool								heatGenerationDefined; // QBDY3, Defines a uniform heat flux load for a boundary surface.
		HeatGeneration						heatGeneration;
	
		bool								gridPointTemperatureDefined;
		Temperature							gridPointTemperature;				// TEMP,  Defines temperature at grid points for determination of thermal loading.

		bool								ambientTemperatureDefined;
		Temperature							ambientTemperature;

		// If geometry.features.size == 0 and gridPointInitialTemperatureDefined, 
		//		then 
		//			temperature applies as an initial temperature to all grid points that do not have an explicit gridPointTemperature settings.  
		//		else
		//			temperature applies to the grid points identified by the geometry.features
		bool								gridPointInitialTemperatureDefined; //	Defines a temperature (starting temperature) value for all grid points 
		Temperature							gridPointInitialTemperature;		//	of the structural model that have not been given
																				//	a temperature on a TEMP entry.

		AnalysisLoad() : forceDefined(false), 
						 pressureDefined(false), 
						 accelerationDefined(false), 
						 heatFluxDefined(false), 
						 heatGenerationDefined(false),
						 gridPointTemperatureDefined(false),
						 gridPointInitialTemperatureDefined(false),
						 ambientTemperatureDefined(false){};
	};


	struct AnalysisPartInterface
	{
		e_AnalysisPartInterfaceType	type;
		std::string					componentID_1;
		std::string					componentID_2;
		AnalysisPartInterface() : type(ANALYSIS_PART_INTERFACE_BONDED) {}; // by default assume BONDED
	};


	struct AnalysisSolver
	{
		//pro_fem_solver_type			type;
		e_CADAnalysisSolverType			type;
		e_AnalysisSolutionType			analysisSolutionType; // ANALYSIS_DECK_BASED, or ANALYSIS_MODEL_BASED, 
		//pro_fem_mesh_type				meshType;
		e_CADAnalysisMeshType			meshType;
		//pro_fem_shell_mesh_type		shellElementType;
		e_CADAnalysisShellElementType	shellElementType;
		//pro_fem_elem_shape_type		elementShapeType;
		e_CADAnalysisElementShapeType	elementShapeType;
	};

	struct CADAnalysisComponentMetrics
	{
		std::string type;
		std::string ID;

		CADAnalysisComponentMetrics( const std::string &in_Type, const std::string &in_ID): type(in_Type), ID(in_ID) {}; 
	};

	struct CADAnalysisComponent
	{
		std::string		componentID;
		bool			infiniteCycle;

		std::list<CADAnalysisComponentMetrics> 	cadAnalysisComponentMetrics;

		CADAnalysisComponent(): infiniteCycle(true) {}
	};

	

	struct CADComputation
	{
		e_ComputationType		computationType;		// COMPUTATION_BOUNDING_BOX, COMPUTATION_CG, COMPUTATION_POINT, COMPUTATION_MASS, COMPUTATION_INTERFERENCE_COUNT, 
														// COMPUTATION_COEFFICIENT_OF_DRAG, COMPUTATION_PLANE, COMPUTATION_FEA
		e_ComputationSubType	computationSubType;		// COMPUTATION_SUBTYPE_GROUND, COMPUTATION_SUBTYPE_NONE
		e_ComputationDimension	computationDimension;	// COMPUTATION_X_COORDINATE, COMPUTATION_Y_COORDINATE, COMPUTATION_Z_COORDINATE, COMPUTATION_VECTOR, COMPUTATION_SCALAR 
		std::string				componentID;
		std::string				metricID;
		std::string				metricName;
		MultiFormatString		datumName;   
		//e_ComputationProcessing computationProcessing;

		CADComputation(const std::string &in_ComponentID ):computationSubType(COMPUTATION_SUBTYPE_NONE), componentID(in_ComponentID), datumName(CAD_NAME_SIZE - 1){};
		
	};

	std::ostream& operator<<(std::ostream& output, const CADComputation &in_CADComputation );

	struct CADComputation_WithParentData
	{
		CADComputation	cADComputation;
		std::string		configurationID;
		std::string		topAssemblyComponentInstanceID;
		CADComputation_WithParentData(const std::string &in_ComponentID ):cADComputation(in_ComponentID){}
	};

	struct AnalysisFEA
	{
		std::string		ID;
		e_AnalysisType	type; // ANALYSIS_STRUCTURAL, ANALYSIS_BUCKLING, ANALYSIS_MODAL, ANALYSIS_THERMAL, ANALYSIS_FATIGUE
		std::list<CADComputation>		metrics;   // these are the components for which anlaysis results will be returned.

		std::list<AnalysisConstraint>		analysisConstraints;
		std::list<AnalysisLoad>				analysisLoads;
		std::list<AnalysisPartInterface>	analysisPartInterfaces;
		std::list<AnalysisSolver>			analysisSolvers;
		//std::list<CADAnalysisComponent>		cADAnalysisComponents;  // these are the components for which anlaysis results will be returned.

		AnalysisFEA() : type(ANALYSIS_STRUCTURAL){}
	};

	struct Shotline
	{
		std::string			name;
		std::string			ballisticThreatRef;
		std::string			datumPoint_ComponentID;
		MultiFormatString   datumPoint;
		double				azimuth;
		double				elevation;
		Shotline() : datumPoint(CAD_NAME_SIZE - 1), azimuth(0.0), elevation(0.0){}
	};


	struct AnalysisBallistic
	{
		std::list<e_ReferencePlane>		referencePlanes;  //  PLANE_WATERLINE, PLANE_GROUND, or  PLANE_NONE
		std::list<CADComputation>		metrics;

		std::list<Shotline>				shotlines;  // this could be empty, for that case predefined shotlines would be used.
	};

	struct AnalysisBlast
	{
		std::list<e_ReferencePlane>		referencePlanes;  //  PLANE_WATERLINE, PLANE_GROUND, or  PLANE_NONE
		std::list<CADComputation>		metrics;
	};

	struct CADHydrostaticParameters
	{
		double		pitchAngleStart;		// Degrees
		double		pitchAngleEnd;			// Degrees
		double		pitchAngleStepSize;		// Degrees

		double		rollAngleStart;			// Degrees
		double		rollAngleEnd;			// Degrees
		double		rollAngleStepSize;		// Degrees

		std::string	fluidMaterial;			// should be a material in the material library.  For now, would be a type of water (sale/fresh).
		double		fluidTemperature;		// Celsius only

		CADHydrostaticParameters() :	pitchAngleStart(0.0),		// Degrees
										pitchAngleEnd(0.0),			// Degrees
										pitchAngleStepSize(0.0),	// Degrees

										rollAngleStart(0.0),		// Degrees
										rollAngleEnd(0.0),			// Degrees
										rollAngleStepSize(0.0),		// Degrees
							
										fluidTemperature(0.0)
		{}
	};

	std::ostream& operator<<(std::ostream& output, const CADHydrostaticParameters &in_CADHydrostaticParameters);

	struct AnalysisCFD
	{ 
		// Only will support one reference plane 
		e_ReferencePlane			referencePlane;  //  PLANE_WATERLINE only waterline supported    (PLANE_GROUND, or  PLANE_NONE not supported)

		bool						cADHydrostaticParameters_exist;
		CADHydrostaticParameters	cADHydrostaticParameters;
		std::list<CADComputation>		metrics;

		AnalysisCFD(): referencePlane(PLANE_NONE), cADHydrostaticParameters_exist(false){};
	};

	struct CADAnalyses
	{
		std::list<AnalysisFEA> analysesFEA;
		std::list<AnalysisBallistic> analysesBallistic;
		std::list<AnalysisBlast> analysesBlast;
		std::list<AnalysisCFD> analysesCFD;

		// 1/26/2015, Indicating an interference analysis "bool interference;" been deprecated
		// Now an inference analyis is specified via
		//    std::list<CADComputation>
		//bool interference;
		//bool cFDAnalysis;   // zzzz is this needed?
		//CADAnalyses(): interference(false) {};

	};

	//struct BoundingBoxMetric_SingleComponent
	//{
	//	std::string boundingBoxMetricID;
	//	e_ComputationDimension boundingBoxDimension;

	//	BoundingBoxMetric_SingleComponent( const string				&in_BoundingBoxMetricID, 
	//									   e_ComputationDimension	in_BoundingBoxDimension ) :
	//									   boundingBoxMetricID(in_BoundingBoxMetricID),
	//										   boundingBoxDimension(in_BoundingBoxDimension) {};
	//};

	enum e_ComputationProcessing
	{
		COMPUTATION_PERFORMED_BY_THIS_PROGRAM,
		COMPUTATION_PERFORMED_BY_EXTERNAL_PROGRAM
	};


	/*! \struct CADComponentData
    \brief Structure containing the bulk of the data about a part/assembly.

    This structure contains meta data about a component, the list of children, the set of dependences, and 
	the address/handle of the Creo part/assembly.  
	*/
	struct UnassembledComponent
	{
		MultiFormatString			name;					/**<  Part/Assembly name without the suffix. */
		//ProMdlType				modelType;	
		e_CADMdlType				modelType;	
		MultiFormatString			geometryRepresentation;
		std::string					componentID;
		/**<  PRO_MDL_PART, PRO_MDL_ASSEMBLY */
		UnassembledComponent(	const std::string			&in_Name, 
								//ProMdlType					in_ModelType, 
								e_CADMdlType					in_ModelType, 
								const std::string			&in_GeometryRepresentation, 
								const std::string			&in_ComponentID) : 
										 name ( in_Name, CAD_NAME_SIZE - 1 ),
										 modelType(in_ModelType),
										 geometryRepresentation(in_GeometryRepresentation),
									     componentID(in_ComponentID) {};
	};

	struct CADDatum
	{
		std::string id;
		std::string displayName;
	};

	struct CADComponentConnector
	{
		std::string parentid; // ID of the parent - the unique id for a connector is a parentid-id pair!
		std::string id;		  // ID of the connector (within a component). This is not a unique id
		std::string displayName;
		std::vector<CADDatum> datums;
	};

	struct BoundingBox
	{
		isis_CADCommon::Point_3D			boundingBox_Point_1;
		isis_CADCommon::Point_3D			boundingBox_Point_2;
		double								Dimensions_xyz[3];
		bool								boundingBox_Defined;

		BoundingBox();  // Intialze boundingBox_Point_1,  boundingBox_Point_2, and Dimensions_xyz to 0.0 and boundingBox_Defined to false
	};


	struct MassProperties
	{
		double   volume;
		double   surfaceArea;
		double   density;
		double   mass;

		// Interia tensor in the coordinate systems of the part/assembly
		double   coordSysInertiaTensor[3][3];

		// Center-of-gravity in the coordinate systems of the part/assembly
		double   centerOfGravity[3];
		double   cGInertiaTensor[3][3];
		double	 principalMomentsOfInertia[3];
		double	 principalAxis_RotationMatrix[3][3];

		// massProperties_RetrievalInvoked means that an attempt was made to read the mass properties from the CAD model.
		// The CAD model may not have mass properties set, and thus for that case massProperties_Defined would be set to false
		// and massProperties_RetrievalInvoked would be set to true because an attempt was made to retrieve the mass properties.
		bool	massProperties_RetrievalInvoked;
		// If the function to complete this structure has been invoked, then massPropertiesDefined would be set to true.
		// Even though the function to complete the structure was invoked, it could be that none of the values were completed, 
		// and in that case the bools for the particular values would be false.
		bool	massProperties_Defined;

		bool	volume_Defined;
		bool	surfaceArea_Defined;
		bool	density_Defined;
		bool	mass_Defined;

		bool	coordSysInertiaTensor_Defined;
		bool	centerOfGravity_Defined;
		bool	cGInertiaTensor_Defined;
		bool	principalMomentsOfInertia_Defined;
		bool	principalAxis_RotationMatrix_Defined;

		MassProperties();
	};

	enum e_Data_Initial_Source
	{
		INITIAL_SOURCE_INPUT_XML_FILE,
		INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	class CADSpecialInstruction
	{
		private:
			std::set<e_CADSpecialInstruction> specialInstructions;
		public:
			bool operator == (e_CADSpecialInstruction in_specialInstruction) const;
			bool operator != (e_CADSpecialInstruction in_specialInstruction) const;
			std::string		ToCommaDelimitedString() const;
			void			FromCommaDelimitedString(const std::string &in_String);
	};

	std::ostream& operator<<(std::ostream& output,const CADSpecialInstruction &in_CADSpecialInstruction );


	struct CADComponentData
	{
		std::string					componentID;
		int							metricID;
		std::string					parentComponentID;
		MultiFormatString			name;					/**<  Part/Assembly name without the suffix. */
		std::string					displayName;			/**<  Name to display in Creo */
		std::string					materialID_FromCyPhy;
		std::string					materialID_FromCreoPart;
		std::string					avmComponentId;			// If applicable, only in component edit mode
		std::string					cyphyInstanceId;		// If applicable, only in design mode
		//ProMdlType					modelType;				/**<  PRO_MDL_ASSEMBLY, PRO_MDL_PART */
		e_CADMdlType					modelType;				/**<  PRO_MDL_ASSEMBLY, PRO_MDL_PART */		

		CADSpecialInstruction		specialInstruction;		/**< CAD_SPECIAL_INSTRUCTION_NONE, CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT, CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINTS */
		// either p_model or modelHandle is not necessary, should review thier uses and eliminate one.
		//ProMdl						*p_model;				/**<  Pointer to the model. */
		//ProMdl						*cADModel_ptr_ptr;				/**<  Pointer to the model. */
		void						**cADModel_ptr_ptr;				/**<  Pointer to the model. */
		
		//ProSolid					modelHandle;			/**< typedef struct sld_part* ProSolid;  */
		//ProSolid					cADModel_hdl;			/**< typedef struct sld_part* ProSolid;  */
		void						*cADModel_hdl;			/**< typedef struct sld_part* ProSolid;  */

		//ProAsmcomp					assembledFeature;		/**< typedef struct pro_model_item  ProAsmcomp; */
		CADAssembledFeature			assembledFeature;


		// ProMatrix				initialPosition;		/** initial position, as stored manually in file */
		MatrixD4x4					initialPosition;		/** initial position, as stored manually in file */
		std::list<int>				componentPaths;			/**< This is the path from the current assembly (i.e. active assembly) to the part/assembly defined by this structure. */
		std::list<std::string>		children;				/**<  Component IDs of the children. */
		std::set<std::string>		dependsOn;				/**< Depends on (i.e. constrained to) ComponentIDs. */
		std::vector<CADComponentConnector> connectors;      // Connector list (received from Cyphy when the design is created)

		ConstraintDefinition		constraintDef;			/**< Definition of the constraints that position the part/assembly. */

		bool						parametricParametersPresent; /**< True if parametric parameters apply to this part/assembly. */
		std::list<CADParameter>		parametricParameters;		/**< List of parametric parameters. */

		bool						analysisTempMaterialDefined;
		AnalysisTempMaterial		analysisTempMaterialDefinition;

		bool						cyPhyComponent;

		MultiFormatString			geometryRepresentation;  // Simplified representations in Creo, e.g. Featured_Rep, Defeatured_rep
															 // if the Creo Model does not have this rep, then the active rep in the Creo
															 // model would be used.
		BoundingBox					boundingBox;

		MassProperties				massProperties;

		e_Data_Initial_Source		dataInitialSource;

		int							partOrdinal;           // This is the order parts are added to the assembly.  First part ordinal would be 1
		int							partAssemblyOrdinal;   // This is the order parts/assemblies are added to the assembly.  
														   // First part/assembly ordinal would be 1, second part/assembly ordinal would be 2

		int							addedToAssemblyOrdinal;  // This is the ordinal position for models added into the assembly.
															 //	The top-assembly would have a value of 1.
															 // The next .prt/.asm added to the top-assembly one have a value 2 ...
															 // Subordinates to CADAssembly.xml leaf assemblies would have a value of LONG_MAX.
															 // This field is only used for tracking CADAssembly.xml models added to the
															 // assembly.  Leaf-assembly subordinates are not tracked and thus the LONG_MAX
															 // setting.

		CADComponentData() :dataInitialSource( INITIAL_SOURCE_INPUT_XML_FILE),
							name(CAD_NAME_SIZE - 1), 
							cADModel_ptr_ptr(0), cADModel_hdl(0), 
							parametricParametersPresent(false),  
							//specialInstruction(CAD_SPECIAL_INSTRUCTION_NONE), 
							analysisTempMaterialDefined(false), 
							cyPhyComponent(true),
							geometryRepresentation(CAD_NAME_SIZE - 1),
							addedToAssemblyOrdinal(LONG_MAX),
							partOrdinal(-1),
							partAssemblyOrdinal(-1)
		{
			assembledFeature.id = 0;
			//assembledFeature.type = PRO_TYPE_INVALID;
			assembledFeature.type = CAD_FEATURE_GEOMETRY_TYPE_INVALID;
			assembledFeature.owner = 0;
		}

	};


	/*! \struct CADComponentData
    \brief Structure containing information about a top-level assembly.

	The structure contains the top-level assembly component ID and the configuration ID.  
	Both of these IDs originate in GME.  The component ID is the ID that uniquely identifies a 
	GME component. The configuration ID is used to link the metrics information created by this 
	program back to a configuration in GME.
	*/
	struct TopLevelAssemblyData
	{
		std::string					assemblyComponentID;
		std::string					configurationID;
		CADAnalyses					analysesCAD;
		std::list<CADComputation>	assemblyMetrics;  // bounding box, cg, ... just dependant on the assembly

		TopLevelAssemblyData() 
		{
			assemblyComponentID = "";
			configurationID = "";
		}

		TopLevelAssemblyData( const std::string &in_AssemblyComponentID, const std::string &in_ConfigurationID ) 
		{
			assemblyComponentID = in_AssemblyComponentID;
			configurationID = in_ConfigurationID;
		}
	
	};


	// This structure contains the allowables.  The values are based on
	// the values in AnalysisMaterialProperties; howerver, they have been 
	// adjusted based on the number of cycles.  Units will always be in MPa.
	struct AnalysisMaterialProperties_Allowables
	{	
		e_MaterialType materialType;
		std::string		materialName;

		double		tensileStrength;	// MPa
		double		bearingStrength;	// MPa
		double		shearStrength;		// MPa

		long 		numberOfCycles;		// Integer

		AnalysisMaterialProperties_Allowables() :	tensileStrength(0.0),
													bearingStrength(0.0),
													shearStrength(0.0),
													numberOfCycles(0) {}

	};



	// Standardized on SI(mm)(Creo millimeter Newton Second) units as follows:
	//		Force	N
	//		Stress	MPa
	//		Length	mm
	//		Time	s
	struct AnalysisMaterialProperties
	{	
		e_MaterialType materialType;
		std::string		materialName;

		bool		modulusOfElasticityDefined;	
		bool		poissonsRatioDefined;			

		bool		tensileYieldStrengthDefined;
		bool		tensileUltimateStrengthDefined;
		bool		shearStrengthDefined;			
		bool		bearingYieldStrengthDefined;	
		bool		bearingUltimateStrengthDefined;	

		bool		fatigueStrengthDefined;	
		bool		fatigueNumberOfCyclesDefined;
		bool		denstiyDefined;

		bool		heatCapacityDefined;
		bool		thermalConductivityDefined;

		double		modulusOfElasticity;		// MPa
		double		poissonsRatio;				// Unitless

		double		tensileYieldStrength;		// MPa
		double		tensileUltimateStrength;	// MPa
		double		shearStrength;				// MPa
		double		bearingYieldStrength;		// MPa
		double		bearingUltimateStrength;	// MPa

		double		fatigueStrength;			// MPa
		long 		fatigueNumberOfCycles;		// Integer

		double		density;			// kg/m3

		double		heatCapacity;
		double		thermalConductivity;


		e_CADUnitsPressure		modulusOfElasticityUnit;			

		e_CADUnitsPressure		tensileYieldStrengthUnit;
		e_CADUnitsPressure		tensileUltimateStrengthUnit;
		e_CADUnitsPressure		shearStrengthUnit;			
		e_CADUnitsPressure		bearingYieldStrengthUnit;	
		e_CADUnitsPressure		bearingUltimateStrengthUnit;	

		e_CADUnitsPressure		fatigueStrengthUnit;	
		e_CADUnitsDensity		densityUnit;

		e_CADUnitsHeatCapacity			heatCapacityUnit;
		e_CADUnitsThermalConductivity	thermalConductivityUnit;

		AnalysisMaterialProperties():	modulusOfElasticityDefined(false),
										poissonsRatioDefined(false),
										tensileYieldStrengthDefined(false),
										tensileUltimateStrengthDefined(false),
										shearStrengthDefined(false),  
										bearingYieldStrengthDefined(false), 
										bearingUltimateStrengthDefined(false), 
										fatigueStrengthDefined(false),
										fatigueNumberOfCyclesDefined(false),
										heatCapacityDefined(false),
										thermalConductivityDefined(false),
										denstiyDefined(false),			
										modulusOfElasticity(0.0),		
										poissonsRatio(0.0),				
										tensileYieldStrength(0.0),		
										tensileUltimateStrength(0.0),	
										shearStrength(0.0),				
										bearingYieldStrength(0.0),		
										bearingUltimateStrength(0.0),	
										fatigueStrength(0.0),			
		 								fatigueNumberOfCycles(0),		
										density(0.0),	
										heatCapacity(0.0),
										thermalConductivity(0.0),
										modulusOfElasticityUnit(CAD_UNITS_MPA),			
										tensileYieldStrengthUnit(CAD_UNITS_MPA),
										tensileUltimateStrengthUnit(CAD_UNITS_MPA),
										shearStrengthUnit(CAD_UNITS_MPA),			
										bearingYieldStrengthUnit(CAD_UNITS_MPA),	
										bearingUltimateStrengthUnit(CAD_UNITS_MPA),	
										fatigueStrengthUnit(CAD_UNITS_MPA),	
										densityUnit(CAD_UNITS_KG_PER_MM_CUBED),
										heatCapacityUnit(CAD_UNITS_J_PER_KG_K),
										thermalConductivityUnit(CAD_UNITS_W_PER_MM_K) {}	
	};

	std::ostream& operator<<(std::ostream& output, const AnalysisMaterialProperties &in_AnalysisMaterialProperties ); 

	struct MaterialMetrics
	{
		double value;
		std::string units;

		MaterialMetrics( double in_Value,  const std::string &in_Units ): value(in_Value), units(in_Units) {};
	};

	struct Material
	{
		e_MaterialType	materialType;
		std::string		materialName;
		std::map<std::string, MaterialMetrics>		materialMetrics_map;  // Key property name (e.g. TensileYieldStrength)
		AnalysisMaterialProperties	analysisMaterialProperties;
		//AnalysisMaterialProperties_Allowables analysisMaterialProperties_Allowables;

		Material(){};
		Material( const std::string &in_MaterialName, e_MaterialType in_MaterialType) :materialName(in_MaterialName), materialType(in_MaterialType) {};
	};

	struct DataExchangeSpecification
	{
		e_DataExchangeFormat  dataExchangeFormat;  // DATA_EXCHANGE_FORMAT_STEP, DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY
		e_DataExchangeVersion dataExchangeVersion; // AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES... STEREOLITHOGRAPHY_ASCII, STEREOLITHOGRAPHY_BINARY

		DataExchangeSpecification(	e_DataExchangeFormat in_DataExchangeFormat, 
									e_DataExchangeVersion in_DataExchangeVersion ) : 
										dataExchangeFormat(in_DataExchangeFormat), 
										dataExchangeVersion( in_DataExchangeVersion ) {};
	};

	/*! \struct CADAssemblies
    \brief Structure containing the top-level assemblies.

    This structure is the top-level container for assemblies.  If the input XML contained multiple assemblies, 
	then cadComponentIDs list would contain the top level component IDs for each assembly.
	*/
	struct ProcessingInstruction
	{
		e_ProcessingInstruction_Primary		primary;  // If processing instruction exists in CADAssembly.xml, then primary must exist
		e_ProcessingInstruction_Secondary	secondary;

		ProcessingInstruction() : secondary(PROCESSING_INSTRUCTION_SECONDARY_NONE){};
	};


	struct CADAssemblies
	{
		std::map<std::string, Material>		 materials;  // Key Material ID
		std::list<TopLevelAssemblyData>		 topLevelAssemblies; 
		std::list<DataExchangeSpecification> DataExchangeSpecifications;
		std::vector<ProcessingInstruction> processingInstructions;
		std::list<e_DataExchangeSpecialInstruction> dataExchangeSpecialInstruction;
		std::list<UnassembledComponent>		 unassembledComponents;
	};

	std::ostream& operator<<(std::ostream& output, const CADComponentData &in_AssembledInfo ); 

	std::string AmalgamateModelNameWithSuffix ( const std::string &in_Name, e_CADMdlType in_pro_model_type );


	struct ComponentEditInfo
	{
		std::string avmId;
		//ProMdl mdl;
		void	*cADModel_ptr;
		std::string resourceId;
		std::vector<CADComponentConnector> connectors; // List of connectors for this component
	};

	enum MetaLinkMode
	{
		UNDEFINEDMODE,
		COMPONENTEDIT,
		DESIGNEDIT,
	};

	struct CADPoint
	{
		double x;
		double y;
		double z;
	};

	enum CADCreateAssemblyError_Severity
	{
		CADCreateAssemblyError_Severity_Warning,
		CADCreateAssemblyError_Severity_Error,
		CADCreateAssemblyError_Severity_Critical, // These are the non-recoverable errors
	};
	
	struct CADCreateAssemblyError
	{
		CADCreateAssemblyError(const std::string &txt, CADCreateAssemblyError_Severity severity) : Text(txt), Severity(severity)
		{
		}
		std::string Text;
		CADCreateAssemblyError_Severity Severity;
	};

}  // END namespace isis

#endif // COMMON_STRUCTURES_H
