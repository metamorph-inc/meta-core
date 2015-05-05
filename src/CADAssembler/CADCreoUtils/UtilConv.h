#ifndef UTIL_CONV_H
#define UTIL_CONV_H


/*--------------------------------------------------------------------*\
Pro/Toolkit includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProMdl.h>
#include <ProModelitem.h>
#include <ProAxis.h>
#include <ProCsys.h>
#include <ProSurface.h>
#include <ProPoint.h>
#include <ProEdge.h>
#include <ProCurve.h>
#include <ProFeature.h>
#include <ProQuilt.h>

/*--------------------------------------------------------------------*\
Pro/Develop includes
\*--------------------------------------------------------------------*/
#include <prodevelop.h>
#include <select3d.h>
#include <prorefer.h>
#include <pro_feattype.h>

#include <string.h>

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/
typedef struct {
    char suff_type[10];
    ProType pt_type;
} TableItem;

static TableItem ConvTable[] = {
{ "PRT", PRO_PART },
{ "ASM", PRO_ASSEMBLY}}; 

#define CONV_TABLE_ITEMS 2

typedef struct {
    ProType obj_type;
    int     sel_type;
} ConvItem;

static ConvItem TypeTable[] = {
    { PRO_ASSEMBLY, SEL_3D_PART },
    { PRO_PART,     SEL_3D_PART },
    { PRO_FEATURE,  SEL_3D_FEAT },
    { PRO_SURFACE,  SEL_3D_SRF },
    { PRO_EDGE,     SEL_3D_EDG },
    { PRO_EDGE_START,  SEL_3D_VERT },
    { PRO_EDGE_END,  SEL_3D_VERT },
    { PRO_AXIS,     SEL_3D_AXIS },
    { PRO_CSYS,     SEL_3D_CSYS },
    { PRO_CURVE,    SEL_3D_CURVE },
    { PRO_CRV_START, SEL_CURVE_END },
    { PRO_CRV_END,  SEL_CURVE_END },
    { PRO_POINT,    SEL_3D_PNT }};

#define TYPE_TABLE_ITEMS 13

#define PRO_TK_CONV_ERROR -21
#define PRO_TK_UNKNOWN_TYPE -22

ProError ProUtilMdlToModel (
    ProMdl 	*tk_obj, 
    Prohandle   *pdev_obj);

ProError ProUtilModelToMdl (
    Prohandle   pdev_obj,
    ProMdl      *tk_obj);

ProError ProUtilSelectionToSel3d ( 
    ProSelection *pt_sel,
    Select3d     *pdev_sel);

ProError ProUtilSel3dToSelection (
    Select3d     *pdev_sel,
    ProSelection *pt_sel);

#endif
