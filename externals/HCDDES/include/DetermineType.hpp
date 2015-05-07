
#ifndef _ISIS_DETERMINETYPE_H_
#define _ISIS_DETERMINETYPE_H_

#include <string>
#include <vector>
#include <map>
#include <list>

#include "ErrHand.h"
#include "ESMoL.h"
#include "Uml.h"


class DetermineType {
private:
	typedef std::map< std::string, int > TypePrecedenceMap;

	class TypePrecedence {
	private:
		TypePrecedenceMap _typePrecedenceMap;

	public:
		TypePrecedence( void );
		TypePrecedenceMap &getMap( void ) { return _typePrecedenceMap; }
	};


public:
	static int getTypePrecedence( const std::string &type ) {
		static TypePrecedence typePrecedence;
		TypePrecedenceMap::iterator tpmItr = typePrecedence.getMap().find( type );
		return tpmItr == typePrecedence.getMap().end() ? 0 : tpmItr->second;
	}


	typedef std::map< std::string, std::string > ParameterMap;
	typedef std::vector< ESMoL::TypeBase >     TypeBaseVector;
	typedef std::list< ESMoL::Port >           PortList;
	typedef std::set< ESMoL::Line >            LineSet;

	class LineToDstPort {
	private:
		PortList _portList;

	public:
		void operator()( ESMoL::Line line ) { _portList.push_back( line.dstLine_end() ); }
		PortList &getPortList( void ) { return _portList; }
	};


	static std::string getDownstreamType( ESMoL::Port port );


	static std::string getHigherPrecedenceType( const std::string &type1, const std::string &type2 ) {
		return getTypePrecedence( type1 ) < getTypePrecedence( type2 ) ? type1 : type2;
	}


	static void getStringTypes( const TypeBaseVector &inputTypes, std::vector< std::string > &stringTypes );


	static std::string getType(
	 const ParameterMap &parameterMap, const TypeBaseVector &inputTypes,
	 const std::string &parameterType = "", const std::string &cast = "", const std::string &input = ""
	) {
		return getType( parameterMap, inputTypes, ESMoL::OutputPort(), parameterType, cast, input );
	}


	static std::string getType(
	 const ParameterMap &parameterMap, const TypeBaseVector &inputTypes, ESMoL::OutputPort outputPort,
	 const std::string &parameterType = "", const std::string &cast = "", const std::string &input = ""
	);


	static std::string getType(
	 const std::string &outDataTypeMode, const std::string &outDataType, const TypeBaseVector &inputTypes,
	 const std::string &parameterType = "", const std::string &cast = "", const std::string &input = ""
	) {
		return getType( outDataTypeMode, outDataType, inputTypes, ESMoL::OutputPort(), parameterType, cast, input );
	}


	static std::string getType(
	 const std::string &outDataTypeMode, const std::string &outDataType, 
	 const TypeBaseVector &inputTypes, ESMoL::OutputPort outputPort,
	 const std::string &parameterType = "", const std::string &cast = "", const std::string &input = ""
	) {
		std::vector< std::string > typesList;
		getStringTypes( inputTypes, typesList );
		if ( parameterType != "" ) typesList.push_back( parameterType );

		return getType( outDataTypeMode, outDataType, typesList, outputPort, cast, input );
	}


	static std::string getType(
	 const std::string &outDataTypeMode, const std::string &outDataType, 
	 const std::vector< std::string > &typesList, ESMoL::OutputPort outputPort,
	 const std::string &cast = "", const std::string &input = ""
	);

};

#endif
