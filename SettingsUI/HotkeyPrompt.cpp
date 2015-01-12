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
    KeyGrabber::Instance()->Grab(_keys.m_hWnd);
    return TRUE;
}

void HotkeyPrompt::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, LBL_KEYS, _keys);
    DDX_Control(pDX, BTN_SAVE, _save);
}

BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
END_MESSAGE_MAP()

