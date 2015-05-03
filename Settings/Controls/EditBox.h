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
};