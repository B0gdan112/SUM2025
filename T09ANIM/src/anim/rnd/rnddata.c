/* FILE NAME: rnddata.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 09.06.2025
 */

#include "rnd.h"

HWND BS7_hRndWnd;
HDC BS7_hRndDC;
HGLRC BS7_hRndGC;
INT BS7_RndFrameW, BS7_RndFrameH;

DBL
  BS7_RndProjSize = 0.1,     /* Project plane fit square */
  BS7_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  BS7_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */
 
MATR
  BS7_RndMatrView, /* View coordinate system matrix */
  BS7_RndMatrProj, /* Projection coordinate system matrix */
  BS7_RndMatrVP;   /* Stored (View * Proj) matrix */