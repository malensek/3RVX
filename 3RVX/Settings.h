#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"
#include "MeterWnd\Animations\AnimationTypes.h"

class HotkeyInfo;
class LanguageTranslator;
class Skin;

#define SETTINGS_APP L"SettingsUI.exe"
#define LANG_DIR L"Languages"
#define SKIN_DIR L"Skins"

class Settings {
public:
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

    AnimationTypes::HideAnimation HideAnim();
    void HideAnim(AnimationTypes::HideAnimation anim);
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

    std::wstring CurrentSkin();
    bool CurrentSkin(std::wstring skinName);
    std::wstring SkinXML();
    std::wstring SkinXML(std::wstring skinName);

    bool NotifyIconEnabled();
    void NotifyIconEnabled(bool enable);
    bool SoundEffectsEnabled();
    void SoundEffectsEnabled(bool enable);

    LanguageTranslator *Translator();

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

    LanguageTranslator *_translator;

    bool HasSetting(std::string elementName);
    bool GetEnabled(std::string elementName, const bool defaultSetting);
    void SetEnabled(std::string elementName, bool enabled);

    std::wstring GetText(std::string elementName);
    void SetText(std::string elementName, std::string text);
    int GetInt(std::string elementName, const int defaultValue = 0);
    void SetInt(std::string elementName, int value);

    tinyxml2::XMLElement *GetOrCreateElement(std::string elementName);

public:
    /* Default settings */
    const bool DefaultOnTop = true;
    const AnimationTypes::HideAnimation DefaultHideAnim = AnimationTypes::Fade;
    const bool DefaultHideFullscreen = false;
    const int DefaultHideSpeed = 765;
    const int DefaultHideTime = 800;
    const wchar_t *DefaultLanguage = L"English";
    const bool DefaultNotifyIcon = false;
    const bool DefaultSoundsEnabled = true;
    const int DefaultOSDOffset = 140;
    const Settings::OSDPos DefaultOSDPosition = OSDPos::Bottom;
    const wchar_t *DefaultSkin = L"Classic";

};