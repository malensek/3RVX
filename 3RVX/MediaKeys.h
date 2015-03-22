#pragma once

#include <unordered_map>

#include "HotkeyProcessor.h"

class HotkeyInfo;

class MediaKeys {
public:
    static void ProcessHotkeys(HotkeyInfo &hki);

private:
    static std::unordered_map<std::wstring, unsigned short> CreateKeyMap();
    static std::unordered_map<std::wstring, unsigned short> mediaKeyMap;
};