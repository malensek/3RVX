#pragma once

#include "MeterWnd/Animations/AnimationTypes.h"
#include "Settings.h"

class SettingsDefaults {
public:
    /* Default settings */
    static const bool OnTop = true;
    static const AnimationTypes::HideAnimation DefaultHideAnim
        = AnimationTypes::Fade;
    static const bool HideFullscreen = false;
    static const bool HideDirectX = false;
    static const int HideSpeed = 765;
    static const int HideTime = 800;
    static constexpr const float VolumeLimit = 1.0f;
    static const bool NotifyIcon = true;
    static const bool ShowOnStartup = true;
    static const bool SoundsEnabled = true;
    static const int OSDOffset = 140;
    static const Settings::OSDPos OSDPosition = Settings::OSDPos::Bottom;
    static const bool AutoUpdate = false;
    static const bool MuteLock = false;
    static const bool SubscribeVolumeEvents = true;
    static const bool SubscribeEjectEvents = true;

    static const bool VolumeOSDEnabled = true;
    static const bool EjectOSDEnabled = true;
    static const bool BrightnessOSDEnabled = true;
    static const bool KeyboardOSDEnabled = false;
};