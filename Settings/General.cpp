#include "General.h"

#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "UIUtils.h"

#define KEY_NAME L"3RVX"
#define STARTUP_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

DLGPROC General::GeneralTabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

    LPNMHDR lpnmhdr;

    switch (message) {
    case WM_INITDIALOG:
        LoadSettings(hDlg);
        break;

    case WM_COMMAND:
        //PropSheet_Changed(GetParent(hdlg), hdlg);
        break;

    case WM_NOTIFY:
        lpnmhdr = (NMHDR FAR *)lParam;

        switch (lpnmhdr->code) {
        case PSN_APPLY:   //sent when OK or Apply button pressed
            break;

        case PSN_RESET:   //sent when Cancel button pressed
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return FALSE;
}

void General::LoadSettings(HWND hDlg) {
    using namespace UIUtils;
    Settings *settings = Settings::Instance();
    SetCheck(hDlg, CHK_STARTUP, RunOnStartup());

//    _startup.SetCheck(RunOnStartup());
//    _notify.SetCheck(settings->NotifyIconEnabled());
//    _sounds.SetCheck(settings->SoundEffectsEnabled());

//    /* Determine which skins are available */
//    std::list<CString> skins = FindSkins(Settings::SkinDir().c_str());
//    for (CString skin : skins) {
//        _skins.AddString(skin);
//    }

//    /* Update the combo box with the current skin */
//    std::wstring current = settings->CurrentSkin();
//    int idx = _skins.SelectString(0, current.c_str());
//    if (idx == CB_ERR) {
//        _skins.SelectString(0, DEFAULT_SKIN);
//    }
//    LoadSkinInfo();

//    /* Populate the language box */
//    std::list<CString> languages = FindLanguages(
//        settings->LanguagesDir().c_str());

//    for (CString language : languages) {
//        _lang.AddString(language);
//    }
//    std::wstring currentLang = settings->LanguageName();
//    _lang.SelectString(0, currentLang.c_str());
}

bool General::RunOnStartup() {
    long res;
    HKEY key;
    bool run = false;

    res = RegOpenKeyEx(HKEY_CURRENT_USER, STARTUP_KEY, NULL, KEY_READ, &key);
    if (res == ERROR_SUCCESS) {
        res = RegQueryValueEx(key, KEY_NAME, NULL, NULL, NULL, NULL);
        if (res == ERROR_SUCCESS) {
            run = true;
        }
    }

    RegCloseKey(key);
    return run;
}