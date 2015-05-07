#pragma once

#include <string>
#include <map>
#include <vector>

typedef std::vector< std::pair< std::string, std::string > > ParamTypeVector; // (Parameter name, Name of parameter holding type string)
typedef std::map< std::string, ParamTypeVector > BlockTypeMap;


class BlockChecker
{
public:
	BlockChecker(void);
	virtual ~BlockChecker(void) { }

	bool CheckBlock( Block udmBlock, std::string & badParams );

private:
	BlockTypeMap _blocktypes;

};

