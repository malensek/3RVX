#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>

#include <prsht.h>

#include "resource.h"

/* Enable Visual Styles */
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

/* Forward Declarations */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateWnd(HINSTANCE hInstance);
void Initialize();
int CALLBACK PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam);
DLGPROC ComboDlgProc(HWND hdlg, UINT uMessage, WPARAM wParam, LPARAM lParam);

DLGPROC GeneralTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DLGPROC DisplayTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DLGPROC HotkeyTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DLGPROC AboutTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
