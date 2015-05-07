grammar Orario;

spec 	:  (procDesc | latencyDesc)*
    ;

procDesc 	:  'Proc' pn=ID fv=FLOAT fq=FREQUNITS t1v=FLOAT t1u=TIMEUNITS t2v=FLOAT t2u=TIMEUNITS
		(COMPDECL tn=ID fv=FLOAT fq=FREQUNITS tv=FLOAT tu=TIMEUNITS )*
		(MSGDECL mn=ID sv=INT su=SIZEUNITS st=taskref
		    (rt=taskref )+ )*
    ;

taskref :  ID '/' ID
    ;

latencyDesc
	:  'Latency' tv=FLOAT tu=TIMEUNITS st=taskref rt=taskref
    ;

FREQUNITS:	('G'|'M'|'k')? 'Hz';

TIMEUNITS:	('m'|'u'|'n'|'p'|'f')? 's';

RATEUNITS:	('G'|'M'|'k')? 'b';

SIZEUNITS:	('G'|'M'|'k')? 'B';

COMPDECL:	'Comp' | 'CompGrp' | 'Task' | 'TaskGrp' ;

MSGDECL :	'Msg' | 'MsgGrp' ;

SYNCDECL :	'Sync';

ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*;

INT :	'0'..'9'+;

FLOAT
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;};


fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

