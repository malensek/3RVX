// HotkeyPrompt.cpp : implementation file
//

#include "stdafx.h"
#include "SettingsUI.h"
#include "HotkeyPrompt.h"
#include "afxdialogex.h"


// HotkeyPrompt dialog

IMPLEMENT_DYNAMIC(HotkeyPrompt, CDialog)

HotkeyPrompt::HotkeyPrompt(CWnd* pParent /*=NULL*/)
	: CDialog(HotkeyPrompt::IDD, pParent)
{

}

HotkeyPrompt::~HotkeyPrompt()
{
}

BOOL HotkeyPrompt::OnInitDialog() {
    CDialog::OnInitDialog();
    KeyGrabber::Instance()->Grab(_keys.m_hWnd);
    return TRUE;
}


BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
END_MESSAGE_MAP()


// HotkeyPrompt message handlers
