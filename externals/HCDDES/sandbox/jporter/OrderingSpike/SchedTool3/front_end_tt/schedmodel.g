grammar schedmodel;

options {
	language=C;
}


@members {

#ifndef __cdecl
#define __cdecl
#endif

	extern int __cdecl setName( pANTLR3_STRING name );

	extern int __cdecl setResolution( pANTLR3_STRING timeval,
					     pANTLR3_STRING timeunits );

	extern int __cdecl newProc( pANTLR3_STRING procname,
				    pANTLR3_STRING tickresolutionval,
				    pANTLR3_STRING ticktimeunits );

	extern int __cdecl addSchedType( pANTLR3_STRING procname,
					 pANTLR3_STRING scheduletypestr );
	
	extern int __cdecl newTTask( pANTLR3_STRING procname,
				     pANTLR3_STRING taskname,
				     pANTLR3_STRING freqspecchar,
				     pANTLR3_STRING taskfreq,
				     pANTLR3_STRING frequnits,
				     pANTLR3_STRING taskdur,
				     pANTLR3_STRING durunits );

	extern int __cdecl newPTask( pANTLR3_STRING procname,
				     pANTLR3_STRING taskname,
				     pANTLR3_STRING freqspecchar,
				     pANTLR3_STRING taskfreq,
				     pANTLR3_STRING frequnits,
				     pANTLR3_STRING taskdur,
				     pANTLR3_STRING durunits,
				     pANTLR3_STRING taskdeadline,
				     pANTLR3_STRING deadlineunits );

	extern int __cdecl newATask( pANTLR3_STRING procname,
				     pANTLR3_STRING taskname,
				     pANTLR3_STRING freqspecchar,
				     pANTLR3_STRING taskfreq,
				     pANTLR3_STRING frequnits,
				     pANTLR3_STRING taskdur,
				     pANTLR3_STRING durunits,
				     pANTLR3_STRING taskdeadline,
				     pANTLR3_STRING deadlineunits );

	extern int __cdecl newSyncMsgRef( pANTLR3_STRING procname, 
						pANTLR3_STRING msgname );
	
	extern int __cdecl newProcMsg( pANTLR3_STRING procname,
				       pANTLR3_STRING msgname,
			  	       pANTLR3_STRING msgsize,
				       pANTLR3_STRING sizeunits,
				       pANTLR3_STRING sndtask );

	extern int __cdecl addProcMsgRcvr( pANTLR3_STRING procname,
				   	   pANTLR3_STRING msgname,
					   pANTLR3_STRING rcvtask );

	extern int __cdecl newTBus( pANTLR3_STRING busname,
				    pANTLR3_STRING datarate,
				    pANTLR3_STRING rateunits,
				    pANTLR3_STRING setuptime,
				    pANTLR3_STRING timeunits );
	
	extern int __cdecl newTTBus( pANTLR3_STRING busname,
				     pANTLR3_STRING datarate,
				     pANTLR3_STRING rateunits,
				     pANTLR3_STRING setuptime,
				     pANTLR3_STRING timeunits,
				     pANTLR3_STRING slottime,
				     pANTLR3_STRING stimeunits );
	
	extern int __cdecl newAFDXBus( pANTLR3_STRING busname,
				       pANTLR3_STRING datarate,
				       pANTLR3_STRING rateunits,
				       pANTLR3_STRING channelrate,
				       pANTLR3_STRING crateunits,
				       pANTLR3_STRING numchannels );
	
	extern int __cdecl newEthBus( pANTLR3_STRING busname,
				      pANTLR3_STRING datarate,
				      pANTLR3_STRING rateunits );
	
	extern int __cdecl newTTEBus( pANTLR3_STRING busname,
				      pANTLR3_STRING datarate,
				      pANTLR3_STRING rateunits,
				      pANTLR3_STRING setuptime,
				      pANTLR3_STRING timeunits,
				      pANTLR3_STRING slottime,
				      pANTLR3_STRING stimeunits );

	extern int __cdecl addBusProc( pANTLR3_STRING busname,
				       pANTLR3_STRING procname );

	extern int __cdecl newSyncMsg( pANTLR3_STRING busname,
				       pANTLR3_STRING msgname,
				       pANTLR3_STRING msgsize,
				       pANTLR3_STRING sizeunits ); 

	extern int __cdecl addSyncBounds( pANTLR3_STRING busname,
				          pANTLR3_STRING msgname,
				          pANTLR3_STRING msgposmin,
					  pANTLR3_STRING timeunitsmin,
					  pANTLR3_STRING msgposmax,
					  pANTLR3_STRING timeunitsmax );

	extern int __cdecl newMsg( pANTLR3_STRING busname,
				   pANTLR3_STRING msgname,
				   pANTLR3_STRING msgsize,
				   pANTLR3_STRING sizeunits,
				   pANTLR3_STRING sndtask );	

	extern int __cdecl addMsgRcvr( pANTLR3_STRING busname,
				       pANTLR3_STRING msgname,
				       pANTLR3_STRING rcvtask );

	extern int __cdecl newBMsg( pANTLR3_STRING busname,
				    pANTLR3_STRING msgname,
				    pANTLR3_STRING msgsize,
				    pANTLR3_STRING sizeunits,
				    pANTLR3_STRING msg2size,
				    pANTLR3_STRING sizeunits2,
				    pANTLR3_STRING deadlinetime,
				    pANTLR3_STRING timeunits,
				    pANTLR3_STRING sndtask,
				    pANTLR3_STRING rcvtask );

	extern int __cdecl newLatencyBound( pANTLR3_STRING timespec,
					    pANTLR3_STRING timeunits,
					    pANTLR3_STRING sndtask,
					    pANTLR3_STRING rcvtask );

	extern int __cdecl newLatencyChain( pANTLR3_STRING timespec,
					    pANTLR3_STRING timeunits );
	
	extern int __cdecl addFirstChainElement( pANTLR3_STRING start_task );
	extern int __cdecl addChainElement( pANTLR3_STRING intermed_elt );
	extern int __cdecl addLastChainElement( pANTLR3_STRING end_task );
}


/* -----------------------------------------------------------------------
 * Parser Rules
 * -----------------------------------------------------------------------*/


specs 	:  nameDesc resDesc ( procDesc )+ ( busDesc )* ( latencyDesc )* ;

nameDesc : PROBDECL nm=IDENT { setName( $nm.text ); } ;

resDesc :  RESDECL rv=NUM ru=TIMEUNITS
		{ setResolution($rv.text, $ru.text); } ;

procDesc
	:  PROCDECL pn=IDENT rv=NUM ru=TIMEUNITS 
		{ newProc($pn.text, $rv.text, $ru.text); } (sc=SCHEDTYPE  { addSchedType( $pn.text, $sc.text ); } )?
		(
		  (TTASKDECL tn=IDENT tfq=FREQQUAL? fv=NUM fq=FREQUNITS dv=NUM du=TIMEUNITS
			{ newTTask($pn.text, $tn.text, $tfq.text, $fv.text, $fq.text, $dv.text, $du.text); } )
		| (PTASKDECL tn=IDENT tfq=FREQQUAL? fv=NUM fq=FREQUNITS tv=NUM tu=TIMEUNITS dv=NUM du=TIMEUNITS
			{ newPTask($pn.text, $tn.text, $tfq.text, $fv.text, $fq.text, $tv.text, $tu.text, $dv.text, $du.text); } )
		| (ATASKDECL tn=IDENT tfq=FREQQUAL? fv=NUM fq=FREQUNITS tv=NUM tu=TIMEUNITS dv=NUM du=TIMEUNITS
			{ newATask($pn.text, $tn.text, $tfq.text, $fv.text, $fq.text, $tv.text, $tu.text, $dv.text, $du.text); } )
		)*
		(SYNCMSGDECL mn=IDENT { newSyncMsgRef( $pn.text, $mn.text ); } )?
		(MSGDECL mn=IDENT sv=NUM su=SIZEUNITS st=IDENT
			{ newProcMsg( $pn.text, $mn.text, $sv.text, $su.text, $st.text ); }
		(rt=IDENT { addProcMsgRcvr($pn.text, $mn.text, $rt.text); } )+ )*;

busDesc :  (( TBUSDECL bn=IDENT rv=NUM ru=RATEUNITS tv=NUM tu=TIMEUNITS 
		{ newTBus($bn.text, $rv.text, $ru.text, $tv.text, $tu.text); } )
	|  ( TTBUSDECL bn=IDENT rv=NUM ru=RATEUNITS sv=NUM su=TIMEUNITS tv=NUM tu=TIMEUNITS 
		{ newTTBus($bn.text, $rv.text, $ru.text, $sv.text, $su.text, $tv.text, $tu.text); } )
	|  ( AFDXBUSDECL bn=IDENT rv=NUM ru=RATEUNITS vv=NUM vu=RATEUNITS nc=NUM 
		{ newAFDXBus($bn.text, $rv.text, $ru.text, $vv.text, $vu.text, $nc.text ); } )
	|  ( ETHBUSDECL bn=IDENT rv=NUM ru=RATEUNITS 
		{ newEthBus( $bn.text, $rv.text, $ru.text ); } )
	|  ( TTEBUSDECL bn=IDENT rv=NUM ru=RATEUNITS sv=NUM su=TIMEUNITS tv=NUM tu=TIMEUNITS 
		{ newTTEBus($bn.text, $rv.text, $ru.text, $sv.text, $su.text, $tv.text, $tu.text); } )
		( pn=IDENT { addBusProc($bn.text, $pn.text); } )* )
	   ( SYNCMSGDECL mn=IDENT tv=NUM tu=SIZEUNITS
		{ newSyncMsg( $bn.text, $mn.text, $tv.text, $tu.text ); }
		( mpmn=NUM tumn=TIMEUNITS mpmx=NUM tumx=TIMEUNITS  { addSyncBounds( $bn.text, $mn.text, $mpmn.text, $tumn.text, $mpmx.text, $tumx.text ); } )? )*
	   ( MSGDECL mn=IDENT sv=NUM su=SIZEUNITS st=elementref
		{ newMsg( $bn.text, $mn.text, $sv.text, $su.text, $st.text ); }
		(rt=elementref { addMsgRcvr($bn.text, $mn.text, $rt.text); } )+ )*
	   ( BMSGDECL mn=IDENT sv=NUM su=SIZEUNITS rv=NUM ru=SIZEUNITS dv=NUM du=TIMEUNITS st=elementref et=elementref
		{ newBMsg( $bn.text, $mn.text, $sv.text, $su.text, $rv.text, $ru.text, $dv.text, $du.text, $st.text, $et.text ); } )*;

elementref :  IDENT '/' IDENT;

latencyDesc
	:  ( LATENCYDECL tv=NUM tu=TIMEUNITS st=elementref rt=elementref
			{ newLatencyBound($tv.text, $tu.text, $st.text, $rt.text); } )
	|  ( LATENCYCHAINDECL tv=NUM tu=TIMEUNITS { newLatencyChain( $tv.text, $tu.text ); }
		st=elementref { addFirstChainElement( $st.text ); }
		( ie=elementref { addChainElement( $ie.text ); } )*
		lt=elementref { addLastChainElement( $lt.text ); } );
	

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

PROBDECL:	'Problem' ;

RESDECL	:	'Resolution' ;
PROCDECL:	'Proc' ;

TTASKDECL:	'TTask' ;
PTASKDECL:	'PTask' ;
ATASKDECL:	'ATask' ;

SYNCMSGDECL:	'SyncMsg';
BMSGDECL:	'BMsg' ;
MSGDECL	:	'Msg';

TTBUSDECL:	'TTBus' ;
TBUSDECL:	'TBus' ;
AFDXBUSDECL:	'AFDXBus' ;
ETHBUSDECL:	'EthBus' ;
TTEBUSDECL:	'TTEBus' ;

LATENCYDECL:	'LatencyBound' ;
LATENCYCHAINDECL:	'LatencyChain' ;

FREQUNITS:	('G'|'M'|'k')? 'Hz';

TIMEUNITS:	('m'|'u'|'n'|'p'|'f')? 's';

RATEUNITS:	('G'|'M'|'k')? 'b';

SIZEUNITS:	('G'|'M'|'k')? 'B';

SCHEDTYPE:	'EDF' | 'RM' | 'DM' | 'Static' ;

IDENT 	:	LETTER ( LETTER | '_' | '.' | DIGIT )+;

NUM 	:	('0'..'9')+ ('.' ('0'..'9')+)?;

FREQQUAL:	'=' | '<';

