#pragma once

#include "Control.h"

class Label : public Control {
public:
    Label() {

    }

    Label(int id, HWND parent) :
    Control(id, parent) {

    }
};