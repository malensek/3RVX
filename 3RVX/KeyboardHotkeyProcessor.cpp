#include "KeyboardHotkeyProcessor.h"

#include <string>

#include "SyntheticKeyboard.h"
#include "HotkeyInfo.h"
#include "Logger.h"

std::unordered_map<std::wstring, unsigned short>
    KeyboardHotkeyProcessor::mediaKeyMap = CreateKeyMap();

void KeyboardHotkeyProcessor::ProcessHotkeys(HotkeyInfo &hki) {
    if (hki.action != HotkeyInfo::MediaKey || hki.args.size() != 1) {
        return;
    }

    std::wstring arg = hki.args[0];
    unsigned short vk = mediaKeyMap[arg];
    CLOG(L"Simulating media keypress: %s", arg.c_str());
    SyntheticKeyboard::SimulateKeypress(vk);
}

std::unordered_map<std::wstring, unsigned short>
        KeyboardHotkeyProcessor::CreateKeyMap() {
    std::unordered_map<std::wstring, unsigned short> map;
    for (unsigned int i = 0; i < HotkeyInfo::MediaKeyNames.size(); ++i) {
        map[HotkeyInfo::MediaKeyNames[i]] = HotkeyInfo::MediaKeyVKs[i];
    }
    return map;
}
