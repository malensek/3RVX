#include "stdafx.h"
#include "Settings.h"
#include <prsht.h>
#include <Windows.h>
#include <pshpack1.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

/* DLGTEMPLATEEX Structure */
#include <pshpack1.h>
typedef struct DLGTEMPLATEEX
{
    WORD dlgVer;
    WORD signature;
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    WORD cDlgItems;
    short x;
    short y;
    short cx;
    short cy;
} DLGTEMPLATEEX, *LPDLGTEMPLATEEX;
#include <poppack.h>

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

#include "../3RVX/Logger.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND mainWnd;

BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND CreateWnd(HINSTANCE hInstance);
void InitializeDialog();
UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp);
int CALLBACK PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam);
DLGPROC ComboDlgProc(HWND hdlg, UINT uMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    Logger::Start();

    CLOG(L"Starting SettingsUI...");
    InitializeDialog();

    /*
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
    */

	//return (int) msg.wParam;
    return 0;
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

    HWND mainWnd = CreateWindowEx(
        NULL, L"3RVX SettingsUI", L"3RVX SettingsUI",
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, hInst, NULL);

    PROPSHEETPAGE psp[4];

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = NULL;
    psp[0].hInstance = hInst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_GENERAL);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[0].pszTitle = NULL;
    psp[0].lParam = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = NULL;
    psp[1].hInstance = hInst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_DISPLAY);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[1].pszTitle = NULL;
    psp[1].lParam = 0;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = NULL;
    psp[2].hInstance = hInst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_HOTKEYS);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[2].pszTitle = NULL;
    psp[2].lParam = 0;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = NULL;
    psp[3].hInstance = hInst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_ABOUT);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = (DLGPROC) ComboDlgProc;
    psp[3].pszTitle = NULL;
    psp[3].lParam = 0;

    PROPSHEETHEADER psh;
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_USECALLBACK;
    psh.hwndParent = mainWnd;
    psh.hInstance = hInst;
    psh.pszIcon = MAKEINTRESOURCE(COLOR_WINDOW);
    psh.pszCaption = L"3RVX Settings";
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = (PFNPROPSHEETCALLBACK) PropSheetProc;

    CLOG(L"Launching modal property sheet.");
    PropertySheet(&psh);
}

LRESULT CALLBACK WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    CLOG(L"Message: %x", message);

    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
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

DLGPROC ComboDlgProc(HWND hdlg,
    UINT uMessage,
    WPARAM wParam,
    LPARAM lParam) {
    LPNMHDR     lpnmhdr;

    switch (uMessage) {
        // on any command notification, tell the property sheet to enable the Apply button
    case WM_COMMAND:
        //PropSheet_Changed(GetParent(hdlg), hdlg);
        break;

    case WM_NOTIFY:
        lpnmhdr = (NMHDR FAR *)lParam;

        switch (lpnmhdr->code) {
        case PSN_APPLY:   //sent when OK or Apply button pressed
            break;

        case PSN_RESET:   //sent when Cancel button pressed
            break;

        case PSN_SETACTIVE:
            //this will be ignored if the property sheet is not a wizard
            //PropSheet_SetWizButtons(GetParent(hdlg), PSWIZB_BACK | PSWIZB_FINISH);
            return FALSE;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return FALSE;
}

int CALLBACK PropSheetProc(HWND hWnd, UINT msg, LPARAM lParam) {
    switch (msg) {
    case PSCB_PRECREATE:
        if (((LPDLGTEMPLATEEX) lParam)->signature == 0xFFFF) {
            ((LPDLGTEMPLATEEX) lParam)->style &= ~DS_CONTEXTHELP;
        } else {
            ((LPDLGTEMPLATE) lParam)->style &= ~DS_CONTEXTHELP;
        }
        return TRUE;
    }

    return TRUE;
}

UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT msg, LPPROPSHEETPAGE ppsp) {
    /*
    if (uMsg == PSPCB_CREATE) {
        HWND startup = GetDlgItem(, CHK_STARTUP);
        CLOG(L"Startup: %d", startup);
        SendMessage(GetDlgItem(hWnd, CHK_STARTUP), BM_SETCHECK, BST_CHECKED, NULL);
    }
    */

    return msg;
}