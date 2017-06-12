#include "WMIBrightnessController.h"

#include <Windows.h>
#include <comutil.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

#include "../../COMUtil.h"
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

    /* Connect to WMI namespace */
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

    /* Set up proxy auth to impersonate the client */
    hr = CoSetProxyBlanket(
       pSvc,
       RPC_C_AUTHN_WINNT,
       RPC_C_AUTHZ_NONE,
       NULL,
       RPC_C_AUTHN_LEVEL_PKT,
       RPC_C_IMP_LEVEL_IMPERSONATE,
       NULL,
       EOAC_NONE);

    if (FAILED(hr)) {
        CLOG(L"Failed to set proxy blanket: 0x%x", hr);
        pSvc->Release();
        pLoc->Release();
        return;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hr = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM WmiMonitorBrightness"),
        WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) {
            break;
        }

        VARIANT vtProp;
        BSTR x;
        pclsObj->GetObjectText(0, &x);
        CLOG(L"x-> %s", x);
        VariantClear(&vtProp);
    }

}