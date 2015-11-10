// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../resource.h"

#include "../../3RVX/Window.h"

void OSD::Initialize() {
    _osdList = new ListView(LST_OSDS, *this);
    _osdList->AddListExStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    _osdList->AddColumn(L"OSD", (int) (_osdList->Width() * .97f));
    _osdList->AddItem(L"Volume");
    _osdList->AddItem(L"Brightness");
    _osdList->AddItem(L"Eject");
    _osdList->AddItem(L"Keyboard");

    _volumeIcon = new Checkbox(CHK_VOLICON, *this);
    _monitorVolEvents = new Checkbox(CHK_MONITORVOL, *this);
    _audioDeviceLabel = new Label(LBL_AUDIODEV, *this);
    _audioDevice = new ComboBox(CMB_AUDIODEV, *this);
    _audioTaperLabel = new Label(LBL_AUDIOTAPER, *this);
    _audioTaper = new ComboBox(CMB_AUDIOTAPER, *this);
    _audioTaperEdit = new EditBox(ED_AUDIOTAPER, *this);
    _limitLabel = new Label(LBL_LIMITER, *this);
    _limitSlider = new Slider(SLD_LIMITER, *this);
    _limitValue = new Label(LBL_LIMITVAL, *this);
    _forceLimit = new Checkbox(CHK_FORCELIMIT, *this);
    _volumeGroup = new GroupBox(GRP_VOLUME, *this);
    _volumeGroup->AddChildren({
        _volumeIcon,
        _monitorVolEvents,
        _audioDeviceLabel, _audioDevice,
        _audioTaperLabel, _audioTaper, _audioTaperEdit,
        _limitLabel, _limitSlider, _limitValue,
        _forceLimit
    });
    _volumeGroup->Visible(false);
}

void OSD::LoadSettings() {

}

void OSD::SaveSettings() {

}