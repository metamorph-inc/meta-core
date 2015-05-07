#ifndef _ISIS_SFMANAGER
#define _ISIS_SFMANAGER

#include <map>
#include <set>
#include <vector>
#include <boost/regex.hpp>

#include "RegexCommon.hpp"
#include "VarResolve.hpp"
#include "SFC.h"
#include "SLSF.h"


class SFManager {

public:
	typedef std::map< std::string, SFC::Function > FunctionHash;
	typedef std::vector< SLSF::State > StateVector;
	typedef std::set< SLSF::TransConnector > TransConnectorSet;
	typedef std::vector< SLSF::SFState > SFStateVector;
	typedef std::set< SLSF::SFTransConnector > SFTransConnectorSet;

private:
	class TransConnectorMap {
	private:
		int _uniqueNumber;

		int getUniqueNumber( void ) { return _uniqueNumber++; }

	public:
		class TransConnectorData {
		private:
			int _number;
			bool _isProcessed;

		public:
			TransConnectorData( int number ) : _number( number ), _isProcessed( false ) { }

			int getNumber( void ) const { return _number; }

			void setIsProcessed( bool isProcessed ) { _isProcessed = isProcessed; }
			bool getIsProcessed( void ) const { return _isProcessed; }
		};

		typedef std::map< SLSF::TransConnector, TransConnectorData > InternalTransConnectorMap;

	private:
		InternalTransConnectorMap _internalTransConnectorMap;

	public:
		TransConnectorMap( void ) : _uniqueNumber( 0 ) { }

		void addTransConnector( SLSF::TransConnector transConnector ) {
			_internalTransConnectorMap.insert(  std::make_pair( transConnector, TransConnectorData( getUniqueNumber() ) )  );
		}

		bool hasTransConnector( SLSF::TransConnector transConnector ) {
			return _internalTransConnectorMap.find( transConnector ) != _internalTransConnectorMap.end();
		}

		int getTransConnectorNumber( SLSF::TransConnector transConnector ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			return itmItr == _internalTransConnectorMap.end() ? -2 : itmItr->second.getNumber();
		}

		bool getIsTransConnectorProcessed( SLSF::TransConnector transConnector ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			return itmItr == _internalTransConnectorMap.end() ? true : itmItr->second.getIsProcessed();
		}

		void setIsTransConnectorProcessed( SLSF::TransConnector transConnector, bool isProcessed ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			if ( itmItr != _internalTransConnectorMap.end() ) itmItr->second.setIsProcessed( isProcessed );
		}

		void clear( void ) {
			_internalTransConnectorMap.clear();
			_uniqueNumber = 0;
		}
	};

	class SFTransConnectorMap {
	private:
		int _uniqueNumber;

		int getUniqueNumber( void ) { return _uniqueNumber++; }

	public:
		class SFTransConnectorData {
		private:
			int _number;
			bool _isProcessed;

		public:
			SFTransConnectorData( int number ) : _number( number ), _isProcessed( false ) { }

			int getNumber( void ) const { return _number; }

			void setIsProcessed( bool isProcessed ) { _isProcessed = isProcessed; }
			bool getIsProcessed( void ) const { return _isProcessed; }
		};

		typedef std::map< SLSF::SFTransConnector, SFTransConnectorData > InternalTransConnectorMap;

	private:
		InternalTransConnectorMap _internalTransConnectorMap;

	public:
		SFTransConnectorMap( void ) : _uniqueNumber( 0 ) { }

		void addTransConnector( SLSF::SFTransConnector transConnector ) {
			_internalTransConnectorMap.insert(  std::make_pair( transConnector, SFTransConnectorData( getUniqueNumber() ) )  );
		}

		bool hasTransConnector( SLSF::SFTransConnector transConnector ) {
			return _internalTransConnectorMap.find( transConnector ) != _internalTransConnectorMap.end();
		}

		int getTransConnectorNumber( SLSF::SFTransConnector transConnector ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			return itmItr == _internalTransConnectorMap.end() ? -2 : itmItr->second.getNumber();
		}

		bool getIsTransConnectorProcessed( SLSF::SFTransConnector transConnector ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			return itmItr == _internalTransConnectorMap.end() ? true : itmItr->second.getIsProcessed();
		}

		void setIsTransConnectorProcessed( SLSF::SFTransConnector transConnector, bool isProcessed ) {
			InternalTransConnectorMap::iterator itmItr = _internalTransConnectorMap.find( transConnector );
			if ( itmItr != _internalTransConnectorMap.end() ) itmItr->second.setIsProcessed( isProcessed );
		}

		void clear( void ) {
			_internalTransConnectorMap.clear();
			_uniqueNumber = 0;
		}
	};
public:

	class StateTransConnectorMap {
	public:
		typedef std::map< SLSF::State, TransConnectorMap > InternalStateTransConnectorMap;

	private:
		InternalStateTransConnectorMap _internalStateTransConnectorMap;

		StateTransConnectorMap( void ) { }

	public:
		static StateTransConnectorMap &get_singleton( void ) {
			static StateTransConnectorMap stateTransConnectorMap;
			return stateTransConnectorMap;
		}

		bool stateAlreadyRegistered( SLSF::State state ) {
			return _internalStateTransConnectorMap.find( state ) != _internalStateTransConnectorMap.end();
		}

		void addState( SLSF::State state ) {

			if (  stateAlreadyRegistered( state )  ) {
				std::string fullStateName = getFullStateName( state );
				std::cerr << "ERROR:  StateTransConnectorMap:  state already registered:  " << fullStateName << std::endl;
				return;
			}

			_internalStateTransConnectorMap.insert(  std::make_pair( state, TransConnectorMap() )  );
		}

		void clearState( SLSF::State state ) {
			if (  stateAlreadyRegistered( state )  ) {
				getTransConnectorMap( state ).clear();
			} else {
				addState( state );
			}
		}

		static TransConnectorMap &getEmptyTransConnectorMap( void ) {
			static TransConnectorMap transConnectorMap;
			transConnectorMap.clear();
			return transConnectorMap;
		}

		TransConnectorMap &getTransConnectorMap( SLSF::State state ) {
			InternalStateTransConnectorMap::iterator ismItr = _internalStateTransConnectorMap.find( state );

			if ( ismItr == _internalStateTransConnectorMap.end() ) {
				std::cerr << "ERROR:  StateTransConnectorMap:  attempt to retrieve TransConnectorMap for unregistered state:  " <<
				 getFullStateName( state ) << std::endl;
				return getEmptyTransConnectorMap();
			}

			return ismItr->second;
		}
	};

	class SFStateTransConnectorMap {
	public:
		typedef std::map< SLSF::SFState, SFTransConnectorMap > InternalStateTransConnectorMap;

	private:
		InternalStateTransConnectorMap _internalStateTransConnectorMap;

		SFStateTransConnectorMap( void ) { }

	public:
		static SFStateTransConnectorMap &get_singleton( void ) {
			static SFStateTransConnectorMap stateTransConnectorMap;
			return stateTransConnectorMap;
		}

		bool stateAlreadyRegistered( SLSF::SFState state ) {
			return _internalStateTransConnectorMap.find( state ) != _internalStateTransConnectorMap.end();
		}

		void addState( SLSF::SFState state ) {

			if (  stateAlreadyRegistered( state )  ) {
				std::string fullStateName = getFullStateName( state );
				std::cerr << "ERROR:  StateTransConnectorMap:  state already registered:  " << fullStateName << std::endl;
				return;
			}

			_internalStateTransConnectorMap.insert(  std::make_pair( state, SFTransConnectorMap() )  );
		}

		void clearState( SLSF::SFState state ) {
			if (  stateAlreadyRegistered( state )  ) {
				getTransConnectorMap( state ).clear();
			} else {
				addState( state );
			}
		}

		static SFTransConnectorMap &getEmptyTransConnectorMap( void ) {
			static SFTransConnectorMap transConnectorMap;
			transConnectorMap.clear();
			return transConnectorMap;
		}

		SFTransConnectorMap &getTransConnectorMap( SLSF::SFState state ) {
			InternalStateTransConnectorMap::iterator ismItr = _internalStateTransConnectorMap.find( state );

			if ( ismItr == _internalStateTransConnectorMap.end() ) {
				std::cerr << "ERROR:  StateTransConnectorMap:  attempt to retrieve TransConnectorMap for unregistered state:  " <<
				 getFullStateName( state ) << std::endl;
				return getEmptyTransConnectorMap();
			}

			return ismItr->second;
		}
	};

	class FunctionArgMap {
	public:
		typedef std::map< std::string, int > ArgMap;

	private:
		ArgMap _argMap;
		int _argNo;

		void fillArgMap( std::string args ) {

			const std::string &const_args = args;

			boost::match_results<std::string::const_iterator> results;
			while(  regex_search( const_args, results, RegexCommon::getIdentifierRegex() )  ) {
				std::string argName( results[0].first, results[0].second );
				_argMap.insert(  std::make_pair( argName, _argNo++ )  );
				args = std::string( results.suffix().first, results.suffix().second );
			}

		}

	public:
		FunctionArgMap( const std::string &funcSpec ) : _argNo( 0 ) {

			boost::match_results< std::string::const_iterator > results;
			if (  !boost::regex_search( funcSpec, results, RegexCommon::getFunctionSpecRegex() )  ) {
				std::cerr << "ERROR:  INVALID FUNCTION HEADER \"" << funcSpec << "\" ENCOUNTERED" << std::endl;
				return;
			}

			std::string inputArgs( results[2] );
			fillArgMap( inputArgs );

			std::string outputArgs( results[1].first, results[1].second );
			if ( !outputArgs.empty() && outputArgs[0] == '[' ) outputArgs = outputArgs.substr( 1, outputArgs.size() - 2 );
			fillArgMap( outputArgs );
		}

		int getArgNo( const std::string &argName ) {
			ArgMap::iterator agmItr = _argMap.find( argName );
			return agmItr == _argMap.end() ? -1 : agmItr->second;
		}
	};

	class FunctionArgMapManager {
	public:
		typedef std::map< SFC::Function, FunctionArgMap > Function2FAMMap;

	private:
		Function2FAMMap _function2FAMMap;

		FunctionArgMapManager( void ) { }

	public:
		static FunctionArgMapManager &get_singleton( void ) {
			static FunctionArgMapManager functionArgMapManager;
			return functionArgMapManager;
		}

		void parseFunctionSpec( SFC::Function function, const std::string functionSpec ) {
			_function2FAMMap.insert(   std::make_pair(  function, FunctionArgMap( functionSpec )  )   );
		}

		int getArgNo( SFC::Function function, const std::string &argName ) {
			Function2FAMMap::iterator ffmItr = _function2FAMMap.find( function );
			if ( ffmItr == _function2FAMMap.end() ) return -2;
			FunctionArgMap &functionArgMap = ffmItr->second;
			return functionArgMap.getArgNo( argName );
		}
	};

	static StateVector getStateAncestors( SLSF::State state ) {
		StateVector stateVector;
		stateVector.push_back( state );

		Udm::Object object = state.GetParent();
		while( object.type() == SLSF::State::meta ) {
			state = SLSF::State::Cast( object );
			stateVector.push_back( state );
			object = state.GetParent();
		}

		return stateVector;
	}

	static std::string getFullStateName( SLSF::State state ) {

		StateVector stateVector = getStateAncestors( state );

		std::string fullStateName;
		bool first = true;
		while( !stateVector.empty() ) {
			if ( first ) first = false;
			else         fullStateName += ".";
			fullStateName += stateVector.back().Name();
			stateVector.pop_back();
		}

		return fullStateName;
	}

	static SFStateVector getStateAncestors( SLSF::SFState state ) {
		SFStateVector stateVector;
		stateVector.push_back( state );

		Udm::Object object = state.GetParent();
		while( object.type() == SLSF::SFState::meta ) {
			state = SLSF::SFState::Cast( object );
			stateVector.push_back( state );
			object = state.GetParent();
		}

		return stateVector;
	}

	static std::string getFullStateName( SLSF::SFState state ) {

		SFStateVector stateVector = getStateAncestors( state );

		std::string fullStateName;
		bool first = true;
		while( !stateVector.empty() ) {
			if ( first ) first = false;
			else         fullStateName += ".";
			fullStateName += stateVector.back().Name();
			stateVector.pop_back();
		}

		return fullStateName;
	}
	static FunctionHash &getFunctionHash( void ) {
		static FunctionHash functionHash;
		return functionHash;
	}

	static VarResolve &getVarResolve( void ) {
		static VarResolve varResolve;
		return varResolve;
	}

	static std::string trim( const std::string &rhs );

	static std::string filterComments( std::string &input );

	static std::string AddTypedEntityScope( SFC::TypedEntity typedEntity );

	static std::string getContainingFunctionScope( Udm::Object object );

	static std::string ResolveReferences( SFC::CompoundStatement compoundStatement, const std::string &expression );

	static void mstat2SFC( SFC::CompoundStatement compoundStatement, const std::string &statement );
	static void mexpr2SFCCondition( SFC::CompoundStatement compoundStatement, const std::string &expression );

private:
	typedef std::set< SFC::LocalVar > LocalVarSet;
	static void initVarAux( SFC::CompoundStatement compoundStatement, SFC::LocalVar localVar, const std::string &initStr );

public:
	static void initVar( SFC::CompoundStatement compoundStatement, SFC::Var var );
};

#endif