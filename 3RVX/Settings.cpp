#include "Settings.h"

#include <Shlwapi.h>
#include <algorithm>

#include "Error.h"
#include "HotkeyActions.h"
#include "HotkeyInfo.h"
#include "Logger.h"
#include "Monitor.h"
#include "Skin.h"
#include "StringUtils.h"

#define XML_AUDIODEV "audioDeviceID"
#define XML_HIDE_WHENFULL "hideFullscreen"
#define XML_HIDEANIM "hideAnimation"
#define XML_HIDETIME "hideDelay"
#define XML_HIDESPEED "hideSpeed"
#define XML_LANGUAGE "language"
#define XML_MONITOR "monitor"
#define XML_NOTIFYICON "notifyIcon"
#define XML_ONTOP "onTop"
#define XML_OSD_OFFSET "osdEdgeOffset"
#define XML_OSD_POS "osdPosition"
#define XML_OSD_X "osdX"
#define XML_OSD_Y "osdY"
#define XML_SKIN "skin"
#define XML_SOUNDS "soundEffects"

std::wstring Settings::_appDir(L"");
Settings *Settings::instance;

std::vector<std::wstring> Settings::OSDPosNames = {
    L"top",
    L"left",
    L"right",
    L"bottom",
    L"center",
    L"top-left",
    L"top-right",
    L"bottom-left",
    L"bottom-right",
    L"custom"
};

std::vector<std::wstring> Settings::HideAnimNames = {
    L"none",
    L"fade"
};

Settings *Settings::Instance() {
    if (instance == NULL) {
        instance = new Settings();
        instance->Reload();
    }
    return instance;
}

void Settings::Reload() {
    _file = AppDir() + L"\\Settings.xml";
    CLOG(L"Loading settings file: %s", _file.c_str());

    std::string u8FileName = StringUtils::Narrow(_file);
    tinyxml2::XMLError result = _xml.LoadFile(u8FileName.c_str());
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        throw std::runtime_error("Failed to parse XML file");
    }

    _root = _xml.GetDocument()->FirstChildElement("settings");
    if (_root == NULL) {
        throw std::runtime_error("Could not find root XML element");
    }

    _skin = new Skin(SkinXML());
}

int Settings::Save() {
    FILE *stream;
    errno_t err = _wfopen_s(&stream, _file.c_str(), L"w");
    if (err != 0) {
        CLOG(L"Could not open settings file for writing!");
        return 100 + err;
    }
    tinyxml2::XMLError result = _xml.SaveFile(stream);
    fclose(stream);
    return result;
}

std::wstring Settings::AppDir() {
    if (_appDir.empty()) {
        wchar_t path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        PathRemoveFileSpec(path);
        _appDir = std::wstring(path);
    }
    return _appDir;
}

std::wstring Settings::SkinDir() {
    return AppDir() + L"\\" + SKIN_DIR;
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
        Error::ErrorMessage(GENERR_NOTFOUND, app);
    }
}

std::wstring Settings::AudioDeviceID() {
    return GetText(XML_AUDIODEV);
}

std::wstring Settings::LanguageName() {
    std::wstring lang = GetText(XML_LANGUAGE);

    if (lang == L"") {
        return DEFAULT_LANGUAGE;
    } else {
        return lang;
    }
}

bool Settings::AlwaysOnTop() {
    return GetEnabled(XML_ONTOP);
}

void Settings::AlwaysOnTop(bool enable) {
    SetEnabled(XML_ONTOP, enable);
}

bool Settings::HideFullscreen() {
    return GetEnabled(XML_HIDE_WHENFULL);
}

void Settings::HideFullscreen(bool enable) {
    SetEnabled(XML_HIDE_WHENFULL, enable);
}

std::wstring Settings::Monitor() {
    return GetText(XML_MONITOR);
}

std::vector<HMONITOR> Settings::MonitorHandles() {
    std::vector<HMONITOR> monitors;
    std::wstring monitorStr = Monitor();

    if (monitorStr == L"") {
        /* Primary Monitor */
        monitors.push_back(Monitor::Primary());
        return monitors;
    }
    
    if (monitorStr == L"*") {
        /* All Monitors */
        std::unordered_map<std::wstring, HMONITOR> map = Monitor::MonitorMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            monitors.push_back(it->second);
        }
        return monitors;
    }

    /* Specific Monitor */
    std::unordered_map<std::wstring, HMONITOR> map = Monitor::MonitorMap();
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (monitorStr == it->first) {
            monitors.push_back(it->second);
        }
    }
    return monitors;
}

void Settings::Monitor(std::wstring monitorName) {
    SetText(XML_MONITOR, StringUtils::Narrow(monitorName));
}

int Settings::OSDEdgeOffset() {
    if (HasSetting(XML_OSD_OFFSET)) {
        return GetInt(XML_OSD_OFFSET);
    } else {
        return DEFAULT_OSD_OFFSET;
    }
}

void Settings::OSDEdgeOffset(int offset) {
    SetInt(XML_OSD_OFFSET, offset);
}

Settings::OSDPos Settings::OSDPosition() {
    std::wstring pos = GetText(XML_OSD_POS);
    std::transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

    for (unsigned int i = 0; i < OSDPosNames.size(); ++i) {
        if (pos == OSDPosNames[i]) {
            return (Settings::OSDPos) i;
        }
    }

    return DEFAULT_OSD_POS;
}

void Settings::OSDPosition(OSDPos pos) {
    std::wstring posStr = OSDPosNames[(int) pos];
    SetText(XML_OSD_POS, StringUtils::Narrow(posStr));
}

int Settings::OSDX() {
    return GetInt(XML_OSD_X);
}

void Settings::OSDX(int x) {
    SetInt(XML_OSD_X, x);
}

int Settings::OSDY() {
    return GetInt(XML_OSD_Y);
}

void Settings::OSDY(int y) {
    SetInt(XML_OSD_Y, y);
}

Skin *Settings::CurrentSkin() {
    return _skin;
}

Settings::HideAnim Settings::HideAnimation() {
    std::wstring anim = GetText(XML_HIDEANIM);
    std::transform(anim.begin(), anim.end(), anim.begin(), ::tolower);

    for (unsigned int i = 0; i < HideAnimNames.size(); ++i) {
        if (anim == HideAnimNames[i]) {
            return (Settings::HideAnim) i;
        }
    }

    return DEFAULT_HIDE_ANIM;
}

void Settings::HideAnimation(Settings::HideAnim anim) {
    std::wstring hideStr = HideAnimNames[(int) anim];
    SetText(XML_HIDEANIM, StringUtils::Narrow(hideStr));
}

int Settings::HideDelay() {
    return GetInt(XML_HIDETIME);
}

void Settings::HideDelay(int delay) {
    SetInt(XML_HIDETIME, delay);
}

int Settings::HideSpeed() {
    return GetInt(XML_HIDESPEED);
}

void Settings::HideSpeed(int speed) {
    SetInt(XML_HIDESPEED, speed);
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

std::wstring Settings::SkinName() {
    std::wstring name = GetText("skin");

    if (name == L"") {
        return DEFAULT_SKIN;
    } else {
        return name;
    }
}

std::wstring Settings::SkinXML() {
    return SkinXML(SkinName());
}

std::wstring Settings::SkinXML(std::wstring skinName) {
    std::wstring skinXML = Settings::AppDir() + L"\\" + SKINS_DIR L"\\"
        + skinName + L"\\" SKIN_XML;
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
        int action = -1;
        hotkey->QueryIntAttribute("action", &action);
        if (action == -1) {
            CLOG(L"No action provided for hotkey; skipping");
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
        
        /* Whew, we made it! */
        CLOG(L"Adding hotkey mapping: %d -> %d", combination, action);
        CLOG(L"%s", hki.ToString().c_str());
        keyMappings[combination] = hki;
    }

    return keyMappings;
}

void Settings::Hotkeys(std::vector<HotkeyInfo> hotkeys) {
    tinyxml2::XMLElement *hkElem = GetOrCreateElement("hotkeys");
    hkElem->DeleteChildren();

    for (HotkeyInfo hotkey : hotkeys) {
        OutputDebugString(L"key");
        tinyxml2::XMLElement *hk = _xml.NewElement("hotkey");

        hk->SetAttribute("combination", hotkey.keyCombination);
        hk->SetAttribute("action", hotkey.action);

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

bool Settings::NotifyIconEnabled() {
    return GetEnabled(XML_NOTIFYICON);
}

void Settings::NotifyIconEnabled(bool enable) {
    SetEnabled(XML_NOTIFYICON, enable);
}

bool Settings::SoundEffectsEnabled() {
    return GetEnabled(XML_SOUNDS);
}

void Settings::SoundEffectsEnabled(bool enable) {
    SetEnabled(XML_SOUNDS, enable);
}

bool Settings::HasSetting(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    return (el != NULL);
}

bool Settings::GetEnabled(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        CLOG(L"Warning: XML element '%s' not found", elementName.c_str());
        return false;
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
        CLOG(L"Warning: XML element '%s' not found", elementName.c_str());
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

int Settings::GetInt(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        CLOG(L"Warning: XML element '%s' not found", elementName.c_str());
        return 0;
    }

    int val = 0;
    el->QueryIntText(&val);
    return val;
}

void Settings::SetInt(std::string elementName, int value) {
    tinyxml2::XMLElement *el = GetOrCreateElement(elementName);
    el->SetText(value);
}

tinyxml2::XMLElement *Settings::GetOrCreateElement(std::string elementName) {
    tinyxml2::XMLElement *el = _root->FirstChildElement(elementName.c_str());
    if (el == NULL) {
        el = _xml.NewElement(elementName.c_str());
        _root->InsertEndChild(el);
    }
    return el;
}