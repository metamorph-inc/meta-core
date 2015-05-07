#ifndef _ISIS_ARGRETURN
#define _ISIS_ARGRETURN

class ArgReturn {
public:
	enum Type { EXPRRETURN, COLONEXPR };
private:
	ExprReturnSP _exprReturnSP;
	Type _type;

protected:
	ArgReturn( void ) : _type( COLONEXPR ) { }
	ArgReturn( ExprReturnSP exprReturnSP ) : _exprReturnSP( exprReturnSP ), _type( EXPRRETURN ) { }

public:
	static ArgReturnSP create( void ) { return ArgReturnSP( new ArgReturn() ); }
	static ArgReturnSP create( ExprReturnSP exprReturnSP ) {
		return ArgReturnSP(  new ArgReturn( exprReturnSP )  );
	}

	static const std::string getName( void ) { return "ArgReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }

	ExprReturnSP getExprReturnSP( void ) const { return _exprReturnSP; }
	Type getType( void ) { return _type; }
};

#endif
