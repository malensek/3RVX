#include "VolumeSlider.h"

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Error.h"
#include "..\Settings.h"
#include "..\Skin.h"
#include "SliderKnob.h"

VolumeSlider::VolumeSlider(CoreAudio &volumeCtrl) :
SliderWnd(L"3RVX-VolumeSlider", L"3RVX Volume Slider", 0),
_settings(*Settings::Instance()),
_volumeCtrl(volumeCtrl) {

    Skin *skin = _settings.CurrentSkin();

    Gdiplus::Bitmap *bg = skin->ControllerBgImg("volume");
    BackgroundImage(bg);

    std::list<Meter*> meters = skin->Meters("volume");
    for (Meter *m : meters) {
        AddMeter(m);
    }

    _knob = skin->Knob("volume");
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

void VolumeSlider::Hide() {
    _visible = false;
    SliderWnd::Hide(false);
}

void VolumeSlider::Show() {
    MeterWnd::MeterLevels(_level);
    Update();
    SliderWnd::Show();
}

bool VolumeSlider::Visible() {
    return _visible;
}