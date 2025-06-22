/* FILE NAME: u_lab.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 19.06.2025
 */

#include "units/units.h"

#define BS7_Max_Chars 200

#if 0

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIM Lab1;
  VEC Pos;
  CHAR Lab[BS7_Max_Chars][BS7_Max_Chars];
} bs7UNIT_LAB;

static VOID BS7_RndPrimsLoadLab1( bs7UNIT_LAB *Uni, CHAR *FileName )
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
  bs7MATERIAL mtl;

  BS7_RndPrimCreate(&Uni->Lab1, BS7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  BS7_RndPrimsLoadLab1(Uni, "bin/heights/Lab.txt");

  mtl = BS7_RndMtlGetDef();
  strcpy(mtl.Name, "Lab Material");
  mtl.ShdNo = BS7_RndShdAdd("cube");
  Uni->Lab1.MtlNo = BS7_RndMtlAdd(&mtl);
} 

static VOID BS7_UnitClose( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimFree(&Uni->Lab1);
} 

static VOID BS7_UnitResponse( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
} 

static VOID BS7_UnitRender( bs7UNIT_LAB *Uni, bs7ANIM *Ani )
{
  MATR m1, m2 = MatrIdentity();

  m1 = MatrMulMatr(m2, MatrMulMatr(MatrScale(VecSet1(0.5)), MatrTranslate(VecSet(0, 0.5, 0))));
  BS7_RndPrimDraw(&Uni->Lab1, m1);

} 

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
#endif 