#pragma once

#include "..\OSD\OSD.h"
#include "SliderWnd.h"

class CoreAudio;
class SliderKnob;

class VolumeSlider : public OSD {
public:
    VolumeSlider(CoreAudio &volCtrl);

    void Hide();
    bool Visible();

    void MeterLevels(float level);

private:
    CoreAudio &_volCtrl;
    SliderWnd _sWnd;
    SliderKnob *_knob;
    bool _visible;
};
