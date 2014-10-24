#include "VolumeOSD.h"

#include <string>

#include "Skin.h"

void VolumeOSD::LoadSkin(std::wstring skinName) {
    Skin skin(skinName);

    Gdiplus::Bitmap *bg = skin.OSDBgImg("volume");
    _mWnd.SetBackgroundImage(bg);

    std::list<Meter*> meters = skin.Meters("volume");
    for each (Meter *m in meters) {
        _mWnd.AddMeter(m);
    }
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
    _mWnd.Show();
}