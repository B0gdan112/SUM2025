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
  bs7PRIMS Pr, Cube, Lab1, Bug;
  VEC Pos, BPos, CamPos;
  CHAR Lab[BS7_Max_Chars1][BS7_Max_Chars1];
  INT CurDir;
  FLT x, z;
  BOOL CanMoveF, CanMoveB;
  INT map[BS7_Max_Chars1][BS7_Max_Chars1];
} bs7UNIT_MODEL;

static VOID BS7_RndPrimsLoadLab( bs7UNIT_MODEL *Uni, CHAR *FileName )
{
  FILE *F;
  INT i = 0, j = 0, b = 0, k = 0;
  CHAR Buf[BS7_Max_Chars1];

  if ((F = fopen(FileName, "r")) != NULL)
  {
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      j = 0;
      for (j = 0; Buf[j] != 0 && j < BS7_Max_Chars1; j++)
      {
        Uni->Lab[i][j] = Buf[j];
        if (Buf[j] == '0')
        {
          Uni->Pos.X = i;
          Uni->Pos.Z = j;
        }
      }
      i++;
    }
    fclose(F);
  }
}

static VOID BS7_UnitInit( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  VEC B;

  Uni->CurDir = 1;

  Uni->CamPos = VecSet(3, 2, 5);
  
  BS7_RndPrimsLoad(&Uni->Lab1, "bin/models/shelf.g3dm");
  BS7_RndPrimsLoadLab(Uni, "bin/heights/lab.txt");
               
  //BS7_RndPrimsLoad(&Uni->Cube, "bin/models/shelf.g3dm");
  Uni->Cube.Trans = MatrMulMatr(MatrScale(VecSet1(0.5)), MatrTranslate(VecSet(0, 0.5, 0)));
  BS7_RndPrimsLoad(&Uni->Pr, "bin/models/hollow_knight.g3dm");

  B = VecSubVec(Uni->Pr.MaxBB, Uni->Pr.MinBB);
  Uni->Pr.Trans =
    MatrMulMatr(MatrTranslate(VecAddVec(VecNeg(Uni->Pr.MinBB), VecSet(-B.X / 2, 0, -B.Z / 2))), 
                MatrScale(VecSet1(50 / B.Z)));
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsFree(&Uni->Lab1);
  //BS7_RndPrimsFree(&Uni->Bug);
  BS7_RndPrimsFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  INT Dx[4] = {0, 1, 0, -1};
  INT Dz[4] = {1, 0, -1, 0};
  INT k = 0;
  CHAR Buf[100];
  VEC POI, V;


  Uni->CanMoveF = TRUE;
  Uni->CanMoveB = TRUE;

  Uni->x = Uni->Pos.X;
  Uni->z = Uni->Pos.Z;

  /* Camera handle */

  V = VecSet(Dx[Uni->CurDir], 0, Dz[Uni->CurDir]);
  POI = VecAddVec(VecSubVec(Uni->Pos, VecMulNum(V, 2)), VecSet(0, 3, 0));
  Uni->CamPos= VecAddVec(Uni->CamPos, VecMulNum(VecSubVec(POI, Uni->CamPos), Ani->DeltaTime));

  if (!Ani->IsPause)
    BS7_RndCamSet(Uni->CamPos, Uni->Pos, VecSet(0, 1, 0));

  /* Player handle */
  if (Uni->Lab[Uni->CurDir < 2 ? 
    (int)floor(Uni->x) + Dx[Uni->CurDir] : (int)ceil(Uni->x) 
    + Dx[Uni->CurDir]][Uni->CurDir < 2 ? (int)floor(Uni->z) 
    + Dz[Uni->CurDir] : (int)ceil(Uni->z) + Dz[Uni->CurDir]] == '*')
    Uni->CanMoveF = FALSE;
  else
    Uni->CanMoveF = TRUE;

  if (Uni->Lab[Uni->CurDir > 1 ? 
    (int)floor(Uni->x) - Dx[Uni->CurDir] : (int)ceil(Uni->x) 
    - Dx[Uni->CurDir]][Uni->CurDir > 1 ? (int)floor(Uni->z) 
    - Dz[Uni->CurDir] : (int)ceil(Uni->z) - Dz[Uni->CurDir]] == '*')
    Uni->CanMoveB = FALSE;
  else
    Uni->CanMoveB = TRUE;

  if (Uni->CanMoveF)
  { 
    if (Ani->Keys['W'])
    {
      Uni->Pos.X += Dx[Uni->CurDir] * 0.5;
      Uni->Pos.Z += Dz[Uni->CurDir] * 0.5;
      Uni->x += Dx[Uni->CurDir] * 0.5;
      Uni->z += Dz[Uni->CurDir] * 0.5;
    }
  }
  if (Uni->CanMoveB)
  { 
    if (Ani->Keys['S'])
    {
      Uni->Pos.X -= Dx[Uni->CurDir] * 0.5;
      Uni->Pos.Z -= Dz[Uni->CurDir] * 0.5;
      Uni->x -= Dx[Uni->CurDir];
      Uni->z -= Dz[Uni->CurDir];
    }
  }

  if (Ani->KeysClick['D'])
    Uni->CurDir = (Uni->CurDir - 1) & 3;
  if (Ani->KeysClick['A'])
    Uni->CurDir = (Uni->CurDir + 1) & 3;

  /*enemy move*/
  for (i = ; i < BS7_Max_Chars1 - Uni->Pos.X; i++)
    for (j = 0; j < BS7_Max_Chars1 - Uni->Pos.Z; j++)
    {
      if (Lab[Uni->Pos.X + i][Uni->Pos.Z + j] != '*')
        map[Uni->Pos.X + i][Uni->Pos.Z + j] = k;
      k++;
      if (map[Uni->Pos.X + i - 1][Uni->Pos.Z] == k - 1 && map[Uni->Pos.X][Uni->Pos.Z + j - 1] == k - 1)
        map[Uni->Pos.X + i][Uni->Pos.Z] = k;
    }
  
  /* Text */
  sprintf(Buf, "%c", Uni->Lab[(int)ceil(Uni->x) + Dx[Uni->CurDir]][(int)ceil(Uni->z) + Dz[Uni->CurDir]]);
  BS7_RndFntDraw(Buf, VecSet(0, -100, 0), 30);
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_MODEL *Uni, bs7ANIM *Ani )
{
  MATR m;
  CHAR Buf[100];
  INT i, j;
  MATR p;
  MATR m1, m2 = MatrIdentity();
  

  m = MatrIdentity();
  m = MatrMulMatr(m, MatrScale(VecSet1(0.01)));
  m = MatrMulMatr(m, MatrRotateY(90 * Uni->CurDir));

  BS7_RndPrimsDraw(&Uni->Pr, MatrMulMatr(m, MatrTranslate(Uni->Pos)));

  //BS7_RndPrimsDraw(&Uni->Cube, MatrTranslate(VecSet(floor(Uni->Pos.X), -0.9, floor(Uni->Pos.Z))));

  p = MatrIdentity();
  for (i = 0; i < BS7_Max_Chars1; i++)
    for (j = 0; j < BS7_Max_Chars1; j++)
      if (Uni->Lab[i][j] == '*')
      {
        m1 = MatrMulMatr(m2, MatrMulMatr(MatrScale(VecSet1(0.5)), MatrTranslate(VecSet(i, 0.5, j))));
        BS7_RndPrimsDraw(&Uni->Lab1, m1);
      }
      else if (Uni->Lab[i][j] == 'E')
      {
        Uni->BPos.X = i;
        Uni->BPos.Z = j;
        BS7_RndPrimsDraw(&Uni->Pr, MatrMulMatr(MatrScale(VecSet1(0.01)), MatrTranslate(Uni->BPos)));
      }

  sprintf(Buf, "%lf, %lf\n %i\n %f %f\n", Uni->Pos.X, Uni->Pos.Z, Uni->CurDir, Uni->x, Uni->z);
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