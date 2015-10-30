#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include "Control.h"

class Slider : public Control {
public:
    Slider(int id, DialogBase &parent) :
    Control(id, parent, false) {

    }

    void Buddy(int buddyId, int position = 0);

    int Position();
    void Position(int position);
    virtual BOOL CALLBACK Notification(NMHDR *nHdr);

public:
    /* Event Handlers */
    std::function<void(NMTRBTHUMBPOSCHANGING *pc)> OnSlide;

};