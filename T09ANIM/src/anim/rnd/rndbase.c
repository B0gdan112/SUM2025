/* FILE NAME: rndbase.c
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: BS7
 * DATE: 12.06.2025
 */

#include "rnd.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

HGLRC BS7_hRndGLRC;

VOID BS7_RndInit( HWND hWnd )
{
  INT i, num;
  HGLRC hRC;
  PIXELFORMATDESCRIPTOR pfd = {0};
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };
 
  /* Store window handle */
  BS7_hRndWnd = hWnd;
 
  /* Get window DC */
  BS7_hRndDC = GetDC(hWnd);
 
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(BS7_hRndDC, &pfd);
  DescribePixelFormat(BS7_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(BS7_hRndDC, i, &pfd);
 
  /* OpenGL init: setup rendering context */
  BS7_hRndGLRC = wglCreateContext(BS7_hRndDC);
  wglMakeCurrent(BS7_hRndDC, BS7_hRndGLRC);
 
  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(BS7_hRndWnd, "Error extensions initializing", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }
 
  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(BS7_hRndDC, PixelAttribs, NULL, 1, &i, &num);
  hRC = wglCreateContextAttribsARB(BS7_hRndDC, NULL, ContextAttribs);
 
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(BS7_hRndGLRC);
 
  BS7_hRndGLRC = hRC;
  wglMakeCurrent(BS7_hRndDC, BS7_hRndGLRC);

#ifndef NDEBUG
  OutputDebugString(glGetString(GL_VERSION));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_VENDOR));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_RENDERER));
  OutputDebugString("\n");
 
  printf("Version : %s\n", glGetString(GL_VERSION));
  printf("Vendor  : %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
#endif /* NDEBUG */

#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, NULL);
#endif /* NDEBUG */

  wglSwapIntervalEXT(1);
  glEnable(GL_DEPTH_TEST);
 
  /* Set default render parameters */
  BS7_RndResize(47, 47);
  BS7_RndCamSet(VecSet1(8), VecSet1(0), VecSet(0, 1, 0));
}

VOID BS7_RndClose( VOID )
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(BS7_hRndGLRC);
  ReleaseDC(BS7_hRndWnd, BS7_hRndDC);
}

VOID BS7_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);
 
  /* Setup projection */
  BS7_RndFrameW = W;
  BS7_RndFrameH = H;
  BS7_RndProjSet();
}
 
VOID BS7_RndCopyFrame( VOID )
{
  SwapBuffers(BS7_hRndDC);
}
 
VOID BS7_RndStart( VOID )
{
  FLT ClearColor[4] = {0.30, 0.47, 0.8, 1};
  FLT DepthClearValue = 1;
 
  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, ClearColor);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID BS7_RndEnd( VOID )
{
  glFinish();
}

VOID BS7_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = BS7_RndProjSize;
  /* Correct aspect ratio */
  if (BS7_RndFrameW > BS7_RndFrameH)
    rx *= (DBL)BS7_RndFrameW / BS7_RndFrameH;
  else
    ry *= (DBL)BS7_RndFrameH / BS7_RndFrameW;
 
  BS7_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      BS7_RndProjDist, BS7_RndProjFarClip);
  BS7_RndMatrVP = MatrMulMatr(BS7_RndMatrView, BS7_RndMatrProj);
}
 
VOID BS7_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  BS7_RndMatrView = MatrView(Loc, At, Up);
  BS7_RndMatrVP = MatrMulMatr(BS7_RndMatrView, BS7_RndMatrProj);
}
