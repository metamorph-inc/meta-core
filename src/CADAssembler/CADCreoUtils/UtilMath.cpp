

/*--------------------------------------------------------------------*\
Pro/DEVELOP includes
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"

/*--------------------------------------------------------------------*\
C System includes
\*--------------------------------------------------------------------*/
#include <math.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "UtilMath.h"

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/

/*====================================================================*\
    FUNCTION :	ProUtilVectorCopy()
    PURPOSE  :	Copy a vector to another
\*====================================================================*/
double *ProUtilVectorCopy(
    double from[3],
    double to[3])
{
    if(from == NULL)
	to[0] = to[1] = to[2] = 0.0;
    else
    {
	to[0] = from[0];
	to[1] = from[1];
	to[2] = from[2];
    }
    return(to);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorCross()
    PURPOSE  :	Cross product of two vectors
\*====================================================================*/
double *ProUtilVectorCross(
    double v1[3],
    double v2[3],
    double c[3])
{
    c[0] = v1[1] * v2[2] - v1[2] * v2[1];
    c[1] = v1[2] * v2[0] - v1[0] * v2[2];
    c[2] = v1[0] * v2[1] - v1[1] * v2[0];

    return(c);
}

/*====================================================================*\
    FUNCTION :	ProUtilPointsDist()
    PURPOSE  :	Distance between two points
\*====================================================================*/
double ProUtilPointsDist(
    double p1[3],
    double p2[3])
{
    double aux, diff[3];

    ProUtilVectorDiff(p1, p2, diff);
    aux = ProUtilVectorDot(diff, diff);
    aux = sqrt(aux);
    return(aux);
}

/*====================================================================*\
    FUNCTION :	ProUtilPointLineDist()
    PURPOSE  :	Distance between a point and a line
\*====================================================================*/
double ProUtilPointLineDist(
    double point[3],
    double end1[3],
    double end2[3])
{
    double v1[3], v2[3], dt, p1[3], dist;

    ProUtilVectorDiff(end1, end2, v1);
    ProUtilVectorNormalize(v1, v1);
    ProUtilVectorDiff(point, end1, v2);
    dt = ProUtilVectorDot(v1, v2);
    ProUtilVectorsLincom(1.0, end1, dt, v1, p1);
    dist = ProUtilPointsDist(point, p1);
    return(dist);
}

/*====================================================================*\
    FUNCTION :	ProUtilPointPlaneDist()
    PURPOSE  :	Distance between a point and a plane
\*====================================================================*/
double ProUtilPointPlaneDist(
    double point[3],	/* The point			*/
    double origin[3],	/* A point on the plane		*/
    double normal[3])	/* The normal to the plane	*/
{
    double v[3], dist;

    ProUtilVectorDiff(point, origin, v);
    dist = ProUtilVectorDot(v, normal);
    return(dist);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorDot()
    PURPOSE  :	Dot product of two vectors
\*====================================================================*/
double ProUtilVectorDot(
    double a[3],
    double b[3])
{
    double dot;
    dot = a[2] * b[2] + a[1] * b[1] + a[0] * b[0];
    return(dot);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorsLincom()
    PURPOSE  :	Linear combination of two vectors
\*====================================================================*/
double *ProUtilVectorsLincom(
    double scalar_a,
    double vector_a[3],
    double scalar_b,
    double vector_b[3],
    double result[3])
{
    result[0] = scalar_a * vector_a[0] + scalar_b * vector_b[0];
    result[1] = scalar_a * vector_a[1] + scalar_b * vector_b[1];
    result[2] = scalar_a * vector_a[2] + scalar_b * vector_b[2];

    return(result);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorNormalize()
    PURPOSE  :	Normalize a vector
\*====================================================================*/
double *ProUtilVectorNormalize(
    double input[3],
    double output[3])
{
    double len;
    len = ProUtilVectorLength(input);
    return(ProUtilVectorScale(1.0/len, input, output));
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorScale()
    PURPOSE  :	Multiply a vector by a scalar
\*====================================================================*/
double *ProUtilVectorScale(
    double scalar,
    double vector[3],
    double result[3])
{
    result[0] = scalar * vector[0];
    result[1] = scalar * vector[1];
    result[2] = scalar * vector[2];
    return(result);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorSum()
    PURPOSE  :	Sum of two vectors
\*====================================================================*/
double *ProUtilVectorSum(
    double a[3],
    double b[3],
    double sum[3])
{
    sum[0] = a[0] + b[0];
    sum[1] = a[1] + b[1];
    sum[2] = a[2] + b[2];
    return(sum);
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorLength()
    PURPOSE  :	Length of a vector
\*====================================================================*/
double ProUtilVectorLength(
    double v[3])
{
    return(sqrt(v[0] * v[0] +
		v[1] * v[1] +
		v[2] * v[2]));
}

/*====================================================================*\
    FUNCTION :	ProUtilVectorDiff()
    PURPOSE  :	Difference of two vectors
\*====================================================================*/
double *ProUtilVectorDiff(
    double a[3],
    double b[3],
    double c[3])
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
    return(c);
}

/*====================================================================*\
    FUNCTION :	ProUtilPlaneLineInter()
    PURPOSE  :	Intersection between a plane and a line
\*====================================================================*/
double *ProUtilPlaneLineX(
    double plane_point[3],	/* IN - point on the plane	*/
    double plane_normal[3],	/* IN - normal to the plane	*/
    double line_point[3],	/* IN - point on the line	*/
    double line_dir[3],		/* IN - direction of the line	*/
    double x_point[3])		/* OUT - intersection point	*/
{
    double proj, vector[3], dist;

    proj = ProUtilVectorDot(plane_normal, line_dir);

/*--------------------------------------------------------------------*\
    If the line is parallel to the plane, return NULL
\*--------------------------------------------------------------------*/
    if(fabs(proj) < EPSM6)
	return(NULL);

    ProUtilVectorDiff(plane_point, line_point, vector);
    dist = ProUtilVectorDot(vector, plane_normal)/proj;
    ProUtilVectorsLincom(1.0, line_point, dist, line_dir, x_point);
    return(x_point);
}

/*====================================================================*\
Function :  ProUtilVectorsToTransf()
Purpose  :  Creates transformation from vectors defining it
\*====================================================================*/
ProError ProUtilVectorsToTransf(
    double x_vector[3],           /* IN - X directional vector   */
    double y_vector[3],           /* IN - Y directional vector   */
    double z_vector[3],           /* IN - Z directional vector   */
    double o_vector[3],           /* IN - Origin                 */
    double transform[4][4])       /* OUT - transformation matrix */
{
    int n;

    if ( x_vector != NULL )
    {
	transform[0][0] = x_vector[0];
	transform[0][1] = x_vector[1];
	transform[0][2] = x_vector[2];
    }

    if( y_vector != NULL )
    {
       transform[1][0] = y_vector[0];
       transform[1][1] = y_vector[1];
       transform[1][2] = y_vector[2];
    }

    if( z_vector != NULL )
    {
       transform[2][0] = z_vector[0];
       transform[2][1] = z_vector[1];
       transform[2][2] = z_vector[2];
    }

    if( o_vector != NULL )
    {
       transform[3][0] = o_vector[0];
       transform[3][1] = o_vector[1];
       transform[3][2] = o_vector[2];
    }

    for(n = 0; n < 4; n++)
	transform[n][3] = (n == 3) ? 1.0 : 0.0;

    return(PRO_TK_NO_ERROR);
}

/*====================================================================*\
Function :  ProUtilTransfToVectors()
Purpose  :  Get the vectors out of the transformation matrix
\*====================================================================*/
ProError ProUtilTransfToVectors(
    double transform[4][4],      /* IN  - transformation matrix  */
    double x_vector[3],           /* OUT - X directional vector   */
    double y_vector[3],           /* OUT - Y directional vector   */
    double z_vector[3],           /* OUT - Z directional vector   */
    double o_vector[3])           /* OUT - Origin                 */
{
    if (transform == NULL)
        {
           ProUtilVectorCopy(NULL, x_vector);
           ProUtilVectorCopy(NULL, y_vector);
           ProUtilVectorCopy(NULL, z_vector);
           ProUtilVectorCopy(NULL, o_vector);
        }
    else
        {
           ProUtilVectorCopy(transform[0], x_vector);
           ProUtilVectorCopy(transform[1], y_vector);
           ProUtilVectorCopy(transform[2], z_vector);
           ProUtilVectorCopy(transform[3], o_vector);
        }

    return(PRO_TK_NO_ERROR);
}

