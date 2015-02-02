#pragma once

#include <vector>

class HotkeyInfo {
public:
    enum HotkeyActions {
        IncreaseVolume,
        DecreaseVolume,
        SetVolume,
        Mute,
        VolumeSlider,
        Settings,
        Exit,
    };
    static std::vector<std::wstring> ActionNames;


public:
    //HotkeyInfo()

};