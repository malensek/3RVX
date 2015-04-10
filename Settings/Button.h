#pragma once

#include "Control.h"

class Button : public Control {
public:
    Button() {

    }

    Button(int id, HWND parent) :
    Control(id, parent) {

    }

};