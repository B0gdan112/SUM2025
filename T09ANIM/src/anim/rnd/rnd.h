/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include <string.h>
#include "def.h"

extern HWND BS7_hRndWnd;
extern HDC BS7_hRndDC;   /* Work window device context */
extern HGLRC BS7_hRndGLRC; /* openGL render context */
extern INT BS7_RndFrameW, BS7_RndFrameH;

extern DBL
  BS7_RndProjSize,     /* Project plane fit square */
  BS7_RndProjDist,     /* Distance to project plane from viewer (near) */
  BS7_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  BS7_RndMatrView, /* View coordinate system matrix */
  BS7_RndMatrProj, /* Projection coordinate system matrix */
  BS7_RndMatrVP;   /* Stored (View * Proj) matrix */

typedef struct tagbs7VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} bs7VERTEX;

typedef struct tagbs7PRIM
{
  bs7VERTEX *V;
  INT NumOfV;
  INT *I;
  INT NumOfI;
  MATR Trans;
} bs7PRIM;

VOID BS7_RndInit( HWND hWnd );
VOID BS7_RndClose( VOID );
VOID BS7_RndResize( INT W, INT H );
VOID BS7_RndCopyFrame( VOID );
VOID BS7_RndStart( VOID );
VOID BS7_RndEnd( VOID );
VOID BS7_RndProjSet( VOID );
VOID BS7_RndCamSet( VEC Loc, VEC At, VEC Up );
BOOL BS7_RndPrimCreate( bs7PRIM *Pr, INT NoofV, INT NoofI );
VOID BS7_RndPrimFree( bs7PRIM *Pr );
VOID BS7_RndPrimDraw( bs7PRIM *Pr, MATR World );
BOOL BS7_RndPrimLoad( bs7PRIM *Pr, CHAR *FileName );
BOOL BS7_RndPrimCreateSphere( bs7PRIM *Pr, DBL R, INT W, INT H );
VOID BS7_RndPrimTriMeshAutoNormals( bs7VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );

#endif /* __rnd_h_ */