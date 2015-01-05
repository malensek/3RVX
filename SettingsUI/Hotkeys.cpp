#include "stdafx.h"
#include "SettingsUI.h"
#include "Hotkeys.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(Hotkeys, CPropertyPage)

Hotkeys::Hotkeys() :
CPropertyPage(Hotkeys::IDD) {

}

Hotkeys::~Hotkeys() {

}

void Hotkeys::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Hotkeys, CPropertyPage)
END_MESSAGE_MAP()
