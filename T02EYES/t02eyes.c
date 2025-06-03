/* FILE NAME  : t02eyes.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 03.06.2025
 * PURPOSE    : to make eyes that will be moving with the mouse.
 *              Startup entry-point module.
 */

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

#include <math.h>
#include <time.h>
#include <stdlib.h>

#include <windows.h>

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );

VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  INT Xc, Yc;
  double k, Len;

  Len = sqrt((Mx - X) * (Mx - X) + (My - Y) * (My - Y));
  k = (R - R1) / Len;

  if ((R - R1) > Len)
  {
    Xc = Mx;
    Yc = My;
  }
  else
  {
    Xc = X + (Mx - X) * k;
    Yc = Y + (My - Y) * k;
  }

  SetDCBrushColor(hDC, RGB(255, 200, 200));
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, Xc - R1, Yc - R1, Xc + R1, Yc + R1);
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  static HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_DESKTOP;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_HAND);
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

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
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
  INT i;
  static INT W, H;
  static HDC hMemDC;
  static HBITMAP hBm;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 1, 30, NULL);
    return 0;

  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    return 0;

  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 0, 100));
    Rectangle(hMemDC, 0, 0, W, H);
    DrawEye(hMemDC, 700 + 250 * sin(3 * (double)clock() / CLOCKS_PER_SEC), 350 + 250 * cos(3 * (double)clock() / CLOCKS_PER_SEC), 100, 50, pt.x, pt.y);
    DrawEye(hMemDC, 1100 + 250 * sin(3 * (double)clock() / CLOCKS_PER_SEC), 350 + 250 * cos(3 * (double)clock() / CLOCKS_PER_SEC), 100, 50, pt.x, pt.y);
    srand(30);
    for (i = 0; i < 10; i++)
    {
      DrawEye(hMemDC, rand() % W, rand() % H, 100, 50, pt.x, pt.y);
    }
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 1);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MyWindowFunc' function */

/* END OF 't02eyes.c' FILE */