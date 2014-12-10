#pragma once

#include "OSD\OSD.h"
#include "MeterWnd\MeterWnd.h"

class VolumeSlider : public OSD {
public:
    VolumeSlider(HINSTANCE hInstance, Settings &settings);

    void Hide();

private:
    MeterWnd _mWnd;

};
