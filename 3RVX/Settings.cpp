// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Settings.h"

#pragma comment(lib, "Shlwapi.lib")

#include <algorithm>
#include <ctime>
#include <ShlObj.h>
#include <Shlwapi.h>

#include "Error.h"
#include "HotkeyInfo.h"
#include "LanguageTranslator.h"
#include "Logger.h"
#include "Monitor.h"
#include "Skin/Skin.h"
#include "StringUtils.h"

std::wstring Settings::_appDir(L"");
Settings *Settings::instance;

std::vector<std::wstring> Settings::OSDPosNames = {
    L"Top",
    L"Left",
    L"Right",
    L"Bottom",
    L"Center",
    L"Top-left",
    L"Top-right",
    L"Bottom-left",
    L"Bottom-right",
    L"Custom"
};

Settings *Settings::Instance() {
    if (instance == NULL) {
        instance = new Settings();
    }
    return instance;
}

void Settings::Load() {
    /* First, clean up (if needed) */
    delete _translator;
    _translator = NULL;

    _file = SettingsFile();
    CLOG(L"Loading settings: %s", _file.c_str());

    FILE *fp;
    _wfopen_s(&fp, _file.c_str(), L"rb");
    if (fp == NULL) {
        QCLOG(L"Failed to open file!");
        LoadEmptySettings();
        return;
    }

    tinyxml2::XMLError result = _xml.LoadFile(fp);
    fclose(fp);
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        LoadEmptySettings();
        return;
    }

    _root = _xml.GetDocument()->FirstChildElement("settings");
    if (_root == NULL) {
        Error::ErrorMessage(Error::GENERR_MISSING_XML, L"<settings>");
        LoadEmptySettings();
        return;
    }
}

void Settings::LoadEmptySettings() {
    _xml.Clear();
    _xml.InsertFirstChild(_xml.NewDeclaration());
    _root = _xml.NewElement("settings");
    _xml.GetDocument()->InsertEndChild(_root);
}

int Settings::Save() {
    CreateSettingsDir();
    FILE *stream;
    errno_t err = _wfopen_s(&stream, _file.c_str(), L"w");
    if (err != 0 || stream == NULL) {
        CLOG(L"Could not open settings file for writing!");
        return 100 + err;
    }
    tinyxml2::XMLError result = _xml.SaveFile(stream);
    fclose(stream);
    return result;
}

bool Settings::Portable() {
    std::wstring portableSettings = AppDir() + L"\\" + SETTINGS_FILE;
    if (PathFileExists(portableSettings.c_str()) == TRUE) {
        return true;
    }

    return false;
}

std::wstring Settings::SettingsDir() {
    /* First, is this a portable installation? */
    if (Portable()) {
        return AppDir();
    }

    /* If the install isn't portable, use the roaming appdata directory. */
    wchar_t appData[MAX_PATH];
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, appData);
    if (FAILED(hr)) {
        HRESULT hr = SHGetFolderPath(
            NULL, CSIDL_LOCAL_APPDATA, NULL, NULL, appData);

        if (FAILED(hr)) {
            // TODO: This probably warrants an error message!
            return AppDir();
        }
    }

    return std::wstring(appData) + L"\\3RVX";
}

void Settings::CreateSettingsDir() {
    std::wstring settingsDir = SettingsDir();
    CLOG(L"Creating settings directory: %s", settingsDir.c_str());

    settingsDir = L"\\\\?\\" + settingsDir; /* Use long file path (\\?\) */
    BOOL result = CreateDirectory(settingsDir.c_str(), NULL);
    if (result == FALSE) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            QCLOG(L"Directory already exists.");
            return;
        }

        if (GetLastError() == ERROR_PATH_NOT_FOUND) {
            QCLOG(L"Path not found!");
            // TODO: error message?
        }
    }
}

std::wstring Settings::SettingsFile() {
    return SettingsDir() + std::wstring(L"\\") + SETTINGS_FILE;
}

std::wstring Settings::AppDir() {
    if (_appDir.empty()) {
        wchar_t path[MAX_PATH] = { 0 };
        if (GetModuleFileName(NULL, path, MAX_PATH)) {
            PathRemoveFileSpec(path);
        }
        _appDir = std::wstring(path);
    }
    return _appDir;
}

std::wstring Settings::SkinDir() {
    return AppDir() + L"\\" + SKIN_DIR;
}

std::wstring Settings::MainApp() {
    return Settings::AppDir() + L"\\" + MAIN_APP;
}

std::wstring Settings::SettingsApp() {
    return Settings::AppDir() + L"\\" + SETTINGS_APP;
}

std::wstring Settings::LanguagesDir() {
    return AppDir() + L"\\" + LANG_DIR;
}

void Settings::LaunchSettingsApp() {
    std::wstring app = SettingsApp();

    CLOG(L"Opening Settings App: %s", app.c_str());
    int exec = (int) ShellExecute(
        NULL, L"open", app.c_str(), NULL, NULL, SW_SHOWNORMAL);

    if (exec <= 32) {
        Error::ErrorMessage(Error::GENERR_NOTFOUND, app);
    }
}

std::wstring Settings::AudioDeviceID() {
    return GetText(XML_AUDIODEV);
}

int Settings::VolumeCurveAdjustment() {
    return GetInt(XML_CURVE_ADJUST);
}

void Settings::VolumeCurveAdjustment(int value) {
    SetElementValue(XML_CURVE_ADJUST, value);
}

float Settings::VolumeLimiter() {
    return GetFloat(XML_VOLUME_LIMITER);
}

void Settings::VolumeLimiter(float limit) {
    SetElementValue(XML_VOLUME_LIMITER, limit);
}

std::wstring Settings::LanguageName() {
    std::wstring lang = GetText(XML_LANGUAGE);

    if (lang == L"") {
        return DefaultLanguage;
    } else {
        return lang;
    }
}

void Settings::LanguageName(std::wstring name) {
    std::string nName = StringUtils::Narrow(name);
    SetText(XML_LANGUAGE, nName);
}

bool Settings::AlwaysOnTop() {
    return GetEnabled(XML_ONTOP, DefaultOnTop);
}

void Settings::AlwaysOnTop(bool enable) {
    SetEnabled(XML_ONTOP, enable);
}

bool Settings::HideFullscreen() {
    return GetEnabled(XML_HIDE_WHENFULL, DefaultHideFullscreen);
}

void Settings::HideFullscreen(bool enable) {
    SetEnabled(XML_HIDE_WHENFULL, enable);
}

bool Settings::HideDirectX() {
    return GetEnabled(XML_HIDE_DIRECTX, DefaultHideDirectX);
}

void Settings::HideDirectX(bool enable) {
    SetEnabled(XML_HIDE_DIRECTX, enable);
}

std::wstring Settings::Monitor() {
    return GetText(XML_MONITOR);
}

void Settings::Monitor(std::wstring monitorName) {
    SetText(XML_MONITOR, StringUtils::Narrow(monitorName));
}

int Settings::OSDEdgeOffset() {
    if (HasSetting(XML_OSD_OFFSET)) {
        return GetInt(XML_OSD_OFFSET);
    } else {
        return DefaultOSDOffset;
    }
}

void Settings::OSDEdgeOffset(int offset) {
    SetElementValue(XML_OSD_OFFSET, offset);
}

Settings::OSDPos Settings::OSDPosition() {
    std::wstring pos = GetText(XML_OSD_POS);
    const wchar_t *posStr = pos.c_str();

    for (unsigned int i = 0; i < OSDPosNames.size(); ++i) {
        if (_wcsicmp(posStr, OSDPosNames[i].c_str()) == 0) {
            return (Settings::OSDPos) i;
        }
    }

    return DefaultOSDPosition;
}

void Settings::OSDPosition(OSDPos pos) {
    std::wstring posStr = OSDPosNames[(int) pos];
    SetText(XML_OSD_POS, StringUtils::Narrow(posStr));
}

int Settings::OSDX() {
    return GetInt(XML_OSD_X);
}

void Settings::OSDX(int x) {
    SetElementValue(XML_OSD_X, x);
}

int Settings::OSDY() {
    return GetInt(XML_OSD_Y);
}

void Settings::OSDY(int y) {
    SetElementValue(XML_OSD_Y, y);
}

AnimationTypes::HideAnimation Settings::HideAnim() {
    std::wstring anim = GetText(XML_HIDEANIM);
    const wchar_t *animStr = anim.c_str();

    std::vector<std::wstring> *names = &AnimationTypes::HideAnimationNames;
    for (unsigned int i = 0; i < names->size(); ++i) {
        if (_wcsicmp(animStr, (*names)[i].c_str()) == 0) {
            return (AnimationTypes::HideAnimation) i;
        }
    }

    return DefaultHideAnim;
}

void Settings::HideAnim(AnimationTypes::HideAnimation anim) {
    std::wstring hideStr = AnimationTypes::HideAnimationNames[(int) anim];
    SetText(XML_HIDEANIM, StringUtils::Narrow(hideStr));
}

int Settings::HideDelay() {
    return GetInt(XML_HIDETIME, DefaultHideTime);
}

void Settings::HideDelay(int delay) {
    SetElementValue(XML_HIDETIME, delay);
}

int Settings::HideSpeed() {
    return GetInt(XML_HIDESPEED, DefaultHideSpeed);
}

void Settings::HideSpeed(int speed) {
    SetElementValue(XML_HIDESPEED, speed);
}

bool Settings::CurrentSkin(std::wstring skinName) {
    std::string name = StringUtils::Narrow(skinName);
    std::wstring xml = SkinXML(skinName);
    if (PathFileExists(xml.c_str()) == FALSE) {
        return false;
    }

    SetText(XML_SKIN, name);
    return true;
}

std::wstring Settings::CurrentSkin() {
    std::wstring name = GetText("skin");

    if (name == L"") {
        return DefaultSkin;
    } else {
        return name;
    }
}

std::wstring Settings::SkinXML() {
    return SkinXML(CurrentSkin());
}

std::wstring Settings::SkinXML(std::wstring skinName) {
    std::wstring skinXML = Settings::AppDir() + L"\\" + SKIN_DIR + L"\\"
        + skinName + L"\\" + SKIN_XML;
    return skinXML;
}

std::unordered_map<int, HotkeyInfo> Settings::Hotkeys() {
    std::unordered_map<int, HotkeyInfo> keyMappings;

    tinyxml2::XMLElement *hotkeys = _root->FirstChildElement("hotkeys");
    if (hotkeys == NULL) {
        return keyMappings;
    }

    tinyxml2::XMLElement *hotkey = hotkeys->FirstChildElement("hotkey");
    for (; hotkey != NULL; hotkey = hotkey->NextSiblingElement()) {
        const char *actionStr = hotkey->Attribute("action");
        if (actionStr == NULL) {
            CLOG(L"No action provided for hotkey; skipping");
            continue;
        }

        int action = -1;
        std::wstring wActionStr = StringUtils::Widen(actionStr);
        for (unsigned int i = 0; i < HotkeyInfo::ActionNames.size(); ++i) {
            const wchar_t *currentAction = HotkeyInfo::ActionNames[i].c_str();
            if (_wcsicmp(wActionStr.c_str(), currentAction) == 0) {
                action = i;
                break;
            }
        }

        if (action == -1) {
            CLOG(L"Hotkey action '%s' not recognized; skipping",
                wActionStr.c_str());
            continue;
        }

        int combination = -1;
        hotkey->QueryIntAttribute("combination", &combination);
        if (combination == -1) {
            CLOG(L"No key combination provided for hotkey; skipping");
            continue;
        }

        HotkeyInfo hki;
        hki.action = action;
        hki.keyCombination = combination;

        /* Does this hotkey action have any arguments? */
        tinyxml2::XMLElement *arg = hotkey->FirstChildElement("arg");
        for (; arg != NULL; arg = arg->NextSiblingElement()) {
            const char *argStr = arg->GetText();
            hki.args.push_back(StringUtils::Widen(argStr));
        }

        /* Do a validity check on the finished HKI object */
        if (hki.Valid() == false) {
            continue;
        }

        /* Whew, we made it! */
        CLOG(L"%s", hki.ToString().c_str());
        keyMappings[combination] = hki;
    }

    return keyMappings;
}

void Settings::Hotkeys(std::vector<HotkeyInfo> hotkeys) {
    tinyxml2::XMLElement *hkElem = GetOrCreateElement("hotkeys");
    hkElem->DeleteChildren();

    for (HotkeyInfo hotkey : hotkeys) {
        if (hotkey.Valid() == false) {
            continue;
        }

        tinyxml2::XMLElement *hk = _xml.NewElement("hotkey");

        hk->SetAttribute("combination", hotkey.keyCombination);
        std::string actionStr = StringUtils::Narrow(
            HotkeyInfo::ActionNames[hotkey.action]);
        hk->SetAttribute("action", actionStr.c_str());

        if (hotkey.args.size() > 0) {
            for (std::wstring arg : hotkey.args) {
                tinyxml2::XMLElement *argElem = _xml.NewElement("arg");
                argElem->SetText(StringUtils::Narrow(arg).c_str());
                hk->InsertEndChild(argElem);
            }
        }

        hkElem->InsertEndChild(hk);
    }
}

LanguageTranslator *Settings::Translator() {
    if (_translator == NULL) {
        std::wstring langDir = Settings::LanguagesDir();
        std::wstring lang = Settings::LanguageName();
        std::wstring langFile = langDir + L"\\" + lang + L".xml";
        if (PathFileExists(langFile.c_str()) == FALSE) {
            _translator = new LanguageTranslator();
        } else {
            _translator = new LanguageTranslator(langFile);
            _translator->LoadTranslations();
        }
    }

    return _translator;
}

bool Settings::NotifyIconEnabled() {
    return GetEnabled(XML_NOTIFYICON, DefaultNotifyIcon);
}

void Settings::NotifyIconEnabled(bool enable) {
    SetEnabled(XML_NOTIFYICON, enable);
}

bool Settings::SoundEffectsEnabled() {
    return GetEnabled(XML_SOUNDS, DefaultSoundsEnabled);
}

void Settings::SoundEffectsEnabled(bool enable) {
    SetEnabled(XML_SOUNDS, enable);
}

bool Settings::HasSetting(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    return (el != NULL);
}

bool Settings::GetEnabled(std::string elementName, const bool defaultSetting) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        if (ENABLE_3RVX_LOG) {
            std::wstring elStr = StringUtils::Widen(elementName);
            CLOG(L"Warning: XML element '%s' not found", elStr.c_str());
        }
        return defaultSetting;
    } else {
        bool val = false;
        el->QueryBoolText(&val);
        return val;
    }
}

void Settings::SetEnabled(std::string elementName, bool enabled) {
    tinyxml2::XMLElement *el = GetOrCreateElement(elementName);
    el->SetText(enabled ? "true" : "false");
}

std::wstring Settings::GetText(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        if (ENABLE_3RVX_LOG) {
            CLOG(L"Warning: XML element %s not found",
                StringUtils::Widen(elementName).c_str());
        }
        return L"";
    }

    const char* str = el->GetText();
    if (str == NULL) {
        return L"";
    } else {
        return StringUtils::Widen(str);
    }
}

void Settings::SetText(std::string elementName, std::string text) {
    tinyxml2::XMLElement *el = GetOrCreateElement(elementName);
    el->SetText(text.c_str());
}

int Settings::GetInt(std::string elementName, const int defaultValue) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        if (ENABLE_3RVX_LOG) {
            std::wstring elStr = StringUtils::Widen(elementName);
            CLOG(L"Warning: XML element '%s' not found", elStr.c_str());
        }
        return defaultValue;
    }

    int val = defaultValue;
    el->QueryIntText(&val);
    return val;
}

float Settings::GetFloat(std::string elementName, const float defaultValue) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        if (ENABLE_3RVX_LOG) {
            std::wstring elStr = StringUtils::Widen(elementName);
            CLOG(L"Warning: XML element '%s' not found", elStr.c_str());
        }
        return defaultValue;
    }

    float val = defaultValue;
    el->QueryFloatText(&val);
    return val;
}

tinyxml2::XMLElement *Settings::GetOrCreateElement(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        el = _xml.NewElement(elementName.c_str());
        _root->InsertEndChild(el);
    }
    return el;
}

bool Settings::AutomaticUpdates() {
    return GetEnabled(XML_UPDATEAUTO, DefaultAutoUpdate);
}

void Settings::AutomaticUpdates(bool enabled) {
    SetEnabled(XML_UPDATEAUTO, enabled);
}

void Settings::LastUpdateCheckNow() {
    LastUpdateCheck(std::time(nullptr));
}

void Settings::LastUpdateCheck(long long time) {
    SetText(XML_UPDATECHECKTIME, std::to_string((long long) time));
}

long long Settings::LastUpdateCheck() {
    tinyxml2::XMLElement *el = _root->FirstChildElement(XML_UPDATECHECKTIME);
    if (el == NULL) {
        return 0;
    }

    const char *timeStr;
    timeStr = el->GetText();
    if (timeStr == NULL) {
        return 0;
    }

    return std::stoll(timeStr);
}

std::wstring Settings::IgnoreUpdate() {
    return GetText(XML_IGNOREUPDATE);
}

void Settings::IgnoreUpdate(std::wstring versionString) {
    SetText(XML_IGNOREUPDATE, StringUtils::Narrow(versionString));
}

bool Settings::ShowOnStartup() {
    return GetEnabled(XML_SHOWONSTART, DefaultShowOnStartup);
}

void Settings::ShowOnStartup(bool show) {
    SetEnabled(XML_SHOWONSTART, show);
}