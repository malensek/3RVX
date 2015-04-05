#include "stdafx.h"
#include "Settings.h"
#include <prsht.h>
#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND CreateWnd(HINSTANCE hInstance);
void InitializeDialog();
UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

void InitializeDialog() {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"3RVX SettingsUI";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        /* throw exception */
    }

    HWND hWnd = CreateWindowEx(
        NULL,
        L"3RVX SettingsUI", L"3RVX Settings",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL, NULL, hInst, NULL);

    PROPSHEETPAGE psp[3];
    PROPSHEETHEADER psh;

    //Fill out the PROPSHEETPAGE data structure for the Background Color sheet
    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = NULL;
    psp[0].hInstance = hInst;
    //psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE_SMALL);
    psp[0].pszIcon = NULL;
    //psp[0].pfnDlgProc = (DLGPROC) ButtonsDlgProc;
    psp[0].pszTitle = NULL;
    psp[0].lParam = NULL;

    //Fill out the PROPSHEETPAGE data structure for the Client Area Shape sheet
    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = NULL;
    psp[1].hInstance = hInst;
    //psp[1].pszTemplate = MAKEINTRESOURCE(IDD_SKIN_PROP);
    psp[1].pszIcon = NULL;
    //psp[1].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[1].pszTitle = NULL;
    psp[1].lParam = 0;

    //Fill out the PROPSHEETPAGE data structure for the Client Area Shape sheet
    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = NULL;
    psp[2].hInstance = hInst;
    //psp[2].pszTemplate = MAKEINTRESOURCE(IDD_ABOUT_PROP);
    psp[2].pszIcon = NULL;
    //psp[2].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[2].pszTitle = NULL;
    psp[2].lParam = 0;

    //Fill out the PROPSHEETHEADER
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_USECALLBACK;
    psh.hwndParent = hWnd;
    psh.hInstance = hInst;
    psh.pszIcon = MAKEINTRESOURCE(COLOR_WINDOW);
    psh.pszCaption = L"3RVX Settings";
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = (PFNPROPSHEETCALLBACK) PropSheetPageProc;

    //And finally display the modal property sheet
    PropertySheet(&psh);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp) {
    return uMsg;
}