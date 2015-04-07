#pragma once
#include "SettingsUI.h"

#include <list>
#include <string>

class UIContext;

class General {
public:
    static DLGPROC GeneralTabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    static HWND _hWnd;
    static UIContext *_ctxt;
    static std::wstring _url;

    static void Command(UINT message, WPARAM wParam, LPARAM lParam);

    static std::list<std::wstring> FindSkins(std::wstring dir);
    static void LoadSettings(HWND hDlg);
    static void LoadSkinInfo(std::wstring skinName);
    static bool RunOnStartup();
};