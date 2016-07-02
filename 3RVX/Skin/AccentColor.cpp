#include "AccentColor.h"

#include <dwmapi.h>

#include "../Logger.h"

SystemColors::SystemColors() {
    HMODULE dwm = LoadLibrary(L"dwmapi.dll");
    if (dwm == NULL) {
        _useUndocumented = false;
    }

    *(FARPROC *) &DwmGetColorizationParameters
        = GetProcAddress(dwm, (LPCSTR) ColorizationParamOrd);
    if (DwmGetColorizationParameters == nullptr) {
        _useUndocumented = false;
    }

    DwmColorizationParams params = { 0 };
    HRESULT hr = DwmGetColorizationParameters(&params);
    if (FAILED(hr)) {
        CLOG(L"couldn't get dwm params");
    }

    CLOG(L"-> %x", params.ColorizationColor);
    CLOG(L"-> %x", params.ColorizationAfterglow);
    CLOG(L"-> %d", params.ColorizationColorBalance);
    CLOG(L"-> %d", params.ColorizationAfterglowBalance);
    CLOG(L"-> %x", params.ColorizationBlurBalance);
    CLOG(L"-> %x", params.ColorizationGlassReflectionIntensity);
    CLOG(L"-> %x", params.ColorizationOpaqueBlend);

    DWORD val;
    BOOL opaque;
    DwmGetColorizationColor(&val, &opaque);

    CLOG(L"=> %x", val);
}

UINT SystemColors::ColorizationColor() const {
    return 0;
}