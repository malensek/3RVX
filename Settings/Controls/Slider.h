#pragma once

#include "Control.h"

class Slider : public Control {
public:
    Slider(int id, DialogBase &parent) :
    Control(id, parent, false) {

    }

    virtual BOOL CALLBACK Notification(NMHDR *nHdr);

public:
    /* Event Handlers */
    std::function<void(NMTRBTHUMBPOSCHANGING *pc)> OnSlide;

};