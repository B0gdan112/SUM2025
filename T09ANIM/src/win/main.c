/* FILE NAME  : main.c
 * PROGRAMMER : BS7
 * LAST UPDATE: 06.06.2025
 * PURPOSE    : draw a globe.
 *              Startup entry-point module.
 */

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
  CONSOLE_FONT_INFOEX cfi = {0};

  AllocConsole();

  SetConsoleTitle("Console");

  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 30;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 200, 0, 200, 0, 255, 100);
  printf("!!!!!!!!!\n");
  printf("Version : %s\n", glGetString(GL_VERSION));
  printf("Vendor  : %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  fflush(stdout); 

  SetDbgMemHooks();


  FreeConsole();
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
  //BS7_AnimUnitAdd(BS7_UnitCreateKuma());
  BS7_AnimUnitAdd(BS7_UnitCreateGrid());
  BS7_AnimUnitAdd(BS7_UnitCreateInit());
  BS7_AnimUnitAdd(BS7_UnitCreateModel());

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
  static BOOL SaveActivity;

  switch(Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y = 100;
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
  case WM_ACTIVATE:
    BS7_Anim.IsActive = LOWORD(wParam) != WA_INACTIVE;
    return 0;
  case WM_ENTERSIZEMOVE:
    SaveActivity = BS7_Anim.IsActive;
    BS7_Anim.IsActive = FALSE;
    return 0;
  case WM_EXITSIZEMOVE:
    BS7_Anim.IsActive = SaveActivity;
    return 0;
  case WM_TIMER:
    BS7_AnimRender();
    BS7_AnimCopyFrame();
    return 0;
  case WM_MOUSEWHEEL:
    BS7_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BS7_AnimRender();
    BS7_AnimCopyFrame();
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