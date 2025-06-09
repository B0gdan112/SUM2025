/* FILE NAME  : t07globe.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "globe.h"
#include "mth.h"
#include "timer.h"

#include <windows.h>

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    GetWindowRect(hWnd, &SaveRect);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);

    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
 
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;  
}  /* End of 'FlipFullScreen' function */

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
  MATR m =
  {
    {
      {4, 3, 2, 1},
      {3, 2, 2, 1},
      {2, 2, 1, 0},
      {1, 4, 4, 1}
    }
  }, m1, m2;
  VEC v = VecSet(0, 8, 9), v1, v2;

  m1 = MatrInverse(m);
  m2 = MatrMulMatr(m, m1);
  m2 = MatrMulMatr(m1, m);

  m1 = MatrInverse(m);

  v1 = VecMulNum(v, 3);
  v2 = VecNeg(v);

  AllocConsole();

  SetConsoleTitle("Matrix");

  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 30;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
   printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 200, 0, 200, 0, 255, 100);
  PrintMatrix(m);
  printf("\n");
  PrintMatrix(m1);
  printf("\n");
  PrintMatrix(m2);
  printf("\n");
  PrintVec(v);
  printf("\n");
  PrintVec(v1);
  printf("\n");
  PrintVec(v2);
  printf("\n");
  printf("%f", MatrDeterm(m));
  fflush(stdout);


  FreeConsole();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_DESKTOP;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME,
    "Ball",
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
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  CHAR Buf[100];
  static FLOAT t, t1;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT W, H, size;
  static BITMAP bm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetStretchBltMode(hDC, COLORONCOLOR);
    SetTimer(hWnd, 1, 1, NULL);

    GLB_Init(1.0);
    GLB_TimerInit();

    return 0;

  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    size = W < H ? W : H;
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);

    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);

    GLB_Resize(W, H);

    return 0;

  case WM_PAINT:
    GetLocalTime(&st);

    hDC = BeginPaint(hWnd, &ps);

    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(170, 36, 76));
    Rectangle(hMemDC, 0, 0, W + 1, H);

    GLB_Draw(hMemDC, 5);

    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(0, 255, 0));
    TextOut(hMemDC, 30, 30, Buf, sprintf(Buf, "FPS: %lf", FPS));

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GLB_TimerResponse();
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
    {
      DeleteObject(hBm);
      DeleteDC(hMemDC);
      KillTimer(hWnd, 1);
      PostQuitMessage(0);
      return 0;
    }
    else if (wParam == VK_F11)
    {
      FlipFullScreen(hWnd);
      return 0;
    }
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MyWindowFunc' function */

/* END OF 't07globe.c' FILE */