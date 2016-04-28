// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"
#include "MeterWnd\Animations\AnimationTypes.h"

class HotkeyInfo;
class LanguageTranslator;
class Skin;

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
    void LoadEmptySettings();
    int Save();

    /* General Settings */
    bool NotifyIconEnabled();
    void NotifyIconEnabled(bool enable);
    bool SoundEffectsEnabled();
    void SoundEffectsEnabled(bool enable);

    /* Skin */
    std::wstring CurrentSkin();
    bool CurrentSkin(std::wstring skinName);
    std::wstring SkinXML();
    std::wstring SkinXML(std::wstring skinName);

    /* Language */
    LanguageTranslator *Translator();
    std::wstring LanguageName();
    void LanguageName(std::wstring name);

    /* Animation */
    AnimationTypes::HideAnimation HideAnim();
    void HideAnim(AnimationTypes::HideAnimation anim);
    int HideDelay();
    void HideDelay(int delay);
    int HideSpeed();
    void HideSpeed(int speed);

    /* Display */
    bool AlwaysOnTop();
    void AlwaysOnTop(bool enable);
    bool HideFullscreen();
    void HideFullscreen(bool enable);
    bool HideDirectX();
    void HideDirectX(bool enable);
    std::wstring Monitor();
    void Monitor(std::wstring monitorName);
    OSDPos OSDPosition();
    void OSDPosition(OSDPos pos);
    int OSDEdgeOffset();
    void OSDEdgeOffset(int offset);
    int OSDX();
    void OSDX(int x);
    int OSDY();
    void OSDY(int y);

    /* OSDs */
    bool BrightnessOSDEnabled();
    void BrightnessOSDEnabled(bool enable);
    bool EjectOSDEnabled();
    void EjectOSDEnabled(bool enable);
    bool KeyboardOSDEnabled();
    void KeyboardOSDEnabled(bool enable);
    bool VolumeOSDEnabled();
    void VolumeOSDEnabled(bool enable);

    /* Volume */
    std::wstring AudioDeviceID();
    int VolumeCurveAdjustment();
    void VolumeCurveAdjustment(int value);
    float VolumeLimiter();
    void VolumeLimiter(float limit);
    bool MuteOnLock();
    void MuteOnLock(bool enable);
    bool SubscribeVolumeEvents();
    void SubscribeVolumeEvents(bool enable);

    /* Eject */
    bool SubscribeEjectEvents();
    void SubscribeEjectEvents(bool enable);

    /* Hotkeys */
    std::unordered_map<int, HotkeyInfo> Hotkeys();
    void Hotkeys(std::vector<HotkeyInfo> hotkeys);

    bool AutomaticUpdates();
    void AutomaticUpdates(bool enabled);
    void LastUpdateCheckNow();
    void LastUpdateCheck(long long time);
    long long LastUpdateCheck();
    std::wstring IgnoreUpdate();
    void IgnoreUpdate(std::wstring versionString);

    bool ShowOnStartup();
    void ShowOnStartup(bool show);

public:
    /* Static settings methods */

    /// <summary>
    /// Retrieves the location of the current executable. Both the main program
    /// and settings application should be in the same directory, but this
    /// method could return different directories if the executables are moved.
    /// </summary>
    static std::wstring AppDir();

    /// <summary>
    /// Determines whether this installation is portable or not.
    /// </summary>
    static bool Portable();

    /// <summary>
    /// Determines where program settings are stored. If this is an installed
    /// version of the program, then the settings are in
    /// %APPDATA%\3RVX\
    /// <p>
    /// If this is a portable installation of the program, the settings are
    /// stored in the same directory as the program executable.
    /// </summary>
    static std::wstring SettingsDir();

    /// <summary>
    /// Ensures the settings directory has been created. If the directory
    /// already exists, calling this function has no effect.
    /// </summary>
    static void CreateSettingsDir();

    static std::wstring SettingsFile();
    static std::wstring SkinDir();
    static std::wstring LanguagesDir();
    static std::wstring MainApp();
    static std::wstring SettingsApp();
    static void LaunchSettingsApp();


private:
    Settings() {
        /* Don't allow instantiation for this Singleton class; see the
         * Instance() method to retrieve an instance instead. */
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
    float GetFloat(std::string elementName, const float defaultValue = 0.0f);

    template<typename T> void SetElementValue(
            std::string elementName, T value) {
        tinyxml2::XMLElement *el = GetOrCreateElement(elementName);
        el->SetText(value);
    }

    tinyxml2::XMLElement *GetOrCreateElement(std::string elementName);

public:
    /* Default settings */
    static const bool DefaultOnTop = true;
    static const AnimationTypes::HideAnimation DefaultHideAnim
        = AnimationTypes::Fade;
    static const bool DefaultHideFullscreen = false;
    static const bool DefaultHideDirectX = false;
    static const int DefaultHideSpeed = 765;
    static const int DefaultHideTime = 800;
    static constexpr const float DefaultVolumeLimit = 1.0f;
    static const bool DefaultNotifyIcon = true;
    static const bool DefaultShowOnStartup = true;
    static const bool DefaultSoundsEnabled = true;
    static const int DefaultOSDOffset = 140;
    static const Settings::OSDPos DefaultOSDPosition = OSDPos::Bottom;
    static const bool DefaultAutoUpdate = false;
    static const bool DefaultMuteLock = false;
    static const bool DefaultSubscribeVolumeEvents = true;
    static const bool DefaultSubscribeEjectEvents = true;

    static const bool DefaultVolumeOSDEnabled = true;
    static const bool DefaultEjectOSDEnabled = true;
    static const bool DefaultBrightnessOSDEnabled = true;
    static const bool DefaultKeyboardOSDEnabled = false;

    static constexpr const wchar_t *MAIN_APP = L"3RVX.exe";
    static constexpr const wchar_t *SETTINGS_APP = L"Settings.exe";
    static constexpr const wchar_t *SETTINGS_FILE = L"Settings.xml";
    static constexpr const wchar_t *LANG_DIR = L"Languages";
    static constexpr const wchar_t *SKIN_DIR = L"Skins";
    static constexpr const wchar_t *SKIN_XML = L"Skin.xml";

    static constexpr const wchar_t *DefaultLanguage = L"English";
    static constexpr const wchar_t *DefaultSkin = L"Classic";

    /* XML tag names */
    static constexpr const char *XML_AUDIODEV = "audioDeviceID";
    static constexpr const char *XML_CURVE_ADJUST = "curveAdjust";
    static constexpr const char *XML_ENABLE_BOSD = "brightnessOSDEnabled";
    static constexpr const char *XML_ENABLE_EOSD = "ejectOSDEnabled";
    static constexpr const char *XML_ENABLE_KOSD = "keyboardOSDEnabled";
    static constexpr const char *XML_ENABLE_VOSD = "volumeOSDEnabled";
    static constexpr const char *XML_HIDE_WHENFULL = "hideFullscreen";
    static constexpr const char *XML_HIDE_DIRECTX = "hideDirectX";
    static constexpr const char *XML_HIDEANIM = "hideAnimation";
    static constexpr const char *XML_HIDETIME = "hideDelay";
    static constexpr const char *XML_HIDESPEED = "hideSpeed";
    static constexpr const char *XML_IGNOREUPDATE = "ignoreUpdateVersion";
    static constexpr const char *XML_LANGUAGE = "language";
    static constexpr const char *XML_MONITOR = "monitor";
    static constexpr const char *XML_MUTELOCK = "muteDuringLock";
    static constexpr const char *XML_NOTIFYICON = "notifyIcon";
    static constexpr const char *XML_ONTOP = "onTop";
    static constexpr const char *XML_OSD_OFFSET = "osdEdgeOffset";
    static constexpr const char *XML_OSD_POS = "osdPosition";
    static constexpr const char *XML_OSD_X = "osdX";
    static constexpr const char *XML_OSD_Y = "osdY";
    static constexpr const char *XML_SHOWONSTART = "showOnStartup";
    static constexpr const char *XML_SKIN = "skin";
    static constexpr const char *XML_SOUNDS = "soundEffects";
    static constexpr const char *XML_SUBSCRIBE_EJECT = "subscribeEjectEvents";
    static constexpr const char *XML_SUBSCRIBE_VOL = "subscribeVolumeEvents";
    static constexpr const char *XML_UPDATEAUTO = "automaticUpdates";
    static constexpr const char *XML_UPDATECHECKTIME = "lastUpdateCheck";
    static constexpr const char *XML_VOLUME_LIMITER = "volumeLimiter";
};
