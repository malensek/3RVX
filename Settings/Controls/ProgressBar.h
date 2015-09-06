#pragma once

#include "Control.h"
#include "StatusCallback.h"

class ProgressBar : public Control, public StatusCallback {
public:
    ProgressBar(int id, Dialog &parent, bool translate = true) :
    Control(id, parent, translate) {

    }

protected:
    virtual IFACEMETHODIMP OnProgress(
        unsigned long ulProgress,
        unsigned long ulProgressMax,
        unsigned long ulStatusCode,
        LPCWSTR szStatusText
    );
};