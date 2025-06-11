/* FILE NAME: rndbase.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#include "rnd.h"

#pragma comment(lib, "opengl32")

VOID BS7_RndInit( HWND hWnd )
{
  BS7_hRndWnd = hWnd;

  BS7_hRndDC = GetDC(hWnd);

  BS7_RndResize(30, 30);
  BS7_RndCamSet(VecSet1(8), VecSet1(0), VecSet(0, 1, 0));
}

VOID BS7_RndClose( VOID )
{
  ReleaseDC(BS7_hRndWnd, BS7_hRndDC);
}

VOID BS7_RndResize( INT W, INT H )
{
  BS7_RndFrameW = W;
  BS7_RndFrameH = H;

  BS7_RndProjSet();
}

VOID BS7_RndCopyFrame( VOID )
{
  SwapBuffers(BS7_hRndDC);
}


VOID BS7_RndStart( VOID )
{
  /* BS7_RndCamSet(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  m = BS7_RndMatrVP;

  p1 = VecSet(0, 0, 0);
  p2 = VecSet(1, 0, 0);

  p = VecMulMatr(p1, m);
  pnts[0].x = (INT)((p.X + 1) * BS7_RndFrameW / 2);
  pnts[0].y = (INT)((-p.Y + 1) * BS7_RndFrameH / 2);

  p = VecMulMatr(p2, m);
  pnts[1].x = (INT)((p.X + 1) * BS7_RndFrameW / 2);
  pnts[1].y = (INT)((-p.Y + 1) * BS7_RndFrameH / 2);

  MoveToEx(BS7_hRndDCFrame, pnts[0].x, pnts[0].y, NULL);
  LineTo(BS7_hRndDCFrame, pnts[1].x, pnts[1].y); */
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
