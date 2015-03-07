#include "VolumeSlider.h"

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Error.h"
#include "..\Settings.h"
#include "..\Skin.h"
#include "SliderKnob.h"

VolumeSlider::VolumeSlider(CoreAudio &volumeCtrl) :
SliderWnd(L"3RVX-VolumeSlider", L"3RVX Volume Slider"),
_volumeCtrl(volumeCtrl) {

    Skin *skin = Settings::Instance()->CurrentSkin();

    /* TODO NULL check */
    BackgroundImage(skin->volumeSliderBackground);

    if (skin->volumeSliderMask != NULL) {
        GlassMask(skin->volumeSliderMask);
    }

    /* TODO NULL check */
    _knob = skin->volumeSliderKnob;
    _vertical = _knob->Vertical();

    for (Meter *m : skin->volumeSliderMeters) {
        AddMeter(m);
    }

    Knob(_knob);
}

void VolumeSlider::SliderChanged() {
    _volumeCtrl.Volume(_knob->Value());
}

void VolumeSlider::MeterLevels(float level) {
    if (Visible() && _dragging == false) {
        MeterWnd::MeterLevels(level);
        Update();
    }
    _level = level;
}

void VolumeSlider::Show() {
    MeterWnd::MeterLevels(_level);
    Update();
    SliderWnd::Show();
    SetActiveWindow(_hWnd);
    SetForegroundWindow(_hWnd);
}

bool VolumeSlider::Visible() {
    return _visible;
}