// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#pragma comment(lib, "Dxva2.lib") 

#include <Windows.h>
#include <HighLevelMonitorConfigurationAPI.h>

class Monitor;

class DDCBrightnessController {
public:
    DDCBrightnessController(HMONITOR monitor);
    DDCBrightnessController(Monitor &monitor);

    float Brightness();
    void Brightness(float level);

private:
    HANDLE _monitorHandle;
    DWORD _minBrightness;
    DWORD _maxBrightness;
    bool _useBrightnessAPI;

    bool SupportsBrightnessAPI(PHYSICAL_MONITOR &pm);
};