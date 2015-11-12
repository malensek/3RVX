#include "GroupBox.h"

void GroupBox::AddChild(Control *control) {
    _controls.push_back(control);
}

void GroupBox::AddChildren(std::initializer_list<Control*> controls) {
    for (Control *ctrl : controls) {
        AddChild(ctrl);
    }
}

void GroupBox::Enable() {
    for (Control *ctrl : _controls) {
        ctrl->Enable();
    }
}

void GroupBox::Disable() {
    for (Control *ctrl : _controls) {
        ctrl->Disable();
    }
}

void GroupBox::Visible(bool visible) {
    for (Control *ctrl : _controls) {
        ctrl->Visible(visible);
    }
    Control::Visible(visible);
}

int GroupBox::X() {
    return Control::X();
}

int GroupBox::Y() {
    return Control::Y();
}

void GroupBox::X(int x) {
    for (Control *ctrl : _controls) {
        int rel = ctrl->X() - Control::X();
        ctrl->X(x + rel);
    }
    Control::X(x);
}

void GroupBox::Y(int y) {
    for (Control *ctrl : _controls) {
        int rel = ctrl->Y() - Control::Y();
        ctrl->Y(y + rel);
    }
    Control::Y(y);
}
