#ifndef SFUNCWRAPPERPRINT_H
#define SFUNCWRAPPERPRINT_H

#ifdef _MSC_VER
#pragma warning (disable : 4786)
#pragma warning (disable : 4503) 
#endif

#include "SLSF.h"
#include "SFC.h"
#include "CTypeMap.hpp"
#include "UdmStatic.h"
#include "SFCUdmEngine.hpp"
#include <set>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

const std::string EQ = "=";
const std::string SP = " ";
const std::string SC = ";";
const std::string NL = "\n";
const std::string TAB= "\t";

static inline std::string createCodeLine( const std::string expr) {
	return expr+ SC+ NL+ TAB;
}

class SFuncWrapperPrint {

private:
	struct InputArg {
		enum Trigger { NONE, RISING, FALLING, EITHER };

		typedef std::set< SLSF::Event > EventSet;
		typedef std::set< SFC::SetVar >    SetVarSet;

		SFC::Arg _arg;
		Trigger _trigger;

		InputArg( const SFC::Arg &arg );
	};

public:
	struct ArgIndexComparator {  
		bool operator()( const SFC::Arg &a, const SFC::Arg &b ) const {
			return a.argIndex() < b.argIndex();  // based on index
		}
	};

	struct InputArgIndexComparator {
		bool operator()(const InputArg &a, const InputArg &b) const {
			return a._arg.argIndex() < b._arg.argIndex();  // based on index
		}
	};

	typedef ArgIndexComparator OutputArgIndexComparator;

private:
	class ArgCopier {
	protected:
		ArgCopier( const std::string& argsVar, std::string& code) : _argsVar( argsVar), _code( code)
		{}
		std::string copyExpr( const std::string& from, const std::string& to) {
			return from+ EQ+ SP+ to;
		}
		void appendCode( const std::string& codeToAdd) {
			_code+= createCodeLine( codeToAdd);
		}
		std::string getArgIdx( int idx ) {
			return _argsVar + "[" + SP + boost::lexical_cast< std::string >( idx ) + "]";
		}
	private:
		const std::string& _argsVar;
		std::string& _code;
	};

	class ArgAppender {
	public:
		ArgAppender( std::string& signature, const std::string& argPrefix= "", bool printTypes= false) 
			: _sig( signature), _argPrefix( argPrefix), _printTypes( printTypes), _prefixComma( false)
		{}

		void operator()( const SFC::Arg &arg );
		void operator()( const InputArg &inputArg );

		void setArgPrefix( const std::string argPrefix) {
			_argPrefix= argPrefix;
		}

	private:
		std::string& _sig;
		std::string _argPrefix;
		bool _printTypes;
		bool _prefixComma;
	};

	///////////////////////////////////////////////////////////////////////
	class InputArgInitializer : public ArgCopier {
	public:
		InputArgInitializer( const std::string& argsVar, std::string& code) : ArgCopier( argsVar, code ), _argIdx( 0 )
		{}
		void operator()( const SFuncWrapperPrint::InputArg &inputArg );

	private:
		int _argIdx;
	};

	///////////////////////////////////////////////////////////////////////
	class OutputArgCopier : public ArgCopier {
	public:
		OutputArgCopier( const std::string& argsVar, std::string& code ) : ArgCopier( argsVar, code ), _argIdx( 0 )
		{}
		void operator()( const SFC::Arg &arg) {
			appendCode(   copyExpr(  getArgIdx( _argIdx++ ), static_cast< std::string >( arg.name() )  )   );
		}
	private:
		int _argIdx;
	};

	class OutputArgInitializer : public ArgCopier {
	public:
		OutputArgInitializer( std::string& code ) : ArgCopier( "", code ), _argIdx( 0 )
		{}
		void operator()( const SFC::Arg &arg );

	private:
		int _argIdx;
	};


public:

	typedef set< SFC::Arg, ArgIndexComparator > ArgSet;
	typedef set< InputArg, InputArgIndexComparator >  InputArgSet;
	typedef set< SFC::Arg, OutputArgIndexComparator > OutputArgSet;


	SFuncWrapperPrint( 
	 const ArgSet &argSet,
	 const std::string& sFuncName,  // the name of the S-Function (S-Func dll from the C code)
	 const std::string& mainFuncName = "",
	 const std::string& initFuncName = ""
	) :
	 _sfn( sFuncName ),
	 _mfn( mainFuncName.empty() ? sFuncName + "_main" : mainFuncName ),
	 _ifn( initFuncName.empty() ? sFuncName + "_init" : initFuncName )
	{ 
		for( ArgSet::const_iterator arsItr = argSet.begin(); arsItr != argSet.end(); ++arsItr ) {
			const SFC::Arg &arg = *arsItr;
			if ( arg.ptr() )	_oArgs.insert( arg );
			else				_iArgs.insert(  InputArg( arg )  );
		} 
	}

	class EventArgTemp {
	private:
		std::string &_code;
	public:
		EventArgTemp( std::string &code ) : _code( code ) { }
		void operator()( const InputArg &inputArg );
	};

	class EventArgTrigger {
	private:
		std::string &_code;
	public:
		EventArgTrigger( std::string &code ) : _code( code ) { }
		void operator()( const InputArg &inputArg );
	};

	//
	std::ostream& print( std::ostream& os) const;
	//

	///////////////////////////////////////////////////////////////////////////////
	//	The template class defines its member function as returning t1.name() < t2.name()
	template< class T>
	class NameCompareFunctor : public std::binary_function< T, T, bool>
	{
	public:
		bool operator()( const T& t1, const T& t2) const {
			const std::string& t1Name= t1.name();
			const std::string& t2Name= t2.name();
			return t1Name < t2Name;
		}
	};


    static void printWrapper( const std::string sFuncName, const SFC::Program& program);


protected:
	void generateOutputCode( std::string& outputCode) const;

private:
	InputArgSet _iArgs;
	OutputArgSet _oArgs;
	std::string _sfn;
	std::string _mfn;
	std::string _ifn;
};

#endif //SFUNCWRAPPERPRINT_H
