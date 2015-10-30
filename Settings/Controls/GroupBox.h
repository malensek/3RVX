#pragma once

#include "Control.h"

class GroupBox : public Control {
public:
    GroupBox(int id, DialogBase &parent, bool translate = true) :
    Control(id, parent, translate) {

    }
};