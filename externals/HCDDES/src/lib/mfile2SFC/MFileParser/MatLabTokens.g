lexer grammar MatLabTokens;

options {
  language=C;
}

@members {
  char acceptTranspose = 0;
}

/* LINE CONTINUATION */
fragment
ELLIPSIS:	'...' ;
CONTINUATION
	:	ELLIPSIS (~NEWLINE)* NEWLINE? { $channel=HIDDEN; } ;

/* KEYWORDS */
BREAK	:	'break';
CATCH	:	'catch';
CONTINUE	:	'continue';
ELSE	:	'else' ;
ELSEIF	:	'elseif' ;
END	:	'end' ;
FOR	:	'for' ;
FUNCTION:	'function' ;
GLOBAL	:	'global' ;
IF	:	'if' ;
RETURN	:	'return' ;
PERSISTENT	:	'persistent';
TRY	:	'try';
WHILE	:	'while' ;
SWITCH  :	'switch' ;
CASE    :	'case' ;
OTHERWISE
        :	'otherwise' ;

/* TRANSPOSE OPERATORS */
CTRANSPOSE
	:	{ acceptTranspose }?=> '\'' ;
TRANSPOSE
	:	{ acceptTranspose }?=> '.\'' ;

/* STRING LITERAL (MUST FROM *AFTER* TRANSPOSE OPERATORS DUE TO ' CHARACTER) */
TEXT
	:	{ !acceptTranspose }?=> ( '.\'' | '\'' ) (  ~( NEWLINE | '\'' ) | '\'\''  )* '\'' ;

/* OTHER OPERATORS */
ASSIGN	:	'='
		{
		  acceptTranspose = 0;
		}
	;
PLASGN	:	'+='
		{
		  acceptTranspose = 0;
		}
	;
MIASGN	:	'-='
		{
		  acceptTranspose = 0;
		}
	;
MUASGN	:	'*='
		{
		  acceptTranspose = 0;
		}
	;
DIASGN	:	'/='
		{
		  acceptTranspose = 0;
		}
	;

AND	:	'&'
		{
		  acceptTranspose = 0;
		}
	;
COLON	:	':'
		{
		  acceptTranspose = 0;
		}
	;
COMMA	:	','
		{
		  acceptTranspose = 0;
		}
	;
DIV	:	'/'
		{
		  acceptTranspose = 0;
		}
	;
LEFTDIV	:	'\\'
		{
		  acceptTranspose = 0;
		}
	;
MINUS	:	'-'
		{
		  acceptTranspose = 0;
		}
	;
MUL	:	'*'
		{
		  acceptTranspose = 0;
		}
	;
NOT	:	'~'
		{
		  acceptTranspose = 0;
		}
	;
OR	:	'|'
		{
		  acceptTranspose = 0;
		}
	;
PLUS	:	'+'
		{
		  acceptTranspose = 0;
		}
	;
POWER	:	'^'
		{
		  acceptTranspose = 0;
		}
	;
PSTDEC	:	'--'
		{
		  acceptTranspose = 0;
		}
	;
PSTINC	:	'++'
		{
		  acceptTranspose = 0;
		}
	;
DOT     :   '.'
		{
		  acceptTranspose = 0;
		}
	;
CNOT	:	'!'
		{
		  acceptTranspose = 0;
		}
	;

fragment  // USED TO DETECT IF A DOT MAY BE PART OF AN ARRAY OPERATOR
ARRAYOPSC
	:	'*'|'/'|'\\'|'^'|'\''
	;
EDIV	:	'./'
		{
		  acceptTranspose = 0;
		}
	;
ELEFTDIV:	'.\\'
		{
		  acceptTranspose = 0;
		}
	;
EMUL	:	'.*'
		{
		  acceptTranspose = 0;
		}
	;
EPOWER	:	'.^'
		{
		  acceptTranspose = 0;
		}
	;

EQUAL	:	'=='
		{
		  acceptTranspose = 0;
		}
	;
GTHAN	:	'>'
		{
		  acceptTranspose = 0;
		}
	;
GTHANE	:	'>='
		{
		  acceptTranspose = 0;
		}
	;
LTHAN	:	'<'
		{
		  acceptTranspose = 0;
		}
	;
LTHANE	:	'<='
		{
		  acceptTranspose = 0;
		}
	;
UNEQUAL	:	'~='
		{
		  acceptTranspose = 0;
		}
	;
CUNEQUAL	:	'!='
		{
		  acceptTranspose = 0;
		}
	;

LAND	:	'&&'
		{
		  acceptTranspose = 0;
		}
	;
LOR	:	'||'
		{
		  acceptTranspose = 0;
		}
	;

SEMI	:	';'
		{
		  acceptTranspose = 0;
		}
	;

/* GROUPING OPERATORS */
LPAREN	:	'('
		{
		  acceptTranspose = 0;
		}
	;
RPAREN	:	')'
		{
		  acceptTranspose = 1;
		}
	;
LBRACK	:	'['
		{
		  acceptTranspose = 0;
		}
	;
RBRACK	:	']'
		{
		  acceptTranspose = 1;
		}
	;

LBRACE	:	'{'
		{
		  acceptTranspose = 0;
		}
	;
RBRACE	:	'}'
		{
		  acceptTranspose = 0;
		}
	;

PERCNT	:	'%' ;
DBLSLS	:	'//' ;

CMTSTA	:	'/*' ;
CMTEND	:	'*/' ;

/* COMMENT */
COMMENT	:	( PERCNT | DBLSLS ) (~NEWLINE)* NEWLINE?
		{
		  acceptTranspose = 0;
		}
	|	CMTSTA .* CMTEND
		{
		  acceptTranspose = 0;
		}
	;

/* NUMERIC LITERALS */
fragment
DIGIT	:	'0'..'9' ;
fragment
MANTISSA:	(  DIGIT+ '.' ~( DIGIT | ARRAYOPSC )  )=> DIGIT+ '.'
	|	DIGIT* '.' DIGIT+
	;
fragment
EXPONENT:	('D'|'d'|'E'|'e') ('+'|'-')? DIGIT+ ;
fragment
IMAGINARYUNIT
	:	'I' | 'i' | 'J' | 'j' ;

INTEGER	:	(  DIGIT+ ( (~'.') | '.' ARRAYOPSC )  )=> DIGIT+
		{
		  acceptTranspose = 1;
		}
	;

DOUBLE	:	INTEGER EXPONENT | MANTISSA EXPONENT?
		{
		  acceptTranspose = 1;
		}
	;

IMAGINARY
	:	( INTEGER | DOUBLE ) IMAGINARYUNIT
		{
		  acceptTranspose = 1;
		}
	;

	/* IDENTIFIER (SHOULD COME *AFTER* ANY RULES THAT START WITH A LETTER */
IDENTIFIER
	:	('a'..'z'|'A'..'Z')('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
		{
		  acceptTranspose = 1;
		}
	;

fragment
HSPACE	:	' ' | '\t' ;
HSPACES	:	HSPACE+
		{
		  acceptTranspose = 0;
		  $channel = HIDDEN;
		}
	;

fragment
NEWLINE	:	'\n' | '\r' | '\f' ;
NEWLINES:	NEWLINE+
		{
		  acceptTranspose = 0;
		}
	;
