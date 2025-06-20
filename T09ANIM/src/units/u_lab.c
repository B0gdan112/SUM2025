/* FILE NAME: u_lab.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 19.06.2025
 */

#include "units/units.h"

#define BS7_Max_Chars 200

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIMS Pr;
  VEC Pos;
  CHAR Lab[BS7_Max_Chars][BS7_Max_Chars];
} bs7UNIT_LAB;

static VOID BS7_RndPrimsLoadLab( bs7UNIT_LAB *Uni, CHAR *FileName )
{
  FILE *F;
  INT i = 0, j = 0;
  CHAR Buf[BS7_Max_Chars];

  if ((F = fopen(FileName, "r")) != NULL)
  {
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      j = 0;
      for (j = 0; Buf[j] != 0 && j < BS7_Max_Chars; j++)
        Uni->Lab[i][j] = Buf[j];
      i++;
    }
    fclose(F);
  }
}

static VOID BS7_UnitInit( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsLoad(&Uni->Pr, "bin/models/shelf.g3dm");
  BS7_RndPrimsLoadLab(Uni, "bin/heights/lab.txt");
} /*End of 'BS7_UnitInit' function*/

static VOID BS7_UnitClose( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsFree(&Uni->Pr);
} /*End of 'BS7_UnitClose' function*/

static VOID BS7_UnitResponse( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
} /*End of 'BS7_UnitResponse' function*/

static VOID BS7_UnitRender( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
  INT i, j;
  MATR p;
  MATR m1, m = MatrIdentity();
  
  p = MatrIdentity();
  for (i = 0; i < BS7_Max_Chars; i++)
    for (j = 0; j < BS7_Max_Chars; j++)
      if (Uni->Lab[i][j] == '*')
      {
        m1 = MatrMulMatr(m, MatrMulMatr(MatrScale(VecSet1(0.5)), MatrTranslate(VecSet(i, 0.5, j))));
        BS7_RndPrimsDraw(&Uni->Pr, m1);
      }
} /*End of 'BS7_UnitResponse' function*/

bs7UNIT * BS7_UnitCreateLab( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_LAB))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  Uni->Close = (VOID *)BS7_UnitClose;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/