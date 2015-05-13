#pragma once

#include <map>
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
        MediaKey,
        RunApp,
        Settings,
        Exit,
    };
    static std::vector<std::wstring> ActionNames;

    enum MediaKeys {
        PlayPause,
        Stop,
        Next,
        Previous,
    };
    static std::vector<std::wstring> MediaKeyNames;
    static std::vector<unsigned short> MediaKeyVKs;

public:
    int keyCombination = 0;
    int action = -1;
    std::vector<std::wstring> args;
    std::map<int, int> intArgs;
    std::map<int, double> doubleArgs;

    int ArgToInt(int argIdx);
    double ArgToDouble(int argIdx);
    bool HasArgs();
    void AllocateArg(unsigned int index);
    std::wstring ToString();

};