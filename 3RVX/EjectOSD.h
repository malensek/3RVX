#pragma once

#include <Windows.h>

#include "OSD.h"
#include "Settings.h"

class EjectOSD : public OSD {
public:
    EjectOSD(HINSTANCE hInstance, Settings &settings);
};