// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Control.h"

class Button : public Control {
public:
    Button(int id, DialogBase &parent, bool translate = true) :
    Control(id, parent, translate) {

    }

    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnClick;

};