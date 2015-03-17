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
        EjectDrive,
        EjectLatestDrive,
        RunApp,
        Settings,
        Exit,
    };
    static std::vector<std::wstring> ActionNames;

public:
    int keyCombination = 0;
    int action = -1;
    std::vector<std::wstring> args;

    int ArgToInt(int argIdx);
    double ArgToDouble(int argIdx);
    std::wstring ToString();

};