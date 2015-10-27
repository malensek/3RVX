#include "General.h"

#include <shellapi.h>

#include "../../3RVX/3RVX.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../../3RVX/Skin/SkinInfo.h"
#include "../resource.h"
#include "../Updater/ProgressWindow.h"
#include "../Updater/Updater.h"

const wchar_t General::REGKEY_NAME[] = L"3RVX";
const wchar_t General::REGKEY_RUN[]
    = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

void General::Initialize() {
    INIT_CONTROL(GRP_BEHAVIOR, GroupBox, _behaviorGroup);
    INIT_CONTROL(CHK_STARTUP, Checkbox, _startup);
    INIT_CONTROL(CHK_SHOWSTARTUP, Checkbox, _showStartup);
    INIT_CONTROL(CHK_SOUNDS, Checkbox, _sounds);
    INIT_CONTROL(CHK_AUTOUPDATE, Checkbox, _autoUpdate);
    INIT_CONTROL(BTN_CHECK, Button, _checkNow);
    _checkNow.OnClick = std::bind(&General::CheckForUpdates, this);

    INIT_CONTROL(GRP_SKIN, GroupBox, _skinGroup);
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

    INIT_CONTROL(GRP_LANGUAGE, GroupBox, _languageGroup);
    INIT_CONTROL(CMB_LANG, ComboBox, _language);
}

void General::LoadSettings() {
    Settings *settings = Settings::Instance();
    LanguageTranslator *lt = settings->Translator();
    _startup.Checked(RunOnStartup());
    _showStartup.Checked(settings->ShowOnStartup());
    _sounds.Checked(settings->SoundEffectsEnabled());
    _autoUpdate.Checked(settings->AutomaticUpdates());

    /* Determine which skins are available */
    std::list<std::wstring> skins = FindSkins(Settings::SkinDir().c_str());
    for (std::wstring skin : skins) {
        _skin.AddItem(skin);
    }

    /* Update the combo box with the current skin */
    std::wstring current = settings->CurrentSkin();
    int idx = _skin.Select(current);
    if (idx == CB_ERR) {
        _skin.Select(Settings::DefaultSkin);
    }
    LoadSkinInfo(current);

    /* Populate the language box */
    std::list<std::wstring> languages = FindLanguages(
        settings->LanguagesDir().c_str());
    for (std::wstring language : languages) {
        int ext = language.find(L".xml");
        if (ext == language.npos) {
            continue;
        }

        _language.AddItem(language.substr(0, ext));
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
    settings->ShowOnStartup(_showStartup.Checked());
    settings->SoundEffectsEnabled(_sounds.Checked());
    settings->AutomaticUpdates(_autoUpdate.Checked());

    settings->CurrentSkin(_skin.Selection());

    std::wstring lang = _language.Selection();
    if (lang != settings->LanguageName()) {
        settings->LanguageName(lang);
        _3RVX::SettingsMessage(_3RVX::MSG_MUSTRESTART, NULL);
    }
}

bool General::RunOnStartup() {
    long res;
    HKEY key = NULL;
    bool run = false;

    res = RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY_RUN, NULL, KEY_READ, &key);
    if (res == ERROR_SUCCESS) {
        res = RegQueryValueEx(key, REGKEY_NAME, NULL, NULL, NULL, NULL);
        run = (res == ERROR_SUCCESS);
        RegCloseKey(key);
    }

    return run;
}

bool General::RunOnStartup(bool enable) {
    long res;
    HKEY key = NULL;
    bool ok = false;

    std::wstring path = Settings::AppDir() + L"\\3RVX.exe";

    res = RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY_RUN,
        NULL, KEY_ALL_ACCESS, &key);
    if (res == ERROR_SUCCESS) {
        if (enable) {
            res = RegSetValueEx(key, REGKEY_NAME, NULL, REG_SZ,
                (LPBYTE) path.c_str(), (path.size() + 1) * sizeof(TCHAR));
            ok = (res == ERROR_SUCCESS);
        } else {
            res = RegDeleteValue(key, REGKEY_NAME);
            ok = (res == ERROR_SUCCESS);
        }
        RegCloseKey(key);
    }

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

    std::wstring transAuthor
        = Settings::Instance()->Translator()->TranslateAndReplace(
        L"Author: {1}", s.Author());
    _author.Text(transAuthor);

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

bool General::CheckForUpdates() {
    _checkNow.Enabled(false);
    HCURSOR waitCursor = LoadCursor(NULL, IDC_WAIT);
    if (waitCursor) {
        SetCursor(waitCursor);
    }

    if (Updater::NewerVersionAvailable()) {
        Settings *settings = Settings::Instance();
        LanguageTranslator *translator = settings->Translator();
        Version vers = Updater::RemoteVersion();

        int msgResult = MessageBox(
            _hWnd,
            translator->TranslateAndReplace(
                L"A new version of 3RVX ({1}) is available. Install now?",
                vers.ToString()).c_str(),
            translator->Translate(L"Update Available").c_str(),
            MB_YESNO | MB_ICONQUESTION);

        if (msgResult == IDYES) {
            ProgressWindow *pw = new ProgressWindow(vers);
        }

    } else {
        MessageBox(
            _hWnd,
            L"Your copy of 3RVX is up-to-date.",
            L"Update Check",
            MB_OK | MB_ICONINFORMATION);
    }

    HCURSOR arrowCursor = LoadCursor(NULL, IDC_ARROW);
    if (arrowCursor) {
        SetCursor(arrowCursor);
    }
    _checkNow.Enabled(true);
    return true;
}