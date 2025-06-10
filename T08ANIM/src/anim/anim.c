/* FILE NAME: anim.c
 * PURPOSE: animation module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include "anim/anim.h"

bs7ANIM BS7_Anim;

VOID BS7_AnimInit( HWND hWnd )
{
  memset(&BS7_Anim, 0, sizeof(bs7ANIM));

  BS7_Anim.hWnd = hWnd;
  BS7_RndInit(hWnd);
  BS7_Anim.hDC = BS7_hRndDCFrame;
  BS7_Anim.W = BS7_RndFrameW;
  BS7_Anim.H = BS7_RndFrameH;

  BS7_TimerInit();
}

VOID BS7_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < BS7_Anim.NumOfUnits; i++)
  {
    BS7_Anim.Units[i]->Close(BS7_Anim.Units[i], &BS7_Anim);
    free(BS7_Anim.Units[i]);
  }
  BS7_RndClose();
  memset(&BS7_Anim, 0, sizeof(bs7ANIM));
}

VOID BS7_AnimResize( INT W, INT H )
{
  BS7_RndResize(W, H);

  BS7_Anim.W = W;
  BS7_Anim.H = H;

  BS7_AnimRender();
}

VOID BS7_AnimCopyFrame( HDC hDC )
{
  BS7_RndCopyFrame(hDC);
}

VOID BS7_AnimRender( VOID )
{
  INT i;

  BS7_TimerResponse();

  for (i = 0; i < BS7_Anim.NumOfUnits; i++)
    BS7_Anim.Units[i]->Response(BS7_Anim.Units[i], &BS7_Anim);

  BS7_RndStart();
  for (i = 0; i < BS7_Anim.NumOfUnits; i++)
    BS7_Anim.Units[i]->Render(BS7_Anim.Units[i], &BS7_Anim);
  BS7_RndEnd();
}

VOID BS7_AnimUnitAdd( bs7UNIT *Uni )
{
  if(BS7_Anim.NumOfUnits < BS7_MAX_UNITS)
    BS7_Anim.Units[BS7_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &BS7_Anim);
}

VOID BS7_AnimFlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
 
  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    GetWindowRect(hWnd, &SaveRect);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);

    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
 
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
}

VOID BS7_DoExit( VOID )
{
}
