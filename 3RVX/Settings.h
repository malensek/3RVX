#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"

class Skin;

#define SETTINGS_APP L"SettingsUI.exe"
#define LANG_DIR L"Languages"

#define DEFAULT_HIDE_ANIM Settings::HideAnim::Fade
#define DEFAULT_LANGUAGE L"English"
#define DEFAULT_OSD_OFFSET 140
#define DEFAULT_OSD_POS Settings::OSDPos::Bottom
#define DEFAULT_SKIN L"Default"

class Settings {
public:
    static Settings *Instance();
    void Reload();
    int Save();

    static std::wstring AppDir();
    static std::wstring SettingsApp();

    std::wstring AudioDeviceID();

    std::wstring LanguagesDir();
    std::wstring LanguageName();

    enum OSDPos { Top, Left, Right, Bottom, Center, Custom };
    bool AlwaysOnTop();
    bool HideFullscreen();
    OSDPos OSDPosition();
    int OSDEdgeOffset();
    int OSDX();
    int OSDY();

    enum HideAnimation { None, Fade };
    HideAnimation HideAnimation();
    int HideDelay();
    int HideSpeed();

    Skin *CurrentSkin();
    bool CurrentSkin(std::wstring skinName);
    std::wstring SkinName();
    std::wstring SkinXML();
    std::wstring SkinXML(std::wstring skinName);

    bool NotifyIconEnabled();
    void NotifyIconEnabled(bool enable);
    bool SoundEffectsEnabled();
    void SoundEffectsEnabled(bool enable);

    std::unordered_map<int, int> Hotkeys();

private:
    Settings();

    static Settings *instance;
    static std::wstring _appDir;

    std::wstring _file;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
    Skin *_skin;

    bool HasSetting(std::string elementName);
    bool GetEnabled(std::string elementName);
    void SetEnabled(std::string elementName, bool enabled);

    std::wstring GetText(std::string elementName);
    void SetText(std::string elementName, std::string text);
    int GetInt(std::string elementName);
    void SetInt(std::string elementName, int value);

    tinyxml2::XMLElement *GetOrCreateElement(std::string elementName);
};