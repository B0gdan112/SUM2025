/* FILE NAME  : t03clock.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 04.06.2025
 * PURPOSE    : to make clock.
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

#include <windows.h>

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );

VOID DrawHand( HDC hDC, INT X, INT Y, INT R, INT r, INT g, INT b, INT W, double ti )
{
  INT i;
  double angle, si, co;
  HPEN hPen, hOldPen;
  POINT pnts[] =
  {
    {0, R}, {W / 2, 0}, {0, -(W / 2)}, {-(W / 2), 0}
  };
  POINT pnts1[4];

  angle = ti * 2 * 3.1415 / 60;
  si = sin(angle);
  co = cos(angle);
  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = X + pnts[i].x * co + pnts[i].y * si;
    pnts1[i].y = Y - pnts[i].y * co - pnts[i].x * si;
  }
  hPen = CreatePen(PS_SOLID, W, RGB(r, g, b));
  hOldPen = SelectObject(hDC, hPen);
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(r, g, b));
  Polygon(hDC, pnts1, sizeof(pnts1) / sizeof(pnts1[0]));
  SelectObject(hDC, GetStockObject(DC_PEN));
  DeleteObject(hPen);
}

/* End of DrawHand function */

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
  PAINTSTRUCT ps;
  HBRUSH hBr;
  POINT pt;
  SYSTEMTIME st;
  CHAR Buf[100];
  RECT rc;
  INT l, i;
  static FLOAT t, t1;
  static CHAR *WD[] =
  {
    "бя", "ом", "бр", "яп", "вр", "ор", "яа"
  };
  static HDC hMemDC, hMemDCLogo, hMemDCAnd, hMemDCXor;
  static HBITMAP hBm, hBmLogo, hBmAnd, hBmXor;
  static INT W, H, size;
  static BITMAP bm, bmAnd;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    hMemDCAnd = CreateCompatibleDC(hDC);
    hMemDCXor = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetStretchBltMode(hDC, COLORONCOLOR);
    SetTimer(hWnd, 1, 30, NULL);

    hBmLogo = LoadImage(NULL, "clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "mand.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "mxor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    SelectObject(hMemDCLogo, hBmLogo);
    SelectObject(hMemDCAnd, hBmAnd);
    SelectObject(hMemDCXor, hBmXor);
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

    GetObject(hBmLogo, sizeof(BITMAP), &bm);

    return 0;

  case WM_PAINT:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    GetLocalTime(&st);

    GetObject(hBmLogo, sizeof(BITMAP), &bm);
    GetObject(hBmAnd, sizeof(BITMAP), &bmAnd);

    hDC = BeginPaint(hWnd, &ps);

    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    SetDCPenColor(hDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W + 1, H + 1);

    SetStretchBltMode(hDC, COLORONCOLOR);
    StretchBlt(hMemDC, (W - size) / 2, (H - size) / 2, size, size, hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    DrawHand(hMemDC, W / 2, H / 2, size * 0.4, 255, 0, 0, size / 100, st.wSecond);
    DrawHand(hMemDC, W / 2, H / 2, size * 0.3, 0, 0, 0, size / 100, st.wMinute);
    DrawHand(hMemDC, W / 2, H / 2, size * 0.2, 0, 0, 0, size / 100, st.wHour % 12 * 5);

    l = wsprintf(Buf, "%02d.%02d.%02d %s", st.wDay, st.wMonth, st.wYear, WD[st.wDayOfWeek % 10]);

    rc.left = (W - size) / 2;
    rc.bottom = (H - size) / 2;
    rc.right = (W + size) / 2;
    rc.top = (H + size) / 2;
    DrawText(hMemDC, Buf, l, &rc,
      DT_VCENTER | DT_CENTER | DT_SINGLELINE );

    BitBlt(hMemDC, pt.x, pt.y, bmAnd.bmWidth, bmAnd.bmHeight, hMemDCAnd, 0, 0, SRCAND);
    BitBlt(hMemDC, pt.x, pt.y, bmAnd.bmWidth, bmAnd.bmHeight, hMemDCXor, 0, 0, SRCINVERT);

    t = (double)clock() / CLOCKS_PER_SEC;

    for (i = 0; i < 11; i++)
    {
      t1 = t - 2 * i;
      BitBlt(hMemDC, W / 2 - 50 + 200 * sin(2 * t1), H / 2 - 75 + 200 * cos(2 * t1), bmAnd.bmWidth, bmAnd.bmHeight, hMemDCAnd, 0, 0, SRCAND);
      BitBlt(hMemDC, W / 2 - 50 + 200 * sin(2 * t1), H / 2 - 75 + 200 * cos(2 * t1), bmAnd.bmWidth, bmAnd.bmHeight, hMemDCXor, 0, 0, SRCINVERT);
    }
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
    {
      DeleteObject(hBm);
      DeleteObject(hBmLogo);
      DeleteDC(hMemDC);
      DeleteDC(hMemDCLogo);
      DeleteObject(hBmXor);
      DeleteObject(hBmAnd);
      DeleteDC(hMemDCXor);
      DeleteDC(hMemDCAnd);
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
    DeleteObject(hBmLogo);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmXor);
    DeleteObject(hBmAnd);
    DeleteDC(hMemDCXor);
    DeleteDC(hMemDCAnd);
    KillTimer(hWnd, 1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MyWindowFunc' function */

/* END OF 't03clock.c' FILE */