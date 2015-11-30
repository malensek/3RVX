// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Control.h"
#include "../../3RVX/Logger.h"

class Label : public Control {
public:
    Label(int id, Dialog &parent, bool translate = true) :
    Control(id, parent, translate) {

    }
};