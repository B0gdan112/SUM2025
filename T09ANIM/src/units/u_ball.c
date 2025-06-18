/* FILE NAME: u_SKY.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM SKY;
  VEC Pos;
} bs7UNIT_SKY;

static VOID BS7_UnitInit( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimCreateSphere(&Uni->SKY, 0.47, 18, 8);
} End of 'BS7_UnitInit' function

static VOID BS7_UnitResponse( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{
  Uni->Pos.Y += Ani->DeltaTime * 2.4;
}End of 'BS7_UnitResponse' function

static VOID BS7_UnitRender( bs7UNIT_SKY *Uni, bs7ANIM *Ani )
{ 
  BS7_RndPrimDraw(&Uni->SKY, MatrIdentity());
} End of 'BS7_UnitResponse' function

bs7UNIT * BS7_UnitCreateSKY( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_SKY))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} End of 'BS7_UnitResponse' function*/