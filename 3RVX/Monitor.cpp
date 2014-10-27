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
    MONITORINFO mInfo;
    mInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &mInfo);
    return mInfo;
}