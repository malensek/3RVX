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

bool HotkeyPrompt::Hook() {
    _mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
        LowLevelMouseProc, NULL, NULL);

    _keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,
        LowLevelKeyboardProc, NULL, NULL);

    return _mouseHook && _keyHook;
}

bool HotkeyPrompt::Unhook() {
    BOOL unMouse = UnhookWindowsHookEx(_mouseHook);
    BOOL unKey = UnhookWindowsHookEx(_keyHook);
    return unMouse && unKey;
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
    DDX_Control(pDX, BTN_SAVE, _save);
}

LRESULT CALLBACK
HotkeyPrompt::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
HotkeyPrompt::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
END_MESSAGE_MAP()

