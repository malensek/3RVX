#include "DownloadStatus.h"

#include "../../3RVX/Logger.h"

STDMETHODIMP DownloadStatus::OnStartBinding(DWORD dwReserved, IBinding * pib) {
    return S_OK;
}

STDMETHODIMP DownloadStatus::OnStopBinding(HRESULT hresult, LPCWSTR szError) {
    return S_OK;
}

STDMETHODIMP DownloadStatus::OnProgress(
        unsigned long ulProgress,
        unsigned long ulProgressMax, 
        unsigned long ulStatusCode,
        LPCWSTR szStatusText) {

    CLOG(L"dl: %d", ulProgress);
    return S_OK;
}

