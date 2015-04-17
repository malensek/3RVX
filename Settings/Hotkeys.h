#pragma once

#include "Tab.h"

#include "../3RVX/HotkeyInfo.h"
class HotkeyInfo;

class Hotkeys : public Tab {
public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    std::vector<HotkeyInfo> _keyInfo;

private:
    /* Controls: */
    ListBox _keyList;
    Button _add;
    Button _remove;

    Button _keys;
    ComboBox _action;
};