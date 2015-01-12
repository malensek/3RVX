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

void HotkeyPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HotkeyPrompt, CDialog)
END_MESSAGE_MAP()


// HotkeyPrompt message handlers
