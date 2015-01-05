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
END_MESSAGE_MAP()