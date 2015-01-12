#include "stdafx.h"
#include "SettingsUI.h"
#include "Hotkeys.h"
#include "afxdialogex.h"

#include "HotkeyPrompt.h"
#include "KeyGrabber.h"

IMPLEMENT_DYNAMIC(Hotkeys, CPropertyPage)

Hotkeys::Hotkeys() :
CPropertyPage(Hotkeys::IDD) {

}

Hotkeys::~Hotkeys() {
}

void Hotkeys::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, LST_KEYS, _list);
    DDX_Control(pDX, BTN_KEYS, _keys);
}

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

void Hotkeys::SelectItem(int idx) {
    /* First unselect the selected item (not strictly necessary in our case) */
    int sel = _list.GetSelectionMark();
    _list.SetItemState(sel, ~LVIS_SELECTED, LVIS_SELECTED);

    /* Select the item */
    _list.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
    _list.SetSelectionMark(idx);

    /* Scroll down to the item, if necessary */
    _list.EnsureVisible(idx, FALSE);
}

BOOL Hotkeys::PreTranslateMessage(PMSG msg) {
    /*
    if (msg->message == WM_KEYDOWN) {
        wchar_t buf[255];
        GetKeyNameText(msg->lParam, buf, 255);
        OutputDebugString(buf);
    }

    */
    return CPropertyPage::PreTranslateMessage(msg);
}

BEGIN_MESSAGE_MAP(Hotkeys, CPropertyPage)
    ON_BN_CLICKED(BTN_ADD, &Hotkeys::OnBnClickedAdd)
    ON_BN_CLICKED(BTN_REMOVE, &Hotkeys::OnBnClickedRemove)
    ON_NOTIFY(LVN_ITEMCHANGED, LST_KEYS, &Hotkeys::OnLvnItemchangedKeys)
    ON_BN_CLICKED(BTN_KEYS, &Hotkeys::OnBnClickedKeys)
END_MESSAGE_MAP()

void Hotkeys::OnBnClickedAdd() {
    int items = _list.GetItemCount();
    for (int i = 0; i < items; ++i) {
        if (_list.GetItemText(i, 0) == L""
            && _list.GetItemText(i, 1) == L"") {
            /* We found a blank item already in the list */
            SelectItem(i);
            return;
        }
    }

    int idx = _list.InsertItem(items, L"");
    _list.SetItemText(idx, 1, L"");
    SelectItem(idx);
}

void Hotkeys::OnBnClickedRemove() {

}

void Hotkeys::OnLvnItemchangedKeys(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void Hotkeys::OnBnClickedKeys() {
    HotkeyPrompt hkp;
    hkp.DoModal();
}
