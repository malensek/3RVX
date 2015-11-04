// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Control.h"

class GroupBox : public Control {
public:
    GroupBox(int id, DialogBase &parent, bool translate = true) :
    Control(id, parent, translate) {

    }
};