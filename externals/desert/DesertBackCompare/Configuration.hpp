#ifndef _ISIS_CONFIGURATION_
#define _ISIS_CONFIGURATION_

#include <set>

#include <DesertIfaceBack.h>

class Configuration {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::Configuration &configuration1, const DesertIfaceBack::Configuration &configuration2 );
	};

	typedef std::set< DesertIfaceBack::Configuration, Comparator > set;

	static bool compare( DesertIfaceBack::Configuration compareFromConfiguration, DesertIfaceBack::Configuration compareToConfiguration );

};

#endif /* _ISIS_CONFIGURATION_ */