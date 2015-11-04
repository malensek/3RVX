// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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