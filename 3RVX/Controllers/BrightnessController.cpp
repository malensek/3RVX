// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "BrightnessController.h"

#pragma comment(lib, "Dxva2.lib") 

#include <HighLevelMonitorConfigurationAPI.h>

#include "../Monitor.h"
#include "../Logger.h"

BrightnessController::BrightnessController(HMONITOR monitor) {
    BOOL result;
    DWORD physicalMonitors = 0;
    result = GetNumberOfPhysicalMonitorsFromHMONITOR(
        monitor, &physicalMonitors);

    if (result == FALSE || physicalMonitors <= 0) {

    }
    CLOG(L"Number of physical monitors detected: %d", physicalMonitors);

    PHYSICAL_MONITOR *pm = new PHYSICAL_MONITOR[physicalMonitors];
    result = GetPhysicalMonitorsFromHMONITOR(monitor, physicalMonitors, pm);
    //CLOG(L"phys handle: %d", pm[0].hPhysicalMonitor);
    CLOG(L"monitor: %s", pm[0].szPhysicalMonitorDescription);

    DWORD cap;
    DWORD color;
    GetMonitorCapabilities(pm[0].hPhysicalMonitor, &cap, &color);

    DWORD min = 0, cur = 0, max = 0;
    result = GetMonitorBrightness(pm[0].hPhysicalMonitor, &min, &cur, &max);
    CLOG(L"result: %d", result);
    if (result == 0) {
        DWORD z = GetLastError();
        CLOG(L"err: %d", z);
    }
    CLOG(L"Got brightness: %d, %d, %d", min, cur, max);
 
}

BrightnessController::BrightnessController(Monitor &monitor) :
BrightnessController(monitor.Handle()) {

}

float BrightnessController::Brightness() {
    return 0;
}

void BrightnessController::Brightness(float level) {

}