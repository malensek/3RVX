// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "SettingsTab.h"

class OSD : public SettingsTab {
public:
    OSD() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_OSD), L"OSD") {

    }


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
