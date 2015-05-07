#ifndef _ISIS_CTYPEMAP
#define _ISIS_CTYPEMAP

class CTypeMap {
private:
	struct StringComparator {
		bool operator()(const std::string& a, const std::string& b) const { return a.compare( b ) < 0; }
	};

	typedef std::map< string, string, StringComparator > Map;

	Map map;
	CTypeMap( void ) {
		map["boolean"]         = "char";        // tbd -- how is boolean mapped check!!
		map["Boolean"]         = "char";        // tbd -- how is boolean mapped check!!
		map["byte"]            = "char";
		map["char"]            = "char";
		map["double"]          = "double";
		map["float"]           = "float";
		map["float('double')"] = "double";
		map["float('single')"] = "float";
		map["int8"]            = "char";
		map["int(8)"]          = "char";
		map["int16"]           = "short";
		map["int(16)"]         = "short";
		map["int32"]           = "int";
		map["int(32)"]         = "int";
		map["integer"]         = "int";
		map["int"]             = "int";
		map["long"]            = "long";
		map["short"]           = "short";
		map["single"]          = "float";
		map["sfix(16)"]        = "double";
		map["State"]           = "int";
		map["string"]          = "char *";
		map["ufix16"]          = "double";
		map["uint8"]           = "unsigned char";
		map["uint(8)"]         = "unsigned char";
		map["uint16"]          = "unsigned short";
		map["uint(16)"]        = "unsigned short";
		map["uint32"]          = "unsigned int";
		map["uint(32)"]        = "unsigned int";
		map["void"]            = "void";
	}

public:
	static const Map &getTypeMap( void ) {
		static CTypeMap typeMap;
		return typeMap.map;
	}

	static const std::string lookup( const std::string &type, const std::string &defaultType = "" ) {
		std::string localtype = type;
		Map typeMap = getTypeMap();
		for( Map::const_iterator it = typeMap.begin() ; it != typeMap.end() ; ++it ) {
			if ( type.find( it->first ) != std::string::npos ) return it->second;
		}
		if (  localtype.find( ' ' ) != std::string::npos  ) localtype = "single";
		return defaultType.empty() ? localtype : defaultType;
	}
};

#endif