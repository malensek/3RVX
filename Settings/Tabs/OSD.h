#pragma once

#include "Tab.h"

class OSD : public Tab {
public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    /* Controls: */
    GroupBox _activeOSDgroup;
    Checkbox _volumeOSD;
    Checkbox _brightnessOSD;
    Checkbox _ejectOSD;
    Checkbox _keyboardOSD;

    GroupBox _notifyIconGroup;
    Checkbox _volumeIcon;
    Checkbox _brightnessIcon;
    Checkbox _ejectIcon;
    Checkbox _keyboardIcon;

    GroupBox _volumeGroup;
    Checkbox _monitorVolEvents;
    ComboBox _audioDevice;
    ComboBox _audioTaper;
    //Slider
    Label _limitValue;
    Checkbox _forceLimit;

    GroupBox _ejectGroup;
    Checkbox _monitorEjectEvents;

    GroupBox _keyboardGroup;
    Checkbox _caps;
    Checkbox _scroll;
    Checkbox _num;
    Checkbox _media;
};
