#include "stdafx.h"
#include "SettingsUI.h"
#include "HotkeyPrompt.h"
#include "afxdialogex.h"

#include "KeyGrabber.h"

IMPLEMENT_DYNAMIC(HotkeyPrompt, CDialog)

HotkeyPrompt::HotkeyPrompt(CWnd* pParent /*=NULL*/) :
CDialog(HotkeyPrompt::IDD, pParent) {

}

HotkeyPrompt::~HotkeyPrompt() {

}

void HotkeyPrompt::ReceiveKeys(int combination) {
    _keys.SetWindowText(std::to_wstring(combination).c_str());
}

BOOL HotkeyPrompt::OnInitDialog() {
    CDialog::OnInitDialog();
    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    KeyGrabber::Instance()->Grab(*this);
    return TRUE;
}

BOOL HotkeyPrompt::OnClose() {
    CDialog::OnClose();
    KeyGrabber::Instance()->Stop();
    return TRUE;
}

void HotkeyPrompt::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, LBL_KEYS, _keys);
    DDX_Control(pDX, BTN_SAVE, _save);
}

BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
END_MESSAGE_MAP()