#pragma once
#include "SettingsUI.h"

namespace General {
    DLGPROC GeneralTabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    void LoadSettings(HWND hDlg);
    bool RunOnStartup();
}