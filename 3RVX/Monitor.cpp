#include "Monitor.h"

HMONITOR Monitor::Default() {
    /* The Default or 'Main' monitor is at (0, 0). */
    const POINT p = { 0, 0 };
    HMONITOR monitor = MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
    return monitor;
}

const int Monitor::Width(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.right - mDims.left;
}

const int Monitor::Height(HMONITOR monitor) {
    MONITORINFO mInfo = Info(monitor);
    RECT mDims = mInfo.rcMonitor;
    return mDims.bottom - mDims.top;
}

RECT Monitor::Rect(HMONITOR monitor) {
    return Info(monitor).rcMonitor;
}

MONITORINFO Monitor::Info(HMONITOR monitor) {
    MONITORINFO mInfo = {};
    mInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &mInfo);
    return mInfo;
}

std::list<DISPLAY_DEVICE> Monitor::ListAllDevices() {
    std::list<DISPLAY_DEVICE> devs;
    DISPLAY_DEVICE dev = {};
    dev.cb = sizeof(DISPLAY_DEVICE);
    for (int i = 0; EnumDisplayDevices(NULL, i, &dev, NULL) != 0; ++i) {
        if ((dev.StateFlags & DISPLAY_DEVICE_ACTIVE) == DISPLAY_DEVICE_ACTIVE) {
            devs.push_back(dev);
        }
    }
    return devs;
}