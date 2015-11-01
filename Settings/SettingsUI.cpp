#include "SettingsUI.h"

#include <iostream>

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

#include "../3RVX/3RVX.h"
#include "../3RVX/CommCtl.h"
#include "../3RVX/LanguageTranslator.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "Tabs/General.h"
#include "Tabs/Display.h"
#include "Tabs/OSD.h"
#include "Tabs/Hotkeys.h"
#include "Tabs/About.h"
#include "UITranslator.h"
#include "Updater/Updater.h"
#include "Updater/UpdaterWindow.h"

/* Needed to determine whether the Apply button is enabled/disabled */
static const int IDD_APPLYNOW = 0x3021;

const wchar_t *MUTEX_NAME = L"Local\\3RVXSettings";
HANDLE mutex;
HWND tabWnd = NULL;
bool relaunch = false;

int APIENTRY wWinMain(
        _In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPTSTR lpCmdLine,
        _In_ int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    Logger::Start();
    CLOG(L"Starting SettingsUI...");

    bool alreadyRunning = false;
    mutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (mutex) {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        alreadyRunning = true;
    }

    /* Inspect command line parameters to determine whether this settings
     * instance is being launched as an update checker. */
    std::wstring cmdLine(lpCmdLine);
    if (cmdLine.find(L"-update") != std::wstring::npos) {
        if (alreadyRunning) {
            return EXIT_SUCCESS;
        } else {
            /* If this is the only settings instance running, we release the 
             * mutex so that the user can launch the settings app. If this
             * happens, the updater is closed to prevent settings file race
             * conditions. */
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }

        if (Updater::NewerVersionAvailable()) {
            Settings::Instance()->Load();
            CLOG(L"An update is available. Showing update icon.");
            UpdaterWindow uw;
            PostMessage(
                uw.Handle(),
                _3RVX::WM_3RVX_SETTINGSCTRL,
                _3RVX::MSG_UPDATEICON,
                NULL);
            uw.DoModal();
        } else {
#if defined(ENABLE_3RVX_LOG) && (defined(ENABLE_3RVX_LOGTOFILE) == FALSE)
            CLOG(L"No update available. Press [enter] to terminate");
            std::cin.get();
#endif
        }

        /* Process was used for updates; time to quit. */
        return EXIT_SUCCESS;
    }

    if (alreadyRunning) {
        HWND settingsWnd = _3RVX::MasterSettingsHwnd();
        CLOG(L"A settings instance is already running. Moving window [%d] "
            L"to the foreground.", (int) settingsWnd);
        SetForegroundWindow(settingsWnd);
        SendMessage(
            settingsWnd,
            _3RVX::WM_3RVX_SETTINGSCTRL,
            _3RVX::MSG_ACTIVATE,
            NULL);

#if defined(ENABLE_3RVX_LOG) && (defined(ENABLE_3RVX_LOGTOFILE) == FALSE)
        CLOG(L"Press [enter] to terminate");
        std::cin.get();
#endif

        return EXIT_SUCCESS;
    }

    HWND updater = _3RVX::UpdaterHwnd();
    if (updater != 0) {
        CLOG(L"Telling updater to close");
        SendMessage(updater, WM_CLOSE, 0, 0);
    }

    SettingsUI mainWnd(hInstance);
    INT_PTR result;
    do {
        result = mainWnd.LaunchPropertySheet();
        CLOG(L"Relaunch: %s", relaunch ? L"TRUE" : L"FALSE");
    } while (relaunch == true);

    return result;
}

SettingsUI::SettingsUI(HINSTANCE hInstance) :
Window(
        Window::Builder(_3RVX::CLASS_3RVX_SETTINGS)
        .Title(_3RVX::CLASS_3RVX_SETTINGS)
        .InstanceHandle(hInstance)
        .Icon(LoadIcon(NULL, MAKEINTRESOURCE(IDI_SETTINGS)))
        .Build()) {

    Settings::Instance()->Load();

    _tabs.push_back((_general = new General));
    _tabs.push_back((_display = new Display));
    _tabs.push_back((_osd = new OSD));
    _tabs.push_back((_hotkeys = new Hotkeys));
    _tabs.push_back((_about = new About));
}

INT_PTR SettingsUI::LaunchPropertySheet() {
    HPROPSHEETPAGE *pages = new HPROPSHEETPAGE[_tabs.size()];
    for (size_t i = 0; i < _tabs.size(); ++i) {
        pages[i] = _tabs[i]->PageHandle();
    }

    PROPSHEETHEADER psh = { 0 };
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_USECALLBACK;
    psh.hwndParent = Window::Handle();
    psh.hInstance = Window::InstanceHandle();
    psh.pszIcon = MAKEINTRESOURCE(IDI_SETTINGS);
    psh.pszCaption = L"3RVX Settings";
    psh.nStartPage = 0;
    psh.nPages = _tabs.size();
    psh.phpage = pages;
    psh.pfnCallback = PropSheetProc;

    tabWnd = NULL;

    /* Position the window if this is the first launch */
    if (relaunch == false) {
        POINT pt = { 0 };
        GetCursorPos(&pt);
        MoveWindow(Window::Handle(),
            pt.x - XOFFSET, pt.y - YOFFSET, 0, 0, TRUE);
    }

    relaunch = false;

    CLOG(L"Launching modal property sheet.");
    return PropertySheet(&psh);
}

LRESULT SettingsUI::WndProc(
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

    if (message == _3RVX::WM_3RVX_SETTINGSCTRL) {
        switch (wParam) {
        case _3RVX::MSG_ACTIVATE:
            CLOG(L"Received request to activate window from external program");
            SetActiveWindow(tabWnd);
            break;

        case _3RVX::MSG_MUSTRESTART:
            relaunch = true;
            break;
        }
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
//                for (SettingsTab *tab : _tabs) {
//                    tab->SaveSettings();
//                }
                Settings::Instance()->Save();

                CLOG(L"Notifying 3RVX process of settings change");
                _3RVX::Message(_3RVX::MSG_LOAD, NULL, true);

                if (lParam == PSBTN_APPLYNOW && relaunch == true) {
                    /* Language was changed */
                    SendMessage(tabWnd, WM_CLOSE, NULL, NULL);
                } else {
                    relaunch = false;
                }
            }
        }
        break;
    }

    return TRUE;
}