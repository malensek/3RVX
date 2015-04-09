#pragma once
#include <Windows.h>

#include <string>

class UIContext {
public:
    UIContext(HWND hWnd);

    void Enable(int id);
    void Disable(int id);
    void SetEnabled(int id, bool enabled);

    bool GetCheck(int chkId);
    bool SetCheck(int chkId, bool checked = true);

    void AddComboItem(int cmbId, std::wstring item);
    int SelectComboItem(int cmbId, std::wstring item);
    void SelectComboItem(int cmbId, int itemIdx);
    std::wstring GetComboSelection(int cmbId);
    int GetComboSelectionIndex(int cmbId);

    void SetSpinRange(int spId, int lo, int hi);

    bool SetText(int id, std::wstring text);
    bool SetText(int id, int value);
    std::wstring GetText(int id);
    int GetTextAsInt(int id);

private:
    HWND _hWnd;
};