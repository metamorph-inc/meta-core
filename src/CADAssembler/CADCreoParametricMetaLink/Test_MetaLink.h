#ifndef TEST_METALINK_H
#define TEST_METALINK_H

// The following should only be defined for testing.  Do not put this in SVN with this macro defined.
//#define TEST_META_LINK

#include <string>
#include <map>
#include "CADFactoryCreo.h"
namespace isis
{
void Test_CreateAssembly(std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
                         isis::MetaLinkAssemblyEditor &in_out_MetaLinkAssemblyEditor,
                         std::string &out_ParentAssemblyInstanceID)
throw(isis::application_exception);

void Test_AddComponentToAssembly(//cad::CadFactoryAbstract						&in_Factory,
								 const std::string								&in_ParentAssemblyInstanceID,
                                 std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
                                 isis::MetaLinkAssemblyEditor					&in_out_MetaLinkAssemblyEditor)
																				throw(isis::application_exception);

void Test_ModifyParameters(const std::string &in_ParentAssemblyInstanceID,
                           std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
                           isis::MetaLinkAssemblyEditor &in_MetaLinkAssemblyEditor)
throw(isis::application_exception);

// Before calling Test_ConstrainComponent, you must first call Test_AddComponentToAssembly
void Test_ConstrainComponent(const std::string &in_ParentAssemblyInstanceID,
                             std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
                             isis::MetaLinkAssemblyEditor &in_MetaLinkAssemblyEditor)
throw(isis::application_exception);

void Test_AddSerachPaths(const std::list<std::string> &in_SearchPaths,
                         isis::MetaLinkAssemblyEditor &in_MetaLinkAssemblyEditor) throw(isis::application_exception);


}  // END namespace isis


#endif

