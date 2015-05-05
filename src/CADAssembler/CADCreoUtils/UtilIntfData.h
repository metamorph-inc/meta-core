

#ifndef UTILINTFDATA_H
#define UTILINTFDATA_H
#include <ProToolkit.h>
#include <ProSurfacedata.h>
#include <ProEdgedata.h>
#include <ProQuiltdata.h>
#include <ProDatumdata.h>
#include <ProIntfData.h>
#include <ProUtil.h>

/*----------------------------------------------------------*\
    Function prototypes
\*----------------------------------------------------------*/

/*=========================================================================*\
    Function:	ProUtilCurvedataPrint()
    Purpose:	Prints curve data to file
\*=========================================================================*/
ProError ProUtilCurvedataPrint(FILE *file, char *str, ProCurvedata *);

/*=========================================================================*\
    Function:	ProUtilSurfacedataPrint()
    Purpose:	Prints surface data to file
\*=========================================================================*/
ProError ProUtilSurfacedataPrint(FILE *file, char *str, ProSurfacedata *);

/*=========================================================================*\
    Function:	ProUtilIntfDataPrint()
    Purpose:	Prints intf data to file
\*=========================================================================*/
ProError ProUtilIntfDataPrint(FILE *file, ProIntfData *);
#endif
