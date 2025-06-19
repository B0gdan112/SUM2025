/* FILE NAME: rndprims.c
 * PURPOSE: primitives
 * PROGRAMMER: BS7
 * DATE: 16.06.2025
 */

#include "rnd.h"
#include "def.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       BS7PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL BS7_RndPrimsCreate( bs7PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(bs7PRIMS));
  if ((Prs->Prims = malloc(NumOfPrims * sizeof(bs7PRIM))) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, NumOfPrims * sizeof(bs7PRIM));
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'BS7_RndPrimsCreate' function */
 
/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       BS7PRIMS *Prs;
 * RETURNS: None.
 */
VOID BS7_RndPrimsFree( bs7PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      BS7_RndPrimFree(&Prs->Prims[i]);
  }
  free(Prs->Prims);
  memset(Prs, 0, sizeof(bs7PRIMS));
} /* End of 'BS7_RndPrimsFree' function */
 
/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       BS7PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID BS7_RndPrimsDraw( bs7PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Trans, World);
 
  /* Draw all nontransparent primitives */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (BS7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      BS7_RndPrimDraw(&Prs->Prims[i], m);
 
  /* Draw all transparent primitives with front face culling */ 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (BS7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      BS7_RndPrimDraw(&Prs->Prims[i], m);
 
  /* Draw all transparent primitives with back face culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (BS7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      BS7_RndPrimDraw(&Prs->Prims[i], m);
 
  glDisable(GL_CULL_FACE);
} /* End of 'BS7_RndPrimsDraw' function */

/* Load primitives from '*.G3DM' file function.
 * ARGUMENTS:
 *   - pointer to primitives to create:
 *       BS7PRIMS *Prs;
 *   - '*.G3DM' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndPrimsLoad( bs7PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, p, m, t;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  struct mtls
  {
    CHAR Name[300]; /* Material name */
    /* Illumination coefficients */
    VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
    FLT Ph;             /* Phong power coefficient – shininess */
    FLT Trans;          /* Transparency factor */
    DWORD Tex[8];       /* Texture references 
                         * (8 time: texture number in G3DM file, -1 if no texture) */
    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;       /* Shader number (uses after load into memory) */
  } *mtls;
 
  memset(Prs, 0, sizeof(bs7PRIMS));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
 
  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
 
  /* Allocate memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  /* Load entire file into memory */
  rewind(F);
  fread(mem, 1, flen, F);
  fclose(F);
 
  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
 
  /* Counts read */
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;
 
  if (!BS7_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }
 
  /* Primitives */
  for (p = 0; p < (INT)NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    DWORD MtlNo;              /* Material number in table below (in material section) */
    bs7VERTEX *V;
    INT *Ind;
 
    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    V = (bs7VERTEX *)ptr;
    ptr += sizeof(bs7VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;
 
    BS7_RndPrimCreate(&Prs->Prims[p], BS7_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = MtlNo + BS7_RndMaterialsSize;
  }
 
  /* Materials */
  mtls = (struct mtls *)ptr;
  ptr += sizeof(struct mtls) * NumOfMaterials;
  for (m = 0; m < (INT)NumOfMaterials; m++)
  {
    bs7MATERIAL mtl = BS7_RndMtlGetDef();
 
    mtl.Ka = mtls[m].Ka;
    mtl.Kd = mtls[m].Kd;
    mtl.Ks = mtls[m].Ks;
    mtl.Ph = mtls[m].Ph;
    mtl.Trans = mtls[m].Trans;
    strncpy(mtl.Name, mtls[m].Name, BS7_STR_MAX - 1);
 
    for (t = 0; t < 8; t++)
      if (mtls[m].Tex[t] != -1)
        mtl.Tex[t] = mtls[m].Tex[t] + BS7_RndTexturesSize;
      else
        mtl.Tex[t] = -1;
    BS7_RndMtlAdd(&mtl);
  }
 
  /* Textures */
  for (t = 0; t < (INT)NumOfTextures; t++)
  {
    DWORD W, H, C;
    CHAR *Name = (CHAR *)ptr;
 
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    BS7_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }

  free(mem);

  return TRUE;
} /* End of 'BS7_RndPrimsDraw' function */

VOID BS7_RndPrimsDrawLab( bs7PRIMS *Pr, CHAR *FileName, MATR m )
{
  FILE *F;
  INT i, j, x = 0, y = 0;
  CHAR Buf[1000];
  INT Lab[1000][1000];

  if ((F = fopen(FileName, "r")) != NULL)
  {
    while (fgets(Buf, 1000, F) != NULL)
    {
      for (i = 0; i < 1000; i++)
      {
        if (Buf[i] == '*')
          Lab[x][y] = 1;
        else if (Buf[i] == ' ')
          Lab[x][y] = 0;
        else if (Buf[i] == '\n')
          y++;
      }
    }
  }

  for (i = 0; i < 1000; i++)
    for (j = 0; j < 1000; j++)
      if (Lab[i][j] == 1)
      {
        m = MatrMulMatr(m, MatrTranslate(VecSet(i, 0, j)));
        BS7_RndPrimsDraw(Pr, m);
      }
}
