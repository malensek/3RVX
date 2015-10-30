#pragma once

#include "Control.h"

class ListBox : public Control {
public:
    ListBox(int id, DialogBase &parent) :
    Control(id, parent, false) {

    }

    int AddItem(std::wstring item);
    int InsertItem(int index, std::wstring item);

    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnSelectionChange;
};