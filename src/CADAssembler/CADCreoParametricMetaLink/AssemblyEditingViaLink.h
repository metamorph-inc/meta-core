#ifndef ASSEMBLY_EDITING_VIA_LINK_H
#define ASSEMBLY_EDITING_VIA_LINK_H
#include <cc_MetaLinkInputArgumentsParser.h>
#include <boost/smart_ptr.hpp>
#include <isis_include_ptc_headers.h>
#include "CADFactoryAbstract.h"

namespace isis
{


class MetaLinkAssemblyEditor
{
public:
    typedef boost::shared_ptr<MetaLinkAssemblyEditor> Pointer;

    // Description:
    //		Constructor
    // Pre-Conditions:
    //		in_TopaAssemblyComponentID must either be NULL or a valid ComponentInstanceID
    //		in_out_CADComponentData_map must be a valid in-memory pointer throughout  the life of an
    //		object created from this class
    // Post-Conditions:
    //		Instantiated Object
    MetaLinkAssemblyEditor(cad::CadFactoryAbstract::ptr						in_cadfactory,
                           const isis::MetaLinkInputArguments				&in_ProgramInputArguments,
                           std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map);

    // Description:
    //      De-Select (remove highlight) for all objects
    // Pre-Conditions:
    //
    // Post-Conditions:
    //
    void ClearSelection();

    // Description:
    //      Select (highlight) the object referenced by the identifier
    // Pre-Conditions:
    //
    // Post-Conditions:
    //
    void SelectComponentOfAssembly(
        const std::string 				&in_ComponentInstanceId,
        std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
    throw(isis::application_exception);

    // Description:
    //      Select (highlight) the object referenced by the identifier
    // Pre-Conditions:
    //
    // Post-Conditions:
    //
    void SelectDatumOfComponent(
        const std::string               &in_FeatureGeometryType,
        const std::string 				&in_DatumName,
        std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
    throw(isis::application_exception);

    // Description:
    //      Locate a component instance in an assembly design
    //      by sending out a SELECT request via MetaLink
    // Pre-Conditions:
    //
    // Post-Conditions:
    //
    std::string LocateComponentOfAssembly(std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
    throw(isis::application_exception);

    // Description:
    //      Locate a datum in an AVM component
    //      by sending out a SELECT request via MetaLink
    // Pre-Conditions:
    //
    // Post-Conditions:
    //
    std::string LocateDatumOfComponent(std::vector<isis::CADCreateAssemblyError> &out_ErrorList)
    throw(isis::application_exception);

    // Description:
    //		Add a component (i.e. part/assembly) to the in_ParentAssemblyInstanceID assembly
    // Pre-Conditions:
    //		None
    // Post-Conditions:
    //		isis::application_exception will be thrown if one or more of the following conditions occur:
    //			1) the assembly identified by in_ParentAssemblyInstanceID could not be found. Not found
    //			   means that it does not exist in cADComponentData_map and/or it is not an assembly
    //			   in the currently active Creo assembly.   Note - A user could have closed the assembly
    //			   via the CREO UI.
    //			2) in_ComponentInstanceID already existing in cADComponentData_map.  A ComponentInstanceID
    //			   can appear no more than one times in an assembly.
    //			3) in_ParentAssemblyInstanceID does not exist in cADComponentData_map.
    //			4) in_ParentAssemblyInstanceID does exis but does not point to an assembly. This would occur
    //			   if in_ParentAssemblyInstanceID points to a part.
    //			5) in_CreoModelName exceeds 31 characters.
    //			6) the part/sub-assembly identified by in_CreoModelName-in_CreoModelType could not be found.  Not
    //			   found means that Creo could not find the part/sub-assembly.  This would be caused by the
    //			   part/sub-assembly not being in the working directory or a directory defined in search_META.pro.
    //		If no exceptions, then in_CreoModelName-in_CreoModelType would have been successfully added to
    //		the in_ParentAssemblyInstanceID assembly and cADComponentData_map updated to reflect that
    //		in_ComponentInstanceID is now part of the active Creo assembly.
    void AddComponentToAssembly(
		cad::CadFactoryAbstract			&in_Factory,
        const std::string 				&in_ComponentInstanceID,
        const std::string 				&in_CreoModelName,
		// ProMdlType                        in_CreoModelType,
		e_CADMdlType						in_CreoModelType,
        const std::string				&in_MaterialID,
        const std::string				&in_DisplayName,
        e_CADSpecialInstruction			in_SpecialInstruction,
        const std::vector<CADParameter>	&in_CADParameters,
        std::vector<isis::CADCreateAssemblyError> &out_ErrorList,
        std::vector<isis::CADComponentConnector> &in_ConnectorList)
    throw(isis::application_exception);

    // Needs Contract
    void ConstrainComponent(const std::string					&in_ConstraintComponentInstanceID,
                            const std::vector< ConstraintPair>  &in_ConstraintPairs)
    throw(isis::application_exception);

    // Update component name
    void UpdateComponentName(const std::string  &in_ComponentInstanceID,
                             const std::string &newName) throw(isis::application_exception);

    // Needs Contract
    void ModifyParameters(const std::string  &in_ComponentInstanceID,
                          const std::vector<CADParameter> &in_Parameters) throw(isis::application_exception);

    // This should be called only once per design ID
    void CreateAssembly(const std::string  &in_AssemblyXMLString) throw(isis::application_exception);

    // This should be called to clear out a design
    bool MetaLinkAssemblyEditor::Clear()  throw(isis::application_exception);

    // if in_SearchPaths.size(0) == 0, then no action would be taken. An exception would not be thrown
    void AddSearchPaths(const std::list<std::string> &in_SearchPaths) throw(isis::application_exception);


    // Called to initialize an avm component
    void InitAvmComponent(const std::string in_AvmComponentID, const std::string creoModelName,
                          const std::string  creoAvmComponentName, const ProMdlType creoModelType) throw(isis::application_exception);

    // May be called repeatedly to change a component via CyPhy
    void UpdateAvmComponentViaXML(const std::string  &in_AssemblyXMLString) throw(isis::application_exception);

    // Deselect all selected items
    void UnHighlightAll();

    // Select a part/assembly
    void HighlightPart(const std::string ID);

    // Select a datum
    void HighlightDatum(const std::string parentID, const std::string datumName);

private:
    //::log4cpp::Category& m_logcat;

    cad::CadFactoryAbstract::ptr m_cadfactory;

    // Don't allow default constructor
    MetaLinkAssemblyEditor();
    std::map<std::string, isis::CADComponentData>	&m_CADComponentData_map;
    std::string										topAssemblyComponentInstanceID;  // Null if top assembly does not exist (i.e. assembly not created yet).
    unsigned int									m_uniqueNameIndex;
    int											    windowID;
    const isis::MetaLinkInputArguments               m_programInputArguments;
    std::string										designID;
    std::set<std::string>							searchPaths;
    ProMdl                                          m_AvmComponentModel;

    int												m_addedToAssemblyOrdinal;


};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateAssemblyViaString(cad::CadFactoryAbstract				&in_factory,
                             const isis::MetaLinkInputArguments				&in_ProgramInputArguments,
                             const std::string								&in_XMLInputFile_String,
                             unsigned int									&in_out_UniqueNameIndex,
                             const std::string								&in_DesingID,
                             isis::CADAssemblies								&out_CADComponentAssemblies,
                             std::map<std::string, isis::CADComponentData>	&out_CADComponentData_map,
                             std::vector<CADCreateAssemblyError>				&errorList)
throw(isis::application_exception);

} // END namespace isis

#endif