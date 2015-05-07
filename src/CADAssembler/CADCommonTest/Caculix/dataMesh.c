/* --------------------------------------------------------------------  */
/*                          CALCULIX                                     */
/*                   - GRAPHICAL INTERFACE -                             */
/*                                                                       */
/*     A 3-dimensional pre- and post-processor for finite elements       */
/*              Copyright (C) 1996 Klaus Wittig                          */
/*                                                                       */
/*     This program is free software; you can redistribute it and/or     */
/*     modify it under the terms of the GNU General Public License as    */
/*     published by the Free Software Foundation; version 2 of           */
/*     the License.                                                      */
/*                                                                       */
/*     This program is distributed in the hope that it will be useful,   */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of    */ 
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/*     GNU General Public License for more details.                      */
/*                                                                       */
/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software       */
/*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.         */
/* --------------------------------------------------------------------  */

#include <cgx.h>

/* Display-lists */
extern GLuint list_animate_light, *list_animate;
extern Scale     scale[1];
extern char datin[MAX_LINE_LENGTH];

/* the copied node-sets which have to be filled with values from new loaded Datasets */
extern CopiedNodeSets copiedNodeSets[1];

int scalFlag=0;

void descalNodes ( int anz_n, Nodes *node, Scale *scale )
{
  int  j;
  //printf("WARNING:  descale data\n");
  if(scalFlag)
  {
   scalFlag=0;
   for (j=0; j<anz_n; j++ )
   {
    node[node[j].nr].nx = (node[node[j].nr].nx* scale->w+scale->x);
    node[node[j].nr].ny = (node[node[j].nr].ny* scale->w+scale->y);
    node[node[j].nr].nz = (node[node[j].nr].nz* scale->w+scale->z);
   }
  }
  //else{ printf("WARNING: try to descale already descaled data\n"); }
}


/* nodes scalieren wg. zb. beleuchtung!  */
void scalNodes ( int anz_n, Nodes *node, Scale *scale )
{
  int  j;

  if(!scalFlag)
  {
   scalFlag=1;
   for (j=0; j<anz_n; j++ )
   {
    node[node[j].nr].nx = (node[node[j].nr].nx-scale->x)/scale->w;
    node[node[j].nr].ny = (node[node[j].nr].ny-scale->y)/scale->w;
    node[node[j].nr].nz = (node[node[j].nr].nz-scale->z)/scale->w;
   }
  }
  //else{ printf("WARNING: try to scale already scaled data\n"); }
}


void calcDatasets( int lc, Summen *anz, Nodes *node, Datasets *lcase )
{
  int   i, j, l, comp, lcp, layers=0;
  double s[6], p[3], y[3], a1[3], a2[3], a3[3];
  double octaStress, vMises, dx,dy,dz, max_val=0., max_alfa=0., disp, alfa, dalfa, dalfa_grd=10., pi180;
  char buffer[MAX_LINE_LENGTH];

  pi180=PI/180.;

  /* in case stress-phases for shells are to be regarded its necessary how much layers exist. All STRESP must show up first */
  if(( compare( lcase[lc].name, "STRESP", 6) == 6)&&(strlen(lcase[lc].name)==8)) layers=atoi(&lcase[lc].name[6])+1;

  if( (compare( lcase[lc].name, "STRESS", 6) == 6)||(compare( lcase[lc].name, "STRAIN", 6) == 6)||( compare( lcase[lc].name, "ZZS", 3) == 3))
    // if( lcase[lc].ictype[0] == 4) /* check first comp if its a tensor (STRESS) */
  {
    /* check if the STRESS has six valid components */
    if(lcase[lc].ncomps < 6 ) goto jumpStresses;
    for (i=1; i<6; i++) if(lcase[lc].ictype[i] != 4 ) goto jumpStresses;
    if((compare(&lcase[lc].compName[0][1], "XX", 2) < 2 ) && (compare(&lcase[lc].compName[0][1], "RR", 2) < 2 )) goto jumpStresses;
    if((compare(&lcase[lc].compName[1][1], "YY", 2) < 2 ) && (compare(&lcase[lc].compName[1][1], "ZZ", 2) < 2 )) goto jumpStresses;
    if((compare(&lcase[lc].compName[2][1], "ZZ", 2) < 2 ) && (compare(&lcase[lc].compName[2][1], "TT", 2) < 2 )) goto jumpStresses;
    if((compare(&lcase[lc].compName[3][1], "XY", 2) < 2 ) && (compare(&lcase[lc].compName[3][1], "YX", 2) < 2 ) && (compare(&lcase[lc].compName[3][1], "RZ", 2) < 2 )) goto jumpStresses;
    if((compare(&lcase[lc].compName[4][1], "YZ", 2) < 2 ) && (compare(&lcase[lc].compName[4][1], "ZY", 2) < 2 ) && (compare(&lcase[lc].compName[4][1], "ZT", 2) < 2 )) goto jumpStresses;
    if((compare(&lcase[lc].compName[5][1], "ZX", 2) < 2 ) && (compare(&lcase[lc].compName[5][1], "XZ", 2) < 2 ) && (compare(&lcase[lc].compName[5][1], "TR", 2) < 2 )) goto jumpStresses;


    /* check if related stress-phase-angles are available. */
    lcp=0;
    sprintf(buffer, "STRESP%c%c", lcase[lc].name[6], lcase[lc].name[7]);
    if((lc>=layers)&&( compare( lcase[lc-layers].name, buffer, strlen(buffer)) == strlen(buffer))&&(lcase[lc-layers].value==lcase[lc].value)) lcp=-layers;

    if(lcp!=0) printf("calculate additional values for %s regarding %s\n", lcase[lc].name, lcase[lc+lcp].name);

    comp=lcase[lc].ncomps;
    lcase[lc].ncomps+=10;
    lcase[lc].irtype=1;

    if ( (lcase[lc].nmax = (int *)realloc( (int *)lcase[lc].nmax, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].nmin = (int *)realloc( (int *)lcase[lc].nmin, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].max = (float *)realloc((float *)lcase[lc].max, lcase[lc].ncomps * sizeof(float))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].min = (float *)realloc((float *)lcase[lc].min, lcase[lc].ncomps * sizeof(float))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].dat = (float **)realloc((float **)lcase[lc].dat, lcase[lc].ncomps * sizeof(float *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].compName = (char **)realloc((char **)lcase[lc].compName, lcase[lc].ncomps * sizeof(char *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icname = (char **)realloc((char **)lcase[lc].icname, lcase[lc].ncomps * sizeof(char *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].ictype = (int *)realloc((int *)lcase[lc].ictype, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icind1 = (int *)realloc((int *)lcase[lc].icind1, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icind2 = (int *)realloc((int *)lcase[lc].icind2, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].iexist = (int *)realloc((int *)lcase[lc].iexist, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );

    for(i=comp; i<lcase[lc].ncomps; i++)
    {
      if ( (lcase[lc].dat[i] = (float *)malloc( (anz->nmax+1) * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );	               
      if ( (lcase[lc].compName[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
        printf("\n\n ERROR: malloc failed\n\n" );
      if ( (lcase[lc].icname[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
        printf("\n\n ERROR: malloc failed\n\n" );
      lcase[lc].max[i]=-MAX_INTEGER;
      lcase[lc].min[i]=MAX_INTEGER;
    }

    /* calculate von Mises Stress  */

    strcpy ( lcase[lc].compName[comp], "Mises   ");
    lcase[lc].ictype[comp] = 1;
    lcase[lc].icind1[comp] = 0;
    lcase[lc].icind2[comp] = 0;
    lcase[lc].iexist[comp] = 1;

    for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      if(lcp!=0)
      {
        /* go over 180 degree in coarse steps */
        max_val=alfa=0.;
        dalfa=dalfa_grd*pi180;
        for ( l=0; l<PI/dalfa; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          s[2]=lcase[lc].dat[2][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[2][node[i].nr]);
          s[3]=lcase[lc].dat[3][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[3][node[i].nr]);
          s[4]=lcase[lc].dat[4][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[4][node[i].nr]);
          s[5]=lcase[lc].dat[5][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[5][node[i].nr]);
          octaStress=
            1./3. *sqrt( ((s[0]-s[1])*(s[0]-s[1])) +((s[1]-s[2])*(s[1]-s[2])) +((s[2]-s[0])*(s[2]-s[0])) +(6.*s[4]*s[4]) +(6.*s[5]*s[5]) +(6.*s[3]*s[3]) );
          vMises = (3./1.4142136) * octaStress;  
          if(vMises>max_val) { max_val=vMises; max_alfa=alfa; }
          /* if(node[i].nr==87) printf("1vMises[%d]:%f  max_val:%f alfa:%f max_alfa=%f\n", l, vMises,max_val,alfa,max_alfa); */
        }
        /* go again over the values close to the maximum in 2 degree steps */
        max_val=0.;
        alfa=max_alfa-dalfa;
        dalfa/=dalfa_grd*.5;
        for ( l=0; l<dalfa_grd; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          s[2]=lcase[lc].dat[2][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[2][node[i].nr]);
          s[3]=lcase[lc].dat[3][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[3][node[i].nr]);
          s[4]=lcase[lc].dat[4][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[4][node[i].nr]);
          s[5]=lcase[lc].dat[5][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[5][node[i].nr]);
          octaStress=
            1./3. *sqrt( ((s[0]-s[1])*(s[0]-s[1])) +((s[1]-s[2])*(s[1]-s[2])) +((s[2]-s[0])*(s[2]-s[0])) +(6.*s[4]*s[4]) +(6.*s[5]*s[5]) +(6.*s[3]*s[3]) );
          vMises = (3./1.4142136) * octaStress;  
          if(vMises>max_val) max_val=vMises;
        }
        lcase[lc].dat[comp][node[i].nr]=max_val;
      }
      else
      {
        s[0]=lcase[lc].dat[0][node[i].nr];
        s[1]=lcase[lc].dat[1][node[i].nr];
        s[2]=lcase[lc].dat[2][node[i].nr];
        s[3]=lcase[lc].dat[3][node[i].nr];
        s[4]=lcase[lc].dat[4][node[i].nr];
        s[5]=lcase[lc].dat[5][node[i].nr];
        octaStress=
          1./3. *sqrt( ((s[0]-s[1])*(s[0]-s[1])) +((s[1]-s[2])*(s[1]-s[2])) +((s[2]-s[0])*(s[2]-s[0])) +(6.*s[4]*s[4]) +(6.*s[5]*s[5]) +(6.*s[3]*s[3]) );
        vMises = (3./1.4142136) * octaStress;  
        lcase[lc].dat[comp][node[i].nr]  = vMises;
      }
    }

    /* calculate horizontal Stress xx+yy */
    j=comp+1;
    strcpy ( lcase[lc].compName[j], "xx+yy   ");
    lcase[lc].ictype[j] = 1;
    lcase[lc].icind1[j] = 0;
    lcase[lc].icind2[j] = 0;
    lcase[lc].iexist[j] = 1;

    for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      if(lcp!=0)
      {
        /* go over 180 degree in coarse steps */
        max_val=alfa=0.;
        dalfa=dalfa_grd*pi180;
        for ( l=0; l<PI/dalfa; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          dx=s[0]+s[1];
          if((dx*dx)>max_val) { max_val=(dx*dx); max_alfa=alfa; }
          /* if(node[i].nr==21607) printf("1dx[%d]:%f  max_val:%f alfa:%f max_alfa=%f\n", l, dx,max_val,alfa,max_alfa); */
        }
        /* go again over the values close to the maximum in 2 degree steps */
        max_val=0.;
        alfa=max_alfa-dalfa;
        dalfa/=dalfa_grd*.5;
        for ( l=0; l<dalfa_grd; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          dx=s[0]+s[1];
          if((dx*dx)>max_val) max_val=(dx*dx);
        }
        lcase[lc].dat[j][node[i].nr]=sqrt(max_val);
      }
      else
      {
        lcase[lc].dat[j][node[i].nr]=lcase[lc].dat[0][node[i].nr]+lcase[lc].dat[1][node[i].nr];
      }
    }

    /* calculate maximum Principal  */

    if(compare( lcase[lc].name, "STRESS", 6) == 6)
    {
    strcpy ( lcase[lc].compName[comp+2], "PS1     ");
    strcpy ( lcase[lc].compName[comp+3], "PS2     ");
    strcpy ( lcase[lc].compName[comp+4], "PS3     ");
    strcpy ( lcase[lc].compName[comp+5], "worstPSx");
    strcpy ( lcase[lc].compName[comp+6], "worstPSy");
    strcpy ( lcase[lc].compName[comp+7], "worstPSz");
    strcpy ( lcase[lc].compName[comp+8], "worstPS ");
    strcpy ( lcase[lc].compName[comp+9], "Tresca ");
    }
    else if(compare( lcase[lc].name, "STRAIN", 6) == 6)
    {
    strcpy ( lcase[lc].compName[comp+2], "PE1     ");
    strcpy ( lcase[lc].compName[comp+3], "PE2     ");
    strcpy ( lcase[lc].compName[comp+4], "PE3     ");
    strcpy ( lcase[lc].compName[comp+5], "worstPEx");
    strcpy ( lcase[lc].compName[comp+6], "worstPEy");
    strcpy ( lcase[lc].compName[comp+7], "worstPEz");
    strcpy ( lcase[lc].compName[comp+8], "worstPE ");
    strcpy ( lcase[lc].compName[comp+9], "Tresca ");
    }
    else 
    {
    strcpy ( lcase[lc].compName[comp+2], "P1      ");
    strcpy ( lcase[lc].compName[comp+3], "P2      ");
    strcpy ( lcase[lc].compName[comp+4], "P3      ");
    strcpy ( lcase[lc].compName[comp+5], "worstPx ");
    strcpy ( lcase[lc].compName[comp+6], "worstPy ");
    strcpy ( lcase[lc].compName[comp+7], "worstPz ");
    strcpy ( lcase[lc].compName[comp+8], "worstP  ");
    strcpy ( lcase[lc].compName[comp+9], "Tresca ");
    }

    lcase[lc].ictype[comp+2] = 1;
    lcase[lc].icind1[comp+2] = 0;
    lcase[lc].icind2[comp+2] = 0;
    lcase[lc].iexist[comp+2] = 1;
    lcase[lc].ictype[comp+3] = 1;
    lcase[lc].icind1[comp+3] = 0;
    lcase[lc].icind2[comp+3] = 0;
    lcase[lc].iexist[comp+3] = 1;
    lcase[lc].ictype[comp+4] = 1;
    lcase[lc].icind1[comp+4] = 0;
    lcase[lc].icind2[comp+4] = 0;
    lcase[lc].iexist[comp+4] = 1;

    lcase[lc].ictype[comp+5] = 2;
    lcase[lc].icind1[comp+5] = 1;
    lcase[lc].icind2[comp+5] = 0;
    lcase[lc].iexist[comp+5] = 1;

    lcase[lc].ictype[comp+6] = 2;
    lcase[lc].icind1[comp+6] = 2;
    lcase[lc].icind2[comp+6] = 0;
    lcase[lc].iexist[comp+6] = 1;

    lcase[lc].ictype[comp+7] = 2;
    lcase[lc].icind1[comp+7] = 3;
    lcase[lc].icind2[comp+7] = 0;
    lcase[lc].iexist[comp+7] = 1;

    lcase[lc].ictype[comp+8] = 2;
    lcase[lc].icind1[comp+8] = 0;
    lcase[lc].icind2[comp+8] = 0;
    lcase[lc].iexist[comp+8] = 1;

    lcase[lc].ictype[comp+9] = 1;
    lcase[lc].icind1[comp+9] = 0;
    lcase[lc].icind2[comp+9] = 0;
    lcase[lc].iexist[comp+9] = 1;


    for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      if(lcp!=0)
      {
        /* go over 180 degree in coarse steps */
        max_val=alfa=0.;
        dalfa=dalfa_grd*pi180;
        for ( l=0; l<PI/dalfa; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          s[2]=lcase[lc].dat[2][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[2][node[i].nr]);
          s[3]=lcase[lc].dat[3][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[3][node[i].nr]);
          s[4]=lcase[lc].dat[4][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[4][node[i].nr]);
          s[5]=lcase[lc].dat[5][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[5][node[i].nr]);
          j=calcPrinc( s, p, a1, a2, a3, 2 );
          if( p[0]*p[0]>max_val) { max_val= p[0]*p[0]; max_alfa=alfa; }
          /* if(node[i].nr==87) printf("1p[%d]:%f %f %f max_val:%f alfa:%f max_alfa=%f\n", l,p[0],p[1],p[2],max_val,alfa,max_alfa); */
        }
        /* go again over the values close to the maximum in 2 degree steps */
        max_val=0.;
        alfa=max_alfa-dalfa;
        dalfa/=dalfa_grd*.5;
        for ( l=0; l<dalfa_grd; l++ )
        {
          alfa+= dalfa;
          s[0]=lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
          s[1]=lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
          s[2]=lcase[lc].dat[2][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[2][node[i].nr]);
          s[3]=lcase[lc].dat[3][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[3][node[i].nr]);
          s[4]=lcase[lc].dat[4][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[4][node[i].nr]);
          s[5]=lcase[lc].dat[5][node[i].nr]* cos(alfa+pi180*lcase[lc+lcp].dat[5][node[i].nr]);
          j=calcPrinc( s, p, a1, a2, a3, 2 );
          if( p[0]*p[0]>max_val) { max_val= p[0]*p[0]; max_alfa=alfa; }
        }
        s[0]=lcase[lc].dat[0][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[0][node[i].nr]);
        s[1]=lcase[lc].dat[1][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[1][node[i].nr]);
        s[2]=lcase[lc].dat[2][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[2][node[i].nr]);
        s[3]=lcase[lc].dat[3][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[3][node[i].nr]);
        s[4]=lcase[lc].dat[4][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[4][node[i].nr]);
        s[5]=lcase[lc].dat[5][node[i].nr]* cos(max_alfa+pi180*lcase[lc+lcp].dat[5][node[i].nr]);
      }
      else
      {
        s[0]=lcase[lc].dat[0][node[i].nr];
        s[1]=lcase[lc].dat[1][node[i].nr];
        s[2]=lcase[lc].dat[2][node[i].nr];
        s[3]=lcase[lc].dat[3][node[i].nr];
        s[4]=lcase[lc].dat[4][node[i].nr];
        s[5]=lcase[lc].dat[5][node[i].nr];
      }

      /* if flag=0  x[0]  >  x[1]  >  x[2]                              */
      /* if flag=1 |x[0]| > |x[1]| > |x[2]|                             */
      /* if flag=2 |x[0]| > |x[1]| > |x[2]| and no angle calculation    */
      j=calcPrinc( s, p, a1, a2, a3, 0 );
      if ( j == -9999 ) /* to activate this: -1 */
      {
        printf("Warning: at node:%d\n", node[i].nr );
      }
      else if ( j == -2 )
      {
        printf("Warning: at node:%d, directions and values of maxPrinc set to 0.\n", node[i].nr );
        for(j=comp+2; j<comp+6; j++) lcase[lc].dat[j][node[i].nr] = 0.;
      }
      else
      {
        lcase[lc].dat[comp+2][node[i].nr]= p[0];
        lcase[lc].dat[comp+3][node[i].nr]= p[1];
        lcase[lc].dat[comp+4][node[i].nr]= p[2];

        /* worstP */
        y[0] = p[0]*p[0];
        y[1] = p[1]*p[1];
        y[2] = p[2]*p[2];
        if ( (y[0]>=y[1]) && (y[0]>=y[2]) )
        { 
          lcase[lc].dat[comp+5][node[i].nr] = a1[0]*p[0];
          lcase[lc].dat[comp+6][node[i].nr] = a1[1]*p[0];
          lcase[lc].dat[comp+7][node[i].nr] = a1[2]*p[0];
          if((lcp!=0)&& (p[0]<0)) lcase[lc].dat[comp+8][node[i].nr]=-p[0];
          else       lcase[lc].dat[comp+8][node[i].nr] = p[0];
        } 
        else if( (y[1]>=y[0]) && (y[1]>=y[2]) )
        {        
          lcase[lc].dat[comp+5][node[i].nr] = a2[0]*p[1];
          lcase[lc].dat[comp+6][node[i].nr] = a2[1]*p[1];
          lcase[lc].dat[comp+7][node[i].nr] = a2[2]*p[1];
          if((lcp!=0)&& (p[1]<0)) lcase[lc].dat[comp+8][node[i].nr]=-p[1];
          else       lcase[lc].dat[comp+8][node[i].nr] = p[1];
        } 
        else if ( (y[2]>=y[0]) && (y[2]>=y[1]) )
        {        
          lcase[lc].dat[comp+5][node[i].nr] = a3[0]*p[2];
          lcase[lc].dat[comp+6][node[i].nr] = a3[1]*p[2];
          lcase[lc].dat[comp+7][node[i].nr] = a3[2]*p[2];
          if((lcp!=0)&& (p[2]<0)) lcase[lc].dat[comp+8][node[i].nr]=-p[2];
          else       lcase[lc].dat[comp+8][node[i].nr] = p[2];
        }
        else
        {
          printf("ERROR in calcDatasets()\n"); exit(-1);
        }

        /* Tresca */
        lcase[lc].dat[comp+9][node[i].nr]=0.5*(p[0]-p[2]);
	
      }
    }

    /* max and min */
    for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      for(j=comp; j<lcase[lc].ncomps; j++)
      {        
        if(lcase[lc].dat[j][node[i].nr] > lcase[lc].max[j])
        {
          lcase[lc].max[j]=lcase[lc].dat[j][node[i].nr];
          lcase[lc].nmax[j]=node[i].nr;
        }
        if(lcase[lc].dat[j][node[i].nr] < lcase[lc].min[j])
        {
          lcase[lc].min[j]=lcase[lc].dat[j][node[i].nr];
          lcase[lc].nmin[j]=node[i].nr;
        }
      }
    }

    jumpStresses:;
  }
  else if(( lcase[lc].ictype[0] == 2)||(lcase[lc].ictype[0]== 12)) /* check first comp if its a vector (DISP, VELO etc.) */
  {
    comp=lcase[lc].ncomps;
    lcase[lc].ncomps++;
    lcase[lc].irtype=1;

    if ( (lcase[lc].nmax = (int *)realloc( (int *)lcase[lc].nmax, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].nmin = (int *)realloc( (int *)lcase[lc].nmin, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].max = (float *)realloc((float *)lcase[lc].max, lcase[lc].ncomps * sizeof(float))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].min = (float *)realloc((float *)lcase[lc].min, lcase[lc].ncomps * sizeof(float))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].dat = (float **)realloc((float **)lcase[lc].dat, lcase[lc].ncomps * sizeof(float *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].compName = (char **)realloc((char **)lcase[lc].compName, lcase[lc].ncomps * sizeof(char *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icname = (char **)realloc((char **)lcase[lc].icname, lcase[lc].ncomps * sizeof(char *))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].ictype = (int *)realloc((int *)lcase[lc].ictype, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icind1 = (int *)realloc((int *)lcase[lc].icind1, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].icind2 = (int *)realloc((int *)lcase[lc].icind2, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );
    if ( (lcase[lc].iexist = (int *)realloc((int *)lcase[lc].iexist, lcase[lc].ncomps * sizeof(int))) == NULL )
      printf("\n\n ERROR: malloc failure\n\n" );

    for(i=comp; i<lcase[lc].ncomps; i++)
    {
      if ( (lcase[lc].dat[i] = (float *)malloc( (anz->nmax+1) * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );	               
      if ( (lcase[lc].compName[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
        printf("\n\n ERROR: malloc failed\n\n" );
      if ( (lcase[lc].icname[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
        printf("\n\n ERROR: malloc failed\n\n" );
      lcase[lc].max[i]=-MAX_INTEGER;
      lcase[lc].min[i]=MAX_INTEGER;
    }

    /* calculate total displacement  */

    strcpy ( lcase[lc].compName[comp], "ALL     ");
    lcase[lc].ictype[comp] = 2;
    lcase[lc].icind1[comp] = 0;
    lcase[lc].icind2[comp] = 0;
    lcase[lc].iexist[comp] = 1;
    strcpy(lcase[lc].icname[comp], "ALL");

    pi180=PI/180.;
    dalfa=3.*pi180;
    for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      if (lcase[lc].ictype[0]== 12)
	{
        max_val=alfa=0.;
        for ( l=0; l<60; l++ )
        {
          alfa+= dalfa;
          dx=(lcase[lc].dat[0][node[i].nr]* cos(alfa+pi180*lcase[lc].dat[3][node[i].nr]));
          dy=(lcase[lc].dat[1][node[i].nr]* cos(alfa+pi180*lcase[lc].dat[4][node[i].nr]));
          dz=(lcase[lc].dat[2][node[i].nr]* cos(alfa+pi180*lcase[lc].dat[5][node[i].nr]));
          disp=dx*dx+dy*dy+dz*dz;
          if(disp>max_val) max_val=disp;
        }
        lcase[lc].dat[comp][node[i].nr]=sqrt(max_val);
	}
      else if (lcase[lc].ictype[0]== 2)
	{
        lcase[lc].dat[comp][node[i].nr]=sqrt(
        (lcase[lc].dat[0][node[i].nr]*lcase[lc].dat[0][node[i].nr]) +
        (lcase[lc].dat[1][node[i].nr]*lcase[lc].dat[1][node[i].nr]) +
        (lcase[lc].dat[2][node[i].nr]*lcase[lc].dat[2][node[i].nr]) );
	}
      else
	{
        lcase[lc].dat[comp][node[i].nr]=0.; 
	}

      if(lcase[lc].dat[comp][node[i].nr] > lcase[lc].max[comp])
      {
        lcase[lc].max[comp]=lcase[lc].dat[comp][node[i].nr];
        lcase[lc].nmax[comp]=node[i].nr;
      }
      if(lcase[lc].dat[comp][node[i].nr] < lcase[lc].min[comp])
      {
        lcase[lc].min[comp]=lcase[lc].dat[comp][node[i].nr];
        lcase[lc].nmin[comp]=node[i].nr;
      }

    }
  }
}


void transformDatasets( int lc, Summen *anz, Nodes *node, Datasets *lcase, char *method, char *axis )
{
  int   i, j, comp, lcp, layers=0, dim, mode=0, dir[3];
  double p1[3];
  double phi=0., r_mat[3][3], s_mat[3][3], prod_mat[3][3];
  double dx,dy,dz,du, pi180, rad,dr;
  char buffer[MAX_LINE_LENGTH];

  double er[3], et[3], ex[3]={0.,0.,0.}, vval[3];

  static int last_stressLC=-1;

  pi180=PI/180.;

  if (toupper(axis[0])=='X')      mode =1;
  else if (toupper(axis[0])=='Y') mode =2;
  else if (toupper(axis[0])=='Z') mode =3;

  if(mode==1) { dir[0]=0; dir[1]=1; dir[2]=2; }
  else if(mode==2) { dir[0]=1; dir[1]=2; dir[2]=0; }
  else if(mode==3) { dir[0]=2; dir[1]=0; dir[2]=1; }
  else { printf("ERROR: axis defined with arguments not supported so far\n"); return; }


  /* determine the mode of transformation */
  for(i=0; i<strlen(method); i++) method[i]=toupper(method[i]);
  if(compareStrings(method,"CYL")>0)
  {
    /* in case stress-phases for shells are to be regarded its necessary how much layers exist. All STRESP must show up first */
    if(( compare( lcase[lc].name, "STRESP", 6) == 6)&&(strlen(lcase[lc].name)==8)) layers=atoi(&lcase[lc].name[6])+1;
  
    if( (compare( lcase[lc].name, "STRESS", 6) == 6)||( compare( lcase[lc].name, "ZZS", 3) == 3))
      // if( lcase[lc].ictype[0] == 4) /* check first comp if its a tensor (STRESS) */
    {
      /* check if the STRESS has six valid components */
      if(lcase[lc].ncomps < 6 ) goto jumpStresses;
      for (i=1; i<6; i++) if(lcase[lc].ictype[i] != 4 ) goto jumpStresses;
      if((compare(lcase[lc].compName[0], "SXX", 3) < 3 ) && (compare(lcase[lc].compName[0], "SRR", 3) < 3 )) goto jumpStresses;
      if((compare(lcase[lc].compName[1], "SYY", 3) < 3 ) && (compare(lcase[lc].compName[1], "SZZ", 3) < 3 )) goto jumpStresses;
      if((compare(lcase[lc].compName[2], "SZZ", 3) < 3 ) && (compare(lcase[lc].compName[2], "STT", 3) < 3 )) goto jumpStresses;
      if((compare(lcase[lc].compName[3], "SXY", 3) < 3 ) && (compare(lcase[lc].compName[3], "SYX", 3) < 3 ) && (compare(lcase[lc].compName[3], "SRZ", 3) < 3 )) goto jumpStresses;
      if((compare(lcase[lc].compName[4], "SYZ", 3) < 3 ) && (compare(lcase[lc].compName[4], "SZY", 3) < 3 ) && (compare(lcase[lc].compName[4], "SZT", 3) < 3 )) goto jumpStresses;
      if((compare(lcase[lc].compName[5], "SZX", 3) < 3 ) && (compare(lcase[lc].compName[5], "SXZ", 3) < 3 ) && (compare(lcase[lc].compName[5], "STR", 3) < 3 )) goto jumpStresses;
  
  
      /* check if related stress-phase-angles are available. */
      lcp=0;
      sprintf(buffer, "STRESP%c%c", lcase[lc].name[6], lcase[lc].name[7]);
      if((lc>=layers)&&( compare( lcase[lc-layers].name, buffer, strlen(buffer)) == strlen(buffer))&&(lcase[lc-layers].value==lcase[lc].value)) lcp=-layers;
  
      if(lcp!=0) printf("calculate additional values for %s regarding %s\n", lcase[lc].name, lcase[lc+lcp].name);
  
  
      comp=16;
      if( compare( lcase[lc].name, "ZZS", 3) == 3) lcase[lc].ncomps=comp+8;
      else lcase[lc].ncomps=comp+6;
      lcase[lc].irtype=1;
  
      if ( (lcase[lc].nmax = (int *)realloc( (int *)lcase[lc].nmax, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].nmin = (int *)realloc( (int *)lcase[lc].nmin, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].max = (float *)realloc((float *)lcase[lc].max, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].min = (float *)realloc((float *)lcase[lc].min, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].dat = (float **)realloc((float **)lcase[lc].dat, lcase[lc].ncomps * sizeof(float *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].compName = (char **)realloc((char **)lcase[lc].compName, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icname = (char **)realloc((char **)lcase[lc].icname, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].ictype = (int *)realloc((int *)lcase[lc].ictype, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind1 = (int *)realloc((int *)lcase[lc].icind1, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind2 = (int *)realloc((int *)lcase[lc].icind2, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].iexist = (int *)realloc((int *)lcase[lc].iexist, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
  
      for(i=comp; i<lcase[lc].ncomps; i++)
      {
        if ( (lcase[lc].dat[i] = (float *)malloc( (anz->nmax+1) * sizeof(float))) == NULL )
          printf("\n\n ERROR: malloc failure\n\n" );	               
        if ( (lcase[lc].compName[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        if ( (lcase[lc].icname[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        lcase[lc].max[i]=-MAX_INTEGER;
        lcase[lc].min[i]=MAX_INTEGER;
      }
  
  
      /* calc stresses in cylinder-system around x */
      strcpy ( lcase[lc].compName[comp+0], "SXX   ");
      strcpy ( lcase[lc].compName[comp+1], "STT   ");
      strcpy ( lcase[lc].compName[comp+2], "SRR   ");
      strcpy ( lcase[lc].compName[comp+3], "SXT   ");
      strcpy ( lcase[lc].compName[comp+4], "STR   ");
      strcpy ( lcase[lc].compName[comp+5], "SRX   ");
      for (i=0; i<6; i++ )
      {
        lcase[lc].ictype[comp+i] = 1;
        lcase[lc].icind1[comp+i] = 0;
        lcase[lc].icind2[comp+i] = 0;
        lcase[lc].iexist[comp+i] = 1;
      }
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        /* calc the node coordinates in r,phi,x */
        p1[0] = node[node[i].nr].nx* scale->w+scale->x;
        p1[1] = node[node[i].nr].ny* scale->w+scale->y;
        p1[2] = node[node[i].nr].nz* scale->w+scale->z;
        if((p1[dir[1]]!=0.)||(p1[dir[2]]!=0.))
        {
          phi= p_angle(p1[dir[2]], p1[dir[1]]);
  
          if(mode==1)
  	{
            r_mat[0][0]=1.;
            r_mat[0][1]=0.;
            r_mat[0][2]=0.;
            r_mat[1][0]=0.;
            r_mat[1][1]=cos(phi);
            r_mat[1][2]=sin(phi);
            r_mat[2][0]=0.;
            r_mat[2][1]=-sin(phi);
            r_mat[2][2]=cos(phi);
  	}
          else if(mode==2)
  	{
            r_mat[0][0]=cos(phi);
            r_mat[0][1]=0.;
            r_mat[0][2]=-sin(phi);
            r_mat[1][0]=0.;
            r_mat[1][1]=1.;
            r_mat[1][2]=0.;
            r_mat[2][0]=sin(phi);
            r_mat[2][1]=0.;
            r_mat[2][2]=cos(phi);
  	}
          else if(mode==3)
  	{
            r_mat[0][0]=cos(phi);
            r_mat[0][1]=sin(phi);
            r_mat[0][2]=0.;
            r_mat[1][0]=-sin(phi);
            r_mat[1][1]=cos(phi);
            r_mat[1][2]=0.;
            r_mat[2][0]=0.;
            r_mat[2][1]=0.;
            r_mat[2][2]=1.;
  	}
          else { printf("ERROR: axis not implemented so far\n"); return; }
  
          s_mat[0][0]= lcase[lc].dat[0][node[i].nr];//   xx;
          s_mat[0][1]= lcase[lc].dat[3][node[i].nr];//   xy;
          s_mat[0][2]= lcase[lc].dat[5][node[i].nr];//   zx;
                     
          s_mat[1][0]= lcase[lc].dat[3][node[i].nr];//   xy;
          s_mat[1][1]= lcase[lc].dat[1][node[i].nr];//   yy;
          s_mat[1][2]= lcase[lc].dat[4][node[i].nr];//   yz;
                    
          s_mat[2][0]= lcase[lc].dat[5][node[i].nr];//   zx;
          s_mat[2][1]= lcase[lc].dat[4][node[i].nr];//   yz;
          s_mat[2][2]= lcase[lc].dat[2][node[i].nr];//   zz;
      
          dim=3;
          m_prod( &dim, &r_mat[0][0], &s_mat[0][0], &prod_mat[0][0]);
          m_prodtr( &dim, &prod_mat[0][0], &r_mat[0][0], &s_mat[0][0]);
  
          if(mode==1)
  	{
            lcase[lc].dat[comp+0][node[i].nr]  = s_mat[0][0]; //ax
            lcase[lc].dat[comp+1][node[i].nr]  = s_mat[1][1]; //tan
            lcase[lc].dat[comp+2][node[i].nr]  = s_mat[2][2]; //rad
            lcase[lc].dat[comp+3][node[i].nr]  = s_mat[1][0]; //xt
            lcase[lc].dat[comp+4][node[i].nr]  = s_mat[1][2]; //tr
            lcase[lc].dat[comp+5][node[i].nr]  = s_mat[0][2]; //rx 
  	}
          else if(mode==2)
  	{
            lcase[lc].dat[comp+0][node[i].nr]  = s_mat[1][1]; //ax 
            lcase[lc].dat[comp+1][node[i].nr]  = s_mat[2][2]; //tan
            lcase[lc].dat[comp+2][node[i].nr]  = s_mat[0][0]; //rad
            lcase[lc].dat[comp+3][node[i].nr]  = s_mat[1][2]; //xt 
            lcase[lc].dat[comp+4][node[i].nr]  = s_mat[0][2]; //tr 
            lcase[lc].dat[comp+5][node[i].nr]  = s_mat[1][0]; //rx 
  	}
          else if(mode==3)
  	{
            lcase[lc].dat[comp+0][node[i].nr]  = s_mat[2][2]; //ax 
            lcase[lc].dat[comp+1][node[i].nr]  = s_mat[0][0]; //tan
            lcase[lc].dat[comp+2][node[i].nr]  = s_mat[1][1]; //rad
            lcase[lc].dat[comp+3][node[i].nr]  = s_mat[0][2]; //xt 
            lcase[lc].dat[comp+4][node[i].nr]  = s_mat[1][0]; //tr 
            lcase[lc].dat[comp+5][node[i].nr]  = s_mat[1][2]; //rx 
  	}
        }
        else
        {
          lcase[lc].dat[comp+0][node[i].nr]  = 0;
          lcase[lc].dat[comp+1][node[i].nr]  = 0;
          lcase[lc].dat[comp+2][node[i].nr]  = 0;
          lcase[lc].dat[comp+3][node[i].nr]  = 0;
          lcase[lc].dat[comp+4][node[i].nr]  = 0;
          lcase[lc].dat[comp+5][node[i].nr]  = 0;
        }
      }
  
      /* in case its an ZZS Dataset (ccx-special-stress-calculation) add the difference for certain values */
      if(( compare( lcase[lc].name, "ZZS", 3) == 3)&&(last_stressLC>-1))
      {
        strcpy ( lcase[lc].compName[comp+6], "dMises   ");
        lcase[lc].ictype[comp+6] = 1;
        lcase[lc].icind1[comp+6] = 0;
        lcase[lc].icind2[comp+6] = 0;
        lcase[lc].iexist[comp+6] = 1;
        strcpy ( lcase[lc].compName[comp+7], "dworstPS ");
        lcase[lc].ictype[comp+7] = 1;
        lcase[lc].icind1[comp+7] = 0;
        lcase[lc].icind2[comp+7] = 0;
        lcase[lc].iexist[comp+7] = 1;
        for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
        {
          lcase[lc].dat[comp+6][node[i].nr]=  lcase[lc].dat[6][node[i].nr]- lcase[last_stressLC].dat[6][node[i].nr];
          lcase[lc].dat[comp+7][node[i].nr]=  lcase[lc].dat[14][node[i].nr]- lcase[last_stressLC].dat[14][node[i].nr];
        }
      }
  
      /* max and min */
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        for(j=comp; j<lcase[lc].ncomps; j++)
        {        
          if(lcase[lc].dat[j][node[i].nr] > lcase[lc].max[j])
          {
            lcase[lc].max[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmax[j]=node[i].nr;
          }
          if(lcase[lc].dat[j][node[i].nr] < lcase[lc].min[j])
          {
            lcase[lc].min[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmin[j]=node[i].nr;
          }
        }
      }
  
      last_stressLC=lc;
      jumpStresses:;
    }
    else if ( lcase[lc].ictype[0] == 2) /* check first comp if its a vector (DISP, VELO etc.) */
    {
      comp=3;
      lcase[lc].ncomps=comp+4;
      lcase[lc].irtype=1;
  
      if ( (lcase[lc].nmax = (int *)realloc( (int *)lcase[lc].nmax, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].nmin = (int *)realloc( (int *)lcase[lc].nmin, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].max = (float *)realloc((float *)lcase[lc].max, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].min = (float *)realloc((float *)lcase[lc].min, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].dat = (float **)realloc((float **)lcase[lc].dat, lcase[lc].ncomps * sizeof(float *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].compName = (char **)realloc((char **)lcase[lc].compName, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icname = (char **)realloc((char **)lcase[lc].icname, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].ictype = (int *)realloc((int *)lcase[lc].ictype, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind1 = (int *)realloc((int *)lcase[lc].icind1, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind2 = (int *)realloc((int *)lcase[lc].icind2, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].iexist = (int *)realloc((int *)lcase[lc].iexist, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
  
      for(i=comp; i<lcase[lc].ncomps; i++)
      {
        if ( (lcase[lc].dat[i] = (float *)malloc( (anz->nmax+1) * sizeof(float))) == NULL )
          printf("\n\n ERROR: malloc failure\n\n" );	               
        if ( (lcase[lc].compName[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        if ( (lcase[lc].icname[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        lcase[lc].max[i]=-MAX_INTEGER;
        lcase[lc].min[i]=MAX_INTEGER;
      }
  
      /* calculate rotated components  */
  
      for(i=comp; i<lcase[lc].ncomps-1; i++)
      {
        sprintf( lcase[lc].compName[i], "DCYL%d  ",i-2);
        lcase[lc].ictype[i] = 2;
        lcase[lc].icind1[i] = i-2;
        lcase[lc].icind2[i] = 0;
        lcase[lc].iexist[i] = 1;
        //strcpy(lcase[lc].icname[i], "ALL");
      }
        sprintf( lcase[lc].compName[i], "ALL     ");
        lcase[lc].ictype[i] = 2;
        lcase[lc].icind1[i] = 0;
        lcase[lc].icind2[i] = 0;
        lcase[lc].iexist[i] = 1;
 
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        /* values will be projected in r,t,x direction of each node */
        /* normalized vectors in r,t,x */
        er[0] = node[node[i].nr].nx* scale->w+scale->x;
        er[1] = node[node[i].nr].ny* scale->w+scale->y;
        er[2] = node[node[i].nr].nz* scale->w+scale->z;
        er[dir[0]]= 0.;       
        v_norm(er,er);

        ex[dir[0]]=1.;       

        v_prod(ex, er, et);
        v_norm(et,et);

        /* projection of the value-vector in the rtx direction (|vproj|=v*e/|e|). */
        vval[0] =lcase[lc].dat[0][node[i].nr];
        vval[1] =lcase[lc].dat[1][node[i].nr];
        vval[2] =lcase[lc].dat[2][node[i].nr];
        dr=v_sprod(vval,er);
        dx=v_sprod(vval,ex);
        du=v_sprod(vval,et);
  
        // printf("n:%d rtx: %e %e %e\n", node[i].nr, dr, du, dx);
        lcase[lc].dat[comp][node[i].nr]=dr;
        lcase[lc].dat[comp+1][node[i].nr]=du;
        lcase[lc].dat[comp+2][node[i].nr]=dx;
        lcase[lc].dat[comp+3][node[i].nr]=sqrt(dr*dr+dx*dx+du*du);
      }
  
      /* max and min */
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        for(j=comp; j<lcase[lc].ncomps; j++)
        {        
          if(lcase[lc].dat[j][node[i].nr] > lcase[lc].max[j])
          {
            lcase[lc].max[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmax[j]=node[i].nr;
          }
          if(lcase[lc].dat[j][node[i].nr] < lcase[lc].min[j])
          {
            lcase[lc].min[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmin[j]=node[i].nr;
          }
        }
      }
    }
  }
  else if(compareStrings(method,"REC")>0)
  {
    if ( lcase[lc].ictype[0] == 2) /* check first comp if its a vector (DISP, VELO etc.) */
    {
      comp=3;
      lcase[lc].ncomps=comp+4;
      lcase[lc].irtype=1;
  
      if ( (lcase[lc].nmax = (int *)realloc( (int *)lcase[lc].nmax, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].nmin = (int *)realloc( (int *)lcase[lc].nmin, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].max = (float *)realloc((float *)lcase[lc].max, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].min = (float *)realloc((float *)lcase[lc].min, lcase[lc].ncomps * sizeof(float))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].dat = (float **)realloc((float **)lcase[lc].dat, lcase[lc].ncomps * sizeof(float *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].compName = (char **)realloc((char **)lcase[lc].compName, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icname = (char **)realloc((char **)lcase[lc].icname, lcase[lc].ncomps * sizeof(char *))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].ictype = (int *)realloc((int *)lcase[lc].ictype, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind1 = (int *)realloc((int *)lcase[lc].icind1, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].icind2 = (int *)realloc((int *)lcase[lc].icind2, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
      if ( (lcase[lc].iexist = (int *)realloc((int *)lcase[lc].iexist, lcase[lc].ncomps * sizeof(int))) == NULL )
        printf("\n\n ERROR: malloc failure\n\n" );
  
      for(i=comp; i<lcase[lc].ncomps; i++)
      {
        if ( (lcase[lc].dat[i] = (float *)malloc( (anz->nmax+1) * sizeof(float))) == NULL )
          printf("\n\n ERROR: malloc failure\n\n" );	               
        if ( (lcase[lc].compName[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        if ( (lcase[lc].icname[i] = (char *)malloc( MAX_LINE_LENGTH * sizeof(char))) == NULL )
          printf("\n\n ERROR: malloc failed\n\n" );
        lcase[lc].max[i]=-MAX_INTEGER;
        lcase[lc].min[i]=MAX_INTEGER;
      }
  
      /* calculate rotated displacements  */
  
      for(i=comp; i<lcase[lc].ncomps-1; i++)
      {
        sprintf( lcase[lc].compName[i], "DREC%d  ",i-2);
        lcase[lc].ictype[i] = 2;
        lcase[lc].icind1[i] = i-2 ;
        lcase[lc].icind2[i] = 0;
        lcase[lc].iexist[i] = 1;
        //strcpy(lcase[lc].icname[i], "ALL");
      }
        sprintf( lcase[lc].compName[i], "ALL     ");
        lcase[lc].ictype[i] = 2;
        lcase[lc].icind1[i] = 0;
        lcase[lc].icind2[i] = 0;
        lcase[lc].iexist[i] = 1;
 
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        /* calc the node coordinates in r,phi,x */
        p1[0] = node[node[i].nr].nx* scale->w+scale->x;
        p1[1] = node[node[i].nr].ny* scale->w+scale->y;
        p1[2] = node[node[i].nr].nz* scale->w+scale->z;

        rad=sqrt(p1[dir[1]]*p1[dir[1]]+p1[dir[2]]*p1[dir[2]]);
        if(rad)
        {
          phi= p_angle(p1[dir[1]], p1[dir[2]]);
          //if(phi>PI) phi-=2*PI;
        }
        else phi=0.;

        if(mode==1)
	{
          dy=cos(phi)*lcase[lc].dat[0][node[i].nr] - sin(phi)*lcase[lc].dat[1][node[i].nr];
          dz=sin(phi)*lcase[lc].dat[0][node[i].nr] + cos(phi)*lcase[lc].dat[1][node[i].nr];
          dx=lcase[lc].dat[2][node[i].nr];
        }
        if(mode==2)
	{
          dz=cos(phi)*lcase[lc].dat[0][node[i].nr] - sin(phi)*lcase[lc].dat[1][node[i].nr];
          dx=sin(phi)*lcase[lc].dat[0][node[i].nr] + cos(phi)*lcase[lc].dat[1][node[i].nr];
          dy=lcase[lc].dat[2][node[i].nr];
        }
        if(mode==3)
	{
          dx=cos(phi)*lcase[lc].dat[0][node[i].nr] - sin(phi)*lcase[lc].dat[1][node[i].nr];
          dy=sin(phi)*lcase[lc].dat[0][node[i].nr] + cos(phi)*lcase[lc].dat[1][node[i].nr];
          dz=lcase[lc].dat[2][node[i].nr];
        }

        lcase[lc].dat[comp][node[i].nr] = dx;
        lcase[lc].dat[comp+1][node[i].nr]=dy;
        lcase[lc].dat[comp+2][node[i].nr]=dz;
        lcase[lc].dat[comp+3][node[i].nr] = sqrt(dx*dx + dy*dy +dz*dz);
      }
  
      /* max and min */
      for (i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        for(j=comp; j<lcase[lc].ncomps; j++)
        {        
          if(lcase[lc].dat[j][node[i].nr] > lcase[lc].max[j])
          {
            lcase[lc].max[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmax[j]=node[i].nr;
          }
          if(lcase[lc].dat[j][node[i].nr] < lcase[lc].min[j])
          {
            lcase[lc].min[j]=lcase[lc].dat[j][node[i].nr];
            lcase[lc].nmin[j]=node[i].nr;
          }
        }
      }
    }
  }
  else
  {
    printf("ERROR: transformation %s not known\n", method);
    return;
  }

}


void calcAnimation( int anim_steps, double anim_faktor, int *anim_alfa, int halfperiode, int centerNode, Summen *anz, Nodes *n_anim, Nodes *node, Elements *e_enqire, Datasets *lcase, int lc, Scale *scale, char surfFlag )
{
  int  i,l,n, n1, n2;
  double dwmax, amplitude, dalfa, alfa;

  if (lcase[lc].ictype[0]== 2)
  {
    /* feld fuer Bildbeschriftung  */
    dalfa=2.*PI/anim_steps;
    alfa=0;
    for ( l=0; l<anim_steps; l++ )
    {
      anim_alfa[l]=sin(alfa) *100;
      alfa+= dalfa;
    }

    /* scalierungsfaktor  */
    if (!halfperiode)
    {
      dwmax=lcase[lc].max[0];
      if (dwmax < lcase[lc].max[1]) { dwmax=lcase[lc].max[1];}
      if (dwmax < lcase[lc].max[2]) { dwmax=lcase[lc].max[2];}
      if (dwmax < -lcase[lc].min[0]) { dwmax=-lcase[lc].min[0];}
      if (dwmax < -lcase[lc].min[1]) { dwmax=-lcase[lc].min[1];}
      if (dwmax < -lcase[lc].min[2]) { dwmax=-lcase[lc].min[2];}
      dwmax*=20.;
      dwmax=1./dwmax;
    }
    else dwmax=1./scale->w;

    /* anlegen der Displaylisten von pi/2 bis pi*3/4  */
    alfa = PI/2.;
    for ( l=anim_steps/4+1; l<anim_steps*3/4+2; l++ )
    {
      amplitude= anim_faktor * sin(alfa) * dwmax;
      printf (" phi:%lf amplitude:%lf\n", alfa*180./PI, amplitude );

      addDispToNodes(anz, n_anim, node, e_enqire, lcase, lc, amplitude);

      /* wenn ein centernode existiert (centerNode!=0), fixiere diesen */
      if(centerNode)
        for ( i=0; i<anz->n; i++ )
        {
          n_anim[node[i].nr].nx-= lcase[lc].dat[0][centerNode] * amplitude;
          n_anim[node[i].nr].ny-= lcase[lc].dat[1][centerNode] * amplitude;
          n_anim[node[i].nr].nz-= lcase[lc].dat[2][centerNode] * amplitude;
        }

      if(surfFlag)  drawDispList( (list_animate_light+l), 'f', n_anim);
      else          drawDispList( (list_animate_light+l), 'e', n_anim);     

      alfa+= dalfa;
    }

    /* anlegen des Displaylistenfeldes ueber eine volle Periode  */
    if (!halfperiode)
    {
      for ( l=0; l<anim_steps*1/4; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+(anim_steps*2/4+1) -l ;
      }
      for ( l=anim_steps/4; l<anim_steps*3/4+1; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+l+1;
      }
      for ( l=anim_steps*3/4+1; l<anim_steps; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+(anim_steps*3/4+1) -(l-(anim_steps*3/4));
      }
    }
    else
    {
      for ( l=0; l<anim_steps*1/4; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+(anim_steps*2/4+1) -l ;
      }
      for ( l=anim_steps/4; l<anim_steps*1/2+1; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+l+1;
      }
      for ( l=anim_steps*2/4+1; l<anim_steps; l++ )
      {
        if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
          printf("\n\n ERROR: realloc failure\n\n" );
        list_animate[l] = list_animate_light+anim_steps*1/2+1;
      }
    }
  }
  else if (lcase[lc].ictype[0]== 12)
  {
    /* feld fuer Bildbeschriftung  */
    dalfa=2.*PI/anim_steps;
    alfa=0.;
    for ( l=0; l<anim_steps; l++ )
    {
      anim_alfa[l]=sin(alfa) *100;
      alfa+= dalfa;
    }

    /* scalierungsfaktor  */
    if (!halfperiode)
    {
      dwmax=lcase[lc].max[0];
      if (dwmax < lcase[lc].max[1]){ dwmax=lcase[lc].max[1];}
      if (dwmax < lcase[lc].max[2]){ dwmax=lcase[lc].max[2];}
      if (dwmax < -lcase[lc].min[0]){ dwmax=-lcase[lc].min[0];}
      if (dwmax < -lcase[lc].min[1]){ dwmax=-lcase[lc].min[1];}
      if (dwmax < -lcase[lc].min[2]){ dwmax=-lcase[lc].min[2];}
      dwmax*=20.;
      dwmax =1./dwmax;
    }
    else dwmax=1./scale->w;

    /* anlegen der Displaylisten von 0 bis 2*pi  */
    alfa=-dalfa;
    for ( l=0; l<=anim_steps; l++ )
    {
      alfa+= dalfa;
      amplitude= anim_faktor * cos(alfa) * dwmax;
      printf (" phi:%lf amplitude:%lf \n", alfa*180./PI, amplitude);

      for ( i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
      {
        n_anim[node[i].nr].nx = node[node[i].nr].nx + (lcase[lc].dat[0][node[i].nr] *
                                anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][node[i].nr]) * dwmax);
        n_anim[node[i].nr].ny = node[node[i].nr].ny + (lcase[lc].dat[1][node[i].nr] *
                                anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][node[i].nr]) * dwmax);
        n_anim[node[i].nr].nz = node[node[i].nr].nz + (lcase[lc].dat[2][node[i].nr] *
                                anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][node[i].nr]) * dwmax);
      }

      for ( i=0; i<anz->e; i++ )
      {
        if(e_enqire[e_enqire[i].nr].type==4)
        {
          for (n=0; n<3; n++)  /* create new nodes in center of areas */
          {
          n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[5+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 5+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 8+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[13+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[13+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[13+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[16+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[16+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[16+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax   ) ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[5+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 5+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax  )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 8+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[13+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[13+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[13+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[16+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[16+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[16+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax    ) ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[5+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 5+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax  )  +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 8+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[13+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[13+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[13+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[16+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[16+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[16+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax    ) ;
          }
  
          /* create  new node in center of area4 */
          n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[7]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[11]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[12]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[19]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[19]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[19]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[15]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[15]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[15]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[7]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax  )    +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[11]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[12]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[19]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[19]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[19]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[15]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[15]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[15]]) * dwmax     ); 
  
          n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[7]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax    )  +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[11]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[12]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[19]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[19]] *
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[19]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[15]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[15]] * 
                  anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[15]]) * dwmax    ); 
  
          for (n=0; n<2; n++)  /* create last 2 new nodes in center of areas */
          {
            n1=n*4;
            n2=n*8;
          n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[1+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 1+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[2+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 2+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[3+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3+n1]]) * dwmax )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n2]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 8+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[9+n2]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 9+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[10+n2]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[10+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[11+n2]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[11+n2]]) * dwmax  );
  
          n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[1+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 1+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[2+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 2+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[3+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3+n1]]) * dwmax  )  +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n2]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 8+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[9+n2]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 9+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 9+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[10+n2]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[10+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[10+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[11+n2]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[11+n2]]) * dwmax   );
  
          n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[1+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 1+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[2+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 2+n1]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[3+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3+n1]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3+n1]]) * dwmax   ) +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[8+n2]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 8+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[9+n2]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 9+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 9+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[10+n2]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[10+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[10+n2]]) * dwmax    +
            node[e_enqire[e_enqire[i].nr].nod[11+n2]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11+n2]] *
                     anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[11+n2]]) * dwmax   );
          }
        }
        if(e_enqire[e_enqire[i].nr].type==5)
        {
          for (n=0; n<2; n++)  /* create new nodes in center of areas */
          {
          n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3+n]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[6+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 6+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[10+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[10+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 9+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 9+n]]) * dwmax   ) ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3+n]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[6+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 6+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 6+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[10+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[10+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[10+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 9+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 9+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 9+n]]) * dwmax   ) ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 1+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[4+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 4+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3+n]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[6+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 6+n]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 6+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[10+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[10+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[10+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[12+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[12+n]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 9+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 9+n]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 9+n]]) * dwmax   ) ;
          }
  
          /* create  new node in center of area3 */
          n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[2]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 9]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 9]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[11]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[2]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 9]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 9]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 9]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[11]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[2]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 9]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 9]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 9]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[11]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[11]]) * dwmax   )  ;
  
          /* create  new node in center of area4 */
          n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[2]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[1]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 7]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 6]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 0]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[2]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[1]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 7]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 6]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 6]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 0]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[2]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[1]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[ 8]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 8]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 7]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 6]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 6]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 0]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax   )  ;
  
          /* create  new node in center of area5 */
          n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[12]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[13]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[13]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[13]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 3]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[12]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[13]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[13]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[13]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 3]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[4]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[5]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )   +  
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[12]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[12]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[13]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[13]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[13]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[14]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[14]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[14]]) * dwmax      +  
            node[e_enqire[e_enqire[i].nr].nod[ 3]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax   )  ;
        }
        if(e_enqire[e_enqire[i].nr].type==10)
        {
          n_anim[e_enqire[e_enqire[i].nr].nod[ 8]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[2]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 5]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   ) ;

          n_anim[e_enqire[e_enqire[i].nr].nod[ 8]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[2]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 6]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 5]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   ) ;

          n_anim[e_enqire[e_enqire[i].nr].nod[ 8]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 0]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[1]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 1]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[3]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 3]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[2]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 2]]) * dwmax   )   +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] *
                   anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 4]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 6]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 6]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 7]]) * dwmax     +
            node[e_enqire[e_enqire[i].nr].nod[ 5]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]]*
                    anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][e_enqire[e_enqire[i].nr].nod[ 5]]) * dwmax   ) ;
	}  
      }

      /* wenn ein centernode existiert (centerNode!=0), fixiere diesen */
      if(centerNode)
      {
        for ( i=0; i<anz->n; i++ )
        {
          n_anim[node[i].nr].nx-= lcase[lc].dat[0][centerNode] *
           anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[3][centerNode]) * dwmax ;
          n_anim[node[i].nr].ny-= lcase[lc].dat[1][centerNode] *
           anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[4][centerNode]) * dwmax;
          n_anim[node[i].nr].nz-= lcase[lc].dat[2][centerNode] *
           anim_faktor * cos(alfa+PI/180.*lcase[lc].dat[5][centerNode]) * dwmax;
        }
      }

      if(surfFlag)  drawDispList( (list_animate_light+l), 'f', n_anim);
      else          drawDispList( (list_animate_light+l), 'e', n_anim);     

      if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
        printf("\n\n ERROR: realloc failure\n\n" );
      list_animate[l] = list_animate_light+l+1;
    }
  }
  else
    errMsg ("\n ERROR: Animation only for displacements!\n");
}



void calcSequence( DsSequence dsSequence, double anim_faktor, int halfperiode, int centerNode, Summen *anz, Nodes *n_anim, Nodes *node, Elements *e_enqire, Datasets *lcase, Scale *scale )
{
  int  i,l,n, n1, n2, s, lc;
  double dwmax, amplitude;

  /* load missing data */
  printf (" please wait, loading data\n");
  dwmax=scale->w;
  for(s=0; s<dsSequence.nds; s++)
  {
    lc=dsSequence.ds[s];
    if (!lcase[lc].loaded)
    {
      if( pre_readfrdblock(copiedNodeSets , lc, anz, node, lcase )==-1) 
      {
        printf("ERROR in calcSequence: Could not read data for Dataset:%d\n", lc+1); 
        return;
      }
      calcDatasets( lc, anz, node, lcase );
    }

    /* scalierungsfaktor  */
    if (!halfperiode)
    {
      dwmax=lcase[lc].max[0];
      if (dwmax <  lcase[lc].max[1]){ dwmax=lcase[lc].max[1];}
      if (dwmax <  lcase[lc].max[2]){ dwmax=lcase[lc].max[2];}
      if (dwmax < -lcase[lc].min[0]){ dwmax=-lcase[lc].min[0];}
      if (dwmax < -lcase[lc].min[1]){ dwmax=-lcase[lc].min[1];}
      if (dwmax < -lcase[lc].min[2]){ dwmax=-lcase[lc].min[2];}
      dwmax*=20.;
    }
  }

  amplitude= anim_faktor/dwmax;

  l=0;
  for(s=0; s<dsSequence.nds; s++)
  {
    lc=dsSequence.ds[s];
    printf (" disp-list:%d ds:%d time:%lf text:%s\n", l+1, lc+1, lcase[lc].value, lcase[lc].dataset_text );

    for ( i=0; i<anz->n; i++ ) if(!node[node[i].nr].pflag)
    {
      n_anim[node[i].nr].nx = node[node[i].nr].nx + lcase[lc].dat[0][node[i].nr] * amplitude;
      n_anim[node[i].nr].ny = node[node[i].nr].ny + lcase[lc].dat[1][node[i].nr] * amplitude;
      n_anim[node[i].nr].nz = node[node[i].nr].nz + lcase[lc].dat[2][node[i].nr] * amplitude;
    }

    for ( i=0; i<anz->e; i++ )
    {
      if(e_enqire[e_enqire[i].nr].type==4)
      {
        for (n=0; n<3; n++)  /* create new nodes in center of areas */
        {
        n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[5+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude  )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[13+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[13+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[16+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[16+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude    ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[5+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude  )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[13+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[13+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[16+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[16+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude    ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[20+n]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[5+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8+n]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[13+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[13+n]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[16+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[16+n]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude   ) ;
        }

        /* create  new node in center of area4 */
        n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[7]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[11]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[12]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[19]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[19]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[15]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[15]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[7]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude)  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[11]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[12]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[19]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[19]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[15]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[15]] * amplitude   ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[23  ]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[7]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude)  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[11]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[12]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[19]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[19]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[15]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[15]] * amplitude   ) ;
        for (n=0; n<2; n++)  /* create last 2 new nodes in center of areas */
        {
          n1=n*4;
          n2=n*8;
        n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3+n1]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n1]] * amplitude)  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n2]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[9+n2]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[10+n2]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11+n2]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11+n2]] * amplitude   ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3+n1]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3+n1]] * amplitude)  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n2]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[9+n2]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 9+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[10+n2]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[10+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11+n2]].ny+ lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11+n2]] * amplitude   ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[24+n]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2+n1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3+n1]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3+n1]] * amplitude)  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[8+n2]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[9+n2]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 9+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[10+n2]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[10+n2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11+n2]].nz+ lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11+n2]] * amplitude   ) ;
        }
      }
      if(e_enqire[e_enqire[i].nr].type==5)
      {
        for (n=0; n<2; n++)  /* create new nodes in center of areas */
        {
        n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[3+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n]] * amplitude  )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[6+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[10+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[ 9+n]].nx+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n]] * amplitude    ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[3+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n]] * amplitude  )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[6+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[10+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[ 9+n]].ny+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n]] * amplitude    ) ;

        n_anim[e_enqire[e_enqire[i].nr].nod[15+n]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[1+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[4+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[3+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3+n]] * amplitude  )  +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[6+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6+n]] * amplitude     +
          node[e_enqire[e_enqire[i].nr].nod[10+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[10+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[12+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12+n]] * amplitude    +
          node[e_enqire[e_enqire[i].nr].nod[ 9+n]].nz+ lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9+n]] * amplitude    ) ;

        }

        /* create  new node in center of area3 */
        n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[2]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 9]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 9]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[2]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 9]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 9]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[17  ]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[2]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 9]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 9]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[11]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[11]] * amplitude   )  ;

        /* create  new node in center of area4 */
        n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 7]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 6]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 0]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 7]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 6]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 0]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[18  ]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[2]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[1]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[ 8]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 8]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 7]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 6]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 0]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   )  ;

        /* create  new node in center of area5 */
        n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].nx = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[12]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[13]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[13]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 3]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].ny = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[12]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[13]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[13]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 3]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   )  ;

        n_anim[e_enqire[e_enqire[i].nr].nod[19  ]].nz = -0.25* (
          node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[4]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[5]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude ) +
          0.5*(
          node[e_enqire[e_enqire[i].nr].nod[12]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[12]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[13]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[13]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[14]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[14]] * amplitude   +
          node[e_enqire[e_enqire[i].nr].nod[ 3]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   )  ;
      }
      if(e_enqire[e_enqire[i].nr].type==10)
        {
          n_anim[e_enqire[e_enqire[i].nr].nod[8   ]].nx = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[1]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[3]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[2]].nx  + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude ) +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4 ]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[5 ]].nx + lcase[lc].dat[0][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[8   ]].ny = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[1]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[3]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[2]].ny  + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude ) +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4 ]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[5 ]].ny + lcase[lc].dat[1][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   )  ;
  
          n_anim[e_enqire[e_enqire[i].nr].nod[8   ]].nz = -0.25* (
            node[e_enqire[e_enqire[i].nr].nod[0]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 0]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[1]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 1]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[3]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 3]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[2]].nz  + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 2]] * amplitude ) +
            0.5*(
            node[e_enqire[e_enqire[i].nr].nod[4 ]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 4]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[6 ]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 6]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[7 ]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 7]] * amplitude   +
            node[e_enqire[e_enqire[i].nr].nod[5 ]].nz + lcase[lc].dat[2][e_enqire[e_enqire[i].nr].nod[ 5]] * amplitude   )  ;
  
      }
    }

    /* wenn ein centernode existiert (centerNode!=0), fixiere diesen */
    if(centerNode)
      for ( i=0; i<anz->n; i++ )
      {
        n_anim[node[i].nr].nx-= lcase[lc].dat[0][centerNode] * amplitude;
        n_anim[node[i].nr].ny-= lcase[lc].dat[1][centerNode] * amplitude;
        n_anim[node[i].nr].nz-= lcase[lc].dat[2][centerNode] * amplitude;
      }

    if ( (list_animate = (GLuint *)realloc( list_animate, (l+1) * sizeof(GLuint))) == NULL )
      printf("\n\n ERROR: realloc failure\n\n" );
    list_animate[l]=list_animate_light+l;
    drawDispList( (list_animate_light+l), 'f', n_anim);

    l++; 
  }
}

