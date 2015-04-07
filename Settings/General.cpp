#include "General.h"

#include <shellapi.h>

#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"
#include "UIUtils.h"
#include "UIContext.h"

#define KEY_NAME L"3RVX"
#define STARTUP_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

HWND General::_hWnd = NULL;
UIContext *General::_ctxt = NULL;
std::wstring General::_url(L"");

DLGPROC General::GeneralTabProc(
        HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

    LPNMHDR lpnmhdr;

    switch (message) {
    case WM_INITDIALOG:
        _hWnd = hDlg;
        _ctxt = new UIContext(hDlg);
        LoadSettings(hDlg);
        break;

    case WM_COMMAND:
        Command(message, wParam, lParam);
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

/// <summary>Handles WM_COMMAND messages.</summary>
void General::Command(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (HIWORD(wParam)) {
    case BN_CLICKED: {
        unsigned short btnId = LOWORD(wParam);
        if (btnId == BTN_WEBSITE) {
            if (_url != L"") {
                ShellExecute(NULL, L"open", _url.c_str(),
                    NULL, NULL, SW_SHOWNORMAL);
            }
        }
    }
    break;

    case CBN_SELCHANGE: {
        unsigned short cmbId = LOWORD(wParam);
        if (cmbId == CMB_SKIN) {
            LoadSkinInfo(_ctxt->GetComboSelection(CMB_SKIN));
        } else if (cmbId == CMB_LANG) {
            // Language selection
        }
    }
    break;
    }
}

void General::LoadSettings(HWND hDlg) {
    Settings *settings = Settings::Instance();
    _ctxt->SetCheck(CHK_STARTUP, RunOnStartup());
    _ctxt->SetCheck(CHK_NOTIFY, settings->NotifyIconEnabled());
    _ctxt->SetCheck(CHK_SOUNDS, settings->SoundEffectsEnabled());

    /* Determine which skins are available */
    std::list<std::wstring> skins = FindSkins(Settings::SkinDir().c_str());
    for (std::wstring skin : skins) {
        _ctxt->AddComboItem(CMB_SKIN, skin);
    }

    /* Update the combo box with the current skin */
    std::wstring current = settings->CurrentSkin();
    int idx = _ctxt->SelectComboItem(CMB_SKIN, current);
    if (idx == CB_ERR) {
        _ctxt->SelectComboItem(CMB_SKIN, DEFAULT_SKIN);
    }
    LoadSkinInfo(current);

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

std::list<std::wstring> General::FindSkins(std::wstring dir) {
    std::list<std::wstring> skins;
    WIN32_FIND_DATA ffd;
    HANDLE hFind;

    CLOG(L"Finding skins in: %s", dir.c_str());
    dir += L"\\*";
    hFind = FindFirstFile(dir.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return skins;
    }

    do {
        std::wstring fName(ffd.cFileName);
        if (fName.at(0) == L'.') {
            continue;
        }
        QCLOG(L"%s", fName.c_str());
        skins.push_back(fName);
    } while (FindNextFile(hFind, &ffd));
    FindClose(hFind);

    return skins;
}

void General::LoadSkinInfo(std::wstring skinName) {
    std::wstring skinXML = Settings::Instance()->SkinXML(skinName);
    SkinInfo s(skinXML);

    std::wstring authorText(L"Author: ");
    authorText.append(s.Author());
    _ctxt->SetText(LBL_AUTHOR, authorText);

    std::wstring url = s.URL();
    if (url == L"") {
        _ctxt->Disable(BTN_WEBSITE);
    } else {
        _url = s.URL();
        _ctxt->Enable(BTN_WEBSITE);
    }
}