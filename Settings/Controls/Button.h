#pragma once

#include "Control.h"

class Button : public Control {
public:
    Button() {

    }

    Button(int id, HWND parent, bool translate = true) :
    Control(id, parent, translate) {

    }

    Button(int id, Dialog &parent, bool translate = true) :
    Control(id, parent, translate) {

    }

    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnClick;

};