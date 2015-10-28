#pragma once

#include "Button.h"

class Checkbox : public Button {
public:
    Checkbox() {

    }

    Checkbox(int id, HWND parent) :
    Button(id, parent) {

    }

    Checkbox(int id, DialogBase &parent, bool translate = true) :
    Button(id, parent, translate) {

    }


    bool Checked();
    bool Checked(bool checked);

};