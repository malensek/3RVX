#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"

class HotkeyInfo;
class Skin;

#define SETTINGS_APP L"SettingsUI.exe"
#define LANG_DIR L"Languages"
#define SKIN_DIR L"Skins"

#define DEFAULT_HIDE_ANIM Settings::HideAnim::Fade
#define DEFAULT_LANGUAGE L"English"
#define DEFAULT_OSD_OFFSET 140
#define DEFAULT_OSD_POS Settings::OSDPos::Bottom
#define DEFAULT_SKIN L"Default"

class Settings {
public:
    enum HideAnim {
        None,
        Fade
    };
    static std::vector<std::wstring> HideAnimNames;

    enum OSDPos {
        Top,
        Left,
        Right,
        Bottom,
        Center,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Custom
    };
    static std::vector<std::wstring> OSDPosNames;

public:
    static Settings *Instance();
    void Load();
    int Save();

    static std::wstring AppDir();
    static std::wstring SkinDir();
    static std::wstring SettingsApp();
    static void LaunchSettingsApp();

    std::wstring AudioDeviceID();

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

    std::unordered_map<int, HotkeyInfo> Hotkeys();
    void Hotkeys(std::vector<HotkeyInfo> hotkeys);

private:
    Settings() {

    }

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