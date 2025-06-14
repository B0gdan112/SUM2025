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
  bs7PRIM Ctrl;
  VEC Pos;
  VEC CamLoc, CamDir;
  DBL AngleSpeed;
} bs7UNIT_CTRL;

static VOID BS7_UnitInit( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  Uni->CamLoc = VecSet1(8);
  Uni->CamDir = VecSet(-BS7_RndMatrView.A[0][2],
                       -BS7_RndMatrView.A[1][2],
                       -BS7_RndMatrView.A[2][2]);
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  /*Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
       VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Ani->Mdz * 10));*/

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateZ(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * -Ani->Mdx * 2));

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * Ani->Mdy * 2));

   Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * 15 *
        (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));

  BS7_RndCamSet(Uni->CamLoc, Uni->CamDir, VecSet(0, 1, 0));

  if (Ani->KeysClick[VK_F11])
    BS7_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  static CHAR Buf[102];

  sprintf(Buf, "%d  CGSG Animation: FPS = %.5f %s %s %s", Ani->IsActive, Ani->FPS, glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION));
  SetWindowText(Ani->hWnd, Buf);
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