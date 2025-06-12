/* FILE NAME: unit.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include "units/units.h"

static VOID BS7_UnitInit( bs7UNIT *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_AnimUnitCreate( INT Size )
{
  bs7UNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(bs7UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
 
  /* Setup unit methods */
  Uni->Init = BS7_UnitInit;
  Uni->Close = BS7_UnitClose;
  Uni->Response = BS7_UnitResponse;
  Uni->Render = BS7_UnitRender;
 
  return Uni;
}