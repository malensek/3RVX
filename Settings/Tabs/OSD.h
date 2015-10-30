#pragma once

#include "SettingsTab.h"

class OSD : public SettingsTab {
public:
    OSD(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title = L"");
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    /* Controls: */
    ListView *_osdList;

    GroupBox *_volumeGroup;
    Checkbox *_monitorVolEvents;
    ComboBox *_audioDevice;
    ComboBox *_audioTaper;
    //Slider
    Label *_limitValue;
    Checkbox *_forceLimit;

    GroupBox *_ejectGroup;
    Checkbox *_monitorEjectEvents;

    GroupBox *_keyboardGroup;
    Checkbox *_caps;
    Checkbox *_scroll;
    Checkbox *_num;
    Checkbox *_media;
};
