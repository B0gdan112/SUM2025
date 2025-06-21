/* FILE NAME: u_ctrl.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 10.06.2025
 */

#include <stdio.h>

#include "units/units.h"

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Ctrl, debug;
  VEC Pos;
  DBL AngleSpeed;
} bs7UNIT_CTRL;

static INT i = 3, j = 45;
static BOOL R, F, L;

static VOID BS7_UnitInit( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  bs7MATERIAL mtl;

  BS7_RndPrimCreate(&Uni->debug, BS7_RND_PRIM_POINTS, NULL, 1, NULL, 0);

  mtl = BS7_RndMtlGetDef();
  strcpy(mtl.Name, "Debug");
  mtl.ShdNo = BS7_RndShdAdd("debug");
  Uni->debug.MtlNo = BS7_RndMtlAdd(&mtl);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->debug);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  FLT
      Dist = VecLen(VecSubVec(BS7_RndCamAt, BS7_RndCamLoc)),
      cosT = (BS7_RndCamLoc.Y - BS7_RndCamAt.Y) / Dist,
      sinT = sqrt(1 - cosT * cosT),
      plen = Dist * sinT,
      cosP = (BS7_RndCamLoc.Z - BS7_RndCamAt.Z) / plen,
      sinP = (BS7_RndCamLoc.X - BS7_RndCamAt.X) / plen,
      Azimuth = R2D(atan2(sinP, cosP)),
      Elevator = R2D(atan2(sinT, cosT)),
      sx, sy, Wp, Hp;
  VEC dv;

  if (Ani->KeysClick[VK_F11])
    BS7_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
  if (Ani->KeysClick['F'])
    BS7_RndCamSet(VecSet1(8), VecSet1(0), VecSet(0, 1, 0));
  if (Ani->Keys[VK_CONTROL])
  {
    Azimuth += Ani->GlobalDeltaTime *
      (-30 * 1 * Ani->Keys[VK_LBUTTON] * Ani->Mdx +
       40 * 1 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));

    Elevator += Ani->GlobalDeltaTime *
      (-10 * 1 * Ani->Keys[VK_LBUTTON] * Ani->Mdy +
       40 * 1 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

    Dist += Ani->GlobalDeltaTime *
      (1 * Ani->Mdz +
       80 * 0.1 * (1 + Ani->Keys[VK_SHIFT] * 30) *
          (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
     
    if (Elevator < 0.08)
      Elevator = 0.08;
    if (Elevator > 178)
      Elevator = 178;
    if (Dist < 0.1)
      Dist = 0.1;

    Wp = BS7_RndProjSize;
    Hp = BS7_RndProjSize;
     
    if (Ani->W > Ani->H)
      Wp *= (FLT)Ani->W / Ani->H;
    else
      Hp *= (FLT)Ani->H / Ani->W;
     
    sx = Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / BS7_RndProjDist;
    sy = Ani->Keys[VK_RBUTTON] * Ani->Mdy * Hp / Ani->H * Dist / BS7_RndProjDist;

    dv = VecAddVec(VecMulNum(BS7_RndCamRight, sx),
                   VecMulNum(BS7_RndCamUp, sy));
    BS7_RndCamAt = VecAddVec(BS7_RndCamAt, dv);
    BS7_RndCamLoc = VecAddVec(BS7_RndCamLoc, dv);
     
    BS7_RndCamSet(PointTransform(VecSet(0, Dist, 0),
                                 MatrMulMatr3(MatrRotateX(Elevator),
                                              MatrRotateY(Azimuth),
                                              MatrTranslate(BS7_RndCamAt))),
                  BS7_RndCamAt,
                  VecSet(0, 1, 0));
  }
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  static CHAR Buf[102];
  MATR p = MatrIdentity();

  BS7_RndPrimDraw(&Uni->debug, p);

  sprintf(Buf, "%d  CGSG Animation: FPS = %.5f\nTime: %lf\n", Ani->IsActive, Ani->FPS, Ani->GlobalTime);
  BS7_RndFntDraw(Buf, VecSet(0, 0, 0), 30);
} /*End of 'BS7_UnitRender' function*/

bs7UNIT * BS7_UnitCreateInit( VOID )
{ 
  bs7UNIT *Uni;

  if ((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_CTRL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/