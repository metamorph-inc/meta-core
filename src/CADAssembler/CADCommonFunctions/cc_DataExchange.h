#ifndef CC_DATA_EXCHANGE_H
#define CC_DATA_EXCHANGE_H
#include "cc_CommonStructures.h"
#include "cc_MultiFormatString.h"
#include "isis_application_exception.h"

namespace isis 
{


	void ExportDataExchangeFiles( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					e_CADMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool													in_LogProgress = false)
																	throw (isis::application_exception);

	void ExportDataExchangeFiles( 
				const std::string									&in_ComponentID,  // Could be an assembly or part
				const std::string									&in_WORKING_DIR,
				const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
				std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
				bool												in_LogProgress )
																throw (isis::application_exception);

	//void ExportRasterImage(	const std::string									&in_AssemblyComponentID, 
	//					std::map<std::string, isis::CADComponentData>			&in_CADComponentData_map )
	//															throw (isis::application_exception);

	//bool SeparateSTEPPartFilesSpecified(const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications);

	bool DataExchange_Format_Version_InList( 
			e_DataExchangeFormat  in_DataExchangeFormat,  // Only currently support DATA_EXCHANGE_FORMAT_STEP
			e_DataExchangeVersion in_DataExchangeVersion, // AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES...
			const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications );






} // END 

#endif