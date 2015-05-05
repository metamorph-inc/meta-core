
#ifndef UTILMATH_H
#define UTILMATH_H

/*--------------------------------------------------------------------*\
System includes
\*--------------------------------------------------------------------*/
#include <math.h>
/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/
#ifdef PI
#undef PI
#endif

#define PI          3.14159265358979323846
#define PID2        1.57079632679489661923
#define TWOPI                 (PI + PI)
#define EPSM6                 1.0e-6
#define EPSM10                1.0e-10


/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application external data
\*--------------------------------------------------------------------*/


extern double *ProUtilVectorCopy(
    double from[3],
    double to[3]);

extern double *ProUtilVectorCross(
    double v1[3],
    double v2[3],
    double c[3]);

extern double ProUtilPointsDist(
    double p1[3],
    double p2[3]);

extern double ProUtilPointLineDist(
    double point[3],
    double end1[3],
    double end2[3]);

extern double ProUtilPointPlaneDist(
    double point[3],
    double origin[3],
    double normal[3]);

extern double ProUtilVectorDot(
    double a[3],
    double b[3]);

extern double *ProUtilVectorsLincom(
    double scalar_a,
    double vector_a[3],
    double scalar_b,
    double vector_b[3],
    double result[3]);

extern double *ProUtilVectorNormalize(
    double input[3],
    double output[3]);

extern double *ProUtilVectorScale(
    double scalar,
    double vector[3],
    double result[3]);

extern double *ProUtilVectorSum(
    double a[3],
    double b[3],
    double sum[3]);

extern double ProUtilVectorLength(
    double v[3]);

extern double *ProUtilVectorDiff(
    double a[3],
    double b[3],
    double c[3]);

extern double *ProUtilPlaneLineX(
    double plane_point[3],      /* IN - point on the plane      */
    double plane_normal[3],     /* IN - normal to the plane     */
    double line_point[3],       /* IN - point on the line       */
    double line_dir[3],         /* IN - direction of the line   */
    double x_point[3]);

ProError ProUtilVectorsToTransf(
   double x_vector[3],           /* IN - X directional vector   */
   double y_vector[3],           /* IN - Y directional vector   */
   double z_vector[3],           /* IN - Z directional vector   */
   double o_vector[3],           /* IN - Origin                 */
   double transform[4][4] );     /* OUT - transformation matrix */

ProError ProUtilTransfToVectors(
   double transform[4][4],       /* IN - transformation matrix   */
   double x_vector[3],           /* OUT - X directional vector   */
   double y_vector[3],           /* OUT - Y directional vector   */
   double z_vector[3],           /* OUT - Z directional vector   */
   double o_vector[3] );         /* OUT - Origin                 */

#endif /* ifdef UTILMATH_H */
