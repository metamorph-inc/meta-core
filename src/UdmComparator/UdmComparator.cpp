#include "UdmComparator.hpp"
#include <boost/lexical_cast.hpp>
#include "UmlExt.h"

UdmComparator::NameUmlAttributeMap UdmComparator::getNameUmlAttributeMap( Uml::Class umlClass ) {

	NameUmlAttributeMap nameUmlAttributeMap;

	UmlAttributeSet umlAttributeSet = umlClass.attributes();

	for( UmlAttributeSet::iterator uasItr = umlAttributeSet.begin() ; uasItr != umlAttributeSet.end() ; ++uasItr ) {
		Uml::Attribute umlAttribute = *uasItr;
		nameUmlAttributeMap.insert(  std::make_pair( umlAttribute.name(), umlAttribute )  );				
	}

	UmlClassSet umlBaseClassSet = umlClass.baseTypes();
	for( UmlClassSet::iterator ucsItr = umlBaseClassSet.begin() ; ucsItr != umlBaseClassSet.end() ; ++ucsItr ) {
		NameUmlAttributeMap baseClassNameUmlAttributeMap = getNameUmlAttributeMap( *ucsItr );
		nameUmlAttributeMap.insert( baseClassNameUmlAttributeMap.begin(), baseClassNameUmlAttributeMap.end() );
	}

	return nameUmlAttributeMap;
}

UdmComparator::UmlAttributeSet UdmComparator::getAllUmlAttributes( Uml::Class umlClass ) {

	UmlAttributeSet umlAttributeSet = umlClass.attributes();

	UmlClassSet umlBaseClassSet = umlClass.baseTypes();
	for( UmlClassSet::iterator ucsItr = umlBaseClassSet.begin() ; ucsItr != umlBaseClassSet.end() ; ++ucsItr ) {
		UmlAttributeSet baseClassUmlAttributeSet = getAllUmlAttributes( *ucsItr );
		umlAttributeSet.insert( baseClassUmlAttributeSet.begin(), baseClassUmlAttributeSet.end() );
	}

	return umlAttributeSet;
}

UdmComparator::UmlAssociationRoleSet UdmComparator::getAllUmlAssociationRoles( Uml::Class umlClass ) {

	UmlAssociationRoleSet umlAssociationRoleSet;

	if ( umlClass.stereotype() == "Connection" ) {
		Uml::Association umlAssociation = umlClass.association();
		if ( umlAssociation != Udm::null ) {
			umlAssociationRoleSet = umlAssociation.AssociationRole_kind_children();
		}
	} else {
		umlAssociationRoleSet = umlClass.associationRoles();
	}

	UmlClassSet umlBaseClassSet = umlClass.baseTypes();
	for( UmlClassSet::iterator ucsItr = umlBaseClassSet.begin() ; ucsItr != umlBaseClassSet.end() ; ++ucsItr ) {
		UmlAssociationRoleSet baseClassUmlAssociationRoleSet = getAllUmlAssociationRoles( *ucsItr );
		umlAssociationRoleSet.insert( baseClassUmlAssociationRoleSet.begin(), baseClassUmlAssociationRoleSet.end() );
	}

	return umlAssociationRoleSet;
}

std::string UdmComparator::ObjectName::operator()( Udm::Object udmObject ) {

	static StringSet reportedClassNameSet;

	Uml::Class umlClass = udmObject.type();
	Uml::Attribute umlNameAttribute;

	UmlClassNameAttributeMap::iterator cnmItr = _umlClassNameAttributeMap.find( umlClass );
	if ( cnmItr != _umlClassNameAttributeMap.end() ) {

		umlNameAttribute = cnmItr->second;

	} else {

		NameUmlAttributeMap nameUmlAttributeMap = getNameUmlAttributeMap( umlClass );
			
		NameUmlAttributeMap::iterator numItr = nameUmlAttributeMap.find( "name" );
		if ( numItr != nameUmlAttributeMap.end() ) {

			umlNameAttribute = numItr->second;

		} else {

			numItr = nameUmlAttributeMap.find( "Name" );
			if ( numItr != nameUmlAttributeMap.end() ) {

				umlNameAttribute = numItr->second;

			} else if (  static_cast< std::string >( umlClass.stereotype() ) != "Connection"  ) {

				std::string className = umlClass.name();
				if ( reportedClassNameSet.find( className ) == reportedClassNameSet.end() ) {
					std::cerr << "WARNING: Class \"" << className << "\" has no \"[Nn]ame\" attribute." << std::endl << std::endl;
					reportedClassNameSet.insert( className );
				}
				return "";

			}

		}

		_umlClassNameAttributeMap.insert(  std::make_pair( umlClass, umlNameAttribute )  );

	}

	std::string udmObjectName = "";
	if ( umlNameAttribute != Udm::null ) {
		udmObjectName = udmObject.getStringAttr( umlNameAttribute );
		if (  umlClass.stereotype() == "Connection" && udmObjectName == static_cast< std::string >( umlClass.name() )  ) {
			udmObjectName = "";
		}
	}

	if ( udmObjectName == "" && umlClass.stereotype() == "Connection" ) {

		UmlAssociationRoleSet umlAssociationRoleSet = getAllUmlAssociationRoles( umlClass );

		for( UmlAssociationRoleSet::iterator arsItr = umlAssociationRoleSet.begin() ; arsItr != umlAssociationRoleSet.end() ; ++arsItr ) {
			
			UdmObjectSet udmObjectSet = udmObject.getAssociation( *arsItr, Udm::TARGETFROMCLASS );
			for( UdmObjectSet::iterator uosItr = udmObjectSet.begin() ; uosItr != udmObjectSet.end() ; ++uosItr ) {

				if ( !udmObjectName.empty() ) udmObjectName += ":";
				udmObjectName += (*this)(*uosItr);

			}

		}

		if ( umlNameAttribute != Udm::null ) {
			udmObject.setStringAttr( umlNameAttribute, udmObjectName );
		}

	}

	Udm::Object udmObjectParent = udmObject.GetParent();

	if ( udmObjectParent != Udm::null ) {
		const auto& it = ObjectNameCache.find(udmObjectParent);
		if (it != ObjectNameCache.end())
		{
			udmObjectName = it->second + "/" + udmObjectName;
		}
		else
		{
			udmObjectName = operator()( udmObjectParent ) + "/" + udmObjectName;
		}
	}

	return udmObjectName;
}


bool UdmComparator::ChildObjectComparator::operator==(const ChildObjectComparator &object2 ) const {
	const Udm::Object& udmObject1 = this->object;
	const Udm::Object& udmObject2 = object2.object;

	if ( udmObject1 == Udm::null ) {
		return udmObject2 == Udm::null;
	}
	if ( udmObject2 == Udm::null ) {
		return false;
	}

	Uml::Class umlClass1 = udmObject1.type();
	Uml::Class umlClass2 = udmObject2.type();

	if (umlClass1.uniqueId() != umlClass2.uniqueId())
		return false;

	Uml::CompositionChildRole umlCompositionChildRole1;
	udmObject1.GetParent().GetChildRole( udmObject1, umlCompositionChildRole1 );
	Uml::CompositionChildRole umlCompositionChildRole2;
	udmObject2.GetParent().GetChildRole( udmObject2, umlCompositionChildRole2 );

	if (umlCompositionChildRole1.uniqueId() != umlCompositionChildRole2.uniqueId())
		return false;

	std::string udmObjectName1 = getObjectNameSingleton()( udmObject1 );
	std::string udmObjectName2 = getObjectNameSingleton()( udmObject2 );

	return udmObjectName1 == udmObjectName2;
}


bool UdmComparator::AssociationObject::operator ==(const AssociationObject &object2) const {
	const Udm::Object &udmObject1 = this->object;
	const Udm::Object &udmObject2 = object2.object;
	if ( udmObject1 == Udm::null ) {
		return udmObject2 == Udm::null;
	}
	if ( udmObject2 == Udm::null ) {
		return false;
	}

	Uml::Class umlClass1 = udmObject1.type();
	Uml::Class umlClass2 = udmObject2.type();

	if ( umlClass1.uniqueId() != umlClass2.uniqueId() ) return false;

	std::string udmObjectName1 = getObjectNameSingleton()( udmObject1 );
	std::string udmObjectName2 = getObjectNameSingleton()( udmObject2 );

	return udmObjectName1 == udmObjectName2;
}

UdmComparator::UdmObjectSet UdmComparator::ClassNameFilter::filterUdmObjectSet( const UdmObjectSet &udmObjectSet ) {

	UdmObjectSet filteredUdmObjectSet;
	
	if ( !_inclusiveClassNameSet.empty() ) {

		for( UdmObjectSet::const_iterator uosCit = udmObjectSet.begin() ; uosCit != udmObjectSet.end() ; ++uosCit ) {
			std::string className = uosCit->type().name();
			if ( _inclusiveClassNameSet.find( className ) != _inclusiveClassNameSet.end() ) {
				_includedClassNameSet.insert( className );
				filteredUdmObjectSet.insert( *uosCit );
			} else {
				_excludedClassNameSet.insert( className );
			}
		}

	} else if ( !_exclusiveClassNameSet.empty() ) {

		for( UdmObjectSet::const_iterator uosCit = udmObjectSet.begin() ; uosCit != udmObjectSet.end() ; ++uosCit ) {
			std::string className = uosCit->type().name();
			if ( _exclusiveClassNameSet.find( className ) == _exclusiveClassNameSet.end() ) {
				_includedClassNameSet.insert( className );
				filteredUdmObjectSet.insert( *uosCit );
			} else {
				_excludedClassNameSet.insert( className );
			}
		}

	} else {

		for( UdmObjectSet::const_iterator uosCit = udmObjectSet.begin() ; uosCit != udmObjectSet.end() ; ++uosCit ) {
			std::string className = uosCit->type().name();
			_includedClassNameSet.insert( className );
		}
		filteredUdmObjectSet = udmObjectSet;

	}

	return filteredUdmObjectSet;
}

UdmComparator::UdmObjectSet UdmComparator::ClassNameFilter::filterConnections( const UdmObjectSet &udmObjectSet ) {

	UdmObjectSet filteredUdmObjectSet;
	
	for( UdmObjectSet::const_iterator uosCit = udmObjectSet.begin() ; uosCit != udmObjectSet.end() ; ++uosCit ) {
		if ( uosCit->type().stereotype() == "Connection" ) continue;
		filteredUdmObjectSet.insert( *uosCit );
	}

	return filteredUdmObjectSet;
}

UdmComparator::UmlAttributeSet UdmComparator::ClassNameFilter::filterUmlAttributeSet( Uml::Class umlClass ) {

	UmlAttributeSet filteredUmlAttributeSet;

	UmlAttributeSet umlAttributeSet = umlClass.attributes();

	UmlClassSet umlBaseClassSet = umlClass.baseTypes();
	for( UmlClassSet::iterator ucsItr = umlBaseClassSet.begin() ; ucsItr != umlBaseClassSet.end() ; ++ucsItr ) {
		UmlAttributeSet baseClassUmlAttributeSet = filterUmlAttributeSet( *ucsItr );
		umlAttributeSet.insert( baseClassUmlAttributeSet.begin(), baseClassUmlAttributeSet.end() );
	}

	std::string umlClassName = umlClass.name();

	StringStringSetMap::iterator ssmItr = _inclusiveClassNameAttributeNameSetMap.find( umlClassName );
	if ( ssmItr != _inclusiveClassNameAttributeNameSetMap.end() ) {

		StringSet &stringSet = ssmItr->second;
		for( UmlAttributeSet::iterator uasItr = umlAttributeSet.begin() ; uasItr != umlAttributeSet.end() ; ++uasItr ) {
			if (  stringSet.find( uasItr->name() ) != stringSet.end()  ) {
				filteredUmlAttributeSet.insert( *uasItr );
			}
		}

	} else {

		StringStringSetMap::iterator ssmItr = _exclusiveClassNameAttributeNameSetMap.find( umlClassName );
		if ( ssmItr != _exclusiveClassNameAttributeNameSetMap.end() ) {

			StringSet &stringSet = ssmItr->second;
			for( UmlAttributeSet::iterator uasItr = umlAttributeSet.begin() ; uasItr != umlAttributeSet.end() ; ++uasItr ) {
				if (  stringSet.find( uasItr->name() ) == stringSet.end()  ) {
					filteredUmlAttributeSet.insert( *uasItr );
				}
			}

		} else {

			filteredUmlAttributeSet = umlAttributeSet;
		}

	}

	return filteredUmlAttributeSet;
}

bool UdmComparator::compareNode( Udm::Object udmObject1, Udm::Object udmObject2 ) {

	bool retval = compareNodeAux( udmObject1, udmObject2 );

	StringSet includedClassNameSet = _classNameFilter.getIncludedClassNameSet();
	std::cout << "Objects with the following class names were included in the comparison:" << std::endl;
	for( StringSet::iterator stsItr = includedClassNameSet.begin() ; stsItr != includedClassNameSet.end() ; ++stsItr ) {
		std::cout << *stsItr << std::endl;
	}
	std::cout << std::endl;

	StringSet excludedClassNameSet = _classNameFilter.getExcludedClassNameSet();
	std::cout << "Objects with the following class names were excluded from the comparison:" << std::endl;
	for( StringSet::iterator stsItr = excludedClassNameSet.begin() ; stsItr != excludedClassNameSet.end() ; ++stsItr ) {
		std::cout << *stsItr << std::endl;
	}
	std::cout << std::endl;

	return retval;
}

bool UdmComparator::compareNodeAux( Udm::Object udmObject1, Udm::Object udmObject2 ) {

	bool retval = true;

	_udmObjectMap.insert(  std::make_pair( udmObject1, udmObject2 )  );

	const Uml::Class &umlClass1 = udmObject1.type();
	const Uml::Class &umlClass2 = udmObject2.type();

	if ( umlClass1 != umlClass2 ) {
		std::cerr << "Classes of objects \"" << udmObject1.getPath( "/" ) << "\" and \"" << udmObject2.getPath( "/" ) << "\" do not match!" << std::endl << std::endl;
		return false;
	}

	getObjectNameSingleton().ObjectNameCache.insert(std::make_pair(udmObject1, getObjectNameSingleton()(udmObject1)));
	getObjectNameSingleton().ObjectNameCache.insert(std::make_pair(udmObject2, getObjectNameSingleton()(udmObject2)));

	UmlAttributeSet umlAttributeSet = _classNameFilter.filterUmlAttributeSet( umlClass1 );
	for( UmlAttributeSet::iterator uasItr = umlAttributeSet.begin() ; uasItr != umlAttributeSet.end() ; ++uasItr ) {
		Uml::Attribute umlAttribute = *uasItr;
		std::string umlAttributeName = umlAttribute.name();
		std::string value1, value2;
		
		std::string umlAttributeType = umlAttribute.type();
		if ( umlAttributeType == "Boolean" ) {
			value1 = boost::lexical_cast< std::string >(  udmObject1.getBooleanAttr( umlAttribute )  );
			value2 = boost::lexical_cast< std::string >(  udmObject2.getBooleanAttr( umlAttribute )  );
		} else if ( umlAttributeType == "Integer" ) {
			value1 = boost::lexical_cast< std::string >(  udmObject1.getIntegerAttr( umlAttribute )  );
			value2 = boost::lexical_cast< std::string >(  udmObject2.getIntegerAttr( umlAttribute )  );
		} else if ( umlAttributeType == "Real" ) {
			value1 = boost::lexical_cast< std::string >(  udmObject1.getRealAttr( umlAttribute )  );
			value2 = boost::lexical_cast< std::string >(  udmObject2.getRealAttr( umlAttribute )  );
		} else {
			value1 = udmObject1.getStringAttr( umlAttribute );
			value2 = udmObject2.getStringAttr( umlAttribute );
		}

		if ( value1 != value2 ) {
			Report::get_singleton().incrementDifferences( udmObject1 );
			std::cerr << "Value of \"" << umlAttributeName << "\" attribute value differs for \"" << udmObject1.getPath( "/" ) << "\" objects:  \"" << value1 << "\" vs. \"" << value2 << "\"" << std::endl << std::endl;
			retval = false;
		}
	}

	UdmObjectSet udmObjectSet1 = _classNameFilter.filterUdmObjectSet( udmObject1.GetChildObjects() );
	UdmObjectSet udmObjectSet2 = _classNameFilter.filterUdmObjectSet( udmObject2.GetChildObjects() );

	if ( udmObjectSet1.size() != udmObjectSet2.size() ) {
		std::cerr << "\"" << udmObject1.getPath( "/" ) << "\" objects have a different number of children:  " << udmObjectSet1.size() << "," << udmObjectSet2.size() << std::endl << std::endl;
		tallyChildrenClasses( udmObjectSet1, udmObjectSet2 );
		retval = false;
	}

	udmObjectSet1 = _classNameFilter.filterConnections( udmObjectSet1 );
	udmObjectSet2 = _classNameFilter.filterConnections( udmObjectSet2 );

	UdmChildObjectSet udmChildObjectSet1;
	udmChildObjectSet1.rehash(udmObjectSet1.size());
	std::copy(udmObjectSet1.begin(), udmObjectSet1.end(), std::inserter(udmChildObjectSet1, udmChildObjectSet1.begin()));
	if ( udmChildObjectSet1.size() != udmObjectSet1.size() ) {
		std::cerr << "Object \"" << udmObject1.getPath( "/" ) << "\" in the first  model has children that cannot be distinguished (duplicate name?)." << std::endl << std::endl;
		retval = false;
	}

	UdmChildObjectSet udmChildObjectSet2;
	udmChildObjectSet2.rehash(udmObjectSet2.size());
	std::copy(udmObjectSet2.begin(), udmObjectSet2.end(), std::inserter(udmChildObjectSet2, udmChildObjectSet2.begin()));
	if ( udmChildObjectSet2.size() != udmObjectSet2.size() ) {
		std::cerr << "Object \"" << udmObject2.getPath( "/" ) << "\" in the second model has children that cannot be distinguished (duplicate name?)." << std::endl << std::endl;
		retval = false;
	}

	UdmChildObjectSet::iterator ucsItr1 = udmChildObjectSet1.begin();
	
	for (; ucsItr1 != udmChildObjectSet1.end(); ++ucsItr1) {

		UdmChildObjectSet::iterator ucsItr2 = udmChildObjectSet2.find(*ucsItr1);

		if (ucsItr2 == udmChildObjectSet2.end()) {
			std::cerr << "No corresponding object for \"" << ucsItr1->object.getPath( "/" ) << "\" in first model found in second model." << std::endl;
			Report::get_singleton().incrementDifferences( udmObject1 );
			retval = false;
			continue;
		} 

		if (compareNodeAux(ucsItr1->object, ucsItr2->object) == false) {
			retval = false;
			Report::get_singleton().addToDifferences( udmObject1 );
		}
		udmChildObjectSet2.erase(ucsItr2);
	}

	UdmChildObjectSet::iterator ucsItr2 = udmChildObjectSet2.begin();
	for (; ucsItr2 != udmChildObjectSet2.end(); ++ucsItr2) {
		std::cerr << "No corresponding object for \"" << ucsItr2->object.getPath( "/" ) << "\" in second model found in first model." << std::endl;
		Report::get_singleton().incrementDifferences( udmObject1 );
		retval = false;
	}
//	if ( !retval ) return false;


	UmlAssociationRoleSet umlAssociationRoleSet = getAllUmlAssociationRoles( umlClass1 );
	for( UmlAssociationRoleSet::iterator arsItr = umlAssociationRoleSet.begin() ; arsItr != umlAssociationRoleSet.end() ; ++arsItr ) {

		Uml::AssociationRole umlAssociationRole = *arsItr;
		UdmObjectSet udmObjectSet1 = _classNameFilter.filterUdmObjectSet(   udmObject1.getAssociation(  Uml::theOther( umlAssociationRole )  )   );
		UdmObjectSet udmObjectSet2 = _classNameFilter.filterUdmObjectSet(   udmObject2.getAssociation(  Uml::theOther( umlAssociationRole )  )   );

		if ( udmObjectSet1.size() != udmObjectSet2.size() ) {
			std::cerr << "Objects \"" << udmObject1.getPath( "/" ) << "\" and \"" << udmObject2.getPath( "/" ) << "\"" << std::endl;
			std::cerr << "have a different number of \"" << umlAssociationRole.name() << "\" associations: " << udmObjectSet1.size() << " vs. " << udmObjectSet2.size() << std::endl << std::endl;
			retval = false;
		}

		UdmAssociationObjectSet udmAssociationObjectSet1;
		udmAssociationObjectSet1.rehash(udmObjectSet1.size());
		std::copy(udmObjectSet1.begin(), udmObjectSet1.end(), std::inserter(udmAssociationObjectSet1, udmAssociationObjectSet1.begin()));
		if ( udmAssociationObjectSet1.size() != udmObjectSet1.size() ) {
			std::cerr << "Object \"" << udmObject1.getPath( "/" ) << "\" has \"" << umlAssociationRole.name() << "\" associated objects that cannot be distinguished (probably due to duplicate name)." << std::endl << std::endl;
			retval = false;
		}

		UdmAssociationObjectSet udmAssociationObjectSet2;
		udmAssociationObjectSet2.rehash(udmObjectSet2.size());
		std::copy(udmObjectSet2.begin(), udmObjectSet2.end(), std::inserter(udmAssociationObjectSet2, udmAssociationObjectSet2.begin()));
		if ( udmAssociationObjectSet2.size() != udmObjectSet2.size() ) {
			std::cerr << "Object \"" << udmObject2.getPath( "/" ) << "\" has \"" << umlAssociationRole.name() << "\" associated objects that cannot be distinguished (probably due to duplicate name)." << std::endl << std::endl;
			retval = false;
		}

		UdmAssociationObjectSet::iterator ucsItr1 = udmAssociationObjectSet1.begin();
		
		for( ; ucsItr1 != udmAssociationObjectSet1.end(); ++ucsItr1 ) {

			UdmAssociationObjectSet::iterator ucsItr2 = udmAssociationObjectSet2.find(*ucsItr1);

			if ( ucsItr2 == udmAssociationObjectSet2.end()) {
				std::cerr << "In first model, \"" << udmObject1.getPath( "/" ) << "\" has a \"" << umlAssociationRole.name() << "\" association object \"" << ucsItr1->object.getPath( "/" ) << "\", but not in the second model." << std::endl;
				Report::get_singleton().incrementDifferences( udmObject1 );
				retval = false;
				continue;
			}

			Udm::Object udmAssociationObject1 = ucsItr1->object;
			Udm::Object udmAssociationObject2 = ucsItr2->object;

			udmAssociationObjectSet2.erase(ucsItr2);

			if ( udmAssociationObject1 == Udm::null ) {
				if ( udmAssociationObject2 != Udm::null ) {
					Report::get_singleton().incrementDifferences( udmObject1 );
					std::cerr << "Association Error:" << std::endl;
					std::cerr << "In first model, object \"" << udmObject1.getPath( "/" ) << "is \"" << umlAssociationRole.name() << "\" associated with Udm::null" << std::endl;
					std::cerr << "However, in the second model, \"" << udmObject2.getPath( "/" ) << "\" is \"" << umlAssociationRole.name() << "\" associated with \"" << udmAssociationObject2.getPath( "/" ) << "\"" << std::endl << std::endl;
					retval = false;
				}
			} else if ( udmAssociationObject2 == Udm::null ) {
				Report::get_singleton().incrementDifferences( udmObject1 );
				std::cerr << "Association Error:" << std::endl;
				std::cerr << "In second model, object \"" << udmObject2.getPath( "/" ) << "is \"" << umlAssociationRole.name() << "\" associated with Udm::null" << std::endl;
				std::cerr << "However, in the first model, \"" << udmObject1.getPath( "/" ) << "\" is \"" << umlAssociationRole.name() << "\" associated with \"" << udmAssociationObject1.getPath( "/" ) << "\"" << std::endl << std::endl;
				retval = false;
			} else {

				UdmObjectMap::iterator uomItr = _udmObjectMap.find( udmAssociationObject1 );
				if ( uomItr != _udmObjectMap.end() ) {

					Udm::Object udmMappedObject = uomItr->second;
					if ( udmMappedObject != udmAssociationObject2 ) {
						Report::get_singleton().incrementDifferences( udmObject1 );
						std::cerr << "Association Error:" << std::endl;
						std::cerr << "In first  model, \"" << udmObject1.getPath( "/" ) << "\" is \"" << umlAssociationRole.name() << "\" associated with \"" << udmAssociationObject1.getPath( "/" ) << "\"." << std::endl;
						std::cerr << "In second model, \"" << udmObject2.getPath( "/" ) << "\" is \"" << umlAssociationRole.name() << "\" associated with \"" << udmAssociationObject2.getPath( "/" ) << "\"." << std::endl;
						std::cerr << "However, \"" << udmAssociationObject1.getPath( "/" ) << "\" of the first model corresponds to \"" << udmMappedObject.getPath( "/" ) << "\"" << std::endl;
						std::cerr << "instead of \"" << udmAssociationObject2.getPath( "/" ) << "\" as it should." << std::endl << std::endl;
						retval = false;
					}

				}
			}
		}

		UdmAssociationObjectSet::iterator ucsItr2 = udmAssociationObjectSet2.begin();
		for( ; ucsItr2 != udmAssociationObjectSet2.end(); ++ucsItr2 ) {
			std::cerr << "In second model, \"" << udmObject2.getPath( "/" ) << "\" has a \"" << umlAssociationRole.name() << "\" association object \"" << ucsItr2->object.getPath( "/" ) << "\", but not in the first model." << std::endl;
			Report::get_singleton().incrementDifferences( udmObject2 );
			retval = false;
			continue;
		}
	}

	Report::get_singleton().addToReport( udmObject1 );

	return retval;
}


void UdmComparator::tallyChildrenClasses( const UdmObjectSet &udmObjectSet1, const UdmObjectSet &udmObjectSet2 ) {

	ChildrenClassesTallyMap childrenClassesTallyMap;

	for( UdmObjectSet::const_iterator uosCit = udmObjectSet1.begin() ; uosCit != udmObjectSet1.end() ; ++uosCit ) {

		std::string className = uosCit->type().name();

		ChildrenClassesTallyMap::iterator ctmItr = childrenClassesTallyMap.find( className );
		if ( ctmItr == childrenClassesTallyMap.end() ) {
			ctmItr = childrenClassesTallyMap.insert(  std::make_pair(  className, IntPair( 0, 0 )  )   ).first;
		}

		++(ctmItr->second.first);
	}

	for( UdmObjectSet::const_iterator uosCit = udmObjectSet2.begin() ; uosCit != udmObjectSet2.end() ; ++uosCit ) {

		std::string className = uosCit->type().name();

		ChildrenClassesTallyMap::iterator ctmItr = childrenClassesTallyMap.find( className );
		if ( ctmItr == childrenClassesTallyMap.end() ) {
			ctmItr = childrenClassesTallyMap.insert(  std::make_pair(  className, IntPair( 0, 0 )  )   ).first;
		}

		++(ctmItr->second.second);
	}

	std::cerr << "Disparity in number of child objects for the following classes:" << std::endl;
	for( ChildrenClassesTallyMap::iterator ctmItr = childrenClassesTallyMap.begin() ; ctmItr != childrenClassesTallyMap.end() ; ++ctmItr ) {
		IntPair intPair = ctmItr->second;
		if ( intPair.first != intPair.second ) {
			std::cerr << "\t" << ctmItr->first << " => " << intPair.first << "," << intPair.second << std::endl << std::endl;
		}
	}
	std::cerr << std::endl;

}
