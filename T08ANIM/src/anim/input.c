/* FILE NAME: input.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include "anim/anim.h"

#define BS7_GET_JOYSTIC_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) – 1)

static VOID BS7_AnimKeyboardInit( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
  {
    BS7_Anim.Keys[i] >>= 7;
    BS7_Anim.KeysClick[i] = BS7_Anim.Keys[i] && !BS7_Anim.KeysOld[i];
  }
  memcpy(BS7_Anim.KeysOld, BS7_Anim.Keys, 256);
}

static VOID BS7_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(BS7_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    BS7_Anim.Keys[i] >>= 7;
    BS7_Anim.KeysClick[i] = BS7_Anim.Keys[i] && !BS7_Anim.KeysOld[i];
  }
  memcpy(BS7_Anim.KeysOld, BS7_Anim.Keys, 256);
}

static VOID BS7_AnimMouseInit( VOID )
{
  INT BS7_MouseWheel = 0;

  BS7_Anim.Mdx = 0;
  BS7_Anim.Mdy = 0;

  BS7_Anim.Mx = 0;
  BS7_Anim.My = 0;

  BS7_Anim.Mdz = 0;
  BS7_Anim.Mz = 0;
}

static VOID BS7_AnimMouseResponse( VOID )
{
  POINT pt;
  INT BS7_MouseWheel = 0;

  GetCursorPos(&pt);
  ScreenToClient(BS7_Anim.hWnd, &pt);

  BS7_Anim.Mdx = pt.x - BS7_Anim.Mx;
  BS7_Anim.Mdy = pt.y - BS7_Anim.My;

  BS7_Anim.Mx = pt.x;
  BS7_Anim.My = pt.y;

  BS7_Anim.Mdz = BS7_MouseWheel;
  BS7_Anim.Mz += BS7_MouseWheel;
  BS7_MouseWheel = 0;
}

static VOID BS7_AnimJoystickInit( VOID )
{
}

static VOID BS7_AnimJoystickResponse( VOID )
{
}

VOID BS7_AnimInputInit( VOID )
{
  BS7_AnimKeyboardInit();
  BS7_AnimMouseInit();
  BS7_AnimJoystickInit();
}

VOID BS7_AnimInputResponse( VOID )
{
  BS7_AnimKeyboardResponse();
  BS7_AnimMouseResponse();
  BS7_AnimJoystickResponse();
}