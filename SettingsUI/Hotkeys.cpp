#include "stdafx.h"
#include "SettingsUI.h"
#include "Hotkeys.h"
#include "afxdialogex.h"

#include "HotkeyInfo.h"
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
    DDX_Control(pDX, CMB_ACTION, _action);
}

BOOL Hotkeys::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    DWORD exStyle = _list.GetExtendedStyle();
    exStyle |= LVS_EX_FULLROWSELECT;
    _list.SetExtendedStyle(exStyle);

    /* Set up listview */
    RECT r;
    _list.GetWindowRect(&r);
    int width = r.right - r.left;

    _list.InsertColumn(0, L"Hotkeys", NULL, (int) (width * .485));
    _list.InsertColumn(1, L"Action", NULL, (int) (width * .445));

    /* Set up editor area */
    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action.AddString(action.c_str());
    }


    HotkeyInfo hi;
    hi.action = HotkeyInfo::DecreaseVolume;
    hi.keyCombination = 3262;
    _keyInfo.push_back(hi);

    for (HotkeyInfo hi : _keyInfo) {
        std::wstring hkStr = HotkeyManager::HotkeysToString(hi.keyCombination);
        _list.InsertItem(0, hkStr.c_str());
        _list.SetItemText(0, 1, HotkeyInfo::ActionNames[hi.action].c_str());
    }

    SelectItem(0);

    return TRUE;
}

void Hotkeys::SelectItem(int idx) {
    /* First unselect the selected item (not strictly necessary in our case) */
    int sel = _list.GetSelectionMark();
    if (sel >= 0) {
        _list.SetItemState(sel, ~LVIS_SELECTED, LVIS_SELECTED);
    }

    /* Make sure we're not selecting something out of range */
    if (idx < 0) {
        idx = 0;
    }
    int numItems = _list.GetItemCount();
    if (idx > numItems - 1) {
        idx = numItems - 1;
    }

    /* Select the item */
    _list.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
    _list.SetSelectionMark(idx);

    /* Scroll down to the item, if necessary */
    _list.EnsureVisible(idx, FALSE);
}

void Hotkeys::LoadSelection(int idx) {
    HotkeyInfo current = _keyInfo[_selIdx];

    _keys.SetWindowText(L"");

    if (current.keyCombination > 0) {
        std::wstring keyStr
            = HotkeyManager::HotkeysToString(current.keyCombination);
        _keys.SetWindowText(keyStr.c_str());
        _list.SetItemText(_selIdx, 0, keyStr.c_str());
    }

    if (current.action >= 0) {
        _list.SetItemText(_selIdx, 1,
            HotkeyInfo::ActionNames[current.action].c_str());
    }
    _action.SetCurSel(current.action);
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

    HotkeyInfo hi;
    _keyInfo.push_back(hi);
    int idx = _list.InsertItem(items, L"");
    _list.SetItemText(idx, 1, L"");
    SelectItem(idx);
}

void Hotkeys::OnBnClickedRemove() {
    _keyInfo.erase(_keyInfo.begin() + _selIdx);
    _list.DeleteItem(_selIdx);

    /* Select the item closest to the previous selection: */
    SelectItem(_selIdx);
}

void Hotkeys::OnLvnItemchangedKeys(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    if (pNMLV->uChanged & LVIF_STATE) {
        if (pNMLV->uNewState & LVIS_SELECTED) {
            int sel = pNMLV->iItem;
            OutputDebugString(std::to_wstring(sel).c_str());
            OutputDebugString(L"\n");
        }
    }

    *pResult = 0;
}

void Hotkeys::OnBnClickedKeys() {
    HotkeyPrompt hkp;
    hkp.DoModal();
    int keyCombo = KeyGrabber::Instance()->KeyCombination();
    if (keyCombo > 0) {
        std::wstring keyStr = HotkeyManager::HotkeysToString(keyCombo);
        _keys.SetWindowText(keyStr.c_str());
    }
}

void Hotkeys::OnCbnSelchangeAction() {
    _keyInfo[_selIdx].action = _action.GetCurSel();
    LoadSelection(_selIdx);
}
