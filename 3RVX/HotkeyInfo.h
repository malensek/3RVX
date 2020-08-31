// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <map>
#include <vector>
#include <string>

/* HOW TO ADD NEW HOTKEY ACTIONS
 * -----------------------------
 * 1.) Update the HotkeyActions enum below.
 * 2.) Modify HotkeyInfo::ActionNames (see HotkeyInfo.cpp) with 
 *     user-friendly names for the hotkey actions.
 *     NOTE: The array index positions of items in the enum and items in the
 *     list need to match.
 * 3.) Update the HotkeyInfo::Valid() method to validate hotkey arguments
 */

/// <summary>
/// Encapsulates hotkey information: physical keys involved, the action taken
/// when the keys are pressed, and any arguments the hotkey might have.
/// </summary>
class HotkeyInfo {
public:

    enum HotkeyActions {
        IncreaseVolume,
        DecreaseVolume,
        SetVolume,
        Mute,
        VolumeSlider,
        EjectDrive,
        EjectLastDisk,
        IncreaseBrightness,
        DecreaseBrightness,
        SetBrightness,
        BrightnessSlider,
        MediaKey,
        VirtualKey,
        Run,
        DisableOSD,
        Settings,
        Exit,
    };
    static std::vector<std::wstring> ActionNames;

    enum VolumeKeyArgTypes {
        NoArgs = -1,
        Units = 0,
        Percentage = 1,
    };

    enum MediaKeys {
        PlayPause,
        Stop,
        Next,
        Previous,
    };
    static std::vector<std::wstring> MediaKeyNames;
    static std::vector<unsigned short> MediaKeyVKs;

public:
    static VolumeKeyArgTypes VolumeArgType(HotkeyInfo &hki);

public:
    HotkeyInfo();

    int keyCombination = 0;
    int action = -1;
    std::vector<std::wstring> args;

    /// <summary>
    /// Retrieves the argument at the given index and converts it to an integer.
    /// Subsequent calls that specify the same index will be cached.
    /// </summary>
    int ArgToInt(unsigned int argIdx);

    /// <summary>
    /// Retrieves the argument at the given index and converts it to a double.
    /// Subsequent calls that specify the same index will be cached.
    /// </summary>
    double ArgToDouble(unsigned int argIdx);

    /// <summary>
    /// Retrieves the argument string at the given index, treats it as a hex
    /// string, and converts it to an integer.
    /// </summary>
    int HexArgToInt(unsigned int argIdx);

    bool HasArgs();
    bool HasArg(unsigned int argIdx);
    void AllocateArg(unsigned int argIdx);

    void EnableArgCache();
    void DisableArgCache();
    void ClearArgCache();

    bool Valid();

    std::wstring ToString();

private:
    bool _cache;
    std::map<int, int> _intArgs;
    std::map<int, double> _doubleArgs;

    void LogInvalid(std::wstring reason);
};