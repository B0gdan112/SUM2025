/* FILE NAME: T05DET.C
 * PROGRAMMER: BS7
 * DATE: 05.06.2025
 * PURPOSE: count determinator for matrix.
 */
#include <stdio.h>
#include <conio.h>

#include <windows.h>

typedef DOUBLE DBL;
#define MAX 10

INT P[MAX];
BOOL IsParity = TRUE;
DBL A[MAX][MAX];
INT N;
DBL Det = 0;
 
BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;
 
  N = 0;
  if ((F = fopen("IN.TXT", "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
} /*End of 'LoadMatrix' function*/ 

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

void Go( INT Pos )
{
  INT i;
  DBL prod;

  if (Pos == N)
  {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity ? 1 : -1);
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      if (i != Pos)
      {
        Swap(&P[Pos], &P[i]);
        IsParity = !IsParity;
      }
      Go(Pos + 1);
      if (i != Pos)
      {
        Swap(&P[Pos], &P[i]);
        IsParity = !IsParity;
      }
    }
  }
} /* End of 'Go' function */

VOID main( VOID )
{ 
  int i;

  LoadMatrix("IN.TXT");
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  printf("Def is %f", Det);
  _getch();
}