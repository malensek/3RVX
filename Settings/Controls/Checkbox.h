#pragma once

#include "Button.h"

class Checkbox : public Button {
public:
    Checkbox() {

    }

    Checkbox(int id, HWND parent) :
    Button(id, parent) {

    }

    bool Checked();
    bool Checked(bool checked);

};