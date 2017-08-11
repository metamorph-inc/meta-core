#include <AssemblyEditingViaLink.h>
#include <ToolKitPassThroughFunctions.h>
#include <BuildAssembly.h>
#include <SetCADModelParameters.h>
#include <CommonUtilities.h>
#include <AssembleUtils.h>
#include <ParametricParameters.h>
#include <ApplyModelConstraints.h>
#include <XMLToProEStructures.h>
#include <ProEStructuresUtils.h>
#include <ProWindows.h>
#include <ProMdl.h>
#include <ProAnnotation.h>
#include <ProSelbuffer.h>
#include <DiagnosticUtilities.h>
#include <ISISConstants.h>
#include <CommonFunctions.h>
#include <CommonStructures.h>
#include <ProUIMessage.h>
#include <algorithm>  
#include <sstream>
#include <time.h>
#include <log4cpp/Category.hh>

#include "UIFunctions.h"
#include "isis_ptc_toolkit_ostream.h"
#include "CommonDefinitions.h"
#include "GlobalModelData.h"


namespace isis
{

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                        Class MetaLinkAssemblyEditor
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MetaLinkAssemblyEditor::MetaLinkAssemblyEditor(
                        const isis::ProgramInputArguments              &in_ProgramInputArguments,
                        std::map<std::string, isis::CADComponentData> &in_CADComponentData_map) :
                                                m_logcat( ::log4cpp::Category::getInstance(std::string("metalink.assembler.editor")) ),
                                                m_programInputArguments(in_ProgramInputArguments),
                                                m_CADComponentData_map(in_CADComponentData_map),
                                                m_uniqueNameIndex(0)
    {
        m_logcat.infoStream()  
            << ::log4cpp::eol << "*****************************************************************************************"
            << ::log4cpp::eol << "**************************** Starting META-Link Mode  ***********************************"
            << ::log4cpp::eol << "*****************************************************************************************"
            << ::log4cpp::eol << "****************** Start MetaLinkAssemblyEditor::MetaLinkAssemblyEditor *****************";


          designID = in_ProgramInputArguments.designID;
         //ProMdl    p_model;
         //isis::isis_ProMdlRetrieve(m_CADComponentData_map[in_TopAssemblyComponentInstanceID].name,PRO_MDL_ASSEMBLY, &p_model);


        //isis::isis_ProMdlDisplay( m_CADComponentData_map[in_TopAssemblyComponentInstanceID].modelHandle);    

            
        //int windowID;
        //isis::isis_ProMdlWindowGet( m_CADComponentData_map[in_TopAssemblyComponentInstanceID].modelHandle, &windowID);

        // Can't do this since the CreateAssembly command will come later isis::isis_ProWindowActivate(windowID);
        // Can't do this since the CreateAssembly command will come laterisis::isis_ProWindowCurrentSet(windowID);

        /*
            bool regenerationSucceeded = false;
            isis::RegenerateModel(     m_CADComponentData_map[in_TopAssemblyComponentInstanceID].modelHandle,
                         m_CADComponentData_map[in_TopAssemblyComponentInstanceID].name,
                         m_CADComponentData_map[in_TopAssemblyComponentInstanceID].componentID,
                         regenerationSucceeded);
*/
        m_logcat.infoStream() << "isis::isis_ProWindowCurrentSet(windowID), windowID: " << windowID;
        m_logcat.infoStream() << "******************** End MetaLinkAssemblyEditor::MetaLinkAssemblyEditor *****************";

        //isis::isis_ProWindowRefresh(windowID);    
        //isis::isis_ProWindowRepaint(windowID);
        //isis::isis_ProWindowRefresh(windowID);    
        //isis::isis_ProMdlDisplay( m_CADComponentData_map[in_TopAssemblyComponentInstanceID].modelHandle);    
        
        m_logcat.infoStream() << "Created MetaLinkAssemblyEditor";

    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::ClearSelection() {    
        m_logcat.debugStream() << "clearing highlights";
		ProError status;
        switch( status = ProWindowRepaint(PRO_VALUE_UNUSED) ) {
        case PRO_TK_NO_ERROR:
            m_logcat.debugStream() << "window repainted" ;
            break;
        case PRO_TK_BAD_INPUTS:
            m_logcat.errorStream() << "the current window is invalid";
            throw isis::application_exception("C09007", "one or more input arguments were invalid");
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * Highlight the named datum and the containing component.
    * If the datum doesn't exist, still highlight the component.
    */

    void MetaLinkAssemblyEditor::SelectComponentOfAssembly (    
                                    const std::string                 &in_ParentAssemblyInstanceId,
                                    const std::string                 &in_ComponentInstanceId,
                                    std::vector<isis::CADCreateAssemblyError> &out_ErrorList) 
                                                            throw (isis::application_exception) {
        log4cpp::Category& logcat = log4cpp::Category::getInstance(std::string("metalink.select.component"));
        logcat.info("select request has been received");

        ProError status;
        ProModelitem selectedComponent;
        ProModelitem selectedDatum;
        ProSelection selection;

        std::map<std::string, isis::CADComponentData>::const_iterator parentIx = m_CADComponentData_map.find(topAssemblyComponentInstanceID);
        if ( parentIx == m_CADComponentData_map.end() ) {
            logcat.warnStream()  
                   << "Function MetaLinkAssemblyEditor::SelectComponentOfAssembly invoked, but ParentAssemblyInstanceID does not exist in the assembly. "
                   <<  in_ParentAssemblyInstanceId << " is-equal-to " << topAssemblyComponentInstanceID;
            throw isis::application_exception("C08002", "parent assembly instance ID is not present");
        }
		logcat.infoStream() << "found top assembly: " << parentIx->second.name;
        ProMdl top_handle = parentIx->second.modelHandle;

        std::map<std::string, isis::CADComponentData>::const_iterator componentIx = m_CADComponentData_map.find(in_ComponentInstanceId);
        if ( componentIx == m_CADComponentData_map.end() ) {
            logcat.warnStream()  
                   << "Function MetaLinkAssemblyEditor::SelectComponentOfAssembly invoked, but ComponentInstanceID does not exist in the assembly. "
                   <<  in_ComponentInstanceId;
            throw isis::application_exception("C08002", "component instance ID is not present");
        }
		logcat.infoStream() << "found component: " << componentIx->second.name;

		ProAsmcomppath assemblyPath;
		switch( status = Retrieve_ProAsmcomppath((ProSolid)top_handle,
			componentIx->second.componentPaths,
			assemblyPath) ) {
		case PRO_TK_NO_ERROR: 
            logcat.debugStream() << "selection of component";
            break; 
        case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09004", "one or more input arguments were invalid");
		}

        logcat.infoStream() << "highlight the component";
        ProMdl component_handle = componentIx->second.modelHandle;
        switch( status = ProMdlToModelitem ( component_handle, &selectedComponent ) ) {
        case PRO_TK_NO_ERROR: 
            logcat.debugStream() << "selection of component";
            break; 
        case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09004", "one or more input arguments were invalid");
        case PRO_TK_NOT_EXIST:
            logcat.errorStream() << "model item does not exist";
            throw isis::application_exception("C09014", "Item with such id and type does not exist");
        }
        switch( status = ProSelectionAlloc(&assemblyPath, &selectedComponent, &selection)) {
        case PRO_TK_NO_ERROR: 
            logcat.debugStream() << "selection of component";
            break; 
        case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09004", "one or more input arguments were invalid");
        }
		//ProSelect(
        switch( status = ProSelectionHighlight(selection, ProColortype::PRO_COLOR_HIGHLITE) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "component highlighted";
            break;
        case PRO_TK_BAD_CONTEXT:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09005", "one or more input arguments were invalid");
        }

        logcat.infoStream() << "selection complete";
        switch( status = ProWindowRefresh(PRO_VALUE_UNUSED) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "window refreshed" ;
            break;
        case PRO_TK_BAD_CONTEXT:
            logcat.errorStream() << "the current view is not valid";
            throw isis::application_exception("C09007", "one or more input arguments were invalid");
        }
        switch( status = ProDetailtreeRefresh((ProSolid)component_handle, PRO_VALUE_UNUSED) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "the detail tree is refreshed" ;
            break;
		case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more of the input arguments are invalid.";
            throw isis::application_exception("C09008", "one or more input arguments were invalid");
		case PRO_TK_NO_CHANGE:
            logcat.errorStream() << "There is no change in the detail tree.";
			break;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::SelectDatumOfComponent (    
                                   	const std::string 				&in_AvmComponentId,
									const std::string               &in_FeatureGeometryType,
									const std::string 				&in_DatumName,
                                    std::vector<isis::CADCreateAssemblyError> &out_ErrorList) 
                                                            throw (isis::application_exception) {

        log4cpp::Category& logcat = log4cpp::Category::getInstance(std::string("metalink.select.datum"));
        logcat.info("select datum request has been received");
        ProError status;

        ProMdl componentHandle = m_AvmComponentModel;
        ProModelitem selectedDatum;
        ProSelection selection;

        logcat.infoStream() << "highlight the datum";
        if ( in_DatumName.size() > (PRO_NAME_SIZE - 1) ) {
            logcat.warnStream()  
                   << "Function MetaLinkAssemblyEditor::SelectComponentOfAssembly invoked, but DatumName is too long. "
                   <<  in_DatumName;
            throw isis::application_exception("C08005", "datum name is too long");
        }
        ProName datumName;
        ProStringToWstring(datumName, const_cast< char* >(in_DatumName.c_str()));

        ProType type = FeatureGeometryType_enum( in_FeatureGeometryType );
        switch ( status = ProModelitemByNameInit(componentHandle, type, datumName, &selectedDatum)) {
        case PRO_TK_NO_ERROR: 
            logcat.debugStream() << "selected datum";
            break; 
        case PRO_TK_E_NOT_FOUND:
            logcat.errorStream() << "The datum was not found: " << datumName;
            throw isis::application_exception("C09002", "datum not found by name");
			break;
        case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09003", "one or more input arguments were invalid");
        }
        switch( status = ProSelectionAlloc(NULL, &selectedDatum, &selection)) {
        case PRO_TK_NO_ERROR: 
            logcat.debugStream() << "selection of datum";
            break; 
        case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09004", "one or more input arguments were invalid");
        }
        switch( status = ProSelectionHighlight(selection, ProColortype::PRO_COLOR_HIGHLITE) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "datum highlighted";
            break;
        case PRO_TK_BAD_CONTEXT:
            logcat.errorStream() << "One or more input arguments was invalid";
            throw isis::application_exception("C09005", "one or more input arguments were invalid");
		}

        logcat.infoStream() << "selection complete";
        switch( status = ProWindowRefresh(PRO_VALUE_UNUSED) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "window refreshed" ;
            break;
        case PRO_TK_BAD_CONTEXT:
            logcat.errorStream() << "the current view is not valid";
            throw isis::application_exception("C09007", "one or more input arguments were invalid");
        }
        switch( status = ProDetailtreeRefresh((ProSolid)componentHandle, PRO_VALUE_UNUSED) ) {
        case PRO_TK_NO_ERROR:
            logcat.debugStream() << "the detail tree is refreshed" ;
            break;
		case PRO_TK_BAD_INPUTS:
            logcat.errorStream() << "One or more of the input arguments are invalid.";
            throw isis::application_exception("C09008", "one or more input arguments were invalid");
		case PRO_TK_NO_CHANGE:
            logcat.errorStream() << "There is no change in the detail tree.";
			break;
        }
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::AddComponentToAssembly (    
                                    const std::string                &in_ParentAssemblyInstanceID,
                                    const std::string                 &in_ComponentInstanceID,
                                    const std::string                 &in_CreoModelName,
                                    ProMdlType                        in_CreoModelType,
                                    const std::string                &in_MaterialID,
                                    e_CADSpecialInstruction            in_SpecialInstruction,
                                    const std::vector<CADParameter>    &in_CADParameters,
                                    std::vector<isis::CADCreateAssemblyError> &out_ErrorList ) 
                                                throw (isis::application_exception)
    {

        m_logcat.infoStream() << "***************** Begin MetaLinkAssemblyEditor::AddComponentToAssembly ******************";
        std::stringstream generalMsg;

        // 8/14/2013 Tempory fix to the problem that CyPhy does not have a way to pass the assembly instnce ID in
        // a consistend manner.  For now, only allow parts/sub-assemblies to be added to the top
        // assembly.
        std::string parentAssemblyInstanceID = topAssemblyComponentInstanceID;

        m_logcat.infoStream() << "Setting parentAssemblyInstanceID to topAssemblyComponentInstanceID." 
                        << ::log4cpp::eol << "  topAssemblyComponentInstanceID: " << topAssemblyComponentInstanceID; 

        //std::string parentAssemblyInstanceID = in_ParentAssemblyInstanceID;

        generalMsg  <<  std::endl << "    programInputArguments.designID: " << m_programInputArguments.designID
                    <<  std::endl << "    topAssemblyComponentInstanceID: " << topAssemblyComponentInstanceID
                    <<  std::endl << "    in_ParentAssemblyInstanceID:    " << in_ParentAssemblyInstanceID 
                    <<  std::endl << "    Temporary workaround, parentAssemblyInstanceID set to topAssemblyComponentInstanceID"
                    <<  std::endl << "    parentAssemblyInstanceID:       " << parentAssemblyInstanceID 
                    <<  std::endl << "    in_ComponentInstanceID:         " << in_ComponentInstanceID
                    <<  std::endl << "    in_CreoModelName:               " << in_CreoModelName
                    <<  std::endl << "    in_MaterialID:                  " << in_MaterialID
                    <<  std::endl << "    in_CreoModelType:               " << ProMdlType_string(in_CreoModelType)
                    <<  std::endl << "    in_SpecialInstruction:          " << in_SpecialInstruction;


        // 8/14/2013 Tempory fix, since we will use the topAssemblyComponentInstanceID (see the comment above),
        // at least make sure the sent designID and the actual design ID match.  in_ParentAssemblyInstanceID
        // should be the DesignIK
        if ( in_ParentAssemblyInstanceID != m_programInputArguments.designID )
        {
            std::stringstream errorString;
            errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly was passed an incorrect DesignID, " <<
                        std::endl << "Passed DesignID:    " << in_ParentAssemblyInstanceID <<
                        std::endl << "Expected DesignID:  " << m_programInputArguments.designID;
            throw isis::application_exception("C08012", errorString);
        }        


       m_logcat.infoStream() << "************* Begin Input Arguments *********************";

        m_logcat.infoStream() << "Adding component to the assembly: " <<  generalMsg.str();
        for each ( CADParameter i in in_CADParameters) m_logcat.infoStream()  << i;

        m_logcat.infoStream() << "*************** End Input Arguments *********************";

        m_logcat.infoStream() << "Adding Creo model"  << in_CreoModelName;

        /////////////////////////////
        // Begin Input Data Checks
        /////////////////////////////
        m_logcat.infoStream() << "******************** Begin Checks ***********************";
        // Verify that a top assembly exists in this class
        if ( topAssemblyComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but no assemblies have been created. "
                        <<      generalMsg.str();
            m_logcat.errorStream() << errorString.str();
            throw isis::application_exception("C08001", errorString);                          ////
        }
        m_logcat.infoStream() << " Top level assembly exists in the internal data structures"; 


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
        if ( m_CADComponentData_map.find(parentAssemblyInstanceID) == m_CADComponentData_map.end() )
        {
            std::stringstream errorString;
            errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but parentAssemblyInstanceID does not exists in the assembly. "
                        <<  generalMsg.str();
            throw isis::application_exception("C08002", errorString);               ///////
        }
        else
        {  
            m_logcat.infoStream() << " Parent assembly exists in the internal data structures"; 

            // We know parentAssemblyInstanceID exists in the assembly, now verify that parentAssemblyInstanceID is an assembly
            std::map<std::string, isis::CADComponentData>::const_iterator itr;
            itr = m_CADComponentData_map.find(parentAssemblyInstanceID );
            if ( itr != m_CADComponentData_map.end() && itr->second.modelType != PRO_ASSEMBLY )
            {
                std::stringstream errorString;
                errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but parentAssemblyInstanceID is not an assembly. "
                            <<  generalMsg.str();
                throw isis::application_exception("C08003", errorString);          
            }
        }
        m_logcat.infoStream() << " Parent assembly is a Creo assembly (i.e. not a part)"; 

        // Verify in_ComponentInstanceID does not exist in the assembly.  If it does exist, then an attempt is
        // being made to add it a second time.  A ComponentInstanceID can exist one and only one time in a assembly. 
        if ( m_CADComponentData_map.find(in_ComponentInstanceID) != m_CADComponentData_map.end() )
        {
            std::stringstream errorString;
            errorString << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but in_ComponentInstanceID already exists in the assembly. "
                        << "A ComponentInstanceID can exist one and only one time in a assembly. "
                        <<  generalMsg.str();
            throw isis::application_exception("C08004", errorString);
        }
        m_logcat.infoStream() << " Add component instance does not already exist in the assembly"; 

        // Make sure in_CreoModelName is not too long
        if ( in_CreoModelName.size() > (PRO_NAME_SIZE - 1) )
        {
            std::stringstream errorString;
            errorString 
                << "Function MetaLinkAssemblyEditor::AddComponentToAssembly invoked, but in_CreoModelName is longer than " 
                << (PRO_NAME_SIZE - 1) << " characters." 
                <<  generalMsg.str();
            throw isis::application_exception("C08005", errorString);
        }
        m_logcat.infoStream() << " Creo model name is within the allowable length of: " << (PRO_NAME_SIZE - 1) << " characters."; 
        m_logcat.infoStream() << "********************* End Checks ************************";
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
        componentData.materialID =            in_MaterialID;
        componentData.specialInstruction =    in_SpecialInstruction;

        // Add parameters
        for each ( CADParameter i in in_CADParameters) componentData.parametricParameters.push_back(i);

        if (in_CADParameters.size() > 0 )
        {
            componentData.parametricParametersPresent = true;
            // For a parametric part, must check if it occurs more than once, if so, must copy to a new name
            // zzz Should probably make existingModelNames a attribute of this class so that it does not have to be
            // created each time.
            std::list<std::string> existingModelNames;
            for each ( std::pair<std::string, isis::CADComponentData> i in m_CADComponentData_map )
                                    existingModelNames.push_back(isis::ConvertToUpperCase(i.second.name));
            
            if ( std::count (existingModelNames.begin(), existingModelNames.end(), isis::ConvertToUpperCase(in_CreoModelName) ) > 0  )
            {

                // Model name already occurs.  Must copy it to a new name.
                std::string origNameWithoutFamilyEntry;
                std::string modelName;
                std::string completeName;

                ++m_uniqueNameIndex;
                
                CreateModelNameWithUniqueSuffix(    m_uniqueNameIndex, 
                                                    in_CreoModelName,
                                                    origNameWithoutFamilyEntry,
                                                    modelName, 
                                                    completeName );        

				m_logcat.infoStream() << "Parametric part/sub-assembly rename: " 
                         << log4cpp::eol << "   Old Name:                          " << in_CreoModelName
                         << log4cpp::eol << "   New Name:                          " << modelName
                         << log4cpp::eol << "   Family Table Name (if applicable): " << completeName;

                ///std::cout << std::endl << "CreateModelNameWithUniqueSuffix:modelName BB" << modelName << "EE"; 

                // Copy Model

                ProMdl     p_model;         
                //zzz does this work with family tables
                isis::MultiFormatString  newModelName(modelName, PRO_NAME_SIZE - 1);
                // Since the source model could be anywhere in the search path, we
                // must open the source model and save it to force a copy to exist in the working directory.
                isis::isis_ProMdlRetrieve(componentData.name, PRO_MDL_PART, &p_model);
                isis::isis_ProMdlSave(p_model);
                isis::isis_ProMdlfileCopy (PRO_MDL_PART, componentData.name, newModelName);
                componentData.name = completeName;  // This would include the family table entry.

                //std::cout << std::endl << "componentData.name: BB" << componentData.name << "EE";
            }

        }        

        //std::pair<std::string, isis::CADComponentData>  cADComponentData_pair(in_ComponentInstanceID,componentData);
        // Add cADComponentData_pair to a tempory map
        std::map<std::string, isis::CADComponentData>    cADComponentData_map_TEMP; 
        //cADComponentData_map_TEMP.insert(cADComponentData_pair);

        cADComponentData_map_TEMP[in_ComponentInstanceID] = componentData;
      
        try
        {
            //m_logcat.infoStream()  << "*********** Begin Temp Structure - Call to Creo SDK to Add the Component **************";
            //m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle: "         << m_CADComponentData_map[parentAssemblyInstanceID].modelHandle;
            //m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
            //m_logcat.infoStream() << "cADComponentData_map_TEMP[in_ComponentInstanceID]:                   " << cADComponentData_map_TEMP[in_ComponentInstanceID];
            //m_logcat.infoStream()  << "************ End Temp Structure - Call to Creo SDK to Add the Component ***************";
            
            isis::Add_Subassemblies_and_Parts(    m_CADComponentData_map[parentAssemblyInstanceID].modelHandle,
                                                m_CADComponentData_map[parentAssemblyInstanceID].name,
                                                toAddComponentInstanceIDs, 
                                                cADComponentData_map_TEMP);

            //isis::isis_ProMdlDisplay( cADComponentData_map_TEMP[in_ComponentInstanceID].modelHandle);    

            bool regenerationSucceeded = false;
            //isis::RegenerateModel(     m_CADComponentData_map[parentAssemblyInstanceID].modelHandle,
            //             m_CADComponentData_map[parentAssemblyInstanceID].name,
            //             m_CADComponentData_map[parentAssemblyInstanceID].componentID,
            //             regenerationSucceeded);
            
            m_logcat.infoStream() << "*********** Begin Temp Structure - Call to Creo SDK to Add the Component **************";
            m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle:      " << m_CADComponentData_map[parentAssemblyInstanceID].modelHandle;
            m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
            m_logcat.infoStream() << "cADComponentData_map_TEMP[in_ComponentInstanceID]:                 " << cADComponentData_map_TEMP[in_ComponentInstanceID];
            m_logcat.infoStream() << "************ End Temp Structure - Call to Creo SDK to Add the Component ***************";

            //for each ( std::string j in toAddComponentInstanceIDs )
            //{
            //    m_logcat.infoStream() << "toAddComponentInstanceID: " << j;
            //    m_logcat.infoStream() << "cADComponentData_map_TEMP[j].p_model: " << cADComponentData_map_TEMP[j].p_model;
            //}
            isis::ApplyParametricParameters( toAddComponentInstanceIDs, cADComponentData_map_TEMP, out_ErrorList );

            isis::RegenerateModel(     m_CADComponentData_map[parentAssemblyInstanceID].modelHandle,
                         m_CADComponentData_map[parentAssemblyInstanceID].name,
                         m_CADComponentData_map[parentAssemblyInstanceID].componentID,
                         regenerationSucceeded);

            isis::isis_ProWindowRepaint(windowID);

            //int windowID;
            //isis::isis_ProMdlWindowGet( m_CADComponentData_map[parentAssemblyInstanceID].modelHandle, &windowID);
            //isis::isis_ProWindowCurrentSet(windowID);
        

        }
        catch (... )
        {
            // The exception could have been because the Creo model was not found or because a parameter was not found.
            // For the latter case, the model would have been added to the assembly and we need to delete it.
            // The ProMdlDelete will not throw an exception if the model could not be found.
            if ( cADComponentData_map_TEMP[in_ComponentInstanceID].p_model != 0 ) ProMdlDelete(cADComponentData_map_TEMP[in_ComponentInstanceID].p_model);
            throw;
        }                                

        // If it made it to here, then the component was successfully added 
        // Add the information to the actial data structures
        m_CADComponentData_map[in_ComponentInstanceID] = cADComponentData_map_TEMP[in_ComponentInstanceID];
        m_CADComponentData_map[parentAssemblyInstanceID].children.push_back(in_ComponentInstanceID);

        m_logcat.infoStream() << "******************* Begin Final Structure - Add the Component *************************";
        m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle:      " << m_CADComponentData_map[parentAssemblyInstanceID].modelHandle;
        m_logcat.infoStream() << "m_CADComponentData_map[parentAssemblyInstanceID].modelHandle.name: " << m_CADComponentData_map[parentAssemblyInstanceID].name;
        m_logcat.infoStream() << "m_CADComponentData_map[in_ComponentInstanceID]:                    " << m_CADComponentData_map[in_ComponentInstanceID];
        m_logcat.infoStream() << "******************* End Final Structure - Add the Component *************************";

        m_logcat.infoStream() << "Successfully added component to the assembly:" <<  generalMsg.str();
        m_logcat.infoStream() << m_CADComponentData_map.find(parentAssemblyInstanceID)->second;

        m_logcat.infoStream() << "******************* End MetaLinkAssemblyEditor::AddComponentToAssembly ******************";

    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::ConstrainComponent (    const std::string                    &in_ConstraintComponentInstanceID,
                                                        const std::vector< ConstraintPair>  &in_ConstraintPairs ) 
                                                                                throw (isis::application_exception)
    {
        m_logcat.infoStream() << "***************** Begin MetaLinkAssemblyEditor::ConstrainComponent ******************";
        m_logcat.infoStream() << "*************** Begin Input Arguments *********************";

        if ( topAssemblyComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "exception : Function MetaLinkAssemblyEditor::ConstrainComponent invoked, but no assemblies have been created. ";
            m_logcat.errorStream() << errorString.str();
            throw isis::application_exception("C08013", errorString);
        }

        m_logcat.infoStream() << "in_ConstraintComponentInstanceID: " << in_ConstraintComponentInstanceID;
        for each (    ConstraintPair i  in in_ConstraintPairs)
        {
            m_logcat.infoStream() << "      constraintFeatures:";
            for each (    ConstraintFeature j in i.constraintFeatures )
            {
                m_logcat.infoStream() 
                   << "         component:               " <<        j.componentInstanceID  << ::log4cpp::eol
                   << "         featureName:             " <<        j.featureName  << ::log4cpp::eol
                   << "         featureOrientationType:  " <<        ProDatumside_string( j.featureOrientationType );

            }
        }
        m_logcat.infoStream() << "*************** End Input Arguments *********************";

        /////////////////////////////
        // Begin Input Data Checks
        /////////////////////////////

        // Verify in_ComponentInstanceID is not an empty string
        if ( in_ConstraintComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "MetaLinkAssemblyEditor::ConstrainComponent invoked, but in_ComponentInstanceID is an empty string."
                        << "A ComponentInstanceID must exist before a constraint can be added.";
            throw isis::application_exception("C08006",errorString);
        }

        // Verify in_ComponentInstanceID does not exists.  If it does exist, then an attempt is
        // being made to modify a parameter on a component that does not exist
        if ( m_CADComponentData_map.find(in_ConstraintComponentInstanceID) == m_CADComponentData_map.end() )
        {
            std::stringstream errorString;
            errorString << "MetaLinkAssemblyEditor::ConstrainComponent invoked, but in_ComponentInstanceID does not exist in the assembly. "
                        << "A ComponentInstanceID must exist before a constraint can be added. in_ComponentInstanceID: " << in_ConstraintComponentInstanceID;
            throw isis::application_exception("C08007",errorString);
        }
        /////////////////////////////
        // End Input Data Checks
        /////////////////////////////

        m_logcat.infoStream() << "Constraining Creo model:"  << m_CADComponentData_map[in_ConstraintComponentInstanceID].name;

        ///////////////////////////////////////////////
        // Add the contraints to m_CADComponentData_map
        ///////////////////////////////////////////////

        // zzz If already constrained, cannot allow a second constraint.  In the future will support deleting
        // constraints.  For now, throw an exception if trying to constrain a component that is already
        // constrained.
        m_logcat.infoStream() << "Number of Constraints: "<< m_CADComponentData_map[topAssemblyComponentInstanceID].constraintDef.constraints.size();
        if ( m_CADComponentData_map[in_ConstraintComponentInstanceID].constraintDef.constraints.size() > 0 )
        {
            std::stringstream errorString;
            errorString << "exception : Function MetaLinkAssemblyEditor::ConstrainComponent invoked, but component is already constrained, in_ConstrainedComponentInstanceID:  " << in_ConstraintComponentInstanceID;
            throw isis::application_exception("C08008",errorString);
        }
        
        ConstraintData temp_ConstraintData;
        for each ( ConstraintPair i in in_ConstraintPairs) temp_ConstraintData.constraintPairs.push_back(i);
        m_CADComponentData_map[in_ConstraintComponentInstanceID].constraintDef.constraints.push_back(temp_ConstraintData);

        ///////////////////////
        // Constrain Model
        //////////////////////
        bool firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound = false;
        if ( m_CADComponentData_map.size() < 2 ) firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound = true;

        std::list<std::string>                componentIDsToBeConstrained;
        componentIDsToBeConstrained.push_back(in_ConstraintComponentInstanceID);


        m_logcat.infoStream() << "**** Before call to ApplyModelConstraints ******";
        m_logcat.infoStream() << m_CADComponentData_map[in_ConstraintComponentInstanceID];

        ApplyModelConstraints(    &m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle,                            
                                componentIDsToBeConstrained,  
                                m_CADComponentData_map,
                                // Provide for the case where the first assembled part does not have
                                // the datums front, top, and right defined. 
                                firstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound );

        bool regenerationSucceeded;

        isis::RegenerateModel(    m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle,
                         m_CADComponentData_map[topAssemblyComponentInstanceID].name,
                         m_CADComponentData_map[topAssemblyComponentInstanceID].componentID,
                         regenerationSucceeded);

        isis::isis_ProWindowRepaint(windowID);

        m_logcat.infoStream() << "**** After call to ApplyModelConstraints ******";

        m_logcat.infoStream() << "Constraint applied successfully";
        m_logcat.infoStream() << "***************** End MetaLinkAssemblyEditor::ConstrainComponent ******************";
		/////////////////////////////
		// Begin Input Data Checks
		/////////////////////////////
		if ( in_ConstraintComponentInstanceID.size() == 0 )
		{
			std::stringstream errorString;
			errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID is an empty string."
						<< "A ComponentInstanceID must exist before a parameter can be modeified.";
			isis::application_exception ex = isis::application_exception("C08009",errorString);
			throw ex;
		}

    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::ModifyParameters (  const std::string  &in_ComponentInstanceID,
                                                    const std::vector<CADParameter> &in_Parameters) throw (isis::application_exception)

    {
        m_logcat.infoStream() << "********************* Begin MetaLinkAssemblyEditor::ModifyParameter **********************";
        if ( topAssemblyComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "exception : Function MetaLinkAssemblyEditor::ModifyParameters invoked, but no assemblies have been created. ";
            m_logcat.errorStream() << errorString.str();
            throw isis::application_exception("C08014", errorString);
        }

        m_logcat.infoStream() << "************* Begin Input Arguments *********************";
        m_logcat.infoStream() << "   in_ComponentInstanceID: " << in_ComponentInstanceID;
        for each ( CADParameter i in in_Parameters)
        {
            m_logcat.infoStream() << i;
        }
        m_logcat.infoStream() << "************* End Input Arguments *********************";


        /////////////////////////////
        // Begin Input Data Checks
        /////////////////////////////
        if ( in_ComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID is an empty string."
                        << "A ComponentInstanceID must exist before a parameter can be modeified.";
            throw isis::application_exception("C08009",errorString);
        }


        // Verify in_ComponentInstanceID does not exists.  If it does exist, then an attempt is
        // being made to modify a parameter on a component that does not exist
        if ( m_CADComponentData_map.find(in_ComponentInstanceID) == m_CADComponentData_map.end() )
        {
            std::stringstream errorString;
            errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID does not exist in the assembly. "
                        << "A ComponentInstanceID must exist before a parameter can be modifed on that component. in_ComponentInstanceID: " << in_ComponentInstanceID;
            throw isis::application_exception("C08010",errorString);
        }
        /////////////////////////////
        // End Input Data Checks
        /////////////////////////////

        // Verify in_ComponentInstanceID is not an empty string
        if ( in_ComponentInstanceID.size() == 0 )
        {
            std::stringstream errorString;
            errorString << "MetaLinkAssemblyEditor::ModifyParameters invoked, but in_ComponentInstanceID is an empty string."
                        << "A ComponentInstanceID must exist before a constraint can be added.";
            throw isis::application_exception("C08011",errorString);
        }

        std::string modelNameWithSuffix = AmalgamateModelNameWithSuffix ( 
                                                m_CADComponentData_map[in_ComponentInstanceID].name, 
                                                m_CADComponentData_map[in_ComponentInstanceID].modelType );

        m_logcat.infoStream() << "Internal data structure parameter values BEFORE modification:";
        for each ( CADParameter i in m_CADComponentData_map[in_ComponentInstanceID].parametricParameters)
        {
            m_logcat.infoStream() << i;
        }

        // Change the paraqmeters
        for each (CADParameter i in in_Parameters)
        {
            SetParametricParameter( modelNameWithSuffix, 
                                    m_CADComponentData_map[in_ComponentInstanceID].p_model,
                                    i.name, i.type, i.value);
        }

        bool regenerationSucceeded;
        isis::RegenerateModel(     m_CADComponentData_map[in_ComponentInstanceID].modelHandle,
                         m_CADComponentData_map[in_ComponentInstanceID].name,
                         m_CADComponentData_map[in_ComponentInstanceID].componentID,
                         regenerationSucceeded);

        isis::isis_ProWindowRepaint(windowID);

        // We must update the parameters in m_CADComponentData_map[in_ComponentInstanceID].parametricParameters
        // as follows:
        //    a) if the parameter already exists, must update its value
        //    b) if the parameter does not exist, add it
        for each (CADParameter i in in_Parameters)
        {
            std::list<CADParameter>::iterator itr;
            itr = find(m_CADComponentData_map[in_ComponentInstanceID].parametricParameters.begin(),
                       m_CADComponentData_map[in_ComponentInstanceID].parametricParameters.end(),
                       i );

            if ( itr != m_CADComponentData_map[in_ComponentInstanceID].parametricParameters.end() )
            {
                // Found, must modify
                itr->type    = i.type;    // would be an error if the types are different
                itr->value    = i.value;
            }
            else
            {
                // Not found, must add
                m_CADComponentData_map[in_ComponentInstanceID].parametricParameters.push_back(i);
            }
        }

        m_logcat.infoStream() << "Internal data structure parameter values AFTER modification:";
        for each ( CADParameter i in m_CADComponentData_map[in_ComponentInstanceID].parametricParameters)
        {
            m_logcat.infoStream() << i;
        }

        m_logcat.infoStream() << "********************* End MetaLinkAssemblyEditor::ModifyParameter ************************";
    }




    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::CreateAssembly(  const std::string  &in_AssemblyXMLString ) throw (isis::application_exception)
    {
		GlobalModelData::Instance.Clear();
        std::vector<isis::CADCreateAssemblyError> errorList;
        CreateAssemblyViaString( m_programInputArguments,
                                 in_AssemblyXMLString,
                                 m_uniqueNameIndex,
                                 designID,
                                 GlobalModelData::Instance.CadAssemblies,
                                 m_CADComponentData_map,
                                 errorList );

		GlobalModelData::Instance.designId = designID;
        if ( GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.size() != 1 )
        {
            std::stringstream errorString;
            errorString << "One and only one assembly allowed in MetaLink mode.  The XML defines " 
				<< GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.size() << std::string(" assemblies.");
                throw isis::application_exception("C08015", errorString);
        }
        topAssemblyComponentInstanceID = GlobalModelData::Instance.CadAssemblies.topLevelAssemblies.begin()->assemblyComponentID;
        m_logcat.infoStream()
            << " top assembly component instance id SET : " << topAssemblyComponentInstanceID;

        bool hasCritical = false;
        if (errorList.size()!=0)
        {
            ostringstream ostr;
            m_logcat.infoStream() << "Error list from MetaLinkAssemblyEditor::CreateAssembly:";
            for (std::vector<isis::CADCreateAssemblyError>::iterator it = errorList.begin(); it != errorList.end(); ++it)
            {
                m_logcat.infoStream() << it->Text;
                ostr << it->Text << std::endl;
                if (it->Severity == CADCreateAssemblyError_Severity_Critical)
                    hasCritical = true;
            }
            CreoMessageDialog(ostr.str(), PROUIMESSAGE_WARNING);
        }



        if (hasCritical)
        {
            isis::application_exception ex("Critical error(s) has been encountered during model construction. The model can't be opened.");
            throw ex;
        }

		GlobalModelData::Instance.mode = DESIGNEDIT;

        //ProMdl p_model;
        //isis::isis_ProMdlRetrieve( m_CADComponentData_map[topAssemblyComponentInstanceID].name,PRO_MDL_ASSEMBLY, &p_model);

        isis::isis_ProMdlDisplay( m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle);

        isis::isis_ProMdlWindowGet( m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle, &windowID);

        isis::isis_ProWindowActivate(windowID);
        isis_ProWindowCurrentSet(windowID);

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CreateAssemblyViaString(    const isis::ProgramInputArguments              &in_ProgramInputArguments,
                                     const std::string                              &in_XMLInputFile_String,
									 unsigned int									&in_out_UniqueNameIndex,
                                     const std::string                              &in_DesingID,
                                     isis::CADAssemblies                            &out_CADComponentAssemblies,
                                     std::map<std::string, isis::CADComponentData>  &out_CADComponentData_map,
                                     std::vector<CADCreateAssemblyError>            &out_ErrorList) 
                                                                                        throw (isis::application_exception)
    {
        log4cpp::Category& logcat = log4cpp::Category::getInstance(std::string("metalink.selection"));
		logcat.infoStream() << "raw xml:" << ::log4cpp::eol << in_XMLInputFile_String;

        bool Template_Copied = false;
        int ExitCode = 0;
        try
        {
            time_t time_start; /* calendar time */
            time_start=time(NULL); /* get current cal time */

            std::string cadPartsLibDir = in_ProgramInputArguments.auxiliaryCADDirectory;

            bool regenerationSucceeded_ForAllAssemblies = true;

            // Verify that the XML file is not null.
            if ( in_XMLInputFile_String.size() == 0 )
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


            if ( out_CADComponentAssemblies.topLevelAssemblies.size() == 0 ) 
            {
                logcat.infoStream() << "*************************** Begin Assembly Creation **************************";
                logcat.infoStream() << "No assemblies were created because the input xml did not define any assemblies."; 
                if ( out_CADComponentAssemblies.unassembledComponents.size() == 0 )
                {
                    std::stringstream errorString;
                    errorString <<
                            "The input xml file did not define any assemblies nor did it define unassembled parts/sub-assemblies.  "
                             << std::endl << "The input xml file must specify assemblies and/or unassembled parts/sub-assemblies."; 
                            throw isis::application_exception(errorString);
                }
                logcat.infoStream() << "************************** End Assembly Creation *****************************";
			}
			// Rename parametric parts that have multiple instances of the same part.
			//std::map<std::string, std::string>  ToPartName_FromPartName_map; 
			//isis::ModifyToHaveAUniqueNameForEachParametricPartOrAssembly( in_out_UniqueNameIndex, out_CADComponentData_map, ToPartName_FromPartName_map );
			std::vector<CopyModelDefinition>			fromModel_ToModel;
			isis::ModifyToHaveAUniqueNameForEachParametricPartOrAssembly(	in_out_UniqueNameIndex, 
																		e_PART_OR_ASSEMBLY_MODEL_TYPE,
																		e_SELECT_ONLY_PARAMETRIC_MODELS,
																		out_CADComponentData_map, 
																		fromModel_ToModel );

			logcat.infoStream()  << "************** Begin Modified Part Names for Multiple Parametric Parts *****************";
			logcat.infoStream()  << "From_Part_Name   To_Part_Name";
			logcat.infoStream() << fromModel_ToModel;
			logcat.infoStream()  << "************** End Modified Part Names for Multiple Parametric Parts *****************";

            // Add the depends-on information to the CADComponentData
            isis::Add_dependsOn( out_CADComponentData_map );

            for each( isis::TopLevelAssemblyData i in out_CADComponentAssemblies.topLevelAssemblies)
            {

                logcat.infoStream() << "************** Begin Entire Tree For a Single Assembly  *****************";
                logcat.infoStream() << "assemblyComponentID: " << i.assemblyComponentID;
                logcat.infoStream() << "************** Begin Analysis Data For a Single Assembly  *****************";
                stream_AnalysisInputData( i.analysesCAD, clog);
                logcat.infoStream() << "************** End Analysis Data For a Single Assembly  *****************";

                logcat.infoStream() << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";
                stream_AssemblyCADComponentData( i.assemblyComponentID, out_CADComponentData_map, clog );
                logcat.infoStream() << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";

                logcat.infoStream() << "************** End Entire Tree For a Single Assembly  *****************";
            }


            ///////////////////////////////////////////////////////////////////////////////////
            // Check for Non-Size-To-Fit components being constrained to Size-To-Fit components
            ////////////////////////////////////////////////////////////////////////////////////
            std::set<std::string> TempIntersection = NonSizeToFitComponents_ReferencedBy_SizeToFitConstraints( out_CADComponentData_map );
            if ( TempIntersection.size() > 0 )
            {
                string err_str = "Erroneous XML File: A NON_SIZE_TO_FIT component cannot be constrained to a SIZE_TO_FIT component. " +
                    std::string("The SIZE_TO_FIT component(s) (i.e. ComponentIDs) that were erroneously referenced are:");
                for ( std::set<std::string>::const_iterator i(TempIntersection.begin()); i != TempIntersection.end(); ++i )
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
			if ( fromModel_ToModel.size() > 0 ) isis::CopyModels(fromModel_ToModel);

            isis::MultiFormatString workingDir_MultiFormat(in_ProgramInputArguments.workingDirectory, PRO_PATH_SIZE - 1);
            //isis::isis_ProDirectoryChange( workingDir_MultiFormat );
            isis::setCreoWorkingDirectory( workingDir_MultiFormat );

            std::string TemplateFile_PathAndFileName = META_PATH() + "\\bin\\CAD\\Creo\\templates\\" + isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX;

            // Copy template model to the working directory
            isis::CopyFileIsis( TemplateFile_PathAndFileName,  in_ProgramInputArguments.workingDirectory );
            Template_Copied = true;

            // *** If model renames occured
            // WARNING - isis::CopyModels must be called after 
            //             1) Creo has started
            //             2) isis_ProDirectoryChange changed to the working directory
            //           been created.
            //             3) the search_META.pro has been set 
            if ( fromModel_ToModel.size() > 0 ) isis::CopyModels(fromModel_ToModel);


            ////////////////////////////////////////
            // Build the assemblies
            ////////////////////////////////////////
            for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(out_CADComponentAssemblies.topLevelAssemblies.begin());
                  i != out_CADComponentAssemblies.topLevelAssemblies.end();
                  ++i )
            {
                bool regenerationSucceeded;
                isis::BuildAssembly( i->assemblyComponentID, in_ProgramInputArguments.workingDirectory, false, out_CADComponentData_map, regenerationSucceeded, out_ErrorList );

                if ( !regenerationSucceeded ) regenerationSucceeded_ForAllAssemblies = false;

                //////////////////////////////////////
                // Display assembly created messages
                ////////////////////////////////////
                time_t time_end;        /* calendar time */
                time_end=time(NULL);    /* get current cal time */

                time_t time_elapsed = time_end - time_start;

                logcat.infoStream() << "Assembly creation completed successfully.";

                // Get component count
                isis::ComponentVistorCountAssemblyComponents  componentVistorCountAssemblyComponents;
                isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, componentVistorCountAssemblyComponents);

                // Log component count
                logcat.infoStream() << "   Number of assembled components: " << componentVistorCountAssemblyComponents.numberOfComponents;        
                logcat.infoStream() << "   Elapsed wall-clock time:        " << time_elapsed << " seconds";

                time_start=time(NULL); // reset start time for subsequent assemblies if any
            }  // END  Build the assemblies

        } // END Try

        catch ( isis::application_exception& ex )
        {
            throw;
        }
        catch ( std::exception& ex )
        {
            throw;
        }
        catch ( ... )
        {
            std::stringstream    exceptionErrorStringStream;
            exceptionErrorStringStream << " std::exception: Caught exception (...).  Please report the error to the help desk.";
            throw isis::application_exception(exceptionErrorStringStream);
        }

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool MetaLinkAssemblyEditor::ClearAssembly() {
        m_logcat.infoStream() << "MetaLinkAssemblyEditor::ClearAssembly(): starting ";

        bool success = true;
        ProError status;
        stringstream msg("ClearAssembly");

        ProMdl model_handle = m_CADComponentData_map[topAssemblyComponentInstanceID].modelHandle;
        if (model_handle == NULL) {
            m_logcat.infoStream() << " nothing to clear " << topAssemblyComponentInstanceID;
            return true;
        }
        m_logcat.infoStream() << " top: " << topAssemblyComponentInstanceID << " handle: " << model_handle;

        switch ( status = ProMdlEraseAll(model_handle) ) {
        case PRO_TK_NO_ERROR: 
            msg << "erased all models";
            m_logcat.infoStream() << msg.str();
            isis_ProMdlEraseNotDisplayed();  // R.O. Added 9/12/2013
            break;
        case PRO_TK_BAD_INPUTS:
            msg << "The model handle is defective: " << model_handle;
            m_logcat.infoStream() << msg.str();
            throw isis::application_exception(msg);
        case PRO_TK_BAD_CONTEXT:
            msg << "Current User Interface context does not allow "
                << "erasure of models (for example, when the Erase button is grayed out).";
            m_logcat.infoStream() << msg.str();
            throw isis::application_exception(msg);
        case PRO_TK_E_IN_USE:
            msg << "The model could not be erased because it is in use, "
                << "for example by another model in the session. ";
            m_logcat.infoStream() << msg.str();
            throw isis::application_exception(msg);
        }

        // Empty out the assembler data structures.
        m_CADComponentData_map.clear();
        topAssemblyComponentInstanceID = "";
        m_logcat.infoStream() << " top assembly component instance id CLEARED";
        m_uniqueNameIndex = 0;
        searchPaths.clear();

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::UnHighlightAll()
    {
        ProSelbufferClear();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void MetaLinkAssemblyEditor::AddSearchPaths(const std::list<std::string> &in_SearchPaths) throw (isis::application_exception)
    {
        ProName        optionName;
        ProPath        optionValue;

        try
        {
            for each ( std::string i in in_SearchPaths )
            {
                m_logcat.infoStream() << "Adding search path: " << i;
                if ( i.size() != 0 )
                {
                    std::string searchPathString = i;

                    // Check if searchPathString is in double quotes.  If not add double quotes.
                    // This is necessary because Creo Config options with spaces must be enclosed in double quotes.
                    if ( searchPathString.find("\"")  == string::npos )    searchPathString = "\"" + searchPathString + "\"";

                    std::string searchPathString_uppercase = isis::ConvertToUpperCase(searchPathString);
                    if ( searchPaths.find( searchPathString_uppercase ) == searchPaths.end() )
                    {
                        MultiFormatString  searchPath_MultiFormat(searchPathString, PRO_PATH_SIZE - 1);
                        wcscpy( optionName, L"search_path");
                        wcscpy( optionValue, searchPath_MultiFormat);
                        isis_ProConfigoptSet(optionName, optionValue  );
                        m_logcat.infoStream() << "Added serach path: " << searchPathString;
                        searchPaths.insert(searchPathString_uppercase);
                    }
                    else
                    {
                        std::stringstream errorString;
                        errorString << "Search path already set in the Creo model. No action taken.  Search path: " << searchPathString;
                        m_logcat.infoStream() << errorString.str();
                    }
                }
                else
                {
                    std::stringstream errorString;
                    errorString << "MetaLinkAssemblyEditor::AddSerachPaths received an empty search path.  This is not considered a error.  Empty search paths are ignored";
                    m_logcat.infoStream() << errorString.str();
                }
            }
        }
        catch ( isis::application_exception& ex )
        {
            std::stringstream    exceptionErrorStringStream;
            exceptionErrorStringStream << "exception : Function: MetaLinkAssemblyEditor::AddSerachPaths, std::application_exception. ";
            throw isis::application_exception("C08016", exceptionErrorStringStream);
        }
        catch ( std::exception& ex )
        {
            std::stringstream    exceptionErrorStringStream;
            exceptionErrorStringStream << "exception : Function: MetaLinkAssemblyEditor::AddSerachPaths, std::exception: ";
            throw isis::application_exception("C08017", exceptionErrorStringStream);
        }
        catch ( ... )
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
    void MetaLinkAssemblyEditor::InitAvmComponent(  const std::string in_AvmComponentID, const std::string in_CreoModelName, 
                const std::string  in_CreoAvmComponentName, const ProMdlType in_CreoModelType ) throw (isis::application_exception)
    {
        m_logcat.infoStream() << "=== MetaLinkAssemblyEditor::InitAvmComponent  === " << " BEGIN ";
        ProError status;

        // load the avm component creo file
        ProName creoName;
        ProStringToWstring(creoName, const_cast< char* >(in_CreoAvmComponentName.c_str()));
        switch ( status = ProMdlRetrieve( creoName, in_CreoModelType, &m_AvmComponentModel) ) {
        case  PRO_TK_NO_ERROR:
            {
            m_logcat.infoStream() << "The function successfully retrieved the model.";
            break;
            }
        case  PRO_TK_BAD_INPUTS:
            {
                isis::application_exception ex("One or more of the input arguments are invalid.");
                throw ex;
            }
        case PRO_TK_E_NOT_FOUND:
            {
                isis::application_exception ex("The model was not found in the current directory.");
                throw ex;
            }
        case PRO_TK_NO_PERMISSION:
            {
                isis::application_exception ex("The function does not have permission to operate on this model.");
                throw ex;
            }
        }   
        /*
        switch ( status = ProMdlSave ( m_AvmComponentModel ) ) {
        case PRO_TK_NO_ERROR: 
            {
            m_logcat.infoStream() << "The function successfully stored the model along with " 
                      << "all its subordinate models, according to their "
                      << "modification status and the configuration option "
                      << "'save_objects'. ";
            break;
            }
        case PRO_TK_CANT_WRITE: 
            {
                stringstream error("Cannot Write: ");
                error << "Failed to save the model or at least one of its "
                    << "subordinate models due to unresolved non-overridable "
                    << "conflicts in the target location. To obtain the exact "
                    << "list of models that were not saved a notification on "
                    << "PRO_MDL_DBMS_FAILURE event can be applied.";
                error << status;
                isis::application_exception ex(error);
            }
        case PRO_TK_GENERAL_ERROR:  
            {
                stringstream error("General Error: ");
                error << "The entire operation failed.";
                error << status;
                isis::application_exception ex(error);
            }
        case PRO_TK_BAD_INPUTS:  
            {
                stringstream error("Bad Inputs: ");
                error << "The argument is invalid.";
                error << status;
                isis::application_exception ex(error);
            }
        case PRO_TK_NO_PERMISSION:  
            {
                stringstream error("No Permission: ");
                error << "The function does not have permission to operate on this model";
                error << status;
                isis::application_exception ex(error);
            }
        case PRO_TK_BAD_CONTEXT:    
            {
                stringstream error("Bad Context: ");
                error << "The function is not available in Design Test Mode.   ";
                error << status;
                isis::application_exception ex(error);
            }
        default:
            {
                stringstream error("could not save.");
                error << status;
                isis::application_exception ex(error);
                throw ex;
            }
        }
        */

		isis::GlobalModelData::Instance.mode = isis::COMPONENTEDIT;
		isis::GlobalModelData::Instance.ComponentEdit.mdl = m_AvmComponentModel;
            
        // get the model-item from the model
        ProModelitem modelItem;
        switch ( status = ProMdlToModelitem(m_AvmComponentModel, &modelItem) ) {
        case PRO_TK_NO_ERROR: 
            {
                m_logcat.infoStream() << "The ProMdlToModelitem() was successful: "
					<< " id: " <<  modelItem.id 
					<< " owner: " << modelItem.owner
					<< " type: " << modelItem.type;
            break;
            }
        case PRO_TK_BAD_INPUTS: 
            {
                std::stringstream    msg;
                msg    << "An argument is NULL, "
                    << " model: " << m_AvmComponentModel 
                    << " model-item: " << modelItem;
                isis::application_exception ex(msg);
                m_logcat.errorStream() << ex;
                throw ex;
            }
        case PRO_TK_INVALID_PTR: 
            {
                isis::application_exception ex("The handle is invalid. ");
                m_logcat.errorStream() << ex;
                throw ex;
            }
        }

        // provide creo with the component properties

		GlobalModelData::Instance.ComponentEdit.avmId = in_AvmComponentID;

        ProParamvalue avmComponentId_value;
        avmComponentId_value.type = PRO_PARAM_STRING;
        ProStringToWstring(avmComponentId_value.value.s_val, const_cast< char* >(in_AvmComponentID.c_str()));
            
         // display the specified model in the current view
         switch ( status = ProMdlDisplay(  m_AvmComponentModel ) ) {
         case PRO_TK_NO_ERROR:
            {
                m_logcat.infoStream() << "successfully displayed the model.";
                break;
            }
         case PRO_TK_E_NOT_FOUND:
             {
                isis::application_exception ex("The model is NULL, and there is no current object.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         case PRO_TK_INVALID_PTR:
             {
                isis::application_exception ex("The specified model is not in memory.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         case PRO_TK_GENERAL_ERROR:
             {
                isis::application_exception ex("There was a general error and the function failed.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         case PRO_TK_INVALID_TYPE:
             {
                isis::application_exception ex("You specified an invalid model type.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         }

         // find the window with the specified model as the top-level object
         switch ( status = ProMdlWindowGet( m_AvmComponentModel, &windowID) ) {
         case PRO_TK_NO_ERROR: break;
            {
                m_logcat.infoStream() << "successfully found the window.";
                break;
            }
         case PRO_TK_E_NOT_FOUND:
             {
                isis::application_exception ex("The function was not able to find the window.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         }

         // activate the specified window
         switch ( status = ProWindowActivate(windowID) ) {
         case PRO_TK_NO_ERROR: break;
            {
                m_logcat.infoStream() << "The function successfully activated the window.";
                break;
            } 
         case PRO_TK_BAD_INPUTS:
             {
                isis::application_exception ex("The input argument is invalid.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         case PRO_TK_BAD_CONTEXT:
             {
                isis::application_exception ex("The function was called in RPC input (non-graphic) mode");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         case PRO_TK_GENERAL_ERROR:
             {
                isis::application_exception ex("The function failed.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         }

         // set the current window 
         switch ( status = ProWindowCurrentSet(windowID) ) {
         case PRO_TK_NO_ERROR: break;
             {
                m_logcat.infoStream() << "successfully set the window to be current. ";
            }
        case PRO_TK_BAD_INPUTS:
             {
                isis::application_exception ex("The argument is invalid.");
                m_logcat.errorStream() << ex;
                throw ex;
            }
         }
        
        m_logcat.infoStream() << "=== MetaLinkAssemblyEditor::InitAvmComponent  === " << " FINISHED ";
    }

    /**
    Example of setting a Creo parameter. In this case we would set AVM_COMPONENT_ID. 
      Solution: CADCreoParametricCreateAssembly 
      File: BuildAssembly.cpp 
      Function: Add_Subassemblies_and_Parts 
    */
    void MetaLinkAssemblyEditor::UpdateAvmComponentViaXML(  const std::string  &in_AssemblyXMLString ) throw (isis::application_exception)
    {
        m_logcat.infoStream() << " MetaLinkAssemblyEditor::UpdateComponentViaXML" << " NOT YET IMPLEMENTED";
    }

} // END namespace isis

