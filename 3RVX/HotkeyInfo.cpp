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
    L"Run Application",
    L"Open Settings Dialog",
    L"Exit 3RVX",
};

int HotkeyInfo::ArgToInt(int argIdx) {
    std::wistringstream str(args[argIdx]);
    int i;
    str >> i;
    return i;
}

double HotkeyInfo::ArgToDouble(int argIdx) {
    std::wistringstream str(args[argIdx]);
    double d;
    str >> d;
    return d;
}

std::wstring HotkeyInfo::ToString() {
    std::wstring combination = HotkeyManager::HotkeysToString(keyCombination);
    std::wstring act = ActionNames[action];
    std::wstring argStrs;
    for (std::wstring arg : args) {
        argStrs += L"'" + arg + L"' ";
    }
    return combination + L" -> " + act + L" [ " + argStrs + L"]";
}

