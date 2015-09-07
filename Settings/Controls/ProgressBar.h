#pragma once

#include "Control.h"
#include "StatusCallback.h"
#include <CommCtrl.h>

class ProgressBar : public Control, public StatusCallback {
public:
    ProgressBar(int id, Dialog &parent, bool translate = true) :
    Control(id, parent, translate) {

    }

    void Range(int min, int max) {
        SendMessage(_hWnd, PBM_SETRANGE32, min, max);
    }

    void Position(int pos) {
        SendMessage(_hWnd, PBM_SETPOS, pos, 0);
    }

    void Marquee(bool enabled, int refresh = 30) {
        SendMessage(_hWnd, PBM_SETMARQUEE, enabled, refresh);
    }

protected:
    virtual IFACEMETHODIMP OnProgress(
        unsigned long ulProgress,
        unsigned long ulProgressMax,
        unsigned long ulStatusCode,
        LPCWSTR szStatusText
    );
};