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

/* Materials */

typedef struct tagbs7MATERIAL
{
  CHAR Name[BS7_STR_MAX]; /* Material name */
 
  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
 
  FLT Trans;                /* Transparency factor */
 
  INT Tex[8];               /* Texture references from texture table (or -1) */
 
  INT ShdNo;                /* Shader number in shader table */
} bs7MATERIAL;

/* Material stock */
#define BS7_MAX_MATERIALS 300
bs7MATERIAL BS7_RndMaterials[BS7_MAX_MATERIALS]; /* Array of materials */
INT BS7_RndMaterialsSize;

bs7MATERIAL BS7_RndMtlGetDef( VOID );

VOID BS7_RndMtlInit( VOID );

VOID BS7_RndMtlClose( VOID );

INT BS7_RndMtlAdd( bs7MATERIAL *Mtl );

UINT BS7_RndMtlApply( INT MtlNo );

/* Textures */

typedef struct tagbs7TEXTURE
{
  CHAR Name[BS7_STR_MAX];
  INT W, H;
  UINT TexId;
} bs7TEXTURE;

#define BS7_MAX_TEXTURES 30
bs7TEXTURE BS7_RndTextures[BS7_MAX_TEXTURES];
INT BS7_RndTexturesSize;

INT BS7_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *ImageData );
INT BS7_RndTexAddFromFile( CHAR *FileName );
INT BS7_RndTextureFree( INT TexNo );
VOID BS7_RndTexInit( VOID );
VOID BS7_RndTexClose( VOID );

#endif /* __rndres_h_ */