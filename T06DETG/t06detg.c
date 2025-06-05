/* FILE NAME: T06DETG.C
 * PROGRAMMER: BS7
 * DATE: 05.06.2025
 * PURPOSE: count determinator for matrix with Gauss method.
 */
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

typedef DOUBLE DBL;
#define MAX 10

INT P[MAX];
BOOL IsParity = TRUE;
DBL A[MAX][MAX];
INT N;
 
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

VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

void DetEval( INT N )
{
  INT i, y, x, max_row, max_col, k, j;
  DBL coef, det = 1;

  for (i = 0; i < N; i++)
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
        {
          max_col = x;
          max_row = y;
        }
    if (A[max_row][max_col] == 0)
    {
      det = 0;
      break;
    }
    if (max_row != i)
    { 
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_row][x]);
      det = -det;
    }
    if (max_col != i)
    {
      for (y = i; y < N; y++)
        Swap(&A[y][i], &A[y][max_col]);
      det = -det;
    }
    for (k = i + 1; k < N; k++)
    {
      coef = A[k][i] / A[i][i];
      A[k][i] = 0;
      for (x = i + 1; x < N; x++)
        A[k][x] -= A[i][x] * coef;
    }

    det *= A[i][i];
  }
  printf("%lf", det);
} /* End of 'DetEval' function */

VOID main( VOID )
{
  int i;

  LoadMatrix("IN.TXT");
  for (i = 0; i < N; i++)
    P[i] = i;
  DetEval(N);
  _getch();
} /* End of main function */