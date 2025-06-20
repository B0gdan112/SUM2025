/* FILE NAME: u_SKY.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 18.06.2025
 */

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Pr;
  VEC Pos;
} bs7UNIT_SKY;

static VOID BS7_UnitInit( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
  bs7MATERIAL mtl;

  BS7_RndPrimCreate(&Uni->Pr, BS7_RND_PRIM_POINTS, NULL, 1, NULL, 0);

  mtl = BS7_RndMtlGetDef();
  strcpy(mtl.Name, "Sky Material");
  mtl.Tex[0] = BS7_RndTexAddFromFile("bin/textures/sky.bmp");
  mtl.ShdNo = BS7_RndShdAdd("sky");
  Uni->Pr.MtlNo = BS7_RndMtlAdd(&mtl);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
  glDepthMask(FALSE);
  BS7_RndPrimDraw(&Uni->Pr, MatrIdentity());
  glDepthMask(TRUE);
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateSky( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_SKY))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  Uni->Close = (VOID *)BS7_UnitClose;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/