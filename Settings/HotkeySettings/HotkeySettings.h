#pragma once

#include <string>

#include "../../3RVX/HotkeyInfo.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../Tabs/Hotkeys.h"

class Hotkeys;

class HotkeySettings {
public:
    HotkeySettings(Hotkeys *hk);
    virtual void LoadHotkeyInfo(HotkeyInfo &hki) = 0;

protected:
    Hotkeys *_hkTab;
    LanguageTranslator *_translator;

    void ActionDescription(std::wstring description);
    void ControlVisibility();

private:
    const int CTRL_CHECK = 1;
    const int CTRL_COMBO = CTRL_CHECK << 1;
    const int CTRL_DROP = CTRL_COMBO << 1;
};