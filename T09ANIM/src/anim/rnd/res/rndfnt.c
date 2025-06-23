/* FILE NAME: rndfnt.c
 * PURPOSE: 3D animation project.
 * PROGRAMMER: BS7
 * DATE: 18.06.2025
 */

#include "anim/rnd/rnd.h"

/* Current font description */
/* Font data */
static bs7FONT BS7_RndFntFont;
 
/* Font characters primitives */
static bs7PRIM BS7_RndFntChars[256];
 
/* Font material */
static INT BS7_RndFntMtlNo;

/* Font material */
static INT BS7_RndFnt3DMtlNo;
 
/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndFntLoad( CHAR *FileName )
{
  INT i;
  DWORD Sign, W, H, *Tex;
  bs7VERTEX Chars[256][4];
  FILE *F;
  bs7MATERIAL mtl;
  
 
  BS7_RndFntClose();
 
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
 
  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
 
  fread(&BS7_RndFntFont, sizeof(bs7FONT), 1, F);
  fread(Chars, sizeof(bs7VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
 
  if ((Tex = malloc(W * H * 4)) == NULL)
  {
    memset(&BS7_RndFntFont, 0, sizeof(BS7_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(Tex, 4, W * H, F);
  fclose(F);
 
  mtl = BS7_RndMtlGetDef();
  strcpy(mtl.Name, FileName);
  mtl.Tex[0] = i = BS7_RndTexAddImg(FileName, W, H, 4, Tex);
  mtl.ShdNo = BS7_RndShdAdd("font");
  BS7_RndFntMtlNo = BS7_RndMtlAdd(&mtl);
 
  mtl.ShdNo = BS7_RndShdAdd("font3d");
  BS7_RndFnt3DMtlNo = BS7_RndMtlAdd(&mtl);
 
  for (i = 0; i < 256; i++)
  {
    BS7_RndPrimCreate(&BS7_RndFntChars[i], BS7_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    BS7_RndFntChars[i].MtlNo = BS7_RndFntMtlNo;
  }
 
  free(Tex);
  return TRUE;
} /* End of 'BS7_RndFntLoad' function */
 
/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndFntInit( VOID )
{
  BS7_RndFntLoad("bin/fonts/Consolas.g3df");
} /* End of 'BS7_RndFntInit' function */
 
/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndFntClose( VOID )
{
  INT i;
 
  for (i = 0; i < 256; i++)
    BS7_RndPrimFree(&BS7_RndFntChars[i]);
  memset(BS7_RndFntChars, 0, sizeof(BS7_RndFntChars));
  memset(&BS7_RndFntFont, 0, sizeof(BS7_RndFntFont));
} /* End of 'BS7_RndFntInit' function */
 
/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID BS7_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  FLT startx = Pos.X;
 
  while (*Str != 0)
  {
    if (*Str == '\n')
    {
      Pos.X = startx;
      Pos.Y -= Size;
    }
    else
    {
      if (BS7_RndFntFont.AdvanceX[(BYTE)*Str] != 0)
      {
        BS7_RndPrimDraw(&BS7_RndFntChars[(BYTE)*Str], MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += BS7_RndFntFont.AdvanceX[(BYTE)*Str] * Size;
      }
    }
    Str++;
  }
} /* End of 'BS7_RndFntDraw' function */

/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID BS7_RndFntDraw3D( CHAR *Str, VEC Pos, FLT Size )
{
  INT i;

  for (i = 0; i < 256; i++)
    BS7_RndFntChars[i].MtlNo = BS7_RndFnt3DMtlNo;

  BS7_RndFntDraw(Str, Pos, Size);

  for (i = 0; i < 256; i++)
    BS7_RndFntChars[i].MtlNo = BS7_RndFntMtlNo;
} /* End of 'BS7_RndFntDraw3D' function */
