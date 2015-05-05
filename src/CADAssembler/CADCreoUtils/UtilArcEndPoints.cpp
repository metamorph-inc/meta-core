



/*--------------------------------------------------------------------*\
    Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"
#include "UtilMath.h"


/*--------------------------------------------------------------------*\
    Pro/DEVELOP includes
\*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*\
    C System includes
\*--------------------------------------------------------------------*/
#include <math.h>


/*--------------------------------------------------------------------*\
    Application includes
\*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*\
    Macros
\*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*\
    Data types
\*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*\
    Application global/external data
\*--------------------------------------------------------------------*/


/*---------------------------------------------------------------------*\
    Functions declaration
\*---------------------------------------------------------------------*/



/*---------------------------------------------------------------------*\
    Function:	ProUtilArc3Points()	( former user_arc_3pts() )
    Purpose:	Create an arc from 3 points
    Returns:	1 - success; 0 - error.
\*---------------------------------------------------------------------*/
int ProUtilArc3Points(
    double pnt1[3],
    double pnt2[3], 
    double pnt3[3], 
    double *p_rad, 
    double center[3])
{
double	vec_a[3], vec_b[3], vec_c[3];
double	len_a, len_b, unit_a[3], unit_b[3];
double  a_dot_b, a_x_b_sq, a_x_b[3];
double  radius, c0, c1;

		/*  Form unit vector a  */
	ProUtilVectorDiff(pnt3, pnt2, vec_a);
	len_a = ProUtilVectorLength(vec_a);
	ProUtilVectorNormalize(vec_a, unit_a);

		/*  Form unit vector b  */
	ProUtilVectorDiff(pnt1, pnt2, vec_b);
	len_b = ProUtilVectorLength(vec_b);
	ProUtilVectorNormalize(vec_b, unit_b);

	if( len_a != 0.0 && len_b != 0.0 )
	  {
		ProUtilVectorCross(unit_a, unit_b, a_x_b);
		a_x_b_sq = ProUtilVectorDot(a_x_b, a_x_b);
		a_dot_b  = ProUtilVectorDot(unit_a, unit_b);
	  }
	else
		a_dot_b = 0.0;


		/*  Check for a straight line  */
	if( len_a == 0.0 || len_b == 0.0 || a_x_b_sq < 1.0e-10 )	
	{
	*p_rad = radius = 10e+10;
	return(0);
	}

		/*  Arc radius  */
	c0 = sqrt(a_x_b_sq);
	ProUtilVectorDiff(vec_a,vec_b,vec_c);
	radius = ProUtilVectorLength(vec_c) / (2.0 * c0);
	*p_rad = radius;

		/*  Arc center  */
	c0 = 0.5 * (1.0 - a_dot_b * len_b / len_a) / a_x_b_sq;
	c1 = 0.5 * (1.0 - a_dot_b * len_a / len_b) / a_x_b_sq;
	ProUtilVectorsLincom(c0, vec_a, c1, vec_b, vec_c);
	ProUtilVectorSum(pnt2, vec_c, center);

	return(1);
}



/*---------------------------------------------------------------------*\
    Function:	ProUtilArcEndPoints() (former user_arc_endpts() )
    Purpose:	Arc is drawn CCW, from end1 to end2
    Returns:	1 - success; 0 - error.
\*---------------------------------------------------------------------*/
int ProUtilArcEndPoints(
    double x1, double y1, double z1, 
    double x2, double y2, double z2, 
    double x3, double y3, double z3, 
    double cx, double cy, double cz, 
    double end1[3], double end2[3])
{
     double tw1 = 0.0, tw3 = 0.0, tw2 = 0.0;

		/*  Check if point 1 is on center  */
	if (y1 != cy || x1 != cx)
	   tw1 = atan2(y1-cy,x1-cx);
    		if (tw1 < 0.0) tw1 += TWOPI;

		/*  Check if point 2 is on center  */
	if (y2 != cy || x2 != cx)
	   tw2 = atan2(y2-cy,x2-cx);
    		if (tw2 < 0.0) tw2 += TWOPI;
 
		/*  Check if point 3 is on center  */
	if (y3 != cy || x3 != cx)
	   tw3 = atan2(y3-cy,x3-cx);
    		if (tw3 < 0.0) tw3 += TWOPI;


   if ( ((tw3 > tw1) && (tw3 > tw2)) || ((tw3 < tw1) && (tw3 < tw2)) )
      {
       if (tw2 > tw1)			/*  Switch endpoints  */
	 {
	end1[0] = x2;   end1[1] = y2;   end1[2] = z2;
	end2[0] = x1;   end2[1] = y1;   end2[2] = z1;
	 }
       else				/*  Same endpoints  */
	 {
	end1[0] = x1;   end1[1] = y1;   end1[2] = z1;
	end2[0] = x2;   end2[1] = y2;   end2[2] = z2;
	 }
      }

   else
     {
      if (tw2 > tw1)			/*  Same endpoints  */
	{
	end1[0] = x1;   end1[1] = y1;   end1[2] = z1;
	end2[0] = x2;   end2[1] = y2;   end2[2] = z2;
	}
      else				/*  Switch endpoints  */
	{
	end1[0] = x2;   end1[1] = y2;   end1[2] = z2;
	end2[0] = x1;   end2[1] = y1;   end2[2] = z1;
	}
     }

	return(0);
}
