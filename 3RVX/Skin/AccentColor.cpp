#include "AccentColor.h"

#include <dwmapi.h>
#include <VersionHelpers.h>

#include "../Logger.h"

AccentColor::AccentColor() {
    if (IsWindows7OrGreater()) {
    DWORD val;
    BOOL opaque;
    DwmGetColorizationColor(&val, &opaque);

    CLOG(L"=> %x", val);


    }
    DWORD dw = GetSysColor(COLOR_ACTIVECAPTION);
    CLOG(L"-=-> %x", dw);
}

UINT32 AccentColor::Color() const {
    return 0;
}

bool AccentColor::HasColor() const {
    return _hasColor;
}

/* WARNING: This method uses an undocumented API. May crash in the future. */
INT64 AccentColor::ColorizationParamColor() {
    struct DwmColorizationParams {
        UINT ColorizationColor;
        UINT ColorizationAfterglow;
        UINT ColorizationColorBalance;
        UINT ColorizationAfterglowBalance;
        UINT ColorizationBlurBalance;
        UINT ColorizationGlassReflectionIntensity;
        UINT ColorizationOpaqueBlend;
    };

    HMODULE dwm = LoadLibrary(L"dwmapi.dll");
    if (dwm == NULL) {
        CLOG(L"Could not load DWM library");
        return -1;
    }

    HRESULT(WINAPI *DwmGetColorizationParameters)
        (DwmColorizationParams *colorParams);
    const int ColorizationParamOrd = 127;

    *(FARPROC *) &DwmGetColorizationParameters
        = GetProcAddress(dwm, (LPCSTR) ColorizationParamOrd);
    if (DwmGetColorizationParameters == nullptr) {
        CLOG(L"Could not retrieve DwmGetColorizationParameters address");
        return -1;
    }

    DwmColorizationParams params = { 0 };
    HRESULT hr = DwmGetColorizationParameters(&params);
    if (FAILED(hr)) {
        CLOG(L"Could not retrieve DWM colorization parameters");
        return -1;
    }

    return (INT64) params.ColorizationColor;
}
