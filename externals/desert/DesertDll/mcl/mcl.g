//
//    file: mcl.g
//  author: Greg Nordstrom (original code by Gabor Karsai)
//    date: 6/22/98
//
// This project creates a library file containing an MCL
// (mulitgraph constraint language) parser. There are two
// entry point methods:
//
//   1. bool ParseMCLString(char* constraint)
//
// The first takes the name of a file containing one or more
// constraint statements and returns true if the file was
// successfully parsed, or false if an error occurred.
//
// The second form allows a single constraint to be parsed.
// The method expects a string containing the constraint,
// and returns true if the constraint was successfully
// parsed, or false otherwise.
//
// If either method returns false, an error message is available
// by calling the
//
//   char* GetMCLErrorMessage()
//
// method.
//
// Both methods, if successful, set up certain data structures
// for later use when verifying that models conform to the
// specified constraints. A list of pointers to the constraint
// data structures is returned by the
//
// 	 const List<ClConstraint*>* GetConstraintList()
//
// method.
//
// Note: The project uses the dlg compiler's -cl MCLLexer switch
// to create an MCLLexer object instead of a DLGLexer object.
//
//  6/10/98 - Data structures updated for MS VC++ (Akos)
//  6/11/98 - Prioritized constraints added
//  6/22/98 - Bug fixes (Akos)
//  10/7/98 - if-then-else-endif fixed (now else is optional)
// GREG: Re-add the "cDescription" (see mclold.g)
//
//////////////////////////////////////////////////////////////////
#header <<
#undef DEBUG	// Uncomment this to see run-time messages -- this is only
				// useful when using a console app driver.

//#include "afx.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//#define new DEBUG_NEW

#include "defs.h"
#include "cldef.h"
#include "pure.h"
#include "MCLLexer.h"
#include "PBlackBox.h"
typedef ANTLRCommonToken ANTLRToken;
extern CTypedPtrList<CPtrList, CStringList*> g_stringlists;
extern CObList g_parlist;
extern ClBase *g_top;
>>

<<
typedef ANTLRCommonToken ANTLRToken;

bool ParseMCLString(const char *constraintString) {
  try {
    return MCLParser::parseString(constraintString);
  } catch (const MCLLexerError& err) {
	MCLParser::setError(const_cast<char*>(err.error.c_str()));
	return false;
  }
}

char* GetMCLErrorMessage() {
  return MCLParser::getError();
}

bool MCLParser::parseErr = false;
bool MCLParser::parsingFile = false;
char MCLParser::errMsg[100];
MCLLexer* MCLParser::currentLexer = nil;
bool _at_end = false;
CTypedPtrList<CPtrList, CStringList*> g_stringlists;
CObList g_parlist;
ClBase *g_top;
>>

#lexclass	START
#token				"@"				<<extern bool _at_end; _at_end = true;>>
#token				"[\ \t]+"		<<skip();>>
#token				"\n"			<<skip(); newline();>>
#token				"\r"			<<skip();>>
#token 	INN			"in"
#token  IF			"if"
#token  THEN		"then"
#token  ELSE		"else"
#token  ENDIF		"endif"
#token  SELF		"self"
#token  AND			"and"
#token  OR			"or"
#token  XOR			"xor"
#token  IMPLIES		"implies"
#token  SET			"Set"
#token  BAG			"Bag"
#token  SEQUENCE	"Sequence"
#token  COLLECTION	"Collection"
#token  NOT			"not"
#token  CONSTRAINT	"constraint"
#token	REQ			"req"
#token	REL			"rel"
#token	RELATION	"relation"
#token	SEMI		";"
#token	LBRACE		"\{"
#token	RBRACE		"\}"
#token	COLON		":"
#token	LBRACKET	"\["
#token	RBRACKET	"\]"
#token	STAR		"\*"
#token	LPAR		"\("
#token	RPAR		"\)"
#token	COMMA		","
#token	PLUS		"\+"
#token	MINUS		"\-"
#token	MUL			"\*"
#token	DIV			"\/"
#token	DOT			"."
#token	ARROW		"\->"
#token	DARROW		"<\-\>"
#token	CARET		"\^"
#token	DDOT		".."

// Himanshu: 02/14/2012: Now that we have got double variables working in
// desert, adding support for various math functions for desert constraints
// ln(x) is logarithm to the base e(2.71828)
// exp(x) is the exponent of e(2.71828) raised to power of x
#token	SIN		"sin"
#token	COS		"cos"
#token	TAN		"tan"
#token	ASIN	"asin"
#token	ACOS	"acos"
#token	ATAN	"atan"
#token	SINH	"sinh"
#token	COSH	"cosh"
#token	TANH	"tanh"
#token	ASINH	"asinh"
#token	ACOSH	"acosh"
#token	ATANH	"atanh"
#token	LOG2	"log2"
#token	LOG10	"log10"
#token	LN		"ln"
#token	EXP		"exp"
#token	SQRT	"sqrt"
#token	SIGN	"sign"
#token	RINT	"rint"
#token	ABS		"abs"


#token	PRIORITY	"priority"

#token		ON					"on"
#token		CREATE_EVENT		"create_event"
#token		OPEN_EVENT			"open_event"
#token		ADD_EVENT			"add_event"
#token		CLOSE_EVENT			"close_event"
#token		DELETE_EVENT		"delete_event"
#token		SAVE_EVENT			"save_event"
#token		CLICK_EVENT			"click_event"
#token		CONNECT_EVENT		"connect_event"
#token		DISCONNECT_EVENT	"disconnect_event"
#token		DEMAND_EVENT		"demand_event"
#tokclass	EDITING_EVENT	{
								CREATE_EVENT OPEN_EVENT ADD_EVENT	
								CLOSE_EVENT	DELETE_EVENT SAVE_EVENT
								CLICK_EVENT CONNECT_EVENT DISCONNECT_EV
								DEMAND_EVENT EDITING_EVENT
							}

// Process C++ style comments
#lexclass	CPP_COMMENT
#token					"@"				<<extern bool _at_end; _at_end = true;>>
#token					"[\n\r]"		<<
											mode(START); skip(); newline();
										>>
#token					"~[\n\r]+"		<< skip(); >>
#lexclass	START
#token					"//"			<<
											mode(CPP_COMMENT);
											skip();
										>>

// see end of file for more token defs

class MCLParser {
  << 
public:
  static bool parseString(const char *constraintString) {
	bool res = false;
    parseErr = false;
	setError("");
	parsingFile = false;

    int parseSignal;
	DLGStringInput *in;
	MCLLexer *scan;
	_ANTLRTokenPtr tok;
	ANTLRTokenBuffer *pipe;
	MCLParser *p;
	extern bool _at_end;

	in = new DLGStringInput((const DLGChar *)constraintString);
	scan = new MCLLexer(in);
	pipe = new ANTLRTokenBuffer(scan);
	tok = new ANTLRToken;
	scan->setToken(tok);
	p = new MCLParser(pipe);
	p->init();
	g_top = 0;

	_at_end = false;
    while(!_at_end && !parseErr){
		p->cdef(&parseSignal);	// parameter needed for exception handling
	}
    	res = !parseErr;
	{
	  POSITION pos = g_stringlists.GetHeadPosition();
	  while(pos) delete g_stringlists.GetNext(pos);
	  g_stringlists.RemoveAll();
	}
	{
	  POSITION pos = g_parlist.GetHeadPosition();
	  while(pos) delete (ClParList *)g_parlist.GetNext(pos);
	  g_parlist.RemoveAll();
	}
	delete p;
	delete tok;
	delete pipe;
	delete scan;
	delete in;
	if (!res) delete g_top;
	return res;
  }

  static char* getError() {
	return(errMsg);
  }

private:
  static bool parseErr;
  static bool parsingFile;
  static MCLLexer *currentLexer;
  static char errMsg[100];

public:
  static void setError(char* e) {
	strcpy(errMsg, e);
  }
>>

exception
	default: 
	<<
		char dummy[100];
		if(parsingFile){
			sprintf(dummy,"Syntax error at line %d at token \"%s\"",
			  currentLexer->line(),LT(1)->getText());
			setError(dummy);
		}
		else{
			sprintf(dummy,"Syntax error at token \"%s\"", LT(1)->getText());
			setError(dummy);
		}
		suppressSignal;
		parseErr = true;
	>>

name > [ CString res ]
		: id:IDENT	<< $res = CString($id->getText()); >>	
        ; 

cdef	:
		<<
			CString _name; CString cName, mName, aName, eName, cDescription;
			int cPriority;
			ClParList* _cpars = new ClParList; g_parlist.AddTail(_cpars); ClExpr* _cexpr;
		>>
		{ INN catname>[cName]
		{ DOT modelname>[mName] { DOT aspectname>[aName] }}}
		{ ON eventname>[eName] }
		CONSTRAINT name>[_name]
		LPAR  { cpars[_cpars] } RPAR
		priority > [cPriority]
		description > [cDescription]
		LBRACE cexpr>[_cexpr] RBRACE
		<<
			#ifdef DEBUG
				printf("constraint: %s\n", _name);
				printf("\tdescription: %s\n", cDescription);
				printf("\t   priority: %d\n", cPriority);
				printf("\t   category: %s\n", cName);
				printf("\t      model: %s\n", mName);
				printf("\t     aspect: %s\n", aName);
				printf("\t      event: %s\n", eName);
			#endif
		>>
		<<
		 ClConstraint::Make(_name,_cpars,_cexpr,cName,mName,aName,eName,cPriority,cDescription);
		 POSITION pos; if (pos=g_parlist.Find(_cpars)) g_parlist.RemoveAt(pos);
		 g_top = ClConstraint::GetLastConstraint();
		>>
	;

priority > [int res]
	:	PRIORITY "=" id:INTEGER	<<
									int n;
									n = atoi($id->getText());
									if(n<0 )
										$res = nil;
									else if(n > 3)
										$res = 3;
									else
										$res = n;
								>>
	|	<<$res = nil;>>
	;
description > [CString res]
	:	id:STR					<<$res = $id->getText();>>
	|	<<$res = "";>>
	;
catname > [CString res]
	:	name>[$res]
	;
modelname > [CString res]
	:	name>[$res]
	;
aspectname > [CString res]
	:	name>[$res]
	;
eventname > [CString res]
	:	id:EDITING_EVENT		<<$res = $id->getText();>>
	;

cpars [ClParList* cpars]
	:  << ClPar* _cpar; >>
	  cpar>[_cpar] << $cpars->AddTail(_cpar); >>
	  ( COMMA cpar>[_cpar] << $cpars->AddTail(_cpar); >>  )*
	;

cpar	> [ClPar* res]
	: << CString pName, pType; $res = nil; >>
	   name>[pName] { ":" name>[pType] }
	  << $res = new ClPar(pName,pType); g_top = $res; >>
	;

cexpr	> [ClExpr* res]
	: << ClLExpr* _lexpr; $res = nil; >>
	  lexpr>[_lexpr]
	  << $res = new ClExpr(_lexpr); g_top = $res; >>
	;

ifexpr	> [ClIfExpr* res]
	: << ClExpr *_condE, *_thenE, *_elseE = nil; $res = nil; >>
//	: << ClExpr *_condE, *_thenE, *_elseE; >>
//	  IF cexpr>[_condE] THEN cexpr>[_thenE] ELSE cexpr>[_elseE] ENDIF
	  IF cexpr>[_condE] THEN cexpr>[_thenE] {ELSE cexpr>[_elseE]} ENDIF
	  << $res = new ClIfExpr(_condE,_thenE,_elseE); g_top = $res; >>
	;

lexpr	> [ClLExpr* res]
	: << ClRelExpr* _relExpr; 
	     ClLExpr::LOp _lOp = ClLExpr::noOp;
	     ClLExpr* _next = nil; $res = nil;
	   >>
	  relexpr>[_relExpr] lexpr_r>[_lOp,_next]
	  << $res = new ClLExpr(_relExpr,_lOp,_next); g_top = $res; >>
	;


lexpr_r	> [ClLExpr::LOp _lOp, ClLExpr* _next]
	: << $_lOp = ClLExpr::noOp; $_next = nil; 
	     ClRelExpr* _relExpr;
	     ClLExpr::LOp rLOp = ClLExpr::noOp;
	     ClLExpr *rNext = nil; >>
	  { lop>[$_lOp] relexpr>[_relExpr] lexpr_r>[rLOp,rNext]
	    << $_next = new ClLExpr(_relExpr,rLOp,rNext); >>
	  }
	;

relexpr	> [ClRelExpr* res]
	: << ClAddExpr* _addExpr;
	     ClRelExpr::RelOp _relOp = ClRelExpr::noOp;
	     ClAddExpr* _next = nil; $res = nil; >>
	  addexpr>[_addExpr] { rop>[_relOp] addexpr>[_next] }
	  << $res = new ClRelExpr(_addExpr,_relOp,_next); g_top = $res; >>
	;

addexpr	> [ClAddExpr* res]
	: << ClMulExpr* _mulExpr;
	     ClAddExpr::AddOp _addOp = ClAddExpr::noOp;
	     ClAddExpr* _next = nil; $res = nil;>>
	   mulexpr>[_mulExpr] addexpr_r>[_addOp,_next]
	   << $res = new ClAddExpr(_mulExpr,_addOp,_next); g_top = $res; >>
	;

addexpr_r> [ClAddExpr::AddOp _addOp, ClAddExpr* _next]
	: << $_addOp = ClAddExpr::noOp; $_next = nil;
	     ClMulExpr* _mulExpr; 
	     ClAddExpr::AddOp rAddOp = ClAddExpr::noOp;
	     ClAddExpr *rNext = nil; >>
	  { addop>[$_addOp] mulexpr>[_mulExpr] addexpr_r>[rAddOp,rNext]
	    << $_next = new ClAddExpr(_mulExpr,rAddOp,rNext); >>
	  }
	;

mulexpr	> [ClMulExpr* res]
	: << ClUnaExpr* _unaExpr;
	     ClMulExpr::MulOp _mulOp = ClMulExpr::noOp;
	     ClMulExpr* _next = nil; $res = nil;>>
	  unaexpr>[_unaExpr] mulexpr_r>[_mulOp,_next]
	  << $res = new ClMulExpr(_unaExpr,_mulOp,_next); g_top = $res; >>
	;

mulexpr_r> [ClMulExpr::MulOp _mulOp, ClMulExpr* _next]
	: << $_mulOp = ClMulExpr::noOp; $_next = nil;
	     ClUnaExpr* _unaExpr; 
	     ClMulExpr::MulOp rMulOp = ClMulExpr::noOp;
	     ClMulExpr *rNext = nil; >>
	  { mulop>[$_mulOp] unaexpr>[_unaExpr] mulexpr_r>[rMulOp,rNext]
	    << $_next = new ClMulExpr(_unaExpr,rMulOp,rNext); >>
	  }
	;

unaexpr	> [ClUnaExpr* res]
	: << ClPostfixExpr* _postfixExpr; ClUnaExpr::UnaOp _unaOp; $res = nil; >>
	  ( unaryop>[_unaOp] postfixexpr>[_postfixExpr] )
	  << $res = new ClUnaExpr(_postfixExpr,_unaOp); g_top = $res; >>
	| postfixexpr>[_postfixExpr] 
	  << $res = new ClUnaExpr(_postfixExpr); g_top = $res; >>
	;

postfixexpr > [ClPostfixExpr* res]
	: << ClPrimExpr* _primExpr; 
	     ClPostfixCall* _postfixCall = nil; $res = nil; >>	
	  primexpr>[_primExpr] postfixcall>[_postfixCall]
	  << $res = new ClPostfixExpr(_primExpr,_postfixCall); g_top = $res; >>
	;

postfixcall > [ClPostfixCall* res]
	: << ClPostfixCall::CallOp _callOp = ClPostfixCall::noOp;
	     ClCall* _call = nil;
	     ClPostfixCall* _next = nil; $res = nil; $res = nil; >>
	  { ( ( DOT 	<<_callOp = ClPostfixCall::objCallOp; >>
		// single_object . call 
	      | ARROW 	<<_callOp = ClPostfixCall::collCallOp;>>
		// collection_object -> call 
	      | CARET	<<_callOp = ClPostfixCall::stdCallOp; >>	
		// single_object ^ standard_function_call
	      ) 
	      call>[_call]
	    )
	    postfixcall>[_next]
	    << $res = new ClPostfixCall(_callOp,_call,_next); g_top = $res; >>
	  }
	;

primexpr > [ClPrimExpr* res]
	: <<ClLitColl* _litColl; ClLit* _lit;
	    ClObjName* _objName; ClCallPars *_callPars = nil;
	    ClExpr* _cexpr; ClIfExpr* _ifExpr; $res = nil;
	  >>
	  litcoll>[_litColl] << $res = new ClPrimExpr(_litColl); g_top = $res; >>
	| lit>[_lit]	<< $res = new ClPrimExpr(_lit); g_top = $res; >>
	| objname>[_objName] callpars>[_callPars]
			// object
	  		// predicate ( parameters )
	  << $res = new ClPrimExpr(_objName,_callPars); g_top = $res; >>
	| LPAR cexpr>[_cexpr] RPAR
	  << $res = new ClPrimExpr(_cexpr); g_top = $res; >>
	| ifexpr>[_ifExpr] 
	  << $res = new ClPrimExpr(_ifExpr); g_top = $res; >>
	;

callpars > [ClCallPars* res]
	: << ClDecl* _decl = nil, *_dummy;
	     ClActParList* _actParList = nil; 
		 $res = nil; >>
	  { LPAR
	     ( ( decl>[_dummy] )? decl>[_decl] { actparlist>[_actParList] } 
						// Decls will enumerate the
					      	// objects of the collection
	       | { actparlist>[_actParList] } 
	     )
	     << $res = new ClCallPars(_decl,_actParList); g_top = $res; >>
	    RPAR
	  }
	;

lit	> [ClLit *res]
	: << CString str; ClNumber* cnum; $res = nil; >>
	  string>[str]	<< $res = new ClLit(str); g_top = $res; >>
	| number>[cnum]	<< $res = new ClLit(cnum); g_top = $res; >>
	;

tname	> [ClType *res]
	: << CString _name; $res = nil; >>
	  name>[_name] 			// type name
	  << $res = new ClType(_name); g_top = $res; >>
	;

litcoll	> [ClLitColl *res]
	: << ClLitColl::CollType _coll;
	     ClExprlRange* _range= nil; $res = nil; >>
	  coll>[_coll] LBRACE { cexprlrange>[_range] } RBRACE
	  << $res = new ClLitColl(_coll,_range); g_top = $res; >>
	;

cexprlrange > [ClExprlRange* res]
	: << ClExpr *_from = nil, *_to = nil, *_next = nil; 
	     ClExprList *_list = nil; $res = nil; >>
	  cexpr>[_from] 
	   { ( << _list = new ClExprList;>>
	       (COMMA cexpr>[_next] << _list->AddTail(_next);>> )+ 
	     )
	     |  ( ".." cexpr>[_to] ) 
	   }
	  << $res = _list ? 
			new ClExprlRange(_from,_list) :
			new ClExprlRange(_from,_to); g_top = $res; >>
	;

call	> [ClCall* res]
	: << CString _name; ClCallPars *_pars; $res = nil; >>
	  name>[_name] callpars>[_pars]
		 // name:
			// - standard object function
			// - standard collection function
			// - attribute/part/rel name
	  << $res = new ClCall(_name,_pars); g_top = $res; >>
	;

decl	> [ClDecl* res]
	: << CStringList* _names = new CStringList; 
	     CString _name; ClType* _tname = nil; g_stringlists.AddTail(_names); $res = nil; >>
	  name>[_name] << _names->AddTail(_name); >>
	  ( COMMA name>[_name] << _names->AddTail(_name);>> )* 
	  { ":" tname>[_tname] } "\|"
	  << $res = new ClDecl(_names,_tname); g_top = $res;
	     POSITION pos; if (pos=g_stringlists.Find(_names)) g_stringlists.RemoveAt(pos); >>
	;

objname	> [ClObjName *res]
	: << CString _name; $res = nil; >>
	  name>[_name] << $res = new ClObjName(_name); g_top = $res; >>
	 		// name:
			// - a variable that evaluates to an object
			// - a type name that evaluates to a set of objects
	| SELF 	<< $res = new ClObjName(); g_top = $res; >>
		// The context where this cexpr belongs to (M/E/R)
	;

actparlist >[ClActParList* res]
	: << ClExpr* _cexpr;
	     ClExprList* _list = new ClExprList; $res = nil; >>
	  cexpr>[_cexpr] << _list->AddTail(_cexpr); >>
	  ( COMMA cexpr>[_cexpr] << _list->AddTail(_cexpr); >> )*
	  << $res = new ClActParList(_list); g_top = $res; >> 
	;

lop	>[ClLExpr::LOp res]
	: AND 	<< $res = ClLExpr::andOp; >>
	| OR 	<< $res = ClLExpr::orOp; >>
	| XOR 	<< $res = ClLExpr::xorOp; >>
	| IMPLIES << $res = ClLExpr::implOp; >>
	;

coll	>[ClLitColl::CollType res]
	: SET 	<< $res = ClLitColl::setColl; >>
	| BAG	<< $res = ClLitColl::bagColl; >>
	| SEQUENCE 	<< $res = ClLitColl::seqColl; >>
	| COLLECTION	<< $res = ClLitColl::collColl; >>
	;

rop	>[ClRelExpr::RelOp res]
	: "="	<< $res = ClRelExpr::eqOp; >>
	| "<" 	<< $res = ClRelExpr::ltOp; >>
	| ">" 	<< $res = ClRelExpr::gtOp; >>
	| ">=" 	<< $res = ClRelExpr::geOp; >>
	| "<=" 	<< $res = ClRelExpr::leOp; >>
	| "<>"	<< $res = ClRelExpr::neOp; >>
	;

addop	>[ClAddExpr::AddOp res]
	: PLUS	<< $res = ClAddExpr::plusOp; >>
	| MINUS << $res = ClAddExpr::minusOp; >>
	;

mulop	>[ClMulExpr::MulOp res]
	: MUL 	<< $res = ClMulExpr::starOp; >>
	| DIV 	<< $res = ClMulExpr::perOp; >>
	;

unaryop	>[ClUnaExpr::UnaOp res]
	: MINUS << $res = ClUnaExpr::minOp; >>
	| NOT	<< $res = ClUnaExpr::notOp; >>
	| SIN	<< $res = ClUnaExpr::sinOp; >>
	| COS	<< $res = ClUnaExpr::cosOp; >>
	| TAN	<< $res = ClUnaExpr::tanOp; >>
	| ASIN	<< $res = ClUnaExpr::asinOp; >>
	| ACOS	<< $res = ClUnaExpr::acosOp; >>
	| ATAN	<< $res = ClUnaExpr::atanOp; >>
	| SINH	<< $res = ClUnaExpr::sinhOp; >>
	| COSH	<< $res = ClUnaExpr::coshOp; >>
	| TANH	<< $res = ClUnaExpr::tanhOp; >>
	| ASINH	<< $res = ClUnaExpr::asinhOp; >>
	| ACOSH	<< $res = ClUnaExpr::acoshOp; >>
	| ATANH	<< $res = ClUnaExpr::atanhOp; >>
	| LOG2	<< $res = ClUnaExpr::log2Op; >>
	| LOG10	<< $res = ClUnaExpr::log10Op; >>
	| LN	<< $res = ClUnaExpr::lnOp; >>
	| EXP	<< $res = ClUnaExpr::expOp; >>
	| SQRT	<< $res = ClUnaExpr::sqrtOp; >>
	| SIGN	<< $res = ClUnaExpr::signOp; >>
	| RINT	<< $res = ClUnaExpr::rintOp; >>
	| ABS	<< $res = ClUnaExpr::absOp; >>
	;

string 	> [ CString res ]
	: str:STR	<< $res = CString(str->getText());
			int n = $res.GetLength();
			$res = $res.Left(n - 1).Right(n - 2); >>	
        ; 

number	> [ClNumber* res]
	: r:REAL << double d = 0; sscanf(r->getText(),"%lf",&d);
		    $res = new ClNumber(d); g_top = $res; >>
	| n:INTEGER	<< int i = 0; sscanf(n->getText(),"%d",&i);
		    $res = new ClNumber(i); g_top = $res; >>
	;

}
// These token defs must remain at the end, so other tokens
// are not "mistaken" for generic strings or identifiers.

#token	STR			"\"~[\"]*\""
#token 	INTEGER			"[0-9]+"
#token  REAL		"([0-9]+.[0-9]* | [0-9]*.[0-9]+) {[eE]{[\-\+]}[0-9]+}"
#token 	IDENT		"[a-zA-Z][a-zA-Z0-9_]*"

// end of mcl.g