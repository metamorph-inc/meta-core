#include "IndexKeeper.h"

// Define the singleton here

IndexKeeper * IndexKeeper::_inst = 0;

IndexKeeper * IndexKeeper::inst()
{
	if ( _inst == 0 )
	{
		_inst = new IndexKeeper();  // We don't catch the exception, since we're dead if this fails.
	}
	
	return _inst;
}