#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../../3RVX/HotkeyInfo.h"
#include "../../3RVX/HotkeyManager.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../../3RVX/SkinInfo.h"

#include "HotkeyPrompt.h"
#include "KeyGrabber.h"
#include "../resource.h"

void Hotkeys::Initialize() {
    using std::placeholders::_1;

    INIT_CONTROL(LST_KEYS, ListView, _keyList);
    _keyList.OnItemChange = std::bind(&Hotkeys::OnKeyListItemChange, this, _1);
    INIT_CONTROL(BTN_ADD, Button, _add);
    _add.OnClick = std::bind(&Hotkeys::OnAddButtonClick, this);
    INIT_CONTROL(BTN_REMOVE, Button, _remove);
    _remove.OnClick = std::bind(&Hotkeys::OnRemoveButtonClick, this);

    INIT_CONTROL(GRP_EDITOR, GroupBox, _editorGroup);
    INIT_CONTROL(LBL_KEYS, Label, _keysLabel);
    INIT_CONTROL(BTN_KEYS, Button, _keys);
    _keys.OnClick = std::bind(&Hotkeys::OnKeysButtonClick, this);

    INIT_CONTROL(LBL_ACTION, Label, _actionLabel);
    INIT_CONTROL(CMB_ACTION, ComboBox, _action);
    _action.OnSelectionChange = std::bind(&Hotkeys::OnActionChange, this);

    INIT_CONTROL(LBL_ARG, Label, _argLabel);
    INIT_CONTROL(CHK_ARG, Checkbox, _argCheck);
    _argCheck.OnClick = std::bind(&Hotkeys::OnArgCheckChange, this);
    INIT_CONTROL(CMB_ARG, ComboBox, _argCombo);
    _argCombo.OnSelectionChange = std::bind(&Hotkeys::OnArgComboChange, this);
    INIT_CONTROL(ED_ARG, EditBox, _argEdit);
    _argEdit.OnTextChange = std::bind(&Hotkeys::OnArgEditTextChange, this);
    INIT_CONTROL(BTN_ARG, Button, _argButton);
    _argButton.OnClick = std::bind(&Hotkeys::OnArgButtonClick, this);
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();
    _translator = settings->Translator();

    /* Translations */
    _hotkeysColumnStr = _translator->Translate(_hotkeysColumnStr);
    _actionColumnStr = _translator->Translate(_actionColumnStr);
    _amountVolArgStr = _translator->Translate(_amountVolArgStr);
    _unitsVolArgStr = _translator->Translate(_unitsVolArgStr);
    _percentVolArgStr = _translator->Translate(_percentVolArgStr);
    _keyArgStr = _translator->Translate(_keyArgStr);
    _driveArgStr = _translator->Translate(_driveArgStr);
    _pathArgStr = _translator->Translate(_pathArgStr);

    /* Make highlighted items span the entire row in the list view */
    _keyList.AddListExStyle(LVS_EX_FULLROWSELECT);

    RECT dims = _keyList.ScreenDimensions();
    int width = dims.right - dims.left;

    _keyList.AddColumn(_hotkeysColumnStr, (int) (width * .485));
    _keyList.AddColumn(_actionColumnStr, (int) (width * .445));

    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action.AddItem(_translator->Translate(action));
    }

    std::unordered_map<int, HotkeyInfo> hotkeys = settings->Hotkeys();
    for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it) {
        _keyInfo.push_back(it->second);
    }

    for (unsigned int i = 0; i < _keyInfo.size(); ++i) {
        HotkeyInfo hki = _keyInfo[i];
        std::wstring hkStr = HotkeyManager::HotkeysToString(hki.keyCombination);
        int idx = _keyList.AddItem(hkStr);
        LoadAction(idx, hki);
    }

    _keyList.Selection(0);
    LoadSelection();
}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    /* If the edit box was the last thing the user touched, then we need to
     * manually sync the changes here. */
    UpdateEditArgument();

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();
    settings->Hotkeys(_keyInfo);
}

HotkeyInfo *Hotkeys::CurrentHotkeyInfo() {
    int selectionIdx = _keyList.Selection();
    if (selectionIdx == -1) {
        return NULL;
    }

    return &_keyInfo[selectionIdx];
}

void Hotkeys::LoadSelection() {
    int idx = _keyList.Selection();
    if (idx < 0) {
        _keys.Text(L"");
        _keys.Disable();
        _action.Select(-1);
        _action.Disable();
        DefaultArgControlStates();
        return;
    }
    LoadSelection(idx);
}

void Hotkeys::LoadSelection(int index) {
    HotkeyInfo selection = _keyInfo[index];
    _keys.Enable();
    _action.Enable();

    _keys.Text(L"");
    if (selection.keyCombination > 0) {
        std::wstring keyStr
            = HotkeyManager::HotkeysToString(selection.keyCombination);
        _keys.Text(keyStr);
        _keyList.ItemText(index, 0, keyStr);
    }

    /* Select the action in the combo box: */
    _action.Select(-1);
    int action = selection.action;

    /* Set argument controls to their default states: */
    DefaultArgControlStates();

    if (action >= 0) {
        _action.Select(action);
        LoadAction(index, selection);
    }
}

void Hotkeys::LoadAction(int index, HotkeyInfo &selection) {
    int action = selection.action;

    if (action < 0) {
        /* Selection has no action */
        return;
    }

    /* Set to the default string (action with no parameters) */
    std::wstring actionStr = _translator->Translate(
        HotkeyInfo::ActionNames[action]);

    /* Default visiblities */
    bool showLabel = false;
    bool showCheck = false;
    bool showCombo = false;
    bool showEdit = false;
    bool showButton = false;

    switch ((HotkeyInfo::HotkeyActions) action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume: {
        if (selection.HasArg(0)) {
            std::wstring arg0 = selection.args[0];
            actionStr = _translator->TranslateAndReplace(
                VolumeActionString(selection), arg0);
        }

        VolumeArgControlStates(selection);
        showCheck = true; showCombo = true; showEdit = true;

        if (action == HotkeyInfo::SetVolume) {
            /* Special case; no checkbox */
            _argLabel.Text(_amountVolArgStr);
            showLabel = true; showCheck = false;
        }
        break;
    }

    case HotkeyInfo::EjectDrive: {
        actionStr = _translator->TranslateAndReplace(
            L"Eject Drive: {1}",
            selection.args[0]);

        _argLabel.Text(_driveArgStr);
        _argCombo.Width(_argCombo.EmSize() * 6);
        for (int i = 0; i < 26; ++i) {
            wchar_t ch = (wchar_t) i + 65;
            _argCombo.AddItem(std::wstring(1, ch));
        }

        if (selection.HasArgs()) {
            _argCombo.Select(selection.args[0]);
        }

        showLabel = true; showCombo = true;
        break;
    }

    case HotkeyInfo::MediaKey: {
        actionStr = _translator->TranslateAndReplace(
            L"Media Key: {1}",
            _translator->Translate(selection.args[0]));

        _argLabel.Text(_keyArgStr);
        for (std::wstring keys : HotkeyInfo::MediaKeyNames) {
            _argCombo.AddItem(_translator->Translate(keys));
        }

        if (selection.HasArgs()) {
            _argCombo.Select(_translator->Translate(selection.args[0]));
        }

        showLabel = true; showCombo = true;
        break;
    }

    case HotkeyInfo::Run: {
        actionStr = _translator->TranslateAndReplace(
            L"Run: {1}",
            selection.args[0]);

        _argLabel.Text(_pathArgStr);
        _argButton.Text(L"…");
        _argButton.Width(_argButton.EmSize() * 3);
        _argEdit.PlaceRightOf(_argLabel);
        _argEdit.X(_action.X());
        _argEdit.Width(_keys.Width() - _argButton.Width() - _argEdit.EmSize());
        _argButton.PlaceRightOf(_argEdit);
        if (selection.HasArgs()) {
            _argEdit.Text(selection.args[0]);
        }

        showLabel = true; showEdit = true; showButton = true;
        break;
    }
    }

    /* First, set the action description */
    _keyList.ItemText(index, 1, actionStr);

    /* Update control visibility */
    _argLabel.Visible(showLabel);
    _argCheck.Visible(showCheck);
    _argCombo.Visible(showCombo);
    _argEdit.Visible(showEdit);
    _argButton.Visible(showButton);

    if (_argCheck.Visible() == false) {
        /* Controls are only disabled for optional arguments (check box).
         * If _argCheck isn't visible, make sure everything is enabled. */
        _argEdit.Enable();
        _argCombo.Enable();
        _argButton.Enable();
    }
}

std::wstring Hotkeys::VolumeActionString(HotkeyInfo &selection) {
    int action = selection.action;
    HotkeyInfo::VolumeKeyArgTypes type = HotkeyInfo::VolumeArgType(selection);
    if (type < 0) {
        CLOG(L"ERROR: No args set!");
        return L"";
    }

    std::wstring itemStr = HotkeyInfo::ActionNames[action];

    if (selection.args[0] == L"") {
        return itemStr;
    }

    switch ((HotkeyInfo::HotkeyActions) action) {
    case HotkeyInfo::IncreaseVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            itemStr = L"Increase Volume {1}%";
        } else {
            itemStr = L"Increase Volume {1} units";
        }
        break;

    case HotkeyInfo::DecreaseVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            itemStr = L"Decrease Volume {1}%";
        } else {
            itemStr = L"Decrease Volume {1} units";
        }
        break;

    case HotkeyInfo::SetVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            itemStr = L"Set Volume: {1}%";
        } else {
            itemStr = L"Set Volume: {1} units";
        }
        break;
    }

    return itemStr;
}

void Hotkeys::UpdateEditArgument() {
    HotkeyInfo *current;
    try {
        current = &_keyInfo.at(_listSelection);
    } catch (std::out_of_range e) {
        return;
    }

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action.SelectionIndex();

    switch (action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
        if (_argEdit.Text() != L"") {
            current->AllocateArg(0);
            current->args[0] = _argEdit.Text();
        }
        break;

    case HotkeyInfo::Run:
        if (_argEdit.Text() != L"") {
            current->AllocateArg(0);
            current->args[0] = _argEdit.Text();
        }
        break;
    }

    /* Refresh the action display with the value that was entered */
    LoadAction(_listSelection, *current);
}

void Hotkeys::DefaultArgControlStates() {
    _argLabel.Visible(false);
    _argCheck.Visible(false);
    _argCombo.Visible(false);
    _argEdit.Visible(false);

    _argCombo.Enabled(false);
    _argEdit.Enabled(false);
    _argButton.Enabled(false);

    _argLabel.Text(L"");
    _argCheck.Text(L"");
    _argCheck.Checked(false);
    _argCombo.Clear();
    _argCombo.Width(_action.Width());
    _argCombo.PlaceRightOf(_argLabel);
    _argCombo.X(_action.X());
    _argEdit.Clear();
    _argEdit.Width(_action.Width());
    _argEdit.PlaceRightOf(_argLabel);
    _argEdit.X(_action.X());
}

void Hotkeys::VolumeArgControlStates(HotkeyInfo &selection) {
    _argCheck.Text(_amountVolArgStr);
    _argCheck.Checked(selection.HasArgs());
    _argEdit.Enabled(_argCheck.Checked());
    _argEdit.Width(_argEdit.EmSize() * 6);
    _argEdit.PlaceRightOf(_argCheck);
    _argEdit.X(_action.X());
    _argCombo.Enabled(_argCheck.Checked());
    _argCombo.AddItem(_unitsVolArgStr);
    _argCombo.AddItem(_percentVolArgStr);
    _argCombo.Select(0);
    _argCombo.PlaceRightOf(_argEdit);
    _argCombo.Width(_action.Width() - (_argCombo.X() - _action.X()));

    if (selection.HasArg(0)) {
        _argEdit.Text(selection.args[0]);
    }
    if (selection.HasArg(1)) {
        _argCombo.Select(selection.ArgToInt(1));
    }
}

std::wstring Hotkeys::OpenFileDialog() {
    wchar_t fileName[1024] = L" ";

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = _hWnd;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 1024;
    ofn.Flags = OFN_HIDEREADONLY | OFN_READONLY;

    GetOpenFileName(&ofn);

    std::wstring fNameStr(fileName);
    if (fNameStr.back() == L' ') {
        fNameStr.back() = L'\0';
    }

    return fNameStr;
}



/// --------------
/// Event Handlers
/// --------------

void Hotkeys::OnKeyListItemChange(NMLISTVIEW *lv) {
    if (lv->uChanged & LVIF_STATE) {
        if (lv->uNewState & LVIS_SELECTED) {
            OnKeyListSelectionChange(lv->iItem);
        }
    }
}

void Hotkeys::OnKeyListSelectionChange(int index) {
    if (_listSelection != -1) {
        /* Update the edit control of the previously-selected item. This avoids
         * constantly updating the key combination state as the user types. */
        UpdateEditArgument();
    }
    /* Update with the new index */
    _listSelection = index;

#if ENABLE_3RVX_LOG != 0
    HotkeyInfo *current = &_keyInfo[index];
    CLOG(L"Selecting key combination %d:", index);
    QCLOG(L"%s", current->ToString().c_str());
#endif

    LoadSelection(index);
}

bool Hotkeys::OnAddButtonClick() {
    int items = _keyList.Count();
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
    if (_keyList.Count() <= 0) {
        LoadSelection();
    }

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

bool Hotkeys::OnActionChange() {
    int actionIdx = _action.SelectionIndex();
    int selectionIdx = _keyList.Selection();
    HotkeyInfo &currentListSelection = _keyInfo[selectionIdx];
    if (currentListSelection.action != actionIdx) {
        currentListSelection.args.clear();
        currentListSelection.action = (HotkeyInfo::HotkeyActions) actionIdx;
        LoadSelection(selectionIdx);
    }
    return true;
}

bool Hotkeys::OnArgButtonClick() {
    int selectionIdx = _keyList.Selection();
    if (selectionIdx == -1) {
        return false;
    }

    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action.SelectionIndex();

    switch (action) {
    case HotkeyInfo::Run: 
        std::wstring fName = OpenFileDialog();
        _argEdit.Text(fName);
        break;
    }

    return true;
}

bool Hotkeys::OnArgComboChange() {
    int selectionIdx = _keyList.Selection();
    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action.SelectionIndex();

    switch (action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
        UpdateEditArgument();
        current->AllocateArg(1);
        current->args[1] = std::to_wstring(_argCombo.SelectionIndex());
        break;

    case HotkeyInfo::EjectDrive:
        current->AllocateArg(0);
        /* We can place the selected string directly into the args */
        current->args[0] = _argCombo.Selection();
        break;

    case HotkeyInfo::MediaKey:
        current->AllocateArg(0);
        current->args[0]
            = HotkeyInfo::MediaKeyNames[_argCombo.SelectionIndex()];
        break;
    }

    LoadSelection(selectionIdx);
    return true;
}

bool Hotkeys::OnArgCheckChange() {
    int selectionIdx = _keyList.Selection();
    if (selectionIdx == -1) {
        return false;
    }

    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action.SelectionIndex();

    switch (action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
        if (_argCheck.Checked() == false) {
            _argEdit.Clear();
            current->args.clear();
        }
        break;
    }

    _argCombo.Enabled(_argCheck.Checked());
    _argEdit.Enabled(_argCheck.Checked());
    _argButton.Enabled(_argCheck.Checked());
    return true;
}

bool Hotkeys::OnArgEditTextChange() {
    HotkeyInfo *current = CurrentHotkeyInfo();
    if (current == NULL) {
        return FALSE;
    }

}
