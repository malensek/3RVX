#pragma once

#include "Tab.h"

#include <CommCtrl.h>

class Display : public Tab {
public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    bool OnAnimationChanged();
    bool OnAnimationSpin(NMUPDOWN *ud);
    bool OnCustomCheckChanged();
    bool OnPositionChanged();

private:
    /* Controls: */
    Checkbox _onTop;
    Checkbox _hideFullscreen;

    ComboBox _position;
    Label _customX;
    EditBox _positionX;
    Label _customY;
    EditBox _positionY;
    Checkbox _customDistance;
    Spinner _edgeSpinner;
    Label _displayDevLabel;
    ComboBox _displayDevice;

    ComboBox _animation;
    Label _hideDelayLabel;
    Spinner _hideDelay;
    Label _hideSpeedLabel;
    Spinner _hideSpeed;

private:
    /* Strings: */
    std::wstring primaryMonitorStr = L"Primary Monitor";
    std::wstring allMonitorStr = L"All Monitors";
    std::wstring customPositionStr = L"Custom";
    std::wstring noAnimStr = L"None";

private:
    /* Constants: */
    const int MIN_EDGE = -65535;
    const int MAX_EDGE = 65535;
    const int MIN_MS = USER_TIMER_MINIMUM;
    const int MAX_MS = 60000;
    const int ANIM_SPIN_INCREMENT = 100;
};