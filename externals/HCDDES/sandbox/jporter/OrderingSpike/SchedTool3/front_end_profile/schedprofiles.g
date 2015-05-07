grammar schedprofiles;

options {
	language=C;
}


@members {

#ifndef __cdecl
#define __cdecl
#endif

	extern int __cdecl setGlobals( pANTLR3_STRING hv, 
								   pANTLR3_STRING hu,
								   pANTLR3_STRING rv, 
								   pANTLR3_STRING ru );

	extern int __cdecl newNodeTTaskProfile( pANTLR3_STRING  nodename, 
											pANTLR3_STRING  typestr, 
											pANTLR3_STRING profname );
	extern int __cdecl addTTaskExtensibility( pANTLR3_STRING  profname, 
											  pANTLR3_STRING  extval );
	extern int __cdecl addTaskInterval( pANTLR3_STRING  taskname, 
										pANTLR3_STRING  start, 
										pANTLR3_STRING  end );
	extern int __cdecl addTTaskProfileIFace( pANTLR3_STRING  nodename, 
											 pANTLR3_STRING  typestr, 
											 pANTLR3_STRING  profname, 
											 pANTLR3_STRING  inodename, 
											 pANTLR3_STRING  itypestr, 
											 pANTLR3_STRING  iprofname );
	extern int __cdecl addTTaskIFaceParams( pANTLR3_STRING  profname, 
										    pANTLR3_STRING  ipd, 
										    pANTLR3_STRING  idead );
	extern int __cdecl addTTaskIFaceInterval( pANTLR3_STRING  taskname, 
											  pANTLR3_STRING  start, 
											  pANTLR3_STRING  end );
	
	extern int __cdecl newNodePTaskProfile( pANTLR3_STRING  nodename, 
											pANTLR3_STRING  typestr, 
											pANTLR3_STRING  profname );
	extern int __cdecl addPTaskExtensibility( pANTLR3_STRING  profname, 
											  pANTLR3_STRING  pdval, 
											  pANTLR3_STRING  capval );
	extern int __cdecl addPTaskParams( pANTLR3_STRING  taskname, 
									   pANTLR3_STRING  pd, 
									   pANTLR3_STRING  dead );
	extern int __cdecl addPTaskProfileIFace( pANTLR3_STRING  nodename, 
											 pANTLR3_STRING  typestr, 
											 pANTLR3_STRING  profname, 
											 pANTLR3_STRING  inodename, 
											 pANTLR3_STRING  itypestr, 
											 pANTLR3_STRING  iprofname );
	extern int __cdecl addPTaskIFaceParams( pANTLR3_STRING  profname, 
											pANTLR3_STRING  ipd, 
											pANTLR3_STRING  idead );
	
	extern int __cdecl newNodeATaskProfile( pANTLR3_STRING  nodename, 
											pANTLR3_STRING  typestr, 
											pANTLR3_STRING  profname );
	extern int __cdecl addATaskExtensibility( pANTLR3_STRING  profname, 
											  pANTLR3_STRING  pdval, 
											  pANTLR3_STRING  capval );
	extern int __cdecl addATaskParams( pANTLR3_STRING  taskname, 
									   pANTLR3_STRING  pd, 
									   pANTLR3_STRING  dead );
	extern int __cdecl addATaskProfileIFace( pANTLR3_STRING  nodename, 
											 pANTLR3_STRING  typestr, 
											 pANTLR3_STRING  profname, 
											 pANTLR3_STRING  inodename, 
											 pANTLR3_STRING  itypestr, 
											 pANTLR3_STRING  iprofname, 
											 pANTLR3_STRING  ipd, 
											 pANTLR3_STRING  idead );
	extern int __cdecl addATaskIFaceParams( pANTLR3_STRING  profname, 
											pANTLR3_STRING  ipd, 
											pANTLR3_STRING  idead );
	
	extern int __cdecl newBusProfile( pANTLR3_STRING  busname, 
									  pANTLR3_STRING  typestr, 
									  pANTLR3_STRING  profname );
	extern int __cdecl addBusExtensibility( pANTLR3_STRING  profname, 
											pANTLR3_STRING  extval );
	extern int __cdecl addMsg( pANTLR3_STRING  msgname, 
							   pANTLR3_STRING  dur );
	extern int __cdecl addMsgWindowInterval( pANTLR3_STRING  msgname, 
											 pANTLR3_STRING  start, 
											 pANTLR3_STRING  end );
	
}


/* -----------------------------------------------------------------------
 * Parser Rules
 * -----------------------------------------------------------------------*/


spec 	:  globalDesc ( procDesc )+ ( busDesc )*;

globalDesc
	:  HPDECL hv=NUM hu=TIMEUNITS  
	   RESDECL rv=NUM ru=TIMEUNITS { setGlobals( $hv.text, $hu.text, $rv.text, $ru.text ); };

procDesc:  NPDECL nodename=IDENT IDENTSEP 
		 (( typename=TTASKDECL IDENTSEP profname=IDENT
			{ newNodeTTaskProfile( $nodename.text, $typename.text, $profname.text ); }
		      ( EXTDECL extval=NUM { addTTaskExtensibility( $profname.text, $extval.text ); } )?
		      ( taskname=IDENT ( '(' start=NUM ',' end=NUM ')' { addTaskInterval( $taskname.text, $start.text, $end.text ); } )* )*
	      	      ( IFACEDECL inodename=IDENT IDENTSEP (itypename=PTASKDECL | itypename=ATASKDECL) IDENTSEP iprofname=IDENT 
		      	{ addTTaskProfileIFace( $nodename.text, $typename.text, $profname.text, $inodename.text, $itypename.text, $iprofname.text ); } 
		      	( ipd=NUM idead=NUM { addTTaskIFaceParams( $profname.text, $ipd.text, $idead.text ); }
		      	( '(' start=NUM ',' end=NUM ')' { addTTaskIFaceInterval( $taskname.text, $start.text, $end.text ); } )* )?
		      )*
	      	)
	      | ( typename=PTASKDECL IDENTSEP profname=IDENT
			{ newNodePTaskProfile( $nodename.text, $typename.text, $profname.text ); }
		      ( EXTDECL ( '(' pdval=NUM ',' capval=NUM ')' { addPTaskExtensibility( $profname.text, $pdval.text, $capval.text ); } )+ )?
		      ( taskname=IDENT  pd=NUM dead=NUM { addPTaskParams( $taskname.text, $pd.text, $dead.text ); } )*
		      ( IFACEDECL inodename=IDENT IDENTSEP (itypename=PTASKDECL | itypename=ATASKDECL) IDENTSEP iprofname=IDENT 
		      	{ addPTaskProfileIFace( $nodename.text, $typename.text, $profname.text, $inodename.text, $itypename.text, $iprofname.text ); } 
		      	( ipd=NUM idead=NUM { addPTaskIFaceParams( $profname.text, $ipd.text, $idead.text ); } )?
	              )*
	      	)
	      | ( typename=ATASKDECL IDENTSEP profname=IDENT
			{ newNodeATaskProfile( $nodename.text, $typename.text, $profname.text ); }
		      ( EXTDECL ( '(' pdval=NUM ',' capval=NUM ')' { addATaskExtensibility( $profname.text, $pdval.text, $capval.text ); } )+ )?
		      ( taskname=IDENT pd=NUM dead=NUM { addATaskParams( $taskname.text, $pd.text, $dead.text ); } )*
		      ( IFACEDECL inodename=IDENT IDENTSEP (itypename=ATASKDECL) IDENTSEP iprofname=IDENT 
		      	{ addATaskProfileIFace( $nodename.text, $typename.text, $profname.text, $inodename.text, $itypename.text, $iprofname.text, $ipd.text, $idead.text ); } 
		      	( ipd=NUM idead=NUM { addATaskIFaceParams( $profname.text, $ipd.text, $idead.text ); } )?
	              )*
	      	) );
	    
busDesc	:  BPDECL busname=IDENT IDENTSEP (typename=MSGDECL | typename=BMSGDECL) IDENTSEP profname=IDENT 
		{ newBusProfile( $busname.text, $typename.text, $profname.text ); }
	      ( EXTDECL extval=NUM { addBusExtensibility( $profname.text, $extval.text ); } )?
	      ( msgname=IDENT dur=NUM { addMsg( $msgname.text, $dur.text ); }
	         ( '(' start=NUM ',' end=NUM ')' { addMsgWindowInterval( $msgname.text, $start.text, $end.text ); } )* )* ;
		
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

HPDECL	:	'Hyperperiod' ;
NPDECL	:	'NProfile' ;
BPDECL	:	'BProfile' ;

EXTDECL	:	'Extensibility' ;
IFACEDECL:	'IFace' ;

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

IDENTSEP:	'/';