
#include "BlockChecker.h"

#include "RegexCommon.hpp"
#include "MatLabUdmBlock.hpp"

BlockChecker::BlockChecker(void)
{
	_blocktypes[ "Constant" ].push_back( make_pair( "Value", "OutDataTypeStr" ) );
	_blocktypes[ "DiscreteStateSpace" ].push_back( make_pair( "A", "double" ) );
	_blocktypes[ "DiscreteStateSpace" ].push_back( make_pair( "B", "double" ) );
	_blocktypes[ "DiscreteStateSpace" ].push_back( make_pair( "C", "double" ) );
	_blocktypes[ "DiscreteStateSpace" ].push_back( make_pair( "D", "double" ) );
	_blocktypes[ "DiscreteTransferFcn" ].push_back( make_pair( "Numerator", "NumCoefDataTypeStr" ) );
	_blocktypes[ "DiscreteTransferFcn" ].push_back( make_pair( "Denominator", "DenCoefDataTypeStr" ) );
	_blocktypes[ "DiscreteZeroPole" ].push_back( make_pair( "Zeros", "double" ) );
	_blocktypes[ "DiscreteZeroPole" ].push_back( make_pair( "Poles", "double" ) );
	_blocktypes[ "DiscreteZeroPole" ].push_back( make_pair( "Gain", "double" ) );
	_blocktypes[ "Gain" ].push_back( make_pair( "Gain", "ParamDataTypeStr" ) ) ;
	_blocktypes[ "Saturate" ].push_back( make_pair( "LowerLimit", "OutDataTypeStr" ) );
	_blocktypes[ "Saturate" ].push_back( make_pair( "UpperLimit", "OutDataTypeStr" ) );
	_blocktypes[ "UnitDelay" ].push_back( make_pair( "InitialCondition", "double" ) );
}

bool BlockChecker::CheckBlock( Block udmBlock, std::string & badParams )
{
	bool passed=true; // Passes by default

	string btype = udmBlock.BlockType();

	BlockTypeMap::iterator findIt = _blocktypes.find( btype );

	if ( findIt != _blocktypes.end() )
	{
		MatLabUdmBlock::ParameterMap parameterMap = MatLabUdmBlock::getParameterMap( udmBlock );

		for ( ParamTypeVector::iterator typeVecIter = findIt->second.begin(); typeVecIter != findIt->second.end(); typeVecIter++ )
		{
			MatLabUdmBlock::ParameterMap::iterator findParam = parameterMap.find( string( typeVecIter->first ) );
			if ( findParam != parameterMap.end() )
			{
				Parameter valParam = findParam->second;
				string valStr = valParam.Value();

				boost::trim( valStr );

				if (  !boost::regex_match( valStr, RegexCommon::getMatrixLiteralRegex() )  ) {
					passed = false;
					badParams += string( typeVecIter->first ) + ": " + valStr + " ";
				}
			}
		}

	}
	
	return passed; 

}
