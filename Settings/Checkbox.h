#pragma once

#include "Control.h"

class Checkbox : public Control {
public:
    Checkbox() {

    }

    Checkbox(int id, HWND parent) :
    Control(id, parent) {

    }

    bool Checked();
    bool Checked(bool checked);

};