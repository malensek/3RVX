// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <initializer_list>

#include "Control.h"

class GroupBox : public Control {
public:
    GroupBox(int id, DialogBase &parent, bool translate = true) :
    Control(id, parent, translate) {

    }

    void AddChild(Control *control);
    void AddChildren(std::initializer_list<Control *> controls);

    virtual void Enable();
    virtual void Disable();

    virtual void Visible(bool visible);

    virtual int X();
    virtual int Y();
    virtual void X(int x);
    virtual void Y(int y);
 
private:
    std::vector<Control *> _controls;
};