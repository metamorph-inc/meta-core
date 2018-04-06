#include "stdafx.h"
#include <AssemblyEditingViaLink.h>
#include <ToolKitPassThroughFunctions.h>
#include <BuildAssembly.h>
#include <SetCADModelParameters.h>
#include <CommonUtilities.h>
#include <AssembleUtils.h>
#include <ParametricParameters.h>
#include <ApplyModelConstraints.h>
#include <cc_XMLtoCADStructures.h>
#include <ProWindows.h>
#include <ProMdl.h>
#include <ProAnnotation.h>
#include <ProSelbuffer.h>
#include <DiagnosticUtilities.h>
#include <ISISConstants.h>
#include <CommonFunctions.h>
#include <ProUIMessage.h>
#include <algorithm>

#include "UIFunctions.h"
#include "isis_ptc_toolkit_ostream.h"
#include "cc_CADFactoryAbstract.h"
#include <cc_CommonUtilities.h>
#include <cc_CommonFunctions.h>
#include <cc_CommonDefinitions.h>
#include <cc_AssemblyUtilities.h>
#include <cc_ApplyModelConstraints.h>


namespace isis
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Class MetaLinkAssemblyEditor
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MetaLinkAssemblyEditor::MetaLinkAssemblyEditor(//cad::CadFactoryAbstract::ptr in_cadfactory,
        const isis::MetaLinkInputArguments              &in_ProgramInputArguments,
        std::map<std::string, isis::CADComponentData> &in_CADComponentData_map) :
   // m_logcat(::log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
    m_programInputArguments(in_ProgramInputArguments),
    m_CADComponentData_map(in_CADComponentData_map),
    m_uniqueNameIndex(0), m_addedToAssemblyOrdinal(0)
{
	isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkAssemblyEditor::MetaLinkAssemblyEditor()";

    //m_cadfactory = in_cadfactory;
    designID = in_ProgramInputArguments.designID;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::ClearSelection()
{
    isis_LOG(lg, isis_FILE, isis_DEBUG) << "ClearSelection() entered";
    ProError status;
    switch(status = ProWindowRepaint(PRO_VALUE_UNUSED))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProWindowRepaint(): Window repainted." ;
        break;
    case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_FILE, isis_WARN) << "ProWindowRepaint(): Unsuccesful" << "[" << __FILE__ << ": " << __LINE__ << "]";
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Highlight the named datum and the containing component.
* If the datum doesn't exist, still highlight the component.
*/

void MetaLinkAssemblyEditor::SelectComponentOfAssembly(
    const std::string                 &in_ComponentInstanceId,
    std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
throw(isis::application_exception)
{
    //log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

    ProError status;
    ProModelitem selectedComponent;
    ProModelitem selectedDatum;
    ProSelection selection;

    std::map<std::string, isis::CADComponentData>::const_iterator parentIx = m_CADComponentData_map.find(topAssemblyComponentInstanceID);
    ProMdl top_handle = parentIx->second.cADModel_hdl;

    std::map<std::string, isis::CADComponentData>::const_iterator componentIx = m_CADComponentData_map.find(in_ComponentInstanceId);
    if(componentIx == m_CADComponentData_map.end())
    {
        isis_LOG(lg, isis_FILE, isis_WARN)
                << "Function MetaLinkAssemblyEditor::SelectComponentOfAssembly invoked, but ComponentInstanceID does not exist in the assembly. "
                <<  in_ComponentInstanceId;
        throw isis::application_exception("C08002", "component instance ID is not present");
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << "found component: " << componentIx->second.name;

    ProAsmcomppath assemblyPath;
    switch(status = Retrieve_ProAsmcomppath((ProSolid)top_handle,
                                            componentIx->second.componentPaths,
                                            assemblyPath))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "selection of component";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09004", "one or more input arguments were invalid");
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "highlight the component";
    ProMdl component_handle = componentIx->second.cADModel_hdl;
    switch(status = ProMdlToModelitem(component_handle, &selectedComponent))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "selection of component";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09004", "one or more input arguments were invalid");
    case PRO_TK_NOT_EXIST:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "model item does not exist";
        throw isis::application_exception("C09014", "Item with such id and type does not exist");
    }
    switch(status = ProSelectionAlloc(&assemblyPath, &selectedComponent, &selection))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "selection of component";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09004", "one or more input arguments were invalid");
    }
    //ProSelect(
    switch(status = ProSelectionHighlight(selection, PRO_COLOR_HIGHLITE))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "component highlighted";
        break;
    case PRO_TK_BAD_CONTEXT:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09005", "one or more input arguments were invalid");
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "selection complete";
    switch(status = ProWindowRefresh(PRO_VALUE_UNUSED))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "window refreshed" ;
        break;
    case PRO_TK_BAD_CONTEXT:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "the current view is not valid";
        throw isis::application_exception("C09007", "one or more input arguments were invalid");
    }
    switch(status = ProDetailtreeRefresh((ProSolid)component_handle, PRO_VALUE_UNUSED))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "the detail tree is refreshed" ;
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more of the input arguments are invalid.";
        throw isis::application_exception("C09008", "one or more input arguments were invalid");
    case PRO_TK_NO_CHANGE:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "There is no change in the detail tree.";
        break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* Locate the named component and the containing assembly.
*/
std::string MetaLinkAssemblyEditor::LocateComponentOfAssembly(
    std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
throw(isis::application_exception)
{
    //log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    isis_LOG(lg, isis_FILE, isis_INFO) << "locate request is being sent";
    ProError status;
    ProSelection *ret_buff;
    switch(status = ProSelbufferSelectionsGet(&ret_buff))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "buffer is in session and was returned.";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "The argument was invalid (NULL).";
        throw isis::application_exception("C0A001", "invalid selection buffer");
    case PRO_TK_CANT_ACCESS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "There is no Object/Action selection tool active.";
        throw isis::application_exception("C0A002", "no active selection");
    case PRO_TK_E_NOT_FOUND:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "There are no objects in the current selection tool.";
        return "";
    }
    ProModelitem component_handle;
    switch(status = ProSelectionModelitemGet(*ret_buff, &component_handle))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "buffer is in session and was returned.";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "The argument was invalid (NULL).";
        throw isis::application_exception("C0A004", "null argument");
    case PRO_TK_NOT_EXIST:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "The model item doesn't exist.";
        throw isis::application_exception("C0A005", "selected model does not exist");
    }
    ProAsmcomppath component_path;
    switch(status = ProSelectionAsmcomppathGet(*ret_buff, &component_path))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "buffer is in session and was returned.";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "The argument was invalid (NULL).";
        throw isis::application_exception("C0A006", "no selelcted assembly path");
    }
    ProSelectionarrayFree(ret_buff);
    isis_LOG(lg, isis_FILE, isis_ERROR) << "caught select event : component: " << component_handle.id << " table: " << component_path.table_num;

    return isis::GlobalModelData::Instance.GetGuidFromModel(component_path);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::SelectDatumOfComponent(
    const std::string               &in_FeatureGeometryType,
    const std::string 				&in_DatumName,
    std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
throw(isis::application_exception)
{

    //log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    isis_LOG(lg, isis_FILE, isis_INFO) <<"select datum request has been received";
    ProError status;

    ProMdl componentHandle = m_AvmComponentModel;
    ProModelitem selectedDatum;
    ProSelection selection;

    isis_LOG(lg, isis_FILE, isis_INFO) << "highlight the datum";
    if(in_DatumName.size() > (PRO_NAME_SIZE - 1))
    {
        isis_LOG(lg, isis_FILE, isis_WARN)
                << "Function MetaLinkAssemblyEditor::SelectComponentOfAssembly invoked, but DatumName is too long. "
                <<  in_DatumName;
        throw isis::application_exception("C08005", "datum name is too long");
    }
    ProName datumName;
    ProStringToWstring(datumName, const_cast< char* >(in_DatumName.c_str()));

    ProType type = FeatureGeometryType_enum(in_FeatureGeometryType);
    switch(status = ProModelitemByNameInit(componentHandle, type, datumName, &selectedDatum))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "selected datum";
        break;
    case PRO_TK_E_NOT_FOUND:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "The datum was not found: " << datumName;
        throw isis::application_exception("C09002", "datum not found by name");
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09003", "one or more input arguments were invalid");
    }
    switch(status = ProSelectionAlloc(NULL, &selectedDatum, &selection))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "selection of datum";
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09004", "one or more input arguments were invalid");
    }
    switch(status = ProSelectionHighlight(selection, PRO_COLOR_HIGHLITE))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "datum highlighted";
        break;
    case PRO_TK_BAD_CONTEXT:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more input arguments was invalid";
        throw isis::application_exception("C09005", "one or more input arguments were invalid");
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "selection complete";
    switch(status = ProWindowRefresh(PRO_VALUE_UNUSED))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "window refreshed" ;
        break;
    case PRO_TK_BAD_CONTEXT:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "the current view is not valid";
        throw isis::application_exception("C09007", "one or more input arguments were invalid");
    }
    switch(status = ProDetailtreeRefresh((ProSolid)componentHandle, PRO_VALUE_UNUSED))
    {
    case PRO_TK_NO_ERROR:
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "the detail tree is refreshed" ;
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "One or more of the input arguments are invalid.";
        throw isis::application_exception("C09008", "one or more input arguments were invalid");
    case PRO_TK_NO_CHANGE:
        isis_LOG(lg, isis_FILE, isis_ERROR) << "There is no change in the detail tree.";
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::AddComponentToAssembly(
	//cad::CadFactoryAbstract			 &in_Factory,
    const std::string                 &in_ComponentInstanceID,
    const std::string                 &in_CreoModelName,
    //ProMdlType                        in_CreoModelType,
	e_CADMdlType						in_CreoModelType,
    const std::string                &in_MaterialID,
    const std::string                &in_DisplayName,
    e_CADSpecialInstruction            in_SpecialInstruction,
    const std::vector<CADParameter>    &in_CADParameters,
    std::vector<isis::CADCreateAssemblyError> &out_ErrorList,
    std::vector<isis::CADComponentConnector> &in_ConnectorList
)	throw(isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();


    isis_LOG(lg, isis_FILE, isis_INFO) << "***************** Begin MetaLinkAssemblyEditor::AddComponentToAssembly ******************";
    std::stringstream generalMsg;

    // 8/14/2013 Tempory fix to the problem that CyPhy does not have a way to pass the assembly instnce ID in
    // a consistend manner.  For now, only allow parts/sub-assemblies to be added to the top
    // assembly.
    std::string parentAssemblyInstanceID = topAssemblyComponentInstanceID;

    isis_LOG(lg, isis_FILE, isis_INFO) << "Setting parentAssemblyInstanceID to topAssemblyComponentInstanceID."
                          << isis_EOL << "  topAssemblyComponentInstanceID: " << topAssemblyComponentInstanceID;

    //std::string parentAssemblyInstanceID = in_ParentAssemblyInstanceID;

    generalMsg  <<  std::endl << "    programInputArguments.designID: " << m_programInputArguments.designID
                <<  std::endl << "    topAssemblyComponentInstanceID: " << topAssemblyComponentInstanceID
                <<  std::endl << "    Temporary workaround, parentAssemblyInstanceID set to topAssemblyComponentInstanceID"
                <<  std::endl << "    parentAssemblyInstanceID:       " << parentAssemblyInstanceID
                <<  std::endl << "    in_ComponentInstanceID:         " << in_ComponentInstanceID
                <<  std::endl << "    in_CreoModelName:               " << in_CreoModelName
                <<  std::endl << "    in_MaterialID:                  " << in_MaterialID
                <<  std::endl << "    in_CreoModelType:               " << ProMdlType_string(in_CreoModelType)
                <<  std::endl << "    in_SpecialInstruction:          " << in_SpecialInstruction;



    isis_LOG(lg, isis_FILE, isis_INFO) << "************* Begin Input Arguments *********************";

    isis_LOG(lg, isis_FILE, isis_INFO) << "Adding component to the assembly: " <<  generalMsg.str();
    for each(CADParameter i in in_CADParameters)
    {
        isis_LOG(lg, isis_FILE, isis_INFO)  << i;
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "*************** End Input Arguments *********************";

    isis_LOG(lg, isis_FILE, isis_INFO) << "Adding Creo model"  << in_CreoModelName;

    /////////////////////////////
    // Begin Input Data Checks
    /////////////////////////////
    isis_LOG(lg, isis_FILE, isis_INFO) << "******************** Begin Checks ***********************";
    // Verify that a top assembly exists in this class
    if(topAssemblyComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but no assemblies have been created. "
                    <<      generalMsg.str();
        isis_LOG(lg, isis_FILE, isis_ERROR) << errorString.str();
        throw isis::application_exception("C08001", errorString);                          ////
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << " Top level assembly exists in the internal data structures";


    // 8/14/2013 Old approach for dealing with the mapping of a CyPhy DesignID to Creo
    //if ( m_CADComponentData_map.find(parentAssemblyInstanceID) == m_CADComponentData_map.end() )
    //{
    //    // Assume this is the parent ID which would have a |1 suffix. When using
    //    // MetaLink, this must always be a |1 because there cannot be any islands in a
    //    // CyPhy model connected to MetaLink.  A |2 would indicate that there was one
    //    // main assembly and one island.
    //    parentAssemblyInstanceID += "|1";
    //}

    // Verify parentAssemblyInstanceID exists in the assembly.
    if(m_CADComponentData_map.find(parentAssemblyInstanceID) == m_CADComponentData_map.end())
    {
        std::stringstream errorString;
        errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but parentAssemblyInstanceID does not exists in the assembly. "
                    <<  generalMsg.str();
        throw isis::application_exception("C08002", errorString);               ///////
    }
    else
    {
        isis_LOG(lg, isis_FILE, isis_INFO) << " Parent assembly exists in the internal data structures";

        // We know parentAssemblyInstanceID exists in the assembly, now verify that parentAssemblyInstanceID is an assembly
        std::map<std::string, isis::CADComponentData>::const_iterator itr;
        itr = m_CADComponentData_map.find(parentAssemblyInstanceID);
        if(itr != m_CADComponentData_map.end() && itr->second.modelType != PRO_ASSEMBLY)
        {
            std::stringstream errorString;
            errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but parentAssemblyInstanceID is not an assembly. "
                        <<  generalMsg.str();
            throw isis::application_exception("C08003", errorString);
        }
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << " Parent assembly is a Creo assembly (i.e. not a part)";

    // Verify in_ComponentInstanceID does not exist in the assembly.  If it does exist, then an attempt is
    // being made to add it a second time.  A ComponentInstanceID can exist one and only one time in a assembly.
    if(m_CADComponentData_map.find(in_ComponentInstanceID) != m_CADComponentData_map.end())
    {
        std::stringstream errorString;
        errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but in_ComponentInstanceID already exists in the assembly. "
                    << "A ComponentInstanceID can exist one and only one time in a assembly. "
                    <<  generalMsg.str();
        throw isis::application_exception("C08004", errorString);
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << " Add component instance does not already exist in the assembly";

    // Make sure in_CreoModelName is not too long
    if(in_CreoModelName.size() > (PRO_NAME_SIZE - 1))
    {
        std::stringstream errorString;
        errorString
                << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but in_CreoModelName is longer than "
                << (PRO_NAME_SIZE - 1) << " characters."
                <<  generalMsg.str();
        throw isis::application_exception("C08005", errorString);
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << " Creo model name is within the allowable length of: " << (PRO_NAME_SIZE - 1) << " characters.";
    isis_LOG(lg, isis_FILE, isis_INFO) << "********************* End Checks ************************";
    /////////////////////////////
    // End Input Data Checks
    /////////////////////////////

    std::list<std::string>    toAddComponentInstanceIDs;
    toAddComponentInstanceIDs.push_back(in_ComponentInstanceID);

    isis::CADComponentData    componentData;
    componentData.componentID =            in_ComponentInstanceID;
    componentData.parentComponentID =    parentAssemblyInstanceID;
    componentData.name =                in_CreoModelName;
    componentData.modelType =            in_CreoModelType;
    componentData.materialID_FromCyPhy =            in_MaterialID;
    //componentData.specialInstruction =    in_SpecialInstruction;
    componentData.displayName = in_DisplayName;

    // Add parameters
    for each(CADParameter i in in_CADParameters)
    {
        componentData.parametricParameters.push_back(i);
    }

    if(in_CADParameters.size() > 0)
    {
        componentData.parametricParametersPresent = true;
        // For a parametric part, must check if it occurs more than once, if so, must copy to a new name
        // zzz Should probably make existingModelNames a attribute of this class so that it does not have to be
        // created each time.
        std::list<std::string> existingModelNames;
        for each(std::pair<std::string, isis::CADComponentData> i in m_CADComponentData_map)
        {
            existingModelNames.push_back(isis::ConvertToUpperCase(i.second.name));
        }

        if(std::count(existingModelNames.begin(), existingModelNames.end(), isis::ConvertToUpperCase(in_CreoModelName)) > 0)
        {

            // Model name already occurs.  Must copy it to a new name.
            std::string origNameWithoutFamilyEntry;
            std::string modelName;
            std::string completeName;

            ++m_uniqueNameIndex;

            CreateModelNameWithUniqueSuffix(//in_Factory,
											m_uniqueNameIndex,
                                            in_CreoModelName,
                                            origNameWithoutFamilyEntry,
                                            modelName,
                                            completeName,
											PRO_NAME_SIZE - 1);

            isis_LOG(lg, isis_FILE, isis_INFO) << "Parametric part/sub-assembly rename: "
                                  << isis_EOL << "   Old Name:                          " << in_CreoModelName
                                  << isis_EOL << "   New Name:                          " << modelName
                                  << isis_EOL << "   Family Table Name (if applicable): " << completeName;

            ///std::cout << std::endl << "CreateModelNameWithUniqueSuffix:modelName BB" << modelName << "EE";

            // Copy Model

            ProMdl     p_model;
            //zzz does this work with family tables
            isis::MultiFormatString  newModelName(modelName, PRO_NAME_SIZE - 1);
            // Since the source model could be anywhere in the search path, we
            // must open the source model and save it to force a copy to exist in the working directory.
            isis::isis_ProMdlRetrieve(componentData.name, PRO_MDL_PART, &p_model);
            isis::isis_ProMdlSave(p_model);
            isis::isis_ProMdlfileCopy(PRO_MDL_PART, componentData.name, newModelName);
            componentData.name = completeName;  // This would include the family table entry.

            //std::cout << std::endl << "componentData.name: BB" << componentData.name << "EE";
        }

    }

    // Add Connector data
    componentData.connectors = in_ConnectorList;

    //std::pair<std::string, isis::CADComponentData>  cADComponentData_pair(in_ComponentInstanceID,componentData);
    // Add cADComponentData_pair to a tempory map
    std::map<std::string, isis::CADComponentData>    cADComponentData_map_TEMP;
    //cADComponentData_map_TEMP.insert(cADComponentData_pair);

    cADComponentData_map_TEMP[in_ComponentInstanceID] = componentData;

    try
    {
        //isis_LOG(lg, isis_FILE, isis_INFO)  << "*********** Begin Temp Structure - Call to Creo SDK to Add the Component **************";
        //isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle: "         << m_CADComponentData_map[parentAssemblyInstanceID].modelHandle;
        //isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
        //isis_LOG(lg, isis_FILE, isis_INFO) << "cADComponentData_map_TEMP[in_ComponentInstanceID]:                   " << cADComponentData_map_TEMP[in_ComponentInstanceID];
        //isis_LOG(lg, isis_FILE, isis_INFO)  << "************ End Temp Structure - Call to Creo SDK to Add the Component ***************";

        //isis::Add_Subassemblies_and_Parts(//*m_cadfactory,
        //                                  m_CADComponentData_map[parentAssemblyInstanceID].cADModel_hdl,
        //                                  m_CADComponentData_map[parentAssemblyInstanceID].name,
        //                                  toAddComponentInstanceIDs,
        //                                  cADComponentData_map_TEMP,
        //                                  m_addedToAssemblyOrdinal);

		modelOperations.addModelsToAssembly(	 parentAssemblyInstanceID,
											 toAddComponentInstanceIDs,
											 cADComponentData_map_TEMP,
											 m_addedToAssemblyOrdinal);


        //isis::isis_ProMdlDisplay( cADComponentData_map_TEMP[in_ComponentInstanceID].modelHandle);

        bool regenerationSucceeded = false;
        //isis::RegenerateModel(     m_CADComponentData_map[parentAssemblyInstanceID].modelHandle,
        //             m_CADComponentData_map[parentAssemblyInstanceID].name,
        //             m_CADComponentData_map[parentAssemblyInstanceID].componentID,
        //             regenerationSucceeded);

        isis_LOG(lg, isis_FILE, isis_INFO) << "*********** Begin Temp Structure - Call to Creo SDK to Add the Component **************";
        isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle:      " << (const void*)m_CADComponentData_map[parentAssemblyInstanceID].cADModel_hdl;
        isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
        isis_LOG(lg, isis_FILE, isis_INFO) << "cADComponentData_map_TEMP[in_ComponentInstanceID]:                 " << cADComponentData_map_TEMP[in_ComponentInstanceID];
        isis_LOG(lg, isis_FILE, isis_INFO) << "************ End Temp Structure - Call to Creo SDK to Add the Component ***************";

        //for each ( std::string j in toAddComponentInstanceIDs )
        //{
        //    isis_LOG(lg, isis_FILE, isis_INFO) << "toAddComponentInstanceID: " << j;
        //    isis_LOG(lg, isis_FILE, isis_INFO) << "cADComponentData_map_TEMP[j].p_model: " << cADComponentData_map_TEMP[j].p_model;
        //}

        isis::ApplyParametricParameters(toAddComponentInstanceIDs, cADComponentData_map_TEMP, out_ErrorList);

		// R.O. 8/25/2015, No need to regenerate models here.  The previous statement (isis::ApplyParametricParameters) did
		// a regenerate of the components that had parameters.
        //isis::RegenerateModel(m_CADComponentData_map[parentAssemblyInstanceID].modelHandle,
        //                      m_CADComponentData_map[parentAssemblyInstanceID].name,
        //                      m_CADComponentData_map[parentAssemblyInstanceID].componentID,
        //                      regenerationSucceeded);

        isis::isis_ProWindowRepaint(windowID);

        //int windowID;
        //isis::isis_ProMdlWindowGet( m_CADComponentData_map[parentAssemblyInstanceID].modelHandle, &windowID);
        //isis::isis_ProWindowCurrentSet(windowID);


    }
    catch(...)
    {
        // The exception could have been because the Creo model was not found or because a parameter was not found.
        // For the latter case, the model would have been added to the assembly and we need to delete it.
        // The ProMdlDelete will not throw an exception if the model could not be found.
        if(cADComponentData_map_TEMP[in_ComponentInstanceID].cADModel_ptr_ptr != 0)
        {
            ProMdlDelete(cADComponentData_map_TEMP[in_ComponentInstanceID].cADModel_ptr_ptr);
        }
        throw;
    }

    // If it made it to here, then the component was successfully added
    // Add the information to the actial data structures
    m_CADComponentData_map[in_ComponentInstanceID] = cADComponentData_map_TEMP[in_ComponentInstanceID];
    m_CADComponentData_map[parentAssemblyInstanceID].children.push_back(in_ComponentInstanceID);

    isis_LOG(lg, isis_FILE, isis_INFO) << "******************* Begin Final Structure - Add the Component *************************";
    isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle:      " << (const void*)m_CADComponentData_map[parentAssemblyInstanceID].cADModel_hdl;
    isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
    isis_LOG(lg, isis_FILE, isis_INFO) << "m_CADComponentData_map[in_ComponentInstanceID]:                    " << m_CADComponentData_map[in_ComponentInstanceID];
    isis_LOG(lg, isis_FILE, isis_INFO) << "******************* End Final Structure - Add the Component *************************";

    isis_LOG(lg, isis_FILE, isis_INFO) << "Successfully added component to the assembly:" <<  generalMsg.str();
    isis_LOG(lg, isis_FILE, isis_INFO) << m_CADComponentData_map.find(parentAssemblyInstanceID)->second;

    isis_LOG(lg, isis_FILE, isis_INFO) << "******************* End MetaLinkAssemblyEditor::AddComponentToAssembly ******************";

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::UpdateComponentName(const std::string &in_ConstraintComponentInstanceID, const std::string &newName) throw(isis::application_exception)
{
    if(topAssemblyComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "exception : Function MetaLinkAssemblyEditor::UpdateComponentName invoked, but no assemblies have been created. ";
        isis_LOG(lg, isis_FILE, isis_ERROR) << errorString.str();
        throw isis::application_exception("C08013", errorString);
    }

    // Verify in_ComponentInstanceID does not exists.  If it does exist, then an attempt is
    // being made to modify a parameter on a component that does not exist
    if(m_CADComponentData_map.find(in_ConstraintComponentInstanceID) == m_CADComponentData_map.end())
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::UpdateComponentName invoked, but in_ComponentInstanceID does not exist in the assembly. "
                    << "A ComponentInstanceID must exist before a constraint can be added. in_ComponentInstanceID: " << in_ConstraintComponentInstanceID;
        throw isis::application_exception("C08007",errorString);
    }

    isis::CADComponentData &data = m_CADComponentData_map[in_ConstraintComponentInstanceID];
    data.displayName = newName;

    std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix(data.name, data.modelType);

    try
    {
        isis::SetParametricParameter(FORCE_KEY, ModelNameWithSuffix, data.cADModel_ptr_ptr, CYPHY_NAME, CAD_STRING, data.displayName);
    }
    catch(isis::application_exception &ex)
    {
        isis_LOG(lg, isis_FILE, isis_ERROR) << "Unable to set component name, error: " << ex.what();
    }

    ProMdl mainMdl;
    ProMdlCurrentGet(&mainMdl);
    ProTreetoolRefresh(mainMdl);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::ConstrainComponent(const std::string                    &in_ConstraintComponentInstanceID,
        const std::vector< ConstraintPair>  &in_ConstraintPairs)
throw(isis::application_exception)
{
    isis_LOG(lg, isis_FILE, isis_INFO) << "***************** Begin MetaLinkAssemblyEditor::ConstrainComponent ******************";
    isis_LOG(lg, isis_FILE, isis_INFO) << "*************** Begin Input Arguments *********************";

    if(topAssemblyComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "exception : Function MetaLinkAssemblyEditor::ConstrainComponent invoked, but no assemblies have been created. ";
        isis_LOG(lg, isis_FILE, isis_ERROR) << errorString.str();
        throw isis::application_exception("C08013", errorString);
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "in_ConstraintComponentInstanceID: " << in_ConstraintComponentInstanceID;
    for each(ConstraintPair i  in in_ConstraintPairs)
    {
        isis_LOG(lg, isis_FILE, isis_INFO) << "      constraintFeatures:";
        for each(ConstraintFeature j in i.constraintFeatures)
        {
            isis_LOG(lg, isis_FILE, isis_INFO)
                    << "         component:               " <<        j.componentInstanceID  << isis_EOL
                    << "         featureName:             " <<        j.featureName  << isis_EOL
                    << "         featureOrientationType:  " <<        ProDatumside_string(j.featureOrientationType);

        }
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << "*************** End Input Arguments *********************";

    /////////////////////////////
    // Begin Input Data Checks
    /////////////////////////////

    // Verify in_ComponentInstanceID is not an empty string
    if(in_ConstraintComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::ConstrainComponent invoked, but in_ComponentInstanceID is an empty string."
                    << "A ComponentInstanceID must exist before a constraint can be added.";
        throw isis::application_exception("C08006",errorString);
    }

    // Verify in_ComponentInstanceID does not exists.  If it does exist, then an attempt is
    // being made to modify a parameter on a component that does not exist
    if(m_CADComponentData_map.find(in_ConstraintComponentInstanceID) == m_CADComponentData_map.end())
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::ConstrainComponent invoked, but in_ComponentInstanceID does not exist in the assembly. "
                    << "A ComponentInstanceID must exist before a constraint can be added. in_ComponentInstanceID: " << in_ConstraintComponentInstanceID;
        throw isis::application_exception("C08007",errorString);
    }
    /////////////////////////////
    // End Input Data Checks
    /////////////////////////////

    isis_LOG(lg, isis_FILE, isis_INFO) << "Constraining Creo model:"  << m_CADComponentData_map[in_ConstraintComponentInstanceID].name;

    ///////////////////////////////////////////////
    // Add the contraints to m_CADComponentData_map
    ///////////////////////////////////////////////

    // zzz If already constrained, cannot allow a second constraint.  In the future will support deleting
    // constraints.  For now, throw an exception if trying to constrain a component that is already
    // constrained.
    isis_LOG(lg, isis_FILE, isis_INFO) << "Number of Constraints: "<< m_CADComponentData_map[topAssemblyComponentInstanceID].constraintDef.constraints.size();
    if(m_CADComponentData_map[in_ConstraintComponentInstanceID].constraintDef.constraints.size() > 0)
    {
        std::stringstream errorString;
        errorString << "exception : Function MetaLinkAssemblyEditor::ConstrainComponent invoked, but component is already constrained, in_ConstrainedComponentInstanceID:  " << in_ConstraintComponentInstanceID;
        throw isis::application_exception("C08008",errorString);
    }

    ConstraintData temp_ConstraintData;
    for each(ConstraintPair i in in_ConstraintPairs)
    {
        temp_ConstraintData.constraintPairs.push_back(i);
    }
    m_CADComponentData_map[in_ConstraintComponentInstanceID].constraintDef.constraints.push_back(temp_ConstraintData);

    ///////////////////////
    // Constrain Model
    //////////////////////
    bool firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound = false;
    if(m_CADComponentData_map.size() < 2)
    {
        firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound = true;
    }

    std::list<std::string>                componentIDsToBeConstrained;
    componentIDsToBeConstrained.push_back(in_ConstraintComponentInstanceID);


    isis_LOG(lg, isis_FILE, isis_INFO) << "**** Before call to ApplyListedModelsConstraints ******";
    isis_LOG(lg, isis_FILE, isis_INFO) << m_CADComponentData_map[in_ConstraintComponentInstanceID];

    ApplyListedModelsConstraints(//*m_cadfactory,
                          //reinterpret_cast<ProSolid*>(&m_CADComponentData_map[topAssemblyComponentInstanceID].cADModel_hdl),
						  topAssemblyComponentInstanceID,
                          componentIDsToBeConstrained,
                          true,
                          m_CADComponentData_map,
                          // Provide for the case where the first assembled part does not have
                          // the datums front, top, and right defined.
                          //false,
                          firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound);

    bool regenerationSucceeded;
    /*
        isis::RegenerateModel(    m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle,
                                  m_CADComponentData_map[topAssemblyComponentInstanceID].name,
                                  m_CADComponentData_map[topAssemblyComponentInstanceID].componentID,
                                  regenerationSucceeded);*/

    isis::isis_ProWindowRepaint(windowID);

    isis_LOG(lg, isis_FILE, isis_INFO) << "**** After call to ApplyListedModelsConstraints ******";

    isis_LOG(lg, isis_FILE, isis_INFO) << "Constraint applied successfully";
    isis_LOG(lg, isis_FILE, isis_INFO) << "***************** End MetaLinkAssemblyEditor::ConstrainComponent ******************";
    /////////////////////////////
    // Begin Input Data Checks
    /////////////////////////////
    if(in_ConstraintComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID is an empty string."
                    << "A ComponentInstanceID must exist before a parameter can be modeified.";
        isis::application_exception ex = isis::application_exception("C08009",errorString);
        throw ex;
    }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::ModifyParameters(const std::string  &in_ComponentInstanceID,
        const std::vector<CADParameter> &in_Parameters) throw(isis::application_exception)

{
    isis_LOG(lg, isis_FILE, isis_INFO) << "********************* Begin MetaLinkAssemblyEditor::ModifyParameter **********************";
    if(topAssemblyComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "exception : Function MetaLinkAssemblyEditor::ModifyParameters invoked, but no assemblies have been created. ";
        isis_LOG(lg, isis_FILE, isis_ERROR) << errorString.str();
        throw isis::application_exception("C08014", errorString);
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "************* Begin Input Arguments *********************";
    isis_LOG(lg, isis_FILE, isis_INFO) << "   in_ComponentInstanceID: " << in_ComponentInstanceID;
    for each(CADParameter i in in_Parameters)
    {
        isis_LOG(lg, isis_FILE, isis_INFO) << i;
    }
    isis_LOG(lg, isis_FILE, isis_INFO) << "************* End Input Arguments *********************";


    /////////////////////////////
    // Begin Input Data Checks
    /////////////////////////////
    if(in_ComponentInstanceID.size() == 0)
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID is an empty string."
                    << "A ComponentInstanceID must exist before a parameter can be modeified.";
        throw isis::application_exception("C08009",errorString);
    }


    // Verify in_ComponentInstanceID does not exists.  If it does exist, then an attempt is
    // being made to modify a parameter on a component that does not exist
    auto in_ComponentInstanceIt = m_CADComponentData_map.find(in_ComponentInstanceID);
    if(in_ComponentInstanceIt == m_CADComponentData_map.end())
    {
        std::stringstream errorString;
        errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID does not exist in the assembly. "
                    << "A ComponentInstanceID must exist before a parameter can be modifed on that component. in_ComponentInstanceID: " << in_ComponentInstanceID;
        throw isis::application_exception("C08010",errorString);
    }
    auto& in_ComponentInstance = in_ComponentInstanceIt->second;
    /////////////////////////////
    // End Input Data Checks
    /////////////////////////////

    std::string modelNameWithSuffix = AmalgamateModelNameWithSuffix(in_ComponentInstance.name, in_ComponentInstance.modelType);

    isis_LOG(lg, isis_FILE, isis_INFO) << "Internal data structure parameter values BEFORE modification:";
    for each(CADParameter i in in_ComponentInstance.parametricParameters)
    {
        isis_LOG(lg, isis_FILE, isis_INFO) << i;
    }

    ProMdl* p_model = (ProMdl*)in_ComponentInstance.cADModel_ptr_ptr;
    bool is_mmKs;
    ParametricParameter_WarnForPartUnitsMismatch(in_ComponentInstance, &is_mmKs);
    // Change the parameters
    for each(CADParameter i in in_Parameters)
    {
        SetParametricParameter(modelNameWithSuffix,
			in_ComponentInstance.cADModel_ptr_ptr,
                               i.name, i.type, i.value, i.units, is_mmKs);
    }

    bool regenerationSucceeded;
    isis::RegenerateModel(static_cast<ProSolid>(in_ComponentInstance.cADModel_hdl),
							in_ComponentInstance.name,
							in_ComponentInstance.componentID,
							regenerationSucceeded);

    isis::isis_ProWindowRepaint(windowID);

    // We must update the parameters in in_ComponentInstance.parametricParameters
    // as follows:
    //    a) if the parameter already exists, must update its value
    //    b) if the parameter does not exist, add it
    for each(CADParameter i in in_Parameters)
    {
        std::list<CADParameter>::iterator itr;
        itr = find(in_ComponentInstance.parametricParameters.begin(),
					in_ComponentInstance.parametricParameters.end(),
                   i);

        if(itr != in_ComponentInstance.parametricParameters.end())
        {
            // Found, must modify
            itr->type    = i.type;    // would be an error if the types are different
            itr->value    = i.value;
        }
        else
        {
            // Not found, must add
			in_ComponentInstance.parametricParameters.push_back(i);
        }
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "Internal data structure parameter values AFTER modification:";
    for each(CADParameter i in in_ComponentInstance.parametricParameters)
    {
        isis_LOG(lg, isis_FILE, isis_INFO) << i;
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << "********************* End MetaLinkAssemblyEditor::ModifyParameter ************************";
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::CreateAssembly(const std::string  &in_AssemblyXMLString) throw(isis::application_exception)
{
    GlobalModelData::Instance.Clear();
    std::vector<isis::CADCreateAssemblyError> errorList;
    CreateAssemblyViaString(//*m_cadfactory,
                            m_programInputArguments,
                            in_AssemblyXMLString,
                            m_uniqueNameIndex,
                            designID,
                            GlobalModelData::Instance.CadAssemblies,
                            m_CADComponentData_map,
                            errorList);

    GlobalModelData::Instance.designId = designID;
    if(GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.size() != 1)
    {
        std::stringstream errorString;
        errorString << "One and only one assembly allowed in MetaLink mode.  The XML defines "
                    << GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.size() << std::string(" assemblies.");
        throw isis::application_exception("C08015", errorString);
    }
    topAssemblyComponentInstanceID = GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID;
    isis_LOG(lg, isis_FILE, isis_INFO)
            << " top assembly component instance id SET : " << topAssemblyComponentInstanceID;

    bool hasCritical = false;
    if(errorList.size()!=0)
    {
        ostringstream ostr;
        isis_LOG(lg, isis_FILE, isis_ERROR) << "Error list from MetaLinkAssemblyEditor::CreateAssembly:";
        for(std::vector<isis::CADCreateAssemblyError>::iterator it = errorList.begin(); it != errorList.end(); ++it)
        {
            isis_LOG(lg, isis_FILE, isis_ERROR) << it->Text;
            ostr << it->Text << std::endl;
            if(it->Severity == CADCreateAssemblyError_Severity_Critical)
            {
                hasCritical = true;
            }
        }
        CreoMessageDialog(ostr.str(), PROUIMESSAGE_WARNING);
    }



    if(hasCritical)
    {
        isis::application_exception ex("Critical error(s) has been encountered during model construction. The model can't be opened.");
        throw ex;
    }

    GlobalModelData::Instance.mode = DESIGNEDIT;

    //ProMdl p_model;
    //isis::isis_ProMdlRetrieve( m_CADComponentData_map[topAssemblyComponentInstanceID].name,PRO_MDL_ASSEMBLY, &p_model);

    isis::isis_ProMdlDisplay(m_CADComponentData_map[topAssemblyComponentInstanceID].cADModel_hdl);

    isis::isis_ProMdlWindowGet(m_CADComponentData_map[topAssemblyComponentInstanceID].cADModel_hdl, &windowID);

    isis::isis_ProWindowActivate(windowID);
    isis_ProWindowCurrentSet(windowID);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateAssemblyViaString(//cad::CadFactoryAbstract						&in_Factory,
                             const isis::MetaLinkInputArguments              &in_ProgramInputArguments,
                             const std::string                              &in_XMLInputFile_String,
                             unsigned int									&in_out_UniqueNameIndex,
                             const std::string                              &in_DesingID,
                             isis::CADAssemblies                            &out_CADComponentAssemblies,
                             std::map<std::string, isis::CADComponentData>  &out_CADComponentData_map,
                             std::vector<CADCreateAssemblyError>            &out_ErrorList)
throw(isis::application_exception)
{
   
    isis_LOG(lg, isis_FILE, isis_INFO) << "raw xml:" << isis_EOL << in_XMLInputFile_String;

    bool Template_Copied = false;
    int ExitCode = 0;
    try
    {
        time_t time_start; /* calendar time */
        time_start=time(NULL); /* get current cal time */

        std::string cadPartsLibDir = in_ProgramInputArguments.auxiliaryCADDirectory;

        bool regenerationSucceeded_ForAllAssemblies = true;

        // Verify that the XML file is not null.
        if(in_XMLInputFile_String.size() == 0)
        {
            std::string TempError = "The assembly definition xml file input string (in_XMLInputFile_String) is empty";
            throw isis::application_exception(TempError.c_str());
        }

        FromXMLFile_PopulateCADComponentAssemblyAndMap(
            XML_DEFINED_BY_STRING,
            in_XMLInputFile_String,
            out_CADComponentAssemblies,
            out_CADComponentData_map,
            out_ErrorList);


        if(out_CADComponentAssemblies.topLevelAssemblies.size() == 0)
        {
            isis_LOG(lg, isis_FILE, isis_INFO) << "*************************** Begin Assembly Creation **************************";
            isis_LOG(lg, isis_FILE, isis_INFO) << "No assemblies were created because the input xml did not define any assemblies.";
            if(out_CADComponentAssemblies.unassembledComponents.size() == 0)
            {
                std::stringstream errorString;
                errorString <<
                            "The input xml file did not define any assemblies nor did it define unassembled parts/sub-assemblies.  "
                            << std::endl << "The input xml file must specify assemblies and/or unassembled parts/sub-assemblies.";
                throw isis::application_exception(errorString);
            }
            isis_LOG(lg, isis_FILE, isis_INFO) << "************************** End Assembly Creation *****************************";
        }
        // Rename parametric parts that have multiple instances of the same part.
        //std::map<std::string, std::string>  ToPartName_FromPartName_map;
        //isis::ModifyToHaveAUniqueNameForEachParametricPartOrAssembly( in_out_UniqueNameIndex, out_CADComponentData_map, ToPartName_FromPartName_map );
        std::vector<CopyModelDefinition>			fromModel_ToModel;
        isis::BuildListOfCADModels_ThatShouldBeCopiedToNewNames(//	in_Factory,
														in_out_UniqueNameIndex,
														e_PART_OR_ASSEMBLY_MODEL_TYPE,
														e_SELECT_ONLY_PARAMETRIC_MODELS,
														true,
														PRO_NAME_SIZE - 1,
														out_CADComponentData_map,
														fromModel_ToModel);

        isis_LOG(lg, isis_FILE, isis_INFO)  << "************** Begin Modified Part Names for Multiple Parametric Parts *****************";
        isis_LOG(lg, isis_FILE, isis_INFO)  << "From_Part_Name   To_Part_Name";
        isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel;
        isis_LOG(lg, isis_FILE, isis_INFO)  << "************** End Modified Part Names for Multiple Parametric Parts *****************";

        // Add the depends-on information to the CADComponentData
        isis::Add_DependsOn(out_CADComponentData_map);

        for each(isis::TopLevelAssemblyData i in out_CADComponentAssemblies.topLevelAssemblies)
        {

            isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Tree For a Single Assembly  *****************";
            isis_LOG(lg, isis_FILE, isis_INFO) << "assemblyComponentID: " << i.assemblyComponentID;
            isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Analysis Data For a Single Assembly  *****************";
            stream_AnalysisInputData(i.analysesCAD, clog);
            isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Analysis Data For a Single Assembly  *****************";

            isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";
            stream_AssemblyCADComponentData(i.assemblyComponentID, out_CADComponentData_map, clog);
            isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";

            isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Entire Tree For a Single Assembly  *****************";
        }


        ///////////////////////////////////////////////////////////////////////////////////
        // Check for Non-Size-To-Fit components being constrained to Size-To-Fit components
        ////////////////////////////////////////////////////////////////////////////////////
        std::set<std::string> TempIntersection = NonSizeToFitComponents_ReferencedBy_SizeToFitConstraints(out_CADComponentData_map);
        if(TempIntersection.size() > 0)
        {
            string err_str = "Erroneous XML File: A NON_SIZE_TO_FIT component cannot be constrained to a SIZE_TO_FIT component. " +
                             std::string("The SIZE_TO_FIT component(s) (i.e. ComponentIDs) that were erroneously referenced are:");
            for(std::set<std::string>::const_iterator i(TempIntersection.begin()); i != TempIntersection.end(); ++i)
            {
                err_str += " " + *i;
            }
            err_str += ".  Search for the ComponentIDs in the log file to identify the places where the SIZE_TO_FIT components are erroneously referenced.";
            throw isis::application_exception(err_str.c_str());

        }

        // *** If model renames occured
        // WARNING - isis::CopyModels must be called after
        //			 1) Creo has started
        //			 2) isis_ProDirectoryChange changed to the working directory
        //           been created.
        //			 3) the search_META.pro has been set
        //if ( ToPartName_FromPartName_map.size() > 0 ) isis::CopyModels(ToPartName_FromPartName_map);
        //if(fromModel_ToModel.size() > 0)
        //{
        //    isis::CopyModels(in_Factory, fromModel_ToModel);
        //}

        isis::MultiFormatString workingDir_MultiFormat(in_ProgramInputArguments.workingDirectory, PRO_PATH_SIZE - 1);
        //isis::isis_ProDirectoryChange( workingDir_MultiFormat );
        isis::setCreoWorkingDirectory(workingDir_MultiFormat);

       std::string TemplateFile_PathAndFileName = META_PATH() + "\\bin\\CAD\\Creo\\templates\\" + isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX;

        // Copy template model to the working directory
        isis::CopyFileIsis(TemplateFile_PathAndFileName,  in_ProgramInputArguments.workingDirectory);
        Template_Copied = true;

        // *** If model renames occured
        // WARNING - isis::CopyModels must be called after
        //             1) Creo has started
        //             2) isis_ProDirectoryChange changed to the working directory
        //           been created.
        //             3) the search_META.pro has been set
        if(fromModel_ToModel.size() > 0)
        {
            isis::CopyModels(//in_Factory, 
								fromModel_ToModel);
        }


        ////////////////////////////////////////
        // Build the assemblies
        ////////////////////////////////////////
        for(std::list<isis::TopLevelAssemblyData>::const_iterator i(out_CADComponentAssemblies.topLevelAssemblies.begin());
                i != out_CADComponentAssemblies.topLevelAssemblies.end();
                ++i)
        {
            bool regenerationSucceeded;
            isis::BuildAssembly(//in_Factory, 
								i->assemblyComponentID, in_ProgramInputArguments.workingDirectory, false, out_CADComponentData_map, regenerationSucceeded, out_ErrorList, true);

            if(!regenerationSucceeded)
            {
                regenerationSucceeded_ForAllAssemblies = false;
            }

            //////////////////////////////////////
            // Display assembly created messages
            ////////////////////////////////////
            time_t time_end;        /* calendar time */
            time_end=time(NULL);    /* get current cal time */

            time_t time_elapsed = time_end - time_start;

            isis_LOG(lg, isis_FILE, isis_INFO) << "Assembly creation completed successfully.";

            // Get component count
            isis::ComponentVistorCountAssemblyComponents  componentVistorCountAssemblyComponents;
            isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, componentVistorCountAssemblyComponents);

            // Log component count
            isis_LOG(lg, isis_FILE, isis_INFO) << "   Number of assembled components: " << componentVistorCountAssemblyComponents.numberOfComponents;
            isis_LOG(lg, isis_FILE, isis_INFO) << "   Elapsed wall-clock time:        " << time_elapsed << " seconds";

            time_start=time(NULL); // reset start time for subsequent assemblies if any
        }  // END  Build the assemblies

    } // END Try

    catch(isis::application_exception& ex)
    {
        throw;
    }
    catch(std::exception& ex)
    {
        throw;
    }
    catch(...)
    {
        std::stringstream    exceptionErrorStringStream;
        exceptionErrorStringStream << " std::exception: Caught exception (...).  Please report the error to the help desk.";
        throw isis::application_exception(exceptionErrorStringStream);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MetaLinkAssemblyEditor::Clear()
{
    isis_LOG(lg, isis_FILE, isis_INFO) << "MetaLinkAssemblyEditor::ClearAssembly(): starting ";

    bool success = true;
    ProError status;
    stringstream msg("ClearAssembly");

    ProMdl model_handle = m_CADComponentData_map[topAssemblyComponentInstanceID].cADModel_hdl;
    if(model_handle == NULL)
    {
        model_handle = GlobalModelData::Instance.ComponentEdit.cADModel_ptr;
    }
    if(model_handle == NULL)
    {
        isis_LOG(lg, isis_FILE, isis_WARN) << "Nothing to clear.";
        return false;
    }

    isis_LOG(lg, isis_FILE, isis_INFO) << " top: " << topAssemblyComponentInstanceID << " handle: " << (const void*)model_handle;

    switch(status = ProMdlEraseAll(model_handle))
    {
    case PRO_TK_NO_ERROR:
        msg << "erased all models";
        isis_LOG(lg, isis_FILE, isis_INFO) << msg.str();
        isis_ProMdlEraseNotDisplayed();  // R.O. Added 9/12/2013
        break;
    case PRO_TK_BAD_INPUTS:
        msg << "The model handle is defective: " << model_handle;
        isis_LOG(lg, isis_FILE, isis_ERROR) << msg.str();
        throw isis::application_exception(msg);
    case PRO_TK_BAD_CONTEXT:
        msg << "Current User Interface context does not allow "
            << "erasure of models (for example, when the Erase button is grayed out).";
        isis_LOG(lg, isis_FILE, isis_ERROR) << msg.str();
        throw isis::application_exception(msg);
    case PRO_TK_E_IN_USE:
        msg << "The model could not be erased because it is in use, "
            << "for example by another model in the session. ";
        isis_LOG(lg, isis_FILE, isis_ERROR) << msg.str();
        throw isis::application_exception(msg);
    }

    // Empty out the assembler data structures.
    m_CADComponentData_map.clear();
    topAssemblyComponentInstanceID = "";
    isis_LOG(lg, isis_FILE, isis_INFO) << " top assembly component instance id CLEARED";
    m_uniqueNameIndex = 0;
    searchPaths.clear();
    GlobalModelData::Instance.Clear();

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::UnHighlightAll()
{
    ProSelbufferClear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetaLinkAssemblyEditor::AddSearchPaths(const std::list<std::string> &in_SearchPaths) throw(isis::application_exception)
{
	isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkAssemblyEditor::AddSearchPaths()";
    ProName        optionName;
    ProPath        optionValue;

    try
    {
        for each(std::string i in in_SearchPaths)
        {
            if(i.size() != 0)
            {
                std::string searchPathString = i;

                // Check if searchPathString is in double quotes.  If not add double quotes.
                // This is necessary because Creo Config options with spaces must be enclosed in double quotes.
                if(searchPathString.find("\"")  == string::npos)
                {
                    searchPathString = "\"" + searchPathString + "\"";
                }

                std::string searchPathString_uppercase = isis::ConvertToUpperCase(searchPathString);
                if(searchPaths.find(searchPathString_uppercase) == searchPaths.end())
                {
                    MultiFormatString  searchPath_MultiFormat(searchPathString, PRO_PATH_SIZE - 1);
                    wcscpy(optionName, L"search_path");
                    wcscpy(optionValue, searchPath_MultiFormat);
                    isis_ProConfigoptSet(optionName, optionValue);
					isis_LOG(lg, isis_FILE, isis_DEBUG) << "Added serach path: " << searchPathString;
                    searchPaths.insert(searchPathString_uppercase);
                }
                else
                {
                    std::stringstream errorString;
                    errorString << "Search path already set in the Creo model. No action taken.  Search path: " << searchPathString;
                    isis_LOG(lg, isis_FILE, isis_INFO) << errorString.str();
                }
            }
            else
            {
                std::stringstream errorString;
                errorString << "MetaLinkAssemblyEditor::AddSerachPaths received an empty search path.  This is not considered a error.  Empty search paths are ignored";
                isis_LOG(lg, isis_FILE, isis_INFO) << errorString.str();
            }
        }
    }
    catch(isis::application_exception& ex)
    {
        std::stringstream    exceptionErrorStringStream;
        exceptionErrorStringStream << "exception : Function: MetaLinkAssemblyEditor::AddSerachPaths, std::application_exception. ";
        throw isis::application_exception("C08016", exceptionErrorStringStream);
    }
    catch(std::exception& ex)
    {
        std::stringstream    exceptionErrorStringStream;
        exceptionErrorStringStream << "exception : Function: MetaLinkAssemblyEditor::AddSerachPaths, std::exception: ";
        throw isis::application_exception("C08017", exceptionErrorStringStream);
    }
    catch(...)
    {
        std::stringstream    exceptionErrorStringStream;
        exceptionErrorStringStream << "exception : Function: MetaLinkAssemblyEditor::AddSerachPaths, Caught exception (...).  Please report the error to the help desk.";
        throw isis::application_exception("C08018", exceptionErrorStringStream);
    }
} // END MetaLinkAssemblyEditor::AddSearchPaths

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
Open Creo Model:
  1) retrieve the part file (ProMdlRetrieve)

Set the Global Parameters:
  1) cast the model to a model-item (ProMdlToModelitem)
  2) initialize a parameter data structure (ProParameterInit)
  2a) if the parameter exists, set the value in the parameter data structure (ProParamValueSet)
  2b) if the parameter does not exist, set the value and key (ProParamCreate)

Update the Display:
  1) update the display with the model (ProMdlDisplay)
  2) associate the model with the current window (ProMdlWindowGet);
  3) (ProWindowActivate(windowID))
  4) (ProWindowCurrentSet(windowID))

Reference:
  http://www.mcadcentral.com/creo-software-development/12424-how-reset-parameter-value-pro-toolkit.html
*/
void MetaLinkAssemblyEditor::InitAvmComponent(const std::string in_AvmComponentID, const std::string in_CreoModelName,
        const std::string  in_CreoAvmComponentName, const ProMdlType in_CreoModelType) throw(isis::application_exception)
{
	isis_LOG(lg, isis_FILE, isis_DEBUG) << "=== MetaLinkAssemblyEditor::InitAvmComponent  === " << " BEGIN ";
    ProError status;

    // load the avm component creo file
    ProName creoName;
    ProStringToWstring(creoName, const_cast< char* >(in_CreoAvmComponentName.c_str()));
    switch(status = ProMdlRetrieve(creoName, in_CreoModelType, &m_AvmComponentModel))
    {
    case  PRO_TK_NO_ERROR:
    {
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProMdlRetrieve() successfully retrieved the model.";
        break;
    }
    case  PRO_TK_BAD_INPUTS:
    {
        isis::application_exception ex(string("One or more of the input arguments are invalid. [") + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_E_NOT_FOUND:
    {
        isis::application_exception ex("The model '" + in_CreoAvmComponentName + "' was not found in the current directory. [" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_NO_PERMISSION:
    {
        isis::application_exception ex(string("The function does not have permission to operate on this model. [") + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    }

    isis::GlobalModelData::Instance.mode = isis::COMPONENTEDIT;
    isis::GlobalModelData::Instance.ComponentEdit.cADModel_ptr = m_AvmComponentModel;

    // get the model-item from the model
    ProModelitem modelItem;
    switch(status = ProMdlToModelitem(m_AvmComponentModel, &modelItem))
    {
    case PRO_TK_NO_ERROR:
    {
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "The ProMdlToModelitem() was successful: "
                              << " id: " <<  modelItem.id
                              << " owner: " << (const void*)modelItem.owner
                              << " type: " << modelItem.type;
        break;
    }
    case PRO_TK_BAD_INPUTS:
    {
        isis::application_exception ex(string("An argument is NULL.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_INVALID_PTR:
    {
        isis::application_exception ex(string("The handle is invalid. ")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    }

    // provide creo with the component properties

    GlobalModelData::Instance.ComponentEdit.avmId = in_AvmComponentID;

    ProParamvalue avmComponentId_value;
    avmComponentId_value.type = PRO_PARAM_STRING;
    ProStringToWstring(avmComponentId_value.value.s_val, const_cast< char* >(in_AvmComponentID.c_str()));

    status = ProMdlDisplay(m_AvmComponentModel);
    // display the specified model in the current view
    switch(status)
    {
    case PRO_TK_NO_ERROR:
    {
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProMdlDisplay() successfully displayed the model.";
        break;
    }
    case PRO_TK_E_NOT_FOUND:
    {
        isis::application_exception ex(string("The model is NULL, and there is no current object.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_INVALID_PTR:
    {
        isis::application_exception ex(string("The specified model is not in memory.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_GENERAL_ERROR:
    {
        isis::application_exception ex(string("There was a general error and the function failed.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_INVALID_TYPE:
    {
        isis::application_exception ex(string("You specified an invalid model type.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    }

    // find the window with the specified model as the top-level object
    status = ProMdlWindowGet(m_AvmComponentModel, &windowID);
    switch(status)
    {
    case PRO_TK_NO_ERROR:
    {
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProMdlWindowGet() successfully found the window.";
        break;
    }
    case PRO_TK_E_NOT_FOUND:
    {
        isis::application_exception ex(string("The function was not able to find the window.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    }

    // Set the current window
    status = ProWindowCurrentSet(windowID);
    switch(status)
    {
    case PRO_TK_NO_ERROR:
    {
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProWindowCurrentSet(): successfully set the window to be current. ";
        break;
    }
    case PRO_TK_BAD_INPUTS:
    {
        isis_LOG(lg, isis_FILE, isis_WARN) << "ProWindowCurrentSet(): invalid argument.";
        break;
    }
    }

    // activate the specified window
    status = ProWindowActivate(windowID);
    switch(status)
    {
    case PRO_TK_NO_ERROR:
    {
        isis_LOG(lg, isis_FILE, isis_DEBUG) << "ProWindowActivate() successfully activated the window.";
        break;
    }
    case PRO_TK_BAD_INPUTS:
    {
        isis::application_exception ex(string("The input argument is invalid.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_BAD_CONTEXT:
    {
        isis::application_exception ex(string("The function was called in RPC input (non-graphic) mode")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    case PRO_TK_GENERAL_ERROR:
    {
        isis::application_exception ex(string("The function failed.")  + "[" + __FILE__ + ": " + BOOST_PP_STRINGIZE(__LINE__) + "]");
        throw ex;
    }
    }

    // Make sure activate takes effect
    ProEventProcess();

    isis_LOG(lg, isis_FILE, isis_INFO) << "=== MetaLinkAssemblyEditor::InitAvmComponent  === " << " FINISHED ";
}

/**
Example of setting a Creo parameter. In this case we would set AVM_COMPONENT_ID.
  Solution: CADCreoParametricCreateAssembly
  File: BuildAssembly.cpp
  Function: Add_Subassemblies_and_Parts
*/
void MetaLinkAssemblyEditor::UpdateAvmComponentViaXML(const std::string  &in_AssemblyXMLString) throw(isis::application_exception)
{
    isis_LOG(lg, isis_FILE, isis_WARN) << " MetaLinkAssemblyEditor::UpdateComponentViaXML" << " NOT YET IMPLEMENTED";
}

} // END namespace isis

