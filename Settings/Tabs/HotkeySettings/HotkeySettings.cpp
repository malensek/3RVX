#include "HotkeySettings.h"

#include "../../../3RVX/Settings.h"

HotkeySettings::HotkeySettings(Hotkeys *hk) {
    _hkTab = hk;
    _translator = Settings::Instance()->Translator();
}

void HotkeySettings::ActionDescription(std::wstring description) {

}