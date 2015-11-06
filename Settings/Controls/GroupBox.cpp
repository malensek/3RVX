#include "GroupBox.h"

void GroupBox::AddChild(Control *control) {
    _controls.push_back(control);
}

void GroupBox::AddChildren(std::initializer_list<Control*> controls) {
    for (Control *ctrl : controls) {
        AddChild(ctrl);
    }
}

void GroupBox::Visible(bool visible) {
    for (Control *ctrl : _controls) {
        ctrl->Visible(visible);
    }
    Control::Visible(visible);
}
