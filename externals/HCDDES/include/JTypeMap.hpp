#ifndef _ISIS_JTYPEMAP
#define _ISIS_JTYPEMAP

class JTypeMap {
private:
	struct StringComparator {
		bool operator()(const std::string& a, const std::string& b) const { return a.compare( b ) < 0; }
	};

	typedef std::map< string, string, StringComparator > Map;

	Map _map;
	JTypeMap( void ) {
		_map["boolean"]         = "boolean";        // tbd -- how is boolean mapped check!!
		_map["Boolean"]         = "boolean";        // tbd -- how is boolean mapped check!!
		_map["byte"]            = "byte";
		_map["char"]            = "byte";
		_map["double"]          = "double";
		_map["float"]           = "float";
		_map["float('double')"] = "double";
		_map["float('single')"] = "float";
		_map["int8"]            = "byte";
		_map["int(8)"]          = "byte";
		_map["int16"]           = "short";
		_map["int(16)"]         = "short";
		_map["int32"]           = "int";
		_map["int(32)"]         = "int";
		_map["integer"]         = "int";
		_map["int"]             = "int";
		_map["long"]            = "long";
		_map["short"]           = "short";
		_map["single"]          = "float";
		_map["sfix(16)"]        = "float";
		_map["State"]           = "int";
		_map["string"]          = "String";
		_map["ufix16"]          = "double";
		_map["uint8"]           = "byte";
		_map["uint(8)"]         = "byte";
		_map["uint16"]          = "short";
		_map["uint(16)"]        = "short";
		_map["uint32"]          = "int";
		_map["uint(32)"]        = "int";
		_map["void"]            = "void";
	}

	static const Map &getTypeMap( void ) {
		static JTypeMap typeMap;
		return typeMap._map;
	}

public:
	static std::string lookup( const std::string &type ) {
		Map typeMap = getTypeMap();
		for( Map::const_iterator it = typeMap.begin() ; it != typeMap.end() ; ++it ) {
			if ( type.find( it->first ) != std::string::npos ) return it->second;
		}
		return type;
	}
};

class JTypePrecedence {
private:
	struct StringComparator {
		bool operator()(const std::string& a, const std::string& b) const { return a.compare( b ) < 0; }
	};

	typedef std::map< string, int, StringComparator > Map;

	Map _map;
	JTypePrecedence( void ) {
		_map["boolean"] = 7;        // tbd -- how is boolean mapped check!!
		_map["byte"]    = 6;
		_map["double"]  = 1;
		_map["float"]   = 2;
		_map["int"]     = 4;
		_map["long"]    = 3;
		_map["short"]   = 5;
	}

	static const Map &getTypePrecedence( void ) {
		static JTypePrecedence typePrecedence;
		return typePrecedence._map;
	}

	static int lookup( const std::string &type ) {
		Map precedenceMap = getTypePrecedence();
		Map::const_iterator it = precedenceMap.find( type );
		if ( it == precedenceMap.end() ) {
			// TODO: throw an exception?
//			std::cerr << "Unknown type " << type;
			return 99;
		}
		return it->second;
	}

public:
	static std::string getDominantType( const std::string &type1, const std::string &type2 ) {
		int type1Precedence = lookup( type1 );
		int type2Precedence = lookup( type2 );

		return type1Precedence < type2Precedence ? type1 : type2;
	}
};

#endif
