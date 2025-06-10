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
  BS7_RndResize(30, 30);
  BS7_RndCamSet(VecSet1(1), VecSet1(0), VecSet(0, 1, 0));
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

  BS7_RndProjSet();
 
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

  SelectObject(BS7_hRndDCFrame, GetStockObject(DC_PEN));
  SelectObject(BS7_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCPenColor(BS7_hRndDCFrame, RGB(0, 255, 0));

 
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

BOOL BS7_RndPrimCreate( bs7PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(bs7PRIM));

  size = sizeof(bs7VERTEX) * NoofV + sizeof(INT) * NoofI;

  Pr->V = malloc(size);
  if (Pr->V == NULL)
    return FALSE;
  memset(Pr->V, 0, size);
  Pr->I = (INT *)(Pr->V + NoofV);

  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  return TRUE;
}

VOID BS7_RndPrimFree( bs7PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(bs7PRIM));
}

VOID BS7_RndPrimDraw( bs7PRIM *Pr, MATR World )
{
  INT i;
  POINT *pnts;
  MATR M = MatrMulMatr3(Pr->Trans, World, BS7_RndMatrVP);

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, M);

    pnts[i].x = (INT)((p.X + 1) * BS7_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * BS7_RndFrameH / 2);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(BS7_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(BS7_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(BS7_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(BS7_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}

BOOL BS7_RndPrimCreateSphere( bs7PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL tetha, phi;

  memset(Pr, 0, sizeof(bs7PRIM));
  if (!BS7_RndPrimCreate(Pr, W * H, (W - 1) * 2 * (H - 1) * 3))
    return FALSE;

  for (k = 0, i = 0, tetha = 0.0; i < H; i++, tetha += PI / (H - 1))
    for (j = 0, phi = 0.0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      Pr->V[k].P.X = R * sin(tetha) * sin(phi);
      Pr->V[k].P.Y = R * cos(tetha);
      Pr->V[k].P.Z = R * sin(tetha) * cos(phi);

      k++;
    }

  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
    }

  return TRUE;
} /* End of 'BS7_RndPrimCreateSphere' function */

BOOL BS7_RndPrimLoad( bs7PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT vn = 0, fn = 0;
  CHAR Buf[1000];
 
  memset(Pr, 0, sizeof(bs7PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
 
  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
          n++;
        i++;
      }
      fn += n - 2;
    }
  }
 
  if (!BS7_RndPrimCreate(Pr, vn, fn * 3))
  {
    fclose(F);
    return FALSE;
  }
 
  /* Read geometry */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[vn++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0, c, c0, c1, c2;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
        {
          sscanf(Buf + i, "%d", &c);
          if (c > 0)
            c--;
          else if (c < 0)
            c = vn + c;
 
          if (n == 0)
            c0 = c;
          else if (n == 1)
            c1 = c;
          else
          {
            c2 = c;
            Pr->I[fn++] = c0;
            Pr->I[fn++] = c1;
            Pr->I[fn++] = c2;
            c1 = c2;
          }
          n++;
        }
        i++;
      }
    }
  }
 
  fclose(F);
  return TRUE;
} /* End of 'BS7_RndPrimCreateSphere' function */

