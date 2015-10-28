#pragma once

#include "../Controls/TabPage.h"

/// <summary>
/// Abstract class that encapsulates functionality for dealing with
/// property sheet pages (tabs).
/// </summary>
class SettingsTab : public TabPage {
public:
    SettingsTab(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title = L"");
    ~SettingsTab();

    /// <summary>Processes messages sent to the tab page.</summary>
    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    /// <summary>Persists changes made on the tab page</summary>
    virtual void SaveSettings() = 0;

protected:
    /// <summary>
    /// Performs intitialization for the tab page, similar to a constructor.
    /// Since tab page windows are created on demand, this method could be
    /// called much later than the constructor for the tab.
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>Applies the current settings state to the tab page.</summary>
    virtual void LoadSettings() = 0;
};