#include "SettingsUI.h"

#include <iostream>

#include "../3RVX/CommCtl.h"

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

/* Needed to determine whether the Apply button is enabled/disabled */
#define IDD_APPLYNOW 0x3021

#include "../3RVX/3RVX.h"
#include "../3RVX/LanguageTranslator.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "UITranslator.h"

/* Tabs*/
#include "Tabs/Tab.h"
#include "Tabs/General.h"
#include "Tabs/Display.h"
#include "Tabs/Hotkeys.h"
#include "Tabs/About.h"
General general;
Display display;
Hotkeys hotkeys;
About about;
Tab *tabs[] = { &general, &display, &hotkeys, &about };

/* Startup x/y location offsets */
#define XOFFSET 70
#define YOFFSET 20

HANDLE mutex;
HWND mainWnd = NULL;
HWND tabWnd = NULL;

int APIENTRY wWinMain(
        _In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPTSTR lpCmdLine,
        _In_ int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    Logger::Start();
    CLOG(L"Starting SettingsUI...");

    mutex = CreateMutex(NULL, FALSE, L"Local\\3RVXSettings");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (mutex) {
            ReleaseMutex(mutex);
        }

        HWND settingsWnd = FindWindow(CLASS_3RVX_SETTINGS, CLASS_3RVX_SETTINGS);

        CLOG(L"A settings instance is already running. Moving window [%d] "
            L"to the foreground.", (int) settingsWnd);
        SetForegroundWindow(settingsWnd);
        SendMessage(settingsWnd, WM_3RVX_SETTINGSCTRL, MSG_ACTIVATE, NULL);

#if defined(ENABLE_3RVX_LOG) && (defined(ENABLE_3RVX_LOGTOFILE) == FALSE)
        CLOG(L"Press [enter] to terminate");
        std::cin.get();
#endif

        return EXIT_SUCCESS;
    }

    Settings *settings = Settings::Instance();
    settings->Load();

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_SETTINGS));
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW);
    wcex.lpszClassName = CLASS_3RVX_SETTINGS;

    if (RegisterClassEx(&wcex) == 0) {
        CLOG(L"Could not register class: %d", GetLastError());
        return EXIT_FAILURE;
    }

    mainWnd = CreateWindowEx(
        NULL, CLASS_3RVX_SETTINGS, CLASS_3RVX_SETTINGS, NULL,
        0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    PROPSHEETPAGE psp[4];

    LanguageTranslator *lt = settings->Translator();
    std::wstring genTitle = lt->Translate(std::wstring(L"General"));
    std::wstring dispTitle = lt->Translate(std::wstring(L"Display"));
    std::wstring hkTitle = lt->Translate(std::wstring(L"Hotkeys"));
    std::wstring aboutTitle = lt->Translate(std::wstring(L"About"));

    psp[0] = { 0 };
    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USETITLE;
    psp[0].hInstance = hInstance;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_GENERAL);
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = (DLGPROC) GeneralTabProc;
    psp[0].pszTitle = &genTitle[0];
    psp[0].lParam = NULL;

    psp[1] = { 0 };
    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USETITLE;
    psp[1].hInstance = hInstance;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_DISPLAY);
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = (DLGPROC) DisplayTabProc;
    psp[1].pszTitle = &dispTitle[0];
    psp[1].lParam = 0;

    psp[2] = { 0 };
    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = PSP_USETITLE;
    psp[2].hInstance = hInstance;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_HOTKEYS);
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = (DLGPROC) HotkeyTabProc;
    psp[2].pszTitle = &hkTitle[0];
    psp[2].lParam = 0;

    psp[3] = { 0 };
    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = PSP_USETITLE;
    psp[3].hInstance = hInstance;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_ABOUT);
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = (DLGPROC) AboutTabProc;
    psp[3].pszTitle = &aboutTitle[0];
    psp[3].lParam = 0;

    PROPSHEETHEADER psh = { 0 };
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_USECALLBACK;
    psh.hwndParent = mainWnd;
    psh.hInstance = hInstance;
    psh.pszIcon = MAKEINTRESOURCE(IDI_SETTINGS);
    psh.pszCaption = L"3RVX Settings";
    psh.nStartPage = 0;
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = (PFNPROPSHEETCALLBACK) PropSheetProc;

    /* Position the window */
    POINT pt = { 0 };
    GetCursorPos(&pt);
    MoveWindow(mainWnd, pt.x - XOFFSET, pt.y - YOFFSET, 0, 0, TRUE);

    CLOG(L"Launching modal property sheet.");
    return PropertySheet(&psh);
}

LRESULT CALLBACK WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
    }

    if (message == WM_3RVX_SETTINGSCTRL && wParam == MSG_ACTIVATE) {
        CLOG(L"Received request to activate window from external program");
        SetActiveWindow(tabWnd);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int CALLBACK PropSheetProc(HWND hWnd, UINT msg, LPARAM lParam) {
    switch (msg) {
    case PSCB_PRECREATE:
        /* Disable the help button: */
        if (((LPDLGTEMPLATEEX) lParam)->signature == 0xFFFF) {
            ((LPDLGTEMPLATEEX) lParam)->style &= ~DS_CONTEXTHELP;
        } else {
            ((LPDLGTEMPLATE) lParam)->style &= ~DS_CONTEXTHELP;
        }

        /* Show window in the taskbar: */
        ((LPDLGTEMPLATE) lParam)->dwExtendedStyle |= WS_EX_APPWINDOW;
        break;

    case PSCB_INITIALIZED:
        UITranslator::TranslateWindowText(hWnd);

        if (tabWnd == NULL) {
            tabWnd = hWnd;
        }

        /* These values are hard-coded in case the user has a non-english GUI
           but wants to change the program language */
        UITranslator::TranslateControlText(
            hWnd, IDOK, std::wstring(L"OK"));
        UITranslator::TranslateControlText(
            hWnd, IDCANCEL, std::wstring(L"Cancel"));
        UITranslator::TranslateControlText(
            hWnd, IDD_APPLYNOW, std::wstring(L"Apply"));
        break;

    case PSCB_BUTTONPRESSED:
        if (lParam == PSBTN_OK || lParam == PSBTN_APPLYNOW) {
            HWND hApply = GetDlgItem(hWnd, IDD_APPLYNOW);
            if (IsWindowEnabled(hApply)) {
                /* Save settings*/
                CLOG(L"Saving settings...");
                for (Tab *tab : tabs) {
                    tab->SaveSettings();
                }
                Settings::Instance()->Save();

                CLOG(L"Notifying 3RVX process of settings change");
                HWND masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");
                SendMessage(masterWnd, WM_3RVX_CONTROL, MSG_LOAD, NULL);
            }
        }
        break;
    }

    return TRUE;
}

DLGPROC GeneralTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return general.TabProc(hDlg, message, wParam, lParam);
}

DLGPROC DisplayTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return display.TabProc(hDlg, message, wParam, lParam);
}

DLGPROC HotkeyTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return hotkeys.TabProc(hDlg, message, wParam, lParam);
}

DLGPROC AboutTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    return about.TabProc(hDlg, message, wParam, lParam);
}