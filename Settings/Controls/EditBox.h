#pragma once

#include "Control.h"

class EditBox : public Control {
public:
    EditBox() {

    }

    EditBox(int id, HWND parent) :
    Control(id, parent) {

    }

    void Clear();

    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnTextChange;
};