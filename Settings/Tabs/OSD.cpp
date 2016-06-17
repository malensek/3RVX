// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../../3RVX/Controllers/Volume/CoreAudio.h"
#include "../../3RVX/Controllers/Volume/VolumeController.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../resource.h"

void OSD::Initialize() {
    using std::placeholders::_1;

    _osdList = new ListView(LST_OSDS, *this);
    _osdList->OnItemChange = std::bind(&OSD::OnOSDListItemChange, this, _1);

    _volumeIcon = new Checkbox(CHK_VOLICON, *this);
    _subscribeVolEvents = new Checkbox(CHK_MONITORVOL, *this);
    _audioDeviceLabel = new Label(LBL_AUDIODEV, *this);
    _audioDevice = new ComboBox(CMB_AUDIODEV, *this);
    _audioTaperLabel = new Label(LBL_AUDIOTAPER, *this);
    _audioTaper = new ComboBox(CMB_AUDIOTAPER, *this);
    _audioTaperEdit = new EditBox(ED_AUDIOTAPER, *this);
    _audioTaper->OnSelectionChange = [this]() {
        _audioTaperEdit->Enabled(_volumeGroup->Enabled()
            && _audioTaper->SelectionIndex() == _audioTaper->Count() - 1);
        return true;
    };

    _limitLabel = new Label(LBL_LIMITER, *this);
    _limitSlider = new Slider(SLD_LIMITER, *this);
    _limitValue = new Label(LBL_LIMITVAL, *this);
    _limitSlider->Buddy(_limitValue);
    _limitSlider->OnSlide = [this]() {
        std::wstring value = std::to_wstring(_limitSlider->Position()) + L"%";
        _limitValue->Text(value);
        return true;
    };
    _forceLimit = new Checkbox(CHK_FORCELIMIT, *this);
    _muteLock = new Checkbox(CHK_MUTELOCK, *this);
    _volumeGroup = new GroupBox(GRP_VOLUME, *this);
    _volumeGroup->AddChildren({
        _volumeIcon,
        _subscribeVolEvents,
        _audioDeviceLabel, _audioDevice,
        _audioTaperLabel, _audioTaper, _audioTaperEdit,
        _limitLabel, _limitSlider, _limitValue,
        _forceLimit,
        _muteLock,
    });

    _ejectIcon = new Checkbox(CHK_EJECTICON, *this);
    _subscribeEjectEvents = new Checkbox(CHK_SUBSCRIBEEJECT, *this);
    _ejectGroup = new GroupBox(GRP_EJECT, *this);
    _ejectGroup->AddChildren({
        _ejectIcon,
        _subscribeEjectEvents,
    });

    _brightnessIcon = new Checkbox(CHK_BRIGHTICON, *this);
    _brightnessGroup = new GroupBox(GRP_BRIGHTNESS, *this);
    _brightnessGroup->AddChildren({
        _brightnessIcon,
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
        _media,
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
}

void OSD::LoadSettings() {
    Settings *settings = Settings::Instance();
    LanguageTranslator *translator = settings->Translator();

    /* Translations */
    _osdStr = translator->Translate(_osdStr);
    _volumeStr = translator->Translate(_volumeStr);
    _brightnessStr = translator->Translate(_brightnessStr);
    _ejectStr = translator->Translate(_ejectStr);
    _keyboardStr = translator->Translate(_keyboardStr);

    _audioDevice->AddItem(translator->Translate(L"Default"));
    _audioDevice->Select(0);
    std::wstring selectedId = settings->AudioDeviceID();
    CoreAudio *volumeCtrl = new CoreAudio(NULL);
    volumeCtrl->Init();
    _audioDevices = volumeCtrl->ListDevices();
    for (VolumeController::DeviceInfo dev : _audioDevices) {
        _audioDevice->AddItem(dev.name);
        if (dev.id == selectedId) {
            _audioDevice->Select(dev.name);
        }
    }
    volumeCtrl->Dispose();

    _taperLevels[0] = L"Disabled";
    _taperLevels[2] = L"Low";
    _taperLevels[4] = L"Medium";
    _taperLevels[6] = L"High";
    for (auto it = _taperLevels.begin(); it != _taperLevels.end(); ++it) {
        _audioTaper->AddItem(translator->Translate(it->second));
    }
    _audioTaper->AddItem(translator->Translate(L"Custom"));

    int level = settings->VolumeCurveAdjustment();
    if (_taperLevels.find(level) == _taperLevels.end()) {
        /* Select the last item (custom) */
        _audioTaper->Select(_taperLevels.size());
        _audioTaperEdit->Text(settings->VolumeCurveAdjustment());
    } else {
        _audioTaper->Select(translator->Translate(_taperLevels[level]));
    }
    _audioTaper->OnSelectionChange();

    _osdList->AddListExStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    _osdList->AddColumn(_osdStr, (int) (_osdList->Width() * .97f));
    _osdList->AddItem(_volumeStr);
    _osdList->AddItem(_brightnessStr);
    _osdList->AddItem(_ejectStr);
    _osdList->AddItem(_keyboardStr);
    _osdList->Selection(0);

    _osdList->Checked(0, settings->VolumeOSDEnabled());
    _osdList->Checked(1, settings->BrightnessOSDEnabled());
    _osdList->Checked(2, settings->EjectOSDEnabled());
    _osdList->Checked(3, settings->KeyboardOSDEnabled());

    _volumeIcon->Checked(settings->VolumeIconEnabled());
    _subscribeVolEvents->Checked(settings->SubscribeVolumeEvents());
    _limitSlider->Position((int) (settings->VolumeLimiter() * 100.0f));
    _muteLock->Checked(settings->MuteOnLock());

    _ejectIcon->Checked(settings->EjectIconEnabled());
    _subscribeEjectEvents->Checked(settings->SubscribeEjectEvents());
}

void OSD::SaveSettings() {
    CLOG(L"Saving: OSD");
    Settings *settings = Settings::Instance();
    
    settings->VolumeOSDEnabled(_osdList->Checked(0));
    settings->BrightnessOSDEnabled(_osdList->Checked(1));
    settings->EjectOSDEnabled(_osdList->Checked(2));
    settings->KeyboardOSDEnabled(_osdList->Checked(3));

    settings->VolumeIconEnabled(_volumeIcon->Checked());
    settings->SubscribeVolumeEvents(_subscribeVolEvents->Checked());
    int selectedDevice = _audioDevice->SelectionIndex();
    if (selectedDevice == 0) {
        settings->AudioDeviceID(L"");
    } else {
        settings->AudioDeviceID(_audioDevices[selectedDevice - 1].id);
    }
    if (_audioTaper->SelectionIndex() == _audioTaper->Count() - 1) {
        /* Custom taper */
        settings->VolumeCurveAdjustment(_audioTaperEdit->TextAsInt());
    } else {
        /* NOTE: We multiply the taper index by two to get the actual taper
         * value. For instance, low = 2, medium = 4, etc. */
        settings->VolumeCurveAdjustment(_audioTaper->SelectionIndex() * 2);
    }
    settings->VolumeLimiter(((float) _limitSlider->Position()) / 100.0f);
    settings->MuteOnLock(_muteLock->Checked());

    settings->EjectIconEnabled(_ejectIcon->Checked());
    settings->SubscribeEjectEvents(_subscribeEjectEvents->Checked());
}

void OSD::ShowGroup(int group) {
    for (GroupBox *grp : _groups) {
        grp->Visible(false);
    }

    GroupBox *showGroup = _groups[group];
    showGroup->Enabled(_osdList->Checked(group));
    showGroup->Visible(true);

    _audioTaper->OnSelectionChange();
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

                /* NOTE: Check state at 0xF000 (1 = un, 2 = check).
                 * Shift to check:
                 * if ((nSimg >> 13) > 0) { checked } else { unchecked }
                 */

                ShowGroup(lv->iItem);
            }
        }

        if (lv->uNewState & LVIS_SELECTED) {
            ShowGroup(lv->iItem);
        }
    }
}