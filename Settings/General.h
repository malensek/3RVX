#pragma once
#include "SettingsUI.h"

#include <list>
#include <string>

namespace General {
    DLGPROC GeneralTabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    std::list<std::wstring> FindSkins(std::wstring dir);
    void LoadSettings(HWND hDlg);
    void LoadSkinInfo();
    bool RunOnStartup();
}