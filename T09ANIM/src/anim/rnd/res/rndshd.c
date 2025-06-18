/* FILE NAME: rndres.c
 * PURPOSE: 3D animation project.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "anim\rnd\rnd.h"

static CHAR * BS7_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);

  rewind(F);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
}

static VOID BS7_RndShdLog( CHAR *FileNamePrefix, CHAR *Part, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/BS7{30}Shd.log", "a")) == NULL)
    return;
  fprintf(F, "%s%s\n%s\n", FileNamePrefix, Part, Text);
  printf("\x1b[38;2;%255;%100;%100mSHDLOG: %s%s\n%s\n\x1b[38;2;%255;%255;%255m", FileNamePrefix, Part, Text);
  fclose(F);
}

static UINT BS7_RndShdLoad( CHAR *FileNamePrefix )
{
  CHAR *txt;
  struct
  {
    CHAR *Name; /* Shader name (e.g. "vert") */
    INT Type;   /* Shader type (e.g. GL_VERETX_SHADER) */
    UINT Id;    /* Created shader Id */
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
    {"geom", GL_GEOMETRY_SHADER}
  };
  INT i, res, NoofS = sizeof(shds) / sizeof(shds[0]);
  UINT prg = 0;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
 
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader file name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shds[i].Name);
 
    /* Load shader file text */
    txt = BS7_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      if (i >= 2)
        continue;
      BS7_RndShdLog(FileNamePrefix, shds[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shds[i].Id = glCreateShader(shds[i].Type);
    if (shds[i].Id == 0)
    {
      free(txt);
      BS7_RndShdLog(FileNamePrefix, shds[i].Name, "Error create shader");
      is_ok = FALSE;
      break;
    }
 
    /* Attach shader source */
    glShaderSource(shds[i].Id, 1, &txt, NULL);
    free(txt);
 
    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      BS7_RndShdLog(FileNamePrefix, shds[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
 
  /* Create shader program */
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      BS7_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        BS7_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }
 
  /* Handle errors */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }
    /* Delete shader program */
    if (prg != 0)
      glDeleteProgram(prg);
  }
  return prg;
}

static VOID BS7_RndShdFree( INT Prg )
{
  UINT shds[5], n, i;
 
  if (Prg == 0 || !glIsProgram(Prg))
    return;
 
  glGetAttachedShaders(Prg, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(Prg, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(Prg);
}

bs7SHADER BS7_RndShaders[BS7_MAX_SHADERS];
INT BS7_RndShadersSize;

VOID BS7_RndShdInit( VOID )
{
  BS7_RndShadersSize = 0;
  BS7_RndShdAdd("default");
}

VOID BS7_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < BS7_RndShadersSize; i++)
    BS7_RndShdFree(BS7_RndShaders[i].ProgId);
  BS7_RndShadersSize = 0;
}

INT BS7_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < BS7_RndShadersSize; i++)
    if(strcmp(ShaderFileNamePrefix, BS7_RndShaders[i].Name) == 0)
      return i;

  if (BS7_RndShadersSize >= BS7_MAX_SHADERS)
    return 0;

  i = BS7_RndShadersSize++;
  strncpy(BS7_RndShaders[i].Name, ShaderFileNamePrefix, BS7_STR_MAX - 1);
  BS7_RndShaders[i].ProgId = BS7_RndShdLoad(ShaderFileNamePrefix);
  return i;
}

VOID BS7_RndShdUpdate( VOID )
{
  INT t;
  static INT OldTime = -1;
 
  t = clock();
  if (OldTime == -1)
    OldTime = t;
 
  if (t - OldTime > 2 * CLOCKS_PER_SEC)
  {
    INT i;
 
    for (i = 0; i < BS7_RndShadersSize; i++)
    {
      BS7_RndShdFree(BS7_RndShaders[i].ProgId);
      BS7_RndShaders[i].ProgId = BS7_RndShdLoad(BS7_RndShaders[i].Name);
    }
    OldTime = t;
  }
}
