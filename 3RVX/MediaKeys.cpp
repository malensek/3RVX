#include "MediaKeys.h"

#include <string>

#include "FakeKeyboard.h"
#include "HotkeyInfo.h"

std::unordered_map<std::wstring, unsigned short> MediaKeys::mediaKeyMap
    = MediaKeys::CreateKeyMap();

void MediaKeys::ProcessHotkeys(HotkeyInfo &hki) {
    if (hki.action != HotkeyInfo::MediaKey || hki.args.size() != 1) {
        return;
    }

    std::wstring arg = hki.args[0];
    unsigned short vk = mediaKeyMap[arg];
    CLOG(L"Simulating media keypress: %s", arg.c_str());
    FakeKeyboard::SimulateKeypress(vk);
}

std::unordered_map<std::wstring, unsigned short> MediaKeys::CreateKeyMap() {
    std::unordered_map<std::wstring, unsigned short> map;
    for (unsigned int i = 0; i < HotkeyInfo::MediaKeyNames.size(); ++i) {
        map[HotkeyInfo::MediaKeyNames[i]] = HotkeyInfo::MediaKeyVKs[i];
    }
    return map;
}
