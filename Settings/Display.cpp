#include "Display.h"

#include <CommCtrl.h>

#include "../3RVX/DisplayManager.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"

#include "UIContext.h"
#include "resource.h"

DLGPROC Display::Command(unsigned short nCode, unsigned short ctrlId) {
    switch (nCode) {
    case BN_CLICKED:
        switch (ctrlId) {
        case CHK_EDGE:
            OnCustomCheckChanged();
            return (DLGPROC) TRUE;
        }

        break;

    case CBN_SELCHANGE:
        switch (ctrlId) {
        case CMB_POSITION:
            OnPositionChanged();
            return (DLGPROC) TRUE;

        case CMB_ANIMATION:
            OnAnimationChanged();
            return (DLGPROC) TRUE;
        }

        break;
    }

    return FALSE;
}

DLGPROC Display::Notification(NMHDR *nHdr) {
    unsigned int ctrlId = nHdr->idFrom;
    switch (nHdr->code) {
    case UDN_DELTAPOS:
        if (ctrlId == SP_DELAY || ctrlId == SP_SPEED) {
            NMUPDOWN *ud = (NMUPDOWN *) nHdr;
            ud->iDelta *= 100;
            return FALSE; /* Allows the change */
        }
    }

    return FALSE;
}

void Display::Initialize() {
    INIT_CONTROL(CHK_ONTOP, Checkbox, _onTop);
    INIT_CONTROL(CHK_FULLSCREEN, Checkbox, _hideFullscreen);

    INIT_CONTROL(CMB_POSITION, ComboBox, _position);
    INIT_CONTROL(LBL_CUSTOMX, Label, _customX);
    INIT_CONTROL(ED_CUSTOMX, EditBox, _positionX);
    INIT_CONTROL(LBL_CUSTOMY, Label, _customY);
    INIT_CONTROL(ED_CUSTOMY, EditBox, _positionY);
    INIT_CONTROL(CHK_EDGE, Checkbox, _customDistance);
    INIT_CONTROL(SP_EDGE, Spinner, _edgeSpinner);
    _edgeSpinner.Buddy(ED_EDGE);

    INIT_CONTROL(LBL_DISPLAYDEV, Label, _displayDevLabel);
    INIT_CONTROL(CMB_MONITOR, ComboBox, _displayDevice);

    INIT_CONTROL(CMB_ANIMATION, ComboBox, _animation);
    INIT_CONTROL(LBL_HIDEDELAY, Label, _hideDelayLabel);
    INIT_CONTROL(SP_HIDEDELAY, Spinner, _hideDelay);
    _hideDelay.Buddy(ED_HIDEDELAY);
    INIT_CONTROL(LBL_HIDESPEED, Label, _hideSpeedLabel);
    INIT_CONTROL(SP_HIDESPEED, Spinner, _hideSpeed);
    _hideSpeed.Buddy(ED_HIDESPEED);
}

void Display::LoadSettings() {
    Settings *settings = Settings::Instance();

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
    _ctxt->AddComboItem(CMB_MONITOR, L"Primary Monitor");
    _ctxt->AddComboItem(CMB_MONITOR, L"All Monitors");
    std::list<DISPLAY_DEVICE> devices = DisplayManager::ListAllDevices();
    for (DISPLAY_DEVICE dev : devices) {
        std::wstring devString(dev.DeviceName);
        _ctxt->AddComboItem(CMB_MONITOR, devString.c_str());
    }
    std::wstring monitorName = settings->Monitor();
    if (monitorName == L"") {
        monitorName = L"Primary Monitor";
    } else if (monitorName == L"*") {
        monitorName = L"All Monitors";
    }
    _ctxt->SelectComboItem(CMB_MONITOR, monitorName);

    /* Animation Settings */
    for (std::wstring anim : AnimationTypes::HideAnimationNames) {
        _ctxt->AddComboItem(CMB_ANIMATION, anim);
    }
    _ctxt->SelectComboItem(CMB_ANIMATION, (int) settings->HideAnim());
    _ctxt->SetSpinRange(SP_DELAY, MIN_MS, MAX_MS);
    _ctxt->SetText(ED_DELAY, settings->HideDelay());
    _ctxt->SetSpinRange(SP_SPEED, MIN_MS, MAX_MS);
    _ctxt->SetText(ED_SPEED, settings->HideSpeed());

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

void Display::OnPositionChanged() {
    std::wstring sel = _ctxt->GetComboSelection(CMB_POSITION);
    bool isCustom = (sel == L"Custom");
    _ctxt->SetEnabled(LBL_CUSTOMX, isCustom);
    _ctxt->SetEnabled(LBL_CUSTOMY, isCustom);
    _ctxt->SetEnabled(ED_CUSTOMX, isCustom);
    _ctxt->SetEnabled(ED_CUSTOMY, isCustom);
}

void Display::OnCustomCheckChanged() {
    bool checked = _ctxt->GetCheck(CHK_EDGE);
    _ctxt->SetEnabled(ED_EDGE, checked);
    _ctxt->SetEnabled(SP_EDGE, checked);
}

void Display::OnAnimationChanged() {
    std::wstring sel = _ctxt->GetComboSelection(CMB_ANIMATION);
    bool animEnabled = (sel != L"None");
    _ctxt->SetEnabled(ED_SPEED, animEnabled);
}