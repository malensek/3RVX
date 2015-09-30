#pragma once

#include <vector>

#include "Control.h"

class ComboBox : public Control {
public:
    ComboBox() {

    }

    ComboBox(int id, HWND parent) :
    Control(id, parent) {

    }

    void AddItem(std::wstring item);
    std::vector<std::wstring> Items();

    void Clear();
    int Count();

    std::wstring Selection();
    int SelectionIndex();
    int Select(std::wstring item);
    int SelectPrefix(std::wstring item);
    void Select(int itemIdx);


    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnSelectionChange;

};