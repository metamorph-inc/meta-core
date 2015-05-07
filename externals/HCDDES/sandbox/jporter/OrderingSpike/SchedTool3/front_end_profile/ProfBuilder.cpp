#include "ProfBuilder.h"
#include "ParseUtils.h"

#include<exception>

using namespace std;

ProfBuilder * _current_pfb;

ProfBuilder::ProfBuilder(void)
{
}

ProfBuilder::~ProfBuilder(void)
{
}

void ProfBuilder::SetCurrentProfBuilder( ProfBuilder * pfb )
{
	_current_pfb = pfb;
}

void ProfBuilder::setGlobals( double hypval, char * hyptimeunits, double resval, char * restimeunits )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::newNodeTTaskProfile( char * nodename, char * typestr , char * profname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addTTaskExtensibility( char * profname, long long extval )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addTaskInterval( char * taskname, long long start, long long end )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addTTaskProfileIFace( char * nodename, char * typestr, char * profname, char * inodename, char * itypestr, char * iprofname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addTTaskIFaceParams( char * profname, long long iperiod, long long ideadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addTTaskIFaceInterval( char * taskname, long long start_tick, long long end_tick )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::newNodePTaskProfile( char * nodename, char * typestr , char * profname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addPTaskExtensibility( char * profname, double period, double capacity )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addPTaskParams( char * taskname, long long period, long long deadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addPTaskProfileIFace( char * nodename, char * typestr, char * profname, char * inodename, char * itypestr, char * iprofname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addPTaskIFaceParams( char * profname, long long iperiod, long long ideadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::newNodeATaskProfile( char * nodename, char * typestr, char * profname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addATaskExtensibility( char * profname, double period, double capacity )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addATaskParams( char * taskname, long long period, long long deadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addATaskProfileIFace( char * nodename, char * typestr, char * profname, char * inodename, char * itypestr, char * iprofname, long long iperiod, long long ideadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addATaskIFaceParams( char * profname, long long iperiod, long long ideadline )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::newBusProfile( char * busname, char * typestr, char * profname )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addBusExtensibility( char * profname, long long extval )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addMsg( char * msgname, long long durationticks )
{
	throw std::exception("The method or operation is not implemented.");
}

void ProfBuilder::addMsgWindowInterval( char * msgname, long long start_tick, long long end_tick )
{
	throw std::exception("The method or operation is not implemented.");
}

#ifndef __cdecl
#define __cdecl
#endif

extern "C" {

#include <antlr3.h>

int __cdecl setGlobals( pANTLR3_STRING hv, 
							pANTLR3_STRING hu,
							pANTLR3_STRING rv, 
							pANTLR3_STRING ru)
{
	_current_pfb->setGlobals( 
		to_double( (char *) hv->to8(hv)->chars ), 
		(char *) hu->to8(hu)->chars,
		to_double( (char *) rv->to8(rv)->chars ), 
		(char *) ru->to8(ru)->chars);
	return 0;
}

int __cdecl newNodeTTaskProfile( pANTLR3_STRING  nodename, 
								pANTLR3_STRING  typestr, 
								pANTLR3_STRING profname )
{
	_current_pfb->newNodeTTaskProfile( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars,
		(char *) profname->to8(profname)->chars );
	return 0;
}

int __cdecl addTTaskExtensibility( pANTLR3_STRING  profname, 
		pANTLR3_STRING  extval )
{
	_current_pfb->addTTaskExtensibility( 
		(char *) profname->to8(profname)->chars, 
		to_integer( (char *) extval->to8(extval)->chars ) );
	return 0;
}


int __cdecl addTaskInterval( pANTLR3_STRING  taskname, 
		pANTLR3_STRING  start, 
		pANTLR3_STRING  end )
{
	_current_pfb->addTaskInterval( 
		(char *) taskname->to8(taskname)->chars, 
		to_integer( (char *) start->to8(start)->chars ), 
		to_integer( (char *) end->to8(end)->chars ) );
	return 0;
}


int __cdecl addTTaskProfileIFace( pANTLR3_STRING  nodename, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname, 
		pANTLR3_STRING  inodename, 
		pANTLR3_STRING  itypestr, 
		pANTLR3_STRING  iprofname )
{
	_current_pfb->addTTaskProfileIFace( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars, 
		(char *) inodename->to8(inodename)->chars, 
		(char *) itypestr->to8(itypestr)->chars, 
		(char *) iprofname->to8(iprofname)->chars );
	return 0;
}


int __cdecl addTTaskIFaceParams( pANTLR3_STRING  profname, 
		pANTLR3_STRING  ipd, 
		pANTLR3_STRING  idead )
{
	_current_pfb->addTTaskIFaceParams( 
		(char *) profname->to8(profname)->chars, 
		to_integer( (char *) ipd->to8(ipd)->chars ), 
		to_integer( (char *) idead->to8(idead)->chars ) );
	return 0;
}


int __cdecl addTTaskIFaceInterval( pANTLR3_STRING  taskname, 
		pANTLR3_STRING  start, 
		pANTLR3_STRING  end )
{
	_current_pfb->addTTaskIFaceInterval( 
		(char *) taskname->to8(taskname)->chars, 
		to_integer( (char *) start->to8(start)->chars ), 
		to_integer( (char *) end->to8(end)->chars ) );
	return 0;
}



int __cdecl newNodePTaskProfile( pANTLR3_STRING  nodename, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname )
{
	_current_pfb->newNodePTaskProfile( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars );
	return 0;
}


int __cdecl addPTaskExtensibility( pANTLR3_STRING  profname, 
		pANTLR3_STRING  pdval, 
		pANTLR3_STRING  capval )
{
	_current_pfb->addPTaskExtensibility( 
		(char *) profname->to8(profname)->chars, 
		to_double( (char *) pdval->to8(pdval)->chars ), 
		to_double( (char *) capval->to8(capval)->chars ) );
	return 0;
}


int __cdecl addPTaskParams( pANTLR3_STRING  taskname, 
		pANTLR3_STRING  pd, 
		pANTLR3_STRING  dead )
{
	_current_pfb->addPTaskParams( 
		(char *) taskname->to8(taskname)->chars, 
		to_integer( (char *) pd->to8(pd)->chars ), 
		to_integer( (char *) dead->to8(dead)->chars ) );
	return 0;
}


int __cdecl addPTaskProfileIFace( pANTLR3_STRING  nodename, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname, 
		pANTLR3_STRING  inodename, 
		pANTLR3_STRING  itypestr, 
		pANTLR3_STRING  iprofname )
{
	_current_pfb->addPTaskProfileIFace( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars, 
		(char *) inodename->to8(inodename)->chars, 
		(char *) itypestr->to8(itypestr)->chars, 
		(char *) iprofname->to8(iprofname)->chars );
	return 0;
}


int __cdecl addPTaskIFaceParams( pANTLR3_STRING  profname, 
		pANTLR3_STRING  ipd, 
		pANTLR3_STRING  idead )
{
	_current_pfb->addPTaskIFaceParams( 
		(char *) profname->to8(profname)->chars, 
		to_integer( (char *) ipd->to8(ipd)->chars ), 
		to_integer( (char *) idead->to8(idead)->chars ) );
	return 0;
}



int __cdecl newNodeATaskProfile( pANTLR3_STRING  nodename, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname )
{
	_current_pfb->newNodeATaskProfile( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars );
	return 0;
}


int __cdecl addATaskExtensibility( pANTLR3_STRING  profname, 
		pANTLR3_STRING  pdval, 
		pANTLR3_STRING  capval )
{
	_current_pfb->addATaskExtensibility( 
		(char *) profname->to8(profname)->chars, 
		to_double( (char *) pdval->to8(pdval)->chars ), 
		to_double( (char *) capval->to8(capval)->chars ) );
	return 0;
}


int __cdecl addATaskParams( pANTLR3_STRING  taskname, 
		pANTLR3_STRING  pd, 
		pANTLR3_STRING  dead )
{
	_current_pfb->addATaskParams( 
		(char *) taskname->to8(taskname)->chars, 
		to_integer( (char *) pd->to8(pd)->chars ), 
		to_integer( (char *) dead->to8(dead)->chars ) );
	return 0;
}


int __cdecl addATaskProfileIFace( pANTLR3_STRING  nodename, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname, 
		pANTLR3_STRING  inodename, 
		pANTLR3_STRING  itypestr, 
		pANTLR3_STRING  iprofname, 
		pANTLR3_STRING  ipd, 
		pANTLR3_STRING  idead )
{
	_current_pfb->addATaskProfileIFace( 
		(char *) nodename->to8(nodename)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars, 
		(char *) inodename->to8(inodename)->chars, 
		(char *) itypestr->to8(itypestr)->chars, 
		(char *) iprofname->to8(iprofname)->chars, 
		to_integer( (char *) ipd->to8(ipd)->chars ), 
		to_integer( (char *) idead->to8(idead)->chars ) );
	return 0;
}


int __cdecl addATaskIFaceParams( pANTLR3_STRING  profname, 
		pANTLR3_STRING  ipd, 
		pANTLR3_STRING  idead )
{
	_current_pfb->addATaskIFaceParams( 
		(char *) profname->to8(profname)->chars, 
		to_integer( (char *) ipd->to8(ipd)->chars ), 
		to_integer( (char *) idead->to8(idead)->chars ) );
	return 0;
}



int __cdecl newBusProfile( pANTLR3_STRING  busname, 
		pANTLR3_STRING  typestr, 
		pANTLR3_STRING  profname )
{
	_current_pfb->newBusProfile( 
		(char *) busname->to8(busname)->chars, 
		(char *) typestr->to8(typestr)->chars, 
		(char *) profname->to8(profname)->chars );
	return 0;
}


int __cdecl addBusExtensibility( pANTLR3_STRING  profname, 
		pANTLR3_STRING  extval )
{
	_current_pfb->addBusExtensibility( 
		(char *) profname->to8(profname)->chars, 
		to_integer( (char *) extval->to8(extval)->chars ) );
	return 0;
}


int __cdecl addMsg( pANTLR3_STRING  msgname, 
		pANTLR3_STRING  dur )
{
	_current_pfb->addMsg( 
		(char *) msgname->to8(msgname)->chars, 
		to_integer( (char *) dur->to8(dur)->chars ) );
	return 0;
}


int __cdecl addMsgWindowInterval( pANTLR3_STRING  msgname, 
		pANTLR3_STRING  start, 
		pANTLR3_STRING  end )
{
	_current_pfb->addMsgWindowInterval( 
		(char *) msgname->to8(msgname)->chars, 
		to_integer( (char *) start->to8(start)->chars ), 
		to_integer( (char *) end->to8(end)->chars ) );
	return 0;
}

}
