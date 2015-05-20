#pragma once

#include "Control.h"
#include "../../3RVX/Logger.h"

class Label : public Control {
public:
    Label() {

    }

    Label(int id, HWND parent) :
    Control(id, parent) {

    }
};