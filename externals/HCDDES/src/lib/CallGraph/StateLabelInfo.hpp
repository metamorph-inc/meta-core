#ifndef _ISIS_STATELABELINFO
#define _ISIS_STATELABELINFO

#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "SFC.h"

//
// FUNCTOR CLASS FOR MANAGING StateLabel INFORMATION:
// StateNumberMap MAPS A StateLabel's NAME TO ITS NUMERIC VALUE
// AndSSMap MAPS A StateLabel's SCOPE TO THE NUMBER OF SUBSTATES IT HAS, PROVIDED IT IS AN "AND" STATE
//

class StateLabelInfo {
private:
	typedef std::set< SFC::StateLabel > StateLabelSet;

public:
	typedef std::map< std::string, int > StateNumberMap;
	typedef std::map< std::string, int > AndSSMap;

private:
	static StateNumberMap &getMutableStateNumberMap( void ) {
		static StateNumberMap stateNumberMap;
		return stateNumberMap;
	}

	static AndSSMap &getMutableAndSSMap( void ) {
		static AndSSMap andSSMap;
		return andSSMap;
	}

public:
	static const StateNumberMap &getStateNumberMap( void ) {
		return getMutableStateNumberMap();
	}
	static const AndSSMap &getAndSSMap( void ) {
		return getMutableAndSSMap();
	}

private:
	struct InitStateNumbers {
		void operator()( const SFC::StateLabel &stateLabel ) const {
			getMutableStateNumberMap()[ static_cast< std::string >( stateLabel.name() ) ] =
			 static_cast< int >( stateLabel.value() );
		}
	};
	struct InitAndSSs {
		void operator()( const SFC::StateLabel &stateLabel ) const {
			getMutableAndSSMap()[ static_cast< std::string >( stateLabel.comment() ) ] =
			 static_cast< int >( stateLabel.andSS() );
		}
	};

	class Init {
	private:
		InitStateNumbers initStateNumbers;
		InitAndSSs initAndSSs;
	public:
		void operator()( const SFC::StateLabel &stateLabel ) const {
			initStateNumbers( stateLabel );
			initAndSSs( stateLabel );
		}
	};
public:
	static void init( const SFC::Program &program ) {
		StateLabelSet stateLabelSet = program.StateLabel_kind_children();
		for_each( stateLabelSet.begin(), stateLabelSet.end(), Init() );
	}

private:
	typedef std::map< int, std::string > NumberStateMap;

public:
	static void dump( void );

};
#endif
