#pragma once
#include <Windows.h>

#include <list>
#include <string>

#include "Resource.h"

class UIContext;

class Tab {
public:
    Tab();
    ~Tab();

    /// <summary>Processes messages sent to the tab page.</summary>
    virtual DLGPROC TabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HWND _hWnd;
    UIContext *_ctxt;

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    /// <param name="ctrlId">Control identifier</param>
    virtual void Command(unsigned short nCode, unsigned short ctrlId) = 0;

    /// <summary>Applies the current settings state to the tab page.</summary>
    virtual void LoadSettings() = 0;

    /// <summary>Persists changes made on the tab page</summary>
    virtual void SaveSettings() = 0;
};