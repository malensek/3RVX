#pragma once

#include "Control.h"
#include "StatusCallback.h"
#include <CommCtrl.h>

class ProgressBar : public Control, public StatusCallback {
public:
    ProgressBar(int id, DialogBase &parent) :
    Control(id, parent, false) {

    }

    void Range(long min, long max);
    void Position(long pos);
    void Marquee(bool enabled, int refresh = 30);

protected:
    virtual IFACEMETHODIMP OnProgress(
        unsigned long ulProgress,
        unsigned long ulProgressMax,
        unsigned long ulStatusCode,
        LPCWSTR szStatusText
    );

private:
    unsigned long _progressMax;
};