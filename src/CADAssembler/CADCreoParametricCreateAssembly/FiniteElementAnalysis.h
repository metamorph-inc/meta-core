#ifndef FINITE_ELEMENT_ANALYSIS_H
#define FINITE_ELEMENT_ANALYSIS_H

namespace isis
{

	bool IsAFEAAnlysisDeckBasedRun( const CADAssemblies &in_CADAssemblies );
	bool IsFEAAnalysisAbaqusModelBasedRun( const CADAssemblies &in_CADAssemblies );

	void Create_FEADecks_BatFiles( 
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					std::map<std::string, Material>						&in_Materials,
					const std::string									&in_WORKING_DIR,
					const std::string									in_ProgramName_Version_TimeStamp,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

	void RetrieveDatumPointCoordinates( const std::string							&in_AssemblyComponentID,
									const std::string								&in_PartComponentID,
									std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
									const MultiFormatString							&in_DatumName,
									CADPoint										&out_CADPoint);


	// Pre-Conditions
	//	in_TopLevelAssemblyData.CADAnalyses.list<AnalysisFEA> can contain only one item.  Multiple Analysis
	//  per assembly are not currently supported.
	//
	//	in_NastranMaterialID_to_CompnentID_map can be empty.  If empty, then FEAElementID="", instead of e.g. PSOLID_1
	void CreateXMLFile_FEA_AnalysisMetaData( 
						const std::string									&in_PathAndFileName,
						const TopLevelAssemblyData							&in_TopLevelAssemblyData,
						std::map<std::string, Material>						&in_Materials,
						std::map<std::string, std::string>                  &in_NastranMaterialID_to_CompnentID_map,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

}
#endif // FINITE_ELEMENT_ANALYSIS_H