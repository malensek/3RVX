#include "stdafx.h"
#include "SettingsUI.h"
#include "Display.h"
#include "afxdialogex.h"

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
}

BOOL Display::OnApply() {
    return CPropertyPage::OnApply();
}

BOOL Display::OnCommand(WPARAM wParam, LPARAM lParam) {
    SetModified();
    return CPropertyPage::OnCommand(wParam, lParam);
}

BOOL Display::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    _position.AddString(L"Top");
    _position.AddString(L"Left");
    _position.AddString(L"Right");
    _position.AddString(L"Bottom");
    _position.AddString(L"Center");
    _position.AddString(L"Custom");

    _spinDelay.SetRange32(MIN_MS, MAX_MS);
    _spinSpeed.SetRange32(MIN_MS, MAX_MS);
    return TRUE;
}

BEGIN_MESSAGE_MAP(Display, CPropertyPage)
    ON_NOTIFY(UDN_DELTAPOS, SP_DELAY, &Display::OnDeltaposDelay)
    ON_NOTIFY(UDN_DELTAPOS, SP_SPEED, &Display::OnDeltaposSpeed)
    ON_CBN_SELCHANGE(CMB_POSITION, &Display::OnCbnSelchangePosition)
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
