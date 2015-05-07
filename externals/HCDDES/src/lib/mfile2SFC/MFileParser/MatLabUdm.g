tree grammar MatLabUdm;

options {
//  language=C++;
  language=C;
  tokenVocab=MatLab;
  output=AST;
  ASTLabelType=pANTLR_COMMON_TREE;
}

scope Block {
  SFC::CompoundStatement _outerBlock;
  SFC::CompoundStatement _innerBlock;
}

@headers {
#include "SFC.h"
#include "RuleReturn.hpp"
}

@members {
  SFC::Project _project;
}

input[ SFC::CompoundStatement block ]
scope Block;
@init {
  $Block::_innerBlock = block;
  if ( block.parent() != Udm::null ) $Block::_outerBlock = SFC::CompoundStatement::Cast( block.parent() );
}
	:	scriptMFile
	|	functionMFile
	;

scriptMFile
	:	statement_list?
	;

statement_list
	:	statement+
	;

statement
// ASSIGNMENTS MOVED UP TO INCORPORATE SYNTACTIC PREDICATES
	:	^(ASSIGN r=reference[ true ] e=expr)
		{
			if ( $r.referenceReturn.getDT() != Udm::null ) {
				$r.referenceReturn.combine( "=", $e.exprReturn ).collapse( block );
			} else {
				$e.exprReturn.collapseWithVar( $r.referenceReturn.getLocalVar() );
			}
		}
	|	^(ASSIGN m_assignee_matrix reference[ false ])

//	|	command_form
	|	expr
	|	for_command
	|	if_command
	|	global_command
	|	while_command
	|	return_command
	|	switch_command
	;

command_form
	:	^(IDENTIFIER text_list)
	;

text_list
	:	TEXT+
	;

expr returns [ExprReturn exprReturn]
	:	^( op=(AND|OR|LAND|LOR|GTHAN|GTHANE|EQUAL|LTHAN|LTHANE) e11=expr e12=expr )
		{
		  $exprReturn = $e11.exprReturn.combine( $op.getText(), $e12.exprReturn );
		}
	|	^( op=(PLUS|MINUS) e21=expr e22=expr? )
		{
		  Exprs exprs = UE( $op.getText(), $e21.exprReturn.getExprs() );
		  $exprReturn = ExprReturn( exprs, $e21.exprReturn.getExprsProxyVector(), $e21.exprReturn.dt() );
		}
	|	^(UNEQUAL e31=expr e32=expr)
		{
		  $exprReturn = $e31.exprReturn.combine( "!=", $e32.exprReturn );
		}
	|	^( op=(EMUL|EDIV) e41=expr e42=expr)
		{
		  std::string opText = $op.getText();
		  opText = opText.substr(1);
		  $exprReturn = $e41.exprReturn.combine( $op.getText(), $e42.exprReturn );
		}
	|	^(ELEFTDIV   e51=expr e52=expr)
		{
		  $exprReturn = $e52.exprReturn.combine( "/", $e51.exprReturn );
		}
	|	^(EPOWER     e61=expr e62=expr)
		{
		  $exprReturn = $e61.exprReturn.combine( $op.getText(), $e62.exprReturn, true );
		}
	|	^( NOT e71=expr )
		{
		  Exprs exprs = UE( "!", $e71.exprReturn.getExprs() );
		  $exprReturn = ExprReturn( exprs, $e71.exprReturn.getExprsProxyVector(), $e71.exprReturn.dt() );
		}
	|	^(MUL	e81=expr e82=expr)
		{
		  $exprReturn = $e81.exprReturn.multiply( $e82.exprReturn );
		}
	|	^(TRANSPOSE  e91=expr)
		{
		  $exprReturn = $e91.exprReturn.transpose();
		}
	|	^(DIV	expr expr)
	|	^(LEFTDIV    expr expr)
	|	^(POWER      expr expr)
	|	^(CTRANSPOSE expr expr)
	|	colon_expr
	|	INTEGER
		{
		  $exprReturn = ExprReturn(   IE(  atoi( INTEGER.getText() )  )   );
		}
	|	DOUBLE
		{
		  $exprReturn = ExprReturn(   DE(  atof( DOUBLE.getText() )  )   );
		}
	|	IMAGINARY
	|	TEXT
	|	r=reference[ false ]
		{
		  $exprReturn = $r.referenceReturn;
		}
	|	matrix
	;

colon_expr returns [ColonReturn colonReturn]
	:	^(COLON e11=expr e12=expr e13=expr?) { $colonReturn = ColonReturn( $e11.exprReturn, $e12.exprReturn ); }
	;

reference[ bool lvalue ] returns [ ReferenceReturn referenceReturn ]
	:	r11=reference_item[ SFC::BasicType(), $lvalue ]
		{ $referenceReturn = $r11.referenceReturn; }

	|	^( DOT
		   r21=reference[ $lvalue ]
		   r22=reference_item[ $r21.referenceReturn.getBaseDT(), $lvalue ]
		 )
		 { $referenceReturn = $r21.referenceReturn.dotCombine( $r22.referenceReturn ); }
	;


reference_item[ SFC::DT dt, bool lvalue ] returns [ ReferenceReturn referenceReturn ]
	:	^( id=IDENTIFIER^ a+=arg* )
		{
		  return ReferenceReturn::makeReferenceReturn(
		   $Block::_innerBlock, $id, $dt, $a, $lvalue,
		  );
		}
	;

arg returns [ ArgReturn argReturn ]
	:	COLON
	|	expr { $argReturn = $expr.exprReturn; }
	;

matrix  :	^( MATRIX row+ )
	;

row	:	^( ROW expr+ )
	;

m_assignee_matrix
	:	^( MATRIX reference[ true ]+ )
	;

for_command
	:	^(FOR for_cmd_list)
	;

for_cmd_list
	:	IDENTIFIER expr statement_list
	;

if_command
scope Block;
@init {
  SFC::CompoundStatement outerBlock = $Block[ $Block.size() - 2 ]::_innerBlock;
  SFC::ConditionalGroup conditionalGroup = SFC::ConditionalGroup::Create( outerBlock );
  numberStatement( outerBlock, conditionalGroup );
  $Block::_innerBlock = SFC::ConditionalBlock::Create( conditionalGroup );
  numberStatement( conditionalGroup, $Block::_innerBlock );
}
	:	^(IF if_cmd_list)
	;

if_cmd_list
	:	e1=expr s1=statement_list
		{
		  ExprReturn exprReturn = $e1.exprReturn.collapseWithVarLogic();
		  exprReturn.buildUdm( $Block::_innerBlock );
		}
		(
		  ^( ELSEIF
		    {
		      $Block::_innerBlock = SFC::ConditionalBlock::Create( conditionalGroup );
		      numberStatement( conditionalGroup, $Block::_innerBlock );
		    }
		    e2=expr s2=statement_list
		    {
		      ExprReturn exprReturn = $e2.exprReturn.collapseWithVarLogic();
		      exprReturn.buildUdm( $Block::_innerBlock );
		    }
		  )
		)*
		(
		 ^( ELSE
		     {
		       $Block::_innerBlock = SFC::ConditionalBlock::Create( conditionalGroup );
		       numberStatement( conditionalGroup, $Block::_innerBlock );
		       Exprs exprs = IE( 1 );
		       exprs.buildUdm( $Block::_innerBlock );
		     }
		     s3+=statement_list
		  )
        )?
	;

global_command
	:	^(GLOBAL global_decl_list)
	;

global_decl_list
	:	IDENTIFIER+
	;

while_command
@init {
  SFC::CompoundStatement outerBlock = $Block[ $Block.size() - 2 ]::_innerBlock;
  $Block::_innerBlock = SFC::IterativeBlock::Create( outerBlock );
  numberStatement( outerBlock, $Block::_innerBlock );
}
	:	^( WHILE while_cmd_list )
	;

while_cmd_list
	:	e1=expr statement_list
		{
		  ExprReturn exprReturn = $e1.exprReturn.collapseWithVarLogic();
		  exprReturn.buildUdm( $Block::_innerBlock );
		}
	;

return_command
	:	RETURN
	;

switch_command
	:	^(SWITCH expr case_list)
	;

case_list
	:	(  ^( CASE expr statement+ )  )* (  ^( OTHERWISE statement+ )  )?
	;

functionMFile
scope Block;
@init {
  $Block::_outerBlock = $Block::_innerBlock = Udm::Function::Create( $Block[ $Block.size() - 2 ]::block );
}
	:	^( FUNCTION f_def_line f_body )
	;

f_def_line
	:	IDENTIFIER f_input? f_output?
	;

f_output:	^( FOUTPUT IDENTIFIER* )
	;

f_input	:	^( FINPUT IDENTIFIER* )
	;

f_body	:	statement_list
	;
