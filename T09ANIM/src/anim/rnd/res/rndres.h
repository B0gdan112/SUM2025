/* FILE NAME: rndres.h
 * PURPOSE: 3D animation project.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

VOID BS7_RndResInit( VOID );

VOID BS7_RndResClose( VOID );

#define BS7_STR_MAX 300

typedef struct tagbs7SHADER
{
  CHAR Name[BS7_STR_MAX];
  UINT ProgId;
} bs7SHADER;

#define BS7_MAX_SHADERS 30
extern bs7SHADER BS7_RndShaders[BS7_MAX_SHADERS];
extern INT BS7_RndShadersSize;

VOID BS7_RndShdInit( VOID );

VOID BS7_RndShdClose( VOID );

INT BS7_RndShdAdd( CHAR *ShaderFileNamePrefix );

VOID BS7_RndShdUpdate( VOID );



#endif /* __rndres_h_ */