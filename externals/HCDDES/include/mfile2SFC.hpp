#ifndef _ISIS_MFILE2SFC
#define _ISIS_MFILE2SFC

#include <string>
#include <map>
#include <vector>
#include <list>
#include "SFC.h"
#include "EnhancedMatrix.hpp"
#include "SFCSymbolTable.hpp"

extern "C" {
	extern int MatLabStateflowFlag;
}

typedef std::vector< std::string > DirectoryVector;
typedef std::list< SFC::DT > DTList;


void addMFileDirectory( const std::string &directoryName );
void addMFileDirectories( const DirectoryVector &directoryVector );
void deleteMFileDirectory( const std::string &directoryName );

void mfile2SFC( SFC::CompoundStatement compoundStatement, DTList &inputDTList, const std::string &mfile );

typedef std::list< SFC::ArgDeclBase> VList;
void emfunc2SFC(
 SFC::CompoundStatement compoundStatement,
 const VList& inputs,
 const VList& outputs,
 const std::string &statement
);

typedef std::map< std::string, SFC::Function > FunctionNameMap;
void mstat2SFC(
 SFC::CompoundStatement compoundStatement,
 const std::string &statement,
 bool zeroIndex = false,
 bool stateflowFlag = false,
 const FunctionNameMap &functionNameMap = FunctionNameMap()
);

void mexprStat2SFC(
 SFC::CompoundStatement compoundStatement,
 const std::string &statement,
 const FunctionNameMap &functionNameMap = FunctionNameMap()
);

void mexpr2SFCCondition(
 SFC::CompoundStatement conditionTypeStatement,
 const std::string &statement,
 bool zeroIndex = false,
 bool stateflowFlag = false,
 const FunctionNameMap &functionNameMap = FunctionNameMap()
);

SFC::DT getMatrixSFCType( SFC::CompoundStatement compoundStatement, const std::string &matrix );
void getDimensions( const std::string &matrix, int &rows, int &columns );
void getMatrixDimensions( const std::string &matrix, int &rows, int &columns );
void getMatrixDimensions( SFC::CompoundStatement compoundStatement, const std::string &matrix, int &rows, int &columns );
EnhancedMatrix getMatrix( const std::string &matrix );
EnhancedMatrix getMatrix( SFC::CompoundStatement compoundStatement, const std::string &origMatrix );

#endif
