#include "VolumeSlider.h"

#include "Skin.h"

VolumeSlider::VolumeSlider(HINSTANCE hInstance, Settings &settings) :
OSD(hInstance, L"3RVX-VolumeSlider", settings),
_sWnd(hInstance, L"3RVX-VolumeSliderWnd", L"3RVX Volume Slider") {

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