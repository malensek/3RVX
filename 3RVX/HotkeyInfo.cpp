#include "HotkeyInfo.h"

#include "HotkeyManager.h"

std::vector<std::wstring> HotkeyInfo::ActionNames = {
    L"Increase Volume",
    L"Decrease Volume",
    L"Set Volume",
    L"Mute",
    L"Show Volume Slider",
    L"Run Application",
    L"Open Settings Dialog",
    L"Exit 3RVX",
};

std::wstring HotkeyInfo::ToString() {
    std::wstring combination = HotkeyManager::HotkeysToString(keyCombination);
    std::wstring act = ActionNames[action];
    std::wstring argStrs;
    for (std::wstring arg : args) {
        argStrs += L"'" + arg + L"' ";
    }
    return combination + L" -> " + act + L" [ " + argStrs + L"]";
}