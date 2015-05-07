header {
#include <string>
}

options {
    language="Cpp";
}

class MExprParser extends Parser;

options {
    k=3; // needed for newline junk
    buildAST = true;
}

expr : (WS!)? ( aexpr | LBRACK^ sexpr (WS!)? RBRACK! ) (WS!)? EOF!;

sexpr : rexpr ( (WS!)? SEM^ rexpr )*;

rexpr : (WS!)? ( aexpr | LBRACK^ sexpr (WS!)? RBRACK! ) (  ( WS^ | CMA^ | WS! CMA^ | CMA^ WS! | WS! CMA^ WS! ) ( aexpr | LBRACK^ sexpr (WS!)? RBRACK! )  )*;

aexpr : mexpr ( ( ADD^ | SUB^ ) mexpr )*;

mexpr : unexpr ( ( MUL^ | DIV^ | MOD^ ) unexpr )*;

unexpr : basexpr
       | ( ADD^ | SUB^ ) unexpr
       ;

basexpr : NUM^
        | VAR^
        | LPAREN! aexpr (WS!)? RPAREN!
        ;



class MExprLexer extends Lexer;

options {
    k=4; // needed for newline junk
    charVocabulary='\u0000'..'\u007F'; // allow ascii
}

ADD : '+' ;
SUB : '-' ;
MUL : '*' ;
DIV : '/' ;
MOD : '%' ;

LPAREN: '('   ;
RPAREN: ')'   ;
LBRACK: '['   ;
RBRACK: ']'   ;
SEM   : ';'   ;
CMA   : ','   ;

NUM   : ('0'..'9')+ ('.' ('0'..'9')*)? (('E'|'e')('+'|'-')('0'..'9')+)?
      | ('.')('0'..'9')+(('E'|'e')('+'|'-')('0'..'9')+)?
      ;
      
VAR   : ('A'..'Z'|'a'..'z'|'_')('A'..'Z'|'a'..'z'|'_'|'0'..'9')*
      ;

WS    : ( ' '
        | '\r' '\n'
        | '\n'
        | '\t'
        )+
//        {  $setType( ANTLR_USE_NAMESPACE(antlr)Token::SKIP );  }
      ;
