// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "BrightnessController.h"

#include "../Monitor.h"
#include "../Logger.h"

BrightnessController::BrightnessController(HMONITOR monitor) {
    BOOL result;
    DWORD numPhysicalMonitors = 0;
    result = GetNumberOfPhysicalMonitorsFromHMONITOR(
        monitor, &numPhysicalMonitors);

    if (result == FALSE || numPhysicalMonitors <= 0) {
        CLOG(L"Could not get physical monitors");
        return;
    }

    CLOG(L"Number of physical monitors detected: %d", numPhysicalMonitors);
    PHYSICAL_MONITOR *monitors = new PHYSICAL_MONITOR[numPhysicalMonitors];
    result = GetPhysicalMonitorsFromHMONITOR(
        monitor, numPhysicalMonitors, monitors);
    for (unsigned int i = 0; i < numPhysicalMonitors; ++i) {
        CLOG(L"Monitor: %s", monitors[i].szPhysicalMonitorDescription);
        bool supportsAPI = SupportsBrightnessAPI(monitors[i]);
        QCLOG(L"Supports *MonitorBrightness APIs: %s",
            supportsAPI ? L"YES" : L"NO");
        if (supportsAPI) {
            /* For now, we use the first compatible monitor found. */
            _monitorHandle = monitors[i].hPhysicalMonitor;
            break;
        }
    }
    delete[] monitors;

//    DWORD min = 0, cur = 0, max = 0;
//    result = GetMonitorBrightness(pm[0].hPhysicalMonitor, &min, &cur, &max);
//    CLOG(L"result: %d", result);
//    if (result == 0) {
//        Logger::LogLastError();
//    }
//    CLOG(L"Got brightness: %d, %d, %d", min, cur, max);
 
}

BrightnessController::BrightnessController(Monitor &monitor) :
BrightnessController(monitor.Handle()) {

}

float BrightnessController::Brightness() {
    return 0;
}

void BrightnessController::Brightness(float level) {

}

bool BrightnessController::SupportsBrightnessAPI(PHYSICAL_MONITOR &pm) {
    DWORD caps, color;
    GetMonitorCapabilities(pm.hPhysicalMonitor, &caps, &color);
    return ((caps & MC_CAPS_BRIGHTNESS) == MC_CAPS_BRIGHTNESS);
}
