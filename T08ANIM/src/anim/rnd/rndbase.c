/* FILE NAME: rndbase.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#include "rnd.h"

VOID BS7_RndInit( HWND hWnd )
{
  HDC hDC;

  BS7_hRndWnd = hWnd;
  hDC = GetDC(hWnd);
  BS7_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  BS7_hRndBmFrame = NULL;
}

VOID BS7_RndClose( VOID )
{
  DeleteDC(BS7_hRndDCFrame);
  if (BS7_hRndBmFrame != NULL)
    DeleteObject(BS7_hRndBmFrame);
}

VOID BS7_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(BS7_hRndWnd);
 
  if (BS7_hRndBmFrame != NULL)
    DeleteObject(BS7_hRndBmFrame);
  BS7_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(BS7_hRndWnd, hDC);

  SelectObject(BS7_hRndDCFrame, BS7_hRndBmFrame);

  BS7_RndFrameW = W;
  BS7_RndFrameH = H;

  BS7_RndProjSet();
}

VOID BS7_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, BS7_RndFrameW, BS7_RndFrameH,
    BS7_hRndDCFrame, 0, 0, SRCCOPY);
}


VOID BS7_RndStart( VOID )
{
  SelectObject(BS7_hRndDCFrame, GetStockObject(NULL_PEN));
  SelectObject(BS7_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCBrushColor(BS7_hRndDCFrame, RGB(0, 0, 0));
  Rectangle(BS7_hRndDCFrame, 0, 0, BS7_RndFrameW + 1, BS7_RndFrameH);

  SelectObject(BS7_hRndDCFrame, GetStockObject(NULL_PEN));
  SelectObject(BS7_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCPenColor(BS7_hRndDCFrame, RGB(255, 255, 255));
}

VOID BS7_RndEnd( VOID )
{
}

VOID BS7_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = BS7_RndProjSize;
  /* Correct aspect ratio */
  if (BS7_RndFrameW > BS7_RndFrameH)
    rx *= (DBL)BS7_RndFrameW / BS7_RndFrameH;
  else
    ry *= (DBL)BS7_RndFrameH / BS7_RndFrameW;
 
  BS7_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      BS7_RndProjDist, BS7_RndProjFarClip);
  BS7_RndMatrVP = MatrMulMatr(BS7_RndMatrView, BS7_RndMatrProj);
}
 
VOID BS7_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  BS7_RndMatrView = MatrView(Loc, At, Up);
  BS7_RndMatrVP = MatrMulMatr(BS7_RndMatrView, BS7_RndMatrProj);
}
