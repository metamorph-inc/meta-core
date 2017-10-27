#ifndef CC_COMMON_FUNCTIONS_H
#define CC_COMMON_FUNCTIONS_H
#include <string>
#include <list>
#include <map>
#include "cc_CommonStructures.h"

namespace isis
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateXMLFile_RequestedMetrics( 
						const	std::string									&in_PathAndFileName,
						const	std::list<TopLevelAssemblyData>				&in_TopLevelAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class ComponentVistor
	{
		public:
			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )=0;
	};


	class ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData) = 0;
	};


	void VisitComponents(	const std::string								&in_ComponentID, 
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
							ComponentVistor									&in_componentVistor) 
																		throw (isis::application_exception);


	class ComponentVistorMaterialTokens: public ComponentVistor
	{
		public:
			//std::map<std::string, AnalysisTempMaterial> materialKey_MaterialDefintion_map;
			std::map<std::string, double> componentID_PoissonsRatio_map;
			std::map<std::string, std::string> materialKey_ComponentID_map;

			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );
	};

	class ComponentVistorCountAssemblyComponents: public ComponentVistor
	{
		public:
			int numberOfComponents;

			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );

			ComponentVistorCountAssemblyComponents();

	};

	class ComponentVistorBuildListOfComponentIDs: public ComponentVistor
	{
		public:
			std::vector<std::string> listOfComponentIDs;
			virtual void operator() ( const std::string  &in_ComponentID, 
					std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );

			ComponentVistorBuildListOfComponentIDs(bool excludeTopAssembly = false);
			ComponentVistorBuildListOfComponentIDs( ComponentPredicate &in_Predicate, bool in_ExcludeTopAssembly = false );
		private:
			bool flagExcludeEntry;
			ComponentPredicate &predicate;

	};

	class SelectAllComponents : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};

	class SelectComponentDerivedFromLeafAssembly : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};

	class SelectComponentInInputXML : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};


	class SelectLeafAssemblies : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif