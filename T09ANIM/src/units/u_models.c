/* FILE NAME: u_models.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 17.06.2025
 */

#include "units/units.h"

#define BS7_Max_Chars1 200

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIMS Pr, Cube;
  VEC Pos;
  CHAR Heights[BS7_Max_Chars1][BS7_Max_Chars1];
  INT Dx[4];
  INT Dz[4];
  INT CurDir;
} bs7UNIT_MODEL;

static VOID BS7_RndPrimsLoadHeights( bs7UNIT_MODEL *Uni, CHAR *FileName )
{
  FILE *F;
  INT i = 0, j = 0;
  CHAR Buf[BS7_Max_Chars1];

  if ((F = fopen(FileName, "r")) != NULL)
  {
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      j = 0;
      for (j = 0; Buf[j] != 0 && j < BS7_Max_Chars1; j++)
      {
        Uni->Heights[i][j] = Buf[j];
        if (Buf[j] == 0)
          Uni->Pos = VecSet(i, 0, j);
      }
      i++;
    }
    fclose(F);
  }
}

static VOID BS7_UnitInit( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsLoad(&Uni->Cube, "bin/models/shelf.g3dm");
  Uni->Cube.Trans = MatrMulMatr(MatrScale(VecSet1(0.5)), MatrTranslate(VecSet(0, 0.5, 0)));
  BS7_RndPrimsLoad(&Uni->Pr, "bin/models/hollow_knight.g3dm");
  BS7_RndPrimsLoadHeights(Uni, "bin/heights/lab.txt");
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  Uni->Dx = {0, 1, 0, -1};
  Uni->Dz = {-1, 0, 1, 0};
  Uni->CurDir = 1;

  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(VecSet(-Ani->Keys['S'] + Ani->Keys['W'], 0, 0)));
  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(VecSet(0, 0, Ani->Keys['D'] - Ani->Keys['A'])));

  if (Uni->Heights[(int)Uni->Pos.X + 1][(int)Uni->Pos.Z] == '*')
    Uni->Pos = VecSet(Uni->Pos.X, 0, Uni->Pos.Z);
  BS7_RndCamSet(BS7_RndCamLoc, Uni->Pos, VecSet(0, 1, 0));
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  MATR m;
  CHAR Buf[100];

  m = MatrIdentity();
  m = MatrMulMatr(m, MatrScale(VecSet1(0.01)));
  m = MatrMulMatr(m, MatrRotateY(90)); 

  BS7_RndPrimsDraw(&Uni->Pr, MatrMulMatr(m, MatrTranslate(Uni->Pos)));

  BS7_RndPrimsDraw(&Uni->Cube, MatrTranslate(VecSet(floor(Uni->Pos.X), -0.9, floor(Uni->Pos.Z))));

  BS7_RndPrimsDraw(&Uni->Pr, MatrMulMatr(m, MatrTranslate(Uni->Pos)));
  sprintf(Buf, "%lf, %lf", Uni->Pos.X, Uni->Pos.Z);
  BS7_RndFntDraw(Buf, VecSet(0, -50, 0), 30);
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateModel( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_MODEL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  Uni->Close = (VOID *)BS7_UnitClose;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/