#include "VolumeSlider.h"

#include "..\Error.h"
#include "..\Skin.h"
#include "SliderKnob.h"

VolumeSlider::VolumeSlider() :
OSD(L"3RVX-VolumeSlider"),
_sWnd(L"3RVX-VolumeSliderWnd", L"3RVX Volume Slider") {

    Skin *skin = _settings.CurrentSkin();

    Gdiplus::Bitmap *bg = skin->ControllerBgImg("volume");
    _sWnd.BackgroundImage(bg);

    std::list<Meter*> meters = skin->Meters("volume");
    for each (Meter *m in meters) {
        _sWnd.AddMeter(m);
    }

    _knob = new SliderKnob(L"skins/pix/control/knob.png", 52, 22, 191, 21);
    _sWnd.Knob(_knob);

    _sWnd.Update();
    _sWnd.Show();
}

void VolumeSlider::Hide() {
    _sWnd.Hide(false);
}