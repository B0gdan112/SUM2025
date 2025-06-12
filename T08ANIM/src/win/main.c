/* FILE NAME  : main.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <time.h>

#include "units/units.h"

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

  SetDbgMemHooks();

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
  BS7_AnimUnitAdd(BS7_UnitCreateKuma());
  BS7_AnimUnitAdd(BS7_UnitCreateInit());

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
} /* End of 'WinMain' function */

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
  INT BS7_MouseWheel = 0;

  switch(Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y += 100;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0; 
  case WM_CREATE:
    BS7_AnimInit(hWnd);
    SetTimer(hWnd, 1, 1, NULL);
    return 0;
  case WM_SIZE:
    BS7_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 1, 0);
    return 0;
  case WM_TIMER:
    BS7_AnimRender();

    hDC = GetDC(hWnd);
    BS7_AnimCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_MOUSEWHEEL:
    BS7_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  /*case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
    {
      DestroyWindow(hWnd);
      return 0;
    }
    else if (wParam == VK_F11)
    {
      BS7_AnimFlipFullScreen();
      return 0;
    }
    else if (wParam == 'P')
      BS7_Anim.IsPause = !BS7_Anim.IsPause;*/
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BS7_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    BS7_AnimClose();
    KillTimer(hWnd, 1);
    PostQuitMessage(1);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WindowFunc' function */

/* END OF 't08anim.c' FILE */