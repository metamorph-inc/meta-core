#include "SFuncWrapperPrint.h"
#include "SfuncCodeTemplate.hpp"
#include <sstream>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
SFuncWrapperPrint::InputArg::InputArg( const SFC::Arg &arg ) : _arg( arg ) {
	_trigger = NONE;

	SetVarSet setVarSet( arg.lvar() );
	if ( setVarSet.empty() ) return;

	SFC::LocalVar localVar = ( *setVarSet.begin() ).lvar_end();

	EventSet eventSet = localVar.event();
	if ( eventSet.empty() ) return;

	SLSF::Event event = *eventSet.begin();
	std::string event_trigger = event.Trigger();
	if ( event_trigger == "RISING_EDGE_EVENT" )			_trigger = RISING;
	else if ( event_trigger == "FALLING_EDGE_EVENT" )	_trigger = FALLING;
	else if ( event_trigger == "EITHER_EDGE_EVENT" )	_trigger = EITHER;
}

///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::ArgAppender::operator()( const SFC::Arg &arg ) {

	std::string baseDTName = SFCTypesManager::getBaseDTName( arg.dt() );
	std::string arg_type = CTypeMap::lookup( baseDTName );

	_sig +=
	 ( _prefixComma ? "," : "" ) +
	 (  _printTypes ? ( SP + arg_type ) : ""  ) + SP +
	 _argPrefix + static_cast< std::string >( arg.name() );

	_prefixComma= true;
}

void SFuncWrapperPrint::ArgAppender::operator()( const InputArg &inputArg ) {
	const SFC::Arg &arg = inputArg._arg;
	std::string baseDTName = SFCTypesManager::getBaseDTName( arg.dt() );
	std::string arg_type = CTypeMap::lookup( baseDTName );

	_sig +=
	 ( _prefixComma ? "," : "") +
	 (  _printTypes ? ( SP + arg_type ) : ""  ) + SP +
	 _argPrefix + static_cast< std::string >( arg.name() ) +
	 ( inputArg._trigger == InputArg::NONE ? "" : "trigger" );

	_prefixComma= true;
}


///////////////////////////////////////////////////////////////////////////////
// This is a different version of 'for_each', here the operand is taken by reference.
// I need it when calling ParamList's func.operator on object wrappers, and gathering some data
// in ParamList data members. As opposed to standard 'for_each', the same 'ParamList' object
// is going to be used by the iterator, not every-time copy-constructed instances.
template< class In, class Op> Op& for_each_ref( In first, In last, Op& f)
{
	while ( first!= last)
		f( *first++);
	return f;
}


///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::InputArgInitializer::operator()( const SFuncWrapperPrint::InputArg &inputArg ) {
	const SFC::Arg &arg = inputArg._arg;
	std::string baseDTName = SFCTypesManager::getBaseDTName( arg.dt() );
	std::string argType(  CTypeMap::lookup( baseDTName )  );

	appendCode( copyExpr(
	  static_cast< std::string >( argType ) + SP + static_cast< std::string >( arg.name() ),
	  getArgIdx( _argIdx++ )
	));
}

///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::OutputArgInitializer::operator()( const SFC::Arg &arg ) {
	std::string baseDTName = SFCTypesManager::getBaseDTName( arg.dt() );
	std::string argType(  CTypeMap::lookup( baseDTName )  );

	appendCode(
	 copyExpr(  argType + SP + static_cast< std::string >( arg.name() ), "0"  )
	);
}

///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::EventArgTemp::operator()( const InputArg &inputArg ) {
	if ( inputArg._trigger == InputArg::NONE ) return;

	std::string baseDTName = SFCTypesManager::getBaseDTName( inputArg._arg.dt() );

	std::string argType(  CTypeMap::lookup( baseDTName )  );
	std::string argName( inputArg._arg.name() );
	_code += createCodeLine( "static " + argType + " " + argName + "old = 0" );
	_code += createCodeLine( "static " + argType + " " + argName + "trigger" );
}
		
///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::EventArgTrigger::operator()( const InputArg &inputArg ) {
	if ( inputArg._trigger == InputArg::NONE ) return;

	std::string argName = inputArg._arg.name();
	std::string codeLine = argName + "trigger = ";
	if ( inputArg._trigger == InputArg::RISING || inputArg._trigger == InputArg::EITHER ) {
		codeLine += argName + "old <= 0 && " + argName + " > 0";
	}
	if ( inputArg._trigger == InputArg::EITHER ) codeLine += " || ";
	if ( inputArg._trigger == InputArg::FALLING || inputArg._trigger == InputArg::EITHER ) {
		codeLine += argName + "old >= 0 && " + argName + " < 0";
	}

	_code += createCodeLine( codeLine );

	_code += createCodeLine( "if ( " + argName + " != 0 ) " + argName + "old " + " = " + argName );
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& SFuncWrapperPrint::print( std::ostream& os) const {
	//sfunc_code_template
	// #define S_FUNCTION_NAME  %s
	// extern %s;
	// extern %s;
	// ssSetInputPortWidth(S, 0, %d);
	// ssSetOutputPortWidth(S, 0, %d); %s
	//static void mdlOutputs(SimStruct *S, int_T tid)
	//{
	//	%s
	//}
	std::string codeTemplate = SfuncCodeTemplate::getTemplate(); 
	char wrapperCode[ 5000 ] = { '\0' };

	std::string initCode= createCodeLine( _ifn + "( 0 )" );
	std::string outputCode;
	generateOutputCode( outputCode);
	std::string include_fname= _sfn + ".h";
	sprintf( wrapperCode, codeTemplate.c_str(), _sfn.c_str(), include_fname.c_str(), _iArgs.size(), _oArgs.size(), initCode.c_str(), outputCode.c_str());
	os << wrapperCode;
	return os;
}

void SFuncWrapperPrint::generateOutputCode( std::string& outputCode ) const
{
	const std::string INPUTARGS  = "inputs";
	const std::string OUTPUTARGS = "outputs";

	outputCode =  createCodeLine( "InputRealPtrsType " + INPUTARGS + " = ssGetInputPortRealSignalPtrs( S, 0 )" );
	outputCode += createCodeLine( "real_T* " + OUTPUTARGS + " = ssGetOutputPortRealSignal(S,0)" );

	std::for_each(  _iArgs.begin(), _iArgs.end(), InputArgInitializer( "*" + INPUTARGS, outputCode )  );
	std::for_each(  _oArgs.begin(), _oArgs.end(), OutputArgInitializer( outputCode )  );

	outputCode += NL + TAB;
	std::for_each(  _iArgs.begin(), _iArgs.end(), SFuncWrapperPrint::EventArgTemp( outputCode )  );
	outputCode += NL + TAB;
	std::for_each(  _iArgs.begin(), _iArgs.end(), SFuncWrapperPrint::EventArgTrigger( outputCode )  );
	outputCode += NL + TAB;

	std::string mainFuncCall = _mfn + "( 0,";
	ArgAppender mainFuncCallArgAppender( mainFuncCall, "");

	for_each_ref( _iArgs.begin(), _iArgs.end(), mainFuncCallArgAppender );
	mainFuncCallArgAppender.setArgPrefix( "&" );
	for_each_ref( _oArgs.begin(), _oArgs.end(),mainFuncCallArgAppender);
	mainFuncCall += ")";
	outputCode += createCodeLine( mainFuncCall );

	//
	std::for_each(  _oArgs.begin(), _oArgs.end(), OutputArgCopier( OUTPUTARGS, outputCode )  );
}

///////////////////////////////////////////////////////////////////////////////
void SFuncWrapperPrint::printWrapper( const std::string sFuncName, const SFC::Program& program )
{
#ifndef SL_CODEGEN
	std::set< SFC::Function, NameCompareFunctor< SFC::Function> > functions= program.Function_kind_children_sorted( NameCompareFunctor< SFC::Function>());
	// create temporary dummy function object for searching of this value
	SFC::Function funcToFind= SFC::Function::Cast( UdmStatic::CreateObject( SFC::Function::meta));
	funcToFind.name()= sFuncName+ "_main";
	// find the main function
	std::set< SFC::Function, NameCompareFunctor< SFC::Function> >::const_iterator itFindMain= functions.find( funcToFind);
//	for ( std::set< SFC::Function, NameCompareFunctor< SFC::Function> >::const_iterator itP= functions.begin(); itP!= functions.end(); ++itP) {
//		std::cout << ( std::string) itP->name() << std::endl;
//	}
	if ( functions.end() == itFindMain) {
		throw udm_exception("SF CodeGen internal error. Main function not found.");
	}
	const SFC::Function& mainFunc= *itFindMain;

	// assumption for output arguments: (1) attribute 'ptr' is set to true. (2) output args come after input args
	SFuncWrapperPrint::ArgSet argSet = mainFunc.Arg_kind_children_sorted( SFuncWrapperPrint::ArgIndexComparator() );

	SFuncWrapperPrint sfuncPrinter( argSet, sFuncName, sFuncName+ "_main", sFuncName+ "_init" );
	std::string filename = std::string( "sfunc_wrapper_" ) + static_cast< std::string >( program.filename() ) + ".c";
	std::ofstream sFuncWrapperFile( filename.c_str(), std::ios_base::binary );
	sfuncPrinter.print( sFuncWrapperFile );
	sFuncWrapperFile.close();
#endif // SL_CODEGEN does not need this
}


