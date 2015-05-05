
#ifndef PROUTILCOLLECTDTMPNTS
#define PROUTILCOLLECTDTMPNTS
/*====================================================================*\
    FUNCTION :	ProUtilCollectSolidDtmPnts()
    PURPOSE  :	Collect solid's datum points\. The solid can be part or 
		assembly
\*====================================================================*/
ProError ProUtilCollectSolidDtmPnts(
    ProSolid solid,	    /* In : solid */
    ProSelection **pp_sel,  /* Out: ProArray of Selection. The function 
				    allocates memory for this argument, but 
				    you must free it. To free the memory, 
				    call the function 
				    ProUtilCollectionArrayFree() */
    int *p_n_pnts);	    /* Out: The number of points in the array */ 

/*====================================================================*\
    FUNCTION :	ProUtilSelectionArrayFree()
    PURPOSE  :	Free ProArray of ProSelection allocated by vollection 
\*====================================================================*/
ProError ProUtilSelectionArrayFree(
    ProSelection **pp_sel);  /* In:ProArray  to free memory */

#endif
