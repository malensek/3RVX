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
BOOL Hotkeys::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    DWORD exStyle = _list.GetExtendedStyle();
    exStyle |= LVS_EX_FULLROWSELECT;
    _list.SetExtendedStyle(exStyle);

    RECT r;
    _list.GetWindowRect(&r);
    int width = r.right - r.left;

    _list.InsertColumn(0, L"Hotkeys", NULL, (int) (width * .465));
    _list.InsertColumn(1, L"Action", NULL, (int) (width * .465));

    _list.InsertItem(0, L"CTRL+ALT+OemVolumeUp");
    _list.SetItemText(0, 1, L"Increase Volume 5% [Parallels Audio Controller]");

    return TRUE;
}
}

BEGIN_MESSAGE_MAP(Hotkeys, CPropertyPage)
END_MESSAGE_MAP()
