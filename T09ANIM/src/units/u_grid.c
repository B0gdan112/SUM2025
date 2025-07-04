/* FILE NAME: u_grid.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#include <stdio.h>

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Land;
  VEC Pos;
} bs7UNIT_GRID;

static bs7GRID G;

static VOID BS7_UnitInit( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  HBITMAP hBm;
  BITMAP bm;
  INT w, h;
  INT x, y;
  bs7MATERIAL mtl;

  if ((hBm = LoadImage(NULL, "bin/heights/flat.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);

    w = bm.bmWidth;
    h = bm.bmHeight;

    if (bm.bmBitsPixel == 8 && BS7_RndGridCreate(&G, w, h) != 0)
    {
      BYTE *Bits = bm.bmBits;
 
      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];
 
          G.V[y * w + x].P = VecSet(x / (w - 1.0) * 100, hgt / 18.0, 1 - y / (h - 1.0) * 100);
          G.V[y * w + x].T = Vec2Set(x / (w - 1.0), (1 - y / (h - 1.0)));
        }
      BS7_RndGridAutoNormals(&G);
      BS7_RndPrimFromGrid(&Uni->Land, &G);
      BS7_RndGridFree(&G);
    }
    DeleteObject(hBm);
  }
  /* material for land */
  mtl = BS7_RndMtlGetDef();
  strcpy(mtl.Name, "Land Material");
  mtl.Tex[0] = BS7_RndTexAddFromFile("bin/textures/stone.bmp");
  mtl.ShdNo = BS7_RndShdAdd("Land");
  Uni->Land.MtlNo = BS7_RndMtlAdd(&mtl);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->Land);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_GRID *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimDraw(&Uni->Land, MatrMulMatr(MatrScale(VecSet1(2)), MatrTranslate(VecSet(0, 0, 150))));
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateGrid( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_GRID))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Close = (VOID *)BS7_UnitClose;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/