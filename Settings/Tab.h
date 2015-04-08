#pragma once

#include <Windows.h>
#include <list>
#include <string>

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
    HWND _hWnd;
    UIContext *_ctxt;

    /// <summary>Applies the current settings state to the tab page.</summary>
    virtual void LoadSettings() = 0;

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    /// <param name="ctrlId">Control identifier</param>
    virtual DLGPROC Command(unsigned short nCode, unsigned short ctrlId) = 0;
};