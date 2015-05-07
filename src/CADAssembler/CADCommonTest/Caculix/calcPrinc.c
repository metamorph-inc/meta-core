
#include <extUtil.h>

#define     TEST         0
#define     TEST1        0
#define     MIN          0.
#define     MIN2         1.e-1
#define     MIN3         1.e-10

int  calcPrinc( double *s, double *p, double *a0, double *a1, double *a2, int flag )
/* *************************************************************************** */
/* berechnet aus dem Spannungstensor die Hauptspannungen und deren Raumwinkel  */
/* s: spannungstensor, p: Principals, a0: alfa_p1, a1: alfa_p2, a2: alfa_p3    */
/*                                                                             */
/*                                                                             */
/* wenn flag=0  x[0]  >  x[1]  >  x[2]                                         */
/* wenn flag=1 |x[0]| > |x[1]| > |x[2]|                                        */
/* wenn flag=2 wie 1 aber ohne winkelberechnung                                */
/* *************************************************************************** */
{
  long double a, b, c, mean;
  long double sd[6], pd[3];
  double y[3], p_[3];
  double p1[3];
  double alfa[9];
  int i;

  
  for (i=0; i<6; i++) { sd[i]=s[i];  }
  mean=0; for(i=0; i<3; i++) mean+=sd[i]; mean*=0.33333333;

  a= -(sd[0]+sd[1]+sd[2]);
  b= (sd[0] * sd[1]) + (sd[1] * sd[2]) + (sd[2] * sd[0]) - (sd[3] * sd[3]) - (sd[4] * sd[4]) - (sd[5] * sd[5] );
  c= -( sd[0]*sd[1]*sd[2] + 2* sd[3]*sd[4]*sd[5] - sd[0]*sd[4]*sd[4] - sd[1]*sd[5]*sd[5] - sd[2]*sd[3]*sd[3] );
#if TEST1  
  printf ("xx:%.12f yy:%.12f zz:%.12f xy:%.12f yz:%.12f zx:%.12f\n", (double)sd[0], (double)sd[1], (double)sd[2], (double)sd[3], (double)sd[4], (double)sd[5]);
  printf ("a :%e b :%e c :%e\n" , (double)a,(double)b,(double)c );
#endif

  /* if all principals 0 */
  if ( ( abs(a)<= MIN )&&( abs(b)<= MIN )&&( abs(c)<= MIN ) )
  {
    p[0]=0.;
    p[1]=0.;
    p[2]=0.;
    a0[0]=0.;
    a0[1]=0.;
    a0[2]=1.;
    a1[0]=0.;
    a1[1]=1.;
    a1[2]=0.;
    a2[0]=-1.;
    a2[1]=0.;
    a2[2]=0.;
    return (1);
  }
  /* if just one principal is != 0 */
  else if ( ( abs(b)<MIN )&&( abs(c)<MIN ) )
  {
    p_[0]=0.;
    p_[1]=0.;
    p_[2]=0.;
    if (flag)
    {
      p_[0]=-a;
    }
    else
    {
      if(-a<0) p_[2]=-a;
      else     p_[0]=-a;
    }
  }
  /* wenn die Zugspannungen gleich sind, dann haben wir einen hydrostatischen Spannungszustand */
  else if(( abs((sd[0]-sd[1])/mean)<MIN3) &&  (abs((sd[0]-sd[2])/mean)<MIN3)  )
  {
    /* die Schubspannungen muessen praktisch 0 sein verglichen mit den Zugspannungen */
    if( (abs(sd[3]/mean)<MIN3) &&  (abs(sd[4]/mean)<MIN3) &&  (abs(sd[5]/mean)<MIN3)  )
    {
      a=0; for(i=0; i<6; i++) a+=sd[i]; a*=0.33333333;
      p[0]=a;
      p[1]=a;
      p[2]=a;
      a0[0]=0.;
      a0[1]=0.;
      a0[2]=1.;
      a1[0]=0.;
      a1[1]=1.;
      a1[2]=0.;
      a2[0]=-1.;
      a2[1]=0.;
      a2[2]=0.;
      return (1);
    }
    if ( gl3grades( a, b, c, p_ ) < 0 )  return (-2);
  }
  else if ( gl3grades( a, b, c, p_ ) < 0 )  return (-2);

  if (flag > 0)
  {
    y[0]=abs(p_[0]);
    y[1]=abs(p_[1]);
    y[2]=abs(p_[2]);
  }
  else
  {
    y[0]=p_[0];
    y[1]=p_[1];
    y[2]=p_[2];
  }

  if ( (y[0]>=y[1]) && (y[0]>=y[2]) ) /* 1st greatest */
  {
    p[0]=p_[0];
    if (y[1]>y[2])
    {
      p[1]=p_[1];
      p[2]=p_[2];
    }
    else
    {
      p[1]=p_[2];
      p[2]=p_[1];
    }
  }
  else if ( (y[1]>=y[0]) && (y[1]>=y[2]) ) /* 2nt greatest */
  {
    p[0]=p_[1];
    if (y[0]>y[2])
    {
      p[1]=p_[0];
      p[2]=p_[2];
    }
    else
    {
      p[1]=p_[2];
      p[2]=p_[0];
    }
  }
  else if ( (y[2]>=y[0]) && (y[2]>=y[1]) ) /* 3rd greatest */
  {
    p[0]=p_[2];
    if (y[0]>y[1])
    {
      p[1]=p_[0];
      p[2]=p_[1];
    }
    else
    {
      p[1]=p_[1];
      p[2]=p_[0];
    }
  }
  else
  {
    printf("ERROR in calcPrinc P:%lf %lf %lf\n",y[0],y[1],y[2] );
  } 

  /* no angles if flag == 2 (fast method) */
  if (flag == 2) return(1);

  for (i=0; i<3; i++) pd[i]=p[i];
  calcPvector( sd, pd, alfa );
  a0[0]=alfa[0];
  a0[1]=alfa[1];
  a0[2]=alfa[2];
  a1[0]=alfa[3];
  a1[1]=alfa[4];
  a1[2]=alfa[5];
  a2[0]=alfa[6];
  a2[1]=alfa[7];
  a2[2]=alfa[8];

  /* kontrolle des letzten vektors mit Vektorprod.  */
  v_prod( a0, a1, p1);
  if(( (p1[0]*p1[0]-a2[0]*a2[0]) > MIN2 )|| ( (p1[1]*p1[1]-a2[1]*a2[1]) > MIN2 )||
  ( (p1[2]*p1[2]-a2[2]*a2[2]) > MIN2 ))
  {
#if TEST
      printf(" WARNING: in calcPrinc, some directions of maxPrinc in error \n");
      printf("  p1: %lf p2: %lf p3: %lf \n", p[0], p[1], p[2] );
      printf("  v1x: %lf v2x: %lf v3x: %lf v1xv2_x: %lf \n", a0[0], a1[0], a2[0], p1[0] );
      printf("  v1y: %lf v2y: %lf v3y: %lf v1xv2_y: %lf \n", a0[1], a1[1], a2[1], p1[1] );
      printf("  v1z: %lf v2z: %lf v3z: %lf v1xv2_z: %lf \n", a0[2], a1[2], a2[2], p1[2] );
      printf("  v1r: %lf v2r: %lf v3r: %lf v1xv2_r: %lf \n", sqrt(a0[0]*a0[0]+a0[1]*a0[1]+a0[2]*a0[2]) ,
        sqrt(a1[0]*a1[0]+a1[1]*a1[1]+a1[2]*a1[2]), sqrt(a2[0]*a2[0]+a2[1]*a2[1]+a2[2]*a2[2]) ,
        sqrt(p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2]) );
#endif
    return (-1);
  }
  return(1);
}
