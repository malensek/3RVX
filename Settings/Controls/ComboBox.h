#pragma once

#include "Control.h"

class ComboBox : public Control {
public:
    ComboBox() {

    }

    ComboBox(int id, HWND parent) :
    Control(id, parent) {

    }

    void AddItem(std::wstring item);
    std::wstring Selection();
    int SelectionIndex();
    int Select(std::wstring item);
    void Select(int itemIdx);

    virtual DLGPROC Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnSelectionChange;

};