#pragma once

#include <Windows.h>
#include "../Logger.h"
#include <math.h>
#include <algorithm>

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

class SystemColors {

    struct DWMColorizationParams {
        UINT ColorizationColor;
        UINT ColorizationAfterglow;
        UINT ColorizationColorBalance;
        UINT ColorizationAfterglowBalance;
        UINT ColorizationBlurBalance;
        UINT ColorizationGlassReflectionIntensity;
        UINT ColorizationOpaqueBlend;
    };

public:
    SystemColors() {
        HMODULE dwm = LoadLibrary(L"dwmapi.dll");
        HRESULT(WINAPI *DwmGetColorizationParameters) (DWMColorizationParams *colorparam);
        *(FARPROC *) &DwmGetColorizationParameters = GetProcAddress(dwm, (LPCSTR) 127);
        DWMColorizationParams params;
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

        COLORREF base = RGB(217, 217, 217);

        DWORD val;
        BOOL opaque;
        DwmGetColorizationColor(&val, &opaque);

        CLOG(L"=> %x", val);

    }


};