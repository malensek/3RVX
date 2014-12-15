#include "Settings.h"

#include <Shlwapi.h>

#include "HotkeyActions.h"
#include "Logger.h"
#include "Skin.h"
#include "StringUtils.h"

std::wstring Settings::_appDir(L"");
Settings *Settings::instance;

Settings::Settings() {

}

Settings *Settings::Instance() {
    if (instance == NULL) {
        instance = new Settings();
        instance->Reload();
    }
    return instance;
}

void Settings::Reload() {
    std::wstring file = AppDir() + L"\\Settings.xml";
    CLOG(L"Loading settings file: %s", file.c_str());

    std::string u8FileName = StringUtils::Narrow(file);
    tinyxml2::XMLError result = _xml.LoadFile(u8FileName.c_str());
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        throw std::runtime_error("Failed to parse XML file");
    }

    _root = _xml.GetDocument()->FirstChildElement("settings");
    if (_root == NULL) {
        throw std::runtime_error("Could not find root XML element");
    }
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

std::wstring Settings::SettingsApp() {
    return Settings::AppDir() + L"\\" + SETTINGS_APP;
}

std::wstring Settings::SkinName() {
    const char* skinName = _root->FirstChildElement("skin")->GetText();
    if (skinName == NULL) {
        return L"Default";
    } else {
        return StringUtils::Widen(skinName);
    }
}

std::wstring Settings::SkinXML() {
    std::wstring skinXML = Settings::AppDir() + L"\\" + SKINS_DIR L"\\"
        + SkinName() + L"\\" SKIN_XML;
    return skinXML;
}

std::unordered_map<int, int> Settings::Hotkeys() {
    std::unordered_map<int, int> keyMappings;

    tinyxml2::XMLElement *hotkeys = _root->FirstChildElement("hotkeys");
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
        
        /* Whew, we made it! */
        CLOG(L"Adding hotkey mapping: %d -> %d", combination, action);
        keyMappings[combination] = action;
    }

    return keyMappings;
}

bool Settings::IsEnabled(std::string elementName) {
    bool val = false;
    _root->FirstChildElement(elementName.c_str())->QueryBoolText(&val);
    return val;
}

std::wstring Settings::GetText(std::string elementName) {
    const char* str = _root->FirstChildElement(elementName.c_str())->GetText();
    if (str == NULL) {
        return L"";
    } else {
        return StringUtils::Widen(str);
    }
}