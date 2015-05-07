/*** Included Header Files ***/
#include "DataHandling.h"


/*** Init Function Definition ***/
void DataHandling_init( DataHandling_context* DataHandling_ctxt ) {
	// Nothing to do for now...
}


/*** Main Function Definition ***/
void DataHandling_main(
	DataHandling_context* DataHandling_ctxt,
	unsigned short pos_ref,
	float pos_data,
	float angle_data,
	float *pos_data_out,
	float *angle_data_out,
	float *pos_ref_out,
	float *pos_data_out2 ) {
	*pos_ref_out = 5.0 * (((float)pos_ref)/1024.0);  *pos_data_out = pos_data; *pos_data_out2 = pos_data; *angle_data_out = angle_data;
}

