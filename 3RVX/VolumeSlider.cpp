#include "VolumeSlider.h"

#include "Skin.h"

VolumeSlider::VolumeSlider(Settings &settings) :
OSD(L"3RVX-VolumeSlider", settings),
_sWnd(L"3RVX-VolumeSliderWnd", L"3RVX Volume Slider") {

    std::wstring skinXML = settings.SkinXML();
    Skin skin(skinXML);

    Gdiplus::Bitmap *bg = skin.ControllerBgImg("volume");
    _sWnd.BackgroundImage(bg);
    _sWnd.Update();
    _sWnd.Show();
}

void VolumeSlider::Hide() {
    _sWnd.Hide(false);
}