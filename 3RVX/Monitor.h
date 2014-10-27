#include <Windows.h>

class Monitor {
public:
    static HMONITOR Default();
    static MONITORINFO Info(HMONITOR monitor);
    static const int Width(HMONITOR monitor);
    static const int Height(HMONITOR monitor);
    static RECT Rect(HMONITOR monitor);
};