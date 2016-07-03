#include "AccentColor.h"

#include <dwmapi.h>
#include <VersionHelpers.h>

AccentColor::AccentColor() {
    Refresh();
}

UINT32 AccentColor::Color() const {
    return _color;
}

void AccentColor::Color(UINT32 color) {
    _color = color;
    _override = true;
}

void AccentColor::UseSystemColor(bool enable) {
    _override = !enable;
    Refresh();
}

void AccentColor::Refresh() {
    if (_override == true) {
        return;
    }

    if (IsWindows7OrGreater()) {
        if (_useUndocumented == true) {
            INT64 color = ColorizationParamColor();
            if (color >= 0) {
                _color = (UINT32) color & 0x00FFFFFF;
                return;
            }
            /* If an error occurs (-1), fall through to the next technique. */
        }

        DWORD color;
        BOOL opaque;
        HRESULT hr = DwmGetColorizationColor(&color, &opaque);
        if (SUCCEEDED(hr)) {
            _color = color & 0x00FFFFFF;
            return;
        }
    }

    /* Our last hope if both methods above failed: */
    DWORD caption = GetSysColor(COLOR_ACTIVECAPTION);
    _color = caption;
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

    if (_dwmLib == NULL) {
        /* May be the first time executing this method */
        _dwmLib = LoadLibrary(L"dwmapi.dll");
        if (_dwmLib == NULL) {
            CLOG(L"Could not load DWM library");
            return -1;
        }
    }

    HRESULT(WINAPI *DwmGetColorizationParameters)
        (DwmColorizationParams *colorParams);
    const int ColorizationParamOrd = 127;

    *(FARPROC *) &DwmGetColorizationParameters
        = GetProcAddress(_dwmLib, (LPCSTR) ColorizationParamOrd);
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
