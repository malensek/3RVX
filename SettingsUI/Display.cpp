#include "stdafx.h"
#include "SettingsUI.h"
#include "Display.h"
#include "afxdialogex.h"

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
    return TRUE;
}

BEGIN_MESSAGE_MAP(Display, CPropertyPage)
END_MESSAGE_MAP()void Display::OnCbnSelchangePosition() {
    int selIdx = _position.GetCurSel();
    CString selected;
    _position.GetLBText(selIdx, selected);
    BOOL isCustom = (selected == L"Custom");
    _lbX.EnableWindow(isCustom);
    _lbY.EnableWindow(isCustom);
    _customX.EnableWindow(isCustom);
    _customY.EnableWindow(isCustom);
}
