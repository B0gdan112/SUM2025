/* FILE NAME  : globe.h
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <windows.h>

#include "mth.h"

typedef double DBL;

VOID GLB_Init( DBL R );
VOID GLB_Resize( INT Ws, INT Hs );
VOID GLB_Draw( HDC hDC,  INT s );
VEC RotateX( VEC P, DBL Angle );
VEC RotateY( VEC P, DBL Angle );
VEC RotateZ( VEC P, DBL Angle );

