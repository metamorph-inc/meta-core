/*** Included Header Files ***/
#include "IndexKeeper.h"


/*** Static Member Initialization ***/
IndexKeeper* IndexKeeper::_inst = NULL;


/***************************************************************************************/


IndexKeeper* IndexKeeper::Inst() {
	if ( IndexKeeper::_inst == NULL ) {
		// We don't catch the exception, since we're dead if this fails.
		IndexKeeper::_inst = new IndexKeeper();
	}
	return IndexKeeper::_inst;
}


/***************************************************************************************/

