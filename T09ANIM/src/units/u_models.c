/* FILE NAME: u_models.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 17.06.2025
 */

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIMS Pr;
  VEC Pos;
} bs7UNIT_MODEL;

static VOID BS7_UnitInit( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsLoad(&Uni->Pr, "bin/models/hollow_knight.g3dm");
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(
                            VecSet(Ani->Keys['A'] * Ani->GlobalDeltaTime * 10 - Ani->Keys['D'] * Ani->GlobalDeltaTime * 10
                            + (-Ani->JX) * 30 * Ani->GlobalDeltaTime, 0, 0)));
  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(
                            VecSet(0, 0, Ani->Keys['W'] * Ani->GlobalDeltaTime * 10
                            - Ani->Keys['S'] * Ani->GlobalDeltaTime * 10 + (-Ani->JY) * 30 * Ani->GlobalDeltaTime)));
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  MATR p;

  p = MatrIdentity();
  p = MatrMulMatr(p, MatrTranslate(VecSet(0, 0, 0)));
  p = MatrMulMatr(p, MatrScale(VecSet1(0.1)));

  if (Ani->Keys['A'])
    p = MatrMulMatr(p, MatrRotateY(90));
  else if (Ani->Keys['D'])
    p = MatrMulMatr(p, MatrRotateY(-90));
  else if (Ani->Keys['S'])
    p = MatrMulMatr(p, MatrRotateY(180));

  BS7_RndPrimsDraw(&Uni->Pr, MatrMulMatr(p, MatrTranslate(Uni->Pos)));
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateModel( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_MODEL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  Uni->Close = (VOID *)BS7_UnitClose;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/