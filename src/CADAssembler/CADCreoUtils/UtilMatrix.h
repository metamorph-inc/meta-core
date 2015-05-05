
#ifndef UTILMATRIX_H
#define UTILMATRIX_H

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data types
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application external data
\*--------------------------------------------------------------------*/


extern void ProUtilMatrixCopy(
    double input[4][4],
    double output[4][4]);

extern void ProUtilPointTrans(
    double m[4][4],
    double p[3],
    double output[3]);

extern void ProUtilVectorTrans(
    double m[4][4],
    double v[3],
    double output[3]);

extern void ProUtilMatrixProduct(
    double m1[4][4],
    double m2[4][4],
    double output[4][4]);

extern int ProUtilMatrixInvert(
    double m[4][4],
    double output[4][4]);

#endif /* ifdef UTILMATRIX_H */
