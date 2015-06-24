#pragma once

#include "HotkeySettings.h"

class Eject : HotkeySettings {
public:
    Eject(Hotkeys *hk);
    virtual void LoadHotkeyInfo(HotkeyInfo &hki);

};