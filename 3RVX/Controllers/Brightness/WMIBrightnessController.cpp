#include "WMIBrightnessController.h"

#include <Windows.h>
#include <comutil.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

#include "../../Logger.h"

WMIBrightnessController::WMIBrightnessController() {
    HRESULT hr;

    /* Create WMI locator */
    IWbemLocator *pLoc = 0;
    hr = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID *) &pLoc);

    if (FAILED(hr)) {
        CLOG(L"Failed to create WMI locator");
        return;
    }

    IWbemServices *pSvc = 0;
    hr = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\WMI"),
        NULL,
        NULL,
        NULL,
        0,
        NULL,
        NULL,
        &pSvc);

    if (FAILED(hr)) {
        CLOG(L"Failed to connect to WMI namespace");
        if (pLoc) {
            pLoc->Release();
        }
        return;
    }
}