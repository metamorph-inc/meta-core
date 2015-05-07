/*
 * Testing utility for the outer loop
 */


/*
 * Here's the procedure:
 *
 * 1. Open a data file (specified on stdin).
 * 2. Loop over all of the data values:
 * 3.    Read in a data value
 * 4.    Run all the sample through the outer loop.
 * 5.    Write the resulting data value, with the timestamp and original.
 *       (in the order timestamp, calculated, original - to match Simulink).
 */

#include <stdio.h>
#include <stdlib.h>

#include "OuterLoop_sl.h"

OuterLoop_context olctxt;

FILE *fp;

int main( int argc, char ** argv )
{
	FILE *fp_in, *fp_out;
	float input[2];
	float output[5];
	float result[3];
	char outfile[] = "rslt.dat";

	if ( argc < 2 )
	{
		printf( "Usage: ./TestOuterLoop <inputfile>\n" );
		exit(1);
	}

	OuterLoop_init( &olctxt );

	fp_in = fopen( argv[1], "r" );
	fp_out = fopen( outfile, "w" );
	
	while ( !feof(fp_in) && (fread( input, sizeof(float), 2, fp_in ) > 0) )
	{
		OuterLoop_main( &olctxt, 0, input[1], &output );
		result[0] = input[0];  /* copy time */
		result[1] = output[0]; /* calculated value */
		result[2] = output[2]; /* original input value */
		fwrite( result, sizeof( float ), 3, fp_out );
	}

	fclose( fp_in );
	fclose( fp_out );

	return 0;
}
