#pragma once

#include <vector>

enum HotkeyActions {
    IncreaseVolume,
    DecreaseVolume,
    SetVolume,
    Mute,
    VolumeSlider,
    Settings,
    Exit,
};

class HotkeyInfo {
public:
    //HotkeyInfo()
    static std::vector<std::wstring> ActionNames;

};