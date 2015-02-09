#include "stdafx.h"
#include <iostream>

#include "SettingsUI.h"
#include "SettingsSheet.h"

#include "About.h"
#include "Display.h"
#include "General.h"
#include "Hotkeys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSettingsUIApp, CWinApp)

END_MESSAGE_MAP()

CSettingsUIApp::CSettingsUIApp() {

}

CSettingsUIApp theApp;

BOOL CSettingsUIApp::InitInstance() {
    /* InitCommonControlsEx() is required on Windows XP if an application
     * manifest specifies use of ComCtl32.dll version 6 or later to enable
     * visual styles.  Otherwise, any window creation will fail. */
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    /* Create the shell manager, in case the dialog contains any shell tree
     * view or shell list view controls. */
    CShellManager *pShellManager = new CShellManager;

    /* Activate "Windows Native" visual manager for enabling themes in MFC
     * controls */
    CMFCVisualManager::SetDefaultManager(
        RUNTIME_CLASS(CMFCVisualManagerWindows));

    SetRegistryKey(_T("3RVX Settings Editor"));

    SettingsSheet settingsSheet(L"3RVX Settings");
    General g;
    Display d;
    Hotkeys h;
    About a;

    UIUtils::pages.push_back(&g);
    UIUtils::pages.push_back(&d);
    UIUtils::pages.push_back(&h);
    UIUtils::pages.push_back(&a);

    for (CPropertyPage *page : UIUtils::pages) {
        settingsSheet.AddPage(page);
    }

    /* Disable help for the PropertySheet and its pages */
    settingsSheet.m_psh.dwFlags &= ~(PSH_HASHELP);
    for (int i = 0; i < settingsSheet.GetPageCount(); ++i) {
        CPropertyPage *page = settingsSheet.GetPage(i);
        page->m_psp.dwFlags &= ~(PSP_HASHELP);
    }

    m_pMainWnd = &settingsSheet;
    INT_PTR nResponse = settingsSheet.DoModal();

    if (nResponse == -1) {
        TRACE(traceAppMsg, 0,
            "Warning: dialog creation failed, so application is terminating " \
            "unexpectedly.\n");
        TRACE(traceAppMsg, 0,
            "Warning: if you are using MFC controls on the dialog, you cannot" \
            " #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    if (pShellManager != NULL) {
        delete pShellManager;
    }

    /* Since the dialog has been closed, return FALSE so that we exit the
     *  application, rather than start the application's message pump. */
    return FALSE;
}