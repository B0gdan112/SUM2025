/* FILE NAME: rndtex.c
 * PURPOSE: textures.
 * PROGRAMMER: BS7
 * DATE: 13.06.2025
 */

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "anim\rnd\rnd.h"

INT BS7_RndTexAddImg( CHAR *Name, INT w, INT h, INT C, VOID *ImageData )
{
  FLT mips;

  if (BS7_RndTexturesSize >= BS7_MAX_TEXTURES)
    return -1;

  glGenTextures(1, &BS7_RndTextures[BS7_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, BS7_RndTextures[BS7_RndTexturesSize].TexId);
  mips = log(w > h ? w : h) / log(2);
  mips = mips < 1 ? 1 : mips;
  glTexStorage2D(GL_TEXTURE_2D, mips, GL_RGB8, w, h);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(BS7_RndTextures[BS7_RndTexturesSize].Name, Name, BS7_STR_MAX - 1);
  BS7_RndTextures[BS7_RndTexturesSize].H = h;
  BS7_RndTextures[BS7_RndTexturesSize].W = w;
                                              
  glBindTexture(GL_TEXTURE_2D, 0);

  return BS7_RndTexturesSize++;
}

INT BS7_RndTexAddFromFile( CHAR *FileName )
{
  HBITMAP hBm;  /* .bmp: */
  INT TexNo;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP bm;
 
    GetObject(hBm, sizeof(bm), &bm);
    TexNo = BS7_RndTexAddImg(FileName, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);
    DeleteObject(hBm);
  }

  return TexNo;
}

VOID BS7_RndTexInit( VOID )
{
  BS7_RndTexturesSize = 0;
}

VOID BS7_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < BS7_RndTexturesSize; i++)
    glDeleteTextures(1, &BS7_RndTextures[i].TexId);
  BS7_RndTexturesSize = 0;
}
