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

}

void Display::LoadSettings() {
    Settings *settings = Settings::Instance();

    /* Visibility Settings */
    _ctxt->SetCheck(CHK_ONTOP, settings->AlwaysOnTop());
    _ctxt->SetCheck(CHK_FULLSCREEN, settings->HideFullscreen());

    /* Position on Screen*/
    for (std::wstring pos : settings->OSDPosNames) {
        _ctxt->AddComboItem(CMB_POSITION, pos);
    }
    _ctxt->SelectComboItem(CMB_POSITION, (int) settings->OSDPosition());

    /* Custom positions/offsets */
    _ctxt->SetText(ED_CUSTOMX, settings->OSDX());
    _ctxt->SetText(ED_CUSTOMY, settings->OSDY());
    _ctxt->SetCheck(CHK_EDGE, settings->OSDEdgeOffset() != DEFAULT_OSD_OFFSET);
    _ctxt->SetText(ED_EDGE, settings->OSDEdgeOffset());
    _ctxt->SetSpinRange(SP_EDGE, MIN_EDGE, MAX_EDGE);

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