grammar sched;

options {
	language=C;
}


@members {

#ifndef __cdecl
#define __cdecl
#endif

	extern int __cdecl handleResolution( pANTLR3_STRING timeval,
					     pANTLR3_STRING timeunits );

	extern int __cdecl handleNewProc( pANTLR3_STRING procname,
					  pANTLR3_STRING freqval,
					  pANTLR3_STRING frequnits,
					  pANTLR3_STRING sndohd,
					  pANTLR3_STRING sndohdunits,
					  pANTLR3_STRING rcvohd,
					  pANTLR3_STRING rcvohdunits );

	extern int __cdecl handleNewTask( pANTLR3_STRING procname,
					  pANTLR3_STRING taskname,
					  pANTLR3_STRING frqspec,
					  pANTLR3_STRING freq,
					  pANTLR3_STRING frequnits,
					  pANTLR3_STRING dur,
					  pANTLR3_STRING durunits );

    extern int __cdecl handleTaskOffset( pANTLR3_STRING procname,
					  pANTLR3_STRING taskname,
					  pANTLR3_STRING offtime,
					  pANTLR3_STRING offunits );

	extern int __cdecl handleNewProcMsg( pANTLR3_STRING procname,
					     pANTLR3_STRING msgname,
				  	     pANTLR3_STRING msgsize,
					     pANTLR3_STRING sizeunits,
					     pANTLR3_STRING sndtask );

	extern int __cdecl handleProcMsgRcvr( pANTLR3_STRING procname,
					      pANTLR3_STRING msgname,
					      pANTLR3_STRING rcvtask );

	extern int __cdecl handleNewBus( pANTLR3_STRING busname,
					 pANTLR3_STRING datarate,
					 pANTLR3_STRING rateunits,
					 pANTLR3_STRING timespec,
					 pANTLR3_STRING timeunits );

	extern int __cdecl handleBusProc( pANTLR3_STRING busname,
					  pANTLR3_STRING procname );

	extern int __cdecl handleNewMsg( pANTLR3_STRING busname,
					 pANTLR3_STRING msgname,
				  	 pANTLR3_STRING msgsize,
					 pANTLR3_STRING sizeunits,
					 pANTLR3_STRING sndtask );

	extern int __cdecl handleMsgRcvr( pANTLR3_STRING busname,
					  pANTLR3_STRING msgname,
					  pANTLR3_STRING rcvtask );

	extern int __cdecl handleMsgOffset( pANTLR3_STRING procname,
					  pANTLR3_STRING taskname,
					  pANTLR3_STRING offtime,
					  pANTLR3_STRING offunits );

	extern int __cdecl handleNewSyncMsg( pANTLR3_STRING busname,
					     pANTLR3_STRING msgname,
				  	     pANTLR3_STRING synctime,
					     pANTLR3_STRING timeunits,
					     pANTLR3_STRING msgposmin,
					     pANTLR3_STRING timeunitsmin );

    extern int __cdecl handleSyncMsgMax( pANTLR3_STRING busname,
					                     pANTLR3_STRING msgname,
					                     pANTLR3_STRING msgposmax,
					                     pANTLR3_STRING timeunitsmax  );

	extern int __cdecl handleLatency( pANTLR3_STRING timespec,
					  pANTLR3_STRING timeunits,
					  pANTLR3_STRING sndtask,
					  pANTLR3_STRING rcvtask );
}


/* -----------------------------------------------------------------------
 * Parser Rules
 * -----------------------------------------------------------------------*/


specs 	:  resDesc ( procDesc | busDesc | latencyDesc )* ;

resDesc :  'Resolution' rv=NUM ru=TIMEUNITS
		{ handleResolution($rv.text, $ru.text); } ;

procDesc
	:  'Proc' pn=IDENT fv=NUM fq=FREQUNITS t1v=NUM t1u=TIMEUNITS t2v=NUM t2u=TIMEUNITS
		{ handleNewProc($pn.text, $fv.text, $fq.text, $t1v.text, $t1u.text, $t2v.text, $t2u.text); }
		(COMPDECL tn=IDENT tfq=FREQQUAL? fv=NUM fq=FREQUNITS tv=NUM tu=TIMEUNITS 
			{ handleNewTask($pn.text, $tn.text, $tfq.text, $fv.text, $fq.text, $tv.text, $tu.text); } 
			(OFFSETDECL ov=NUM ou=TIMEUNITS { handleTaskOffset($pn.text, $tn.text, $ov.text, $ou.text); } )? )*
		(MSGDECL mn=IDENT sv=NUM su=SIZEUNITS st=taskref
			{ handleNewProcMsg( $pn.text, $mn.text, $sv.text, $su.text, $st.text ); }
		(rt=taskref { handleProcMsgRcvr($pn.text, $mn.text, $rt.text); } )+ )*;

busDesc :  'Bus' bn=IDENT rv=NUM ru=RATEUNITS tv=NUM tu=TIMEUNITS
		{ handleNewBus($bn.text, $rv.text, $ru.text, $tv.text, $tu.text); } ( pn=IDENT { handleBusProc($bn.text, $pn.text); } )*
		(SYNCDECL mn=IDENT tv=NUM tu=TIMEUNITS mpmn=NUM tumn=TIMEUNITS
			{ handleNewSyncMsg( $bn.text, $mn.text, $tv.text, $tu.text, $mpmn.text, $tumn.text ); }
			( mpmx=NUM tumx=TIMEUNITS  { handleSyncMsgMax( $bn.text, $mn.text, $mpmx.text, $tumx.text ); } )? )*
		(MSGDECL mn=IDENT sv=NUM su=SIZEUNITS st=taskref
			{ handleNewMsg( $bn.text, $mn.text, $sv.text, $su.text, $st.text ); }
		(rt=taskref { handleMsgRcvr($bn.text, $mn.text, $rt.text); } )+ 
			(OFFSETDECL ov=NUM ou=TIMEUNITS { handleMsgOffset( $bn.text, $mn.text, $ov.text, $ou.text); } )? )*;

taskref :  IDENT '/' IDENT;

latencyDesc
	:  'Latency' tv=NUM tu=TIMEUNITS st=taskref rt=taskref
			{ handleLatency($tv.text, $tu.text, $st.text, $rt.text); };


/* -----------------------------------------------------------------------
 * Lexer Rules
 * -----------------------------------------------------------------------*/


WS	:	(' '|'\t'|'\n'|'\r')+ {$channel=HIDDEN;} ;

COMMENT	: 	'%'  .* '\n' {$channel=HIDDEN;} ;

fragment
DIGIT
	:	'0'..'9';

fragment
LETTER
	:	'a'..'z'|'A'..'Z';

COMPDECL:	'Comp' | 'CompGrp' ;

MSGDECL	:	'Msg' | 'MsgGrp' ;

SYNCDECL :	'Sync';

OFFSETDECL : 'Offset';

FREQUNITS:	('G'|'M'|'k')? 'Hz';

TIMEUNITS:	('m'|'u'|'n'|'p'|'f')? 's';

RATEUNITS:	('G'|'M'|'k')? 'b';

SIZEUNITS:	('G'|'M'|'k')? 'B';

IDENT 	:	LETTER ( LETTER | '_' | '.' | DIGIT )+;

NUM 	:	('0'..'9')+ ('.' ('0'..'9')+)?;

FREQQUAL:	'=' | '<';

