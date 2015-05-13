#include "HotkeyInfo.h"

#include <sstream>

#include "HotkeyManager.h"

std::vector<std::wstring> HotkeyInfo::ActionNames = {
    L"Increase Volume",
    L"Decrease Volume",
    L"Set Volume",
    L"Mute",
    L"Show Volume Slider",
    L"Eject Drive",
    L"Eject Latest Drive",
    L"Media Key",
    L"Run Application",
    L"Open Settings Dialog",
    L"Exit 3RVX",
};

std::vector<std::wstring> HotkeyInfo::MediaKeyNames = {
    L"Play/Pause",
    L"Stop",
    L"Next",
    L"Previous",
};

std::vector<unsigned short> HotkeyInfo::MediaKeyVKs = {
    VK_MEDIA_PLAY_PAUSE,
    VK_MEDIA_STOP,
    VK_MEDIA_NEXT_TRACK,
    VK_MEDIA_PREV_TRACK,
};

int HotkeyInfo::ArgToInt(unsigned int argIdx) {
    if (_intArgs.count(argIdx) > 0) {
        return _intArgs[argIdx];
    }

    std::wistringstream str(args[argIdx]);
    int i;
    str >> i;

    _intArgs[argIdx] = i;

    return i;
}

double HotkeyInfo::ArgToDouble(unsigned int argIdx) {
    if (_doubleArgs.count(argIdx) > 0) {
        return _doubleArgs[argIdx];
    }

    std::wistringstream str(args[argIdx]);
    double d;
    str >> d;

    _doubleArgs[argIdx] = d;

    return d;
}

bool HotkeyInfo::HasArgs() {
    return args.size() > 0;
}

void HotkeyInfo::AllocateArg(unsigned int index) {
    unsigned int newSize = index + 1;
    if (args.size() >= newSize) {
        return;
    }
    args.resize(newSize);
}

std::wstring HotkeyInfo::ToString() {
    std::wstring combination = HotkeyManager::HotkeysToString(keyCombination);
    std::wstring act = L"(none)";
    if (action > 0 && (unsigned int) action < ActionNames.size()) {
        act = ActionNames[action];
    }
    std::wstring argStrs;
    for (std::wstring arg : args) {
        argStrs += L"'" + arg + L"' ";
    }
    return combination + L" -> " + act + L" [ " + argStrs + L"]";
}

