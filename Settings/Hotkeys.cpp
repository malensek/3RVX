#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../3RVX/HotkeyInfo.h"
#include "../3RVX/HotkeyManager.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "resource.h"

void Hotkeys::Initialize() {
    using std::placeholders::_1;

    INIT_CONTROL(LST_KEYS, ListView, _keyList);
    _keyList.OnItemChange = std::bind(&Hotkeys::OnKeyListItemChange, this, _1);
    INIT_CONTROL(BTN_ADD, Button, _add);
    INIT_CONTROL(BTN_REMOVE, Button, _remove);

    INIT_CONTROL(BTN_KEYS, Button, _keys);
    INIT_CONTROL(CMB_ACTION, ComboBox, _action);
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();

    /* Make highlighted items span the entire row in the list view */
    _keyList.AddListExStyle(LVS_EX_FULLROWSELECT);

    RECT dims = _keyList.Dimensions();
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
}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();
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
    CLOG(L"Removing: %d", sel);
    _keyInfo.erase(_keyInfo.begin() + sel);
    _keyList.RemoveItem(sel);

    /* Select the item closest to the previous selection: */
    _keyList.Selection(sel);

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
}