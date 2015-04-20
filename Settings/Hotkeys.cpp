#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../3RVX/HotkeyInfo.h"
#include "../3RVX/HotkeyManager.h"
#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "resource.h"

void Hotkeys::Initialize() {
    INIT_CONTROL(LST_KEYS, ListView, _keyList);
    INIT_CONTROL(BTN_ADD, Button, _add);
    INIT_CONTROL(BTN_REMOVE, Button, _remove);

    INIT_CONTROL(BTN_KEYS, Button, _keys);
    INIT_CONTROL(CMB_ACTION, ComboBox, _action);
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();

    /* Make highlighted items span the entire row in the list view */
    _keyList.AddWindowExStyle(LVS_EX_FULLROWSELECT);

    RECT dims = _keyList.Dimensions();
    int width = dims.right - dims.left;

    _keyList.AddColumn(L"Hotkeys", (int) (width * .485));
    _keyList.AddColumn(L"Action", (int) (width * .445));

    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action.AddItem(action);
    }

    std::unordered_map<int, HotkeyInfo> hotkeys = settings->Hotkeys();
    for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it) {
        _keyInfo.push_back(it->second);
    }

    for (unsigned int i = 0; i < _keyInfo.size(); ++i) {
        //LoadSelection(i);
        HotkeyInfo hki = _keyInfo[i];
        std::wstring hkStr = HotkeyManager::HotkeysToString(hki.keyCombination);
        int idx = _keyList.AddItem(hkStr);
        _keyList.ItemText(idx, 1, HotkeyInfo::ActionNames[hki.action]);
    }

//    SelectItem(0);


}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();
}

