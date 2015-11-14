// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../resource.h"

void OSD::Initialize() {
    using std::placeholders::_1;

    _osdList = new ListView(LST_OSDS, *this);
    _osdList->AddListExStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    _osdList->AddColumn(L"OSD", (int) (_osdList->Width() * .97f));
    _osdList->AddItem(L"Volume");
    _osdList->AddItem(L"Brightness");
    _osdList->AddItem(L"Eject");
    _osdList->AddItem(L"Keyboard");
    _osdList->OnItemChange = std::bind(&OSD::OnOSDListItemChange, this, _1);

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
    _limitSlider->Buddy(_limitValue);
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

    _ejectIcon = new Checkbox(CHK_EJECTICON, *this);
    _monitorEjectEvents = new Checkbox(CHK_MONITOREJECT, *this);
    _ejectGroup = new GroupBox(GRP_EJECT, *this);
    _ejectGroup->AddChildren({
        _ejectIcon,
        _monitorEjectEvents
    });

    _brightnessIcon = new Checkbox(CHK_BRIGHTICON, *this);
    _brightnessGroup = new GroupBox(GRP_BRIGHTNESS, *this);
    _brightnessGroup->AddChildren({
        _brightnessIcon
    });

    _keyboardIcon = new Checkbox(CHK_KEYICON, *this);
    _caps = new Checkbox(CHK_ENABLECAPS, *this);
    _scroll = new Checkbox(CHK_ENABLESCROLL, *this);
    _num = new Checkbox(CHK_ENABLENUM, *this);
    _media = new Checkbox(CHK_ENABLEMK, *this);
    _keyboardGroup = new GroupBox(GRP_KEYBOARD, *this);
    _keyboardGroup->AddChildren({
        _keyboardIcon,
        _caps, _scroll, _num,
        _media
    });

    /* Define groupbox order */
    GroupBox *groups[] = {
        _volumeGroup,
        _brightnessGroup,
        _ejectGroup,
        _keyboardGroup
    };

    /* Move other groupboxes into position (same as volume) */
    int groupX = _volumeGroup->X();
    int groupY = _volumeGroup->Y();
    for (GroupBox *grp : groups) {
        grp->X(groupX);
        grp->Y(groupY);
        grp->Visible(false);
        _groups.push_back(grp);
    }

    _osdList->Selection(0);
}

void OSD::LoadSettings() {
    Settings *settings = Settings::Instance();
    LanguageTranslator *translator = settings->Translator();

    _osdList->Checked(0, settings->VolumeOSDEnabled());
    _osdList->Checked(1, settings->BrightnessOSDEnabled());
    _osdList->Checked(2, settings->EjectOSDEnabled());
    _osdList->Checked(3, settings->KeyboardOSDEnabled());

}

void OSD::ShowGroup(int group) {
    for (GroupBox *grp : _groups) {
        grp->Visible(false);
    }

    GroupBox *showGroup = _groups[group];
    if (_osdList->Checked(group)) {
        showGroup->Enable();
    } else {
        showGroup->Disable();
    }
    showGroup->Visible(true);
}

void OSD::OnOSDListItemChange(NMLISTVIEW *lv) {
    if (lv->uChanged & LVIF_STATE) {
        UINT oSimg = lv->uOldState & LVIS_STATEIMAGEMASK;
        UINT nSimg = lv->uNewState & LVIS_STATEIMAGEMASK;
        if (oSimg != nSimg) {
            /* Checkbox changed */
            if (_osdList->Selection() == lv->iItem) {
                /* Activate the Apply button */
                PropSheet_Changed(GetParent(SettingsTab::DialogHandle()), NULL);

                /* Check state at 0xF000 (1 = un, 2 = check). Shift to check: */
                _groups[lv->iItem]->Enabled((nSimg >> 13) > 0);
            }
        }

        if (lv->uNewState & LVIS_SELECTED) {
            CLOG(L"selecting");
            ShowGroup(lv->iItem);
        }
    }
}

void OSD::SaveSettings() {

}