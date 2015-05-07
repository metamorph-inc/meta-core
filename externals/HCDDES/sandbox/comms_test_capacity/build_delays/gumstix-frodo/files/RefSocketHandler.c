/*** Included Header Files ***/
#include "RefSocketHandler.h"
#include <stdlib.h>

/*** Init Function Definition ***/
void RefSocketHandler_init( RefSocketHandler_context* RefSocketHandler_ctxt ) {
	// Nothing to do for now...
}


/*** Main Function Definition ***/
void RefSocketHandler_main(
	RefSocketHandler_context* RefSocketHandler_ctxt,
	float raw_pos_ref[4],
	float *(pos_ref[4]) ) {
	memcpy( *pos_ref, raw_pos_ref, sizeof(float[4]) );
}

