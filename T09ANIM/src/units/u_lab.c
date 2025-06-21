/* FILE NAME: u_BUG.c
 * PURPOSE: units module.
 * PROGRAMMER: BS7
 * DATE: 19.06.2025
 */


/*#include "units/units.h"

#define BS7_Max_Chars 200

typedef struct
{
  BS7_UNIT_BASE_FIELDS;
  bs7PRIMS Bug;
  VEC Pos;
  CHAR Lab[BS7_Max_Chars][BS7_Max_Chars];
} bs7UNIT_BUG;

static VOID BS7_RndPrimsLoadLab( bs7UNIT_BUG *Uni, CHAR *FileName )
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

static VOID BS7_UnitInit( bs7UNIT_BUG *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsLoad(&Uni->Bug, "bin/models/bug.g3dm");
  BS7_RndPrimsLoadLab(Uni, "bin/heights/Lab.txt");
  Uni->Pos = VecSet(0, 0, 0);
} 

static VOID BS7_UnitClose( bs7UNIT_BUG *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsFree(&Uni->Bug);
} 

static VOID BS7_UnitResponse( bs7UNIT_BUG *Uni, bs7ANIM *Ani )
{
} 

static VOID BS7_UnitRender( bs7UNIT_BUG *Uni, bs7ANIM *Ani )
{
  BS7_RndPrimsDraw(&Uni->Bug, MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet1(100))));
} 

bs7UNIT * BS7_UnitCreateBug( VOID )
{ 
  bs7UNIT *Uni;

  if((Uni = BS7_AnimUnitCreate(sizeof(bs7UNIT_BUG))) == NULL)
    return NULL;

  Uni->Init = (VOID *)BS7_UnitInit;
  Uni->Response = (VOID *)BS7_UnitResponse;
  Uni->Render = (VOID *)BS7_UnitRender;
  Uni->Close = (VOID *)BS7_UnitClose;
  return Uni;
} /*End of 'BS7_UnitResponse' function*/
