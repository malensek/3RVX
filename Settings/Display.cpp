#include "Display.h"

#include <CommCtrl.h>

#include "../3RVX/DisplayManager.h"
#include "../3RVX/LanguageTranslator.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"

#include "UIContext.h"
#include "resource.h"

void Display::Initialize() {
    using std::placeholders::_1;

    INIT_CONTROL(CHK_ONTOP, Checkbox, _onTop);
    INIT_CONTROL(CHK_FULLSCREEN, Checkbox, _hideFullscreen);

    INIT_CONTROL(CMB_POSITION, ComboBox, _position);
    _position.OnSelectionChange
        = std::bind(&Display::OnPositionChanged, this);
    INIT_CONTROL(LBL_CUSTOMX, Label, _customX);
    INIT_CONTROL(ED_CUSTOMX, EditBox, _positionX);
    INIT_CONTROL(LBL_CUSTOMY, Label, _customY);
    INIT_CONTROL(ED_CUSTOMY, EditBox, _positionY);
    INIT_CONTROL(CHK_EDGE, Checkbox, _customDistance);
    _customDistance.OnClick = std::bind(&Display::OnCustomCheckChanged, this);
    INIT_CONTROL(SP_EDGE, Spinner, _edgeSpinner);
    _edgeSpinner.Buddy(ED_EDGE);

    INIT_CONTROL(LBL_DISPLAYDEV, Label, _displayDevLabel);
    INIT_CONTROL(CMB_MONITOR, ComboBox, _displayDevice);

    INIT_CONTROL(CMB_ANIMATION, ComboBox, _hideAnimation);
    _hideAnimation.OnSelectionChange
        = std::bind(&Display::OnAnimationChanged, this);
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

    /* Visibility Settings */
    _onTop.Checked(settings->AlwaysOnTop());
    _hideFullscreen.Checked(settings->HideFullscreen());

    /* Position on Screen*/
    for (std::wstring position : settings->OSDPosNames) {
        _position.AddItem(position);
    }
    _position.Select((int) settings->OSDPosition());

    /* Custom positions/offsets */
    _positionX.Text(settings->OSDX());
    _positionY.Text(settings->OSDY());
    _customDistance.Checked(settings->OSDEdgeOffset() != DEFAULT_OSD_OFFSET);
    _edgeSpinner.Text(settings->OSDEdgeOffset());
    _edgeSpinner.Range(MIN_EDGE, MAX_EDGE);

    /* Display Devices */
    _displayDevice.AddItem(primaryMonitorStr);
    _displayDevice.AddItem(allMonitorStr);
    std::list<DISPLAY_DEVICE> devices = DisplayManager::ListAllDevices();
    for (DISPLAY_DEVICE dev : devices) {
        std::wstring devString(dev.DeviceName);
        _displayDevice.AddItem(devString);
    }
    std::wstring monitorName = settings->Monitor();
    if (monitorName == L"") {
        monitorName = primaryMonitorStr;
    } else if (monitorName == L"*") {
        monitorName = allMonitorStr;
    }
    _displayDevice.Select(monitorName);

    /* Animation Settings */
    for (std::wstring anim : AnimationTypes::HideAnimationNames) {
        _hideAnimation.AddItem(anim);
    }
    _hideAnimation.Select((int) settings->HideAnim());
    _hideDelay.Range(MIN_MS, MAX_MS);
    _hideDelay.Text(settings->HideDelay());
    _hideSpeed.Range(MIN_MS, MAX_MS);
    _hideSpeed.Text(settings->HideSpeed());

    /* Refresh control states */
    OnPositionChanged();
    OnCustomCheckChanged();
    OnAnimationChanged();
}

void Display::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Display");
    Settings *settings = Settings::Instance();

    settings->AlwaysOnTop(_ctxt->GetCheck(CHK_ONTOP));
    settings->HideFullscreen(_ctxt->GetCheck(CHK_FULLSCREEN));

    Settings::OSDPos pos = (Settings::OSDPos)
        _ctxt->GetComboSelectionIndex(CMB_POSITION);
    settings->OSDPosition(pos);
    if (pos == Settings::OSDPos::Custom) {
        int x = _ctxt->GetTextAsInt(ED_CUSTOMX);
        int y = _ctxt->GetTextAsInt(ED_CUSTOMY);
        settings->OSDX(x);
        settings->OSDY(y);
    }

    if (_ctxt->GetCheck(CHK_EDGE) == true) {
        int edge = _ctxt->GetTextAsInt(ED_EDGE);
        settings->OSDEdgeOffset(edge);
    } else {
        /* We have to write the default here, just in case somebody unchecked
         * the checkbox. */
        settings->OSDEdgeOffset(DEFAULT_OSD_OFFSET);
    }

    std::wstring monitor = _ctxt->GetComboSelection(CMB_MONITOR);
    int monitorIdx = _ctxt->GetComboSelectionIndex(CMB_MONITOR);
    if (monitorIdx == 0) {
        monitor = L"";
    } else if (monitorIdx == 1) {
        monitor = L"*";
    }
    settings->Monitor(monitor);

    int hideAnimIdx = _ctxt->GetComboSelectionIndex(CMB_ANIMATION);
    settings->HideAnim((AnimationTypes::HideAnimation) hideAnimIdx);

    settings->HideDelay(_ctxt->GetTextAsInt(ED_DELAY));
    settings->HideSpeed(_ctxt->GetTextAsInt(ED_SPEED));
}

bool Display::OnAnimationChanged() {
    _hideSpeed.Enabled(_hideAnimation.Selection() != noAnimStr);
    return true;
}

bool Display::OnAnimationSpin(NMUPDOWN *ud) {
    /* Increase the up/down spin increment: */
    ud->iDelta *= ANIM_SPIN_INCREMENT;
    return FALSE; /* Allows the change */
}

bool Display::OnCustomCheckChanged() {
    _edgeSpinner.Enabled(_customDistance.Checked());
    return true;
}

bool Display::OnPositionChanged() {
    bool isCustom = (_position.Selection() == customPositionStr);
    _customX.Enabled(isCustom);
    _customY.Enabled(isCustom);
    _positionX.Enabled(isCustom);
    _positionY.Enabled(isCustom);
    return true;
}
