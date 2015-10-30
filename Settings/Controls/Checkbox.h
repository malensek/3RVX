#pragma once

#include "Button.h"

class Checkbox : public Button {
public:
    Checkbox(int id, DialogBase &parent, bool translate = true) :
    Button(id, parent, translate) {

    }

    bool Checked();
    bool Checked(bool checked);
};