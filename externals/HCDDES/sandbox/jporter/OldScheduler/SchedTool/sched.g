grammar sched;

options {
	language=C;
}


/* -----------------------------------------------------------------------
 * Parser Rules
 * -----------------------------------------------------------------------*/


specs 	:	resDesc ( procDesc | busDesc | latencyDesc )* ;

resDesc :	'Resolution' rv=NUM ru=TIMEUNITS { handleResolution($rv.text, $ru.text); } ;

procDesc
	:	'Proc' pn=IDENT fv=NUM fq=FREQUNITS t1v=NUM t1u=TIMEUNITS t2v=NUM t2u=TIMEUNITS 
			{ handleNewProc($pn.text, $fv.text, $fq.text, $t1v.text, $t1u.text, $t2v.text, $t2u.text); }
		('Task' tn=IDENT tfq=FREQQUAL? fv=NUM fq=FREQUNITS tv=NUM tu=TIMEUNITS 
			{ handleNewTask($pn.text, $tn.text, $tfq.text, $fv.text, $fq.text, $tv.text, $tu.text); } )*;
		
busDesc :	'Bus' bn=IDENT rv=NUM ru=RATEUNITS tv=NUM tu=TIMEUNITS { handleNewBus($bn.text, $rv.text, $ru.text, $tv.text, $tu.text); }
		('Msg' mn=IDENT sv=NUM su=SIZEUNITS st=taskref 
			{ handleNewMsg( $bn.text, $mn.text, $sv.text, $su.text, $st.text ); } 
		(rt=taskref { handleMsgRcvr($bn.text, $mn.text, $rt.text); } )+ )*;
		
taskref :	IDENT '/' IDENT;

latencyDesc 
	:	'Latency' tv=NUM tu=TIMEUNITS st=taskref rt=taskref 
			{ handleLatency($tv.text, $tu.text, $st.text, $rt.text); };


/* -----------------------------------------------------------------------
 * Lexer Rules
 * -----------------------------------------------------------------------*/


WS	:	(' '|'\t'|'\n'|'\r')+ {$channel=HIDDEN;} ;

COMMENT	: 	'%'  .* '\n' {$channel=HIDDEN;} ;

fragment DIGIT
	:	'0'..'9';

fragment LETTER
	:	'a'..'z'|'A'..'Z';

FREQUNITS:	('G'|'M'|'k')? 'Hz';

TIMEUNITS:	('m'|'u'|'n'|'p'|'f')? 's';

RATEUNITS:	('G'|'M'|'k')? 'b';

SIZEUNITS:	('G'|'M'|'k')? 'B';

IDENT 	:	LETTER ( LETTER | '_' | DIGIT )+;

NUM 	:	('0'..'9')+ ('.' ('0'..'9')+)?;	

FREQQUAL:	'=' | '>';
