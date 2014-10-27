#include "VolumeOSD.h"

#include <string>

#include "Monitor.h"
#include "Skin.h"

void VolumeOSD::LoadSkin(std::wstring skinName) {
    Skin skin(skinName);

    Gdiplus::Bitmap *bg = skin.OSDBgImg("volume");
    _mWnd.SetBackgroundImage(bg);

    std::list<Meter*> meters = skin.Meters("volume");
    for each (Meter *m in meters) {
        _mWnd.AddMeter(m);
    }

    _mWnd.Update();
    HMONITOR monitor = Monitor::Default();
    const int mWidth = Monitor::Width(monitor);
    const int mHeight = Monitor::Height(monitor);
    _mWnd.X(mWidth / 2 - _mWnd.Width() / 2);
    _mWnd.Y(mHeight - _mWnd.Height() - 140);
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
    _mWnd.Show();
}