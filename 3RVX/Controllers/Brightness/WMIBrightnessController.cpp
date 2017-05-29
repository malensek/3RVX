#include "WMIBrightnessController.h"

#include <Windows.h>
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
        Logger::LogLastError();
        if (pLoc) {
            pLoc->Release();
        }
        return;
    }

}