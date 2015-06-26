#include "DisplayManager.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include "Logger.h"

static std::unordered_map<std::wstring, Monitor> monitorMap;

std::unordered_map<std::wstring, Monitor> &DisplayManager::MonitorMap() {
    return monitorMap;
}

void DisplayManager::UpdateMonitorMap() {
    monitorMap.clear();
    EnumDisplayMonitors(NULL, NULL, &MonitorEnumProc, NULL);
}

Monitor DisplayManager::Primary() {
    /* The Primary or 'Main' monitor is at (0, 0). */
    const POINT p = { 0, 0 };
    HMONITOR monitor = MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mInfo = Info(monitor);
    return Monitor(L"Primary", mInfo.rcMonitor);
}

Monitor DisplayManager::MonitorAtPoint(POINT &pt, bool workingArea) {
    Monitor m;
    HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
    if (monitor != NULL) {
        MONITORINFO mInfo = Info(monitor);
        if (workingArea) {
            return Monitor(L"Monitor@Point", mInfo.rcWork);
        } else {
            return Monitor(L"Monitor@Point", mInfo.rcMonitor);
        }
    }

    return m;
}

const int DisplayManager::Width(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.right - mDims.left;
}

const int DisplayManager::Height(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.bottom - mDims.top;
}

RECT DisplayManager::Rect(HMONITOR monitor) {
    return Info(monitor).rcMonitor;
}

bool DisplayManager::IsFullscreen(HWND hWnd) {
    /*
    HWND fg = GetForegroundWindow();
    if (fg == NULL) {
        return false;
    }

    HWND shell = GetShellWindow();
    if (fg == shell) {
        return false;
    }

    HWND dt = GetDesktopWindow();
    if (fg == dt) {
        return false;
    }

    RECT wndRect = { 0 };
    GetWindowRect(fg, &wndRect);
    // check window dimensions vs screen dimensions

    */
    IDirect3D9Ex *pDirect3DEx;
    LPDIRECT3DDEVICE9EX pDeviceEx;
    DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    Direct3DCreate9Ex(D3D_SDK_VERSION, &pDirect3DEx);
    HRESULT hr = pDirect3DEx->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        behaviorFlags,
        &d3dpp,
        NULL,
        &pDeviceEx);

    hr = pDeviceEx->CheckDeviceState(NULL);
    if (hr == S_PRESENT_OCCLUDED) {
        CLOG(L"FULLSCREEN");
    }


    CLOG(L"hresult: %d", hr);

}

MONITORINFO DisplayManager::Info(HMONITOR monitor) {
    MONITORINFO mInfo = {};
    mInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &mInfo);
    return mInfo;
}

std::list<DISPLAY_DEVICE> DisplayManager::ListAllDevices() {
    std::list<DISPLAY_DEVICE> devs;
    DISPLAY_DEVICE dev = {};
    dev.cb = sizeof(DISPLAY_DEVICE);
    for (int i = 0; EnumDisplayDevices(NULL, i, &dev, NULL) != 0; ++i) {
        if (dev.StateFlags & DISPLAY_DEVICE_ACTIVE) {
            devs.push_back(dev);
        }
    }
    return devs;
}

BOOL CALLBACK DisplayManager::MonitorEnumProc(
    HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {

    MONITORINFOEX mInfo = {};
    mInfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &mInfo);

    std::wstring monitorName = std::wstring(mInfo.szDevice);
    Monitor mon(monitorName, mInfo.rcMonitor);
    monitorMap[monitorName] = mon;

    return TRUE;
}