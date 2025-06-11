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
  VEC CamLoc;
  DBL AngleSpeed;
} bs7UNIT_CTRL;

static VOID BS7_UnitInit( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  Uni->CamLoc = VecSet(0, 0, 0);
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx));

  if (Ani->KeysClick[VK_F11])
    BS7_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick[VK_ESCAPE])
    DestroyWindow(BS7_Anim.hWnd); 
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_CTRL *Uni, bs7ANIM *Ani )
{
  /*INT i, len;
  RECT rc = {0, 0, Ani->W, Ani->H};
  static CHAR Buf[10000];

  len = sprintf(Buf, "Keys: ");
  for (i = 0; i < 256; i++)
    len += sprintf(Buf + len, "%02X:%s ", i, Ani->Keys[i] ? "*" : " ");

  if (Ani->Keys[VK_F11] || Ani->KeysClick[0x7A])
    BS7_AnimFlipFullScreen();

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  DrawText(Ani->hDC, Buf, len, &rc, DT_WORDBREAK | DT_CENTER | DT_BOTTOM);*/

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