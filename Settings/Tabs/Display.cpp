// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Display.h"

#include <CommCtrl.h>

#include "../../3RVX/DefaultSettings.h"
#include "../../3RVX/DisplayManager.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../resource.h"

void Display::Initialize() {
    using std::placeholders::_1;

    _visibilityGroup = new GroupBox(GRP_VISIBILITY, *this);
    _onTop = new Checkbox(CHK_ONTOP, *this);
    _hideFullscreen = new Checkbox(CHK_FULLSCREEN, *this);
    _hideDirectX = new Checkbox(CHK_DIRECTX, *this);

    _positionGroup = new GroupBox(GRP_POSITION, *this);
    _position = new ComboBox(CMB_POSITION, *this);
    _position->OnSelectionChange
        = std::bind(&Display::OnPositionChange, this);
    _customX = new Label(LBL_CUSTOMX, *this);
    _positionX = new EditBox(ED_CUSTOMX, *this);
    _customY = new Label(LBL_CUSTOMY, *this);
    _positionY = new EditBox(ED_CUSTOMY, *this);

    _customDistance = new Checkbox(CHK_EDGE, *this);
    _customDistance->OnClick = std::bind(&Display::OnCustomCheckChange, this);
    _edgeSpinner = new Spinner(SP_EDGE, *this);
    _edgeSpinner->Buddy(ED_EDGE);
    _displayDevLabel = new Label(LBL_DISPLAYDEV, *this);
    _displayDevice = new ComboBox(CMB_MONITOR, *this);

    _animationGroup = new GroupBox(GRP_ANIMATION, *this);
    _hideAnimation = new ComboBox(CMB_ANIMATION, *this);
    _hideAnimation->OnSelectionChange
        = std::bind(&Display::OnAnimationChange, this);
    _hideDelayLabel = new Label(LBL_HIDEDELAY, *this);
    _hideDelay = new Spinner(SP_HIDEDELAY, *this);
    _hideDelay->Buddy(ED_HIDEDELAY);
    _hideDelay->OnSpin = std::bind(&Display::OnAnimationSpin, this, _1);
    _hideSpeedLabel = new Label(LBL_HIDESPEED, *this);
    _hideSpeed = new Spinner(SP_HIDESPEED, *this);
    _hideSpeed->Buddy(ED_HIDESPEED);
    _hideSpeed->OnSpin = std::bind(&Display::OnAnimationSpin, this, _1);
}

void Display::LoadSettings() {
    Settings *settings = Settings::Instance();
    LanguageTranslator *translator = settings->Translator();

    /* Translations */
    _allMonitorStr = translator->Translate(_allMonitorStr);
    _primaryMonitorStr = translator->Translate(_primaryMonitorStr);
    _customPositionStr = translator->Translate(_customPositionStr);
    _noAnimStr = translator->Translate(_noAnimStr);

    /* Visibility Settings */
    _onTop->Checked(settings->AlwaysOnTop());
    _hideFullscreen->Checked(settings->HideFullscreen());
    _hideDirectX->Checked(settings->HideDirectX());

    /* Position on Screen*/
    for (std::wstring position : settings->OSDPosNames) {
        _position->AddItem(translator->Translate(position));
    }
    _position->Select((int) settings->OSDPosition());

    /* Custom positions/offsets */
    _positionX->Text(settings->OSDX());
    _positionY->Text(settings->OSDY());
    _customDistance->Checked(
        settings->OSDEdgeOffset() != DefaultSettings::OSDOffset);
    _edgeSpinner->Text(settings->OSDEdgeOffset());
    _edgeSpinner->Range(MIN_EDGE, MAX_EDGE);

    /* Display Devices */
    _displayDevice->AddItem(_primaryMonitorStr);
    _displayDevice->AddItem(_allMonitorStr);
    std::list<DISPLAY_DEVICE> devices = DisplayManager::ListAllDevices();
    for (DISPLAY_DEVICE dev : devices) {
        std::wstring devString(dev.DeviceName);
        _displayDevice->AddItem(devString);
    }
    std::wstring monitorName = settings->Monitor();
    if (monitorName == L"") {
        monitorName = _primaryMonitorStr;
    } else if (monitorName == L"*") {
        monitorName = _allMonitorStr;
    }
    _displayDevice->Select(monitorName);

    /* Animation Settings */
    for (std::wstring anim : AnimationTypes::HideAnimationNames) {
        _hideAnimation->AddItem(translator->Translate(anim));
    }
    _hideAnimation->Select((int) settings->HideAnim());
    _hideDelay->Range(MIN_MS, MAX_MS);
    _hideDelay->Text(settings->HideDelay());
    _hideSpeed->Range(MIN_MS, MAX_MS);
    _hideSpeed->Text(settings->HideSpeed());

    /* Refresh control states */
    OnPositionChange();
    OnCustomCheckChange();
    OnAnimationChange();
}

void Display::SaveSettings() {
    CLOG(L"Saving: Display");
    Settings *settings = Settings::Instance();

    settings->AlwaysOnTop(_onTop->Checked());
    settings->HideFullscreen(_hideFullscreen->Checked());
    settings->HideDirectX(_hideDirectX->Checked());

    Settings::OSDPos pos = (Settings::OSDPos) _position->SelectionIndex();
    settings->OSDPosition(pos);
    if (pos == Settings::OSDPos::Custom) {
        settings->OSDX(_positionX->TextAsInt());
        settings->OSDY(_positionY->TextAsInt());
    }

    if (_customDistance->Checked()) {
        settings->OSDEdgeOffset(_edgeSpinner->TextAsInt());
    } else {
        /* We have to write the default here, just in case somebody unchecked
         * the checkbox. */
        settings->OSDEdgeOffset(DefaultSettings::OSDOffset);
    }

    std::wstring monitor = _displayDevice->Selection();
    int monitorIdx = _displayDevice->SelectionIndex();
    if (monitorIdx == 0) {
        monitor = L"";
    } else if (monitorIdx == 1) {
        monitor = L"*";
    }
    settings->Monitor(monitor);

    settings->HideAnim(
        (AnimationTypes::HideAnimation) _hideAnimation->SelectionIndex());

    settings->HideDelay(_hideDelay->TextAsInt());
    settings->HideSpeed(_hideSpeed->TextAsInt());
}

bool Display::OnAnimationChange() {
    _hideSpeed->Enabled(_hideAnimation->Selection() != _noAnimStr);
    return true;
}

bool Display::OnAnimationSpin(NMUPDOWN *ud) {
    /* Increase the up/down spin increment: */
    ud->iDelta *= ANIM_SPIN_INCREMENT;
    return FALSE; /* Allows the change */
}

bool Display::OnCustomCheckChange() {
    _edgeSpinner->Enabled(_customDistance->Checked());
    return true;
}

bool Display::OnPositionChange() {
    bool isCustom = (_position->Selection() == _customPositionStr);
    _customX->Enabled(isCustom);
    _customY->Enabled(isCustom);
    _positionX->Enabled(isCustom);
    _positionY->Enabled(isCustom);
    return true;
}
