#pragma once

#include "Tab.h"

#include "../../3RVX/HotkeyInfo.h"
#include "../HotkeySettings/HotkeySettings.h"

class Hotkeys : public Tab {

friend class HotkeySettings;

public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

protected:
    int _listSelection = -1;
    HotkeySettings *_currentHkSettings;
    std::vector<HotkeyInfo> _keyInfo;

    void LoadSelection();
    void LoadSelection(int index);
    void LoadAction(int index, HotkeyInfo &selection);
    void LoadActionParameters(HotkeyInfo &selection);
    std::wstring VolumeActionString(HotkeyInfo &selection);

    void DefaultArgControlStates();
    void UpdateArgControlStates(
        bool label = false,
        bool check = false,
        bool combo = false,
        bool edit = false,
        bool button = false);
    void VolumeArgControlStates(HotkeyInfo &selection);
    void UpdateEditArgument();
    std::wstring OpenFileDialog();

private:
    /* Control Event Handlers */
    void OnKeyListItemChange(NMLISTVIEW *lv);
    void OnKeyListSelectionChange(int index);

    bool OnAddButtonClick();
    bool OnRemoveButtonClick();

    bool OnKeysButtonClick();
    bool OnActionChange();
    bool OnArgComboChange();
    bool OnArgCheckChange();
    bool OnArgButtonClick();

protected:
    /* Controls: */
    ListView _keyList;
    Button _add;
    Button _remove;

    GroupBox _editorGroup;
    Label _keysLabel;
    Button _keys;
    Label _actionLabel;
    ComboBox _action;

    Label _argLabel;
    Checkbox _argCheck;
    ComboBox _argCombo;
    EditBox _argEdit;
    Button _argButton;

protected:
    /* Strings: */
    std::wstring _hotkeysColumnStr = L"Hotkeys";
    std::wstring _actionColumnStr = L"Action";
    std::wstring _amountVolArgStr = L"Amount:";
    std::wstring _unitsVolArgStr = L"Volume Units";
    std::wstring _percentVolArgStr = L"Percent";
    std::wstring _keyArgStr = L"Key:";
    std::wstring _driveArgStr = L"Drive:";
    std::wstring _pathArgStr = L"Path:";
};