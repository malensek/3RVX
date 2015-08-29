
#pragma once

#include <Windows.h>
#include <urlmon.h>

class DownloadStatus : public IBindStatusCallback {
public:
    /* IBindStatusCallback */
    IFACEMETHODIMP GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP GetPriority(LONG *pnPriority) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP OnLowResource(DWORD reserved) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP OnObjectAvailable(REFIID riid, IUnknown *punk) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP OnDataAvailable(DWORD grfBSCF, DWORD dwSize,
            FORMATETC *pformatetc, STGMEDIUM *pstgmed) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP OnProgress(
        unsigned long ulProgress,
        unsigned long ulProgressMax,
        unsigned long ulStatusCode,
        LPCWSTR szStatusText
    );

    IFACEMETHODIMP OnStartBinding(DWORD dwReserved, IBinding *pib);
    IFACEMETHODIMP OnStopBinding(HRESULT hresult, LPCWSTR szError);

    /* IUnknown */
    IFACEMETHODIMP_(ULONG) AddRef() {
        return 0;
    }

    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppvObject) {
        return E_NOTIMPL;
    }

    IFACEMETHODIMP_(ULONG) Release() {
        return 0;
    }
};