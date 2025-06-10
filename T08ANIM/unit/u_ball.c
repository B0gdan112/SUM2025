/* FILE NAME: u_ball.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include "anim/anim.h"
typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  VEC Pos;
} bs7UNIT_BALL;

static VOID BS7_UnitInit( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  Uni->Pos = VecSet(0, 1, 0);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{

} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{
  /*Uni->Pos += Ani->DeltaTime * 2.4;*/
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_BALL *Uni, bs7ANIM *Ani )
{ 
  BS7_RndPrimDrawSphere(Uni->Pos, 3);
} /*End of 'BS7_UnitResponse' function*/

static bs7UNIT * BS7_UnitCreateBall( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/