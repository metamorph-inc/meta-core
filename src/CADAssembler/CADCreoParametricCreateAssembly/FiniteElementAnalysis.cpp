#include "stdafx.h"
#include <Nastran.h>
#include <GraphicsFunctions.h>
#include <MaterialProperties.h>
#include "UdmBase.h"
#include <CADPostProcessingParameters.h>
#include <CADAnalysisMetaData.h>
#include <ToolKitPassThroughFunctions.h>

#include "MiscellaneousFunctions.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <sstream>

namespace isis
{
	const std::string NASTRAN_COMMENT_TAG = "$";
	const std::string NASTRAN_META_COMMENT_TAG = "$META  ";

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	void ValidateAnalysisGeometry(  const std::string &in_ConfigurationIDSentence,
								const std::string &in_ConstraintOrLoad,
								const AnalysisGeometry &in_AnalysisGeometry ) throw (isis::application_exception)
{
	std::string TempError = in_ConfigurationIDSentence;

	if (  (in_AnalysisGeometry.geometryType == CAD_GEOMETRY_POLYGON) && (in_AnalysisGeometry.features.size()) < 3 )
	{
		TempError += "For a polygon " + in_ConstraintOrLoad + ", there must at least three features (i.e. Datum Points)";
		throw isis::application_exception(TempError.c_str());	
	}

	if (  (in_AnalysisGeometry.geometryType == CAD_GEOMETRY_CYLINDER) && (in_AnalysisGeometry.features.size()) != 2 )
	{
		TempError += "For a pin " + in_ConstraintOrLoad + ", there must two features (i.e. Datum Points)";
		throw isis::application_exception(TempError.c_str());	
	}

	if (  (in_AnalysisGeometry.geometryType == CAD_GEOMETRY_SPHERE) && (in_AnalysisGeometry.features.size()) != 2 )
	{
		TempError += "For a ball " + in_ConstraintOrLoad + ", there must two features (i.e. Datum Points)";
		throw isis::application_exception(TempError.c_str());	
	}


}
*/
void ValidateAnalysisGeometry(  const std::string &in_ConfigurationIDSentence,
								const std::string &in_ConstraintOrLoad,
								const AnalysisGeometry &in_AnalysisGeometry,
								const std::list<AnalysisSolver>	&analysisSolvers) throw (isis::application_exception)
{
	std::string TempError = in_ConfigurationIDSentence;

	// 11/24/2015 Added support for FACE with deck-based.  modelBasedAbaqusSolver not needed anymore
	//bool modelBasedAbaqusSolver = false;
	//for each ( AnalysisSolver i in analysisSolvers)
	//{
	//	if ( i.analysisSolutionType == ANALYSIS_MODEL_BASED )
	//	{
	//		modelBasedAbaqusSolver = true;
	//		break;
	//	}
	//}

	int numberOfFeatures = in_AnalysisGeometry.features.begin()->features.size();

	if ( numberOfFeatures == 0 )
	{

		if (  (in_AnalysisGeometry.features.begin()->geometryType == CAD_GEOMETRY_POLYGON) && ( numberOfFeatures < 3 ))
		{
			TempError += "For a polygon " + in_ConstraintOrLoad + ", there must at least three features (i.e. Datum Points)";
			throw isis::application_exception(TempError.c_str());	
		}

		if (  (in_AnalysisGeometry.features.begin()->geometryType == CAD_GEOMETRY_CYLINDER) && (numberOfFeatures != 3 ))
		{
			TempError += "For a pin " + in_ConstraintOrLoad + ", there must two features (i.e. Datum Points)";
			throw isis::application_exception(TempError.c_str());	
		}

		if (  (in_AnalysisGeometry.features.begin()->geometryType == CAD_GEOMETRY_SPHERE) && (numberOfFeatures != 2 ))
		{
			TempError += "For a ball " + in_ConstraintOrLoad + ", there must two features (i.e. Datum Points)";
			throw isis::application_exception(TempError.c_str());	
		}
	
		// 11/24/2015 Added support for FACE with deck-based
		//if (  (in_AnalysisGeometry.features.begin()->geometryType == CAD_GEOMETRY_FACE) && ( !modelBasedAbaqusSolver))
		//{
		//	TempError += "For FEA, Feature GeometryType=\"FACE\" the solver type must be Abaqus-Model-Based.  Deck-based Nastran, Abaqus, and Calculix do not support the FACE feature.";
		//	throw isis::application_exception(TempError.c_str());	
		//}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ValidateFEAAnalysisInputs (const std::string	&in_ConfigurationID, const CADAnalyses &in_AnalysesCAD,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map) 
																	throw (isis::application_exception)	
{
	//char tempBuffer[64];
	std::string TempError =  "Configuration ID: " + in_ConfigurationID + " ";

	//  Verify that there is one and only one FEA analysis.  Later, we will support multiple analyses.   
	if ( in_AnalysesCAD.analysesFEA.size() != 1 )
	{
		TempError += "Per assembly, there can be one and only one FEA analysis.  Therefore, there must be one and only <Analyses> <FEA...> xml entries per assembly.  In the future multiple analyses per assembly will be supported.  <FEA...> count: " + in_AnalysesCAD.analysesFEA.size();
		throw isis::application_exception(TempError.c_str());	
	}

	for ( list<AnalysisFEA>::const_iterator i(in_AnalysesCAD.analysesFEA.begin());
		  i != in_AnalysesCAD.analysesFEA.end();
		  ++ i )
	{	

		//////////////////////////////////////////////
		// Verify at least one Metrics was requested
		//////////////////////////////////////////////
		// Verify AnalysisComponents contains at least one components
		if (i->metrics.size() == 0 )
		{
			TempError += "Per FEA analysis, there must be at least one Metric in AnalysisFEA.metrics.";
			throw isis::application_exception(TempError.c_str());	
		}

		//////////////////////////////
		// Constraints
		//////////////////////////////
		// Verify at least one constraint
		if (i->analysisConstraints.size() == 0 && i->type == ANALYSIS_STRUCTURAL )
		{
			TempError += "Per FEA analysis, there must be at least on AnalysisConstraint.";
			throw isis::application_exception(TempError.c_str());	
		}

		for ( std::list<AnalysisConstraint>::const_iterator j(i->analysisConstraints.begin());
			  j != i->analysisConstraints.end();
			  ++j )
		{
			int count = 0;
			if ( j->analysisBallDefined ) ++count;
			if ( j->analysisDisplacementDefined ) ++count;
			if ( j->analysisPinDefined ) ++count;
			if ( j->convectionBoundaryDefined ) ++count;
			if ( count != 1 )
			{
				TempError += "For each AnalysisConstraint, there must be one and only one of Displacement, Ball, Pin, or ConvectionBoundary.";
				throw isis::application_exception(TempError.c_str());	
			}
			
			// If convectionBoundaryDefined  then ambientTemperature must be defined
			if ( j->convectionBoundaryDefined ) 
			{ 
				bool foundAmbientTemp = false;
				for each ( const AnalysisLoad &aload  in i->analysisLoads)
				{
					if ( aload.ambientTemperatureDefined)
					{
						foundAmbientTemp = true;
						break;
					}
				}		
				if ( !foundAmbientTemp )
				{
					TempError += "Thermal convection defined but an ambient temperature was not defined.";
					throw isis::application_exception(TempError.c_str());	
				}
			}

			if ( j->geometry.features.size() == 0 )
			{
				// All constraints must have at least one geometry feature
				std::stringstream errorString;
				errorString <<
					"Function - " << __FUNCTION__ << " FEA constraints must reference at least one geometry feature (e.g. points)" << std::endl <<
					"Constraint Type: ";
				if ( j->analysisBallDefined ) errorString << "Ball";
				else if ( j->analysisDisplacementDefined )  errorString << "Displacement"; 
				else if ( j->analysisPinDefined )  errorString << "Pin"; 
				else if ( j->convectionBoundaryDefined )  errorString << "Convection Boundary"; 
				throw isis::application_exception(errorString.str());	
			}
			else
			{
				// The following function is only valid for the case where j->geometry.features.size() > 0 
				ValidateAnalysisGeometry(TempError, "AnalysisConstraint", j->geometry, i->analysisSolvers );
			}
		}

		//////////////////////////////
		// Loads
		//////////////////////////////
		// Verify at least one load
		if (i->analysisLoads.size() == 0 )
		{
			TempError += "Per FEA analysis, there must be at least on Load.";
			throw isis::application_exception(TempError.c_str());	
		}

		int ambientTemperature_count = 0;
		for ( std::list<AnalysisLoad>::const_iterator j(i->analysisLoads.begin());
			  j != i->analysisLoads.end();
			  ++j )
		{
			int count = 0;
			if ( j->forceDefined ) ++count;
			if ( j->pressureDefined ) ++count;
			if ( j->accelerationDefined ) ++count;
			if ( j->gridPointInitialTemperatureDefined ) ++count;
			if ( j->gridPointTemperatureDefined ) ++count;
			if ( j->heatFluxDefined ) ++count;
			if ( j->heatGenerationDefined ) ++count;
			if ( j->ambientTemperatureDefined )
			{
				++count;
				++ambientTemperature_count;
			}

			if ( count != 1 )
			{
				TempError += "For each Load, there must be one and only one of ForceMoment, Pressure, Acceleration, gridPointInitialTemperature, PointTemperature, HeatFlux, HeatGeneration, or AmbientTemperature.";
				throw isis::application_exception(TempError.c_str());	
			}
			

			if ( ambientTemperature_count > 1 )
			{
				TempError += "More than one AmbientTemperature defined.  Only one AmbientTemperature allowed.";
				throw isis::application_exception(TempError.c_str());	
			}

			if ( j->heatGenerationDefined  )
			{
				if ( j->geometry.features.size() > 0 )
				{
					TempError += "HeatGeneration defined with geometry features. HeatGeneration applies only to parts and not part features.";
					throw isis::application_exception(TempError.c_str());	
				}
				if ( j->geometry.GeometryPerEntireComponent_ComponentInstanceIDs.size() == 0 )
				{
					TempError += "HeatGeneration defined without a component (i.e. part) defined. HeatGeneration applies to a part and thus must have a component defined.";
					throw isis::application_exception(TempError.c_str());	
				}
				for each ( const std::string &compID in j->geometry.GeometryPerEntireComponent_ComponentInstanceIDs )
				{
					std::map<std::string, isis::CADComponentData>::const_iterator  comp_itr = in_CADComponentData_map.find(compID);
					if ( comp_itr == in_CADComponentData_map.end() || comp_itr->second.modelType != PRO_MDL_PART )
					{
						TempError += "HeatGeneration was applied to a component that is not a part. HeatGeneration applies to a part and thus must not have an assembly component reference.";
						throw isis::application_exception(TempError.c_str());	
					}
				}
			}

			// Geometry is not associated with acceleration. For force and pressure ValidateAnalysisGeometry
			if ( !j->accelerationDefined ) continue;

			// if no geometry the temperature applies to all nodes that do not have an explicit gridPointTemperature
			if ( j->gridPointInitialTemperatureDefined && j->geometry.features.size() == 0 ) continue;  
				

			if ( !j->accelerationDefined  && !j->ambientTemperatureDefined )  // No geometry for acceleration and ambient-temperature
			{
				if ( j->geometry.features.size() == 0 )
				{
					// All loads (except acceleration and ambient-temperature)  must have at least one geometry feature
					std::stringstream errorString;
					errorString <<
						"Function - " << __FUNCTION__ << " Except for acceleration and ambient-temperature loads, loads must reference at least one geometry feature (e.g. points)" << std::endl <<
						"Load Type: ";
					if ( j->forceDefined )  errorString << "Force"; 
					else if ( j->pressureDefined )  errorString << "Pressure"; 
					else if ( j->heatFluxDefined )  errorString << "Heat Flux"; 
					else if ( j->heatGenerationDefined )  errorString << "Heat Generation"; 
					else if ( j->gridPointTemperatureDefined )  errorString << "Grid Point Temperature"; 
					throw isis::application_exception(errorString.str());	
				}
				else
				{
					// The following function is only valid for the case where j->geometry.features.size() > 0 
					ValidateAnalysisGeometry(TempError, "Load", j->geometry, i->analysisSolvers );
				}
			}
		}

		//////////////////////////////
		// Solvers
		//////////////////////////////
		// Verify one and only one solver
		if ( i->analysisSolvers.size() != 1 )
		{
			TempError += "Per FEA analysis, there must be on and only one solver specified.";
			throw isis::application_exception(TempError.c_str());	
		}

		for ( std::list<AnalysisSolver>::const_iterator j( i->analysisSolvers.begin());
			  j != i->analysisSolvers.end();
			  ++j )
		{
			if ( (j->type != PRO_FEM_FEAS_ABAQUS &&  j->type != PRO_FEM_FEAS_NASTRAN &&  j->type != PRO_FEM_FEAS_PATRAN ) || j->meshType != PRO_FEM_SOLID_MESH || j->elementShapeType != PRO_FEM_MIDPNT_PARABOLIC_FIXED )
			{
				TempError += " For FEA analysis, the only supported solver settings are Type=\"ABAQUS/NASTRAN/PATRAN_NASTRAN\", MeshType=\"SOLID\", ShellElementType=\"N/A\", and ElementShapeType=\"MIDPOINT_PARABOLIC_FIXED\"";
				throw isis::application_exception(TempError.c_str());		
			}
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void MeshModel (
	ProSolid				in_pro_solid, 
	pro_fem_analysis_type	in_AnalysisType,//  PRO_FEM_ANALYSIS_STRUCTURAL, PRO_FEM_ANALYSIS_MODAL.
											//	PRO_FEM_ANALYSIS_THERMAL, PRO_FEM_ANALYSIS_FLOW

	pro_fem_solver_type		in_SolverType,	//	PRO_FEM_FEAS_NONE, PRO_FEM_FEAS_NEUTRAL, PRO_FEM_FEAS_NASTRAN, 
											//	PRO_FEM_FEAS_ANSYS, PRO_FEM_FEAS_ABAQUS

	pro_fem_mesh_type		in_MeshType,	//  PRO_FEM_FEAS_ANSYS, PRO_FEM_FEAS_NASTRAN,
											//  PRO_FEM_FEAS_ABAQUS

	pro_fem_shell_mesh_type	in_ShellMeshType, //PRO_FEM_SHELL_MESH_TRIANGLE, PRO_FEM_SHELL_MESH_QUADRANGLE 

	pro_fem_elem_shape_type in_ElemShapeType, //PRO_FEM_MIDPNT_LINEAR, PRO_FEM_MIDPNT_PARABOLIC, PRO_FEM_MIDPNT_PARABOLIC_FIXED  

	const std::string	&in_PathAndFileName ) throw (isis::application_exception)
{

	//std::cout << std::endl << "MeshModel, in_pro_solid: " << in_pro_solid;

	ProFemmeshData		femMeshData;

	femMeshData.mesh_type	= in_MeshType;		// PRO_FEM_SOLID_MESH, PRO_FEM_SHELL_MESH, PRO_FEM_MIXED_MESH, 
												// PRO_FEM_QUILT_MESH, PRO_FEM_BOUNDARY_MESH, PRO_FEM_BAR_MESH 
	
	femMeshData.shell_type	= in_ShellMeshType; // This field is ignored for a solid mesh.  
												// PRO_FEM_SHELL_MESH_TRIANGLE   = 0, PRO_FEM_SHELL_MESH_QUADRANGLE

	femMeshData.num_quilts	= 0;
	//femMeshData.pro_quilt_ref_arr = NULL; 

	femMeshData.analysis = in_AnalysisType;		// PRO_FEM_ANALYSIS_STRUCTURAL, PRO_FEM_ANALYSIS_MODAL, 
												// PRO_FEM_ANALYSIS_THERMAL, PRO_FEM_ANALYSIS_FLOW  

	femMeshData.elem_shape = in_ElemShapeType;	// PRO_FEM_MIDPNT_LINEAR, PRO_FEM_MIDPNT_PARABOLIC, 
												// PRO_FEM_MIDPNT_PARABOLIC_FIXED


	femMeshData.solver = in_SolverType;			// PRO_FEM_FEAS_NONE, PRO_FEM_FEAS_NEUTRAL, PRO_FEM_FEAS_NASTRAN, 
												// PRO_FEM_FEAS_ANSYS, PRO_FEM_FEAS_ABAQUS

	ProFemcsysref  femCsyRef;
	femCsyRef.id = PRO_VALUE_UNUSED;
	femMeshData.csys_ref = femCsyRef;   		// if id is PRO_VALUE_UNUSED - not used 

	femMeshData.num_aux_csys = 0;

	femMeshData.aux_csys_ref_arr = NULL;

	//wchar_t  femOutputFileName_wide[PRO_PATH_SIZE ];
	//char	 femOutputFileName[PRO_PATH_SIZE ];
	//strcpy(femOutputFileName, in_PathAndFileName.c_str() );
	//ProStringToWstring(femOutputFileName_wide, femOutputFileName );

	// Must display a model before it can be meshed.
	isis::isis_ProMdlDisplay( (ProMdl)in_pro_solid );

	MultiFormatString femOutputFileName(in_PathAndFileName);

	//std::cout << std::endl <<  " femOutputFileName: " << femOutputFileName;
	//isis::isis_ProFemmeshExport( in_pro_solid, &femMeshData, femOutputFileName_wide );
	isis::isis_ProFemmeshExport( in_pro_solid, &femMeshData, (wchar_t*) (const wchar_t*)femOutputFileName );
}
//end using solid elements ****/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveDatumPointCoordinates( const std::string							&in_AssemblyComponentID,
									const std::string							&in_PartComponentID,
									std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
									const MultiFormatString						&in_DatumName,
									CADPoint									&out_CADPoint) 
																				throw (isis::application_exception)						
{

	//wchar_t  datum_name[PRO_NAME_SIZE ];
	//ProStringToWstring(datum_name, (char *)in_DatumName.c_str() );


	ProModelitem  datum_point;
	isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
		in_PartComponentID, in_CADComponentData_map[in_PartComponentID].name, in_CADComponentData_map[in_PartComponentID].modelType,
		in_CADComponentData_map[in_PartComponentID].modelHandle, PRO_POINT, (wchar_t*)(const wchar_t*)in_DatumName, &datum_point);
	//in_CADComponentData_map[in_PartComponentID].modelHandle, PRO_POINT, datum_name, &datum_point);

	ProPoint  point;
	isis::isis_ProPointInit (	(ProSolid) datum_point.owner,  // ProSolid   owner_handle,
								datum_point.id,
								&point);

	ProVector part_xyz_point;
	isis::isis_ProPointCoordGet (point, part_xyz_point);



	
	double transformationMatrix[4][4];
	RetrieveTranformationMatrix_Assembly_to_Child (	in_AssemblyComponentID,
														in_CADComponentData_map[in_PartComponentID].componentPaths,
														in_CADComponentData_map,  
														PRO_B_TRUE,  // bottom up
														transformationMatrix);
			
	 ProVector from_assembly_xyz_point;
	 isis::isis_ProPntTrfEval( part_xyz_point, transformationMatrix, from_assembly_xyz_point);

	 out_CADPoint.x = from_assembly_xyz_point[0];
	 out_CADPoint.y = from_assembly_xyz_point[1];
	 out_CADPoint.z = from_assembly_xyz_point[2];
	 
}

// If at lease one of the assemblies in in_CADAssemblies contains analysisFEA and the analysisFEA is for ANALYSIS_DECK_BASED then return true.
	bool IsAFEAAnlysisDeckBasedRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesFEA.size() > 0   ) 
			{
				for each ( 	AnalysisFEA j in i->analysesCAD.analysesFEA )
				{
					for each ( AnalysisSolver k in j.analysisSolvers) 
					{
						if ( k.analysisSolutionType == ANALYSIS_DECK_BASED ) return true;
					}
				}
			}	//return true;
		}
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If at lease one of the assemblies in in_CADAssemblies contains analysisFEA and the analysisFEA is for ANALYSIS_DECK_BASED then return true.
	bool IsFEAAnalysisAbaqusModelBasedRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesFEA.size() > 0   ) 
			{
				for each ( 	AnalysisFEA j in i->analysesCAD.analysesFEA )
				{
					for each ( AnalysisSolver k in j.analysisSolvers) 
					{
						if ( k.analysisSolutionType == ANALYSIS_MODEL_BASED ) return true;
					}
				}
			}	//return true;
		}
		return false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetPolygonAnalysisGeometry( 
					const std::string								&in_AssemblyComponentID,
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
					const AnalysisGeometry							&in_AnalysisGeometry, 
					std::vector<isis_CADCommon::Point_3D>			&out_Polygon ) 
														throw (isis::application_exception)
{
	int numberPolygonPoints = 0;
	
	//for (std::list<AnalysisGeometryFeature>::const_iterator	l(in_AnalysisGeometry.features.begin()); l != in_AnalysisGeometry.features.end(); ++l ) 
	for (std::list<CADFeature>::const_iterator	l(in_AnalysisGeometry.features.begin()->features.begin()); l != in_AnalysisGeometry.features.begin()->features.end(); ++l ) 
	{
		isis::CADPoint  point;
		//isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
		//										in_AnalysisGeometry.componentID,
		//										in_CADComponentData_map,
		//										*l,  // Datum Point Name
		//										point);

		isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
												l->componentID,
												in_CADComponentData_map,
												l->datumName,  // // Datum Point Name 
												point);

		out_Polygon.push_back( isis_CADCommon::Point_3D( point.x, point.y, point.z));
		++numberPolygonPoints;

	}
	if ( numberPolygonPoints < 3 )
	{
		//std::string TempError = "Polygon does not have at least three points. ComponentID: " + 
		//						l->componentID + "ComponentName: " +
		//						in_CADComponentData_map[in_AnalysisGeometry.componentID].name + 
		//						" Datum feature Names: ";
		//for ( std::list<std::string>::const_iterator l( in_AnalysisGeometry.features.begin()); l != in_AnalysisGeometry.features.end(); ++l )
		//		TempError += *l + " ";	

		//std::string TempError = "Polygon does not have at least three points. ComponentID: " + 
		//						l->componentID + "ComponentName: " +
		//						in_CADComponentData_map[in_AnalysisGeometry.componentID].name + 
		//						" Component ID, Component Name, Datum Name: ";
		//	for (std::list<AnalysisGeometryFeature>::const_iterator	l(in_AnalysisGeometry.features.begin()); l != in_AnalysisGeometry.features.end(); ++l ) 
		//		TempError += l->componentID + " ";	
		std::string TempError = "Polygon does not have at least three points.  "; 
							//	l->componentID + "ComponentName: " +
							//	in_CADComponentData_map[in_AnalysisGeometry.componentID].name + 
							//	" Component ID, Component Name, Datum Name: ";
		for (std::list<CADFeature>::const_iterator	l(in_AnalysisGeometry.features.begin()->features.begin()); l != in_AnalysisGeometry.features.begin()->features.end(); ++l ) 
			TempError += "Component Instance ID: " + l->componentID + " Datum Name: " + (const std::string&)l->datumName;	

		throw isis::application_exception(TempError.c_str());
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string AnalysisGeometry_error(	const AnalysisGeometry	&in_AnalysisGeometry,
									const std::string       &in_CallingFunction,		
									const std::string		&in_Expected_or_Minimum,  // Should be Expected or Minimum
									int						in_NumPoints,
									int						in_ReceivedNumPoints )
{
	std::stringstream errorString;
	errorString << "Function - " << in_CallingFunction << ", received incorrect number of points defining the analysis geometry." << std::endl <<
	in_Expected_or_Minimum << " Number of Points: " << in_NumPoints << std::endl <<
	"Received Number of Points: " << in_ReceivedNumPoints << std::endl <<
	//"Geometry Type: " << CADGeometryType_string(in_AnalysisGeometry) << std::endl <<
	//"Component Instance ID: "  << in_AnalysisGeometry.componentID << std::endl <<
	"Geometry Type: " << CADGeometryType_string(in_AnalysisGeometry.features.begin()->geometryType) << std::endl;
	//"Component Instance ID: "  << in_AnalysisGeometry.componentID << std::endl <<
	//"Datum Names: " << std::endl;
	for (std::list<CADFeature>::const_iterator i(in_AnalysisGeometry.features.begin()->features.begin()); i !=in_AnalysisGeometry.features.begin()->features.end(); ++ i )
		errorString << std::endl << "Component Instance ID: " << i->componentID << "  Datum Name: " << i->datumName;

	return errorString.str();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
std::string NoGridPointsWithInMesh_ErrorSstring(	const std::string &in_ComponentID, 
												const std::string &in_ComponentName, 
												const std::list<std::string> &in_DatumPointNames ) 
{
	std::string TempError = "Component_ID: " + in_ComponentID + ", Component Name: " +  in_ComponentName + " Datum Points: ";

	for ( std::list<std::string>::const_iterator tmp_i( in_DatumPointNames.begin()); tmp_i != in_DatumPointNames.end(); ++tmp_i )
	{
		TempError += " " + *tmp_i;
	}
	TempError +=  " This problem could be due to one of three problems:  1) The above datum points are not ";
	TempError +=  "ordered in a manner that would define a polygon 2) The above datum points do not define a ";
	TempError +=  "convex polygon (concave polygons are not supported at this time) or 3) The polygon is well ";
	TempError +=  "formed, but there are no mesh grid points within the polygon.";

	return TempError;
}
*/
std::string NoGridPointsWithInMesh_ErrorSstring( const std::list<CADFeature> &in_ComponentID_DatumName ) 
{
	std::string TempError;

	for ( std::list<CADFeature>::const_iterator tmp_i( in_ComponentID_DatumName.begin()); tmp_i != in_ComponentID_DatumName.end(); ++tmp_i )
	{
		TempError += "\nComponent_ID: "  + tmp_i->componentID + "  Datum Point: " + (const std::string&)tmp_i->datumName;
	}

	return TempError;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ComponentInstanceID_DatumFeatureName
{
	std::string			componentInstanceID;
	MultiFormatString	datumName;

	ComponentInstanceID_DatumFeatureName(	const std::string &in_ComponentInstanceID, 
											const MultiFormatString &in_DatumName ) :
												componentInstanceID(in_ComponentInstanceID), 
												datumName(in_DatumName) {};

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DatumPointCoordinates
{
	ComponentInstanceID_DatumFeatureName	point_ID_Name;
	CADPoint								cADPoint;

	DatumPointCoordinates( const ComponentInstanceID_DatumFeatureName &in_ComponentInstanceID_DatumFeatureName,
						   const CADPoint							  &in_CADPoint) :
								point_ID_Name(in_ComponentInstanceID_DatumFeatureName),
								cADPoint(in_CADPoint) {};
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetDatumPointsReferencedByAnalyses( const	std::list<AnalysisConstraint>				&in_AnalysisConstraints,
										 const	std::list<AnalysisLoad>						&in_AnalysisLoads,
										 std::vector<ComponentInstanceID_DatumFeatureName>	&out_ComponentInstanceID_DatumFeatureName )
																				

{
	for each ( const AnalysisConstraint &i in in_AnalysisConstraints )
	{
		for each ( const AnalysisGeometryFeature &j in  i.geometry.features )
		{
			if ( j.analysisFeatureGeometryType == CAD_ANALYSIS_FEATURE_GEOMETRY_POINT )
			{
				for each ( const CADFeature &k in j.features )
				{
					ComponentInstanceID_DatumFeatureName  c_d(k.componentID, k.datumName );
					out_ComponentInstanceID_DatumFeatureName.push_back(c_d);								
				}
			}
		}
	}

	for each ( const AnalysisLoad &i in in_AnalysisLoads )
	{
		for each ( const AnalysisGeometryFeature &j in  i.geometry.features )
		{
			if ( j.analysisFeatureGeometryType == CAD_ANALYSIS_FEATURE_GEOMETRY_POINT )
			{
				for each ( const CADFeature &k in j.features )
				{
					ComponentInstanceID_DatumFeatureName  c_d(k.componentID, k.datumName );
					out_ComponentInstanceID_DatumFeatureName.push_back(c_d);								
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetDatumPointsCoordinates(	const std::vector<ComponentInstanceID_DatumFeatureName> &in_ComponentInstanceID_DatumFeatureName,   
								const std::string										&in_AssemblyComponentID,
								std::map<string, isis::CADComponentData>				&in_CADComponentData_map,
								std::vector<DatumPointCoordinates>						&out_datumPointCoordinates )
{
	for each ( const ComponentInstanceID_DatumFeatureName &i in in_ComponentInstanceID_DatumFeatureName )
	{
		CADPoint  point;
		RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
										i.componentInstanceID,
										in_CADComponentData_map,
										i.datumName,  // Datum Point Name
										point);
		out_datumPointCoordinates.push_back( DatumPointCoordinates(i, point ));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DatumKey_to_Point_map( const std::vector<DatumPointCoordinates>	&in_datumPointCoordinates,
							std::map<std::string, CADPoint>				&out_DatumKey_to_Point_map )
																		throw (isis::application_exception)
{
	for each ( const DatumPointCoordinates &i in in_datumPointCoordinates )
	{
		std:string key = i.point_ID_Name.componentInstanceID + ":" +  (std::string)i.point_ID_Name.datumName;
		
		std::map<std::string, CADPoint>::const_iterator  point_itr;

		point_itr = out_DatumKey_to_Point_map.find(key);

		if ( point_itr != out_DatumKey_to_Point_map.end() )
		{
			std::stringstream errorString;
			errorString <<
				"Function - " << __FUNCTION__ << " Duplicate datum point information with the key of: " <<  key;
			throw isis::application_exception(errorString.str());	
		}

		out_DatumKey_to_Point_map[key] = i.cADPoint;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FindPointsOnSurface( 	const std::map<int,isis_CADCommon::GridPoint>		&in_GridPoints_map,
							const std::string									&in_AssemblyComponentID,
							const std::string									&in_PartComponentID,
							std::map<string, isis::CADComponentData>			&in_CADComponentData_map,
							const MultiFormatString								&in_DatumName_PointOnSurface,
							std::vector<int>									&out_GridPointIds_WithinGeometry ) 
																				throw (isis::application_exception)						
{

	////////////////////////////////////////
	// Get point on surface coordinates
	////////////////////////////////////////
	CADPoint pointOnSurface_CADPoint;
	RetrieveDatumPointCoordinates( in_AssemblyComponentID,
									in_PartComponentID,
									in_CADComponentData_map,
									in_DatumName_PointOnSurface,
									pointOnSurface_CADPoint); 
	
	ProPoint3d pointOnSurface_XYZ_In;
	ProPoint3d pointOnSurface_XYZ_Out;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BEGIN Transform Point:  Must transform the point from the assembly coordinates to the part coordinates
	double transformationMatrix[4][4];
	RetrieveTranformationMatrix_Assembly_to_Child (	in_AssemblyComponentID,
														in_CADComponentData_map[in_PartComponentID].componentPaths,
														in_CADComponentData_map,  
														PRO_B_FALSE,  // bottom up
														transformationMatrix);
			
	 ProVector from_assembly_xyz_point;

	 ProVector pointInAssemblyCoordinates;
	 pointInAssemblyCoordinates[0] = pointOnSurface_CADPoint.x;
	 pointInAssemblyCoordinates[1] = pointOnSurface_CADPoint.y;
	 pointInAssemblyCoordinates[2] = pointOnSurface_CADPoint.z;

	 ProVector pointInPartCoordinates;

	 isis::isis_ProPntTrfEval( pointInAssemblyCoordinates, transformationMatrix, pointOnSurface_XYZ_In);
	// END Transform Point: 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//pointOnSurface_XYZ_In[0] = pointOnSurface_CADPoint.x;
	//pointOnSurface_XYZ_In[1] = pointOnSurface_CADPoint.y;
	//pointOnSurface_XYZ_In[2] = pointOnSurface_CADPoint.z;

	isis_LOG(lg, isis_FILE, isis_INFO);
	isis_LOG(lg, isis_FILE, isis_INFO) << __FUNCTION__ << " Data:";  
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Name:                      " << in_CADComponentData_map[in_PartComponentID].name;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   in_DatumName_PointOnSurface:         " << in_DatumName_PointOnSurface;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   in_DatumName_PointOnSurface (x,y,z): " << pointOnSurface_XYZ_In[0] << "  " << pointOnSurface_XYZ_In[1] << "  " << pointOnSurface_XYZ_In[2];
	isis_LOG(lg, isis_FILE, isis_INFO) << "   in_CADComponentData_map[in_PartComponentID].name:         " << in_CADComponentData_map[in_PartComponentID].name;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   in_CADComponentData_map[in_PartComponentID].modelHandle:  " << in_CADComponentData_map[in_PartComponentID].modelHandle;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   *in_CADComponentData_map[in_PartComponentID].p_model:     " << *in_CADComponentData_map[in_PartComponentID].p_model;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Find all Surfaces in Model (i.e. in_CADComponentData_map[in_PartComponentID].modelHandle
	////////////////////////////////////////////////////////////////////////////////////////////
	ProSurface	    *surfaces;
	isis_ProUtilCollectSolidSurfaces (in_CADComponentData_map[in_PartComponentID].modelHandle, &surfaces);

	//////////////////////////////////
	// Find surfaces containing point
	//////////////////////////////////
 	int surfaces_num;

	isis_ProArraySizeGet ((ProArray)surfaces, &surfaces_num);

	isis_LOG(lg, isis_FILE, isis_INFO) << "Number of surfaces in the component: " << surfaces_num;  

	if ( surfaces_num < 1 ) 
	{
		std::stringstream errorString;
		errorString <<
			"Function - " << __FUNCTION__ <<", No surfaces found in model (i.e. component).  There must be at least one surface in the model if a point is used to identify a surface." << std::endl <<
			"in_PartComponentID           " << (string)in_CADComponentData_map[in_PartComponentID].componentID << std::endl <<
			"in_PartComponentID name      " << (string)in_CADComponentData_map[in_PartComponentID].name << std::endl <<
			"in_DatumName_PointOnSurface: " <<   (string)in_DatumName_PointOnSurface << std::endl <<
			"Point coordinates (x,y,z)    " <<  pointOnSurface_XYZ_In[0] << "  " << pointOnSurface_XYZ_In[1]  << "  " << pointOnSurface_XYZ_In[2];
		throw isis::application_exception(errorString.str());
	}
	
	std::vector<int> foundOnSurfaces_index;

	for (int i = 0; i < surfaces_num; i++)
    {
		ProBoolean onSurface;

		isis_ProPoint3dOnsurfaceFind (	pointOnSurface_XYZ_In, surfaces[i], &onSurface, pointOnSurface_XYZ_Out );
		if ( onSurface == PRO_B_TRUE) foundOnSurfaces_index.push_back(i);
    }

	if ( foundOnSurfaces_index.size() == 0 ) 
	{
		std::stringstream errorString;
		errorString <<
			"Function - " << __FUNCTION__ <<", No surface found with the in_DatumName_PointOnSurface on a surface in the model (i.e. component)." << std::endl <<
			"in_PartComponentID           " << (string)in_CADComponentData_map[in_PartComponentID].componentID << std::endl <<
			"in_PartComponentID name      " << (string)in_CADComponentData_map[in_PartComponentID].name << std::endl <<
			"in_DatumName_PointOnSurface: " <<   (string)in_DatumName_PointOnSurface << std::endl <<
			"Point coordinates (x,y,z)    " <<  pointOnSurface_XYZ_In[0] << "  " << pointOnSurface_XYZ_In[1]  << "  " << pointOnSurface_XYZ_In[2];
		throw isis::application_exception(errorString.str());
	}

	isis_LOG(lg, isis_FILE, isis_INFO) << "Number of surfaces that the point on the face touches: " << foundOnSurfaces_index.size();

	ProPoint3d point_XYZ_In;
	ProPoint3d point_XYZ_Out;

	//   PRO_ACCURACY_RELATIVE = 1,
	//   PRO_ACCURACY_ABSOLUTE = 2
	//double accuracyRelative;
	//double accuracyAbsolute;

	// ProAccuracyType r_type_relative = PRO_ACCURACY_RELATIVE;
	// ProAccuracyType r_type_absolute = PRO_ACCURACY_ABSOLUTE;

	//ProErr local_error;
	//local_error = ProSolidAccuracyGet (in_CADComponentData_map[in_PartComponentID].modelHandle,
    //                                 &r_type_relative,
    //                                 &accuracyRelative);
	//isis_LOG(lg, isis_FILE, isis_INFO) << "local_error: " << local_error;
	//
	//local_error = ProSolidAccuracyGet (in_CADComponentData_map[in_PartComponentID].modelHandle,
    //                                 &r_type_absolute,
    //                                 &accuracyAbsolute);
	//isis_LOG(lg, isis_FILE, isis_INFO) << "local_error: " << local_error;
	//
	//isis_LOG(lg, isis_FILE, isis_INFO) << "Accuracy, relative, absolute " << accuracyRelative << "  " << accuracyAbsolute;

	bool firstErrorMessage = true;

	for each ( const std::pair<int,isis_CADCommon::GridPoint> &i in in_GridPoints_map )
	{
		point_XYZ_In[0] = i.second.point.x;
		point_XYZ_In[1] = i.second.point.y;
		point_XYZ_In[2] = i.second.point.z;
		
		//isis_LOG(lg, isis_FILE, isis_INFO) << "Grid Point: " << point_XYZ_In[0] << "  " << point_XYZ_In[1] << "  " << point_XYZ_In[2];


		for each ( const int &j in foundOnSurfaces_index )
		{ 			
			ProBoolean onSurface;
			//isis_LOG(lg, isis_FILE, isis_INFO) << "point_XYZ_In,  " << point_XYZ_In[0] << "  " << point_XYZ_In[1] << "  " << point_XYZ_In[2];

			try
			{
				// We must transform the Nastran grid points to the coordinate system of the part before checking if the point (i.e. grid point)
				// is on the surface.
				ProPoint3d pointOnSurface_XYZ_In_partCys;
				isis::isis_ProPntTrfEval( point_XYZ_In, transformationMatrix, pointOnSurface_XYZ_In_partCys);
				isis_ProPoint3dOnsurfaceFind (	pointOnSurface_XYZ_In_partCys, surfaces[j], &onSurface, point_XYZ_Out );	
				//isis_ProPoint3dOnsurfaceFind (	point_XYZ_In, surfaces[j], &onSurface, point_XYZ_Out );	
			}
			catch ( isis::application_exception ex )
			{
				// ProPoint3dOnsurfaceFind seems to always work for the case where the point is on the surface.  If the point is off of the
				// surface, it fails some of the time.  This maybe because point_XYZ_Out cannot be calculated within the Creo model tolerances.
				// For example, if a point would project so that it would be mostly tangential to the surface then the projection would 
				// probably fail.
				// In general, this failure is not a problem because we are interested in only points on the surface.
				if ( firstErrorMessage )
				{
					isis_LOG(lg, isis_FILE, isis_INFO);
					isis_LOG(lg, isis_FILE, isis_INFO) << "INFORMATION Function: " << __FUNCTION__;
					isis_LOG(lg, isis_FILE, isis_INFO) << "   The computation of a point on a surface (i.e. ProPoint3dOnsurfaceFind) failed.";
					isis_LOG(lg, isis_FILE, isis_INFO) << "   This is usually occurs because the point is not on the surface and the projection";
					isis_LOG(lg, isis_FILE, isis_INFO) << "   of the point onto the surface fails. This projection failure could occur because";
					isis_LOG(lg, isis_FILE, isis_INFO) << "   the projection would be mostly tangential to the surface.  We are only interested";
					isis_LOG(lg, isis_FILE, isis_INFO) << "   in points on the surface and thus this failure does not matter.";
					firstErrorMessage = false;
				}

				isis_LOG(lg, isis_FILE, isis_INFO) << "      INFORMATION, ProPoint3dOnsurfaceFind could not project point (x, y, z): " << point_XYZ_In[0] << "  " << point_XYZ_In[1] << "  " << point_XYZ_In[2];
				//isis_LOG(lg, isis_FILE, isis_INFO) << "   Error message: " <<ex.what();

			}
			// isis_LOG(lg, isis_FILE, isis_INFO) << "point_XYZ_Out,  " << point_XYZ_Out[0] << "  " << point_XYZ_Out[1] << "  " << point_XYZ_Out[2];

			if ( onSurface == PRO_B_TRUE ) out_GridPointIds_WithinGeometry.push_back(i.first);
			// A point on the edge of the surface is considered on the surface.  Therefore, the following code is not needed.
			//if ( onSurface == PRO_B_TRUE ||
			//	(	abs( point_XYZ_In[0] - point_XYZ_Out[0]  ) < accuracyRelative &&
			//		abs( point_XYZ_In[1] - point_XYZ_Out[1]  ) < accuracyRelative &&
			//		abs( point_XYZ_In[2] - point_XYZ_Out[2]  ) < accuracyRelative ) )
			//{
			//	out_GridPointIds_WithinGeometry.push_back(i.first);
			//}
		}
	} //END for each ( const std::pair<int,isis_CADCommon::GridPoint> &i in in_GridPoints_map )
	
	isis_ProArrayFree ((ProArray*)&surfaces);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetGridPointsWithinAnalysisGeometry( 
	const std::string								&in_AssemblyComponentID,
	std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
	const AnalysisGeometry							&in_AnalysisGeometry,
	std::map<int,isis_CADCommon::GridPoint>			&in_GridPoints_map,
	double											in_Precision,
	std::vector<int>								&out_GridPointIds_WithinGeometry ) 
										throw (isis::application_exception)
{

	// old with features were layed out differently, old strucs had features then feature    // if ( in_AnalysisGeometry.features.size() != 1  || in_AnalysisGeometry.setOperationDefined )  // Not supporting set operations yet
	if (  in_AnalysisGeometry.setOperationDefined )  // Not supporting set operations yet
	{
		// qqqq Need a beter error message
		std::string TempError = "Set operations are not currently supported."; 
		throw isis::application_exception(TempError.c_str());	
	}


	///////////////////////////////////////////////////////////////////////////////////////
	// Retrieve CAD Point Coordinates that define the Geometry (e.g. cylinder, sphere,...)
	///////////////////////////////////////////////////////////////////////////////////////

	std::vector<int>			gridPointIds_WithinGeometry;

	for ( std::list<AnalysisGeometryFeature>::const_iterator i(in_AnalysisGeometry.features.begin());
		  i != in_AnalysisGeometry.features.end(); ++i )
	{
		if ( i->geometryType == CAD_GEOMETRY_FACE )
		{
			// CAD_GEOMETRY_FACE is different because it depends on Creo surface geometry.  Therefore, the Creo suface geometry
			// must be used to determine if a point is on the surface.

			// For CAD_GEOMETRY_FACE there should be one and only one datum point

			if ( i->analysisFeatureGeometryType != CAD_ANALYSIS_FEATURE_GEOMETRY_POINT )
			{
				std::stringstream errorString;
				errorString <<
					"Function - " << __FUNCTION__ <<", CAD_GEOMETRY_FACE (i.e. surface) must have a featureGeometryType of CAD_ANALYSIS_FEATURE_GEOMETRY_POINT." << std::endl <<
					"featureGeometryType: " << CADAnalysisFeatureGeometryType_string(i->analysisFeatureGeometryType);
				throw isis::application_exception(errorString.str());
			}
			if ( i->features.size() != 1 )
			{
				std::stringstream errorString;
				errorString <<
					"Function - " << __FUNCTION__ <<", CAD_GEOMETRY_FACE (i.e. surface) must have one and only one datum point defining the face (i.e. surface)." << std::endl <<
					"features.size(): " << i->features.size();
				throw isis::application_exception(errorString.str());

			}

			FindPointsOnSurface( 	in_GridPoints_map,
									in_AssemblyComponentID,
									i->features.begin()->componentID,
									in_CADComponentData_map,
									i->features.begin()->datumName,
									gridPointIds_WithinGeometry );
		}
		else
		{
			// This section covers geometry that is defined by points
			std::vector< isis_CADCommon::Point_3D >  geometry3DPoints;

			// Retrieve point coordinates defining the geometry
			for ( std::list<CADFeature>::const_iterator k( i->features.begin());
					k != in_AnalysisGeometry.features.begin()->features.end();
					++k )
			{
				isis::CADPoint  point;
				isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
														k->componentID,
														in_CADComponentData_map,
														k->datumName,  // Datum Point Name
														point);
				geometry3DPoints.push_back( isis_CADCommon::Point_3D( point.x, point.y, point.z));
				//++numberPolygonPoints;
			}

			isis_CADCommon::Geometry_3D *Geometry_3D_ptr;

			// Thd following three variable could be used by switch case CAD_GEOMETRY_EXTRUSION 
			int tempCount = 0;
			std::vector< isis_CADCommon::Point_3D >  temp3DPoints;
			isis_CADCommon::Point_3D   temp3DPoint;

			switch (i->geometryType)
			{
				case CAD_GEOMETRY_POLYGON : 
					if ( geometry3DPoints.size() < 3 ) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", "Minimum", 3,  geometry3DPoints.size()).c_str());
					Geometry_3D_ptr = new isis_CADCommon::Polygon_3D(geometry3DPoints, in_Precision, "GetGridPointsWithinAnalysisGeometry" );
					break;
				case CAD_GEOMETRY_CIRCLE : 
					if ( geometry3DPoints.size() != 3) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", "Expected",3,  geometry3DPoints.size()).c_str());
					Geometry_3D_ptr = new isis_CADCommon::Circle_3D( geometry3DPoints[0], geometry3DPoints[1], geometry3DPoints[2], in_Precision );
					break;
				case CAD_GEOMETRY_CONCENTRIC_CIRCLES:
					if ( geometry3DPoints.size() != 3) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", "Expected",3, geometry3DPoints.size()).c_str());
					Geometry_3D_ptr = new isis_CADCommon::ConcentricCircles_3D( geometry3DPoints[0], geometry3DPoints[1], geometry3DPoints[2], in_Precision );
					break;
				case CAD_GEOMETRY_CYLINDER : 
					if ( i->secondaryGeometryQualifier != CAD_INCLUDE_END_CAPS && i->secondaryGeometryQualifier != CAD_EXCLUDE_END_CAPS)
					{
						std::string TempError = "For Cylinders, the End Caps qualifier must be set to " + CADSecondaryGeometryQualifier_string(CAD_INCLUDE_END_CAPS) + 
							" or " + CADSecondaryGeometryQualifier_string(CAD_EXCLUDE_END_CAPS) + ".  Feature ID: " + i->featureID; 
						throw isis::application_exception(TempError.c_str());	
					}
					isis_CADCommon::e_CylinderGeometryInclusionSpecifier  cylinderGeometryInclusionSpecifier;
					if ( i->secondaryGeometryQualifier == CAD_INCLUDE_END_CAPS ) 
						cylinderGeometryInclusionSpecifier = isis_CADCommon::GEOMETRY_INCLUDE_END_CAP;
					else
						cylinderGeometryInclusionSpecifier = isis_CADCommon::GEOMETRY_EXCLUDE_END_CAP;

					if ( geometry3DPoints.size() != 3) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", "Expected",3, geometry3DPoints.size()).c_str());
					Geometry_3D_ptr = new isis_CADCommon::Cylinder_3D(geometry3DPoints[0], geometry3DPoints[1], geometry3DPoints[2], cylinderGeometryInclusionSpecifier, in_Precision );
					break;
				//case CAD_GEOMETRY_CYLINDER_SURFACE :   // This was replace by computing the cylinder-all-points and subtracting the cylinder-internal-points 
				//	 if ( geometry3DPoints.size() != 3) throw isis::application_exception(
				//			AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", 3, geometry3DPoints.size()).c_str());
				//	 Geometry_3D_ptr = new isis_CADCommon::Cylinder_3D(geometry3DPoints[0], geometry3DPoints[1], geometry3DPoints[2], isis_CADCommon::GEOMETRY_EXCLUDE_END_CAP, in_Precision );
				//	 break;
				case CAD_GEOMETRY_SPHERE : 
					if ( geometry3DPoints.size() != 2) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", "Expected", 2, geometry3DPoints.size()).c_str());
					Geometry_3D_ptr = new isis_CADCommon::Sphere_3D(geometry3DPoints[0], geometry3DPoints[1], in_Precision );
					break;


				case CAD_GEOMETRY_EXTRUSION : 
					if ( geometry3DPoints.size() < 4) throw isis::application_exception(
						AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry","Minimum", 4, geometry3DPoints.size()).c_str());

					for each ( const isis_CADCommon::Point_3D &i in geometry3DPoints )
					{
						++tempCount;
						if ( tempCount <  geometry3DPoints.size() )
							temp3DPoints.push_back(i);
						else
							temp3DPoint = i;
					}

					// Extrusion_3D(	const std::vector<Point_3D>  in_Polygon_Points,
					//			const Point_3D	&in_offsetFromPolygon,  
					//			double in_Tolerance = DEFAULT_TOLERANCE )throw (isis::application_exception);
					Geometry_3D_ptr = new isis_CADCommon::Extrusion_3D(temp3DPoints, temp3DPoint, in_Precision );
					break;


				//case CAD_GEOMETRY_SPHERE_SURFACE: 
				//	if ( geometry3DPoints.size() != 2) throw isis::application_exception(
				//		AnalysisGeometry_error(	in_AnalysisGeometry, "GetGridPointsWithinAnalysisGeometry", 2, geometry3DPoints.size()).c_str());
				//	Geometry_3D_ptr = new isis_CADCommon::SphereSurface_3D(geometry3DPoints[0], geometry3DPoints[1], in_Precision );
				//	break;
				default:
				std::stringstream errorString;
				errorString <<
					"Function - GetGridPointsWithinAnalysisGeometry, received geometryType that is not currently supported." << std::endl <<
					//"Geometry Type: " << CADGeometryType_string(in_AnalysisGeometry.geometryType);
					"Geometry Type: " << CADGeometryType_string(in_AnalysisGeometry.features.begin()->geometryType);
				throw isis::application_exception(errorString.str());
			}

			if ( i->primaryGeometryQualifier != CAD_INTERIOR_ONLY && 
				 i->primaryGeometryQualifier != CAD_BOUNDARY_ONLY && 
				 i->primaryGeometryQualifier != CAD_INTERIOR_AND_BOUNDARY )
			{
				std::string TempError = "Function: GetGridPointsWithinAnalysisGeometry, recieved an erroneous PrimaryGeometryQualifier (" + CADPrimaryGeometryQualifier_string(i->primaryGeometryQualifier) + 
					" ).  Feature ID: " + i->featureID; 
				throw isis::application_exception(TempError.c_str());	
			}

			isis_CADCommon::e_GeneralGeometryInclusionSpecifier generalGeometryInclusionSpecifier;
			switch ( i->primaryGeometryQualifier )
			{
				case CAD_INTERIOR_ONLY:			generalGeometryInclusionSpecifier = isis_CADCommon::GEOMETRY_INTERIOR_ONLY;	break;
				case CAD_BOUNDARY_ONLY:			generalGeometryInclusionSpecifier = isis_CADCommon::GEOMETRY_BOUNDARY_ONLY;	break;
				case CAD_INTERIOR_AND_BOUNDARY: generalGeometryInclusionSpecifier = isis_CADCommon::GEOMETRY_INTERIOR_AND_BOUNDARY; break;
			}

			//std::cout << std::endl << "in_GridPoints_map size: " << in_GridPoints_map.size();
			isis_CADCommon::GetGridPointsWithinGeometry( in_GridPoints_map, *Geometry_3D_ptr, generalGeometryInclusionSpecifier, in_Precision, gridPointIds_WithinGeometry);
			delete Geometry_3D_ptr;
		}  // END if ( i->geometryType == CAD_GEOMETRY_FACE ) else
	}  // END for ( std::list<AnalysisGeometryFeature>::const_iterator i...

	if ( gridPointIds_WithinGeometry.size() == 0 )
	{
		std::stringstream errorString;
		errorString <<  "Function - GetGridPointsWithinAnalysisGeometry, Failed to find FEA grid points within the geometry defined by:" << std::endl <<
						NoGridPointsWithInMesh_ErrorSstring(  in_AnalysisGeometry.features.begin()->features ); 
		throw isis::application_exception(errorString.str());		
	}
	// Remove duplicates, if any
	set<int> tempSet( gridPointIds_WithinGeometry.begin(), gridPointIds_WithinGeometry.end() );
	out_GridPointIds_WithinGeometry.assign(tempSet.begin(), tempSet.end());

}

pro_fem_analysis_type ProFemAnalysisType( e_AnalysisType in_AnalysisType )
{
	switch ( in_AnalysisType )
	{
		case ANALYSIS_STRUCTURAL:
			return PRO_FEM_ANALYSIS_STRUCTURAL;
			break;
		case ANALYSIS_BUCKLING:
			return PRO_FEM_ANALYSIS_STRUCTURAL;
			break;

		case ANALYSIS_MODAL:
			return PRO_FEM_ANALYSIS_MODAL;
			break;

		case ANALYSIS_THERMAL:
			return PRO_FEM_ANALYSIS_THERMAL;
			break;

		case ANALYSIS_FATIGUE:
			return PRO_FEM_ANALYSIS_STRUCTURAL;
			break;

		default:
			return PRO_FEM_ANALYSIS_STRUCTURAL;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. Output mesh of existing assembly
// 2. Create uniquely defined materials
// 3. Modify the mesh as follows:
//		a) Add Comments, Date, .exe version number, assembly config ID, analysis ID, Material to ComponentIDs.
//		b) Add constraints
//		c) Add loads
//
void CreateFEADeck(	const std::map<std::string, Material>			&in_Materials,
					const std::string								&in_ProgramName_Version_TimeStamp,
					const std::string								&in_WorkingDir,  
					const std::string								&in_OriginalMeshFileName,
					const std::string								&in_ModifiedMeshFileName,
					const std::string								&in_AssemblyConfigurationID,
					const std::string								&in_AssemblyComponentID,
					const CADAnalyses								&in_CADAnalyses,
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
					std::map<std::string, std::string>				&out_NastranMaterialID_to_CompnentID_map) 
													throw (isis::application_exception)
{
	// Must modify the parts to have a default material that is unique for each Creo part.
	// This is necessary so that the mesh file will have a separate material for each part; and thus,
	// each material can be mapped to a GME component ID.
	isis::CreateUniquelyNamedMaterials(in_AssemblyComponentID, in_CADComponentData_map );

	// The precision is based on the significant figures in the mesh file generated by Creo.
	// Should read the grid points in the mesh file and determine an appropriate precision.
	// This is important because the value will vary based on the dimensions of the model.
	// For now, since mm is always used, use .01
	double precision = .01;  							 

	int subCaseID = 0;
	int	constraintSetID = 259;
	int	loadStatementID = 559;
	int	loadSetID = 59;
	int temperatureInitialSetID = 102;
	int nLParmID = 110;
 

	for ( std::list<AnalysisFEA>::const_iterator i(in_CADAnalyses.analysesFEA.begin()); i != in_CADAnalyses.analysesFEA.end(); ++i )
	{
		//std::string MeshUnmodified_PathAndFileName = in_WorkingDir + "\\" + in_CADComponentData_map[in_AssemblyComponentID].name +
		//											 "_Nastran_orig.nas";	

		//std::string MeshModified_PathAndFileName = in_WorkingDir + "\\" + in_CADComponentData_map[in_AssemblyComponentID].name +
		//											 "_Nastran_mod.nas";	

		std::string MeshUnmodified_PathAndFileName = in_WorkingDir + "\\" + in_OriginalMeshFileName;

		std::string MeshModified_PathAndFileName =   in_WorkingDir + "\\" + in_ModifiedMeshFileName;

		isis::MeshModel ( in_CADComponentData_map[in_AssemblyComponentID].modelHandle,
						  // Use PRO_FEM_ANALYSIS_STRUCTURAL even if thermal analysis.  This is needed so that a 
						  // Mat1 card would be created for each part so that poission's ratio could be mapped to componentInstanceIDs
						  PRO_FEM_ANALYSIS_STRUCTURAL, 
						  // Old ProFemAnalysisType(i->type) , should use this when mapping poisons ratio to component ID is no longer needed
						  PRO_FEM_FEAS_NASTRAN,
						  (*i->analysisSolvers.begin()).meshType,
						  (*i->analysisSolvers.begin()).shellElementType,
						  (*i->analysisSolvers.begin()).elementShapeType,
						   MeshUnmodified_PathAndFileName );

		// Read the mesh
		isis_CADCommon::NastranDeck     nastranDeck;  // Only supporting Nastran deck for now, will have to change this later.
		nastranDeck.ReadNastranDeck(MeshUnmodified_PathAndFileName);

		isis_CADCommon::NastranDeckHelper nastranDeckHelper(nastranDeck);


		nastranDeck.AppendHeaderComment( NASTRAN_META_COMMENT_TAG + in_ProgramName_Version_TimeStamp);
		nastranDeck.AppendHeaderComment(NASTRAN_COMMENT_TAG );  // Add space

		std::string tempString =  in_AssemblyConfigurationID;
		nastranDeck.AppendHeaderComment(NASTRAN_META_COMMENT_TAG + "Assembly ConfigurationID: " + tempString );
		nastranDeck.AppendHeaderComment(NASTRAN_META_COMMENT_TAG + "FEA AnalysisID:           " + i->ID );
		nastranDeck.AppendHeaderComment(NASTRAN_META_COMMENT_TAG + "FEA AnalysisType:         " + AnalysisType_string(i->type));
		nastranDeck.AppendHeaderComment(NASTRAN_COMMENT_TAG );  // Add space

		//std::map<std::string, std::string> NastranMaterialID_to_CompnentID_map;
		
		//////////////////////////////
		// Replace Material Tokens
		/////////////////////////////
		
		// in_AssemblyComponentID is the top assembly.  We need to modify the material properties
		// for all parts subordinate to the top assembly.
		
		ComponentVistorMaterialTokens componentVistorMaterialTokens;
		VisitComponents(in_AssemblyComponentID, in_CADComponentData_map, componentVistorMaterialTokens);

		nastranDeck.ReplaceMaterialTokens_ReturnMaterialToComponentID(  
														in_AssemblyComponentID,
														componentVistorMaterialTokens.materialKey_ComponentID_map,
														componentVistorMaterialTokens.componentID_PoissonsRatio_map,
														out_NastranMaterialID_to_CompnentID_map );


		std::map<std::string, std::string> compnentID_to_NastranMaterialID_map;

		for ( std::map<std::string, std::string>::const_iterator j(out_NastranMaterialID_to_CompnentID_map.begin());
			  j != out_NastranMaterialID_to_CompnentID_map.end();
			  ++j )
		{
			std::string tempString = "MaterialID: " + j->first + "  ComponentID: " + j->second + "  ComponentName: " + (const std::string&)in_CADComponentData_map[j->second].name;
			nastranDeck.AppendHeaderComment(NASTRAN_META_COMMENT_TAG + tempString);
			compnentID_to_NastranMaterialID_map.insert(std::pair<std::string,std::string>(j->second, j->first));
		}

		nastranDeck.AppendHeaderComment(NASTRAN_COMMENT_TAG );  // Add space

		// By default, the type is ANALYSIS_STRUCTURAL, which would be SOL SESTATIC (i.e. 101).  This solution would have been set by the Creo mesher
		if ( i->type ==  ANALYSIS_THERMAL)
		{
			// Need to comment out the MAT1 cards because they would have a density retrieved from the Creo model.
			// The MAT4 cards have a density retrieved from the materials library.  Don't want the MAT1 and MAT4 cards
			// density to disagree.  For thermal runs, a MAT1 card is not needed.
			nastranDeck.CommentOutMat1Cards();
			nastranDeck.ReplaceExecutiveControlWithThermalStatements();
			
			nastranDeck.ReplaceCaseControlWithThermalStatements(	in_CADComponentData_map[in_AssemblyComponentID].name,
																	//temperatureInitialSetID, 
																	nLParmID );
			nastranDeck.AddSubCaseAndLoadStatement(subCaseID, constraintSetID, loadStatementID, loadSetID, false );

			///////////////////////////////
			// Add the MAT4 Cards
			////////////////////////////////
			// If the Creo model was used with Creo Thermal Simulate, then MAT4 cards are created.
			// We want to use Mat4 cards based on the material library entries.
			if ( nastranDeck.getMaterialData_MAT4().size() > 0 ) nastranDeck.deleteAllMaterialData_MAT4();
	
			int Mat4Card_counter = 0;
			for ( std::map<std::string, std::string>::const_iterator ir_p_to_c =  out_NastranMaterialID_to_CompnentID_map.begin();
				  ir_p_to_c != out_NastranMaterialID_to_CompnentID_map.end(); 
				  ++ir_p_to_c)
			{
				std::cout << std::endl << " Material Key: " << ir_p_to_c->first << "  ComponentID: " << ir_p_to_c->second;

				std::string materialName = in_CADComponentData_map[ir_p_to_c->second].materialID_FromCreoPart;

				std::cout << std::endl << " Material Name: " <<  materialName;

				++Mat4Card_counter;
				// Create the MAT4 card
				std::map<std::string, Material>::const_iterator mat_itr;

				mat_itr = in_Materials.find(materialName);

				if ( mat_itr == in_Materials.end() )
				{
					std::stringstream errorString;
					errorString <<
						"Function - " << __FUNCTION__ << " Could not find material in in_Materials, material name: " <<  materialName;
					throw isis::application_exception(errorString.str());	
				}

				if ( !mat_itr->second.analysisMaterialProperties.thermalConductivityDefined )
				{
					std::stringstream errorString;
					errorString <<
						"Function - " << __FUNCTION__ << " Could not find thermalConductivity for material, material name: " <<  materialName;
					throw isis::application_exception(errorString.str());	
				}

				if ( !mat_itr->second.analysisMaterialProperties.heatCapacityDefined )
				{
					std::stringstream errorString;
					errorString <<
						"Function - " << __FUNCTION__ << " Could not find heatCapacity for material, material name: " <<  materialName;
					throw isis::application_exception(errorString.str());	
				}

				if ( !mat_itr->second.analysisMaterialProperties.denstiyDefined )
				{
					std::stringstream errorString;
					errorString <<
						"Function - " << __FUNCTION__ << "Could not find density for material, material name: " <<  materialName;
					throw isis::application_exception(errorString.str());	
				}
				

				// boost::lexical_cast was resulting in 
				// MAT4,1,0.0155,510,7.9500000000000001e-009
				// the 7.9500000000000001 was not being handled by Calculix.
				isis_CADCommon::MAT4 mat4_temp;
				mat4_temp.MID = Mat4Card_counter;

				stringstream thermalConductivity_temp;
				thermalConductivity_temp << mat_itr->second.analysisMaterialProperties.thermalConductivity;
				mat4_temp.K = thermalConductivity_temp.str();
				//mat4_temp.K =	boost::lexical_cast<string>(mat_itr->second.analysisMaterialProperties.thermalConductivity);
				
				stringstream heatCapacity_temp;
				heatCapacity_temp << mat_itr->second.analysisMaterialProperties.heatCapacity;
				mat4_temp.CP = heatCapacity_temp.str();
				//mat4_temp.CP =  boost::lexical_cast<string>(mat_itr->second.analysisMaterialProperties.heatCapacity);
				
				stringstream density_temp;
				density_temp << mat_itr->second.analysisMaterialProperties.density;
				mat4_temp.p = density_temp.str();
				//mat4_temp.p =   boost::lexical_cast<string>(mat_itr->second.analysisMaterialProperties.density);
				nastranDeck.AddMaterialData_MAT4(mat4_temp);

			} // for ( ir_p_to_c = componentVistorMa

		}
		else
		{
			nastranDeck.AddSubCaseAndLoadStatement(subCaseID, constraintSetID, loadStatementID, loadSetID );
		}


		////////////////////////////////
		// Retrieve mesh grid points.
		////////////////////////////////
		std::map<int,isis_CADCommon::GridPoint> gridPoints_map = nastranDeck.getGridPointData();
		//nastranDeck.GetGridPoints(gridPoints_map);

		//std::cout << std::endl << "Before  for ( list<AnalysisFEA>::const_iterator j , count: " << in_CADAnalyses.analysisFEA.size();
		for ( list<AnalysisFEA>::const_iterator j(in_CADAnalyses.analysesFEA.begin());
			  j != in_CADAnalyses.analysesFEA.end();
			  ++j )
		{
			//////////////////////////
			// Add the constraints
			//////////////////////////
			for ( std::list<AnalysisConstraint>::const_iterator k(j->analysisConstraints.begin());
				  k != j->analysisConstraints.end();
				  ++ k)
			{
				if ( k->convectionBoundaryDefined ) continue;  // Will do this later because the default temp must be defined.

				std::vector<isis_CADCommon::Point_3D>   polygon;
				int numberPolygonPoints = 0;

				std::vector<int>		gridPointIds_WithinGeometry;

				//////////////////////////////
				// Get points within geometry
				//////////////////////////////
				GetGridPointsWithinAnalysisGeometry( in_AssemblyComponentID, 
													 in_CADComponentData_map,
													 k->geometry, 
													 gridPoints_map,
													 precision,
													 gridPointIds_WithinGeometry);

				if ( gridPointIds_WithinGeometry.size() == 0 )
				{
					//std::string TempError = NoGridPointsWithInMesh_ErrorSstring(  k->geometry.componentID, 
					//								in_CADComponentData_map[k->geometry.componentID].name, 
					//								 k->geometry.features );
					// qqqqq
					std::string TempError = NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features); 

					throw isis::application_exception(TempError.c_str());					
				}

				//////////////////////////////////////////
				// Add the Contraints to the Nastran Deck
				//////////////////////////////////////////
				///////////////////////
				// Displacement
				///////////////////////
				if( k->analysisDisplacementDefined )
				{
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						bool x_DisplacementDefined = true;
						bool y_DisplacementDefined = true;
						bool z_DisplacementDefined = true;

						if (  k->displacement.translation.x_FixedFree == ANALYSIS_CONSTRAINT_FREE ) x_DisplacementDefined = false;
						if (  k->displacement.translation.y_FixedFree == ANALYSIS_CONSTRAINT_FREE ) y_DisplacementDefined = false;
						if (  k->displacement.translation.z_FixedFree == ANALYSIS_CONSTRAINT_FREE ) z_DisplacementDefined = false;

						nastranDeck.AddConstraintToDeck( constraintSetID, *m, 
														x_DisplacementDefined,
														y_DisplacementDefined,
														z_DisplacementDefined,
														k->displacement.translation.x, 
														k->displacement.translation.y,
														k->displacement.translation.z);
					}
				}

				///////////////////////
				// Pin
				///////////////////////
				if( k->analysisPinDefined )
				{
					//if ( k->geometry.geometryType != CAD_GEOMETRY_CYLINDER_SURFACE )
					if ( k->geometry.features.begin()->geometryType != CAD_GEOMETRY_CYLINDER )
					{
						std::stringstream errorString;
						errorString <<
							"Function - CreateFEADeck, when defining a Pin constrait, the geometry type must be " << CADGeometryType_string(CAD_GEOMETRY_CYLINDER)  << std::endl <<
						//	"Geometry Type: " << CADGeometryType_string(k->geometry.geometryType);
							"Geometry Type: " << CADGeometryType_string(k->geometry.features.begin()->geometryType);
						throw isis::application_exception(errorString.str());	
					}

						std::vector< isis_CADCommon::Point_3D >  geometry3DPoints;

						//for ( std::list<std::string>::const_iterator l( k->geometry.features.begin());
						//		l != k->geometry.features.end();
						//		++l )
						for ( std::list<CADFeature>::const_iterator l( k->geometry.features.begin()->features.begin());
								l != k->geometry.features.begin()->features.end();
								++l )
						{
							isis::CADPoint  point;
							//isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
							//										k->geometry.componentID,
							//										in_CADComponentData_map,
							//										*l,  // Datum Point Name
							//										point);
							isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
																	l->componentID,
																	in_CADComponentData_map,
																	l->datumName,  // Datum Point Name
																	point);

							geometry3DPoints.push_back( isis_CADCommon::Point_3D( point.x, point.y, point.z));
							++numberPolygonPoints;
						}

						if ( geometry3DPoints.size() != 3 )
						{
							std::stringstream errorString;
							errorString <<
							"Function - CreateFEADeck, when defining a Pin constrait, the defining geometry must have 3 Datum Points " << std::endl <<
							//"Geometry Type: " << CADGeometryType_string(k->geometry.geometryType) << std::endl <<
							//"Component Instance ID: " << k->geometry.componentID;
							// qqqqq
							NoGridPointsWithInMesh_ErrorSstring(k->geometry.features.begin()->features);
							throw isis::application_exception(errorString.str());	

						}

						int addedCoordinateSystemID;
						nastranDeck.AddCylindricalOrSphericalCoordinateSystem(   geometry3DPoints[0], // orgin
															 geometry3DPoints[1],  // Z axis
															 geometry3DPoints[2],  // point in z x plane
															 1,
															 isis_CADCommon::NastranDeck::CYLINDRICAL_COORDINATE_SYSTEM,
															 addedCoordinateSystemID);

						std::set<int> gridPointIds_WithinGeometry_set(gridPointIds_WithinGeometry.begin(), gridPointIds_WithinGeometry.end());
						nastranDeck.ModifyGridPointsDisplacementCoordinateSystemID( gridPointIds_WithinGeometry_set, addedCoordinateSystemID );

						
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						nastranDeck.AddConstraintToDeck( constraintSetID, *m, 1, 0 ); 
						if ( k->pin.axialDisplacementConstraint == ANALYSIS_CONSTRAINT_FIXED ) nastranDeck.AddConstraintToDeck( constraintSetID, *m, 3, 0 ); 
						if ( k->pin.axialRotationConstraint == ANALYSIS_CONSTRAINT_FIXED ) nastranDeck.AddConstraintToDeck( constraintSetID, *m, 2, 0 ); 
					}
				}

				///////////////////////
				// Ball
				///////////////////////
				if( k->analysisBallDefined )
				{
					// if ( k->geometry.geometryType != CAD_GEOMETRY_SPHERE_SURFACE )
					if ( k->geometry.features.begin()->geometryType != CAD_GEOMETRY_SPHERE )
					
					{
						std::stringstream errorString;
						errorString <<
							"Function - CreateFEADeck, when defining a Ball constrait, the geometry type must be " << CADGeometryType_string(CAD_GEOMETRY_SPHERE)  << std::endl <<
							// "Geometry Type: " << CADGeometryType_string(k->geometry.geometryType);
						    "Geometry Type: " << CADGeometryType_string(k->geometry.features.begin()->geometryType);
						throw isis::application_exception(errorString.str());	
					}

						std::vector< isis_CADCommon::Point_3D >  geometry3DPoints;

						//for ( std::list<std::string>::const_iterator l( k->geometry.features.begin());
						//		l != k->geometry.features.end();
						//		++l )
						for ( std::list<CADFeature>::const_iterator l( k->geometry.features.begin()->features.begin());
								l != k->geometry.features.begin()->features.end();
								++l )
						{
							isis::CADPoint  point;
							//isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
							//										k->geometry.componentID,
							//										in_CADComponentData_map,
							//										*l,  // Datum Point Name
							//										point);
							isis::RetrieveDatumPointCoordinates(	in_AssemblyComponentID,
																	l->componentID,
																	in_CADComponentData_map,
																	l->datumName,  // Datum Point Name
																	point);
							geometry3DPoints.push_back( isis_CADCommon::Point_3D( point.x, point.y, point.z));
							++numberPolygonPoints;
						}

						if ( geometry3DPoints.size() != 2 )
						{
							std::stringstream errorString;
							errorString <<
							"Function - CreateFEADeck, when defining a Ball constrait, the defining geometry must have 2 Datum Points " << std::endl <<
							//"Geometry Type: " << CADGeometryType_string(k->geometry.geometryType) << std::endl <<
							//"Component Instance ID: " << k->geometry.componentID;
							NoGridPointsWithInMesh_ErrorSstring(k->geometry.features.begin()->features);
							throw isis::application_exception(errorString.str());	
						}

						int addedCoordinateSystemID;
						
						// Need a point other than geometry3DPoints[1]
						std::vector<double>  vector_Z;
						vector_Z.push_back(geometry3DPoints[1].x - geometry3DPoints[0].x );
						vector_Z.push_back(geometry3DPoints[1].y - geometry3DPoints[0].y );
						vector_Z.push_back(geometry3DPoints[1].z - geometry3DPoints[0].z );

						std::vector<double>  vector_Temp;
						vector_Temp.push_back( vector_Z[0] + 10 );
						vector_Temp.push_back( vector_Z[1]);
						vector_Temp.push_back( vector_Z[2] );

						std::vector<double>  vector_YZ = isis_CADCommon::CrossProduct_3D(vector_Z, vector_Temp);

						isis_CADCommon::Point_3D point_YZ( 
											vector_YZ[0] +  geometry3DPoints[0].x,
											vector_YZ[1] +  geometry3DPoints[0].y,
											vector_YZ[2] +  geometry3DPoints[0].z);


						nastranDeck.AddCylindricalOrSphericalCoordinateSystem(   geometry3DPoints[0], // orgin
															 geometry3DPoints[1],  // Z axis
															 point_YZ,  // point in z x plane
															 1,
															 isis_CADCommon::NastranDeck::SPHERICAL_COORDINATE_SYSTEM,
															 addedCoordinateSystemID);

						std::set<int> gridPointIds_WithinGeometry_set(gridPointIds_WithinGeometry.begin(), gridPointIds_WithinGeometry.end());
						nastranDeck.ModifyGridPointsDisplacementCoordinateSystemID( gridPointIds_WithinGeometry_set, addedCoordinateSystemID );

						
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						nastranDeck.AddConstraintToDeck( constraintSetID, *m, 1, 0 ); 
					}
				} //


			} // END for ( std::list<AnalysisConstraint>::const_iterator k(j->analysisConstraints.begin());

			//////////////////////////
			// Add Loads
			//////////////////////////

			double ambientTemperature_temp = 0.0;
			bool   ambientTemperature_temp_defined = false;

			//std::cout << std::endl << "Before  for ( std::list<AnalysisLoad>::const_iterator , count: " << j->analysisLoads.size();
			for ( std::list<AnalysisLoad>::const_iterator k(j->analysisLoads.begin());
				  k != j->analysisLoads.end();
				  ++ k)
			{
	
				std::vector<isis_CADCommon::Point_3D>   polygon;
				int numberPolygonPoints = 0;

				//////////////////////////////
				// Get points within geometry
				//////////////////////////////
				std::vector<int>		gridPointIds_WithinGeometry;

				if	(	k->forceDefined ||   
						k->pressureDefined || 
					(	k->gridPointInitialTemperatureDefined &&  k->geometry.features.size() > 0 ||
						k->gridPointTemperatureDefined ||
						k->heatFluxDefined))  // Geometry does not apply to acceleration			
				{
					GetGridPointsWithinAnalysisGeometry( in_AssemblyComponentID, 
														 in_CADComponentData_map,
														 k->geometry,
														 gridPoints_map,
														 precision,
														 gridPointIds_WithinGeometry);


					if ( gridPointIds_WithinGeometry.size() == 0 && !k->gridPointInitialTemperatureDefined )
					{
						//std::string TempError = NoGridPointsWithInMesh_ErrorSstring(  k->geometry.componentID, 
						//								in_CADComponentData_map[k->geometry.componentID].name, 
						//								 k->geometry.features );

						std::stringstream errorString;
						errorString <<  "Function - CreateFEADeck, Failed to find FEA grid points within the geometry defined by:" << std::endl <<
										NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features ); 
						throw isis::application_exception(errorString.str());					
					}
				}

				//std::cout << std::endl << "Points within polygon, count: " << gridPointIds_WithinPolygon.size();

				//////////////////////////////////////////
				// Add Forces to Nastran Deck
				//////////////////////////////////////////
				if( k->forceDefined )
				{
					double numberPoints = gridPointIds_WithinGeometry.size();
					double force_x = k->force.force.x / numberPoints;
					double force_y = k->force.force.y / numberPoints;
					double force_z = k->force.force.z / numberPoints;

					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						nastranDeck.AddForceToDeck( loadSetID, *m, 1, 1, force_x,
																  force_y,
																  force_z);
					}
				}


				if( k->pressureDefined )
				{

					std::set<int> gridPointIds_WithinGeometry_set(gridPointIds_WithinGeometry.begin(),gridPointIds_WithinGeometry.end()); 

					// Normally this logging would be commented out.  Only for debugging purposes
					//isis_LOG(lg, isis_FILE, isis_INFO) << "Grid Points (IDs) Defining Pressure Load: ";
					//for each ( const int &g_id in gridPointIds_WithinGeometry_set )
					//{
					//	isis_LOG(lg, isis_FILE, isis_INFO) << "   " << g_id;
					//}

					std::multimap< int, std::vector<int>> elementID_to_SurfacePoints_map;

					nastranDeckHelper.getSurfaceElementsContainingGridPoints( gridPointIds_WithinGeometry_set, elementID_to_SurfacePoints_map );

					// Normally this logging would be commented out.  Only for debugging purposes
					//isis_LOG(lg, isis_FILE, isis_INFO) << "Element Surfaces containing grid points, elementID_to_SurfacePoints_map: ";
					//for each ( const std::pair< int, std::vector<int>> &id_pts in elementID_to_SurfacePoints_map )
					//{
					//	isis_LOG(lg, isis_FILE, isis_INFO) << "   Element ID: " << id_pts.first;
					//
					//	for each ( const int ptIDs in id_pts.second )
					//	{
					//		isis_LOG(lg, isis_FILE, isis_INFO) << "      Point ID: " << ptIDs;
					//	}
					//}

					if ( elementID_to_SurfacePoints_map.size() == 0  )
					{
						std::stringstream errorString;
						errorString <<  "Function - " << std::string(__FUNCTION__) << ", Failed to find FEA elements within the geometry defined by:" << std::endl <<
										NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features ); 
						throw isis::application_exception(errorString.str());					
					}


					for each ( const std::pair<int, std::vector<int>> &Elem_to_SurfPnts in elementID_to_SurfacePoints_map )
					{

						const std::map<int, isis_CADCommon::FEAElement> &elementData_ref = nastranDeck.getElementData();

						std::map<int, isis_CADCommon::FEAElement>::const_iterator elem_itr = elementData_ref.find(Elem_to_SurfPnts.first);

						if ( elem_itr == elementData_ref.end() )
						{	
							std::stringstream errorString;
							errorString <<
								"Function - " + std::string(__FUNCTION__) +
								" element not found in \"std::map<int, FEAElement> elementData\".  Elemenet ID: " << Elem_to_SurfPnts.first;
							throw isis::application_exception(errorString.str().c_str());
						}
						
						int anchorPoint;
						int diagonalPoint;

						elem_itr->second.getDiagonalPointForSurface( Elem_to_SurfPnts.second, anchorPoint, diagonalPoint );

						nastranDeck.AddPressureToDeck( loadSetID, Elem_to_SurfPnts.first, k->pressure.value, anchorPoint, diagonalPoint );
					}	
				}

				if( k->accelerationDefined )
				{
					nastranDeck.AddAccelerationToDeck(loadSetID, 1, 1,	k->acceleration.direction.x, 
																		k->acceleration.direction.y,
																		k->acceleration.direction.z);
				}
		

				if ( k->gridPointInitialTemperatureDefined )
				{
					if ( gridPointIds_WithinGeometry.size() == 0 )
					{
						// This is the TEMPD setting
						nastranDeck.AddCardToCaseControl( "TEMPERATURE(INITIAL) = " +  isis_CADCommon::IntegerToString(temperatureInitialSetID));
						isis_CADCommon::TEMPD tempD;
						tempD.SID = temperatureInitialSetID;
						stringstream ss_temp;
						ss_temp << k->gridPointInitialTemperature.value;
						tempD.T = ss_temp.str();
						//tempD.T = boost::lexical_cast<string>(k->gridPointInitialTemperature.value);
						nastranDeck.AddTemperature_TEMPD(tempD);				
					}
					else
					{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", not currently supporting initial temperature of specified grid points.";
							throw isis::application_exception(errorString.str());	

					}
				}

				// gridPointTemperatureDefined
				if ( k->gridPointTemperatureDefined )
				{
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						// Add SPC card
						nastranDeck.AddConstraintToDeck( constraintSetID, *m, 
														true,
														false,
														false,
														k->gridPointTemperature.value, 
														0.0,
														0.0);
						// Add Temp card
						isis_CADCommon::TEMP temp;
						temp.G1 = *m;
						
						stringstream ss_temp;
						ss_temp << k->gridPointTemperature.value;
						temp.T1 = ss_temp.str();
						//temp.T1 = boost::_cast<string>(k->gridPointTemperature.value);
						temp.SID = temperatureInitialSetID;
						nastranDeck.AddTemperature_TEMP(temp );
					}

				}

				//  heatFluxDefined
				if ( k->heatFluxDefined )
				{
					std::set<int> gridPointIDs_set;
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						gridPointIDs_set.insert(*m);
					}

					std::multimap< int, std::vector<int>> elementID_to_SurfacePoints_map;

					nastranDeckHelper.getSurfaceElementsContainingGridPoints(gridPointIDs_set, elementID_to_SurfacePoints_map);

					if ( elementID_to_SurfacePoints_map.size() == 0 )
					{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", for HeatFlux specification, no surface elements found.  Surface elements not found for the grid points defined by the features. "	<<
							NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features); 
							throw isis::application_exception(errorString.str());	
					}

					for each ( const std::pair< int, std::vector<int>> &i_elem_surf in elementID_to_SurfacePoints_map)
					{
						// QBDY3 cards
						isis_CADCommon::QBDY3 temp_q;
						
						//temp_q.SID = loadStatementID;
						temp_q.SID = loadSetID;

						stringstream ss_temp;
						ss_temp << k->heatFlux.value;
						temp_q.Q0 = ss_temp.str();
						//temp_q.Q0 =   boost::lexical_cast<string>(k->heatFlux.value);
						temp_q.EID1 = ++nastranDeck.createdElementCounter;

						nastranDeck.AddHeatFlux_QBDY3(temp_q);

						// CHBDYG  

						isis_CADCommon::CHBDYG temp_c;
						temp_c.EID = temp_q.EID1;
						temp_c.TYPE = "AREA" +  isis_CADCommon::IntegerToString(i_elem_surf.second.size());
						int counter = 1;
						for each ( const int &i_surf in i_elem_surf.second)
						{
							     if (counter == 1 ) { temp_c.G1 = i_surf; ++counter; } 
							else if (counter == 2 ) { temp_c.G2 = i_surf; ++counter; } 
							else if (counter == 3 ) { temp_c.G3 = i_surf; ++counter; } 
							else if (counter == 4 ) { temp_c.G4 = i_surf; ++counter; } 
							else if (counter == 5 ) { temp_c.G5 = i_surf; ++counter; } 
							else if (counter == 6 ) { temp_c.G6 = i_surf; ++counter; } 
							else if (counter == 7 ) { temp_c.G7 = i_surf; ++counter; } 
							else if (counter == 8 ) { temp_c.G8 = i_surf; ++counter; } 
						}
						
						nastranDeck.AddSurfaceElement_CHBDYG(temp_c);
					}

				} // if ( k->heatFluxDefined )

				if ( k->ambientTemperatureDefined )
				{
					// This might be used in the constraint section for convection
					ambientTemperature_temp = k->ambientTemperature.value;
					ambientTemperature_temp_defined = true;

				}

				if (k->heatGenerationDefined )
				{
					// For heatGeneration, no features would be defined and thus no grid points would have been queried.
					// The heat generation applies to a part.  Applying to an assembly is not allowed.
					// The steps follow:
					//		1. Find all elements for the PSolids, the MatId represents a PSolid
					//		2. Compute the volume of all the elments
					//		3. temp_QVOL.QVOL_Value =  k->heatGeneration.value / volume-all-elements
					//		4. Create the QVOL objects
					//		5. Add the QVOL objects to the Nastran Deck
					isis_CADCommon::QVOL temp_QVOL;
					temp_QVOL.SID = loadSetID;

					std::vector<int> pSolidIDs;
					std::vector<int> elementIDs;
					
					for each ( const std::string  &i_CompId in k->geometry.GeometryPerEntireComponent_ComponentInstanceIDs )
					{
						std::map<std::string, std::string>::const_iterator compToMatID_iter = compnentID_to_NastranMaterialID_map.find(i_CompId);
						if ( compToMatID_iter == compnentID_to_NastranMaterialID_map.end() )
						{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", for HeatGeneration could not find the PSolid for ComponentInstanceID:  "	<< i_CompId;
							throw isis::application_exception(errorString.str());	
						}
						pSolidIDs.push_back(atoi(compToMatID_iter->second.c_str()));
					}	

					nastranDeck.FindElementsFromPSolids(pSolidIDs, elementIDs);

					if ( elementIDs.size() == 0 )
					{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", for HeatGeneration no elements found for PSolids:";
							for each ( const int &i_psolid in pSolidIDs ) errorString << " " <<  i_psolid;
							throw isis::application_exception(errorString.str());
					}

					double totalVolume = nastranDeck.VolumeOfPSolids(pSolidIDs);

					if ( totalVolume == 0.0 )
					{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", for HeatGeneration volume of PSolids == 0.0, the PSolids must have a volume, PSolids:";
							for each ( const int &i_psolid in pSolidIDs ) errorString << " " <<  i_psolid;
							throw isis::application_exception(errorString.str());
					}

					stringstream volume_temp;
					volume_temp <<  k->heatGeneration.value / totalVolume;;
					temp_QVOL.QVOL_Value = volume_temp.str();

					// Add QVOL cards
					// Five element IDs per QVOL card
					int elementCountPerCard = 1;
					char buffer_temp[32];
					for each ( const int &i_elemID in elementIDs )
					{
						switch ( elementCountPerCard )
						{
							case 1: temp_QVOL.EID1 = i_elemID; break;
							case 2: temp_QVOL.EID2 = i_elemID; break;
							case 3: temp_QVOL.EID3 = i_elemID; break;
							case 4: temp_QVOL.EID4 = i_elemID; break;
							case 5: temp_QVOL.EID5 = i_elemID; break;
							default:
								std::stringstream errorString;
								errorString <<
								"Function - " << __FUNCTION__ << ", for HeatGeneration elementCountPerCard received invalid value, value:" << elementCountPerCard;
								throw isis::application_exception(errorString.str());
						}

						if (elementCountPerCard == 5 )
						{
							nastranDeck.AddHeatVolume_QVOL(temp_QVOL);
							temp_QVOL.EID1 = 0;
							temp_QVOL.EID2 = 0;
							temp_QVOL.EID3 = 0;
							temp_QVOL.EID4 = 0;
							temp_QVOL.EID5 = 0;
							elementCountPerCard = 1;
						}
						else
						{
							++elementCountPerCard;
						}
					}
					if ( elementCountPerCard != 1 ) nastranDeck.AddHeatVolume_QVOL(temp_QVOL);
				} // END if (k->heatGenerationDefined )
			} // END for ( std::list<AnalysisConstraint>::const_iterator k(j->analysisConstraints.begin());

			///////////////////////////////
			// Add the thermal constraints
			///////////////////////////////
			for ( std::list<AnalysisConstraint>::const_iterator k(j->analysisConstraints.begin());
				  k != j->analysisConstraints.end();
				  ++ k)
			{
				if ( !k->convectionBoundaryDefined ) continue;  // Only handling thermal convection

				std::vector<isis_CADCommon::Point_3D>   polygon;
				int numberPolygonPoints = 0;

				std::vector<int>		gridPointIds_WithinGeometry;

				//////////////////////////////
				// Get points within geometry
				//////////////////////////////
				GetGridPointsWithinAnalysisGeometry( in_AssemblyComponentID, 
													 in_CADComponentData_map,
													 k->geometry, 
													 gridPoints_map,
													 precision,
													 gridPointIds_WithinGeometry);

				if ( gridPointIds_WithinGeometry.size() == 0 )
				{
					//std::string TempError = NoGridPointsWithInMesh_ErrorSstring(  k->geometry.componentID, 
					//								in_CADComponentData_map[k->geometry.componentID].name, 
					//								 k->geometry.features );
					// qqqqq
					std::string TempError = NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features); 

					throw isis::application_exception(TempError.c_str());					
				}

				if( k->convectionBoundaryDefined )
				{

					if ( !ambientTemperature_temp_defined )
					{
						std::stringstream errorString;
						errorString <<
						"Function - " << __FUNCTION__ << ", No ambient temperature set. Convection setting requires an ambient temperature.";
						throw isis::application_exception(errorString.str());
						
					}

					int gridPointID = nastranDeck.getNextGridPointID();

					// SPOINT,145
					isis_CADCommon::SPOINT temp_sp;
					temp_sp.ID = gridPointID;
					nastranDeck.AddPoint_SPOINT(temp_sp);

					// SPC Card e.g. SPC,1,145,1,297.
					nastranDeck.AddConstraintToDeck( constraintSetID, gridPointID, 
														true,
														false,
														false,
														ambientTemperature_temp,
														0.0,
														0.0);
					// MAT4,1001,,,,100.
					isis_CADCommon::MAT4 temp_m4;
					int materialID = ++nastranDeck.createdElementCounter;
					temp_m4.MID = materialID;
						
					stringstream ss_temp;
					ss_temp << k->convectionBoundary.convectionCoefficient;
					temp_m4.H = ss_temp.str();
					//temp_m4.H = boost::lexical_cast<string>(k->convectionBoundary.convectionCoefficient); 
					nastranDeck.AddMaterialData_MAT4(temp_m4);

					//PCONV,1,1001,0,0.
					isis_CADCommon::PCONV temp_pconv;
					temp_pconv.MID = materialID;
					int pconid = ++nastranDeck.createdElementCounter;
					temp_pconv.PCONID = pconid;
					temp_pconv.FORM = "0";
					temp_pconv.EXPF = "0.";
					nastranDeck.AddConvection_PCONV(temp_pconv);

					std::set<int> gridPointIDs_set;
					for ( std::vector<int>::const_iterator m( gridPointIds_WithinGeometry.begin()); m != gridPointIds_WithinGeometry.end(); ++m)
					{
						gridPointIDs_set.insert(*m);
					}

					std::multimap< int, std::vector<int>> elementID_to_SurfacePoints_map;

					nastranDeckHelper.getSurfaceElementsContainingGridPoints(gridPointIDs_set, elementID_to_SurfacePoints_map);

					if ( elementID_to_SurfacePoints_map.size() == 0 )
					{
							std::stringstream errorString;
							errorString <<
							"Function - " << __FUNCTION__ << ", for HeatFlux specification, no surface elements found.  Surface elements not found for the grid points defined by the features. "	<<
							NoGridPointsWithInMesh_ErrorSstring(  k->geometry.features.begin()->features); 

							throw isis::application_exception(errorString.str());	
					}

					for each ( const std::pair< int, std::vector<int>> &i_elem_surf in elementID_to_SurfacePoints_map)
					{

						//CONV,100002,1,0,0,145
						isis_CADCommon::CONV temp_conv;
						int conid = ++nastranDeck.createdElementCounter;
						temp_conv.EID = conid;
						temp_conv.PCONID = pconid;
						temp_conv.FLMND = "0";
						temp_conv.CNTRLND = "0";
						temp_conv.TA1 = temp_sp.ID;
						nastranDeck.AddConvection_CONV(temp_conv);

						// CHBDYG  

						isis_CADCommon::CHBDYG temp_c;
						temp_c.EID = conid;
						temp_c.TYPE = "AREA" +  isis_CADCommon::IntegerToString(i_elem_surf.second.size());
						int counter = 1;
						for each ( const int &i_surf in i_elem_surf.second)
						{
							     if (counter == 1 ) { temp_c.G1 = i_surf; ++counter; } 
							else if (counter == 2 ) { temp_c.G2 = i_surf; ++counter; } 
							else if (counter == 3 ) { temp_c.G3 = i_surf; ++counter; } 
							else if (counter == 4 ) { temp_c.G4 = i_surf; ++counter; } 
							else if (counter == 5 ) { temp_c.G5 = i_surf; ++counter; } 
							else if (counter == 6 ) { temp_c.G6 = i_surf; ++counter; } 
							else if (counter == 7 ) { temp_c.G7 = i_surf; ++counter; } 
							else if (counter == 8 ) { temp_c.G8 = i_surf; ++counter; } 
						}
						
						nastranDeck.AddSurfaceElement_CHBDYG(temp_c);
					}


				}

			} // End thermal constraints, for ( std::list<AnalysisConstraint

		} // END for ( list<AnalysisFEA>::const_iterator j(in_CADAnalyses.analysisFEA.begin());
		nastranDeck.WriteNastranDeck( MeshModified_PathAndFileName);

	} // END for ( std::list<AnalysisFEA>::const_iterator i(in_CADAnalyses.analysisFEA.begin()); i != in_CADAnalyses.analysisFEA.end(); ++i )
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-Conditions
//	in_TopLevelAssemblyData.CADAnalyses.list<AnalysisFEA> can contain only one item.  Multiple Analysis
//  per assembly are not currently supported.
//
void CreateXMLFile_FEAPostProcessingParameters( 
						const std::string									&in_PathAndFileName,
						const TopLevelAssemblyData							&in_TopLevelAssemblyData,
						std::map<std::string, Material>						&in_Materials,
						std::map<std::string, std::string>                  &in_NastranMaterialID_to_CompnentID_map,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	// Example XML file
	//
	//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	//<Components ConfigurationID="100000452" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">
	//	<Component ComponentID="0" FEAElementID="PSOLID_1" _id="id78">
	//		<Material Bearing="165" Mises="165" Shear="82.64999999999999" Units="MPa" _id="id79"/>
	//		<Metrics _id="id7a">
	//			<Metric MetricID="id-0066-00005242" Type="Mises" _id="id7b"/>
	//			<Metric MetricID="id-0066-00005243" Type="Shear" _id="id7c"/>
	//			<Metric MetricID="id-0066-00005244" Type="Bearing" _id="id7d"/>
	//			<Metric MetricID="id-0066-00005245" Type="FactorOfSafety" _id="id7e"/>
	//		</Metrics>
	//  </Component>
	//</Components>
	

	std::map<std::string, std::string> ComponentID_to_NastranMaterialID_map;

	for ( std::map<std::string, std::string>::const_iterator ii( in_NastranMaterialID_to_CompnentID_map.begin());
		  ii != in_NastranMaterialID_to_CompnentID_map.end(); ++ii)
	{
		ComponentID_to_NastranMaterialID_map[ii->second] = ii->first;
	}

	//std::cout << std::endl << "in_FileName: " << in_FileName;

	Udm::SmartDataNetwork dn_FEAPostProcessingParameters( CADPostProcessingParameters::diagram );
	dn_FEAPostProcessingParameters.CreateNew( in_PathAndFileName, "CADPostProcessingParameters", CADPostProcessingParameters::Components::meta);
	
	try
	{
		CADPostProcessingParameters::Components componentsRoot = 
			      CADPostProcessingParameters::Components::Cast(dn_FEAPostProcessingParameters.GetRootObject());

		componentsRoot.ConfigurationID() = in_TopLevelAssemblyData.configurationID;
	
		// Outer loop - CADAnalysisComponents  ( std::list<CADAnalysisComponent>
		// We can loop through via the following function because list<AnalysisFEA> analysisFEA can only contain 
		// one item (i.e. multiple analyses per assembly are currently not supported.

		std::map<std::string, std::list<CADComputation>> componentID_to_ListofComputations_map;
		std::set<std::string> componentIDs_set;


		for each ( AnalysisFEA i in in_TopLevelAssemblyData.analysesCAD.analysesFEA )
		{
			OrganizeMetricsBasedOnComponentIDs(	i.metrics,
												componentID_to_ListofComputations_map,
												componentIDs_set );
		}


		//for (std::list<isis::CADAnalysisComponent>::const_iterator i(in_TopLevelAssemblyData.analysesCAD.analysesFEA.begin()->cADAnalysisComponents.begin());
		//i != in_TopLevelAssemblyData.analysesCAD.analysesFEA.begin()->cADAnalysisComponents.end();
		//++i )
		for each ( const std::string i in componentIDs_set)	
		{
			if ( in_CADComponentData_map[i].modelType == PRO_MDL_PART )
			{
				/////////////////////////////////////////////////////////////////////
				// Add ComponentID and FEAElementID
				// e.g. <Component ComponentID="100000678" FEAElementID="PSOLID_1">
				/////////////////////////////////////////////////////////////////////
				CADPostProcessingParameters::Component  componentRoot = CADPostProcessingParameters::Component::Create( componentsRoot );
				componentRoot.ComponentInstanceID() = i;
				componentRoot.FEAElementID() = "PSOLID_" + ComponentID_to_NastranMaterialID_map[i];
		
				/////////////////////////////////////////////////////////////////////
				// Add Material Properties
				// e.g <Material Mises="165" Bearing="165" Shear="82.65" Units="MPa"/>
				/////////////////////////////////////////////////////////////////////

				// zzz Fix this later to retrive num of cycles
			
				//in_CADComponentData_map[i->componentID]
				//if ( i->infiniteCycle ) 
				//		numberOfCycles = isis::c_InifiteLifeNumberOfCycles;
				//else
				//	numberOfCycles = 1;

				int numberOfCycles;
				numberOfCycles = isis::c_InifiteLifeNumberOfCycles;

				isis::AnalysisMaterialProperties_Allowables  materialPropertiesAllowables; 
				//std::string tempMatierialID = in_CADComponentData_map[i].materialID_FromInputXML;

				std::string tempMatierialID = in_CADComponentData_map[i].materialID_FromCreoPart;

				isis_LOG(lg, isis_FILE, isis_INFO) << "Material Info: ";
				isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Instance ID: "  << i;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Name:        "  <<  in_CADComponentData_map[i].name;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   MaterialId:            "  <<  tempMatierialID;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   in_CADComponentData_map[i].materialID_FromCreoPart: " << in_CADComponentData_map[i].materialID_FromCreoPart;

				isis::ComputeAllowableStressLevels(	numberOfCycles,in_Materials[tempMatierialID].analysisMaterialProperties, 
											materialPropertiesAllowables );

				CADPostProcessingParameters::Material materialRoot = CADPostProcessingParameters::Material::Create( componentRoot );

				materialRoot.Mises()   = materialPropertiesAllowables.tensileStrength;
				materialRoot.Bearing() = materialPropertiesAllowables.bearingStrength;
				materialRoot.Shear()   = materialPropertiesAllowables.shearStrength;
				materialRoot.Units()   = "MPa";

				/////////////////////////////////////////////////////////////////////
				// Add Metrics
				// e.g   <Metrics>
				//			<Metric MetricID="m1" Type="Shear"/>
				//			<Metric MetricID="m2" Type="Bearing"/>
				//			<Metric MetricID="m3" Type="Mises"/>
				//			<Metric MetricID="m4" Type="FactorOfSafety"/>
				//		</Metrics>
				/////////////////////////////////////////////////////////////////////
				//if ( i->cadAnalysisComponentMetrics.size() > 0 )
				if (componentID_to_ListofComputations_map[i].size() > 0 )
				{
					CADPostProcessingParameters::Metrics metricsRoot = CADPostProcessingParameters::Metrics::Create( componentRoot );
	
					//for (std::list<isis::CADAnalysisComponentMetrics>::const_iterator j(i->cadAnalysisComponentMetrics.begin());
					//	j != i->cadAnalysisComponentMetrics.end(); ++j )
					for each ( const CADComputation j in componentID_to_ListofComputations_map[i] )
					{
						CADPostProcessingParameters::Metric metricRoot = CADPostProcessingParameters::Metric::Create( metricsRoot );
						metricRoot.MetricID() = j.metricID;
						metricRoot.Type() =  ComputationType_string(j.computationType);
					}
				}
			}
		}  // end for i loop

		///////////////////////////
		// Write XML File
		//////////////////////////
		dn_FEAPostProcessingParameters.CloseWithUpdate();
	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		dn_FEAPostProcessingParameters.CloseNoUpdate();
		throw;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEA_AnalysisMetaData_Add_SubAssemblies_Parts( 
						const std::string									&in_ComponentInstanceID,
						std::map<std::string, std::string>					in_componentID_to_NastranMaterialID_map,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
						CADAnalysisMetaData::Component						&out_ComponentRoot )
																	throw (isis::application_exception)
{
	bool NastranMaterialIDsSpecified = true;
	if ( in_componentID_to_NastranMaterialID_map.size() == 0 ) NastranMaterialIDsSpecified = false;

	out_ComponentRoot.Name() = in_CADComponentData_map[in_ComponentInstanceID].name;
	out_ComponentRoot.ComponentInstanceID() = in_CADComponentData_map[in_ComponentInstanceID].componentID;
	out_ComponentRoot.MaterialID() = in_CADComponentData_map[in_ComponentInstanceID].materialID_FromCreoPart;

	if ( in_CADComponentData_map[in_ComponentInstanceID].modelType == PRO_MDL_PART )
	{
		out_ComponentRoot.Type() = "PART";

		if ( NastranMaterialIDsSpecified )
		{
			out_ComponentRoot.FEAElementType() =  "TETRA_10_NODE";
			out_ComponentRoot.FEAElementID() = "PSOLID_" + in_componentID_to_NastranMaterialID_map[in_ComponentInstanceID]; 
		}
		else
		{
			// We don't know if a TETRA_4_NODE, TETRA_10_NODE, or surface model.  For surfaces, refactoring will be needed.
			out_ComponentRoot.FEAElementType() =  "";

			if ( in_CADComponentData_map[in_ComponentInstanceID].partOrdinal < 1 )
			{
				// This should normally not occur, but possibly could occur when parts not known to CyPhy (e.g. parts in a leaf sub-assembly)
				// are supported in FEA mode.
				std::stringstream errorString;
				errorString <<
				"Function - " << __FUNCTION__ << " Incorrect part ordinal (i.e. order the part was added into the assembly)."
				<< "  This could occur for parts not known to CyPhy (i.e. part in a leaf assembly). Ordinal: " <<  in_CADComponentData_map[in_ComponentInstanceID].partOrdinal;
				throw isis::application_exception(errorString.str());	
			}

			out_ComponentRoot.FEAElementID() = "PSOLID_" + boost::lexical_cast<string>(in_CADComponentData_map[in_ComponentInstanceID].partOrdinal); 
		}
	}
	else
	{
		out_ComponentRoot.Type() = "ASSEMBLY";
		out_ComponentRoot.FEAElementType() =  "";
		out_ComponentRoot.FEAElementID() = "";

		for each ( std::string i in in_CADComponentData_map[in_ComponentInstanceID].children )
		{
			CADAnalysisMetaData::Component componentRoot = CADAnalysisMetaData::Component::Create(out_ComponentRoot);
			FEA_AnalysisMetaData_Add_SubAssemblies_Parts(	i,
															in_componentID_to_NastranMaterialID_map,
															in_CADComponentData_map,
															componentRoot);

		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-Conditions
//	in_TopLevelAssemblyData.CADAnalyses.list<AnalysisFEA> can contain only one item.  Multiple Analysis
//  per assembly are not currently supported.
//
void CreateXMLFile_FEA_AnalysisMetaData( 
						const std::string									&in_PathAndFileName,
						const TopLevelAssemblyData							&in_TopLevelAssemblyData,
						std::map<std::string, Material>						&in_Materials,
						std::map<std::string, std::string>                  &in_NastranMaterialID_to_CompnentID_map,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	std::map<std::string, std::string> ComponentID_to_NastranMaterialID_map;

	for ( std::map<std::string, std::string>::const_iterator ii( in_NastranMaterialID_to_CompnentID_map.begin());
		  ii != in_NastranMaterialID_to_CompnentID_map.end(); ++ii)
	{
		ComponentID_to_NastranMaterialID_map[ii->second] = ii->first;
	}

	//std::cout << std::endl << "in_FileName: " << in_FileName;

	Udm::SmartDataNetwork dn_AnalysisMetaData( CADAnalysisMetaData::diagram );  

	
	try
	{
		dn_AnalysisMetaData.CreateNew( in_PathAndFileName, "CADAnalysisMetaData", CADAnalysisMetaData::CADAnalysisMetaData::meta);
		CADAnalysisMetaData::CADAnalysisMetaData cADAnalysisMetaDataRoot = CADAnalysisMetaData::CADAnalysisMetaData::Cast(dn_AnalysisMetaData.GetRootObject());

		CADAnalysisMetaData::Materials materialsRoot = CADAnalysisMetaData::Materials::Create(cADAnalysisMetaDataRoot);

		//std::map<std::string, Material>						&in_Materials,
		for each ( std::pair<std::string, Material> i in in_Materials )
		{
			CADAnalysisMetaData::Material materialRoot = CADAnalysisMetaData::Material::Create(materialsRoot);
			materialRoot.MaterialID() = i.second.materialName;

			int numberOfCycles = 0;
			isis::AnalysisMaterialProperties_Allowables  materialPropertiesAllowables; 
			isis::ComputeAllowableStressLevels(	numberOfCycles,i.second.analysisMaterialProperties, 
										materialPropertiesAllowables );

			CADAnalysisMetaData::MaterialProperties materialPropertiesRoot = CADAnalysisMetaData::MaterialProperties::Create(materialRoot);

			CADAnalysisMetaData::AllowableTensileStress allowableTensileStressRoot = CADAnalysisMetaData::AllowableTensileStress::Create(materialPropertiesRoot);		
			allowableTensileStressRoot.Value() = materialPropertiesAllowables.tensileStrength;
			allowableTensileStressRoot.Units() = "MPa";
			allowableTensileStressRoot.Source() = "EMPERICAL";  // fffff fix this


			CADAnalysisMetaData::AllowableShearStress allowableShearStressRoot = CADAnalysisMetaData::AllowableShearStress::Create(materialPropertiesRoot);		
			allowableShearStressRoot.Value() = materialPropertiesAllowables.shearStrength;
			allowableShearStressRoot.Units() = "MPa";
			allowableShearStressRoot.Source() = "EMPERICAL";  // fffff fix this

			CADAnalysisMetaData::AllowableBearingStress allowableBearingStressRoot = CADAnalysisMetaData::AllowableBearingStress::Create(materialPropertiesRoot);		
			allowableBearingStressRoot.Value() = materialPropertiesAllowables.bearingStrength;
			allowableBearingStressRoot.Units() = "MPa";
			allowableBearingStressRoot.Source() = "EMPERICAL";  // fffff fix this

		}

		////////////////////////////
		// AnalysisSupportingData
		/////////////////////////////
		CADAnalysisMetaData::AnalysisSupportingData analysisSupportingDataRoot = CADAnalysisMetaData::AnalysisSupportingData::Create(cADAnalysisMetaDataRoot);
		analysisSupportingDataRoot.AnalysisType() = AnalysisType_string(ANALYSIS_STRUCTURAL);  // fffff fix this

		CADAnalysisMetaData::LifeCycle lifeCycleRoot = CADAnalysisMetaData::LifeCycle::Create(analysisSupportingDataRoot);
		lifeCycleRoot.Duration() = "INFINITE";  // fffff fix this

		////////////////////////////
		// Components
		/////////////////////////////

		isis::ComponentVistorBuildListOfComponentIDs  assembllyComponenteIDs_temp;
		// The following builds assembllyComponenteIDs_temp.listOfComponentIDs, where the lists includes i->assemblyComponentID
		isis::VisitComponents(in_TopLevelAssemblyData.assemblyComponentID, in_CADComponentData_map, assembllyComponenteIDs_temp );

		// Note - there will always be at least a top assembly; otherwise, an analysis run would not have made it this far.
		CADAnalysisMetaData::Assemblies assembliesRoot = CADAnalysisMetaData::Assemblies::Create(cADAnalysisMetaDataRoot);
		CADAnalysisMetaData::Assembly assemblyRoot = CADAnalysisMetaData::Assembly::Create(assembliesRoot);
		assemblyRoot.ConfigurationID() = in_TopLevelAssemblyData.configurationID;
		CADAnalysisMetaData::Component componentRoot = CADAnalysisMetaData::Component::Create(assemblyRoot);

		FEA_AnalysisMetaData_Add_SubAssemblies_Parts(	in_TopLevelAssemblyData.assemblyComponentID,
														ComponentID_to_NastranMaterialID_map,
														in_CADComponentData_map,
														componentRoot );
		/*
		// Old approach, Did not have assembly hierarchy
		CADAnalysisMetaData::Components componentsRoot = CADAnalysisMetaData::Components::Create(cADAnalysisMetaDataRoot);

		for each ( std::string j in assembllyComponenteIDs_temp.listOfComponentIDs )
		{
			CADAnalysisMetaData::Component componentRoot = CADAnalysisMetaData::Component::Create(componentsRoot);

			componentRoot.Name() = in_CADComponentData_map[j].name;
			if ( in_CADComponentData_map[j].modelType == PRO_MDL_PART )
			{
				componentRoot.Type() = "PART";
				componentRoot.FEAElementType() =  "TETRA_10_NODE";
				componentRoot.FEAElementID() = "PSOLID_" + ComponentID_to_NastranMaterialID_map[j]; 
			}
			else
			{
				componentRoot.Type() = "ASSEMBLY";
				componentRoot.FEAElementType() =  "";
				componentRoot.FEAElementID() = ""; 
			}

			componentRoot.ComponentInstanceID() = in_CADComponentData_map[j].componentID;
			componentRoot.MaterialID() = in_CADComponentData_map[j].materialID_FromCreoPart;
		}

		*/

		///////////////////////////
		// Write XML File
		//////////////////////////
		dn_AnalysisMetaData.CloseWithUpdate();
	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		dn_AnalysisMetaData.CloseNoUpdate();
		throw;
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Create_FEADecks_BatFiles( 
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					std::map<std::string, Material>						&in_Materials,
					const std::string									&in_WORKING_DIR,
					const std::string									in_ProgramName_Version_TimeStamp,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	
	

	//////////////////////////////////////
	// Create Analysis Directories
	//////////////////////////////////////
	std::string createAnalysisDir = "if not exist Analysis mkdir Analysis";
	isis::ExecuteSystemCommand( createAnalysisDir);

	// If Abaqus Directory already exists, delete it. 
	// This would be necessary because a previous job manager job was restarted.
	try 
	{
		::boost::filesystem::remove_all("Analysis\\Abaqus");
	}
	catch (...)
	{
		// In case the following error occurs, allow this program to continue.
		// Most likely the files that matter (abaqus db, deck) would be deleted even if the exception occurred.
		// ERROR: boost::filesystem::remove: The process cannot access the file because it is being used by another process: "Analysis\Abaqus"
	}

	isis::ExecuteSystemCommand( "if not exist Analysis\\Abaqus mkdir Analysis\\Abaqus");
	isis::ExecuteSystemCommand( "if not exist Analysis\\CalculiX mkdir Analysis\\CalculiX");
	isis::ExecuteSystemCommand( "if not exist Analysis\\Nastran mkdir Analysis\\Nastran");

	//////////////////////////////////////
	// Set File Name Strings
	//////////////////////////////////////
	std::string analysisDirName = "Analysis";
	std::string abaqusDirName   = "Abaqus";
	std::string caculixDirName  = "CalculiX";
	std::string nastranDirName  = "Nastran";

	std::string analysisWorkingDir = in_WORKING_DIR +     "\\" + analysisDirName;
	std::string abaqusWorkingDir   = analysisWorkingDir + "\\" + abaqusDirName;
	std::string caculixWorkingDir  = analysisWorkingDir + "\\" + caculixDirName;
	std::string nastranWorkingDir  = analysisWorkingDir + "\\" + nastranDirName;

	// Since Creo will create the Nastran deck, the name of the created file cannot exceed 31 characters.
	// Therefore, allot 23 characters plus the suffix of 8 charaters (i.e. _Nas_org or _Nas_mod)
	std::string tempMeshFilePreFix = in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name;
	if (tempMeshFilePreFix.size() > 23 ) tempMeshFilePreFix = tempMeshFilePreFix.substr(0, 23 );			

	std::string originalMeshFileName = "Nastran_orig.nas";	
	std::string modifiedMeshFileNameWithoutSuffix = "Nastran_mod";	
	std::string modifiedMeshFileName = modifiedMeshFileNameWithoutSuffix + ".nas";

	// No length restriction on the length of the Calculix deck name
	std::string meshFileName_Calculix = "CalculiX";	
    std::string abaqusPostProcessingParametersXMLFileName = "Abaqus.xml";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// The ValidateFEAAnalysisInputs line will throw an execption if the input xml for the FEA analysis contains errors.
	// Note - This fucntion inforces that only one FEA analysis per assembly is currently supported.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValidateFEAAnalysisInputs (in_TopLevelAssemblyData.configurationID, in_TopLevelAssemblyData.analysesCAD, in_CADComponentData_map );

	////////////////////////////
	// Write Abaqus bat file
	////////////////////////////

	std::ofstream  abaqusAnalysisBatFile; 
	//std::string abaqusAnalysisBatFileName =  (const std::string&)in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name + "_Abaqus.bat" ;
	std::string abaqusAnalysisBatFileName =  "runAnalysis.bat";
	std::string analysisDirectoryAndBatFileName = abaqusWorkingDir + "\\" + abaqusAnalysisBatFileName ;

	abaqusAnalysisBatFile.open (analysisDirectoryAndBatFileName.c_str(),std::ios::out | std::ios::trunc  );

	abaqusAnalysisBatFile << "REM " + in_ProgramName_Version_TimeStamp << std::endl;
	abaqusAnalysisBatFile << "REM The following system environment variable must be defined:" << std::endl;
	abaqusAnalysisBatFile << "FOR /F \"skip=2 tokens=2,*\" %%A IN ('%SystemRoot%\\SysWoW64\\REG.exe query \"HKLM\\software\\META\" /v \"META_PATH\"') DO set MetaPath=%%B" << std::endl;
	abaqusAnalysisBatFile	<< "echo off" << std::endl;
	abaqusAnalysisBatFile << "REM Invoke Abaqus" << std::endl;
	abaqusAnalysisBatFile	<< "echo." << std::endl;
	abaqusAnalysisBatFile << "echo Invoke Abaqus" << std::endl;
	abaqusAnalysisBatFile	<< std::endl;
	abaqusAnalysisBatFile << "call abaqus fromnastran job=" + (const std::string&)in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name + " input=..\\" + modifiedMeshFileName << std::endl;
	abaqusAnalysisBatFile << "call abaqus analysis interactive job=" + (const std::string&)in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name << std::endl;
	abaqusAnalysisBatFile	<< "call abaqus odbreport job=" + (const std::string&)in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name + " results" << std::endl;
	abaqusAnalysisBatFile	<< std::endl;
	abaqusAnalysisBatFile	<< "echo." << std::endl;
	abaqusAnalysisBatFile << "echo Analysis completed. See the " << in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name << ".rep for the results." << std::endl;
	abaqusAnalysisBatFile	<< "echo." << std::endl;
	abaqusAnalysisBatFile	<< std::endl;
	abaqusAnalysisBatFile << "REM Invoke Post Processing" << std::endl;
	abaqusAnalysisBatFile	<< "echo Invoking Post Processing" << std::endl;
	abaqusAnalysisBatFile	<< "REM The following line (set PYTHONPATH...) is needed so that other python scripts (e.g. ComputedMetricsSummary.py) can be located when invoking Abaqus post processing." << std::endl;
	abaqusAnalysisBatFile	<< "set PYTHONPATH=%MetaPath%\\bin\\CAD" << std::endl;
	abaqusAnalysisBatFile	<< "call abaqus cae noGUI=\"%MetaPath%\"\\bin\\CAD\\ABQ_CompletePostProcess.py";
	abaqusAnalysisBatFile	<< " -- -o " <<  in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name << ".odb" << 
		  " -p " << "..\\AnalysisMetaData.xml" << " -m " << "..\\..\\RequestedMetrics.xml" << 
		  " -j " << "..\\..\\testbench_manifest.json" << std::endl;	
	abaqusAnalysisBatFile	<< std::endl;
	abaqusAnalysisBatFile	<< "echo." << std::endl;
	abaqusAnalysisBatFile	<< "echo Post processing completed" << std::endl;
	abaqusAnalysisBatFile	<< "echo." << std::endl;
	//abaqusAnalysisBatFile	<< "set /p ExitPrompt= Type Enter to exit" << std::endl;
	abaqusAnalysisBatFile.close();

	////////////////////////////////////////////////////
	// Write "convert Nastran to CalculiX" bat file
	////////////////////////////////////////////////////
	std::ofstream  calculixBatFile_ConvertDeck; 
	std::string calculixDeckBatFileName =  "CalculiX_Deck.bat";
	analysisDirectoryAndBatFileName = caculixWorkingDir + "\\" + calculixDeckBatFileName ;

	calculixBatFile_ConvertDeck.open (analysisDirectoryAndBatFileName.c_str(),std::ios::out | std::ios::trunc  );

	calculixBatFile_ConvertDeck << "REM " + in_ProgramName_Version_TimeStamp << std::endl;
	calculixBatFile_ConvertDeck << "REM	The following system environment variable must be defined:" << std::endl;
	calculixBatFile_ConvertDeck << "REM	   PROE_ISIS_EXTENSIONS	// typically set to	C:\\Program Files\\Proe ISIS Extensions" << std::endl;
	calculixBatFile_ConvertDeck << "REM Invoke DeckConverter ( Convert Nastran deck to CalculiX deck)" << std::endl;
	calculixBatFile_ConvertDeck	<< "echo off" << std::endl;
	calculixBatFile_ConvertDeck	<< std::endl;
	calculixBatFile_ConvertDeck	<< "echo." << std::endl;
	calculixBatFile_ConvertDeck	<< "echo Invoke deck conversion from Nastran to CalculiX" << std::endl;
	calculixBatFile_ConvertDeck	<< "echo." << std::endl;
	calculixBatFile_ConvertDeck << "\"%PROE_ISIS_EXTENSIONS%\"\\bin\\DeckConverter.exe -i ..\\" + modifiedMeshFileName + " -o " + meshFileName_Calculix << std::endl;
	calculixBatFile_ConvertDeck	<< "echo." << std::endl;
	calculixBatFile_ConvertDeck	<< "echo Conversion completed" << std::endl;
	calculixBatFile_ConvertDeck	<< "echo." << std::endl;
	calculixBatFile_ConvertDeck	<< "set /p ExitPrompt= Type Enter to exit" << std::endl;
	calculixBatFile_ConvertDeck.close();

	////////////////////////////////////////////////
	// Write "invoke CalculiX" bat file
	///////////////////////////////////////////////
	std::ofstream  calculixBatFile_RunDeck; 
	std::string calculixLinuxBatFileName =  "CalculiX_LinuxRun.bat";
	analysisDirectoryAndBatFileName = caculixWorkingDir + "\\" + calculixLinuxBatFileName ;
				
	calculixBatFile_RunDeck.open (analysisDirectoryAndBatFileName.c_str(),std::ios::out | std::ios::trunc  );
	calculixBatFile_RunDeck << "REM " + in_ProgramName_Version_TimeStamp << std::endl;
	calculixBatFile_RunDeck << "REM	This bat file should be invoked on a Linux machine with CalculiX 2.3 installed:";
	calculixBatFile_RunDeck	<< std::endl;
	calculixBatFile_RunDeck << "ccx_2.3 -i " + meshFileName_Calculix;
	calculixBatFile_RunDeck.close();

	////////////////////////////
	// Write Nastran bat file
	////////////////////////////

	std::ofstream  nastranAnalysisBatFile; 
	//std::string nastranAnalysisBatFileName =  (const std::string&)in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name + "_Nastran.bat" ;
	std::string nastranAnalysisBatFileName =  "runAnalysis.bat";
	analysisDirectoryAndBatFileName = nastranWorkingDir + "\\" + nastranAnalysisBatFileName ;

	nastranAnalysisBatFile.open (analysisDirectoryAndBatFileName.c_str(),std::ios::out | std::ios::trunc  );

	nastranAnalysisBatFile << "REM Invoke Nastran Solver and Post Processing"  << std::endl;
	nastranAnalysisBatFile <<  std::endl;
	nastranAnalysisBatFile << "FOR /F \"skip=2 tokens=2,*\" %%A IN ('%SystemRoot%\\SysWoW64\\REG.exe query \"HKLM\\software\\META\" /v \"META_PATH\"') DO set MetaPath=%%B" << std::endl;
	nastranAnalysisBatFile << "set DECK_NAME=" << modifiedMeshFileName << std::endl;
	nastranAnalysisBatFile << "set MODEL_NAME=" << in_CADComponentData_map[in_TopLevelAssemblyData.assemblyComponentID].name << std::endl;
	nastranAnalysisBatFile << "set RESULTS_DB_Name=" << modifiedMeshFileNameWithoutSuffix << ".xdb" << std::endl;
	nastranAnalysisBatFile << "set NASTRAN_SOLVER_SCRIPT=\"%MetaPath%bin\\CAD\\Nastran.py\"" << std::endl;
	nastranAnalysisBatFile << "set NASTRAN_POST_PROCESSING_SCRIPT=\"%MetaPath%bin\\CAD\\Patran_PP.py\"" << std::endl;
	nastranAnalysisBatFile << "echo off" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << "pushd %~dp0" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << "REM Get location of META installed Python" << std::endl;
	nastranAnalysisBatFile << "set PythonExe=%MetaPath%\\bin\\Python27\\Scripts\\Python.exe" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << "\"%PythonExe%\"  %NASTRAN_SOLVER_SCRIPT%   ..\\%DECK_NAME%" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << "set ERROR_CODE=%ERRORLEVEL%" << std::endl;
	nastranAnalysisBatFile << "if %ERROR_CODE% EQU 0 GOTO CONTINUE_1" << std::endl;
	nastranAnalysisBatFile << "   set ERROR_MSG=\"Encountered error during execution of %NASTRAN_SOLVER_SCRIPT%, error level is %ERROR_CODE%.\"" << std::endl;
	nastranAnalysisBatFile << "   goto :ERROR_SECTION" << std::endl;
	nastranAnalysisBatFile << ":CONTINUE_1" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << "\"%PythonExe%\" %NASTRAN_POST_PROCESSING_SCRIPT% ..\\%DECK_NAME% %RESULTS_DB_Name% ..\\AnalysisMetaData.xml ..\\..\\RequestedMetrics.xml ..\\..\\testbench_manifest.json" << std::endl;
	nastranAnalysisBatFile << std::endl;	
	nastranAnalysisBatFile << "set ERROR_CODE=%ERRORLEVEL%" << std::endl;
	nastranAnalysisBatFile << "if %ERROR_CODE% EQU 0  GOTO CONTINUE_2" << std::endl;
	nastranAnalysisBatFile << "   set ERROR_MSG=\"Encountered error during execution of %NASTRAN_POST_PROCESSING_SCRIPT%, error level is %ERROR_CODE%.\"" << std::endl;
	nastranAnalysisBatFile << "   goto :ERROR_SECTION" << std::endl;
	nastranAnalysisBatFile << ":CONTINUE_2" << std::endl;
	nastranAnalysisBatFile << std::endl;	
	nastranAnalysisBatFile << "popd" << std::endl;
	nastranAnalysisBatFile << "exit 0" << std::endl;
	nastranAnalysisBatFile << std::endl;
	nastranAnalysisBatFile << ":ERROR_SECTION" << std::endl;
	nastranAnalysisBatFile << "popd" << std::endl;
	nastranAnalysisBatFile << "echo %ERROR_MSG% >>_FAILED.txt" << std::endl;
	nastranAnalysisBatFile << "echo %ERROR_MSG%" << std::endl;
	nastranAnalysisBatFile << "exit /b %ERROR_CODE%" << std::endl;
	
	nastranAnalysisBatFile.close();

	/////////////////////////////
	// Create FEA Deck
	/////////////////////////////
	std::map<std::string, std::string> NastranMaterialID_to_CompnentID_map;

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)  << "Creating finite element mesh, this could take a few minutes ...";
	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "Creating finite element mesh";
	// WARNING - Do not save the assembly/models after this point.  Doing so will save the temporarily created material.
	isis::CreateFEADeck(	in_Materials,
							in_ProgramName_Version_TimeStamp, 
							analysisWorkingDir, 
							originalMeshFileName,
							modifiedMeshFileName,
							in_TopLevelAssemblyData.configurationID,
							in_TopLevelAssemblyData.assemblyComponentID,  
							in_TopLevelAssemblyData.analysesCAD,
							in_CADComponentData_map,
							NastranMaterialID_to_CompnentID_map);

	///////////////////////////////////////////////////////////////////////////////////
	// Create XML File containing Component IDs, Material Allowables, and Metric IDs
	///////////////////////////////////////////////////////////////////////////////////
	/* 9/18/2014 The post processing file was only used by deck-based Abaqus. Deck-based now uses AnalysisMetaData.xml.
	std::string fEAPostProcessingParametersFile = abaqusWorkingDir + "\\" + abaqusPostProcessingParametersXMLFileName;
	CreateXMLFile_FEAPostProcessingParameters(	fEAPostProcessingParametersFile,
												in_TopLevelAssemblyData,
												in_Materials,
												NastranMaterialID_to_CompnentID_map,
												in_CADComponentData_map );

	*/
	///////////////////////////////////////////////////////////
	// AnalysisMetaData.xml
	//////////////////////////////////////////////////////////
	
	std::string fEAAnalysisMetaDataFile = "AnalysisMetaData.xml";
	std::string fEAAnalysisMetaDataFile_PathAndFileName = analysisWorkingDir + "\\" + fEAAnalysisMetaDataFile; 
	CreateXMLFile_FEA_AnalysisMetaData(	fEAAnalysisMetaDataFile_PathAndFileName,
												in_TopLevelAssemblyData,
												in_Materials,
												NastranMaterialID_to_CompnentID_map,
												in_CADComponentData_map );

	//std::string fEAAnalysisMetaDataFile = "AnalysisMetaData.xml";
	//std::string fEAAnalysisMetaDataFile_PathAndFileName = nastranWorkingDir + "\\" + fEAAnalysisMetaDataFile; 
	//CreateXMLFile_FEA_AnalysisMetaData(	fEAAnalysisMetaDataFile_PathAndFileName,
	//											in_TopLevelAssemblyData,
	//											in_Materials,
	//											NastranMaterialID_to_CompnentID_map,
	//											in_CADComponentData_map );




	// Modify fEAPostProcessingParametersFile to have Version and TimeStamp
	/* 9/18/2014 The post processing file was only used by deck-based Abaqus. Deck-based now uses AnalysisMetaData.xml.
	std::ofstream  postProcessingParametersFile; 
	postProcessingParametersFile.open( fEAPostProcessingParametersFile, ios::app );
	postProcessingParametersFile << std::endl;
	postProcessingParametersFile << "<!--  " + in_ProgramName_Version_TimeStamp << " -->";
	postProcessingParametersFile.close();
	*/

	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" << originalMeshFileName;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" << modifiedMeshFileName ;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + abaqusDirName + "\\" <<  abaqusAnalysisBatFileName ;
	//isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + abaqusDirName + "\\" <<  abaqusPostProcessingParametersXMLFileName;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + caculixDirName + "\\" <<  calculixDeckBatFileName;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + caculixDirName + "\\" <<  calculixLinuxBatFileName;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + nastranDirName + "\\" <<  nastranAnalysisBatFileName ;
	isis_LOG(lg, isis_FILE, isis_INFO) << "   Created: .\\" + analysisDirName + "\\" + nastranDirName + "\\" <<  fEAAnalysisMetaDataFile;


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // end  namespace isis 
