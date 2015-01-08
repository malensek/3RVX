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

    enum HideAnim { None, Fade };
    static std::vector<std::wstring> HideAnimNames;
    HideAnim HideAnimation();
    void HideAnimation(HideAnim anim);
    int HideDelay();
    void HideDelay(int delay);
    int HideSpeed();
    void HideSpeed(int speed);

    std::wstring LanguagesDir();
    std::wstring LanguageName();

    std::wstring Monitor();
    void Monitor(std::wstring monitorName);

    enum OSDPos { Top, Left, Right, Bottom, Center, Custom };
    static std::vector<std::wstring> OSDPosNames;
    bool AlwaysOnTop();
    void AlwaysOnTop(bool enable);
    bool HideFullscreen();
    void HideFullscreen(bool enable);
    OSDPos OSDPosition();
    void OSDPosition(OSDPos pos);
    int OSDEdgeOffset();
    void OSDEdgeOffset(int offset);
    int OSDX();
    void OSDX(int x);
    int OSDY();
    void OSDY(int y);

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