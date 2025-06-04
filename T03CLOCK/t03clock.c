/* FILE NAME  : t03clock.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 03.06.2025
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

VOID DrawHand( HDC hDC, INT X, INT Y, INT R, INT r, INT g, INT b, INT W, FLOAT ti )
{
  FLOAT angle;
  HPEN hPen, hOldPen;

  hPen = CreatePen(PS_SOLID, W, RGB(r, g, b));
  hOldPen = SelectObject(hDC, hPen);
  angle = ti * 2 * 3.1415 / 60;
  MoveToEx(hDC, X + R * sin(angle), Y - R * cos(angle), NULL);
  LineTo(hDC, X, Y);
  SelectObject(hDC, GetStockObject(DC_PEN));
  DeleteObject(hPen);
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
  PAINTSTRUCT ps;
  HBRUSH hBr;
  POINT pt;
  SYSTEMTIME st;
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm, hBmLogo;
  static INT W, H, size;
  static BITMAP bm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetStretchBltMode(hDC, COLORONCOLOR);
    SetTimer(hWnd, 1, 30, NULL);
    hBmLogo = LoadImage(NULL, "clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCLogo, hBmLogo);
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
    return 0;

  case WM_PAINT:
    GetLocalTime(&st);
    GetObject(hBmLogo, sizeof(BITMAP), &bm);
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    SetDCPenColor(hDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W + 1, H + 1);
    SetStretchBltMode(hDC, COLORONCOLOR);
    StretchBlt(hMemDC, (W - size) / 2, (H - size) / 2, size, size, hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    DrawHand(hMemDC, W / 2, H / 2, size * 0.4, 255, 0, 0, 2, (st.wSecond + st.wMilliseconds / 1000));
    DrawHand(hMemDC, W / 2, H / 2, size * 0.3, 0, 0, 0, 5, (st.wMinute + st.wSecond / 60));
    DrawHand(hMemDC, W / 2, H / 2, size * 0.2, 0, 0, 0, 5, (st.wHour % 12 + st.wMinute / 60));
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmLogo);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCLogo);
    KillTimer(hWnd, 1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MyWindowFunc' function */

/* END OF 't03clock.c' FILE */