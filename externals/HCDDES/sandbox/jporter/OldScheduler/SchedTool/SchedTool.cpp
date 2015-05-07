// SchedTool.cpp : Scheduler language lexer / parser invocation


#include "ConfigKeeper.h"
#include <iostream>
#include "schedAdapter.h"
#include "schedModel.h"


extern "C" {

#include <antlr3.h>

int __cdecl handleResolution( pANTLR3_STRING timeval, pANTLR3_STRING timeunits )
{
	SchedAdapter::instance()->setResolution( to_double((char *) timeval->to8(timeval)->chars),
											 (char *) timeunits->to8( timeunits )->chars );

	return 0;
}

int __cdecl handleNewProc( pANTLR3_STRING procname, 
						   pANTLR3_STRING freqval, pANTLR3_STRING frequnits, 
						   pANTLR3_STRING sndohd, pANTLR3_STRING sndohdunits, 
						   pANTLR3_STRING rcvohd, pANTLR3_STRING rcvohdunits  )
{
	SchedAdapter::instance()->AddNewProc( (char *) procname->to8(procname)->chars,
										  to_double( (char *) freqval->to8(freqval)->chars ),
										  (char *) frequnits->to8(frequnits)->chars,
										  to_double( (char *) sndohd->to8(sndohd)->chars),
										  (char *) sndohdunits->to8(sndohdunits)->chars,
										  to_double( (char *) rcvohd->to8(rcvohd)->chars),
										  (char *) rcvohdunits->to8(rcvohdunits)->chars );
	return 0;
}

int __cdecl handleNewTask( pANTLR3_STRING procname, pANTLR3_STRING taskname, 
						   pANTLR3_STRING frqspec, pANTLR3_STRING freq, pANTLR3_STRING frequnits, 
						   pANTLR3_STRING dur, pANTLR3_STRING durunits )
{
	SchedAdapter::instance()->AddNewTask( (char *) procname->to8(procname)->chars, 
										  (char *) taskname->to8(taskname)->chars, 
										  (char *) frqspec->to8(frqspec)->chars, 
										  to_double( (char *) freq->to8(freq)->chars),
										  (char *) frequnits->to8(frequnits)->chars,
										  to_double( (char *) dur->to8(dur)->chars),
										  (char *) durunits->to8(durunits)->chars );
	return 0;
}

int __cdecl handleNewBus( pANTLR3_STRING busname, 
						  pANTLR3_STRING datarate, pANTLR3_STRING rateunits, 
						  pANTLR3_STRING timespec, pANTLR3_STRING timeunits )
{
	SchedAdapter::instance()->AddNewBus( (char *) busname->to8(busname)->chars,
										 to_double( (char *) datarate->to8(datarate)->chars),
										 (char *) rateunits->to8(rateunits)->chars,
										 to_double( (char *) timespec->to8(timespec)->chars),
										 (char *) timeunits->to8(timeunits)->chars );
	return 0;
}

int __cdecl handleNewMsg( pANTLR3_STRING busname, pANTLR3_STRING msgname, 
						  pANTLR3_STRING msgsize, pANTLR3_STRING sizeunits, 
						  pANTLR3_STRING sndtask )
{
		SchedAdapter::instance()->AddNewMsg( (char *) busname->to8(busname)->chars,
											 (char *) msgname->to8(msgname)->chars,
											 to_double( (char *) msgsize->to8(msgsize)->chars),
											 (char *) sizeunits->to8(sizeunits)->chars,
											 (char *) sndtask->to8(sndtask)->chars );
	return 0;
}

int __cdecl handleMsgRcvr( pANTLR3_STRING busname, pANTLR3_STRING msgname, 
						   pANTLR3_STRING rcvtask )
{
	SchedAdapter::instance()->AddMsgRcvr( (char *) busname->to8(busname)->chars,
										  (char *) msgname->to8(msgname)->chars,
										  (char *) rcvtask->to8(rcvtask)->chars );
	return 0;
}

int __cdecl handleLatency( pANTLR3_STRING timespec, pANTLR3_STRING timeunits,
						   pANTLR3_STRING sndtask, pANTLR3_STRING rcvtask )
{
	SchedAdapter::instance()->AddLatency( to_double( (char *) timespec->to8(timespec)->chars),
										  (char *) timeunits->to8(timeunits)->chars,
										  (char *) sndtask->to8(sndtask)->chars,
										  (char *) rcvtask->to8(rcvtask)->chars );
	return 0;
}

#include "schedLexer.h"
#include "schedParser.h"
}

int main(int argc, char* argv[])
{
	ConfigKeeper * cfg = ConfigKeeper::inst();
	if (!cfg->processCmdLine(argc, argv))
	{
		std::cout << "Failed to process command line: " << std::endl;
		for (int lidx = 0; lidx < argc; lidx++ )
		{
			std::cout << argv[lidx] << std::endl;
		}
		std::cout << std::endl;
		return 1; // exit early
	}

	std::string SchedModelName( cfg->getSchedModel() );
	std::string fname(cfg->getInFileName());

	pANTLR3_INPUT_STREAM pIStream = antlr3AsciiFileStreamNew((pANTLR3_UINT8) fname.c_str() );
	// Make sure there is no error opening the file
	if ((pANTLR3_INPUT_STREAM)ANTLR3_ERR_NOFILE == pIStream)
	{
		std::cout << "File " << fname << " not found. " << std::endl;
		exit(-1);
	}

	// Make sure we have something to call back to before we start!
	if ( SchedAdapter::instance()->SetSchedModel( SchedModel::GetSchedModel( SchedModelName ) ) )
	{
		pschedLexer pLexer = schedLexerNew( pIStream );

		pANTLR3_COMMON_TOKEN_STREAM pTokens = antlr3CommonTokenStreamSourceNew( 0, pLexer->pLexer->tokSource );
		pschedParser pParser = schedParserNew( pTokens );

		pParser->specs(pParser);

		SchedAdapter::instance()->RunSchedModel();
	}
	else
	{
		std::cout << "Invalid scheduling model: " << SchedModelName << std::endl;
		exit(-2);
	}

	return 0;
}

