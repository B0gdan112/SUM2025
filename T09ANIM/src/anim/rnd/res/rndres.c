/* FILE NAME: rndres.h
 * PURPOSE: 3D animation project.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#include "anim/rnd/rnd.h"

VOID BS7_RndResInit( VOID )
{
  BS7_RndShdInit();
  BS7_RndTexInit();
  BS7_RndMtlInit();
  BS7_RndFntInit();
}

VOID BS7_RndResClose( VOID )
{
  BS7_RndFntClose();
  BS7_RndMtlClose();
  BS7_RndTexClose();
  BS7_RndShdClose();
}
