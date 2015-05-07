#include "StdAfx.h"
#include "Notifier.h"

using namespace std;

Notifier::Notifier(void)
{
}


Notifier::~Notifier(void)
{
}

void Notifier::attachObserver(Observer *obs)
{
	pObserverSet.insert(obs);
}

void Notifier::attachObserverSet(set<Observer*> obsSet)
{
	pObserverSet.insert(obsSet.begin(), obsSet.end());
}

void Notifier::notify(IMSG* iMsg)
{
	for(set<Observer*>::iterator pObsIt = pObserverSet.begin(); pObsIt != pObserverSet.end(); pObsIt++)
	{
		(*pObsIt)->observe(iMsg);
	}
}
