#include <Windows.h>
#include <list>
#include <string>
#include <unordered_map>

class Monitor {
public:
    static HMONITOR Primary();
    static MONITORINFO Info(HMONITOR monitor);
    static const int Width(HMONITOR monitor);
    static const int Height(HMONITOR monitor);
    static RECT Rect(HMONITOR monitor);
    static std::list<DISPLAY_DEVICE> ListAllDevices();

    static void UpdateMonitorMap();
    static std::unordered_map<std::wstring, HMONITOR> MonitorMap();

private:
    static BOOL CALLBACK MonitorEnumProc(
        HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
};