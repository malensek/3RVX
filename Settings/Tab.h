#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Controls/Control.h"
#include "Controls/Controls.h"

class UIContext;

#define INIT_CONTROL(ctrlId, ctrlType, var) { \
    var = ctrlType(ctrlId, _hWnd); \
    _controlMap[ctrlId] = &var; \
}

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

    /// <summary>Maps control IDs to their respective instances.</summary>
    std::unordered_map<int, Control *> _controlMap;

    /// <summary>
    /// Performs intitialization for the tab page, similar to a constructor.
    /// Since tab page windows are created on demand, this method could be
    /// called much later than the constructor for the tab.
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>Applies the current settings state to the tab page.</summary>
    virtual void LoadSettings() = 0;
};