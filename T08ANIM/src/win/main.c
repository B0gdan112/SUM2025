/* FILE NAME  : main.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <time.h>

#include "anim/rnd/rnd.h"

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg,
                             WPARAM wParam, LPARAM lParam );

/* Main startup program function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstance;
 *   - previouse application instance handle
 *     (not used, alway NULL):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - command line window show parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) system error level value.
 */

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  static HWND hWnd;
  MSG msg;
  CONSOLE_FONT_INFOEX cfi = {0};

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_DESKTOP;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME,
    "Title",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 1, 0);
  }
  return msg.wParam;
}

/* End of 'WinMain' function */


/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message type (see WM_***):
 *       UINT Msg;
 *   - message 'word' parameter:
 *       WPARAM wParam;
 *   - message 'long' parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg,
                             WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  static bs7PRIM Pr, PrS, PrO;

  switch(Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y += 100;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0; 
  case WM_CREATE:
    BS7_RndInit(hWnd);
    if (BS7_RndPrimCreate(&Pr, 4, 6))
    {
      Pr.V[0].P = VecSet(0, 0, 0);
      Pr.V[1].P = VecSet(2, 0, 0);
      Pr.V[2].P = VecSet(0, 2, 0);
      Pr.V[3].P = VecSet(2, 2, 0);
 
      Pr.I[0] = 0;
      Pr.I[1] = 1;
      Pr.I[2] = 2;
 
      Pr.I[3] = 2;
      Pr.I[4] = 1;
      Pr.I[5] = 3;
    }
    BS7_RndPrimCreateSphere(&PrS, 2.0, 15, 15);
    BS7_RndPrimLoad(&PrO, "Monokuma.obj");
    SetTimer(hWnd, 1, 1, NULL);
    return 0;
  case WM_SIZE:
    BS7_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 1, 0);
    return 0;
  case WM_TIMER:
    BS7_RndStart();
    BS7_RndCamSet(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
    BS7_RndPrimDraw(&PrS, MatrRotateZ(1 * clock() / 1000.0));
    BS7_RndPrimDraw(&Pr, MatrRotateZ(3 * clock() / 1000.0));
    BS7_RndPrimDraw(&PrO, MatrMulMatr(MatrScale(VecSet(2, 2, 2)), MatrRotateZ(3 * clock() / 1000.0)));
    BS7_RndEnd();
    hDC = GetDC(hWnd);
    BS7_RndCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BS7_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    BS7_RndPrimFree(&PrS);
    BS7_RndPrimFree(&Pr);
    BS7_RndPrimFree(&PrO);
    BS7_RndClose();
    KillTimer(hWnd, 1);
    PostQuitMessage(1);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'WindowFunc' function */

/* END OF 't08anim.c' FILE */