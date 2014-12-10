#include "VolumeSlider.h"

#include "Skin.h"

VolumeSlider::VolumeSlider(HINSTANCE hInstance, Settings &settings) :
OSD(hInstance, L"3RVX-VolumeSlider", settings),
_mWnd(hInstance, L"3RVX-VolumeSliderWnd", L"3RVX Volume Slider") {

    std::wstring skinXML = settings.SkinXML();
    Skin skin(skinXML);

    long styles = GetWindowLongPtr(_mWnd.HWnd(), GWL_EXSTYLE);
    styles &= ~(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);
    SetWindowLongPtr(_mWnd.HWnd(), GWL_EXSTYLE, styles);

    Gdiplus::Bitmap *bg = skin.ControllerBgImg("volume");
    _mWnd.BackgroundImage(bg);
    _mWnd.Update();
    _mWnd.Show();

}

void VolumeSlider::Hide() {
    _mWnd.Hide(false);
}