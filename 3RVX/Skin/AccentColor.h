#pragma once

#include <Windows.h>

class AccentColor {

public:
    AccentColor();

    UINT ColorizationColor() const;

private:
    struct DwmColorizationParams {
        UINT ColorizationColor;
        UINT ColorizationAfterglow;
        UINT ColorizationColorBalance;
        UINT ColorizationAfterglowBalance;
        UINT ColorizationBlurBalance;
        UINT ColorizationGlassReflectionIntensity;
        UINT ColorizationOpaqueBlend;
    };

private:
    bool _useUndocumented;

    const int ColorizationParamOrd = 127;
    HRESULT(WINAPI *DwmGetColorizationParameters)
        (DwmColorizationParams *colorParams);

};