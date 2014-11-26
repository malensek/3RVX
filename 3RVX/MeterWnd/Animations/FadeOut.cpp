#include "FadeOut.h"

bool FadeOut::Animate(MeterWnd &meterWnd) {
    byte current = meterWnd.Transparency();
    int newTrans = current - 5;
    if (newTrans < 0) {
        newTrans = 0;
        return true;
    }

    meterWnd.Transparency(newTrans);
    return false;
}

void FadeOut::Reset(MeterWnd &meterWnd) {
    meterWnd.Transparency(255);
}