#include "VolumeSlider.h"

#include "../Controllers/Volume/CoreAudio.h"
#include "../Error.h"
#include "../Settings.h"
#include "../Skin/Skin.h"
#include "../Skin/SkinManager.h"
#include "SliderKnob.h"

#define SCROLL_INCREMENT 0.05f

VolumeSlider::VolumeSlider(CoreAudio &volumeCtrl) :
SliderWnd(L"3RVX-VolumeSlider", L"3RVX Volume Slider"),
_volumeCtrl(volumeCtrl) {

    Skin *skin = SkinManager::Instance()->CurrentSkin();

    /* TODO NULL check */
    BackgroundImage(skin->volumeSliderBackground);

    if (skin->volumeSliderMask != NULL) {
        EnableGlass(skin->volumeSliderMask);
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

void VolumeSlider::ScrollUp() {
    IncreaseVolume();
}

void VolumeSlider::ScrollDown() {
    DecreaseVolume();
}

void VolumeSlider::KeyPress(unsigned int vk) {
    if (_visible == false) {
        return;
    }

    switch (vk) {
    case VK_ESCAPE:
    case VK_RBUTTON:
        Hide();
        break;

    case VK_MBUTTON:
        Mute();
        break;

    case VK_UP:
        if (_vertical) {
            IncreaseVolume();
        }
        break;

    case VK_DOWN:
        if (_vertical) {
            DecreaseVolume();
        } else {
            Mute();
        }
        break;

    case VK_LEFT:
        if (_vertical == false) {
            DecreaseVolume();
        } else {
            Mute();
        }
        break;

    case VK_RIGHT:
        if (_vertical == false) {
            IncreaseVolume();
        }
        break;
    }
}

void VolumeSlider::IncreaseVolume() {
    if (_volumeCtrl.Muted()) {
        _volumeCtrl.Muted(false);
    }

    float vol = _volumeCtrl.Volume() + SCROLL_INCREMENT;
    _volumeCtrl.Volume(vol);
}

void VolumeSlider::DecreaseVolume() {
    if (_volumeCtrl.Muted()) {
        _volumeCtrl.Muted(false);
    }

    float vol = _volumeCtrl.Volume() - SCROLL_INCREMENT;
    _volumeCtrl.Volume(vol);
}

void VolumeSlider::Mute() {
    _volumeCtrl.ToggleMute();
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
    SetActiveWindow(Window::Handle());
    SetForegroundWindow(Window::Handle());
}

bool VolumeSlider::Visible() {
    return _visible;
}