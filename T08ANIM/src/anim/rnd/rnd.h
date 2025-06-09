/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#ifndef __rnd_h__
#define __rnd_h__

#include "def.h"

extern HWND BS7_hRndWnd;
extern HDC BS7_hRndDCFrame;
extern HBITMAP BS7_hRndBmFrame;
extern INT BS7_RndFrameW, BS7_RndFrameH;

extern DBL
  BS7_RndProjSize,     /* Project plane fit square */
  BS7_RndProjDist,     /* Distance to project plane from viewer (near) */
  BS7_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  BS7_RndMatrView, /* View coordinate system matrix */
  BS7_RndMatrProj, /* Projection coordinate system matrix */
  BS7_RndMatrVP;   /* Stored (View * Proj) matrix */

typedef struct tagbs7PRIM
{
  VEC P;
  INT NumOfV;

  INT *I;
  INT NumOfI;
  MATR Trans;
} bs7PRIM;

typedef struct tagbs7VERTEX
{
  VEC V;
} bs7VERTEX;

VOID BS7_RndInit( HWND hWnd );
VOID BS7_RndClose( VOID );
VOID BS7_RndResize( INT W, INT H );
VOID BS7_RndCopyFrame( HDC hDC );
VOID BS7_RndStart( VOID );
VOID BS7_RndEnd( VOID );
VOID BS7_RndProjSet( VOID );
VOID BS7_RndCamSet( VEC Loc, VEC At, VEC Up );

#endif /* __def_h_ */