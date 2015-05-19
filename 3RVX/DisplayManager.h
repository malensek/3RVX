#pragma once
#include <Windows.h>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "Monitor.h"

class DisplayManager {
public:
    static std::unordered_map<std::wstring, Monitor> &MonitorMap();
    static void UpdateMonitorMap();
    static Monitor Primary();
    static Monitor MonitorAtPoint(POINT &pt, bool workingArea = false);
    static std::list<DISPLAY_DEVICE> ListAllDevices();

private:
    static MONITORINFO Info(HMONITOR monitor);
    static const int Width(HMONITOR monitor);
    static const int Height(HMONITOR monitor);
    static RECT Rect(HMONITOR monitor);
    static bool IsFullscreen();

    static BOOL CALLBACK MonitorEnumProc(
        HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
};