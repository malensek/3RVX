#include "FadeOut.h"

bool FadeOut::Animate() {
    byte current = _meterWnd.Transparency();
    int newTrans = current - 3;
    if (newTrans < 0) {
        newTrans = 0;
        return true;
    }

    _meterWnd.Transparency(newTrans);
    return false;
}

void FadeOut::Reset() {
    _meterWnd.Transparency(255);
}