

/*--------------------------------------------------------------------*\
Pro/DEVELOP includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProAssembly.h>
#include <ProGeomitemdata.h>
#include <ProMdl.h>
#include <ProSolid.h>
#include <ProSelection.h>
/*--------------------------------------------------------------------*\
C System includes
\*--------------------------------------------------------------------*/
#include <math.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "UtilMath.h"
#include "UtilMatrix.h"
#include "TestError.h"

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/

static double identity_matrix[4][4] = { {1.0, 0.0, 0.0, 0.0},
					{0.0, 1.0, 0.0, 0.0},
					{0.0, 0.0, 1.0, 0.0},
					{0.0, 0.0, 0.0, 1.0} };

/*====================================================================*\
    FUNCTION :	ProUtilMatrixCopy()
    PURPOSE  :	Copy one matrix to another, or initialize a matrix to
		be the unitary matrix
\*====================================================================*/
void ProUtilMatrixCopy(
    double input[4][4],
    double output[4][4])
{
    int i,j;

    if(input == NULL)
    {
	for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
		output[i][j] = identity_matrix[i][j];
    }
    else
    {
	for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
		output[i][j] = input[i][j];
    }
}

/*====================================================================*\
    FUNCTION :	ProUtilPointTrans()
    PURPOSE  :	Transform a 3d point by a matrix
\*====================================================================*/
void ProUtilPointTrans(
    double m[4][4],
    double p[3],
    double output[3])
{
    double buff[4];
    ProError status;

    if(m == NULL)
    {
	ProUtilVectorCopy(p, output);
	return;
    }

    status = ProPntTrfEval(p, m, buff);
    TEST_CALL_REPORT("ProPntTrfEval()", "ProUtilPointTrans()", 
			status, status!=PRO_TK_NO_ERROR);
    ProUtilVectorCopy(buff, output);

}

/*====================================================================*\
    FUNCTION :	ProUtilVectorTrans()
    PURPOSE  :	Transform a 3d vector by a matrix
\*====================================================================*/
void ProUtilVectorTrans(
    double m[4][4],
    double v[3],
    double output[3])
{
    ProError status;
    
    if(m == NULL)
    {
	ProUtilVectorCopy(v, output);
	return;
    }

    status = ProVectorTrfEval(v, m, output);
    TEST_CALL_REPORT("ProVectorTrfEval()", "ProUtilPointTrans()", 
			status, status!=PRO_TK_NO_ERROR);

}

/*====================================================================*\
    FUNCTION :	ProUtilMatrixProduct()
    PURPOSE  :	Multiply two transformation matrices
\*====================================================================*/
void ProUtilMatrixProduct(
    double m1[4][4],
    double m2[4][4],
    double output[4][4])
{
    int i,j,k;

/*--------------------------------------------------------------------*\
    If the first matrix is zero, just copy the second
    (So, if the second is zero, copy the identity matrix.)
\*--------------------------------------------------------------------*/
    if(m1 == NULL)
    {
	ProUtilMatrixCopy(m2, output);
	return;
    }
/*--------------------------------------------------------------------*\
    If the second matrix is zero, just copy the first
\*--------------------------------------------------------------------*/
    if(m2 == NULL)
    {
	ProUtilMatrixCopy(m1, output);
	return;
    }

/*--------------------------------------------------------------------*\
    Do the multiplication
\*--------------------------------------------------------------------*/
    for(i=0;i<4;i++)
    {
	for(j=0;j<4;j++)
	{
	    output[i][j] = 0.0;
	    for(k=0;k<4;k++)
		output[i][j] += m1[i][k] * m2[k][j];
	}
    }
}

/*====================================================================*\
    FUNCTION :	ProUtilMatrixInvert()
    PURPOSE  :	Find the inverse of a transformation matrix
\*====================================================================*/
int ProUtilMatrixInvert(
    double m[4][4],
    double output[4][4])
{
    double vec[3], scale_sq, inv_sq_scale;
    int i,j;

/*--------------------------------------------------------------------*\
    If the matrix is null, return the identity matrix
\*--------------------------------------------------------------------*/
    if(m == NULL)
    {
	ProUtilMatrixCopy(NULL, output);
	return(1);
    }

/*--------------------------------------------------------------------*\
    Obtain the matrix scale
\*--------------------------------------------------------------------*/
    vec[0] = m[0][0];
    vec[1] = m[0][1];
    vec[2] = m[0][2];
    scale_sq = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];

/*--------------------------------------------------------------------*\
    Check whether there is an inverse, and if not, return 0
\*--------------------------------------------------------------------*/
    if(scale_sq < (.000000001 * .000000001))
	return(0);

/*--------------------------------------------------------------------*\
    Need the inverse scale squared 
\*--------------------------------------------------------------------*/
    inv_sq_scale = 1.0 / scale_sq;

/*--------------------------------------------------------------------*\
    The orientation vectors
\*--------------------------------------------------------------------*/
    for(j=0;j<3;j++)
    {
	for(i=0;i<3;i++)
	    output[j][i] = m[i][j] * inv_sq_scale;
	output[j][3] = 0.0;
    }

/*--------------------------------------------------------------------*\
    The shift vectors
\*--------------------------------------------------------------------*/
    for(i=0;i<3;i++)
    {
	output[3][i] = 0.0;
	for(j=0;j<3;j++)
	    output[3][i] -= m[i][j] * m[3][j] * inv_sq_scale;
    }
    output[3][3] = 1.0;

    return(1);
}

/*=============================================================*\
  FUNCTION: ProUtilInvertCsysMatrix()
  PURPOSE:  Develop the inverted selection csys transform
\*=============================================================*/
ProError ProUtilInvertCsysMatrix( 
	ProSelection sel, ProGeomitemdata *p_data, ProMatrix tmatrix )
{
    ProError            err;
    ProMatrix           matrix, csysmat;
    ProAsmcomppath path;

	/*  Get the "Asmcomppath transform"  */
	err = ProSelectionAsmcomppathGet(sel, &path);
	err = ProAsmcomppathTrfGet(&path, PRO_B_TRUE, matrix);

	/*  Create the csys transform  */
	ProUtilMatrixCopy( NULL, csysmat );
        ProUtilVectorCopy(p_data->data.p_csys_data->x_vector, csysmat[0]);
        ProUtilVectorCopy(p_data->data.p_csys_data->y_vector, csysmat[1]);
        ProUtilVectorCopy(p_data->data.p_csys_data->z_vector, csysmat[2]);
        ProUtilVectorCopy(p_data->data.p_csys_data->origin,   csysmat[3]);

	/*  Transform the csys 'vectors' by the "Asmcomppath" transform */
	ProUtilMatrixProduct( csysmat, matrix, tmatrix );
        ProUtilMatrixCopy  ( tmatrix, matrix );

	/*  Invert the "selection csys" transform  */
        ProUtilMatrixInvert( matrix, tmatrix );

    return (PRO_TK_NO_ERROR);
}

