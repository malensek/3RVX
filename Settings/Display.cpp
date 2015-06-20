#include "Display.h"

#include <CommCtrl.h>

#include "../3RVX/DisplayManager.h"
#include "../3RVX/LanguageTranslator.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"

#include "resource.h"

void Display::Initialize() {
    using std::placeholders::_1;

    INIT_CONTROL(GRP_VISIBILITY, GroupBox, _visibilityGroup);
    INIT_CONTROL(CHK_ONTOP, Checkbox, _onTop);
    INIT_CONTROL(CHK_FULLSCREEN, Checkbox, _hideFullscreen);

    INIT_CONTROL(GRP_POSITION, GroupBox, _positionGroup);
    INIT_CONTROL(CMB_POSITION, ComboBox, _position);
    _position.OnSelectionChange
        = std::bind(&Display::OnPositionChange, this);
    INIT_CONTROL(LBL_CUSTOMX, Label, _customX);
    INIT_CONTROL(ED_CUSTOMX, EditBox, _positionX);
    INIT_CONTROL(LBL_CUSTOMY, Label, _customY);
    INIT_CONTROL(ED_CUSTOMY, EditBox, _positionY);
    INIT_CONTROL(CHK_EDGE, Checkbox, _customDistance);
    _customDistance.OnClick = std::bind(&Display::OnCustomCheckChange, this);
    INIT_CONTROL(SP_EDGE, Spinner, _edgeSpinner);
    _edgeSpinner.Buddy(ED_EDGE);
    INIT_CONTROL(LBL_DISPLAYDEV, Label, _displayDevLabel);
    INIT_CONTROL(CMB_MONITOR, ComboBox, _displayDevice);

    INIT_CONTROL(GRP_ANIMATION, GroupBox, _animationGroup);
    INIT_CONTROL(CMB_ANIMATION, ComboBox, _hideAnimation);
    _hideAnimation.OnSelectionChange
        = std::bind(&Display::OnAnimationChange, this);
    INIT_CONTROL(LBL_HIDEDELAY, Label, _hideDelayLabel);
    INIT_CONTROL(SP_HIDEDELAY, Spinner, _hideDelay);
    _hideDelay.Buddy(ED_HIDEDELAY);
    _hideDelay.OnSpin = std::bind(&Display::OnAnimationSpin, this, _1);
    INIT_CONTROL(LBL_HIDESPEED, Label, _hideSpeedLabel);
    INIT_CONTROL(SP_HIDESPEED, Spinner, _hideSpeed);
    _hideSpeed.Buddy(ED_HIDESPEED);
    _hideSpeed.OnSpin = std::bind(&Display::OnAnimationSpin, this, _1);
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
    _onTop.Checked(settings->AlwaysOnTop());
    _hideFullscreen.Checked(settings->HideFullscreen());

    /* Position on Screen*/
    for (std::wstring position : settings->OSDPosNames) {
        _position.AddItem(translator->Translate(position));
    }
    _position.Select((int) settings->OSDPosition());

    /* Custom positions/offsets */
    _positionX.Text(settings->OSDX());
    _positionY.Text(settings->OSDY());
    _customDistance.Checked(
        settings->OSDEdgeOffset() != Settings::DefaultOSDOffset);
    _edgeSpinner.Text(settings->OSDEdgeOffset());
    _edgeSpinner.Range(MIN_EDGE, MAX_EDGE);

    /* Display Devices */
    _displayDevice.AddItem(_primaryMonitorStr);
    _displayDevice.AddItem(_allMonitorStr);
    std::list<DISPLAY_DEVICE> devices = DisplayManager::ListAllDevices();
    for (DISPLAY_DEVICE dev : devices) {
        std::wstring devString(dev.DeviceName);
        _displayDevice.AddItem(devString);
    }
    std::wstring monitorName = settings->Monitor();
    if (monitorName == L"") {
        monitorName = _primaryMonitorStr;
    } else if (monitorName == L"*") {
        monitorName = _allMonitorStr;
    }
    _displayDevice.Select(monitorName);

    /* Animation Settings */
    for (std::wstring anim : AnimationTypes::HideAnimationNames) {
        _hideAnimation.AddItem(translator->Translate(anim));
    }
    _hideAnimation.Select((int) settings->HideAnim());
    _hideDelay.Range(MIN_MS, MAX_MS);
    _hideDelay.Text(settings->HideDelay());
    _hideSpeed.Range(MIN_MS, MAX_MS);
    _hideSpeed.Text(settings->HideSpeed());

    /* Refresh control states */
    OnPositionChange();
    OnCustomCheckChange();
    OnAnimationChange();
}

void Display::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Display");
    Settings *settings = Settings::Instance();

    settings->AlwaysOnTop(_onTop.Checked());
    settings->HideFullscreen(_hideFullscreen.Checked());

    Settings::OSDPos pos = (Settings::OSDPos) _position.SelectionIndex();
    settings->OSDPosition(pos);
    if (pos == Settings::OSDPos::Custom) {
        settings->OSDX(_positionX.TextAsInt());
        settings->OSDY(_positionY.TextAsInt());
    }

    if (_customDistance.Checked()) {
        settings->OSDEdgeOffset(_edgeSpinner.TextAsInt());
    } else {
        /* We have to write the default here, just in case somebody unchecked
         * the checkbox. */
        settings->OSDEdgeOffset(Settings::DefaultOSDOffset);
    }

    std::wstring monitor = _displayDevice.Selection();
    int monitorIdx = _displayDevice.SelectionIndex();
    if (monitorIdx == 0) {
        monitor = L"";
    } else if (monitorIdx == 1) {
        monitor = L"*";
    }
    settings->Monitor(monitor);

    settings->HideAnim(
        (AnimationTypes::HideAnimation) _hideAnimation.SelectionIndex());

    settings->HideDelay(_hideDelay.TextAsInt());
    settings->HideSpeed(_hideSpeed.TextAsInt());
}

bool Display::OnAnimationChange() {
    _hideSpeed.Enabled(_hideAnimation.Selection() != _noAnimStr);
    return true;
}

bool Display::OnAnimationSpin(NMUPDOWN *ud) {
    /* Increase the up/down spin increment: */
    ud->iDelta *= ANIM_SPIN_INCREMENT;
    return FALSE; /* Allows the change */
}

bool Display::OnCustomCheckChange() {
    _edgeSpinner.Enabled(_customDistance.Checked());
    return true;
}

bool Display::OnPositionChange() {
    bool isCustom = (_position.Selection() == _customPositionStr);
    _customX.Enabled(isCustom);
    _customY.Enabled(isCustom);
    _positionX.Enabled(isCustom);
    _positionY.Enabled(isCustom);
    return true;
}
