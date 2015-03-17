#pragma once

#include "SliderWnd.h"

class CoreAudio;
class Settings;
class SliderKnob;

class VolumeSlider : public SliderWnd {
public:
    VolumeSlider(CoreAudio &volumeCtrl);

    virtual void Show();
    bool Visible();
    void MeterLevels(float level);

protected:
    void SliderChanged();
    void ScrollUp();
    void ScrollDown();
    void KeyPress(unsigned int vk);

    void IncreaseVolume();
    void DecreaseVolume();
    void Mute();

private:
    SliderKnob *_knob;
    float _level;
    CoreAudio &_volumeCtrl;
};
