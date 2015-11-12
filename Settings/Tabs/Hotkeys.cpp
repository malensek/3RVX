// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../../3RVX/HotkeyInfo.h"
#include "../../3RVX/HotkeyManager.h"
#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Logger.h"
#include "../../3RVX/Settings.h"
#include "../../3RVX/Skin/SkinInfo.h"

#include "HotkeyPrompt.h"
#include "KeyGrabber.h"
#include "../resource.h"

void Hotkeys::Initialize() {
    using std::placeholders::_1;

    _keyList = new ListView(LST_KEYS, *this);
    _keyList->OnItemChange = std::bind(&Hotkeys::OnKeyListItemChange, this, _1);
    _add = new Button(BTN_ADD, *this);
    _add->OnClick = std::bind(&Hotkeys::OnAddButtonClick, this);
    _remove = new Button(BTN_REMOVE, *this);
    _remove->OnClick = std::bind(&Hotkeys::OnRemoveButtonClick, this);

    _editorGroup = new GroupBox(GRP_EDITOR, *this);
    _keysLabel = new Label(LBL_KEYS, *this);
    _keys = new Button(BTN_KEYS, *this);
    _keys->OnClick = std::bind(&Hotkeys::OnKeysButtonClick, this);

    _actionLabel = new Label(LBL_ACTION, *this);
    _action = new ComboBox(CMB_ACTION, *this);
    _action->OnSelectionChange = std::bind(&Hotkeys::OnActionChange, this);

    _argLabel = new Label(LBL_ARG, *this);
    _argCheck = new Checkbox(CHK_ARG, *this);
    _argCheck->OnClick = std::bind(&Hotkeys::OnArgCheckChange, this);
    _argCombo = new ComboBox(CMB_ARG, *this);
    _argCombo->OnSelectionChange = std::bind(&Hotkeys::OnArgComboChange, this);
    _argEdit = new EditBox(ED_ARG, *this);
    _argEdit->OnTextChange = std::bind(&Hotkeys::OnArgEditTextChange, this);
    _argButton = new Button(BTN_ARG, *this);
    _argButton->OnClick = std::bind(&Hotkeys::OnArgButtonClick, this);
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
    _vkArgStr = _translator->Translate(_vkArgStr);

    _ejectActionStr = _translator->Translate(_ejectActionStr);
    _mediaActionStr = _translator->Translate(_mediaActionStr);
    _runActionStr = _translator->Translate(_runActionStr);
    _vkActionStr = _translator->Translate(_vkActionStr);

    /* Make highlighted items span the entire row in the list view */
    _keyList->AddListExStyle(LVS_EX_FULLROWSELECT);

    RECT dims = _keyList->ScreenDimensions();
    int width = dims.right - dims.left;

    _keyList->AddColumn(_hotkeysColumnStr, (int) (width * .485));
    _keyList->AddColumn(_actionColumnStr, (int) (width * .445));

    for (std::wstring action : HotkeyInfo::ActionNames) {
        _action->AddItem(_translator->Translate(action));
    }

    std::unordered_map<int, HotkeyInfo> hotkeys = settings->Hotkeys();
    for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it) {
        _keyInfo.push_back(it->second);
    }

    for (unsigned int i = 0; i < _keyInfo.size(); ++i) {
        HotkeyInfo hki = _keyInfo[i];
        std::wstring hkStr = HotkeyManager::HotkeysToString(hki.keyCombination);
        int idx = _keyList->AddItem(hkStr);
        LoadAction(idx, hki);
    }

    _keyList->Selection(0);
    LoadSelection();
}

void Hotkeys::SaveSettings() {
    if (DialogHandle() == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");


    Settings *settings = Settings::Instance();
    settings->Hotkeys(_keyInfo);
}

HotkeyInfo *Hotkeys::CurrentHotkeyInfo() {
    int selectionIdx = _keyList->Selection();
    if (selectionIdx == -1) {
        return NULL;
    }

    return &_keyInfo[selectionIdx];
}

void Hotkeys::LoadSelection() {
    int idx = _keyList->Selection();
    if (idx < 0) {
        _keys->Text(L"");
        _keys->Disable();
        _action->Select(-1);
        _action->Disable();
        DefaultArgControlStates();
        return;
    }
    LoadSelection(idx);
}

void Hotkeys::LoadSelection(int index) {
    HotkeyInfo selection = _keyInfo[index];
    _keys->Enable();
    _action->Enable();

    _keys->Text(L"");
    if (selection.keyCombination > 0) {
        std::wstring keyStr
            = HotkeyManager::HotkeysToString(selection.keyCombination);
        _keys->Text(keyStr);
        _keyList->ItemText(index, 0, keyStr);
    }

    /* Select the action in the combo box: */
    _action->Select(-1);
    int action = selection.action;

    /* Set argument controls to their default states: */
    DefaultArgControlStates();

    if (action >= 0) {
        _action->Select(action);
        LoadAction(index, selection);
    }
}

void Hotkeys::LoadAction(int index, HotkeyInfo &selection) {
    int action = selection.action;

    if (action < 0) {
        /* Selection has no action */
        return;
    }

    /* First, set the action description */
    _keyList->ItemText(index, 1, ActionString(selection));

    /* Default visiblities */
    bool showLabel = false;
    bool showCheck = false;
    bool showCombo = false;
    bool showEdit = false;
    bool showButton = false;

    switch ((HotkeyInfo::HotkeyActions) action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
        VolumeArgControlStates(selection);
        showCheck = true; showCombo = true; showEdit = true;
        break;

    case HotkeyInfo::SetVolume:
        VolumeArgControlStates(selection);
        showLabel = true; showCombo = true; showEdit = true;
        break;

    case HotkeyInfo::EjectDrive:
        _argLabel->Text(_driveArgStr);
        _argCombo->Width(_argCombo->EmSize() * 6);
        for (int i = 0; i < 26; ++i) {
            wchar_t ch = (wchar_t) i + 65;
            _argCombo->AddItem(std::wstring(1, ch));
        }

        if (selection.HasArgs()) {
            _argCombo->Select(selection.args[0]);
        }

        showLabel = true; showCombo = true;
        break;

    case HotkeyInfo::MediaKey:
        _argLabel->Text(_keyArgStr);
        for (std::wstring keys : HotkeyInfo::MediaKeyNames) {
            _argCombo->AddItem(_translator->Translate(keys));
        }

        if (selection.HasArgs()) {
            _argCombo->Select(_translator->Translate(selection.args[0]));
        }

        showLabel = true; showCombo = true;
        break;

    case HotkeyInfo::VirtualKey:
        _argLabel->Text(_vkArgStr);
        if (selection.HasArgs()) {
            _argEdit->Text(selection.args[0]);
        }
        showLabel = true; showEdit = true;
        break;

    case HotkeyInfo::Run:
        _argLabel->Text(_pathArgStr);
        _argButton->Text(L"…");
        _argButton->Width(_argButton->EmSize() * 3);
        _argEdit->PlaceRightOf(*_argLabel);
        _argEdit->X(_action->X());
        _argEdit->Width(_keys->Width() - _argButton->Width() - _argEdit->EmSize());
        _argButton->PlaceRightOf(*_argEdit);

        if (selection.HasArgs()) {
            _argEdit->Text(selection.args[0]);
        }

        showLabel = true; showEdit = true; showButton = true;
        break;
    }

    /* Update control visibility */
    _argLabel->Visible(showLabel);
    _argCheck->Visible(showCheck);
    _argCombo->Visible(showCombo);
    _argEdit->Visible(showEdit);
    _argButton->Visible(showButton);

    if (_argCheck->Visible() == false) {
        /* Controls are only disabled for optional arguments (check box).
         * If _argCheck isn't visible, make sure everything is enabled. */
        _argEdit->Enable();
        _argCombo->Enable();
        _argButton->Enable();
    }
}

std::wstring Hotkeys::ActionString(HotkeyInfo &selection) {
    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) selection.action;

    if (action < 0) {
        /* Selection has no action */
        return L"";
    }

    /* Set to the default string (action with no parameters) */
    std::wstring actionStr = _translator->Translate(
        HotkeyInfo::ActionNames[action]);

    if (selection.HasArgs() == false) {
        /* We're done */
        return actionStr;
    }

    if (selection.args[0] == L"") {
        /* Blank arg = no arg. */
        return actionStr;
    }

    switch ((HotkeyInfo::HotkeyActions) selection.action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
        actionStr = _translator->TranslateAndReplace(
            VolumeActionString(selection),
            selection.args[0]);
        break;

    case HotkeyInfo::EjectDrive:
        actionStr = _translator->Replace(_ejectActionStr, selection.args[0]);
        break;

    case HotkeyInfo::MediaKey:
        actionStr = _translator->Replace(_mediaActionStr,
            _translator->Translate(selection.args[0]));
        break;

    case HotkeyInfo::VirtualKey:
        actionStr = _translator->Replace(_vkActionStr, selection.args[0]);
        break;

    case HotkeyInfo::Run:
        actionStr = _translator->Replace(_runActionStr, selection.args[0]);
        break;
    }

    return actionStr;
}

std::wstring Hotkeys::VolumeActionString(HotkeyInfo &selection) {
    std::wstring actionStr;
    HotkeyInfo::VolumeKeyArgTypes type = HotkeyInfo::VolumeArgType(selection);

    switch ((HotkeyInfo::HotkeyActions) selection.action) {
    case HotkeyInfo::IncreaseVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            actionStr = L"Increase Volume {1}%";
        } else {
            actionStr = L"Increase Volume {1} units";
        }
        break;

    case HotkeyInfo::DecreaseVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            actionStr = L"Decrease Volume {1}%";
        } else {
            actionStr = L"Decrease Volume {1} units";
        }
        break;

    case HotkeyInfo::SetVolume:
        if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            actionStr = L"Set Volume: {1}%";
        } else {
            actionStr = L"Set Volume: {1} units";
        }
        break;
    }

    return actionStr;
}

void Hotkeys::DefaultArgControlStates() {
    CLOG(L"Setting default control states");
    _argLabel->Visible(false);
    _argCheck->Visible(false);
    _argCombo->Visible(false);
    _argEdit->Visible(false);
    _argButton->Visible(false);

    _argCombo->Enabled(false);
    _argEdit->Enabled(false);
    _argButton->Enabled(false);

    _argLabel->Text(L"");
    _argCheck->Text(L"");
    _argCheck->Checked(false);
    _argCombo->Clear();
    _argCombo->Width(_action->Width());
    _argCombo->PlaceRightOf(*_argLabel);
    _argCombo->X(_action->X());
    _argEdit->Clear();
    _argEdit->Width(_action->Width());
    _argEdit->PlaceRightOf(*_argLabel);
    _argEdit->X(_action->X());
}

void Hotkeys::VolumeArgControlStates(HotkeyInfo &selection) {
    _argLabel->Text(_amountVolArgStr);
    _argCheck->Text(_amountVolArgStr);
    _argCheck->Checked(selection.HasArgs());
    _argEdit->Enabled(_argCheck->Checked());
    _argEdit->Width(_argEdit->EmSize() * 6);
    _argEdit->PlaceRightOf(*_argCheck);
    _argEdit->X(_action->X());
    _argCombo->Enabled(_argCheck->Checked());
    _argCombo->AddItem(_unitsVolArgStr);
    _argCombo->AddItem(_percentVolArgStr);
    _argCombo->Select(0);
    _argCombo->PlaceRightOf(*_argEdit);
    _argCombo->Width(_action->Width() - (_argCombo->X() - _action->X()));

    if (selection.HasArg(0)) {
        _argEdit->Text(selection.args[0]);
    }
    if (selection.HasArg(1)) {
        _argCombo->Select(selection.ArgToInt(1));
    }
}

std::wstring Hotkeys::OpenFileDialog() {
    wchar_t fileName[1024] = L" ";

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = DialogHandle();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 1024;
    ofn.Flags = OFN_HIDEREADONLY | OFN_READONLY;

    BOOL result = GetOpenFileName(&ofn);
    if (result == FALSE) {
        /* User clicked cancel */
        return L"";
    }

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
            int index = lv->iItem;
#if ENABLE_3RVX_LOG != 0
            HotkeyInfo *current = &_keyInfo[index];
            CLOG(L"Selecting key combination %d:", index);
            QCLOG(L"%s", current->ToString().c_str());
#endif
            LoadSelection(index);
        }
    }
}

bool Hotkeys::OnAddButtonClick() {
    int items = _keyList->Count();
    for (int i = 0; i < items; ++i) {
        if (_keyList->ItemText(i, 0) == L"" && _keyList->ItemText(i, 1) == L"") {
            /* We found a blank item already in the list */
            _keyList->Selection(i);
            return true;
        }
    }

    HotkeyInfo hi;
    _keyInfo.push_back(hi);
    int idx = _keyList->InsertItem(items, L"");
    _keyList->ItemText(idx, 1, L"");
    _keyList->Selection(idx);
    return true;
}

bool Hotkeys::OnRemoveButtonClick() {
    int sel = _keyList->Selection();
    if (sel < 0) {
        return false;
    }

    _keyInfo.erase(_keyInfo.begin() + sel);
    _keyList->RemoveItem(sel);

    /* Select the item closest to the previous selection: */
    _keyList->Selection(sel);

    /* This handles disabling the other controls if the last item was removed:*/
    if (_keyList->Count() <= 0) {
        LoadSelection();
    }

    return true;
}

bool Hotkeys::OnKeysButtonClick() {
    HotkeyPrompt::Show(DialogHandle());
    KeyGrabber::Instance()->Unhook();
    int keyCombo = KeyGrabber::Instance()->KeyCombination();
    if (keyCombo > 0) {
        std::wstring keyStr = HotkeyManager::HotkeysToString(keyCombo);
        _keys->Text(keyStr);
        int sel = _keyList->Selection();
        _keyInfo[sel].keyCombination = keyCombo;
        LoadSelection(sel);
    }
    return true;
}

bool Hotkeys::OnActionChange() {
    int actionIdx = _action->SelectionIndex();
    int selectionIdx = _keyList->Selection();
    HotkeyInfo &currentListSelection = _keyInfo[selectionIdx];
    if (currentListSelection.action != actionIdx) {
        currentListSelection.args.clear();
        currentListSelection.action = (HotkeyInfo::HotkeyActions) actionIdx;
        LoadSelection(selectionIdx);
    }
    return true;
}

bool Hotkeys::OnArgButtonClick() {
    int selectionIdx = _keyList->Selection();
    if (selectionIdx == -1) {
        return false;
    }

    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action->SelectionIndex();

    switch (action) {
    case HotkeyInfo::Run: 
        std::wstring fName = OpenFileDialog();
        if (fName != L"") {
            _argEdit->Text(fName);
        }
        break;
    }

    return true;
}

bool Hotkeys::OnArgComboChange() {
    int selectionIdx = _keyList->Selection();
    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action->SelectionIndex();

    switch (action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
        current->AllocateArg(1);
        current->args[1] = std::to_wstring(_argCombo->SelectionIndex());
        break;

    case HotkeyInfo::EjectDrive:
        current->AllocateArg(0);
        /* We can place the selected string directly into the args */
        current->args[0] = _argCombo->Selection();
        break;

    case HotkeyInfo::MediaKey:
        current->AllocateArg(0);
        current->args[0]
            = HotkeyInfo::MediaKeyNames[_argCombo->SelectionIndex()];
        break;
    }

    LoadSelection(selectionIdx);
    return true;
}

bool Hotkeys::OnArgCheckChange() {
    int selectionIdx = _keyList->Selection();
    if (selectionIdx == -1) {
        return false;
    }

    HotkeyInfo *current = &_keyInfo[selectionIdx];

    HotkeyInfo::HotkeyActions action
        = (HotkeyInfo::HotkeyActions) _action->SelectionIndex();

    switch (action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
        if (_argCheck->Checked() == false) {
            _argEdit->Clear();
            current->args.clear();
            _keyList->ItemText(selectionIdx, 1, ActionString(*current));
        }
        break;
    }

    _argCombo->Enabled(_argCheck->Checked());
    _argEdit->Enabled(_argCheck->Checked());
    _argButton->Enabled(_argCheck->Checked());
    return true;
}

bool Hotkeys::OnArgEditTextChange() {
    if (_argEdit->Enabled() == false || _argEdit->Visible() == false) {
        return FALSE;
    }

    int currentIndex = _keyList->Selection();
    HotkeyInfo *current = CurrentHotkeyInfo();
    if (current == NULL) {
        return FALSE;
    }

    switch ((HotkeyInfo::HotkeyActions) current->action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
    case HotkeyInfo::SetVolume:
    case HotkeyInfo::Run:
        current->AllocateArg(0);
        current->args[0] = _argEdit->Text();
        _keyList->ItemText(currentIndex, 1, ActionString(*current));
        break;
    }

    return TRUE;
}
