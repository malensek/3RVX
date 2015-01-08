#include <Windows.h>
#include <list>

class Monitor {
public:
    static HMONITOR Primary();
    static MONITORINFO Info(HMONITOR monitor);
    static const int Width(HMONITOR monitor);
    static const int Height(HMONITOR monitor);
    static RECT Rect(HMONITOR monitor);
    static std::list<DISPLAY_DEVICE> ListAllDevices();
};