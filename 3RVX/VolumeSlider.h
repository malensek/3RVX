#pragma once

#include "OSD\OSD.h"
#include "SliderWnd.h"

class VolumeSlider : public OSD {
public:
    VolumeSlider(HINSTANCE hInstance, Settings &settings);

    void Hide();

private:
    SliderWnd _sWnd;

};
