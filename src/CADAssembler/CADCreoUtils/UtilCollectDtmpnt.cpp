
#include <ProToolkit.h>
#include <ProAsmcomp.h>
#include <ProAsmcomppath.h>
#include <ProFeatType.h>
#include <ProMdl.h>
#include <ProSolid.h>
#include <ProSelection.h>

#include "UtilMath.h"
#include "UtilMatrix.h"
#include "UtilCollect.h"


/*====================================================================*\
    FUNCTION :	ProUtilCollectOneLevelAsmcomppath()
    PURPOSE  :	Collect Asmcomppathes on one level only.
		Util function to be call from ProUtilCollectAsmcomppath
    NOTE     :  Do not call it directly
\*====================================================================*/
static ProError ProUtilCollectOneLevelAsmcomppath(
    ProAssembly assy,
    ProAsmcomppath *p_path,
    int level,
    ProAsmcomppath **pp_path)
{
    ProError err;
    ProAsmcomp *p_comps;
    int i, n_comps;
    ProMdl mdl;
    ProMdlType mdl_type;

    err = ProUtilCollectAsmcomp(assy, &p_comps);
    if (err != PRO_TK_NO_ERROR)
	return (err);

    err = ProArraySizeGet((ProArray)p_comps, &n_comps);
    for (i=0; i<n_comps; i++)
    {
	/* Fill full Asmcomppath structure for component */ 
	p_path->comp_id_table[level] = p_comps[i].id;
	p_path->comp_id_table[level + 1] = -1;
	p_path->table_num = level + 1;

	/* Add Asmcomppath to the array */
	ProArrayObjectAdd((ProArray*)pp_path, PRO_VALUE_UNUSED, 1, p_path);

	/* Check if components is assembly for recursive checking */
	err = ProAsmcompMdlGet(p_comps+i, &mdl);
	err = ProMdlTypeGet(mdl, &mdl_type);
	if (mdl_type == PRO_MDL_ASSEMBLY)
	{
	    err = ProUtilCollectOneLevelAsmcomppath((ProAssembly)mdl, p_path, 
		level+1, pp_path);
	}
    }
    err = ProArrayFree((ProArray*)&p_comps);
    return (PRO_TK_NO_ERROR);
}

/*====================================================================*\
    FUNCTION :	ProUtilCollectAsmcomppath()
    PURPOSE  :	Collect asmcomppathes for all levels of assembly,
		including parts and subassemblies
\*====================================================================*/
ProError ProUtilCollectAsmcomppath(
    ProAssembly assy,		/* In : Assembly */
    ProAsmcomppath **pp_path,	/* Out: The ProArray of collected pathes.
					The function allocates memory for 
					this argument, but you must free it. 
					To free the memory, call the function 
					ProArrayFree() */
    int *p_n_pathes)		/* Out: The number of pathes in the array */
{
    ProError err;
    ProAsmcomppath path;
    int n_pathes;
    
    if((assy == (ProAssembly)NULL) || pp_path == NULL )
	return (PRO_TK_BAD_INPUTS);

    err = ProArrayAlloc(0, sizeof(ProAsmcomppath), 10, (ProArray*)pp_path);
    path.owner = (ProSolid)assy;
    
    err = ProUtilCollectOneLevelAsmcomppath(assy, &path, 0, pp_path);
    err = ProArraySizeGet((ProArray)*pp_path, &n_pathes);
    if (n_pathes<=0)
    {	
	err = ProArrayFree((ProArray*)pp_path);
	return (PRO_TK_E_NOT_FOUND);
    }

    p_n_pathes[0] = n_pathes;
    return (PRO_TK_NO_ERROR);
}


/*====================================================================*\
    FUNCTION :	ProUtilCollectOneLevelDtmPnts()
    PURPOSE  :	Collect Datum points in top level solid only.
		Util function to be call from ProUtilCollectSolidDtmPnts
    NOTE     :  Do not call it directly
\*====================================================================*/
static ProError ProUtilCollectOneLevelDtmPnts(
    ProSolid solid,
    ProAsmcomppath *p_path,
    ProSelection    **p_sel)
{
    ProError err;
    ProFeature *p_features;
    ProModelitem *p_points;
    int i, j, n_points, n_feats;
    ProSelection sel;

    err = ProUtilCollectSolidFeaturesWithFilter (solid, 
        (ProFeatureFilterAction)ProUtilCollectDtmPointFeatFilter, &p_features);

    if (err != PRO_TK_NO_ERROR)
	return (err);
    err = ProArraySizeGet((ProArray)p_features, &n_feats);

    for (i=0; i<n_feats; i++)
    {
	err = ProUtilCollectFeatureGeomitems(p_features + i, PRO_POINT,
	    (ProGeomitem**)&p_points);
	if (err != PRO_TK_NO_ERROR)
	    continue;
	err = ProArraySizeGet((ProArray)p_points, &n_points);
	for (j=0; j<n_points; j++)
	{
	    err = ProSelectionAlloc(p_path, p_points+j, &sel);
	    err = ProArrayObjectAdd((ProArray*)p_sel, PRO_VALUE_UNUSED, 1,
		&sel); 
	}
	err = ProArrayFree((ProArray*)&p_points);
    }
    err = ProArrayFree((ProArray*)&p_features);
    return (PRO_TK_NO_ERROR);
}

/*====================================================================*\
    FUNCTION :  ProUtilCollectSolidDtmPnts()
    PURPOSE  :  Collect solid's  datum points. The solid can be part or
                assembly. In case of assebly all level points will be
                collected
\*====================================================================*/
ProError ProUtilCollectSolidDtmPnts(
    ProSolid solid,	    /* In : solid */
    ProSelection **pp_sel,  /* Out: ProArray of Selection. The function 
				    allocates memory for this argument, but 
				    you must free it. To free the memory, 
				    call the function 
				    ProUtilCollectionArrayFree() */
    int *p_n_pnts)	    /* Out: The number of points in the array */ 
{
    ProError err;
    ProMdlType mdl_type;
    ProAsmcomppath *p_pathes;
    int i, n_pathes, n_sel;

    if ((solid == (ProSolid)NULL) || (pp_sel == NULL))
	return (PRO_TK_BAD_INPUTS);

    err = ProArrayAlloc(0, sizeof(ProSelection), 10, (ProArray*)pp_sel);

    /* First collect Datum points in the model */
    err = ProUtilCollectOneLevelDtmPnts(solid, NULL, pp_sel);

    /* Check the model type */
    err = ProMdlTypeGet((ProMdl)solid, &mdl_type);
    if (mdl_type == PRO_MDL_ASSEMBLY)
    {
	err = ProUtilCollectAsmcomppath((ProAssembly)solid, &p_pathes,
	    &n_pathes);
	if (err == PRO_TK_NO_ERROR)
	{
	    for (i=0; i<n_pathes; i++)
	    {
		err = ProAsmcomppathMdlGet(p_pathes + i, (ProMdl*)&solid);
		err = ProUtilCollectOneLevelDtmPnts(solid, p_pathes+i, pp_sel);
	    }
	    err = ProArrayFree((ProArray*)&p_pathes);
	}
    }
    err = ProArraySizeGet((ProArray)*pp_sel, &n_sel);
    if (n_sel <=0)
    {
	err  = ProArrayFree((ProArray*)pp_sel);
	return (PRO_TK_E_NOT_FOUND);
    }
    
    p_n_pnts[0] = n_sel;
    return (PRO_TK_NO_ERROR);
}


/*====================================================================*\
    FUNCTION :	ProUtilSelectionArrayFree()
    PURPOSE  :	Free ProArray of ProSelection allocated by collection 
\*====================================================================*/
ProError ProUtilSelectionArrayFree(
    ProSelection **pp_sel)  /* In:ProArray  to free memory */
{
    ProError err;
    int i, n_sel;

    err = ProArraySizeGet((ProArray)*pp_sel, &n_sel);
    for (i=0; i<n_sel; i++)
	err = ProSelectionFree(pp_sel[0]+i);
    err = ProArrayFree((ProArray*)pp_sel);
    pp_sel[0] = (ProSelection*)NULL;
    return (PRO_TK_NO_ERROR);
}
