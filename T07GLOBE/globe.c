/* FILE NAME  : globe.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <math.h>
#include <time.h>
#include <windows.h>

#include "mth.h"
#include "globe.h"


typedef double DBL;

#define GRID_W 31
#define GRID_H 18

static VEC GLB_Geom[GRID_H][GRID_W];
static INT GLB_Ws, GLB_Hs;
static DBL GLB_ProjDist = 0.1, GLB_ProjSize = 0.1, GLB_Wp, GLB_Hp;

DBL Power( DBL A, DBL B )
{
  if (A < 0)
    return -pow(-A, B);
  return pow(A, B);
} /* End of 'Power' function */

VOID GLB_Init( DBL R )
{
  INT i, j;
  DBL f, t;
  static POINT pnts[GRID_H][GRID_W];

  for (i = 0, t = 0; i < GRID_H; i++, t += PI / (GRID_H - 1))
    for (j = 0, f = 0; j < GRID_W; j++, f += 2 * PI / (GRID_W - 1))
    {
      GLB_Geom[i][j].X = 3 * R * Power(sin(t), 1) * Power(sin(f), 1);
      GLB_Geom[i][j].Y = 3 * R * cos(t);
      GLB_Geom[i][j].Z = 3 * R * Power(sin(t), 1) * Power(cos(f), 1);
    }
} /* End of 'GLB_Init' function */

VOID GLB_Resize( INT Ws, INT Hs )
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;

  if (GLB_Ws >= GLB_Hs)
    GLB_Wp = GLB_ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = GLB_ProjSize;
  else
    GLB_Hp = GLB_ProjSize, GLB_Wp = GLB_ProjSize * GLB_Hs / GLB_Ws;
} /* End of 'GLB_Resize' function */

VOID GLB_Draw( HDC hDC, INT s )
{
   INT i, j;
   DBL t;
   POINT ps[4];
   MATR p;
   VEC a;
   COLORREF color = RGB(25, 55, 25);
   static POINT pnts[GRID_H][GRID_W];

   t = (double)clock() / CLOCKS_PER_SEC;

   SelectObject(hDC, GetStockObject(DC_BRUSH));
   SelectObject(hDC, GetStockObject(NULL_PEN));
   SetDCBrushColor(hDC, color);
   /*SetDCPenColor(hDC, RGB(255, 255, 255));*/

   p = MatrMulMatr6(MatrRotateX(t * 0.5), 
     MatrRotateY(t * 0.5), 
     MatrRotateZ(t * 0.5), 
     MatrTranslate(VecSet(0, 2, 0)), 
     MatrView(VecSet1(8), VecSet(0, 0, 0), VecSet(0, 1, 0)), 
     MatrFrustum(-GLB_Wp / 2, GLB_Wp / 2, -GLB_Hp / 2, GLB_Hp / 2, GLB_ProjDist, 10));

   for (i = 0; i < GRID_H; i++)
     for (j = 0; j < GRID_W; j++)
     {
       a = VecMulMatr(GLB_Geom[i][j], p);

       pnts[i][j].x = (INT)((a.X + 1) * GLB_Ws / 2);
       pnts[i][j].y = (INT)((-a.Y + 1) * GLB_Hs / 2);
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

       if (j % 2 == 0)
         SetDCBrushColor(hDC, RGB(24, 153, 88));
       else
         SetDCBrushColor(hDC, color);
       if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y ) +
           (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y ) +
           (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y ) +
           (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y ) >= 0)
         Polygon(hDC, ps, 4);
     }
} /* End of 'GLB_Draw' function */