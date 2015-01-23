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

BOOL HotkeyPrompt::OnInitDialog() {
    CDialog::OnInitDialog();
    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    KeyGrabber::Instance()->SetHwnd(_keys.m_hWnd);
    KeyGrabber::Instance()->Grab();
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
}

BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
    ON_BN_CLICKED(BTN_CANCEL, &HotkeyPrompt::OnBnClickedCancel)
END_MESSAGE_MAP()



void HotkeyPrompt::OnBnClickedCancel() {
    /* Close Window here */
}
