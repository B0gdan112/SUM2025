/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 12.06.2025
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include <string.h>
#include "res/rndres.h"
#include "anim/anim.h"

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

typedef enum tagbs7PRIM_TYPE
{
  BS7_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  BS7_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  BS7_RND_PRIM_TRIMESH,
  BS7_RND_PRIM_TRISTRIP  /* Triangle mesh - array of triangles – GL_TRIANGLES */
} bs7PRIM_TYPE;

typedef struct tagbs7PRIM
{
  bs7PRIM_TYPE Type; /* Primitive type */
  UINT VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */
  INT NumOfElements; /* Number of indices/vecrtices */
  VEC MinBB, MaxBB;  /* Bound box */
  MATR Trans;   /* Additional transformation matrix */
} bs7PRIM;

VOID BS7_RndInit( HWND hWnd );
VOID BS7_RndClose( VOID );
VOID BS7_RndResize( INT W, INT H );
VOID BS7_RndCopyFrame( VOID );
VOID BS7_RndStart( VOID );
VOID BS7_RndEnd( VOID );
VOID BS7_RndProjSet( VOID );
VOID BS7_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID BS7_RndPrimCreate( bs7PRIM *Pr, bs7PRIM_TYPE Type, bs7VERTEX *V, INT NoofV, INT *Ind, INT NoofI  );
VOID BS7_RndPrimFree( bs7PRIM *Pr );
VOID BS7_RndPrimDraw( bs7PRIM *Pr, MATR World );
BOOL BS7_RndPrimLoad( bs7PRIM *Pr, CHAR *FileName );
BOOL BS7_RndPrimCreateSphere( bs7PRIM *Pr, DBL R, INT W, INT H );
VOID BS7_RndPrimTriMeshAutoNormals( bs7VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id, 
 *   - message severity:
 *       UINT severity, 
 *   - message text length:
 *       INT Length, 
 *   - message text:
 *       CHAR *Message, 
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message,
                             const VOID *UserParam );

/* Grid topology representation type */
typedef struct tagbs7GRID
{
  INT W, H;      /* Grid size (in vertices) */
  bs7VERTEX *V;  /* Array (2D) of vertex */
} bs7GRID;
 
/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndGridCreate( bs7GRID *G, INT W, INT H );
 
/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndGridFree( bs7GRID *G );
 
/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       BS7PRIM *Pr;
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndPrimFromGrid( bs7PRIM *Pr, bs7GRID *G );
 
/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 * RETURNS: None.
 */
VOID BS7_RndGridAutoNormals( bs7GRID *G );
 
/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       BS7GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL BS7_RndGridCreateSphere( bs7GRID *G, FLT R, INT W, INT H );

#endif /* __rnd_h_ */