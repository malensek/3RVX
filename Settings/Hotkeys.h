#pragma once

#include "Tab.h"

class Hotkeys : public Tab {
public:
    virtual void SaveSettings();

    virtual void Initialize();
    virtual void LoadSettings();
private:
    /* Controls: */
    ListBox _keyList;
    Button _add;
    Button _remove;

    Button _keys;
    ComboBox _action;
};