#include "TaskTypes.h"

#include <iostream>
#include <string>

using namespace std;


void PTask::PrintOtherParameters()
{
	cout << " deadline " << getDeadline();
}


void ATask::PrintOtherParameters()
{
	cout << " deadline " << getDeadline();
}

