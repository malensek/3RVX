#include "stdafx.h"
#include "SettingsUI.h"
#include "Display.h"
#include "afxdialogex.h"

#include "Monitor.h"
#include "Settings.h"

#define MIN_EDGE -65535
#define MAX_EDGE 65535
#define MIN_MS USER_TIMER_MINIMUM
#define MAX_MS 60000

IMPLEMENT_DYNAMIC(Display, CPropertyPage)

Display::Display() :
CPropertyPage(Display::IDD) {
}

Display::~Display() {

}

void Display::DoDataExchange(CDataExchange* pDX) {
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, CHK_ONTOP, _onTop);
    DDX_Control(pDX, SP_DELAY, _spinDelay);
    DDX_Control(pDX, SP_SPEED, _spinSpeed);
    DDX_Control(pDX, CHK_FULLSCREEN, _fullscreen);
    DDX_Control(pDX, CMB_POSITION, _position);
    DDX_Control(pDX, LBL_CUSTOMX, _lbX);
    DDX_Control(pDX, LBL_CUSTOMY, _lbY);
    DDX_Control(pDX, ED_CUSTOMX, _customX);
    DDX_Control(pDX, ED_CUSTOMY, _customY);
    DDX_Control(pDX, ED_DELAY, _delay);
    DDX_Control(pDX, ED_SPEED, _speed);
    DDX_Control(pDX, CMB_ANIMATION, _animation);
    DDX_Control(pDX, CMB_MONITOR, _monitor);
    DDX_Control(pDX, SP_EDGE, _customEdge);
    DDX_Control(pDX, CHK_EDGE, _useCustomEdge);
    DDX_Control(pDX, ED_EDGE, _edgeEdit);
}

BOOL Display::OnApply() {
    OutputDebugString(L"-> Display\n");
    Settings *settings = Settings::Instance();

    settings->AlwaysOnTop(CHECKED(_onTop));
    settings->HideFullscreen(CHECKED(_fullscreen));

    Settings::OSDPos pos = (Settings::OSDPos) _position.GetCurSel();
    settings->OSDPosition(pos);
    if (pos == Settings::OSDPos::Custom) {
        int x = UIUtils::TextToInt(_customX);
        int y = UIUtils::TextToInt(_customY);

        settings->OSDX(x);
        settings->OSDY(y);
    }
    
    if (CHECKED(_useCustomEdge)) {
        int edge = UIUtils::TextToInt(_edgeEdit);
        settings->OSDEdgeOffset(edge);
    } else {
        /* We have to write the default here, just in case somebody unchecked
         * the checkbox. */
        settings->OSDEdgeOffset(DEFAULT_OSD_OFFSET);
    }

    int monitorIdx = _monitor.GetCurSel();
    CString monitorStr;
    _monitor.GetLBText(monitorIdx, monitorStr);
    if (monitorIdx == 0) {
        monitorStr = L"";
    } else if (monitorIdx == 1) {
        monitorStr = L"*";
    }
    settings->Monitor((LPCWSTR) monitorStr);

    int hideAnimIdx = _animation.GetCurSel();
    settings->HideAnimation((Settings::HideAnim) hideAnimIdx);

    int hideDelay = UIUtils::TextToInt(_delay);
    settings->HideDelay(hideDelay);

    int hideSpeed = UIUtils::TextToInt(_speed);
    settings->HideSpeed(hideSpeed);

    UIUtils::SaveSettings(*this);
    return CPropertyPage::OnApply();
}

BOOL Display::OnCommand(WPARAM wParam, LPARAM lParam) {
    SetModified();
    return CPropertyPage::OnCommand(wParam, lParam);
}

BOOL Display::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    Settings *settings = Settings::Instance();

    /* Visibility Settings */
    _onTop.SetCheck(settings->AlwaysOnTop());
    _fullscreen.SetCheck(settings->HideFullscreen());

    /* Position on Screen*/
    for (std::wstring pos : settings->OSDPosNames) {
        CString positionName = UIUtils::Capitalize(pos);
        _position.AddString(positionName);
    }
    Settings::OSDPos position = settings->OSDPosition();
    _position.SetCurSel((int) position);
    OnCbnSelchangePosition();
    _customEdge.SetRange32(MIN_EDGE, MAX_EDGE);
    _customX.SetWindowText(std::to_wstring(settings->OSDX()).c_str());
    _customY.SetWindowText(std::to_wstring(settings->OSDY()).c_str());

    _useCustomEdge.SetCheck(settings->OSDEdgeOffset() != DEFAULT_OSD_OFFSET);
    int edge = settings->OSDEdgeOffset();
    std::wstring edgeStr = std::to_wstring(edge);
    _edgeEdit.SetWindowTextW(edgeStr.c_str());
    OnBnClickedEdge();

    _monitor.AddString(L"Primary Monitor");
    _monitor.AddString(L"All Monitors");
    std::list<DISPLAY_DEVICE> devices = Monitor::ListAllDevices();
    for (DISPLAY_DEVICE dev : devices) {
        std::wstring devString(dev.DeviceName);
        _monitor.AddString(devString.c_str());
    }
    std::wstring monitorName = settings->Monitor();
    if (monitorName == L"") {
        monitorName = L"Primary Monitor";
    } else if (monitorName == L"*") {
        monitorName = L"All Monitors";
    }
    _monitor.SelectString(0, monitorName.c_str());

    /* Animation Settings */
    for (std::wstring anim : settings->HideAnimNames) {
        CString animName = UIUtils::Capitalize(anim);
        _animation.AddString(animName);
    }
    Settings::HideAnim hAnim = settings->HideAnimation();
    _animation.SetCurSel((int) hAnim);

    _spinDelay.SetRange32(MIN_MS, MAX_MS);
    int delay = settings->HideDelay();
    std::wstring delayStr = std::to_wstring(delay);
    _delay.SetWindowTextW(delayStr.c_str());

    _spinSpeed.SetRange32(MIN_MS, MAX_MS);
    int speed = settings->HideSpeed();
    std::wstring speedStr = std::to_wstring(speed);
    _speed.SetWindowTextW(speedStr.c_str());

    return TRUE;
}

BEGIN_MESSAGE_MAP(Display, CPropertyPage)
    ON_NOTIFY(UDN_DELTAPOS, SP_DELAY, &Display::OnDeltaposDelay)
    ON_NOTIFY(UDN_DELTAPOS, SP_SPEED, &Display::OnDeltaposSpeed)
    ON_CBN_SELCHANGE(CMB_POSITION, &Display::OnCbnSelchangePosition)
    ON_BN_CLICKED(CHK_EDGE, &Display::OnBnClickedEdge)
END_MESSAGE_MAP()

void Display::OnDeltaposDelay(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    pNMUpDown->iDelta *= 100;
    *pResult = 0;
}


void Display::OnDeltaposSpeed(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    pNMUpDown->iDelta *= 100;
    *pResult = 0;
}

void Display::OnCbnSelchangePosition() {
    int selIdx = _position.GetCurSel();
    CString selected;
    _position.GetLBText(selIdx, selected);
    BOOL isCustom = (selected == L"Custom");
    _lbX.EnableWindow(isCustom);
    _lbY.EnableWindow(isCustom);
    _customX.EnableWindow(isCustom);
    _customY.EnableWindow(isCustom);
}

void Display::OnBnClickedEdge() {
    _customEdge.EnableWindow(CHECKED(_useCustomEdge));
    _edgeEdit.EnableWindow(CHECKED(_useCustomEdge));
}