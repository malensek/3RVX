#pragma once

#include "Tab.h"

#include "../3RVX/HotkeyInfo.h"

class Hotkeys : public Tab {
public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    std::vector<HotkeyInfo> _keyInfo;

    void LoadSelection();
    void LoadSelection(int index);
    void LoadActionParameters(int action, HotkeyInfo &selection);

private:
    void OnKeyListItemChange(NMLISTVIEW *lv);
    void OnKeyListSelectionChange(int index);

    bool OnAddButtonClick();
    bool OnRemoveButtonClick();

    bool OnKeysButtonClick();
    bool OnActionChange();
    bool OnArgComboChange();
    bool OnArgCheckChange();

private:
    /* Controls: */
    ListView _keyList;
    Button _add;
    Button _remove;

    Button _keys;
    ComboBox _action;

    Label _argLabel;
    Checkbox _argCheck;
    ComboBox _argCombo;
    EditBox _argEdit;

private:
    /* Strings: */
    std::wstring hotkeysColumnStr = L"Hotkeys";
    std::wstring actionColumnStr = L"Action";
};