#pragma once

#include <unordered_map>

#include "HotkeyProcessor.h"

class HotkeyInfo;

class KeyboardHotkeyProcessor : HotkeyProcessor {
public:
    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    /// <summary>Contains media key to VK mappings.</summary>
    static std::unordered_map<std::wstring, unsigned short> mediaKeyMap;

    /// <summary>Creates the media key to VK map.</summary>
    static std::unordered_map<std::wstring, unsigned short> CreateKeyMap();
};