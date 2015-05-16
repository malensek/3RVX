#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../3RVX/HotkeyInfo.h"
#include "../3RVX/HotkeyManager.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "HotkeyPrompt.h"
#include "KeyGrabber.h"
#include "resource.h"

void Hotkeys::Initialize() {
    using std::placeholders::_1;

    INIT_CONTROL(LST_KEYS, ListView, _keyList);
    _keyList.OnItemChange = std::bind(&Hotkeys::OnKeyListItemChange, this, _1);
    INIT_CONTROL(BTN_ADD, Button, _add);
    _add.OnClick = std::bind(&Hotkeys::OnAddButtonClick, this);
    INIT_CONTROL(BTN_REMOVE, Button, _remove);
    _remove.OnClick = std::bind(&Hotkeys::OnRemoveButtonClick, this);

    INIT_CONTROL(BTN_KEYS, Button, _keys);
    _keys.OnClick = std::bind(&Hotkeys::OnKeysButtonClick, this);

    INIT_CONTROL(CMB_ACTION, ComboBox, _action);
    _action.OnSelectionChange = std::bind(&Hotkeys::OnActionChange, this);

    INIT_CONTROL(LBL_ARG, Label, _argLabel);
    INIT_CONTROL(CHK_ARG, Checkbox, _argCheck);
    _argCheck.OnClick = std::bind(&Hotkeys::OnArgCheckChange, this);
    INIT_CONTROL(CMB_ARG, ComboBox, _argCombo);
    _argCombo.OnSelectionChange = std::bind(&Hotkeys::OnArgComboChange, this);
    INIT_CONTROL(ED_ARG, EditBox, _argEdit);
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();

    /* Make highlighted items span the entire row in the list view */
    _keyList.AddListExStyle(LVS_EX_FULLROWSELECT);

    RECT dims = _keyList.ScreenDimensions();
    int width = dims.right - dims.left;

    _keyList.AddColumn(hotkeysColumnStr, (int) (width * .485));
    _keyList.AddColumn(actionColumnStr, (int) (width * .445));

    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action.AddItem(action);
    }

    std::unordered_map<int, HotkeyInfo> hotkeys = settings->Hotkeys();
    for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it) {
        _keyInfo.push_back(it->second);
    }

    for (unsigned int i = 0; i < _keyInfo.size(); ++i) {
        HotkeyInfo hki = _keyInfo[i];
        std::wstring hkStr = HotkeyManager::HotkeysToString(hki.keyCombination);
        int idx = _keyList.AddItem(hkStr);
        _keyList.ItemText(idx, 1, HotkeyInfo::ActionNames[hki.action]);
    }

    _keyList.Selection(0);
    LoadSelection();
}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();
    settings->Hotkeys(_keyInfo);
}

void Hotkeys::LoadSelection() {
    int idx = _keyList.Selection();
    if (idx < 0) {
        return;
    }
    LoadSelection(idx);
}

void Hotkeys::LoadSelection(int index) {
    HotkeyInfo selection = _keyInfo[index];

    _keys.Text(L"");
    if (selection.keyCombination > 0) {
        std::wstring keyStr
            = HotkeyManager::HotkeysToString(selection.keyCombination);
        _keys.Text(keyStr);
        _keyList.ItemText(index, 0, keyStr);
    }

    int action = selection.action;
    if (action >= 0) {
        _keyList.ItemText(index, 1, HotkeyInfo::ActionNames[action]);
        LoadActionParameters(action, selection);
        _action.Select(action);
    }
}

void Hotkeys::LoadActionParameters(int action, HotkeyInfo &selection) {
    bool showLabel = false;
    bool showCheck = false;
    bool showCombo = false;
    bool showEdit = false;

    /* Clean things up */
    _argLabel.Text(L"");
    _argCheck.Text(L"");
    _argCheck.Checked(false);
    _argCombo.Clear();
    _argEdit.Clear();

    switch ((HotkeyInfo::HotkeyActions) action) {
    case HotkeyInfo::EjectDrive:
        _argLabel.Text(L"Drive:");
        for (int i = 0; i < 26; ++i) {
            wchar_t ch = (wchar_t) i + 65;
            _argCombo.AddItem(std::wstring(1, ch));
        }

        if (selection.HasArgs()) {
            _argCombo.Select(selection.args[0]);
        }

        showLabel = true;
        showCombo = true;
        break;

    case HotkeyInfo::MediaKey:
        _argLabel.Text(L"Key:");
        for (std::wstring keys : HotkeyInfo::MediaKeyNames) {
            _argCombo.AddItem(keys);
        }
        if (selection.HasArgs()) {
            _argCombo.Select(selection.args[0]);
        }

        showLabel = true;
        showCombo = true;
        break;
    }

    _argLabel.Visible(showLabel);
    _argCheck.Visible(showCheck);
    _argCombo.Visible(showCombo);
    _argEdit.Visible(showEdit);
}

bool Hotkeys::OnAddButtonClick() {
    int items = _keyList.Size();
    for (int i = 0; i < items; ++i) {
        if (_keyList.ItemText(i, 0) == L"" && _keyList.ItemText(i, 1) == L"") {
            /* We found a blank item already in the list */
            _keyList.Selection(i);
            return true;
        }
    }

    HotkeyInfo hi;
    _keyInfo.push_back(hi);
    int idx = _keyList.InsertItem(items, L"");
    _keyList.ItemText(idx, 1, L"");
    _keyList.Selection(idx);
    return true;
}

bool Hotkeys::OnRemoveButtonClick() {
    int sel = _keyList.Selection();
    if (sel < 0) {
        return false;
    }

    _keyInfo.erase(_keyInfo.begin() + sel);
    _keyList.RemoveItem(sel);

    /* Select the item closest to the previous selection: */
    _keyList.Selection(sel);

    /* This handles disabling the other controls if the last item was removed:*/
    if (_keyList.Size() <= 0) {
        LoadSelection();
    }

    return true;
}

bool Hotkeys::OnActionChange() {
    int actionIdx = _action.SelectionIndex();
    int selectionIdx = _keyList.Selection();
    HotkeyInfo &currentListSelection = _keyInfo[selectionIdx];
    currentListSelection.action = (HotkeyInfo::HotkeyActions) actionIdx;
    LoadSelection(selectionIdx);
    return true;
}

bool Hotkeys::OnKeysButtonClick() {
    HotkeyPrompt::Show(_hWnd);
    KeyGrabber::Instance()->Unhook();
    int keyCombo = KeyGrabber::Instance()->KeyCombination();
    if (keyCombo > 0) {
        std::wstring keyStr = HotkeyManager::HotkeysToString(keyCombo);
        _keys.Text(keyStr);
        int sel = _keyList.Selection();
        _keyInfo[sel].keyCombination = keyCombo;
        LoadSelection(sel);
    }
    return true;
}

void Hotkeys::OnKeyListItemChange(NMLISTVIEW *lv) {
    if (lv->uChanged & LVIF_STATE) {
        if (lv->uNewState & LVIS_SELECTED) {
            OnKeyListSelectionChange(lv->iItem);
        }
    }
}

void Hotkeys::OnKeyListSelectionChange(int index) {
    HotkeyInfo current = _keyInfo[index];
    CLOG(L"Selecting key combination %d:", index);
    QCLOG(L"%s", current.ToString().c_str());
    LoadSelection(index);
}

bool Hotkeys::OnArgComboChange() {
    int selectionIdx = _keyList.Selection();
    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action.SelectionIndex();

    switch (action) {
    case HotkeyInfo::EjectDrive:
    case HotkeyInfo::MediaKey:
        current->AllocateArg(0);
        current->args[0] = _argCombo.Selection();
        break;
    }

    LoadSelection(selectionIdx);
    return true;
}

bool Hotkeys::OnArgCheckChange() {
    _argCombo.Enabled(_argCheck.Checked());
    _argEdit.Enabled(_argCheck.Checked());
    return true;
}