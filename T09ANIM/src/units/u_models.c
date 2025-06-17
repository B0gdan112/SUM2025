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
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  MATR p;
  
  p = MatrIdentity();
  p = MatrMulMatr(p, MatrTranslate(VecSet(10, -1060, 10)));
  p = MatrMulMatr(p, MatrScale(VecSet1(0.01)));

  BS7_RndPrimsDraw(&Uni->Pr, p);
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