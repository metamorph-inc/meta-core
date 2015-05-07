parser grammar MatLab;

options {
  language=C;
  tokenVocab=MatLabTokens;
  output=AST;
}

tokens {
  EXPR;
  IMAGINARY;
  FBODY;
  FCALL;
  FINPUT;
  FNAME;
  FOUTPUT;
  MATRIX;
  ZERO;
  ROW;
}

scope Matrix {
  int scope;
}

@members {
  const int NONE      = 0;
  const int MATRIXDEF = 1;
  const int LD        = 2;
  
  int MatLabStateflowFlag = 0;
}


input
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	:	scriptMFile
	|	functionMFile
	;

scriptMFile
	:	delimiter? statement_list?
	;

delimiter
	:	null_lines empty_lines?
	|	empty_lines
	;

null_lines
	:	null_line+
	;

null_line
	:	empty_lines? ( COMMA | SEMI )!
	;

empty_lines
	:	line+
	;

line
	:	COMMENT!
	|	NEWLINES!
	;

statement_list
	:	statement ( delimiter statement )* delimiter?
	;

statement
options {
  k=4;
}
// ASSIGNMENTS MOVED UP TO INCORPORATE SYNTACTIC PREDICATES
// CAN'T PUT IN TEST { ( MatLabStateflowFlag == 1 ) }?=> ON 2ND AND 3RD ALTERNATIVES
//  IT INCREASES THE AMOUNT OF CODE BY 6 TIMES -- VS BUILD WON'T COMPLETE!
	:	(    reference (   ASSIGN | (  { MatLabStateflowFlag != 0 }?=> ( PLASGN | MIASGN | MUASGN | DIASGN ) )   )    )=>
	    reference ( ASSIGN | PLASGN | MIASGN | MUASGN | DIASGN )^ expr
	|	(   reference (  { MatLabStateflowFlag != 0 }?=> ( PSTDEC | PSTINC )  )   )=> reference ( PSTDEC | PSTINC )^
	|	( s_assignee_matrix ASSIGN )=> s_assignee_matrix ASSIGN^ expr
	|	( m_assignee_matrix ASSIGN )=> m_assignee_matrix ASSIGN^ reference

	|	persistent_variable
	|	expr
//	|	command_form
	|	for_command
	|	if_command
	|	global_command
	|	while_command
	|	return_command
	|	switch_command
	|	try_catch_command
	|	BREAK
	|	CONTINUE
	;

persistent_variable
	:	PERSISTENT^ IDENTIFIER
	;

command_form
	:	IDENTIFIER^ TEXT+
	;

text_list
	:	TEXT+
	;

expr_entry
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	:	(COMMENT)* ( expr (COMMENT)* )?
	;

expr
	:	land_expr ( LOR^ land_expr )*
	;

expr_with_end
	:	land_expr_with_end ( LOR^ land_expr_with_end )*
	;

land_expr
	:	or_expr ( LAND^ or_expr )*
	;

land_expr_with_end
	:	or_expr_with_end ( LAND^ or_expr_with_end )*
	;

or_expr	:	and_expr ( OR^ and_expr )*
	;

or_expr_with_end
	:	and_expr_with_end ( OR^ and_expr_with_end )*
	;

and_expr
	:	compare_expr ( AND^ compare_expr )*
	;

and_expr_with_end
	:	compare_expr_with_end ( AND^ compare_expr_with_end )*
	;

compare_expr
	:	colon_expr (   (  ( LTHAN | LTHANE | GTHAN | GTHANE | EQUAL | UNEQUAL )^ | ( { MatLabStateflowFlag != 0 }?=> CUNEQUAL^ )  )  colon_expr   )*
	;

compare_expr_with_end
	:	colon_expr_with_end
	     (   (  ( LTHAN | LTHANE | GTHAN | GTHANE | EQUAL | UNEQUAL )^ | ( { MatLabStateflowFlag != 0 }?=> CUNEQUAL^ )  )  colon_expr_with_end   )*
	;

colon_expr
scope
{
  int third_operand;
}
@init {
  $colon_expr::third_operand = 0;
}
	:	add_expr
		(
		  (
		    (  { !$colon_expr::third_operand }?=> COLON^ { $colon_expr::third_operand = 1; }  ) 
		    |                                     COLON! { $colon_expr::third_operand = 0; }
		  ) add_expr
		)*
	;

colon_expr_with_end
scope
{
  int third_operand;
}
@init {
  $colon_expr_with_end::third_operand = 0;
}
	:	add_expr_with_end
		(
		  (
		    (  { !$colon_expr_with_end::third_operand }?=> COLON^ { $colon_expr_with_end::third_operand = 1; }  ) 
		    |                                              COLON! { $colon_expr_with_end::third_operand = 0; }
		  ) add_expr_with_end
		)*
	;

add_expr
	:	mul_expr
		(
		  {
//		    (
//                    $Matrix::scope == NONE ||
//		      (  (CommonToken)( (CommonTokenStream)input ).LT(-1)  ).getStopIndex() + 1 ==
//		       (  (CommonToken)( (CommonTokenStream)input ).LT(1)  ).getStartIndex() ||
//		      (  (CommonToken)( (CommonTokenStream)input ).LT(1)  ).getStopIndex() + 1 !=
//		       (  (CommonToken)( (CommonTokenStream)input ).LT(2)  ).getStartIndex()
//                  )

		    (
                $Matrix::scope == NONE ||
		        LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  ) ||
		        LT( 1 )->getStopIndex(  LT( 1 )  ) + 1   != LT( 2 )->getStartIndex(  LT( 2 )  )
            )
		  }?=>
		  ( PLUS | MINUS )^ mul_expr
		)*
	;

add_expr_with_end
	:	mul_expr_with_end
		(
		  {
		    (
                $Matrix::scope == NONE ||
		        LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  ) ||
		        LT( 1 )->getStopIndex(  LT( 1 )  ) + 1   != LT( 2 )->getStartIndex(  LT( 2 )  )
            )
		  }?=>
		  ( PLUS | MINUS )^ mul_expr_with_end
		)*
	;

mul_expr
	:	unary_expr (  ( MUL | EMUL | DIV | EDIV | LEFTDIV | ELEFTDIV )^ unary_expr  )*
	;

mul_expr_with_end
	:	unary_expr_with_end (  ( MUL | EMUL | DIV | EDIV | LEFTDIV | ELEFTDIV )^ unary_expr_with_end  )*
	;

unary_expr
	:	(  ( NOT | PLUS | MINUS )^ | ( { MatLabStateflowFlag != 0 }?=> CNOT^ )  )? transpose_expr
	;

unary_expr_with_end
	:	(  ( NOT | PLUS | MINUS )^ | ( { MatLabStateflowFlag != 0 }?=> CNOT^ )  )? transpose_expr_with_end
	;

transpose_expr
	:	power_expr (  ( TRANSPOSE | CTRANSPOSE )^  )?
	;

transpose_expr_with_end
	:	power_expr_with_end (  ( TRANSPOSE | CTRANSPOSE )^  )?
	;

power_expr
	:	atom_expr (  ( POWER | EPOWER )^ exponent_unary_expr  )*
	;

power_expr_with_end
	:	atom_expr_with_end (  ( POWER | EPOWER )^ exponent_unary_expr_with_end  )*
	;

exponent_unary_expr
	:	(  ( NOT | PLUS | MINUS )^ | ( { MatLabStateflowFlag != 0 }?=> CNOT^ )  )? atom_expr
	;

exponent_unary_expr_with_end
	:	(  ( NOT | PLUS | MINUS )^ | ( { MatLabStateflowFlag != 0 }?=> CNOT^ )  )? atom_expr_with_end
	;

atom_expr
	:	INTEGER
	|	DOUBLE
	|	IMAGINARY
	|	TEXT
	|	reference
	|	matrix
	|	paren_expr
	|	cell_expr
	;

atom_expr_with_end
	:	INTEGER
	|	DOUBLE
	|	IMAGINARY
	|	TEXT
	|	reference
	|	matrix_with_end
	|	paren_expr_with_end
	|	cell_expr
	|	END
	;

paren_expr
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	:	LPAREN^ expr RPAREN!
	;

paren_expr_with_end
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	:	LPAREN^ expr_with_end RPAREN!
	;

cell_expr
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	:	LBRACE^ ( expr (COMMA! expr)* )? RBRACE!
	;

reference
	:       reference_item ( DOT^ reference_item )*
	;

reference_item
	:	{
//		  (
//                   $Matrix::scope == NONE ||
//		   input.LA(-1) != INTEGER && input.LA(-1) != DOUBLE ||
//		   (  (CommonToken)( (CommonTokenStream)input ).LT(-1)  ).getStopIndex() + 1 !=
//		   (  (CommonToken)( (CommonTokenStream)input ).LT(1)  ).getStartIndex()
//                 )
//                )
		 (
           $Matrix::scope == NONE ||
		   LA( -1 ) != INTEGER && LA( -1 ) != DOUBLE ||
		   LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 != LT( 1 )->getStartIndex(  LT( 1 )  )
         )
		}?=>
		IDENTIFIER^ paren_arglist
        ;

paren_arglist
options {
  k=3;
}
scope Matrix;
@init {
  $Matrix::scope = NONE;
}
	://	{
//		  (
//                   !( $Matrix::scope == MATRIXDEF ) ||
//		    (  (CommonToken)( (CommonTokenStream)input ).LT(-1)  ).getStopIndex() + 1 ==
//		    (  (CommonToken)( (CommonTokenStream)input ).LT(1)  ).getStartIndex()
//                  )
//		  (
//            !( $Matrix::scope == MATRIXDEF ) ||
//		    LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  )
//          )
//		}?=>
		cellref_list* argument_list?
	|	c_arglist
	;

cellref_list
	:   {
	      (
            !( $Matrix::scope == MATRIXDEF ) ||
		    LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  )
          )
		}?=>
		LBRACE^ (  arg ( COMMA! arg )*  )? RBRACE!
	;

argument_list
	:   {
	      (
            !( $Matrix::scope == MATRIXDEF ) ||
		    LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  )
          )
		}?=>
		LPAREN^ (  arg ( COMMA! arg )*  )? RPAREN!
	;

c_arglist
	:	{
		  (
		    MatLabStateflowFlag == 1 &&
			(
			  !( $Matrix::scope == MATRIXDEF ) ||
			  LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  )
			)
		  )
		}?=> ( LBRACK arg RBRACK ) c_arglist_remainder? -> ^(LPAREN arg c_arglist_remainder?)
	;

c_arglist_remainder
	:	{
		  (
		    MatLabStateflowFlag == 1 &&
			(
			  !( $Matrix::scope == MATRIXDEF ) ||
			  LT( -1 )->getStopIndex(  LT( -1 )  ) + 1 == LT( 1 )->getStartIndex(  LT( 1 )  )
			)
		  )
		}?=> ( LBRACK! arg RBRACK! ) c_arglist_remainder?
	;

arg
	:	COLON
	|	expr_with_end
	;

matrix
scope Matrix;
@init {
  $Matrix::scope = MATRIXDEF;
}
	:	LBRACK rows RBRACK -> ^( MATRIX rows )
	|	LBRACK RBRACK -> ^(MATRIX ^(ROW ZERO) )
	;

matrix_with_end
scope Matrix;
@init {
  $Matrix::scope = MATRIXDEF;
}
	:	LBRACK rows_with_end RBRACK -> ^( MATRIX rows_with_end )
	|	LBRACK RBRACK -> ^(MATRIX ^(ROW ZERO) )
	;

rows
	:	row (  ( SEMI | line )+ row )* ( SEMI | line )* -> row+
	;

rows_with_end
	:	row_with_end (  ( SEMI | line )+ row_with_end )* ( SEMI | line )* -> row_with_end+
	;

row	:	( expr COMMA? )+ -> ^( ROW expr+ )
	;

row_with_end
	:	( expr_with_end COMMA? )+ -> ^( ROW expr_with_end+ )
	;

s_assignee_matrix
scope Matrix;
@init {
  $Matrix::scope = LD;
}
	:	LBRACK! reference RBRACK!
	;

m_assignee_matrix
scope Matrix;
@init {
  $Matrix::scope = LD;
}
	:
		LBRACK r1=reference COMMA? ( r2+=reference COMMA? )+ RBRACK -> ^( MATRIX $r1 $r2+ )
	;

reference_list
	:	reference ( COMMA! reference )*
	;

for_command
	:	FOR^ for_cmd_list END!
	;

for_cmd_list
	:	IDENTIFIER ASSIGN! colon_expr delimited_input
	;

if_command
	:	IF^ if_cmd_list END!
	;

if_cmd_list
	:	pe=expr pc=delimited_input
		 ( ELSEIF ee+=expr ec+=delimited_input )* ( ELSE dc=delimited_input )?
		-> ^( IF $pe $pc ) ^( IF $ee $ec )* ^( ELSE $dc )?
        ;

global_command
	:	GLOBAL^ global_decl_list
	;

global_decl_list
	:	IDENTIFIER+
	;

while_command
	:	WHILE^ while_cmd_list END!
	;

while_cmd_list
	:	expr delimited_input
	;

return_command
	:	RETURN
	;

switch_command
	:	SWITCH^ expr delimiter? case_list END!
	;

case_list
	:	(  CASE ee+=expr delimiter ec+=case_action  )*
		(  OTHERWISE delimiter dc+=case_action  )?
		-> ^( CASE ^(EXPR $ee) $ec )* ^( OTHERWISE $dc )?
	;

case_action
	:	( statement delimiter )+	
	;

try_catch_command
	:	TRY t=delimited_input (CATCH (empty_lines|c=delimited_input?))? END -> ^( TRY ^(TRY $t) ^(CATCH $c?)? )
	;

delimited_input
	:	( delimiter )? ( statement delimiter )+
	;

functionMFile
	:	empty_lines? f_def_line f_body (END delimiter?)?  -> ^( FUNCTION f_def_line f_body )
	;

f_def_line
	:	FUNCTION f_output ASSIGN IDENTIFIER f_input -> ^(FNAME IDENTIFIER) f_input f_output
	|	FUNCTION IDENTIFIER f_input                 -> ^(FNAME IDENTIFIER) f_input
	;

f_output:	IDENTIFIER                    -> ^( FOUTPUT IDENTIFIER )
	|	LBRACK f_argument_list RBRACK -> ^( FOUTPUT f_argument_list )
	;

f_input	:	( LPAREN f_argument_list? RPAREN )? -> ^( FINPUT f_argument_list )?
	;

f_argument_list
	:	IDENTIFIER ( COMMA! IDENTIFIER )*
	;

f_body	:	delimiter statement_list
	|	delimiter?
	;

number	:	INTEGER
	|	DOUBLE
	;
