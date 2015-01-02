#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"

class Skin;

#define SETTINGS_APP L"SettingsUI.exe"

class Settings {
public:
    static Settings *Instance();

    static std::wstring AppDir();
    static std::wstring SettingsApp();

    Skin *CurrentSkin();
    std::wstring SkinName();
    std::wstring SkinXML();
    std::wstring SkinXML(std::wstring skinName);

    bool HasSetting(std::string elementName);
    bool IsEnabled(std::string elementName);

    std::wstring GetText(std::string elementName);
    int GetInt(std::string elementName);

    std::unordered_map<int, int> Hotkeys();

    void Reload();

private:
    static Settings *instance;
    static std::wstring _appDir;

    std::wstring _file;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
    Skin *_skin;

    Settings();
};