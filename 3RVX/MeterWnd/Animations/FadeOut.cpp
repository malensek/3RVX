// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "FadeOut.h"

#include "..\MeterWnd.h"
#include "..\..\Logger.h"

FadeOut::FadeOut(int speed) :
Animation(speed) {

    /* Determine the best step/interval combination that gets us a nice linear
     * animation without being excessively early or late (based on the speed) */
    int bestError = 255;
    int bestInterval = 10;
    for (int i = 10; i <= 20; ++i) {
        int si = _speed / i;
        int error = 255 - 255 / si * si;
        if (error < bestError) {
            bestError = error;
            bestInterval = i;
        }
    }

    _interval = bestInterval;
    _step = 255 / (_speed / _interval);
}


bool FadeOut::Animate(MeterWnd *meterWnd) {
    byte current = meterWnd->Transparency();
    int newTrans = current - _step;
    if (newTrans < 0) {
        newTrans = 0;
        return true;
    }

    meterWnd->Transparency(newTrans);
    return false;
}

void FadeOut::Reset(MeterWnd *meterWnd) {
    meterWnd->Transparency(255);
}

int FadeOut::UpdateInterval() {
    return _interval;
}
