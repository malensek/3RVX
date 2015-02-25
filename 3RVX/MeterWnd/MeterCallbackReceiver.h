#pragma once

class MeterCallbackReceiver {
public:
    virtual void MeterChangeCallback(int units) = 0;
};