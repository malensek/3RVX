#pragma once

#include "..\OSD\OSD.h"
#include "SliderWnd.h"

class SliderKnob;

class VolumeSlider : public OSD {
public:
    VolumeSlider();

    void Hide();

private:
    SliderWnd _sWnd;
    SliderKnob *_knob;
};
