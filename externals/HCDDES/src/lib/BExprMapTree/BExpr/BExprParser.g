grammar BExprParser;

options {
    language=C;
    output=AST;
    k=2; // needed for newline junk
}

expr
	: CBL^ pexpr CBR! EOF!;

pexpr
	: cexpr ( SEM! cexpr )*;

cexpr
	: ( NME^ | CBL! NME^ CMA! CBL! pexpr CBR! CBR! );

nexpr
	: dexpr ( CMA^ aexpr )?;

aexpr
	: dexpr ( CMA! dexpr )*;

dexpr
	: NME^ ( DOT! NME )*;


CBL   : '{'   ;
CBR   : '}'   ;
CMA   : ','   ;
SEM   : ';'   ;
DOT   : '.'   ;

NME   : ('A'..'Z'|'a'..'z'|'_')('A'..'Z'|'a'..'z'|'_'|'0'..'9')*   ;


WS    : ( ' ' | '\r' | '\n' | '\t' )+ { $channel = HIDDEN; } ;
