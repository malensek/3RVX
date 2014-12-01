#include "OSD.h"

OSD::OSD(HINSTANCE hInstance, std::wstring className, Settings &settings) :
_settings(settings) {

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = &OSD::StaticWndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className.c_str();
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);
    /* throw exception if failed? */

    _hWnd = CreateWindowEx(
        NULL, className.c_str(), className.c_str(),
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, hInstance, this);

    _masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");
}

void OSD::Hide() {
    //_animOut.Reset();
    //SetTimer(_hWnd, TIMER_ANIMOUT, 15, NULL);
}

LRESULT CALLBACK
OSD::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    OSD* osd;

    if (message == WM_CREATE) {
        osd = (OSD*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) osd);
    } else {
        osd = (OSD*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!osd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return osd->WndProc(hWnd, message, wParam, lParam);
}

LRESULT
OSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}