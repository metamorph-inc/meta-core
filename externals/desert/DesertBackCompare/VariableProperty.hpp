#ifndef _ISIS_VARIABLEPROPERTY_
#define _ISIS_VARIABLEPROPERTY_

#include <set>

#include <DesertIfaceBack.h>

class VariableProperty {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::VariableProperty &v1, const DesertIfaceBack::VariableProperty &v2 ) {
			return static_cast< std::string >( v1.name() ) < static_cast< std::string >( v2.name() );
		}
	};

	typedef std::set< DesertIfaceBack::VariableProperty, Comparator > set;

	static bool compare( DesertIfaceBack::VariableProperty compareFromVariableProperty, DesertIfaceBack::VariableProperty compareToVariableProperty ) {
		return compareFromVariableProperty.name() == compareToVariableProperty.name();
	}

};

#endif /* _ISIS_VARIABLEPROPERTY_ */