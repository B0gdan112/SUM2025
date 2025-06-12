/* FILE NAME: u_monokuma.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Pr;
  VEC Pos;
} bs7UNIT_BALL;

static VOID BS7_UnitInit( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  Uni->Pos = VecSet1(0);
  BS7_RndPrimLoad(&Uni->Pr, "bin/models/monokuma.obj");
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  Uni->Pos.X += 0;
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  MATR p;

  p = MatrMulMatr(
    MatrRotateZ(3 * Ani->Time),
    MatrTranslate(VecSet(3, fabs(2 * sin(2 * Ani->Time)) + 2, 4)));

  BS7_RndPrimDraw(&Uni->Pr, p);
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateKuma( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Close = (VOID *)BS7_UnitClose;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/