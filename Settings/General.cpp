#include "General.h"

#include <shellapi.h>

#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "resource.h"

const wchar_t General::REGKEY_NAME[] = L"3RVX";
const wchar_t General::REGKEY_RUN[]
    = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

void General::Initialize() {
    INIT_CONTROL(CHK_STARTUP, Checkbox, _startup);
    INIT_CONTROL(CHK_NOTIFY, Checkbox, _notifyIcon);
    INIT_CONTROL(CHK_SOUNDS, Checkbox, _sounds);

    INIT_CONTROL(CMB_SKIN, ComboBox, _skin);
    _skin.OnSelectionChange = [this]() {
        LoadSkinInfo(_skin.Selection());
        return true;
    };
    INIT_CONTROL(LBL_AUTHOR, Label, _author);
    INIT_CONTROL(BTN_WEBSITE, Button, _website);
    _website.OnClick = [this]() {
        if (_url != L"") {
            ShellExecute(NULL, L"open", _url.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
        }
        return true;
    };

    INIT_CONTROL(CMB_LANG, ComboBox, _language);
    _language.OnSelectionChange = [this]() {
        // Handle language selection change 
        return true;
    };
}

void General::LoadSettings() {
    Settings *settings = Settings::Instance();
    _startup.Checked(RunOnStartup());
    _notifyIcon.Checked(settings->NotifyIconEnabled());
    _sounds.Checked(settings->SoundEffectsEnabled());

    /* Determine which skins are available */
    std::list<std::wstring> skins = FindSkins(Settings::SkinDir().c_str());
    for (std::wstring skin : skins) {
        _skin.AddItem(skin);
    }

    /* Update the combo box with the current skin */
    std::wstring current = settings->CurrentSkin();
    int idx = _skin.Select(current);
    if (idx == CB_ERR) {
        _skin.Select(DEFAULT_SKIN);
    }
    LoadSkinInfo(current);

    /* Populate the language box */
    std::list<std::wstring> languages = FindLanguages(
        settings->LanguagesDir().c_str());
    for (std::wstring language : languages) {
        _language.AddItem(language);
    }
    std::wstring currentLang = settings->LanguageName();
    _language.Select(currentLang);
}

void General::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: General");
    Settings *settings = Settings::Instance();

    RunOnStartup(_startup.Checked());
    settings->NotifyIconEnabled(_notifyIcon.Checked());
    settings->SoundEffectsEnabled(_sounds.Checked());

    settings->CurrentSkin(_skin.Selection());
}

bool General::RunOnStartup() {
    long res;
    HKEY key;
    bool run = false;

    res = RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY_RUN, NULL, KEY_READ, &key);
    if (res == ERROR_SUCCESS) {
        res = RegQueryValueEx(key, REGKEY_NAME, NULL, NULL, NULL, NULL);
        run = (res == ERROR_SUCCESS);
    }

    RegCloseKey(key);
    return run;
}

bool General::RunOnStartup(bool enable) {
    long res;
    HKEY key;
    bool ok = false;

    std::wstring path = Settings::AppDir() + L"\\3RVX.exe";

    res = RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY_RUN,
        NULL, KEY_ALL_ACCESS, &key);
    if (res == ERROR_SUCCESS) {
        if (enable) {
            res = RegSetValueEx(key, REGKEY_NAME, NULL, REG_SZ,
                (LPBYTE) path.c_str(), path.size() + 1);
            ok = (res == ERROR_SUCCESS);
        } else {
            res = RegDeleteValue(key, REGKEY_NAME);
            ok = (res == ERROR_SUCCESS);
        }
    }
    RegCloseKey(key);
    return ok;
}

std::list<std::wstring> General::FindSkins(std::wstring dir) {
    std::list<std::wstring> skins;
    WIN32_FIND_DATA ffd;
    HANDLE hFind;

    CLOG(L"Finding skins in: %s", dir.c_str());
    dir += L"\\*";
    hFind = FindFirstFile(dir.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        CLOG(L"FindFirstFile() failed");
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
    _author.Text(authorText);

    std::wstring url = s.URL();
    if (url == L"") {
        _website.Disable();
    } else {
        _url = s.URL();
        _website.Enable();
    }
}

std::list<std::wstring> General::FindLanguages(std::wstring dir) {
    std::list<std::wstring> languages;
    WIN32_FIND_DATA ffd;
    HANDLE hFind;

    CLOG(L"Finding language translations in: %s", dir.c_str());
    dir += L"\\*.xml";
    hFind = FindFirstFile(dir.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        CLOG(L"FindFirstFile() failed");
        return languages;
    }

    do {
        std::wstring fName(ffd.cFileName);

        if (fName.at(0) == L'.') {
            continue;
        }

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        QCLOG(L"%s", fName.c_str());
        languages.push_back(fName);
    } while (FindNextFile(hFind, &ffd));
    FindClose(hFind);

    return languages;
}