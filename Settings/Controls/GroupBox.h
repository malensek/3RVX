#pragma once

#include "Control.h"

class GroupBox : public Control {
public:
    GroupBox() {

    }

    GroupBox(int id, HWND parent) :
    Control(id, parent) {

    }
};