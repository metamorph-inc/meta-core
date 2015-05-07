/* ---------------------------------------------------------------- */
/* berechnet die eigenvectoren                                      */
/* ---------------------------------------------------------------- */

#include <extUtil.h>

#define     MIN          1.e-33
#define     pi          3.14159265358979323846264338327950288

int calcPvector( long double *s, long double *p, double *a )
{
  long double D[3];
  long double max, maxD=-1.;
  double  al, v1[3],v2[3],v3[3];
  int i,j, max_indx, iflag=0, maxDi=-1;

  for(i=0; i<3; i++)
  {
    /* berechnung der drei determinanten fuer die faelle x=1, y=1, z=1 */
    D[0]=((s[1]-p[i])*(s[2]-p[i]))-(s[4]*s[4]);
    D[1]=((s[0]-p[i])*(s[2]-p[i]))-(s[5]*s[5]);
    D[2]=((s[0]-p[i])*(s[1]-p[i]))-(s[3]*s[3]);

    /* bestimme die maximale determinante und den index */
    max=0.; max_indx=-1;
    for (j=0; j<3; j++) if(D[j]*D[j]>max) { max=D[j]*D[j]; max_indx=j; }

    /* bestimme den index der maximalen determinante aller Richtungen */ 
    if(max>maxD) { maxD=max; maxDi=i; }
    
    /* printf(" maxD:%e maxDi:%d D: %e %e %e max:%e indx:%d \n"
	   ,(double)maxD, maxDi, (double)D[0], (double)D[1],(double)D[2], (double)max,max_indx);
    */
    if(max<MIN)
    {
      iflag=1;
    }
    else if(max_indx==0)
    {
      a[0+(i*3)]=1.;
      a[1+(i*3)]=( (-s[3]*(s[2]-p[i])) - (-s[5]*s[4]) ) / D[max_indx];
      a[2+(i*3)]=( ((s[1]-p[i])*-s[5]) - (s[4]*(-s[3])) ) / D[max_indx];
    }
    else if(max_indx==1)
    {
      a[0+(i*3)]=( (-s[3]*(s[2]-p[i])) - (-s[4]*s[5]) ) / D[max_indx];
      a[1+(i*3)]=1.;
      a[2+(i*3)]=( ((s[0]-p[i])*-s[4]) - (s[5]*(-s[3])) ) / D[max_indx];
    }
    else if(max_indx==2)
    {
      a[0+(i*3)]=( (-s[5]*(s[1]-p[i])) - (-s[4]*s[3]) ) / D[max_indx];
      a[1+(i*3)]=( ((s[0]-p[i])*-s[4]) - (s[3]*(-s[5])) ) / D[max_indx];
      a[2+(i*3)]=1.;
    }
  }

  /* vectoren normieren */
  for (i=0; i<3; i++)
  {
    al=sqrt(a[0+(i*3)]*a[0+(i*3)]+a[1+(i*3)]*a[1+(i*3)]+a[2+(i*3)]*a[2+(i*3)]);
    a[0+(i*3)]/=al;
    a[1+(i*3)]/=al;
    a[2+(i*3)]/=al;
    /* printf (" calcPvector max_indx:%d  p[%d]=%e a:%e %e %e  %e \n"
       ,max_indx,i, (double)p[i],(double)a[0+(i*3)],(double)a[1+(i*3)],(double)a[2+(i*3)], (double)al); */
  }

  /* kontrolle ob richtungen nicht bestimmt wurden */
  if (iflag)
  {
    if(maxDi==0) 
    {
      v1[0]=a[0];
      v1[1]=a[1];
      v1[2]=a[2];
      v2[0]=a[1];
      v2[1]=a[2];
      v2[2]=a[0];
      v_prod(v1,v2,v3);
      v_prod(v1,v3,v2);
      a[3]=v2[0];
      a[4]=v2[1];
      a[5]=v2[2];
      a[6]=v3[0];
      a[7]=v3[1];
      a[8]=v3[2];
    }
    if(maxDi==1) 
    {
      v1[0]=a[3];
      v1[1]=a[4];
      v1[2]=a[5];
      v2[0]=a[4];
      v2[1]=a[5];
      v2[2]=a[3];
      v_prod(v1,v2,v3);
      v_prod(v1,v3,v2);
      a[0]=v2[0];
      a[1]=v2[1];
      a[2]=v2[2];
      a[6]=v3[0];
      a[7]=v3[1];
      a[8]=v3[2];
    }
    if(maxDi==2) 
    {
      v1[0]=a[6];
      v1[1]=a[7];
      v1[2]=a[8];
      v2[0]=a[7];
      v2[1]=a[8];
      v2[2]=a[6];
      v_prod(v1,v2,v3);
      v_prod(v1,v3,v2);
      a[0]=v2[0];
      a[1]=v2[1];
      a[2]=v2[2];
      a[3]=v3[0];
      a[4]=v3[1];
      a[5]=v3[2];
    }

    /* vectoren nochmal normieren */
    for (i=0; i<3; i++)
    {
      al=sqrt(a[0+(i*3)]*a[0+(i*3)]+a[1+(i*3)]*a[1+(i*3)]+a[2+(i*3)]*a[2+(i*3)]);
      a[0+(i*3)]/=al;
      a[1+(i*3)]/=al;
      a[2+(i*3)]/=al;
      /* printf (" calcPvector max_indx:%d  p[%d]=%lf a:%lf %lf %lf \n"
	 ,max_indx,i,p[i],a[0+(i*3)],a[1+(i*3)],a[2+(i*3)]); */
    }
  }

  return (1);
}
  
  
  
  
  
