#pragma once
#include "Observer.h"
#include <set>

#define PObserverSet		set<Observer*>				// Set of pointers to Observer
#define PObserverSetIter	set<Observer*>::iterator	// Iterator of Set of pointers to Observer

class Notifier
{
public:
	Notifier(void);
	~Notifier(void);
	void attachObserver(Observer *obs);
	void attachObserverSet(set<Observer*> obsSet);
protected:
	PObserverSet pObserverSet;
	void notify(IMSG* iMsg);
};

