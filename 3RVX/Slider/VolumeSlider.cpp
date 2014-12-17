#include "VolumeSlider.h"

#include "..\Error.h"
#include "..\Skin.h"
#include "SliderKnob.h"

VolumeSlider::VolumeSlider(CoreAudio &volCtrl) :
OSD(L"3RVX-VolumeSlider"),
_sWnd(L"3RVX-VolumeSliderWnd", L"3RVX Volume Slider"),
_volCtrl(volCtrl) {

    Skin *skin = _settings.CurrentSkin();

    Gdiplus::Bitmap *bg = skin->ControllerBgImg("volume");
    _sWnd.BackgroundImage(bg);

    std::list<Meter*> meters = skin->Meters("volume");
    for (Meter *m : meters) {
        _sWnd.AddMeter(m);
    }

    _knob = new SliderKnob(L"skins/pix/control/knob.png", 52, 22, 191, 21);
    _sWnd.Knob(_knob);

    _knob->Value(0.5f);

    _sWnd.Update();
    _sWnd.Show();
}

void VolumeSlider::Hide() {
    _visible = false;
    _sWnd.Hide(false);
}

bool VolumeSlider::Visible() {
    return _visible;
}