/* FILE NAME: mth.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#ifndef __mth_h_
#define __mth_h_

#include <stdio.h>
#include <math.h>

#include <windows.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))
#define Degree2Radian(a) D2R(a)

#define VecAddVec3(A, B, C) VecAddVec(A, VecAddVec(B, C))
#define VecAddVec4(A, B, C, Â) VecAddVec(VecAddVec(A, B), VecAddVec(C, D))

#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr3(A, B, C), D)
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr(MatrMulMatr4(A, B, C, D), E)
#define MatrMulMatr6(A, B, C, D, E, F) MatrMulMatr(MatrMulMatr5(A, B, C, D, E), F)
#define MatrMulMatr7(A, B, C, D, E, F, G) MatrMulMatr(MatrMulMatr6(A, B, C, D, E, F), G)

/* Base float point types */
typedef double DBL;
typedef float FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tagVEC4
{
  FLT X, Y, Z, M; /* Vector coordinates */
} VEC4;

typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

/* Transformation matrix representation type */
typedef struct tagMATR
{
  FLT A[4][4]; /* Matrix elements */
} MATR;

/*                Vector functions */

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

__inline VEC VecSet1( FLT A )
{
  VEC r = {A, A, A};

  return r;
} /* End of 'VecSet1' function */

__inline VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT M )
{
  VEC4 v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  v.M = M;
  return v;
} /* End of 'Vec4Set' function */

__inline VEC4 Vec4Set1( FLT A )
{
  VEC4 r = {A, A, A, A};

  return r;
} /* End of 'Vec4Set1' function */

__inline VEC2 Vec2Set( FLT X, FLT Y )
{
  VEC2 v;

  v.X = X;
  v.Y = Y;
  return v;
} /* End of 'VecSet' function */

__inline VEC2 Vec2Set1( FLT A )
{
  VEC2 r = {A, A};

  return r;
} /* End of 'Vec2Set1' function */

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */


/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

__inline VEC VecMinVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X < V2.X ? V1.X : V2.X,
                V1.Y < V2.Y ? V1.Y : V2.Y,
                V1.Z < V2.Z ? V1.Z : V2.Z);
}

__inline VEC VecMaxVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X > V2.X ? V1.X : V2.X,
                V1.Y > V2.Y ? V1.Y : V2.Y,
                V1.Z > V2.Z ? V1.Z : V2.Z);
}

__inline VEC4 Vec4SetVec3( VEC P, FLT A )
{
  return Vec4Set(P.X, P.Y, P.Z, A);
}

__inline VEC VecDivNum( VEC V, FLT a )
{
  VEC r;

  r.X = V.X / a;
  r.Y = V.Y / a;
  r.Z = V.Z / a;

  return r;
} /* End of 'VecDivNum' function */

__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  FLT r;

  r = V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;

  return r;
} /* End of 'VecDotVec' function */

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecCrossVec' function */

__inline VEC VecMulNum( VEC V, FLT a )
{
  VEC r;

  r.X = V.X * a;
  r.Y = V.Y * a;
  r.Z = V.Z * a;

  return r;
} /* End of 'VecMulNum' function */

__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'VecLen' function */

__inline FLT VecLen2( VEC V )
{
  return VecLen(V) * VecLen(V);
} /* End of 'VecLen2' function */

__inline VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* End of 'VecNormalize' function */

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' function */

__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTrasform' function */

__inline VOID PrintVec( VEC r )
{
  printf("%f, %f, %f\n", r.X, r.Y, r.Z);
} /* End of 'PrintVec' function */

/*                Matrix functions */

__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
              FLT A10, FLT A11, FLT A12, FLT A13,
              FLT A20, FLT A21, FLT A22, FLT A23,
              FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR r =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33}
    }
  };
 
  return r;
} /* End of 'MatrSet' function */

__inline MATR MatrIdentity()
{
  MATR r =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return r;
} /* End of 'MatrIdentity' function */

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r = {{{0}}};
  INT i, j, k;
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
         r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' function */

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];

  return r;
} /* End of 'VecDotVec' function */

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

__inline FLT MatrDeterm( MATR M )
{
  return
    +M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                               M.A[2][1], M.A[2][2], M.A[2][3],
                               M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    +M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                               M.A[2][0], M.A[2][1], M.A[2][3],
                               M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);
  INT s, i, j, P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
 
  if (det == 0)
    return MatrIdentity();
 
  for (s = 1, i = 0; i < 4; i++)
    for (j = 0; j < 4; j++, s = -s)
      r.A[j][i] =
        s * MatrDeterm3x3(M.A[P[i][0]][P[j][0]], M.A[P[i][0]][P[j][1]], M.A[P[i][0]][P[j][2]],
                          M.A[P[i][1]][P[j][0]], M.A[P[i][1]][P[j][1]], M.A[P[i][1]][P[j][2]],
                          M.A[P[i][2]][P[j][0]], M.A[P[i][2]][P[j][1]], M.A[P[i][2]][P[j][2]]) / det;

  return r;
} /* End of 'MatrInverse' function */

__inline MATR MatrTranslate( VEC T )
{
  MATR r =
  {
    {
      {1.0, 0, 0, 0},
      {0, 1.0, 0, 0},
      {0, 0, 1.0, 0},
      {T.X, T.Y, T.Z, 1.0}
    }
  };

  return r;
} /* End of 'MatrTranslate' function */

__inline MATR MatrScale( VEC S )
{
  MATR r =
  {
    {
      {S.X, 0, 0, 0},
      {0, S.Y, 0, 0},
      {0, 0, S.Z, 0},
      {0, 0, 0, 1}
    }
  };

  return r;
} /* End of 'MatrScale' function */

__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree);
  MATR r =
  {
    {
      {cos(a), sin(a), 0, 0},
      {-sin(a), cos(a), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return r;
} /* End of 'MatrRotateX' function */

__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree);
  MATR r =
  {
    {
      {1, 0, 0, 0},
      {0, cos(a), sin(a),  0},
      {0, -sin(a), cos(a), 0},
      {0, 0, 0, 1}
    }
  };

  return r;
} /* End of 'MatrRotateY' function */

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree);
  MATR r =
  {
    {
      {cos(a), 0, -sin(a), 0},
      {0, 1, 0, 0},
      {sin(a), 0, cos(a), 0},
      {0, 0, 0, 1}
    }
  };

  return r;
} /* End of 'MatrRotateZ' function */

__inline VOID PrintMatrix( MATR r )
{
  INT i, j;

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
      printf(" %f", r.A[i][j]);
    printf("\n");
  }
} /* End of 'PrintMatr' function */

__inline MATR MatrRotate( FLT AngleInDegree, VEC V )
{
  FLT a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  VEC A = VecNormalize(V);
  MATR m =
  {
    {
      {c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s, 0},
      {A.Y * A.X * (1 - c) + A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * s, 0},
      {A.Z * A.X * (1 - c) - A.Y * s, A.Z * A.Y * (1 - c) - A.X * s, c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }  
  };

  return m;
} /* End of 'MatrRotate' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( FLT L, FLT R, FLT B, FLT T, FLT N, FLT F )
{
  MATR r =
  {
    {
      {2 * N / (R - L), 0, 0, 0},
      {0, 2 * N / (T - B), 0, 0},
      {(R + L) / (R - L), (T + B) / (T - B), (F + N) / (N - F), -1},
      {0, 0, 2 * N * F / (N - F), 0}
    }
  };

  return r;
}

#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */