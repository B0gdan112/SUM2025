/* FILE NAME: units.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#ifndef __units_h__
#define __units_h__

#include "anim/anim.h"
#include "anim/rnd/rnd.h"

VOID BS7_UnitInit( bs7UNIT *Uni, bs7ANIM *Ani );
VOID BS7_UnitClose( bs7UNIT *Uni, bs7ANIM *Ani );
VOID BS7_UnitResponse( bs7UNIT *Uni, bs7ANIM *Ani );
VOID BS7_UnitRender( bs7UNIT *Uni, bs7ANIM *Ani );
bs7UNIT * BS7_AnimUnitCreate( INT Size );

bs7UNIT * BS7_UnitCreateBall( VOID );

#endif /*__units_h_ */