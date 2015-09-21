// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

class Monitor;

class BrightnessController {
public:
    BrightnessController(HMONITOR monitor);
    BrightnessController(Monitor &monitor);

    float Brightness();
    void Brightness(float level);

private:
};