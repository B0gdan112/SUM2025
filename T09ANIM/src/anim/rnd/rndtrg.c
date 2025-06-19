/* FILE NAME: rndtrgt.c
 * PURPOSE: Render target.
 * PROGRAMMER: BS7
 * DATE: 19.06.2025
 */
 
#include "rnd.h"

/* Color textures */
#define BS7_RND_TARGETS 2
static INT BS7_RndTargetTexNo[BS7_RND_TARGETS];

/* FBO number */
static UINT BS7_RndTargetFBO;

/* Render buffer */
static INT BS7_RndTargetRndBuf;
 
/* Target shaders */
static INT BS7_RndTargetShdNo;
 
/* Target material */
static INT BS7_RndTargetMtlNo;
 
/* Target full screen primitive */
static bs7PRIM BS7_RndTargetPr;
 
/* Initialize render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndTargetInit( VOID )
{
  INT i;
  bs7MATERIAL mtl = BS7_RndMtlGetDef();
 
  strncpy(mtl.Name, "Target Material", BS7_STR_MAX - 1);
  mtl.ShdNo = BS7_RndTargetShdNo = BS7_RndShdAdd("target0");
 
  BS7_RndTargetCreate(BS7_RndFrameW, BS7_RndFrameH);
  for (i = 0; i < BS7_RND_TARGETS; i++)
    mtl.Tex[i] = BS7_RndTargetTexNo[i];
 
  BS7_RndPrimCreate(&BS7_RndTargetPr, BS7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
 
  BS7_RndTargetPr.MtlNo = BS7_RndTargetMtlNo = BS7_RndMtlAdd(&mtl);
} /* End of 'BS7_RndTargetInit' function */
 
/* Deinitialize render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndTargetClose( VOID )
{
  BS7_RndTargetFree();
} /* End of 'BS7_RndTargetClose' function */
 
/* Create render target function.
 * ARGUMENTS:
 *   - target frame buffer size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID BS7_RndTargetCreate( INT W, INT H )
{
  INT i, DrawBuffer[BS7_RND_TARGETS], status;
 
  /* Create frame buffer */
  glGenFramebuffers(1, &BS7_RndTargetFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, BS7_RndTargetFBO);
 
  /* Create attachment textures */
  for (i = 0; i < BS7_RND_TARGETS; i++)
  {
    DrawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
    BS7_RndTargetTexNo[i] = BS7_RndTexAddFmt("Attachment", W, H, GL_RGBA32F);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
      GL_TEXTURE_2D, BS7_RndTextures[BS7_RndTargetTexNo[i]].TexId, 0);
  }
  glDrawBuffers(BS7_RND_TARGETS, DrawBuffer);
 
  /* Create depth bvuffer as render buffer */
  glGenRenderbuffers(1, &BS7_RndTargetRndBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, BS7_RndTargetRndBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, W, H);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    GL_RENDERBUFFER, BS7_RndTargetRndBuf);
 
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugString("Error create FBO\n");
 
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} /* End of 'BS7_RndTargetCreate' function */
 
/* Free render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndTargetFree( VOID )
{ 
  glBindFramebuffer(GL_FRAMEBUFFER, BS7_RndTargetFBO);
 
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  glDeleteRenderbuffers(1, &BS7_RndTargetRndBuf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &BS7_RndTargetFBO);
} /* End of 'BS7_RndTargetFree' function */
 
/* Start frame through target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndTargetStart( VOID )
{
  INT i;
  FLT col[BS7_RND_TARGETS][4] = {{0, 0, 0, 0}}, depth = 1;
 
  glBindFramebuffer(GL_FRAMEBUFFER, BS7_RndTargetFBO);
  for (i = 0; i < BS7_RND_TARGETS; i++)
    glClearBufferfv(GL_COLOR, i, col[i]);
  glClearBufferfv(GL_DEPTH, 0, &depth);
  glViewport(0, 0, BS7_RndFrameW, BS7_RndFrameH);
} /* End of 'BS7_RndTargetStart' function */
 
 
/* Finalize frame through target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BS7_RndTargetEnd( VOID )
{
  FLT col[4] = {0.30, 0.47, 0.8, 1}, depth = 1;
 
  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, BS7_RndFrameW, BS7_RndFrameH);
  glClearBufferfv(GL_COLOR, 0, col);
  glClearBufferfv(GL_DEPTH, 0, &depth);
 
  BS7_RndPrimDraw(&BS7_RndTargetPr, MatrIdentity());
  glFinish();
} /* End of 'BS7_RndTargetEnd' function */
/* Resize render target frame buffer function.
 * ARGUMENTS:
 *   - target frame buffer size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID BS7_RndTargetResize( INT W, INT H )
{
  BS7_RndTargetFree();
  BS7_RndTargetCreate(W, H);
} /* End of 'BS7_RndTargetResize' function */
 
/* END OF 'rndtrgt.c' FILE */