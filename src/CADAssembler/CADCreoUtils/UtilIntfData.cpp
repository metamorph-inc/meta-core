

/*----------------------------------------------------------*\
    Headers
\*----------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProObjects.h>
#include <ProEdgedata.h>
#include <ProFeatType.h>
#include <ProFeature.h>
#include <ProIntfData.h>
#include <ProImportfeat.h>
#include <ProQuiltdata.h>
#include <ProSolid.h>
#include <ProSurfacedata.h>
#include <ProMenu.h>
#include <ProMessage.h>
#include <ProUtil.h>

#include "UtilMath.h"
#include "TestError.h"
#include "UtilString.h"
#include <ProTKRunTime.h>
/*----------------------------------------------------------*\
    Macros
\*----------------------------------------------------------*/
#define VECTOR(a) a[0], a[1], a[2]

/*=========================================================================*\
    Function:	ProUtilDoubleArrayPrint()
    Purpose:	Prints double array to file
\*=========================================================================*/

void ProUtilDoubleArrayPrint(
    FILE    *file,          /* In : file */
    char    *str,           /* In : the line prefix */
    double  *p_double,      /* In : the double array */
    int	    n_double)       /* In : the number of values in the array */
{
    int i;
    ProCharLine buf, line;

    ProUtilstrcpy(line, (const char *)str);
    for (i=0; i<n_double; i++)
    {
	/* This trick in sprintf is necessary because on some platforms
	   close-to-zero values could be printfed out as -0.000 */
	ProTKSprintf(buf, "%.5f", fabs(p_double[i])<EPSM6 ? 0.0 : p_double[i]);
	if (i <  n_double -1)
	    ProUtilstrcat(buf, ",");
	ProUtilstrcat(line, (const char *)buf);
	if (strlen(line) > 70 || (i == n_double -1))
	{
	    ProTKFprintf(file, "%s\n", line);
	    ProUtilstrcpy(line, (const char *)str);
	}
	else
	{
	    ProUtilstrcat(line, " ");
	}
    }
}

/*=========================================================================*\
    Function:	ProUtilCurvedataPrint()
    Purpose:	Prints curve data to file
\*=========================================================================*/
ProError ProUtilCurvedataPrint(
    FILE *file,
    char *str,
    ProCurvedata *p_curve_data)
{
    ProError err;
    ProEnttype c_type;
    ProCharLine next_line, next_line2;
    int                     i, n_point, n_knots, degree, n_comp;
    double                  *p_par_arr, *p_weight_arr, st_a, en_a, rad;
    double                  start_ang, end_ang;
    double                  d1, d2; 
    Pro3dPnt                p1, p2;
    ProVector               v1, v2;
    ProPoint3d              *p_pnt_arr, *p_tan_arr;
    ProComponentCurvedata   *p_comp_arr;
    ProCurvedata	    curve_data;
    ProCurveDir		    dir;

    ProUtilstrcpy(next_line, (const char *)str);
    ProUtilstrcat(next_line, "    ");
    ProUtilstrcpy(next_line2, (const char *)next_line);
    ProUtilstrcat(next_line2, "    ");

    err = ProCurvedataTypeGet(p_curve_data, &c_type);
    TEST_CALL_REPORT("ProCurvedataTypeGet()",
			"ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
    switch(c_type)
    {
    case PRO_ENT_POINT:
	err = ProPointdataGet(p_curve_data, p1);
    	TEST_CALL_REPORT("ProPointdataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file, "%sType Point\n", str);
	ProTKFprintf(file, "%spos ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(p1));
	break;
    case PRO_ENT_LINE:
	err = ProLinedataGet(p_curve_data, p1, p2);
    	TEST_CALL_REPORT("ProLinedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType Line \n", str);
	ProTKFprintf(file,"%send1 ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(p1));
	ProTKFprintf(file,"%send2 ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(p2));
	break;
    case PRO_ENT_ARC:
	err = ProArcdataGet(p_curve_data, v1, v2, p1, &st_a, &en_a, &rad);
    	TEST_CALL_REPORT("ProArcdataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType Arc\n", str);
	ProTKFprintf(file,"%svec1 ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(v1));
	ProTKFprintf(file,"%svec2 ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(v2));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(p1));
	ProTKFprintf(file,"%sstart_angle %.5f\n", next_line, st_a);
	ProTKFprintf(file,"%send_angle %.5f\n", next_line, en_a);
	ProTKFprintf(file,"%sradius %.5f\n", next_line, rad);
	break;
    case PRO_ENT_SPLINE:
	err = ProSplinedataGet(p_curve_data, &p_par_arr, &p_pnt_arr,
	    &p_tan_arr, &n_point);
    	TEST_CALL_REPORT("ProSplinedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType Spline\n", str);
 	ProTKFprintf(file,"%sParameters\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_par_arr, n_point);
 	ProTKFprintf(file,"%sPoints\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_pnt_arr,
	    n_point * 3);
 	ProTKFprintf(file,"%sTangent Vectors\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_tan_arr,
	    n_point * 3);
	break;
    case PRO_ENT_B_SPLINE:
	err = ProBsplinedataGet(p_curve_data, &degree, &p_par_arr,
	    &p_weight_arr, &p_pnt_arr, &n_knots, &n_point);
    	TEST_CALL_REPORT("ProBsplinedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType B_spline\n", str);
	ProTKFprintf(file,"%sDegree %d\n", str, degree);
 	ProTKFprintf(file,"%sParameters\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_par_arr, n_knots);
	if (p_weight_arr != NULL)
	{
 	    ProTKFprintf(file,"%sWeights\n", str);
	    ProUtilDoubleArrayPrint(file, next_line, p_weight_arr, n_point);
	}
 	ProTKFprintf(file,"%sPoints\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_pnt_arr,
            n_point * 3);
	break;
    case PRO_ENT_CMP_CRV:
	err = ProCompositeCurvedataGet(p_curve_data, &p_comp_arr);
    	TEST_CALL_REPORT("ProCompositeCurvedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	err = ProArraySizeGet((ProArray)p_comp_arr, &n_comp);    
	if (err != PRO_TK_NO_ERROR)
	    break;
	ProTKFprintf(file,"%sType Composite\n", str);
	ProTKFprintf(file,"%sComponents [%d]\n", str, n_comp);
	for (i=0; i<n_comp; i++)
	{
	    err = ProComponentCurvedataGet(p_comp_arr+i, &curve_data, &dir);
	    TEST_CALL_REPORT("ProComponentCurvedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	    ProTKFprintf(file,"%sComponent %d\n", next_line, i);
	    ProTKFprintf(file,"%sDirection %d\n", next_line, dir);
	    ProUtilCurvedataPrint(file, next_line, &curve_data);
	}
	break;
    case PRO_ENT_ELLIPSE:
        err = ProEllipsedataGet (p_curve_data, p1, v1, v2, &d1, &d2, &st_a,
	    &en_a);
        TEST_CALL_REPORT("ProEllipsedataGet()", 
	           "ProUtilCurvedataPrint()", err, err != PRO_TK_NO_ERROR);
	start_ang = atan2(d2*sin(st_a), d1*cos(st_a));
	end_ang = atan2(d2*sin(en_a), d1*cos(en_a));
	ProTKFprintf(file,"%sType Ellipse\n", str);
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(p1));
	ProTKFprintf(file,"%sx axis ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(v1));
	ProTKFprintf(file,"%sy axis ( %.5f, %.5f, %.5f)\n", next_line, VECTOR(v2));
	ProTKFprintf(file,"%sx axis length %.5f\n", next_line, d1);
	ProTKFprintf(file,"%sy axis length %.5f\n", next_line, d2);
	ProTKFprintf(file,"%sstart_angle parameter %.5f\n", next_line, st_a);
	ProTKFprintf(file,"%send_angle parameter %.5f\n", next_line, en_a);
	ProTKFprintf(file,"%sstart_angle %.5f\n", next_line, start_ang);
	ProTKFprintf(file,"%send_angle %.5f\n", next_line, end_ang);
        break;
    default:
	ProTKFprintf(file,"%sType Unsupported (%d)\n", str, c_type);
 	break;
    }
    return (err);
}

/*=========================================================================*\
    Function:	ProUtilSurfacedataPrint()
    Purpose:	Prints surface data to file
\*=========================================================================*/
ProError ProUtilSurfacedataPrint(
    FILE *file,
    char *str,
    ProSurfacedata *p_surface_data)
{
    ProCharLine next_line, next_line2;
    ProError err;
    ProSrftype srftype;
    ProUvParam uv_min, uv_max;
    ProSurfaceOrient s_orient; 
    ProSurfaceshapedata s_shapedata;
    int i, j, s_id, num_u, num_v, num_point, n_conts, n_edges, deg[2];
    int  *p_edge_arr;
    double rad1, rad2, alpha, *p_u_arr, *p_v_arr, *p_wg_arr;
    ProContourdata *p_cont_arr;
    ProContourTraversal trav;
    ProVector e1, e2, e3, *p_u_t_arr, *p_v_t_arr, *p_uvd_arr;
    Pro3dPnt origin, *p_arr;
    ProCurvedata cur1, cur2;
    ProSplinedata pnt_spline, ctr_spline, tan_spline;
    ProSplinesrfdata  splsrf;
        
    ProUtilstrcpy(next_line, (const char *)str);
    ProUtilstrcat(next_line, "    ");
    ProUtilstrcpy(next_line2, (const char *)next_line);
    ProUtilstrcat(next_line2, "    ");
    err = ProSurfacedataGet(p_surface_data, &srftype, uv_min, uv_max,
	&s_orient, &s_shapedata, &s_id);
    TEST_CALL_REPORT("ProSurfacedataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
    ProTKFprintf(file,"%sId  %d\n", str, s_id);
    ProTKFprintf(file,"%sUv min ( %.5f, %.5f)\n", str, uv_min[0], uv_min[1]);
    ProTKFprintf(file,"%sUv max ( %.5f, %.5f)\n", str, uv_max[0], uv_max[1]);
    ProTKFprintf(file,"%sOrient %d\n", str, s_orient);

    err = ProSurfacedataContourArrayGet( p_surface_data, &p_cont_arr);
    TEST_CALL_REPORT("ProSurfacedataContourArrayGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
    err = ProArraySizeGet((ProArray)p_cont_arr, &n_conts);
    ProTKFprintf(file,"%sContours [%d]\n", str, n_conts);
    for (i=0; i<n_conts; i++)
    {
        err = ProContourdataGet(p_cont_arr+i, &trav);
    	TEST_CALL_REPORT("ProContourdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);

	err = ProContourdataEdgeIdArrayGet(p_cont_arr+i, &p_edge_arr);
    	TEST_CALL_REPORT("ProContourdataEdgeIdArrayGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);

	ProTKFprintf(file,"%sContour %d\n", str, i+1);
	ProTKFprintf(file,"%sTraversal %s\n", next_line, 
            trav == PRO_CONTOUR_TRAV_NONE ? "none" :
	    trav == PRO_CONTOUR_TRAV_INTERNAL ? "internal" : "external" );
	
	err = ProArraySizeGet((ProArray)p_edge_arr, &n_edges);
	ProTKFprintf(file,"%sEdge Id Array [%d]\n", next_line,  n_edges); 
	ProTKFprintf(file,"%s", next_line2); 
	for (j=0; j<n_edges; j++)
	{
	    ProTKFprintf(file,"%d", p_edge_arr[j]);
	    if (j < n_edges-1)
		ProTKFprintf(file, ",");
	}
	ProTKFprintf(file, "\n");
    }

    switch (srftype)
    {
    case  PRO_SRF_PLANE:
        err = ProPlanedataGet( &s_shapedata, e1, e2, e3, origin);
        TEST_CALL_REPORT("ProPlanedataGet()",
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType Plane\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
	break;
    case PRO_SRF_CYL:
        err = ProCylinderdataGet( &s_shapedata, e1, e2, e3, origin, &rad1);
        TEST_CALL_REPORT("ProCylinderdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
	ProTKFprintf(file,"%sType Cylinder\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
	ProTKFprintf(file,"%sradius %.5f\n", str, rad1);
	break;
    case PRO_SRF_CONE:
        err = ProConedataGet( &s_shapedata, e1, e2, e3, origin, &alpha);
        TEST_CALL_REPORT("ProConedataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Cone\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
	ProTKFprintf(file,"%salpha %.5f\n", str, alpha);
	break;
    case PRO_SRF_TORUS:
        err = ProTorusdataGet( &s_shapedata, e1, e2, e3, origin, &rad1, &rad2);
        TEST_CALL_REPORT("ProTorusdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Torus\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
	ProTKFprintf(file,"%sradius1 %.5f\n", str, rad1);
	ProTKFprintf(file,"%sradius2 %.5f\n", str, rad2);
	break;
    case PRO_SRF_FIL:
        err = ProFilsrfdataGet( &s_shapedata, &pnt_spline, &ctr_spline,
	    &tan_spline);
        TEST_CALL_REPORT("ProFilsrfdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Fillet\n", str);
 	ProTKFprintf(file,"%sPnt Spline\n", str);
	ProUtilCurvedataPrint(file, next_line, (ProCurvedata*)&pnt_spline);
 	ProTKFprintf(file,"%sCtr Spline\n", str);
	ProUtilCurvedataPrint(file, next_line, (ProCurvedata*)&ctr_spline);
 	ProTKFprintf(file,"%sTan Spline\n", str);
	ProUtilCurvedataPrint(file, next_line, (ProCurvedata*)&tan_spline);
	break;
    case PRO_SRF_RUL:
        err = ProRulsrfdataGet( &s_shapedata, e1, e2, e3, origin, &cur1,
	    &cur2);
        TEST_CALL_REPORT("ProRulsrfdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Ruled\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
 	ProTKFprintf(file,"%sCurve_1\n", str);
	ProUtilCurvedataPrint(file, next_line, &cur1);
 	ProTKFprintf(file,"%sCurve_2\n", str);
	ProUtilCurvedataPrint(file, next_line, &cur2);
	break;
    case PRO_SRF_REV:
        err = ProSrfrevdataGet( &s_shapedata, e1, e2, e3, origin, &cur1);
        TEST_CALL_REPORT("ProSrfrevdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Rev\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
 	ProTKFprintf(file,"%sCurve\n", str);
	ProUtilCurvedataPrint(file, next_line, &cur1);
	break;
    case PRO_SRF_TABCYL:
        err = ProTabcyldataGet( &s_shapedata, e1, e2, e3, origin, &cur1);
        TEST_CALL_REPORT("ProTabcyldataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Tabcyl\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
 	ProTKFprintf(file,"%sCurve\n", str);
	ProUtilCurvedataPrint(file, next_line, &cur1);
	break;
    case PRO_SRF_B_SPL:
        err = ProBsplinesrfdataGet( &s_shapedata, deg, &p_u_arr, &p_v_arr,
	    &p_wg_arr, &p_arr, &num_u, &num_v, &num_point);
        TEST_CALL_REPORT("ProBsplinesrfdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType B_spline\n", str);
 	ProTKFprintf(file,"%sdegree %d, %d\n", str, deg[0], deg[1]);
 	ProTKFprintf(file,"%sU Param Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_u_arr, num_u);
 	ProTKFprintf(file,"%sV Param Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_v_arr, num_v);
	if (p_wg_arr != NULL)
	{
 	    ProTKFprintf(file,"%sWeights Array\n", str);
	    ProUtilDoubleArrayPrint(file, next_line, p_wg_arr, num_point);
	}
 	ProTKFprintf(file,"%sPoint Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line,  (double*)p_arr,
	    num_point * 3);
	break;
    case PRO_SRF_CYL_SPL:
        err = ProCylsplsrfdataGet( &s_shapedata, e1, e2, e3, origin, &splsrf);
        TEST_CALL_REPORT("ProCylsplsrfdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Cyl Spl\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str, VECTOR(e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str, VECTOR(origin));
	ProTKFprintf(file,"%sSpline Srf\n", str);

        err = ProSplinesrfdataGet( (ProSurfaceshapedata*)&splsrf, &p_u_arr,
	    &p_v_arr, &p_arr, &p_u_t_arr, &p_v_t_arr, &p_uvd_arr, &num_u,
	    &num_v);
        TEST_CALL_REPORT("ProSplinesrfdataGet()", 
	    "ProUtilSurfaceshapedataCopy()", err, err != PRO_TK_NO_ERROR);

 	ProTKFprintf(file,"%sU Param Array\n", str);
	num_point = num_u * num_v;
	ProUtilDoubleArrayPrint(file, next_line, p_u_arr, num_u);
 	ProTKFprintf(file,"%sV Param Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_v_arr, num_v);
 	ProTKFprintf(file,"%sPoint Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sU Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_u_t_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sV Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_v_t_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sUV Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_uvd_arr,
	    num_point * 3);
	break;
    case PRO_SRF_SPL:
        err = ProSplinesrfdataGet( &s_shapedata, &p_u_arr, &p_v_arr, &p_arr,
	    &p_u_t_arr, &p_v_t_arr, &p_uvd_arr, &num_u, &num_v);
        TEST_CALL_REPORT("ProSplinesrfdataGet()", 
	           "ProUtilSurfacedataPrint()", err, err != PRO_TK_NO_ERROR);
 	ProTKFprintf(file,"%sType Spline\n", str);
 	ProTKFprintf(file,"%sU Param Array\n", str);
	num_point = num_u * num_v;
	ProUtilDoubleArrayPrint(file, next_line, p_u_arr, num_u);
 	ProTKFprintf(file,"%sV Param Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, p_v_arr, num_v);
 	ProTKFprintf(file,"%sPoint Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sU Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_u_t_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sV Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_v_t_arr,
	    num_point * 3);
 	ProTKFprintf(file,"%sUV Der Array\n", str);
	ProUtilDoubleArrayPrint(file, next_line, (double*)p_uvd_arr,
	    num_point * 3);
	break;
    case PRO_SRF_FOREIGN :
 	ProTKFprintf(file,"%sType Foreign\n", str);
	ProTKFprintf(file,"%se1 ( %.5f, %.5f, %.5f)\n", str,
	    VECTOR(s_shapedata.frgnsrf.e1));
	ProTKFprintf(file,"%se2 ( %.5f, %.5f, %.5f)\n", str,
	    VECTOR(s_shapedata.frgnsrf.e2));
	ProTKFprintf(file,"%se3 ( %.5f, %.5f, %.5f)\n", str,
	    VECTOR(s_shapedata.frgnsrf.e3));
	ProTKFprintf(file,"%sorigin ( %.5f, %.5f, %.5f)\n", str,
	    VECTOR(s_shapedata.frgnsrf.origin));
	ProTKFprintf(file,"%sForein Id %d\n", str, s_shapedata.frgnsrf.foreign_id);
	break;
    default:
 	ProTKFprintf(file,"%sType Unsupported (%d)\n", str, srftype);
	break;	
    }
    return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
    Function:	ProUtilIntfDataPrint()
    Purpose:	Prints intf data to file
\*=========================================================================*/
ProError ProUtilIntfDataPrint(
    FILE *file,
    ProIntfData *p_intf_data)
{
    ProAccuracytype accuracy_type;
    ProOutline outline;
    double accuracy;
    int i, j, n_edges, n_surfaces, id, edge_surf_id[2], n_uv_points;
    int n_quilts;
    ProError err;
    ProEdgeDir	edge_dirs[2];
    ProUvParam	(*p_edge_uv_point_arr)[2];
    ProEdgedata *p_edge_data;
    ProSurfacedata *p_surface_data, **p_surf_arr;	
    ProCurvedata edge_curve_data, *p_edge_uv_curve_data[2];
    ProCurvedata edge_uv_curve_0, edge_uv_curve_1;
    ProQuiltdata *p_quilt_data;

    p_edge_uv_curve_data[0] = &edge_uv_curve_0;
    p_edge_uv_curve_data[1] = &edge_uv_curve_1;

    err = ProIntfDataAccuracyGet(p_intf_data, &accuracy);
    err = ProIntfDataAccuracytypeGet(p_intf_data, &accuracy_type);
    err = ProIntfDataOutlineGet(p_intf_data, outline);

    ProTKFprintf(file, "Accuracy %.5f\n", accuracy);
    ProTKFprintf(file, "Accuracy type %d\n", accuracy_type);;
    ProTKFprintf(file, "Outline ( %.5f, %.5f, %.5f) - ( %.5f, %.5f, %.5f)\n", 
	outline[0][0], outline[0][1], outline[0][2], 
	outline[1][0], outline[1][1], outline[1][2]);

    err = ProIntfDataSurfaceCount(p_intf_data, &n_surfaces);
    ProTKFprintf(file, "Surfaces [%d]\n", n_surfaces);
    for (i=0; i<n_surfaces; i++)
    {
	err = ProIntfDataSurfaceGet(p_intf_data, i, &p_surface_data);
	ProTKFprintf(file, "Surface  %d\n", i + 1);
	ProUtilSurfacedataPrint(file, (char *)"    ", p_surface_data);
	err = ProSurfacedataMemoryFree(p_surface_data);
    }

    err = ProIntfDataEdgeCount(p_intf_data, &n_edges);
    ProTKFprintf(file, "Edges [%d]\n", n_edges);
    for (i=0; i<n_edges; i++)
    {
	
	err = ProIntfDataEdgeGet(p_intf_data, i, &p_edge_data);
	err = ProEdgedataGet(p_edge_data, &id, edge_surf_id, edge_dirs,
	    &p_edge_uv_point_arr, p_edge_uv_curve_data, &edge_curve_data);
        
	err = ProEdgedataMemoryFree(p_edge_data);        

	ProTKFprintf(file, "Edge %d\n", i + 1);
	ProTKFprintf(file, "    Id %d\n", id);
	ProTKFprintf(file, "    Surfaces Ids %d %d\n", edge_surf_id[0],
	    edge_surf_id[1]);
	ProTKFprintf(file, "    Edge dirs %d %d\n", edge_dirs[0], edge_dirs[1]);
	ProTKFprintf(file, "    Edge curve\n");
	ProUtilCurvedataPrint(file, (char *)"    ", &edge_curve_data);
	
	ProTKFprintf(file, "    Edge uv curve[0]\n");
	ProUtilCurvedataPrint(file, (char *)"        ", p_edge_uv_curve_data[0]);
	
	ProTKFprintf(file, "    Edge uv curve[1]\n");
	ProUtilCurvedataPrint(file, (char *)"        ", p_edge_uv_curve_data[1]);

	ProTKFprintf(file, "    Edge uv point array\n");
	err = ProArraySizeGet((ProArray)p_edge_uv_point_arr, &n_uv_points);
	ProUtilDoubleArrayPrint(file, (char *)"        ",
            (double*)p_edge_uv_point_arr, n_uv_points * 4);
    }

    err = ProIntfDataQuiltCount(p_intf_data, &n_quilts);
    ProTKFprintf(file, "Quits [%d]\n", n_quilts);
    for (i=0; i<n_quilts; i++)
    {
	err = ProIntfDataQuiltGet(p_intf_data, i, &p_quilt_data);
	ProTKFprintf(file, "Quit %d\n", i + 1);
	err = ProQuiltdataGet(p_quilt_data, &id);
	ProTKFprintf(file, "    Id %d\n", id);
	err = ProQuiltdataSurfArrayGet(p_quilt_data, &p_surf_arr);
	err = ProArraySizeGet((ProArray)p_surf_arr, &n_surfaces);

	ProTKFprintf(file, "    Quit surfaces %d\n", n_surfaces);
	for (j=0; j<n_surfaces; j++)
	{
	    ProTKFprintf(file, "    Quit surface  %d\n", i + 1);
	    ProUtilSurfacedataPrint(file, (char *)"        ", p_surf_arr[j]);
	}
	err = ProQuiltdataMemoryFree(p_quilt_data);
    }
    return ( PRO_TK_NO_ERROR);
}

#undef VECTOR

