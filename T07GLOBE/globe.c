/* FILE NAME  : globe.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <math.h>
#include "globe.h"
#include <time.h>

#include <windows.h>

typedef double DBL;

#define GRID_W 31
#define GRID_H 18

static VEC GLB_Geom[GRID_H][GRID_W];
static INT GLB_Ws, GLB_Hs;
static DBL GLB_ProjDist = 0.1, GLB_ProjSize = 0.1, GLB_Wp, GLB_Hp;

static DBL PI = 3.14159265359;

VOID GLB_Init( DBL R )
{
  INT i, j;
  DBL f, t;
  static POINT pnts[GRID_H][GRID_W];

  for (i = 0, t = 0; i < GRID_H; i++, t += PI / (GRID_H - 1))
    for (j = 0, f = 0; j < GRID_W; j++, f += 2 * PI / (GRID_W - 1))
    {
      GLB_Geom[i][j].X = 3.8 * R * sin(t) * sin(f);
      GLB_Geom[i][j].Y = 2.5 * R * cos(t);
      GLB_Geom[i][j].Z = 2.0 * R * sin(t) * cos(f);
    }

  for (i = 0, t = 0; i < GRID_H; i++, t += PI / GRID_H)
    for (j = 0, f = 0; j < GRID_W; j++, f += 2 * PI / GRID_W)
    {
      pnts[i][j].x = GLB_Geom[i][j].X + GLB_Ws / 2;
      pnts[i][j].y = GLB_Geom[i][j].Y - GLB_Hs / 2;
    }
} /* End of 'GLB_Init' function */

VOID GLB_Resize( INT Ws, INT Hs )
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;
} /* End of 'GLB_Resize' function */

VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  NewP.X = P.X;
  return NewP;
} /* End of 'RotateX' function */

VEC RotateY( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.Z = P.Z * co - P.X * si;
  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  return NewP;
} /* End of 'RotateY' function */

VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
} /* End of 'RotateZ' function */


VOID GLB_Draw( HDC hDC, COLORREF color, INT s )
{
   INT i, j, size = GLB_Ws < GLB_Hs ? GLB_Ws : GLB_Hs;
   DBL t, a, xp, yp;
   POINT ps[4];
   VEC p;
   static POINT pnts[GRID_H][GRID_W];

   t = (double)clock() / CLOCKS_PER_SEC;
   
   size /= 3;

   if (GLB_Ws >= GLB_Hs)
     GLB_Wp = GLB_ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = GLB_ProjSize;
   else
     GLB_Hp = GLB_ProjSize, GLB_Wp = GLB_ProjSize * GLB_Hs / GLB_Ws;

   SelectObject(hDC, GetStockObject(DC_BRUSH));
   SelectObject(hDC, GetStockObject(NULL_PEN));
   SetDCBrushColor(hDC, color);
   /*SetDCPenColor(hDC, RGB(255, 255, 255));*/

   for (i = 0; i < GRID_H; i++)
     for (j = 0; j < GRID_W; j++)
     {
       p = RotateZ(RotateY(RotateX(GLB_Geom[i][j], t * 30), t * 20), t * 10);
       p.Z -= 8;

       xp = p.X * GLB_ProjDist / -p.Z;
       yp = -p.Y * GLB_ProjDist / -p.Z;

       pnts[i][j].x = xp * GLB_Ws / GLB_Wp + GLB_Ws / 2;
       pnts[i][j].y = -yp * GLB_Hs / GLB_Hp + GLB_Hs / 2;
     }

   /*for (i = 0; i < GRID_H; i++)
     for (j = 0; j < GRID_W; j++)
     {
       Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s,
         pnts[i][j].x + s, pnts[i][j].y + s);
     }*/

   for (i = 0; i < GRID_H; i++)
   {
     MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
     for (j = 0; j < GRID_W; j++)
       LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
   }

   for (i = 0; i < GRID_H - 1; i++)
     for (j = 0; j < GRID_W - 1; j++)
     {
       ps[0] = pnts[i][j];
       ps[1] = pnts[i][j + 1];
       ps[2] = pnts[i + 1][j + 1];
       ps[3] = pnts[i + 1][j];
       
       if (i % 2 == 0 && j % 2 == 0)
         SetDCBrushColor(hDC, RGB(170, 180, 97));
       else if (i % 2 == 1 && j % 2 == 1)
         SetDCBrushColor(hDC, RGB(24, 153, 88));
       else
         SetDCBrushColor(hDC, color);
       if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y ) +
           (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y ) +
           (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y ) +
           (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y ) <= 0)
         Polygon(hDC, ps, 4);
     }
} /* End of 'GLB_Draw' function */