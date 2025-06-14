/* FILE NAME: u_grid.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#include <stdio.h>

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Land;
  VEC Pos;
} bs7UNIT_GRID;

static VOID BS7_UnitInit( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  INT i, j;
  bs7GRID G;

  BS7_RndGridCreate(&G, 200, 200);

  for (i = 0; i < G.H; i++)
    for (j = 0; j < G.W; j++)
    {
      bs7VERTEX *V = &G.V[i * G.W + j];

      V->P = VecSet(j / (G.W - 1.0) * 50 - 30, -5.0, i / (G.H - 1.0) * 500 -30);
    }

  BS7_RndPrimFromGrid(&Uni->Land, &G);
  BS7_RndGridFree(&G);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->Land);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimDraw(&Uni->Land, MatrIdentity());
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateGrid( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_GRID))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Close = (VOID *)BS7_UnitClose;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/