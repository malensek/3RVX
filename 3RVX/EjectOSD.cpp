#include "EjectOSD.h"

EjectOSD::EjectOSD(HINSTANCE hInstance, Settings &settings) :
OSD(hInstance, L"3RVX-EjectDispatcher", settings) {

}

LRESULT
EjectOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DEVICECHANGE) {
        if (wParam == DBT_DEVICEREMOVECOMPLETE) {
            CLOG(L"Device removal notification received");
        }
    }
}