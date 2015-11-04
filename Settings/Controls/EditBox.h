// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Control.h"

class EditBox : public Control {
public:
    EditBox(int id, DialogBase &parent) :
    Control(id, parent, false) {

    }

    void Clear();

    virtual BOOL CALLBACK Command(unsigned short nCode);

public:
    /* Event Handlers */
    std::function<bool()> OnTextChange;
};