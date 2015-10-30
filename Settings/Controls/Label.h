#pragma once

#include "Control.h"
#include "../../3RVX/Logger.h"

class Label : public Control {
public:
    Label(int id, DialogBase &parent, bool translate = true) :
    Control(id, parent, translate) {

    }
};