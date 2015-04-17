#pragma once

#include "Tab.h"

class Hotkeys : public Tab {
public:
    virtual void SaveSettings();

    virtual void Initialize();
    virtual void LoadSettings();
};