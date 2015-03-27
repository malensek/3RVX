#include "stdafx.h"
#include "SettingsUI.h"
#include "Hotkeys.h"
#include "afxdialogex.h"

#include <unordered_map>

#include "HotkeyInfo.h"
#include "HotkeyPrompt.h"
#include "KeyGrabber.h"
#include "Settings.h"

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
    DDX_Control(pDX, LBL_ARG, _argument);
    DDX_Control(pDX, CMB_DRIVE, _argCombo);
}

BOOL Hotkeys::OnApply() {
    OutputDebugString(L"-> Hotkeys\n");
    Settings *settings = Settings::Instance();
    settings->Hotkeys(_keyInfo);
    UIUtils::SaveSettings(*this);
    return CPropertyPage::OnApply();
}


BOOL Hotkeys::OnCommand(WPARAM wParam, LPARAM lParam) {
    SetModified();
    return CPropertyPage::OnCommand(wParam, lParam);
}

BOOL Hotkeys::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    Settings *settings = Settings::Instance();

    DWORD exStyle = _list.GetExtendedStyle();
    exStyle |= LVS_EX_FULLROWSELECT;
    _list.SetExtendedStyle(exStyle);

    /* Set up listview and editor area */
    RECT r;
    _list.GetWindowRect(&r);
    int width = r.right - r.left;

    _list.InsertColumn(0, L"Hotkeys", NULL, (int) (width * .485));
    _list.InsertColumn(1, L"Action", NULL, (int) (width * .445));

    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action.AddString(action.c_str());
    }

    std::unordered_map<int, HotkeyInfo> hotkeys = settings->Hotkeys();
    for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it) {
        _keyInfo.push_back(it->second);
    }

    for (unsigned int i = 0; i < _keyInfo.size(); ++i) {
        LoadSelection(i);
        HotkeyInfo hki = _keyInfo[i];
        std::wstring hkStr = HotkeyManager::HotkeysToString(hki.keyCombination);
        _list.InsertItem(i, hkStr.c_str());
        _list.SetItemText(i, 1, HotkeyInfo::ActionNames[hki.action].c_str());
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

        switch ((HotkeyInfo::HotkeyActions) current.action) {
        case HotkeyInfo::EjectDrive:
            _argument.SetWindowTextW(L"Drive:");
            _argCombo.ResetContent();
            for (int i = 0; i < 26; ++i) {
                wchar_t ch = (wchar_t) i + 65;
                _argCombo.InsertString(i, CString(ch));
            }
            if (current.args.size() > 0) {
                _argCombo.SelectString(0, current.args[0].c_str());
            }

            _argument.ShowWindow(SW_SHOW);
            _argCombo.ShowWindow(SW_SHOW);
            break;

        case HotkeyInfo::MediaKey: {
            _argument.SetWindowTextW(L"Action:");
            _argCombo.ResetContent();
            unsigned int numMediaKeys = HotkeyInfo::MediaKeyNames.size();
            for (unsigned int i = 0; i < numMediaKeys; ++i) {
                _argCombo.InsertString(i, HotkeyInfo::MediaKeyNames[i].c_str());
            }
            if (current.args.size() > 0) {
                //TODO: this needs to be translated again
                _argCombo.SelectString(0, current.args[0].c_str());
            }

            _argument.ShowWindow(SW_SHOW);
            _argCombo.ShowWindow(SW_SHOW);
        }
            break;

        default:
            _argument.ShowWindow(SW_HIDE);
            _argCombo.ShowWindow(SW_HIDE);
        }
    } else {
        /* TODO fix these nasty repeated hides */
        _argument.ShowWindow(SW_HIDE);
        _argCombo.ShowWindow(SW_HIDE);
    }

    _action.SetCurSel(current.action);
}

BEGIN_MESSAGE_MAP(Hotkeys, CPropertyPage)
    ON_BN_CLICKED(BTN_ADD, &Hotkeys::OnBnClickedAdd)
    ON_BN_CLICKED(BTN_REMOVE, &Hotkeys::OnBnClickedRemove)
    ON_NOTIFY(LVN_ITEMCHANGED, LST_KEYS, &Hotkeys::OnLvnItemchangedKeys)
    ON_BN_CLICKED(BTN_KEYS, &Hotkeys::OnBnClickedKeys)
    ON_CBN_SELCHANGE(CMB_ACTION, &Hotkeys::OnCbnSelchangeAction)
    ON_CBN_SELCHANGE(CMB_ARG, &Hotkeys::OnCbnSelchangeArg)
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
            _selIdx = sel;
            OutputDebugString(std::to_wstring(sel).c_str());
            OutputDebugString(L"\n");
            LoadSelection(sel);
        }
    }

    *pResult = 0;
}

void Hotkeys::OnBnClickedKeys() {
    HotkeyPrompt hkp;
    hkp.DoModal();
    KeyGrabber::Instance()->Unhook();
    int keyCombo = KeyGrabber::Instance()->KeyCombination();
    if (keyCombo > 0) {
        std::wstring keyStr = HotkeyManager::HotkeysToString(keyCombo);
        _keys.SetWindowText(keyStr.c_str());
        _keyInfo[_selIdx].keyCombination = keyCombo;
        LoadSelection(_selIdx);
    }
}

void Hotkeys::OnCbnSelchangeAction() {
    int sel = _action.GetCurSel();
    _keyInfo[_selIdx].action = sel;
    LoadSelection(_selIdx);
}

void Hotkeys::OnCbnSelchangeArg() {
    HotkeyInfo *current = &_keyInfo[_selIdx];

    HotkeyInfo::HotkeyActions action 
        = (HotkeyInfo::HotkeyActions) _action.GetCurSel();

    switch (action) {
    case HotkeyInfo::EjectDrive: {
        wchar_t buf[2];
        _argCombo.GetWindowText(buf, 2);
        if (current->args.size() <= 0) {
            current->args.resize(1);
        }
        current->args[0] = buf;
    }
        break;

    case HotkeyInfo::MediaKey: {
        wchar_t buf[128];
        _argCombo.GetWindowText(buf, 128);
        if (current->args.size() <= 0) {
            current->args.resize(1);
        }
        current->args[0] = buf;
    }
        break;

    }
    LoadSelection(_selIdx);
}
