
#ifdef WIN32
  #include <windows.h>
  #undef near
  #undef max
  #undef min
  #define DEV_NULL  " "
  #define DEV_NULL2 " "
#else
  #define DEV_NULL   " >/dev/null"
  #define DEV_NULL2 " 2>/dev/null"
  #include <unistd.h>
#endif

#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GL/glx.h>

#ifdef WIN32
  #include <GL/glut.h>
  #define printf printf_fflush
  #ifdef __cplusplus
extern "C" {
  #endif
void printf_fflush(const char *fmt,...);
  #ifdef __cplusplus
}
  #endif
#else
  #include <GL/glut_cgx.h>
#endif

#define     PI          3.14159265358979323846264338327950288
#define     MAX_INTEGER 2147483647
#define     MAX_FLOAT   1.e32

#define     MAX_LINE_LENGTH 256
#define     NODES       1000000
#define     ELEMENTS    1000000

/* intpol2.c, spline.c */
#define     PNTS  10000

/* from #include "f2c.h" */
#define abs(x) ((x) >= 0 ? (x) : -(x))
#define dabs(x) (double)abs(x)
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define smin(a,b) (float)min(a,b)
#define smax(a,b) (float)max(a,b)
#define dmin(a,b) (double)min(a,b)
#define dmax(a,b) (double)max(a,b)


typedef struct {
  char  model[MAX_LINE_LENGTH]; /* model-name header*/
  char  **uheader; /* user header */
  char  **pheader; /* project header (remark: leading dataset-project-headers are stored in the related dataset!) */
  int   u;         /* number of user headers */
  int   p;         /* number of project headers */
  int   n;         /* number of nodes */
  int   e;         /* number of elements  */
  int   f;         /* number of faces */
  int   g;         /* number of edges */
  int   sets;      /* sets (groups) of entities */
  int   mats;      /* materials   */
  int   amps;      /* amplitudes  */
  int   l;         /* number of loadcases (Datasets) */
  int   b;         /* number of nodeBlocks */
  int   c;         /* number of 'cuts' over all nodeBlocks (block-to-block interfaces for isaac) */
  int   etype[100];/* number of elements of a certain type */
  int   nmax;      /* maximum nodenumber */
  int   nmin;      /* minimum nodenumber */
  int   emax;      /* maximum elemnumber */
  int   emin;      /* minimum elemnumber */
  int   orignmax;  /* max-node-nr-of-original-nodes (w/o nodes for drawing purposes) */
  int   orign;     /* nr-of-original-nodes (w/o nodes for drawing purposes) */
  int   olc;       /* nr-of-original-loadcases (w/o cgx generated datasets (lc)) */
  int   noffs;     /* node-nr offset, defined with asgn */
  int   eoffs;     /* elem-nr offset, defined with asgn */
} Summen;


typedef struct {
  int   nr;              /*   external node-nr (node[node-indx].nr) */
  int   indx;            /*   node-index (node[ext-node-nr].indx)   */
  char  pflag;           /*   1 if used for display purposes    */
                         /*  -1 if the node is deleted          */
                         /*   0 default                         */
  double nx;             /*   coordinates  node[ext-node-nr].nx */
  double ny;
  double nz;
} Nodes;


typedef struct {
  int nr;                /* external element-nr */
  int type;              /* element type (1:Hexa8)  */
  int group;
  int mat;
  int attr;              /* -1:unstructured mesh */
                         /*  0:default           */
                         /*  1:reduced integration */
                         /*  2:incompatible modes */
  int nod[26];
  double **side;         /* side[Nr.][x|y|z]== normal vector */
} Elements;


/* for structured cfd meshes */
typedef struct {
  int dim;                /* 2: surf, 3: body */
  int i,j,k;              /* block dimension in i,j,k direction */
  int *nod;
  int geo;                /* related surf/body */
  int bcface[6];          /* index of the line/face of the related surf/body */
  int neighbor[6];        /* adjacent surface/body index */
  int map[6][3];          /* relative orientation of the neighbor 1:i==i_neigh, 4:i==-i_neigh */
  int strt1[6][3]; 
  int end_1[6][3]; 
  int strt2[6][3]; 
  int end_2[6][3];
  char bctype[6][MAX_LINE_LENGTH];          /* bondary condition type */
} NodeBlocks;


typedef struct {
  int nr;                /* element-face-nr in Abaqus Format  (-1 for shell elements) */
  int elem_nr;           /* reference to external element number  */ 
  int type;
  int group;
  int mat;
  int nod[10];
  double **side;          /* normal vector: side[Nr.][x|y|z] */
} Faces;


typedef struct {
  int nod[3];
  double ncol[3][3];
  int elem_nr;
  int group;
  int mat;
} CTri3;


typedef struct {
  char  name[MAX_LINE_LENGTH];
  double rho;                 /* *DENSITY */
  int    nela;                /* *ELASTIC */
  double *tela, *nue, *ela;
  int    nexp;                /* *EXPANSION */
  double *texp, *exp;
  int    ncon;                /* *CONDUCTIVITY */
  double *tcon, *con;
  int    nsph;                /* *SPECIFIC HEAT */
  double *tsph, *sph;
} Materials;


typedef struct {
  char  name[MAX_LINE_LENGTH];
  int    n;
  double *x, *y;
} Amplitudes;


typedef struct {
  char  **pheader;    /* project header */
  int   npheader;              /* number of headers */
  char  **compName;
  char  **icname;
  char  name[MAX_LINE_LENGTH];
  char  dataset_name[MAX_LINE_LENGTH];
  char  dataset_text[MAX_LINE_LENGTH];
  char  analysis_name[MAX_LINE_LENGTH];
  float value;
  char  filename[MAX_LINE_LENGTH];
  FILE *handle;
  fpos_t *fileptr;
  int   loaded;       /* if data are stored:1 else: 0 */
  int format_flag;
  int analysis_type;
  int step_number;
  int ncomps;
  int irtype;
  int *ictype;
  int *icind1;
  int *icind2;
  int *iexist;
  float **dat;        /* node related data */
  float ***edat;      /* element related data */
  float *max;         /* maximum datum */
  float *min;         /* minimum datum */
  int *nmax;          /* node with maximum datum */
  int *nmin;          /* node with minimum datum */
} Datasets;


typedef struct {
  int p1;
  int p2;
} Edges;


typedef struct {
  double w;                     /* scalierung in den Einheitswuerfel==max(xyzmax) */
  double x,y,z;                     /* Mittelpunktsversatz der unscalierten Vektoren */
  double smin, smax;                            /* max,min Werte der Scala  */
  double xmax, xmin, ymax, ymin, zmax, zmin;    /* max==|max|Abstand eines Punktes vom Zentrum */
} Scale;


typedef struct {
  char *name;
  char flag;                  /* if the set is open: 'o' else: 'c' */
  char type;                  /* ordered entities:1 (seq) or not: 0 (set) */ 
  int material;
  int index;                           /* running number of type-0 sets, assigned and updated in prnt(), eval. in getSetNr() */
  int anz_n;
  int anz_e;
  int anz_f;
  int anz_p;
  int anz_l;
  int anz_c;
  int anz_s;
  int anz_b;
  int anz_nurl;
  int anz_nurs;
  int anz_se;
  int anz_sh;
  int *node;
  int *elem;
  int *face;
  int *pnt;
  int *line;
  int *lcmb;
  int *surf;
  int *body;
  int *nurl;
  int *nurs;
  int *set;
  int *shp;
  int etyp;
  int eattr;       /* -1:unstructured mesh, 0:default, 1:reduced integration, 2:incompatible modes */
} Sets;


typedef struct {
  int nr;
  int *set;
} OpenSets;


/* compiles the actual displayed sets */
typedef struct {
  int nr;
  char type[MAX_LINE_LENGTH];
  int col;
} Psets;


/* sum of char of asci letters */
typedef struct {
  int  max_suma;                   /* maximum value of sum_ascii of aliases stored  */
  int  *anza;                      /* nr of aliases stored */
  int  **aindx;                    /* alias indexes       */
  int  max_sump;                   /* maximum value of sum_ascii of points stored  */
  int  *anzp;                      /* nr of points stored */
  int  **pindx;                    /* point indexes       */
  int  max_suml;                   /* maximum value of sum_ascii of lines stored  */
  int  *anzl;                      /* nr of lines stored  */
  int  **lindx;                    /* line indexes        */
  int  max_sumc;                   /* maximum value of sum_ascii of lcmbs stored  */
  int  *anzc;                      /* nr of lcmbs stored  */
  int  **cindx;                    /* lcmb indexes        */
  int  max_sums;                   /* maximum value of sum_ascii of surfs stored  */
  int  *anzs;                      /* nr of surfs stored  */
  int  **sindx;                    /* surf indexes        */
  int  max_sumb;                   /* maximum value of sum_ascii of bodies stored  */
  int  *anzb;                      /* nr of bodys stored  */
  int  **bindx;                    /* body indexes        */
  int  max_sumS;                   /* maximum value of sum_ascii of nurs stored  */
  int  *anzS;                      /* nr of Nurs stored  */
  int  **Sindx;                    /* nurs indexes        */
  int  max_sumse;                  /* maximum value of sum_ascii of sets stored  */
  int  *anzse;                     /* nr of sets stored  */
  int  **seindx;                   /* set indexes        */
  int  max_sumsh;                  /* maximum value of sum_ascii of shapes stored  */
  int  *anzsh;                     /* nr of shapes stored  */
  int  **shindx;                   /* shape indexes        */
} SumAsci;


typedef struct {
  char  model[MAX_LINE_LENGTH]; /* haeder model */
  int   alias;                           /* alias-names     */
  int   p;                               /* points */
  int   l;                               /* lines  */
  int   c;                               /* lcmbs  */
  int   s;                               /* surfs  */
  int   b;                               /* bodys  */
  int   sh;                              /* shapes  */
  int   nurl;                            /* nurbs lines     */
  int   nurs;                            /* nurbs surfaces  */
  int   psets;                           /* actal displayed sets */ 
} SumGeo;


typedef struct{
  char   *name;
  char   *entitieName;
} Alias;


typedef struct {
  char  *name;
  int type;              /* 0:plane, 1:cyl, 2: cone, 3:sph, 4:nurbs */
  int p[5];              /* 0:3p, 1:4p(2r+2ax) 2:6p(2r+2ax+2rad), 3:2p(r+c) */
  GLint   npgn;           /* size of feed-back-Buffer pgn */
  GLdouble *pgn;           /* stores poligons which defines the interiour */  
} Shapes;


typedef struct {
  char  *name;
  double px;     /* coordinates will be scaled during run-time, scalPoints() */
  double py;
  double pz;
  int   nn;
  int   *nod;
} Points;


typedef struct {
  char  *name;
  char  typ;      /* straight:' ', arc:'a', spline:'s', nurbs:'n' */
  int   p1;
  int   p2;
  int   trk;      /* if arc: 3.pnt, if spline: setname, if nurbs: nurblname */
  int   div;
  double bias;
  int   nip;
  double *ip;
  int   fail;             /* if 1 then the meshing failed */
  int   nn;
  int   *nod;
  int   ne;
  int   *elem;
  int  etyp;
  int  eattr;       /* -1:unstructured mesh, 0:default, 1:reduced integration, 2:incompatible modes */
} Lines;

typedef struct {
  char  *name;
  char  nl;
  char  *o;      /* l-orient +- */
  int   *l;               /* lines */
  int   p1;               /* starting point */
  int   p2;               /* end point */
  double cx;               /* CG, not used so far */
  double cy;
  double cz;
} Lcmb;

typedef struct {
  char  *name;
  char  ori;     /* surface-orientation +- */
  int   sh;               /* embedded shape (>-1:nurs, -1:BLEND, <-1:shape) */
  char   sho;    /* embedded shape (nurs) orientation  */
  int   nl;               /* corners (ether line or lcmb) */
  char  *typ;    /* type: l=line c=lcmb */
  char  *o;      /* l-orient +- */
  int   *l;               /* index of lines, lcmbs, also used to store the index of a substitute surf (last item) */
  int   *cp;              /* index of points at the junction between  lines, lcmbs, starting at the beginning of the first line */
  int   nc;               /* number of closed-line-loops (curves) */
  int   *c;               /* number of lines in each curve */
  double cx;               /* CG, in orient determined, not scaled during run! */
  double cy;
  double cz;
  int   fail;             /* if 1 then the meshing failed */
  int   nn;
  int   *nod;
  int   ne;
  int   *elem;
  int   etyp;
  int   eattr;       /* -1:unstructured mesh, 0:default, 1:reduced integration, 2:incompatible modes */
  int   patch;            /* number of trimming patch in the related nurbs */
  GLint   npgn;           /* size of feed-back-Buffer pgn */
  GLdouble *pgn;           /* stores poligons of the trimmed nurbs which defines the interiour */
} Gsur;

typedef struct {
  char  *name;
  char  ori;     /* body-orientation +- */
  int   ns;               /* nr. of surfaces (currently must be 6 for meshing) */
  char  *o;      /* s-orient +- */
  int   *s;               /* surfaces */
  double cx;               /* CG, in orient determined, not scaled during run! */
  double cy;
  double cz;
  int   fail;             /* if 1 then the meshing failed */
  int   nn;
  int   *nod;
  int   ne;
  int   *elem;
  int   etyp;
  int  eattr;       /* -1:unstructured mesh, 0:default, 1:reduced integration, 2:incompatible modes */
} Gbod;

typedef struct {
  char  *name;
  char  endFlag;
  GLUnurbsObj *Nurb;
  GLint   u_npnt;
  GLint   u_exp;
  GLint   u_nknt;
  GLfloat *uknt;
  GLint   u_stride;
  GLfloat *ctlarray;
  GLfloat *weight;
  GLenum  type;
  GLint   *ctlpnt;
} Nurbl;

typedef struct {
  char  *name;
  char  endFlag;
  GLUnurbsObj *Nurb;
  GLint   u_npnt;
  GLint   u_exp;
  GLint   u_nknt;
  GLint   v_npnt;
  GLint   v_exp;
  GLint   v_nknt;
  GLfloat *uknt;
  GLfloat *vknt;
  GLint   u_stride;
  GLint   v_stride;
  GLfloat *ctlarray;
  GLenum  type;
  GLint   **ctlpnt;
  GLfloat **weight;

  /* additional values for rendering purposes */
  int trimFlag;      /* 1: is trimmed, 0: new trimming necessary (not used so far) */
  int ures, vres;    /* division per u or v */
  int patches;        /* number of trimming patches (separate surfaces)*/
  int *nc;             /* number of trimming curves */
  int **np;            /* number of points in each trimming curve */
  GLfloat ***uv;         /* u,v coordinates of the spline-points of the trimming curves */
  double ***xyz;        /* x,y,z coordinates of the spline-points of the trimming curves */
  double tx, ty, tz;   /* average position for the name-string */
  double *umax, *vmax;   /* max val for u,v, for rendering purposes */
} Nurbs;



typedef struct {
  double alpha, beta;
  int nadapt;
} Meshp;

typedef struct {
  char name[MAX_LINE_LENGTH];
  char bctype[MAX_LINE_LENGTH];
  int surfs;
  int *surf;
} Dunsbou;


void adjustMidsideNode(double *P1, double *P2, double *Pm, int method);
int strsplt( char *rec_str, char breakchar, char ***ptr);
int strfind (char *as1, char *as2);
int readEdges( char *datin, Summen *anz, Nodes **nptr, Elements **eptr );
int *innerFacesHe8(Elements *elems, int numElems, Faces **ptr);

void calcElemNorm_quad4(int i, int n1, int n2, int n3, int n4, int f, Nodes *node, Elements *elem );
void calcElemNorm_tri3(int i, int n1, int n2, int n3, int f, Nodes *node, Elements *elem );
void calcFaceNorm_quad4(int i, int n1, int n2, int n3, int n4, int f, Nodes *node, Faces *face);
void calcFaceNorm_tri3(int i, int n1, int n2, int n3, int f, Nodes *node, Faces *face );
void getElemNormalen( Elements *e_enqire, Nodes *node, int elems );
void getFaceNormalen( Faces *face, Nodes *node, int nr );


int renumberfrd( int firstelem, int firstnode, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr, int **enew_ptr, int **nnew_ptr  );

void define_rgb(float v, float *r, float *g, float *b);
void text(double x, double y, double z,char *msg, void *glut_font);
void scala_rgb( double dx, double dy, int divisions, double bmin, double bmax, double *col, void *glut_font);
void scala_indx( double dx, double dy, int divisions, double bmin, double bmax, int offset, int col, void *glut_font);
void scala_tex( double dx, double dy, int divisions, double bmin, double bmax, double scale, double *col, void *glut_font);
int button(double dx, double dy, char *msg, double mx, double my, void *glut_font);
void polymark ( int n, double *col_r, double *col_g, double *col_b, double *x,
		double *y, double *z );

double nullstelle(double xmin, double xmax, double funktion(double), double *result);
int  calcPrinc( double *s, double *p, double *a1, double *a2, double *a3, int sortFlag );
int calcPvector( long double *s, long double *p, double *a );
void stopClock(int zaeler);
void bsort (double *wert, double *wertsort, int *isort, int anzahl);
int *bsort2(double *wert, int anzahl, int n );
int *bsortf(double *wert, int anzahl, int n );
int *bsortfp(double *wert, int anzahl, int n );
int *bsorti( int *wert, int anzahl, int n );
int compare (char *str1, char *str2, int length);
int compareStrings (char *str1, char *str2);
int elemChecker(int sum_e, Nodes *node, Elements *elem);
int frecord( FILE *handle1,  char *string);
void  getGeoDataTria( double *p1, double *p2, double *p3, double *Ix, double *Iy, double *Ixy,
                double *A, double *pcg);
int getrecord (int bufstart, int bufsize, char *inputdata, char *buffer);
int gl3grades( long double a, long double b, long double c, double *x);
int AsplitA( double *pa1, double *pa2, double *pa3, double *pb1, double *pb2, double *pb3, double *ps1, double *ps2);
double AsplitL( double *b, double *eu, double *ev, double *eg );
double interpol(double *x, double *y, int nn, double x0);
double intpol2(double *x, double *y, int n, double x0, int *method );
double intpol3(double *x, double *y, int n, double x0, int *method );
double intpol(double *x, double *y, int n, double x0 );
int nearNodes(double *po, int *n, double *p, int *node, int *k, int *flag);
int iinsert(int **ipnt, int n, int x0 );
int ifind(int **ipnt, int n, int x0 );
int iremove(int **ipnt, int n, int x0 );
void linelength(double *x, double *y, double *z, int n, double *s );
int kbrecord( char *string);
int p3_finder(int anzahl_nodes, double *nx, double *nz, int *p);
int parser( char gkey, char *record, int *curshft);
int readAnsys(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr, double phi, int frequency );
int readDyna( char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int readfrd(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr, int read_mode );
int readfrdblock( int lc, Summen *anz,   Nodes     *node, Datasets *lcase );
int readOneNode( int lc, Summen *anz, Datasets *lcase, int nodenr, double **vptr, long *offset );
int read2frd(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int readFElt(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int readDuns(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr, int elem_type );
int readIsaac(char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr, NodeBlocks **bptr, int elem_type,  double A, double B, double C );
int readGiff( char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int readNG( char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int readStl( char *datin, Summen *anz, Nodes **nptr, Elements **eptr, Datasets **lptr );
int rotiere(double *x, double *y, int n, double phi, double *xneu, double *yneu);
int checkIfNumber( char *string );
int srecord( char *handle1,  char *string);
double stof(char *string, int a, int b);
int stoi(char *string, int a, int b);
void stos(char *string, int a, int b, char *puffer);
int sins(char *string, int a, int b, char *puffer);
int sword( char *string, char *word);
double p_angle(double x, double y);
void v_add( double *A, double *B, double *C );
double v_betrag(double *a);
void  v_matmult(double *v, double *m);
double v_norm( double *A, double *C );
void v_prod( double *A, double *B, double *C );
void v_result( double *A, double *B, double *C );
int v_rot(double fi, double *p0, double *v, double *pin, double *pout);
void v_scal( double *A, double *B, double *C );
double v_sprod( double *a, double *b);
double v_angle( double *v0, double *v1 );
double v_angle_ref( double *v0, double *v1, double *en );
double v_distA( double *N0, double *N1, double *N2, double *N, double *vray);
int  v_rec2cyl( double *pr, int axis, int *csys, double *pc );
int v_sgg( double *p1, double *p2, double *e1s, double *e2s, double *ps);

void vl_add( double *A, double *B, double *C );
double vl_betrag(double *a);
void  vl_matmult(double *v, double *m);
double vl_norm( double *A, double *C );
void vl_prod( double *A, double *B, double *C );
void vl_result( double *A, double *B, double *C );
void vl_scal( double *A, double *B, double *C );
double vl_sprod( double *a, double *b);
double vl_angle( double *v0, double *v1, double *v2, double *scg, double *bcg );
void  m_copy(double *s, double *m);
void  m_print( double *m);

int m_prod(int *n,double *a,double *b,double *c);
int m_prodtr(int *n,double *a,double *b,double *c);

void  m_sub(double *ms, double *m, double *s);
int write2frd(char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase );
int write2nas(char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase );
int write2aba(char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase, char **dat );
int write2ansys(char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase );
int write2aster(char *datout, Summen *anz, Nodes *node, Elements *elem, Sets *set, Datasets *lcase ); //TODD
int write2darwin( char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase, char **dat );
int write2samcef(char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase );
int write2isaac( char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase, NodeBlocks *nBlock );
int write2duns( char *datout, Summen *anz, Nodes *node, Elements *elem, Datasets *lcase, NodeBlocks *nBlock, int bouNr, Dunsbou *dunsbou );



void readStdCmap( Display **ptr_dpy, int *ptr_dpycells, Colormap *ptr_cmap, XColor **ptr_c,
                 unsigned long **ptr_pix, unsigned int *ptr_npixels, int anzCells );

/*selectFaces.c */
int selectDisplayFacesHe8 (Elements *elems, int numElems, int **pfaces, int *);
int selectDisplayFacesHe20 (Elements *elems, int numElems, int **pfaces, int *);
int selectDisplayFacesTet4 (Elements *elems, int numElems, int **ptr, int *edges);
int selectDisplayFacesTet10 (Elements *elems, int numElems, int **ptr, int *edges);
int selectDisplayFacesPe6 (Elements *elems, int numElems, int **ptr, int *edges);
int selectDisplayFacesPe20 (Elements *elems, int numElems, int **ptr, int *edges);
int commonEdge3 (int *a, int *b);
int findCTri3Edges(Elements *elems, int numElems, int **edges);
int findCTri6Edges(Elements *elems, int numElems, int **edges);
int findCQuad4Edges(Elements *elems, int numElems, int **edges);
int findCQuad8Edges(Elements *elems, int numElems, int **edges);
int compareFaces (int *a, int *b);
int compareFaces3 (int *a, int *b);
int compareFaces4 (int *a, int *b);

double spline_int( int nc, double xneu, double **c );
void createSpline( int n, double **c);
int read2fbd( char *datin, SumGeo *anz, Points **pntptr, Lines **lineptr, Lcmb **lcmbptr, Gsur **surptr, Gbod **bodptr, Nurbs **nursptr, Sets *set );

void shape4q(double xi, double et, double *xl, double *xsj);
void shape6tri(double xi, double et, double *xl, double *xsj);
void shape8q(double xi, double et, double *xl, double *xsj);
int  attach_new(double *coords,double *conode,int *i,double *ratio,double *dist,double *elemcoords);
int distattach_new(double *xig, double *etg, double *pneigh, double *pnode, double *a, double *p, double *ratio, int *nterms);
int attachhe20_(double *co, int *nk, int *neigh, int *node, double *xi_loc__,double *et_loc__);
int attachhe8_(double *co, int *nk, int *neigh, int *node,double *xi_loc__,double *et_loc__);
int distattachhe20_(double *xi,double *et,double *pneigh,double *pnode,double *dist,double *p);
int distattachhe8_(double *xi,double * et, double *pneigh, double *pnode, double *dist, double *p);
int shapefhe20_(double *xi, double *et, double *coef);
int shapefhe8_(double *xi, double *et, double *coef);

int mesh2d(int *_nt, int _nb, int *npc, double **_pnt_u, double **_pnt_v, int **_pnt_flag, int **_tri3, double _alpha, double _beta, int _nadapt);

void errMsg(char *msg, ...);

void transformatrix( double *xab, double *p, double **a);
void rectcyl(int icntrl, double *csab, int nr, Nodes *node, Datasets *lcase, int lc, char type);

int e_c3d_nodes_(double *xl,char *elty, int *elem, double *eqal);
