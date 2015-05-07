
#include <extUtil.h>



/* berechne normale und speichere direkt im element */

void calcElemNorm_quad4(int i, int n1, int n2, int n3, int n4, int f, Nodes *node, Elements *elem )
{
  static double v1[3], v2[3], v3[3];

      /* Vereinfachte Normale auf Flaeche des i.Elements*/
      v_result( &node[n1].nx, &node[n3].nx, v1);
      v_result( &node[n2].nx, &node[n4].nx, v2);
      v_prod( v1, v2, v3 );
      v_norm( v3, elem[i].side[f] );

}

void calcElemNorm_tri3(int i, int n1, int n2, int n3, int f, Nodes *node, Elements *elem )
{
  static double v1[3], v2[3], v3[3];

      v_result( &node[n1].nx, &node[n2].nx, v1);
      v_result( &node[n1].nx, &node[n3].nx, v2);
      v_prod( v1, v2, v3 );
      v_norm( v3, elem[i].side[f] );
}

/* berechne normale und speichere direkt im face */

void calcFaceNorm_quad4(int i, int n1, int n2, int n3, int n4, int f, Nodes *node, Faces *face)
{
  static double v1[3], v2[3], v3[3];

      /* Vereinfachte Normale auf Flaeche des i.Elements*/
      v_result( &node[n1].nx, &node[n3].nx, v1);
      v_result( &node[n2].nx, &node[n4].nx, v2);
      v_prod( v1, v2, v3 );
      v_norm( v3, face[i].side[f] );

}

void calcFaceNorm_tri3(int i, int n1, int n2, int n3, int f, Nodes *node, Faces *face )
{
  static double v1[3], v2[3], v3[3];

      v_result( &node[n1].nx, &node[n2].nx, v1);
      v_result( &node[n1].nx, &node[n3].nx, v2);
      v_prod( v1, v2, v3 );
      v_norm( v3, face[i].side[f] );
}



void getElemNormalen( Elements *e_enqire, Nodes *node, int elems )
{
  int i;

  for (i=0; i<elems; i++ )
  {
    if(e_enqire[e_enqire[i].nr].type == 1)
    {
      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[2],
                  e_enqire[e_enqire[i].nr].nod[6], e_enqire[e_enqire[i].nr].nod[5], 0, node, e_enqire);

      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[5], e_enqire[e_enqire[i].nr].nod[6],
                  e_enqire[e_enqire[i].nr].nod[7], e_enqire[e_enqire[i].nr].nod[4], 1, node, e_enqire);

      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[4], e_enqire[e_enqire[i].nr].nod[7],
                  e_enqire[e_enqire[i].nr].nod[3], e_enqire[e_enqire[i].nr].nod[0], 2, node, e_enqire);

      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[5], e_enqire[e_enqire[i].nr].nod[4],
                  e_enqire[e_enqire[i].nr].nod[0], e_enqire[e_enqire[i].nr].nod[1], 3, node, e_enqire);

      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[0],
                  e_enqire[e_enqire[i].nr].nod[3], e_enqire[e_enqire[i].nr].nod[2], 4, node, e_enqire);

      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[2], e_enqire[e_enqire[i].nr].nod[3],
                  e_enqire[e_enqire[i].nr].nod[7], e_enqire[e_enqire[i].nr].nod[6], 5, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 2)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 1], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[ 4], 1, node, e_enqire);
      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[0], e_enqire[e_enqire[i].nr].nod[1],
                  e_enqire[e_enqire[i].nr].nod[4], e_enqire[e_enqire[i].nr].nod[3], 2, node, e_enqire);
      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[2],
                  e_enqire[e_enqire[i].nr].nod[5], e_enqire[e_enqire[i].nr].nod[4], 3, node, e_enqire);
      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[3], e_enqire[e_enqire[i].nr].nod[5],
                  e_enqire[e_enqire[i].nr].nod[2], e_enqire[e_enqire[i].nr].nod[0], 4, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 3)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 3], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 3], 1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 3], 2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 1], 3, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 4)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 9], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 2], 1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[14], 2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[ 6], 3, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[17], 4, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[ 5], 5, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[13], 6, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[21], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[ 1], 7, node, e_enqire);
      
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[10], 8, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[ 3], 9, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[15],10, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[ 7],11, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[18],12, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 6],13, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[14],14, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[22], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[ 2],15, node, e_enqire);
      
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[12],16, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[ 4],17, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[19],18, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[ 7],19, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[15],20, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[ 3],21, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[11],22, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[23], e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[ 0],23, node, e_enqire);
      
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 8],24, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 0],25, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[11],26, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[ 3],27, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[10],28, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[ 2],29, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 9],30, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[24], e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 1],31, node, e_enqire);
      
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[16],32, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[ 5],33, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[17],34, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[ 6],35, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[18],36, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 7],37, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[19],38, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[25], e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[ 4],39, node, e_enqire);
      
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 8],40, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 1],41, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[13],42, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[ 5],43, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[16],44, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[ 4],45, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[12],46, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[20], e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[ 0],47, node, e_enqire);
    }
    if( e_enqire[e_enqire[i].nr].type == 5 )
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[ 6], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[15], 1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[ 0], 2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[ 9], 3, node, e_enqire);
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[15], 4, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[12], 5, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[15], e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[ 9], 6, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[ 3], 7, node, e_enqire);
   
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[ 7], 8, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[16], 9, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[ 1],10, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[10],11, node, e_enqire);
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[16],12, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[13],13, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[16], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[10],14, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[10], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[ 4],15, node, e_enqire);
   
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 8],16, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[17],17, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[ 2],18, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[11],19, node, e_enqire);
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[17],20, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[14],21, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[17], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[11],22, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[11], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[ 5],23, node, e_enqire);
   
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 8],24, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[18],25, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 0],26, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 0],27, node, e_enqire);
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[18],28, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 6],29, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[18], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 0],30, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 0],31, node, e_enqire);
   
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[12],32, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[19],33, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[12], e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[ 3],34, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[ 3],35, node, e_enqire);
   
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[13], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[19],36, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[14],37, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[19], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[ 3],38, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[14], e_enqire[e_enqire[i].nr].nod[ 3],39, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 6)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 7], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 8], 1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 1], 2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 3], 3, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 8], 4, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 9], 5, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 2], 6, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 3], 7, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 9], 8, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 7], 9, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 0],10, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 9], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 3],11, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 4],12, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 5],13, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 2],14, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 1],15, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 7)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[0], e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[2],0, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 8)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[0], e_enqire[e_enqire[i].nr].nod[3], e_enqire[e_enqire[i].nr].nod[5],0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[2], e_enqire[e_enqire[i].nr].nod[5], e_enqire[e_enqire[i].nr].nod[4],1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[4], e_enqire[e_enqire[i].nr].nod[5], e_enqire[e_enqire[i].nr].nod[3],2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[3], e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[4],3, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 9)
    {
      calcElemNorm_quad4(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[0], e_enqire[e_enqire[i].nr].nod[1], e_enqire[e_enqire[i].nr].nod[2],e_enqire[e_enqire[i].nr].nod[3], 0, node, e_enqire);
    }
    if(e_enqire[e_enqire[i].nr].type == 10)
    {
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 0], e_enqire[e_enqire[i].nr].nod[ 4], 0, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 4], e_enqire[e_enqire[i].nr].nod[ 1], 1, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 1], e_enqire[e_enqire[i].nr].nod[ 5], 2, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 5], e_enqire[e_enqire[i].nr].nod[ 2], 3, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 2], e_enqire[e_enqire[i].nr].nod[ 6], 4, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 6], e_enqire[e_enqire[i].nr].nod[ 3], 5, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 3], e_enqire[e_enqire[i].nr].nod[ 7], 6, node, e_enqire);
      calcElemNorm_tri3(e_enqire[i].nr,e_enqire[e_enqire[i].nr].nod[ 8], e_enqire[e_enqire[i].nr].nod[ 7], e_enqire[e_enqire[i].nr].nod[ 0], 7, node, e_enqire);
    }
  }
}



void getFaceNormalen( Faces *face, Nodes *node, int nr )
{
  int i;

  for (i=0; i<nr; i++ )
  {
    if(face[i].type == 7)
    {
      calcFaceNorm_tri3(i,face[i].nod[0], face[i].nod[1], face[i].nod[2],0, node, face);
    }
    if(face[i].type == 8)
    {
      calcFaceNorm_tri3(i,face[i].nod[0], face[i].nod[3], face[i].nod[5],0, node, face);
      calcFaceNorm_tri3(i,face[i].nod[2], face[i].nod[5], face[i].nod[4],1, node, face);
      calcFaceNorm_tri3(i,face[i].nod[4], face[i].nod[5], face[i].nod[3],2, node, face);
      calcFaceNorm_tri3(i,face[i].nod[3], face[i].nod[1], face[i].nod[4],3, node, face);
    }
    if(face[i].type == 9)
    {
      calcFaceNorm_quad4(i,face[i].nod[0], face[i].nod[1], face[i].nod[2],face[i].nod[3], 0, node, face);
    }
    if(face[i].type == 10)
    {
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 0], face[i].nod[ 4], 0, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 4], face[i].nod[ 1], 1, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 1], face[i].nod[ 5], 2, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 5], face[i].nod[ 2], 3, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 2], face[i].nod[ 6], 4, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 6], face[i].nod[ 3], 5, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 3], face[i].nod[ 7], 6, node, face);
      calcFaceNorm_tri3(i,face[i].nod[ 8], face[i].nod[ 7], face[i].nod[ 0], 7, node, face);
    }
  }
}

