//################################################################################################
//
// Decorator exception classs
//	DecoratorExceptions.h
//
//################################################################################################

#ifndef __DECORATOREXCEPTIONS_H_
#define __DECORATOREXCEPTIONS_H_


#include "StdAfx.h"
#include <GmeApi.h>
#include <exception>


enum DecoratorExceptionCode {
	EDecoratorUnknownParameter		= E_DECORATOR_UNKNOWN_PARAMETER,
	EDecoratorInitWithNull			= E_DECORATOR_INIT_WITH_NULL,
	EDecoratorUninitialized			= E_DECORATOR_UNINITIALIZED,
	EDecoratorLocationIsNotSet		= E_DECORATOR_LOCISNOTSET,
	EDecoratorPortNotFound			= E_DECORATOR_PORTNOTFOUND,
	EDecoratorUsingDeprecatedFunc	= E_DECORATOR_USING_DEPRECATED_FUNCTION,
	EDecoratorNotImplemented		= E_DECORATOR_NOT_IMPLEMENTED,
	SDecoratorEventHandled			= S_DECORATOR_EVENT_HANDLED,
	SDecoratorOK					= S_OK,
	EFail							= E_FAIL
};


//################################################################################################
//
// CLASS : DecoratorException
//
//################################################################################################

class DecoratorException: public std::exception
{
public:
	DecoratorException() throw();
	DecoratorException(const DecoratorException &e) throw();
	DecoratorException(DecoratorExceptionCode ec) throw();
	DecoratorException &operator=(const DecoratorException &e) throw();
	DecoratorException &operator=(DecoratorExceptionCode ec) throw();
	~DecoratorException() throw();
	virtual const char* what(void) const throw();	// std exception

	HRESULT GetHResult(void) const throw();

protected:
	DecoratorExceptionCode ecode;
};


//################################################################################################
//
// CLASS : NotImplementedException
//
//################################################################################################

class NotImplementedException: public DecoratorException
{
public:
	NotImplementedException() throw();
	~NotImplementedException() throw();
};


//################################################################################################
//
// CLASS : UnknownParameterException
//
//################################################################################################

class UnknownParameterException: public DecoratorException
{
public:
	UnknownParameterException() throw();
	~UnknownParameterException() throw();
};


//################################################################################################
//
// CLASS : PortNotFoundException
//
//################################################################################################

class PortNotFoundException: public DecoratorException
{
public:
	PortNotFoundException() throw();
	~PortNotFoundException() throw();
};


//################################################################################################
//
// CLASS : DecoratorEventHandledException
//
//################################################################################################

class DecoratorEventHandledException: public DecoratorException
{
public:
	DecoratorEventHandledException() throw();
	~DecoratorEventHandledException() throw();
};


#endif //__DECORATOREXCEPTIONS_H_
