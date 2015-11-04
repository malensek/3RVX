// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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

    /// <summary>Sets the range (min, max) for the slider control.</summary>
    /// <param name="lo">Lower bound for the slider.</param>
    /// <param name="hi">Upper bound for the slider.</param>
    void Range(int lo, int hi);

    virtual BOOL CALLBACK Notification(NMHDR *nHdr);

private:
    HWND _buddyWnd;

public:
    /* Event Handlers */
    std::function<void(NMTRBTHUMBPOSCHANGING *pc)> OnSlide;

};