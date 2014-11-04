// SettingsUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <iostream>

#include "SettingsUI.h"
#include "SettingsSheet.h"

#include "General.h"
#include "Display.h"

#include "../3RVX/StringUtils.h"
#include "../3RVX/Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSettingsUIApp

BEGIN_MESSAGE_MAP(CSettingsUIApp, CWinApp)

END_MESSAGE_MAP()


// CSettingsUIApp construction

CSettingsUIApp::CSettingsUIApp() {
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CSettingsUIApp object

CSettingsUIApp theApp;


// CSettingsUIApp initialization

BOOL CSettingsUIApp::InitInstance() {
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(
        RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    Settings s(L"../3RVX/Settings.xml");

    SettingsSheet settingsSheet(L"3RVX Settings");

    General g(&s);
    Display d;

    settingsSheet.AddPage(&g);
    settingsSheet.AddPage(&d);

    /* Disable help for the PropertySheet and its pages */
    settingsSheet.m_psh.dwFlags ^= PSH_HASHELP;
    for (int i = 0; i < settingsSheet.GetPageCount(); ++i) {
        CPropertyPage *page = settingsSheet.GetPage(i);
        page->m_psp.dwFlags ^= PSP_HASHELP;
    }

    m_pMainWnd = &settingsSheet;

    //((CButton *) g.GetDlgItem(IDC_CHECK3))->SetCheck(BST_CHECKED);
    //LoadCheckbox(3, "notifyicon", &g._notifyicon);
    INT_PTR nResponse = settingsSheet.DoModal();

    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    } else if (nResponse == -1) {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    // Delete the shell manager created above.
    if (pShellManager != NULL) {
        delete pShellManager;
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return false;
}