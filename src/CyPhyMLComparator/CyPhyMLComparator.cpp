#include "UdmComparator.hpp"
#include "CyPhyML.h"

int main( int argc, char **argv ) {
	try
	{
		if (argc < 2) {
			std::cerr << "Usage: CyPhyMLComparator project1.mga project2.mga" << std::endl;
			return 2;
		}

		Udm::SmartDataNetwork udmSmartDataNetwork1( CyPhyML::diagram );
		udmSmartDataNetwork1.OpenExisting( argv[1], "", Udm::CHANGES_LOST_DEFAULT );

		Udm::Object rootObject1 = udmSmartDataNetwork1.GetRootObject();


		Udm::SmartDataNetwork udmSmartDataNetwork2( CyPhyML::diagram );
		udmSmartDataNetwork2.OpenExisting( argv[2], "", Udm::CHANGES_LOST_DEFAULT );

		Udm::Object rootObject2 = udmSmartDataNetwork2.GetRootObject();


		UdmComparator udmComparator;

		UdmComparator::StringSet exclusiveClassNameSet;
		exclusiveClassNameSet.insert( "ReferenceCoordinateSystem" );

		UdmComparator::StringStringSetMap exclusiveClassNameAttributeNameSetMap;
		UdmComparator::StringSet exclusiveAttributeNameSet;
		exclusiveAttributeNameSet.insert( "position" );
		exclusiveClassNameAttributeNameSetMap.insert(  std::make_pair( "MgaObject", exclusiveAttributeNameSet )  );

		exclusiveAttributeNameSet.clear();
		exclusiveAttributeNameSet.insert( "InstanceGUID" );
		exclusiveClassNameAttributeNameSetMap.insert(  std::make_pair( "ComponentRef", exclusiveAttributeNameSet )  );
		
		exclusiveAttributeNameSet.clear();
		exclusiveAttributeNameSet.insert( "Path" );
		exclusiveClassNameAttributeNameSetMap.insert(  std::make_pair( "Component", exclusiveAttributeNameSet )  );

		UdmComparator::ClassNameFilter classNameFilter;
		classNameFilter.setExclusiveClassNameSet( exclusiveClassNameSet );
		classNameFilter.setExclusiveClassNameAttributeNameMap( exclusiveClassNameAttributeNameSetMap );
		udmComparator.setClassNameFilter( classNameFilter );
		
		bool result = true;
		result = udmComparator.compareNode( rootObject1, rootObject2 );

		std::cerr << UdmComparator::Report::get_singleton().getString() << std::endl;

		udmSmartDataNetwork1.CloseNoUpdate();
		udmSmartDataNetwork2.CloseNoUpdate();

		return result ? 0 : 1;
	}
	catch (const udm_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 3;
	}
}