#pragma once

#include <Windows.h>
#include <vector>

#include "Control.h"
#include "Controls.h"

class UIContext;
/// <summary>
/// Abstract class that encapsulates functionality for dealing with
/// property sheet pages (tabs).
/// </summary>
class Tab {
public:
    Tab();
    ~Tab();

    /// <summary>Processes messages sent to the tab page.</summary>
    virtual DLGPROC TabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>Persists changes made on the tab page</summary>
    virtual void SaveSettings() = 0;

protected:
    /// <summary>Window handle of this tab.</summary>
    HWND _hWnd;
    UIContext *_ctxt;

    /// <summary>
    /// Performs intitialization for the tab page, similar to a constructor.
    /// Since tab page windows are created on demand, this method could be
    /// called much later than the constructor for the tab.
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>Applies the current settings state to the tab page.</summary>
    virtual void LoadSettings() = 0;

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    /// <param name="ctrlId">Control identifier</param>
    virtual DLGPROC Command(unsigned short nCode, unsigned short ctrlId) = 0;

    /// <summary>Handles WM_NOTIFY messages.</summary>
    /// <param name="nHdr">Notification header structure</param>
    virtual DLGPROC Notification(NMHDR *nHdr) = 0;
};