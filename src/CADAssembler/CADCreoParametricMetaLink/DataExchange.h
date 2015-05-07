#ifndef DATA_EXCHANGE_H
#define DATA_EXCHANGE_H
#include <isis_application_exception.h>
#include <string>
#include <map>
#include <CommonStructures.h>

namespace isis
{

void ExportDataExchangeFiles(
    const MultiFormatString								&in_ModelName,
    ProMdlType											in_ModelType,
    const std::string									&in_WORKING_DIR,
    const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
    bool												in_LogProgress = false)
throw(isis::application_exception);

void ExportDataExchangeFiles(
    const MultiFormatString								&in_ComponentID,  // Could be an assembly or part
    const std::string									&in_WORKING_DIR,
    const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
    std::map<string, isis::CADComponentData>			&in_CADComponentData_map,
    bool												in_LogProgress)
throw(isis::application_exception);

void ExportRasterImage(const std::string									&in_AssemblyComponentID,
                       std::map<string, isis::CADComponentData>			&in_CADComponentData_map)
throw(isis::application_exception);

//bool SeparateSTEPPartFilesSpecified(const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications);

bool DataExchange_Format_Version_InList(
    e_DataExchangeFormat  in_DataExchangeFormat,  // Only currently support DATA_EXCHANGE_FORMAT_STEP
    e_DataExchangeVersion in_DataExchangeVersion, // AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES...
    const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications);

}  // End namespace isis

#endif