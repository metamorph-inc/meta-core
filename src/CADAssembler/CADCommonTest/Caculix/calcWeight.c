#include <cgx.h>


#define real double
#define integer int

/* Subroutine */ int hexaeder_(integer *iel, integer *istat, real *x, real *y,
	 real *z__, real *v, real *xcg, real *ycg, real *zcg)
{
    /* Initialized data */

    static integer ih[24]	/* was [6][4] */ = { 1,3,5,7,5,3,4,2,2,8,6,4,
	    2,4,6,6,8,8,5,6,4,3,4,6 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real a[9]	/* was [3][3] */;
    static integer i__, j, k;
    static real vt;
    static integer ihh[24]	/* was [6][4] */, nit;
    extern /* Subroutine */ int sarrus_(real *, real *);
    //_******************************
    //Added 20110119 PProkopczuk
    double cog[3] = {0.0,0.0,0.0};
    double hexCog[3] = {0.0,0.0,0.0};
    //_******************************

/* ********************************************************* */
/* *   Berechnet das Volumen v eines bel. Hexaeders mit    * */
/* *   8 Knoten x,y,z (Anordnung in NASTRAN-Konvention).   * */
/* *   Der Hexaeder wird in 6 Tetraeder unterteilt, deren  * */
/* *   Volumen bestimmt wird (Bartsch S. 295).             * */
/* *   Da die Tetraeder auf 4 verschiedene Arten           * */
/* *   angeordnet werden kˆnnen und sich dabei jeweils     * */
/* *   unterschiedliche Sensitivit‰ten des Volumens von    * */
/* *   den Knoten ergibt (Verschiebung des Knoten 1 wirkt  * */
/* *   sich anders aus bei einer anderen Tetraeder-        * */
/* *   anordnung) werden die Tetraeder zyklisch rotiert    * */
/* *   (k-Loop).                                           * */
/* *   (Auﬂerdem werden die Schwerpunktkoordinaten durch    * */
/* *   arithmetische Mittelung der Koordinaten der Eck-    * */
/* *   punkte bestimmt)										* */
/* *   Geaendert 19.1.2011 PProkopczuk						* */
/* *   Schwerpunktkoordinaten werden durch volumengewichtete * */
/* *   Addition der Tetraederschwerpunkte ermittelt			* */
/* ********************************************************* */

/*   Variante mit 5 Tetraedern (ntet=5): */
/*   (#5: ein groﬂer mit allen Fl‰chendiagonalen als Kanten) */
/*     data ih /1, 3, 6, 8, 2, */
/*    &         4, 2, 7, 5, 5, */
/*    &         2, 4, 5, 7, 4, */
/*    &         5, 7, 2, 4, 7/ */
/*   Variante mit 6 Tetraedern (ntet=6): */
/*   (alle Tetraeder gleich groﬂ) */
    /* Parameter adjustments */
    --z__;
    --y;
    --x;
    --istat;

    /* Function Body */

    nit = 4;
    *v = 0.f;

    i__1 = nit;
    for (k = 1; k <= i__1; ++k) {

	for (i__ = 1; i__ <= 6; ++i__) {
	    for (j = 1; j <= 4; ++j) {
		if (ih[i__ + j * 6 - 7] <= 4) {
		    ihh[i__ + j * 6 - 7] = (ih[i__ + j * 6 - 7] + k - 2) % 4 
			    + 1;
		} else {
		    ihh[i__ + j * 6 - 7] = (ih[i__ + j * 6 - 7] + k - 2) % 4 
			    + 5;
		}
	    }
/*         write (*,*)k,i,(ihh(i,j),j=1,4) */
	}

	for (i__ = 1; i__ <= 6; ++i__) {
	    //_******************************
	    //Added 20110119 PProkopczuk
	    //Ersten Eckpunkt in cog-Array schreiben
	    cog[0]=x[ihh[i__ - 1]];
	    cog[1]=y[ihh[i__ - 1]];
	    cog[2]=z__[ihh[i__ - 1]];
	    //_******************************
/*          Belegen der Matrix: */
	    for (j = 1; j <= 3; ++j) {
		a[j - 1] = x[ihh[i__ - 1]] - x[ihh[i__ + (j + 1) * 6 - 7]];
		a[j + 2] = y[ihh[i__ - 1]] - y[ihh[i__ + (j + 1) * 6 - 7]];
		a[j + 5] = z__[ihh[i__ - 1]] - z__[ihh[i__ + (j + 1) * 6 - 7]];
		//_******************************
	    	//Added 20110119 PProkopczuk
	    	//Weitere Eckpunkte addieren
		cog[0]+=x[ihh[i__ + (j + 1) * 6 - 7]];
	    cog[1]+=y[ihh[i__ + (j + 1) * 6 - 7]];
	    cog[2]+=z__[ihh[i__ + (j + 1) * 6 - 7]];	
		//_******************************
	    } 
	    //_******************************
	    //Added 20110119 PProkopczuk
	    //Schwerpunkt des Tetraeders ist arithmetisches Mittel der Eckpunkte
	    cog[0]/=4.0;
	    cog[1]/=4.0;
	    cog[2]/=4.0;
	    //_******************************/
/*          Berechnung Volumina der Teil-Tetraeder: */
	    sarrus_(a, &vt);
	    if (vt < 0.f) {
/*              write (*,*)'WARNING! NEG. VOLUME OF TETRAHEDRON #',i, */
/*     &        ' IN ELEMENT #',iel */
		++istat[1];
		vt = -vt;
	    } else if (vt == 0.f) {
/*              write (*,*)'WARNING! ZERO VOLUME OF TETRAHEDRON #',i, */
/*     &       ' IN ELEMENT #',iel */
		++istat[2];
	    } else {
		++istat[3];
	    }
	    //_******************************
	    //Added 20110119 PProkopczuk
	    //Schwerpunkt mit Volumen multiplizieren fuer Gewichtung
	    hexCog[0]+=cog[0]*vt;
	    hexCog[1]+=cog[1]*vt;
	    hexCog[2]+=cog[2]*vt;
	    //_******************************
	    *v += vt;
	    	    
	}
    }
	//_***********************************
    //Added 20110119 PProkopczuk
    //Wichtig, COG Berechnung muss vor finaler Volumen-Berechnung erfolgen!! 
    *xcg = hexCog[0]/ *v;
    *ycg = hexCog[1]/ *v;
    *zcg = hexCog[2]/ *v;
    //_******************************

    *v = *v / 6.f / (real) nit;
//_*************************************    
//Removed 20110119 PProkopczuk
/*     Bestimmung des Schwerpunkts: */
/*     (vorl. durch Mittelung der Koordinaten der Eckpunkte) */
/*
    *xcg = 0.f;
    *ycg = 0.f;
    *zcg = 0.f;
    for (i__ = 1; i__ <= 8; ++i__) {
	*xcg += x[i__];
	*ycg += y[i__];
	*zcg += z__[i__];
    }
    *xcg /= 8.f;
    *ycg /= 8.f;
    *zcg /= 8.f;

*/
//_***************************************
    return 0;
} /* hexaeder_ */

//_*****************************************************************************

/* Subroutine */ int tetraeder_(integer *iel, integer *istat, real *x, real *y,
	 real *z__, real *v, real *xcg, real *ycg, real *zcg)
{
    /* Initialized data */

    static integer ih[4]	/* was [1][4] */ = { 1,2,3,4 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real a[9]	/* was [3][3] */;
    static integer i__, j, k;
    static real vt;
    static integer ihh[4]	/* was [1][4] */, nit;
    extern /* Subroutine */ int sarrus_(real *, real *);

/* ********************************************************* */
/* *   Berechnet das Volumen v eines bel. Hexaeders mit    * */
/* *   8 Knoten x,y,z (Anordnung in NASTRAN-Konvention).   * */
/* *   Der Hexaeder wird in 6 Tetraeder unterteilt, deren  * */
/* *   Volumen bestimmt wird (Bartsch S. 295).             * */
/* *   Da die Tetraeder auf 4 verschiedene Arten           * */
/* *   angeordnet werden kˆnnen und sich dabei jeweils     * */
/* *   unterschiedliche Sensitivit‰ten des Volumens von    * */
/* *   den Knoten ergibt (Verschiebung des Knoten 1 wirkt  * */
/* *   sich anders aus bei einer anderen Tetraeder-        * */
/* *   anordnung) werden die Tetraeder zyklisch rotiert    * */
/* *   (k-Loop).                                           * */
/* *   Auﬂerdem werden die Schwerpunktkoordinaten durch    * */
/* *   arithmetische Mittelung der Koordinaten der Eck-    * */
/* *   punkte bestimmt                                     * */
/* ********************************************************* */

/*   Variante mit 5 Tetraedern (ntet=5): */
/*   (#5: ein groﬂer mit allen Fl‰chendiagonalen als Kanten) */
/*     data ih /1, 3, 6, 8, 2, */
/*    &         4, 2, 7, 5, 5, */
/*    &         2, 4, 5, 7, 4, */
/*    &         5, 7, 2, 4, 7/ */
/*   Variante mit 1 Tetraeder  (ntet=1): */
/*   (alle Tetraeder gleich groﬂ) */
    /* Parameter adjustments */
    --z__;
    --y;
    --x;
    --istat;

    /* Function Body */

    nit = 4;
    *v = 0.f;

    i__1 = nit;
    for (k = 1; k <= i__1; ++k) {

	for (i__ = 1; i__ <= 1; ++i__) {
	    for (j = 1; j <= 4; ++j) {
		if (ih[i__ + j - 2] <= 4) {
		    ihh[i__ + j - 2] = (ih[i__ + j - 2] + k - 2) % 4 + 1;
		} else {
		    ihh[i__ + j - 2] = (ih[i__ + j - 2] + k - 2) % 4 + 5;
		}
	    }
/*         write (*,*)k,i,(ihh(i,j),j=1,4) */
	}

	for (i__ = 1; i__ <= 1; ++i__) {
/*          Belegen der Matrix: */
	    for (j = 1; j <= 3; ++j) {
		a[j - 1] = x[ihh[i__ - 1]] - x[ihh[i__ + (j + 1) - 2]];
		a[j + 2] = y[ihh[i__ - 1]] - y[ihh[i__ + (j + 1) - 2]];
		a[j + 5] = z__[ihh[i__ - 1]] - z__[ihh[i__ + (j + 1) - 2]];
	    }
/*          Berechnung der Teil-Tetraeder: */
	    sarrus_(a, &vt);
	    if (vt < 0.f) {
/*              write (*,*)'WARNING! NEG. VOLUME OF TETRAHEDRON #',i, */
/*     &        ' IN ELEMENT #',iel */
		++istat[1];
		vt = -vt;
	    } else if (vt == 0.f) {
/*              write (*,*)'WARNING! ZERO VOLUME OF TETRAHEDRON #',i, */
/*     &       ' IN ELEMENT #',iel */
		++istat[2];
	    } else {
		++istat[3];
	    }
	    *v += vt;
	}
    }

    *v = *v / 6.f / (real) nit;


/*     Bestimmung des Schwerpunkts: */
/*     (vorl. durch Mittelung der Koordinaten der Eckpunkte) */

    *xcg = 0.f;
    *ycg = 0.f;
    *zcg = 0.f;
    for (i__ = 1; i__ <= 4; ++i__) {
	*xcg += x[i__];
	*ycg += y[i__];
	*zcg += z__[i__];
    }
    *xcg /= 4.f;
    *ycg /= 4.f;
    *zcg /= 4.f;


    return 0;
} /* tetraeder_*/


/* Subroutine */ int sarrus_(real *a, real *d__)
{
    static integer i__, j;
    static real ah[15]	/* was [3][5] */, pm, pp;

/* ********************************************************* */
/* *  Berechnung der Determinante D einer 3*3 Matrix A     * */
/* *  nach der Regel von Sarrus (Bartsch, S. 111)          * */
/* ********************************************************* */

    /* Parameter adjustments */
    a -= 4;

    /* Function Body */
    for (i__ = 1; i__ <= 3; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    ah[i__ + j * 3 - 4] = a[i__ + j * 3];
	    if (j < 3) {
		ah[i__ + (j + 3) * 3 - 4] = a[i__ + j * 3];
	    }
	}
    }


    *d__ = 0.f;
    for (i__ = 1; i__ <= 3; ++i__) {
	pp = 1.f;
	pm = -1.f;
	for (j = 1; j <= 3; ++j) {
	    pp *= ah[j + (j + i__ - 1) * 3 - 4];
	    pm *= ah[j + (i__ + 3 - j) * 3 - 4];
	}
	*d__ = *d__ + pp + pm;
    }

/* L9999: */
    return 0;
} /* sarrus_ */
