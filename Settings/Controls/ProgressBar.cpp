#include "ProgressBar.h"

IFACEMETHODIMP ProgressBar::OnProgress(
        unsigned long ulProgress, unsigned long ulProgressMax,
        unsigned long ulStatusCode, LPCWSTR szStatusText) {

    return StatusCallback::OnProgress(
        ulProgress,
        ulProgressMax,
        ulStatusCode,
        szStatusText);
}