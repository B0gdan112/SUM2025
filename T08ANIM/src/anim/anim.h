/* FILE NAME: anim.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#ifndef __anim_h__
#define __anim_h__

#include "rnd/rnd.h"

#define BS7_MAX_UNITS 3000

typedef struct tagbs7UNIT bs7UNIT;
typedef struct tagbs7ANIM bs7ANIM;

#define BS7_UNIT_BASE_FIELDS \
  VOID (*Init)( bs7UNIT *Uni, bs7ANIM *Ani );     \
  VOID (*Close)( bs7UNIT *Uni, bs7ANIM *Ani );    \
  VOID (*Response)( bs7UNIT *Uni, bs7ANIM *Ani ); \
  VOID (*Render)( bs7UNIT *Uni, bs7ANIM *Ani )

struct tagbs7UNIT
{
  BS7_UNIT_BASE_FIELDS;
};

typedef struct tagbs7ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  bs7UNIT *Units[BS7_MAX_UNITS];
  INT NumOfUnits;

  DBL GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
  BOOL IsPause;

  BYTE Keys[256];
  BYTE KeysClick[256];
  BYTE KeysOld[256];

  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  BYTE JBut[32], JButOld[32], JButClick[32];
  INT JPov;
  DBL JX, JY, JZ, JR;
};

extern bs7ANIM BS7_Anim;
extern bs7UNIT BS7_Unit;
extern INT BS7_MouseWheel;

VOID BS7_AnimInit( HWND hWnd );
VOID BS7_AnimClose( VOID );
VOID BS7_AnimResize( INT W, INT H );
VOID BS7_AnimCopyFrame( HDC hDC );
VOID BS7_AnimRender( VOID );
VOID BS7_AnimFlipFullScreen( VOID );
VOID BS7_AnimUnitAdd( bs7UNIT *Uni );
VOID BS7_DoExit( VOID );
bs7UNIT * BS7_AnimUnitCreate( INT size );
VOID BS7_TimerInit( VOID );
VOID BS7_TimerResponse( VOID );
VOID BS7_AnimInputInit( VOID );
VOID BS7_AnimInputResponse( VOID );

#endif /*__anim_h_ */

/* End of 'anim.h' file */