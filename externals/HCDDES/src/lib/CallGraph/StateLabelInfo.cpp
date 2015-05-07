#include "StateLabelInfo.hpp"

void StateLabelInfo::dump( void ) {

	NumberStateMap numberStateMap;
	StateLabelInfo::StateNumberMap::const_iterator snmItr = StateLabelInfo::getStateNumberMap().begin();
	while( snmItr != StateLabelInfo::getStateNumberMap().end() ) {
		numberStateMap.insert(  NumberStateMap::value_type( snmItr->second, snmItr->first )  );
		advance( snmItr, 1 );
	}

	std::cerr << "--- Dump StateLabelInfo ---" << std::endl;
	std::cerr << "StateNumberMap:" << std::endl;
	NumberStateMap::const_iterator nsmItr = numberStateMap.begin();
	while( nsmItr != numberStateMap.end() ) {
		std::cerr << nsmItr->second << " => " << nsmItr->first << std::endl;
		advance( nsmItr, 1 );
	}

	std::cerr << "AndSSMap:" << std::endl;
	StateLabelInfo::AndSSMap::const_iterator asmItr = StateLabelInfo::getAndSSMap().begin();
	while( asmItr != StateLabelInfo::getAndSSMap().end() ) {
		std::cerr << asmItr->first << " => " << asmItr->second << std::endl;
		advance( asmItr, 1 );
	}
}
