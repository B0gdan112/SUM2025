/* FILE NAME: rndmtl.c
 * PURPOSE: materials.
 * PROGRAMMER: BS7
 * DATE: 14.06.2025
 */

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "anim\rnd\rnd.h"

bs7MATERIAL BS7_RndMtlGetDef( VOID )
{
  bs7MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.90, 0.90, 0.90},
    {0.30, 0.30, 0.30},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };
 
  return def_mtl;
}

VOID BS7_RndMtlInit( VOID )
{
  bs7MATERIAL def = BS7_RndMtlGetDef();
 
  BS7_RndMaterialsSize = 0;
  BS7_RndMtlAdd(&def);
}

VOID BS7_RndMtlClose( VOID )
{
  bs7MATERIAL def = {
    "Default",
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    0, 1,
    {0, 0, 0, 0, 0, 0, 0, 0},
    0
  };

  BS7_RndMtlAdd(&def);
  BS7_RndMaterialsSize = 0;
}

INT BS7_RndMtlAdd( bs7MATERIAL *Mtl )
{
  if (BS7_RndMaterialsSize >= BS7_MAX_MATERIALS)
    return 0;
  BS7_RndMaterials[BS7_RndMaterialsSize] = *Mtl;
  return BS7_RndMaterialsSize++;
}

UINT BS7_RndMtlApply( INT MtlNo )
{
  INT prg;
  bs7MATERIAL *mtl;
  INT i, loc;
 
  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= BS7_RndMaterialsSize)
    MtlNo = 0;
  mtl = &BS7_RndMaterials[MtlNo];
 
  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= BS7_RndShadersSize)
    prg = 0;
  prg = BS7_RndShaders[prg].ProgId;
 
  if (prg == 0)
    return 0;
 
  glUseProgram(prg);
 
  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, BS7_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, BS7_Anim.GlobalTime);
 
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";
 
    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, BS7_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
}
