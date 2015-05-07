#ifndef _SCOPESYMBOLTABLE_
#define _SCOPESYMBOLTABLE_

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

class ScopeSymbolTable {
public:
	class Exception : public std::exception {
	private:
		std::string _message;
	public:
		Exception( void ) throw() { }
		Exception( const std::string &message ) throw() { _message = message; }
		~Exception( void ) throw() { }
		void setMessage( const std::string &message ) throw() { _message = message; }
		virtual const char *what( void ) const throw() { return _message.c_str(); }
	};

	typedef std::set< std::string > SymbolSet;
	typedef std::vector< SymbolSet > ScopeStack;

	typedef std::vector< std::string > SymbolStack;
	typedef std::vector< std::string > StringVector;
	typedef std::map< std::string, SymbolStack > SymbolMap;
	typedef std::map< std::string, std::string > SpecialSymbolMap;

	class UniqueVarNameFunctor {
	public:
		typedef boost::shared_ptr< UniqueVarNameFunctor > SP;

		virtual std::string operator()( const std::string &varName ) { return varName; }
		virtual SP clone( void ) const { return SP(  new UniqueVarNameFunctor( *this )  ); }
	};

	class DestroyUniqueVarFunctor {
	public:
		typedef boost::shared_ptr< DestroyUniqueVarFunctor > SP;

		virtual void operator()( const std::string &varName ) const { };
		virtual SP clone( void ) const { return SP(  new DestroyUniqueVarFunctor( *this )  ); }
	};

	class LimitedSplitPredicate {
	public:
		typedef boost::shared_ptr< int > IntSP;

	private:
		std::string _delimiters;
		int _maxSplits;
		IntSP _noSplits;

	public:
		LimitedSplitPredicate( const std::string &delimiters, int maxSplits ) :_delimiters( delimiters ), _maxSplits( maxSplits ), _noSplits( new int ) {
			*_noSplits = 0;
		}

	bool operator()( char c ) const {
		if ( _maxSplits <  0 ) {
			return _delimiters.find( c ) != std::string::npos;
		}
		if (  *_noSplits >= _maxSplits || _delimiters.find( c ) == std::string::npos  ) {
			return false;
		}
		++*_noSplits;
		return true;
	  }
	};

private:
	SymbolMap _symbolMap;
	ScopeStack _scopeStack;
	SpecialSymbolMap _specialSymbolMap;
	UniqueVarNameFunctor::SP _uniqueVarNameFunctorSP;
	DestroyUniqueVarFunctor::SP _destroyUniqueVarFunctorSP;

public:
	ScopeSymbolTable( void ) : _uniqueVarNameFunctorSP( new UniqueVarNameFunctor() ), _destroyUniqueVarFunctorSP( new DestroyUniqueVarFunctor() ) { }

	void setUniqueVarNameFunctor( const UniqueVarNameFunctor &uniqueVarNameFunctor ) {
		_uniqueVarNameFunctorSP = uniqueVarNameFunctor.clone();
	}

	void setDestroyUniqueVarFunctor( const DestroyUniqueVarFunctor &destroyUniqueVarFunctor ) {
		_destroyUniqueVarFunctorSP = destroyUniqueVarFunctor.clone();
	}

	void newScope( void ) {
		_scopeStack.push_back( SymbolSet() );
	}

	void deleteScope( void );

	std::string addSymbol( const std::string &varName );

	void addSpecialSymbol( const std::string &varName, const std::string &value ) {
		_specialSymbolMap[ varName ] = value;
	}

	void deleteSpecialSymbol( const std::string &varName ) {
		_specialSymbolMap.erase( varName );
	}

	void clearSpecialSymbols( void ) {
		_specialSymbolMap.clear();
	}

	bool isInCurrentScope( const std::string &varName ) {
		if ( _scopeStack.empty() ) return false;
		SymbolSet &symbolSet = _scopeStack.back();
		return symbolSet.find( varName ) != symbolSet.end();
	}

	bool atTopLevel( const std::string &varName ) {
		SymbolMap::iterator sbmItr = _symbolMap.find( varName );
		if ( sbmItr == _symbolMap.end() ) return true;
		return sbmItr->second.size() <= 1;
	}

	std::string getSymbol( const std::string &varName ) {
		SymbolMap::iterator sbmItr = _symbolMap.find( varName );
		if ( sbmItr == _symbolMap.end() ) {
			SpecialSymbolMap::iterator ssmItr = _specialSymbolMap.find( varName );
			return ssmItr == _specialSymbolMap.end() ? varName : ssmItr->second;
		}
		return sbmItr->second.back();
	}

	std::string performSimpleSubstitution( std::string inputString );

	void throwException( const std::string &message ) {
		std::cerr << message << std::endl;
		throw Exception( message );
	}

};

#endif // _SCOPESYMBOLTABLE_
