#pragma once
#include <Windows.h>

#include <string>

class UIContext {
public:
    UIContext(HWND hWnd);

    bool GetCheck(int chkId);
    bool SetCheck(int chkId, bool checked = true);

    void AddComboItem(int cmbId, std::wstring item);
    int SelectComboItem(int cmbId, std::wstring item);
    std::wstring GetComboSelection(int cmbId);
    bool SetText(int id, std::wstring text);

    void Enable(int id);
    void Disable(int id);
    void SetEnabled(int id, bool enabled);

private:
    HWND _hWnd;
};