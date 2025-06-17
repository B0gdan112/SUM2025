/* FILE NAME: rndgrid.c
 * PURPOSE: animation module.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */
 
#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndGridCreate( bs7GRID *G, INT W, INT H )
{
  G->V = malloc(sizeof(bs7VERTEX) * W * H);

  if (G->V == 0)
    return FALSE;

  G->H = H;
  G->W = W;

  memset(G->V, 0, sizeof(bs7VERTEX) * W * H);
  return TRUE;
}

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndGridFree( bs7GRID *G )
{
  free(G->V);
}
 
/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       BS7PRIM *Pr;
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndPrimFromGrid( bs7PRIM *Pr, bs7GRID *G )
{
  INT *Ind;
  INT i, k, j;

  memset(Pr, 0, sizeof(bs7PRIM));
  if ((Ind = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return;

  for (k = 0, i = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = (i + 1) * G->W + j;
      Ind[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }
  BS7_RndPrimCreate(Pr, BS7_RND_PRIM_TRISTRIP, G->V, G->W * G->H,
    Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(Ind);
}
 
/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndGridAutoNormals( bs7GRID *G )
{
   INT i, j;

  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++)
    {
      bs7VERTEX
        *P00 = G->V + i * G->W + j,
        *P01 = G->V + i * G->W + j + 1,
        *P10 = G->V + (i + 1) * G->W + j,
        *P11 = G->V + (i + 1) * G->W + j + 1;
      VEC N;
 
      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
 
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);
 
      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
 
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }

   for (i = 0; i < G->W * G->H; i++)
     G->V[i].N = VecNormalize(G->V[i].N);
}
 
/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndGridCreateSphere( bs7GRID *G, FLT R, INT W, INT H )
{
  return FALSE;
}
