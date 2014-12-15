#pragma once

#include "OSD\OSD.h"
#include "SliderWnd.h"

class VolumeSlider : public OSD {
public:
    VolumeSlider(Settings &settings);

    void Hide();

private:
    SliderWnd _sWnd;

};
