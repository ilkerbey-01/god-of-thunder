#include "win_man.h"

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

void win_man_initialize() {
#ifdef WIN32
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_MINIMIZE);  //won't hide the window without SW_MINIMIZE
  ShowWindow(hWnd, SW_HIDE);
#endif
}

void win_man_close() {
#ifdef WIN32
#ifdef DEBUG
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_SHOW);
  ShowWindow(hWnd, SW_NORMAL);
#endif
#endif
}
