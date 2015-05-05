#ifndef TESTCOLLECT_H
#define TESTCOLLECT_H



/*---------------------------------------------------------------------------*\
    Headers
\*---------------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProEdge.h>
#include <ProTool.h>


/*---------------------------------------------------------------------------*\
    Macros
\*---------------------------------------------------------------------------*/
#define DAT_FILE_NAME	"visitcollect.dat"
#define FRAME_NUM	10

#ifdef PI
#undef PI
#endif

#define PI          3.14159265358979323846


/*---------------------------------------------------------------------------*\
    Data types
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
    Global variables
\*---------------------------------------------------------------------------*/


/*----------------------------------------------------------*\
    Function prototypes
\*----------------------------------------------------------*/
int ProTestCollection( ProAppData p_appdata, int int_dummy );
int ProTestCollectAnimation( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidCsys( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidAxis( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidQuilts( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidSurfaces( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidSimpreps( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidFeatures( ProAppData p_appdata, int int_dummy );
int ProTestCollectModelNotes( ProAppData p_appdata, int int_dummy );
int ProTestCollectMfgTools( ProAppData p_appdata, int int_dummy );
int ProTestCollectFeatureGeomitems( ProAppData p_appdata, int int_dummy );
int ProTestCollectElemtreeElements( ProAppData p_appdata, int int_dummy );
int ProTestCollectCurveComponents( ProAppData p_appdata, int int_dummy );
int ProTestCollectContours( ProAppData p_appdata, int int_dummy );
int ProTestCollectExtobj( ProAppData p_appdata, int int_dummy );
int ProTestFindByName(ProAppData p_appdata, int int_dummy );
int ProTestCollectDimensions( ProAppData p_appdata, int int_dummy );
int ProTestCollectSolidAxisByPlane( ProAppData p_appdata, int int_dummy);
void ProUtilXRot( double angle,	ProMatrix mx );

ProError ProUtilSelectedMdlGet( ProSelection selection, ProMdl *p_model );
ProError ProUtilSelectedSurfaceGet( ProSelection selection, 
    ProSurface *p_surface );
ProError ProUtilSelectedCurveGet( ProSelection selection, 
    ProCurve *p_curve );
char *ProUtilSurfaceTypeToString( ProSrftype surf_type );
char *ProUtilFeatureStatusToString( ProFeatStatus feat_type );
char *ProUtilGeomitemTypeToString( ProType item_type );
char *ProUtilValueToString( ProValue p_value );
char *ProUtilEdgeTypeToString( ProEnttype edge_ddtype );
char *ProUtilToolTypeToString( ProToolType tool_type );

#endif    /* TESTCOLLECT_H */
