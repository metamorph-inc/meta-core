//################################################################################################
//
// Decorator exception classs
//	DecoratorExceptions.h
//
//################################################################################################

#include "StdAfx.h"
#include "DecoratorExceptions.h"
#include <stdio.h>


//################################################################################################
//
// CLASS : DecoratorException
//
//################################################################################################

DecoratorException::DecoratorException():
	ecode(SDecoratorOK)
{
}

DecoratorException::DecoratorException(const DecoratorException &e)
{
	ecode = e.ecode;
}

DecoratorException::DecoratorException(DecoratorExceptionCode ec)
{
	ecode = ec;
}

DecoratorException &DecoratorException::operator=(const DecoratorException &e)
{
	ecode = e.ecode;
	return *this;
}

DecoratorException &DecoratorException::operator=(DecoratorExceptionCode ec)
{
	ecode = ec;
	return *this;
}

DecoratorException::~DecoratorException()
{
}

const char *DecoratorException::what() const
{
	static char message[80];
	sprintf(message, "Exception code (HRESULT = 0x%08lx) exception", GetHResult());
	return message;
}

HRESULT DecoratorException::GetHResult(void) const
{
	return (HRESULT)ecode;
}


//################################################################################################
//
// CLASS : NotImplementedException
//
//################################################################################################

NotImplementedException::NotImplementedException()
{
	ecode = EDecoratorNotImplemented;
}

NotImplementedException::~NotImplementedException()
{
}


//################################################################################################
//
// CLASS : UnknownParameterException
//
//################################################################################################

UnknownParameterException::UnknownParameterException()
{
	ecode = EDecoratorUnknownParameter;
}

UnknownParameterException::~UnknownParameterException()
{
}


//################################################################################################
//
// CLASS : PortNotFoundException
//
//################################################################################################

PortNotFoundException::PortNotFoundException()
{
	ecode = EDecoratorPortNotFound;
}

PortNotFoundException::~PortNotFoundException()
{
}


//################################################################################################
//
// CLASS : DecoratorEventHandledException
//
//################################################################################################

DecoratorEventHandledException::DecoratorEventHandledException()
{
	ecode = SDecoratorEventHandled;
}

DecoratorEventHandledException::~DecoratorEventHandledException()
{
}
