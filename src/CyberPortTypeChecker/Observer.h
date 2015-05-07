#pragma once
#include "InterpreterMsg.h"

class Observer
{
public:
	Observer(void);
	~Observer(void);
	virtual void observe(IMSG* pIMsg) = 0;
};

