/*** Included Header Files ***/
#include "RefSocketHandler.h"


/*** Init Function Definition ***/
void RefSocketHandler_init( RefSocketHandler_context* RefSocketHandler_ctxt ) {
	// Nothing to do for now...
}


/*** Main Function Definition ***/
void RefSocketHandler_main(
	RefSocketHandler_context* RefSocketHandler_ctxt,
	float[4] raw_pos_ref,
	float[4] *pos_ref ) {
	memcpy( &pos_ref, &raw_pos_ref, sizeof(float[4]) );
}

