/* FILE NAME: rndprim.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#include "def.h"
#include "rnd.h"

VOID BS7_RndPrimCreate( bs7PRIM *Pr, bs7PRIM_TYPE Type,
                        bs7VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  INT i;

  memset(Pr, 0, sizeof(bs7PRIM));
  glGenVertexArrays(1, &Pr->VA);

  if (V != NULL && NoofV != 0)
  {
    glBindVertexArray(Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bs7VERTEX) * NoofV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(bs7VERTEX),
                          (VOID *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(bs7VERTEX),
                          (VOID *)sizeof(VEC));
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(bs7VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2)));
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(bs7VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));
     
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
  
    glBindVertexArray(0);

    /* Obtain BB */
    Pr->MinBB = Pr->MaxBB = V[0].P;
    for (i = 1; i < NoofV; i++)
    {
      Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
      Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
    }
  }

  if (Ind != 0 && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;

  Pr->Trans = MatrIdentity();

  Pr->Type = Type;
  Pr->Trans = MatrIdentity();
}

VOID BS7_RndPrimFree( bs7PRIM *Pr )
{
  glBindVertexArray(Pr->VA);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  memset(Pr, 0, sizeof(bs7PRIM));
}

/* Draw primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to free:
 *       BS7PRIM *Pr;
 *   - transformation 'world' matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID BS7_RndPrimDraw( bs7PRIM *Pr, MATR World )
{
  INT loc;
  UINT ProgId;
  MATR 
    w = MatrMulMatr(Pr->Trans, World),
    WInv = MatrTranspose(MatrInverse(w)),
    M = MatrMulMatr(w, BS7_RndMatrVP);
  INT gl_prim_type = Pr->Type == BS7_RND_PRIM_LINES ? GL_LINES :
                     Pr->Type == BS7_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                     Pr->Type == BS7_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                     GL_POINTS;
  UINT TexId = BS7_RndTextures[0].TexId;

  if ((ProgId = BS7_RndMtlApply(Pr->MtlNo)) == 0)
    return;
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, BS7_Anim.Time);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WInv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &BS7_RndCamLoc.X);

  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
  {
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
  }
  glUseProgram(0);
} /* End of 'BS7_RndPrimDraw' function */

/*BOOL BS7_RndPrimCreateSphere( bs7PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL tetha, phi;
  INT vn = W * H, fn = (W - 1) * 2 * (H - 1) * 3

  memset(Pr, 0, sizeof(bs7PRIM));
  if (!BS7_RndPrimCreate(Pr, BS7_RND_PRIM_TRIMESH, V, vn, I, fn))
    return FALSE;

  for (k = 0, i = 0, tetha = 0.0f; i < H; i++, tetha += PI / (H - 1))
    for (j = 0, phi = 0.0f; j < W; j++, phi += 2 * PI / (W - 1))
    {
      Pr->VA[k].P.X = R * sin(tetha) * sin(phi);
      Pr->VA[k].P.Y = R * cos(tetha);
      Pr->VA[k].P.Z = R * sin(tetha) * cos(phi);

      k++;
    }

  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Pr->IBuf[k++] = i * W + j;
      Pr->IBuf[k++] = i * W + j + 1;
      Pr->IBuf[k++] = (i + 1) * W + j;
    }

  return TRUE;
} /* End of 'BS7_RndPrimCreateSphere' function */

BOOL BS7_RndPrimLoad( bs7PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT vn = 0, fn = 0, size;
  bs7VERTEX *V;
  INT *I;
  CHAR Buf[1000];
 
  memset(Pr, 0, sizeof(bs7PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
 
  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
          n++;
        i++;
      }
      fn += n - 2;
    }
  }

  size = sizeof(bs7VERTEX) * vn + sizeof(INT) * fn * 3;
  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  I = (INT *)(V + vn);

  /* Read geometry */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[vn].C = Vec4Set(0.0, 0.0, 0.0, 0);
      V[vn++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0, c, c0, c1, c2;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
        {
          sscanf(Buf + i, "%d", &c);
          if (c > 0)
            c--;
          else if (c < 0)
            c = vn + c;
 
          if (n == 0)
            c0 = c;
          else if (n == 1)
            c1 = c;
          else
          {
            c2 = c;
            I[fn++] = c0;
            I[fn++] = c1;
            I[fn++] = c2;
            c1 = c2;
          }
          n++;
        }
        i++;
      }
    }
  }
  fclose(F);
  BS7_RndPrimTriMeshAutoNormals(V, vn, I, fn);
  BS7_RndPrimCreate(Pr, BS7_RND_PRIM_TRIMESH, V, vn, I, fn);
  free(V);
  return TRUE;
} /* End of 'BS7_RndPrimCreateSphere' function */

VOID BS7_RndPrimTriMeshAutoNormals( bs7VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;
  //VEC L = VecNormalize(VecSet(1, 3, 2));

  for (i = 0; i < NumOfV; i++)
     V[i].N = VecSet1(0);

  for (i = 0; i < NumOfI; i += 3)
  {
    bs7VERTEX 
      *P0 = &V[Ind[i]],
      *P1 = &V[Ind[i + 1]],
      *P2 = &V[Ind[i + 2]];
    VEC N = VecNormalize(VecCrossVec(VecSubVec(P1->P, P0->P), VecSubVec(P2->P, P0->P)));
 
    P0->N = VecAddVec(P0->N, N);
    P1->N = VecAddVec(P1->N, N);
    P2->N = VecAddVec(P2->N, N);
  }

  for (i = 0; i < NumOfV; i++)
     V[i].N = VecNormalize(V[i].N);

  for (i = 0; i < NumOfV; i++)
  {
    //FLT nl = VecDotVec(L, V[i].N);
    
    //V[i].C = Vec4SetVec3(VecMulNum(VecSet(0, 0.5, 0), nl < 0.1 ? 0.1 : nl));
    V[i].C = Vec4SetVec3(VecSet(0, 0.5, 0), 1);
  }
} /* End of 'BS7_RndPrimTriMeshAutoNormals' function */

